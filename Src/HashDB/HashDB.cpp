#include "HashDB.h"

const u32 MAGIC = 0x54544c42;

HashDatabase::HashDatabase(DataStream* stream) {
	if (!stream)throw "bad stream";
	this->db_stream = stream;
	this->db_pages = NULL;
	this->flags = 0;
	this->numPages = 0;
	this->cached_page = NULL;
	this->bufferedPage = NULL;
}

HashDatabase::Page* HashDatabase::PageAt(int index) {
	if (index >= this->NumPages())return NULL;
	return this->db_pages ? this->db_pages[index] : NULL;
}

HashDatabase::Page** HashDatabase::Pages() {
	return this->db_pages;
}

void HashDatabase::FindEntry(HashDatabase::Page* page, u64 crc, String* result) {
	if (page) {
		if (!(cached_page && cached_page == page)) {
			this->db_stream->SetPosition(page->offset, DataStreamSeekType::eSeekType_Begin);
			if (bufferedPage)free(bufferedPage);
			bufferedPage = (char*)malloc(page->count * 0xC);
			this->db_stream->Serialize(bufferedPage, page->count * 0xC);
			cached_page = page;
		}
		char* cp;
		u32 c;
		for (int i = 0; i < page->count; i++) {
			cp = (bufferedPage + 0xC * i);
			if (*((u64*)(cp)) == crc) {
				cp = (bufferedPage + 0xC * i + 8);
				c = *((u32*)(cp));
				db_stream->SetPosition((page->count * 0xC) + page->offset + c, DataStreamSeekType::eSeekType_Begin);
				int len = 0;
				if (i + 1 != page->count) {
					cp = (bufferedPage + (0xC * (i+1)) + 8);
					len = *((u32*)(cp));
					len -= c;
				}
				else {
					len = page->size - (0xC * page->count) - c;
				}
				char* t = new char[len];
				db_stream->Serialize(t, len);
				result->operator=(t);
				delete[] t;
				return;
			}
		}
		return;
	}
	else {
		String s;
		for (int i = 0; i < NumPages(); i++) {
			FindEntry(PageAt(i), crc, &s);
			if (!s.empty())result->assign(s);
		}
		return;
	}
}

bool HashDatabase::Open() {
	if (!this->db_stream)return false;
	DataStream* stream = this->db_stream;
	int magic;
	stream->Serialize((char*)&magic, 4);
	if (magic != MAGIC)return false;
	stream->Serialize((char*)&flags, 1);
	stream->Serialize((char*)&this->numPages, 4);
	if (this->db_pages)return false;//cannot open another
	this->db_pages = (HashDatabase::Page**)calloc(sizeof(HashDatabase::Page*), this->numPages);
	if (!this->db_pages)return false;
	for (int i = 0; i < this->numPages; i++) {
		HashDatabase::Page* page = new Page();
		stream->Serialize((char*)&page->count, 4);
		stream->Serialize((char*)&page->flags, 1);
		int len;
		stream->Serialize((char*)&len, 4);
		char* tmp = new char[len + 1];
		tmp[len] = 0;
		stream->Serialize(tmp, len);
		page->pageName = tmp;
		stream->Serialize((char*)&page->size, 4);
		page->offset = stream->GetPosition();
		stream->SetPosition(page->size + stream->GetPosition(), DataStreamSeekType::eSeekType_Begin);
		this->db_pages[i] = page;
	}
	return true;
}

u32 HashDatabase::Flags() {
	return this->flags;
}

HashDatabase::Page* HashDatabase::FindPage(const char* n) {
	if (!this->db_pages)return NULL;
	int pages = this->NumPages();
	for (int i = 0; i < pages; i++) {
		if (!_stricmp(PageAt(i)->pageName, n)) return PageAt(i);
	}
	return NULL;
}

u32 HashDatabase::NumEntries() {
	u32 ret = 0;
	int pages = NumPages();
	for (int i = 0; i < pages; i++) {
		ret += PageAt(i)->count;
	}
	return ret;
}

u32 HashDatabase::NumPages() {
	return this->numPages;
}

HashDatabase::~HashDatabase() {
	if (this->db_stream)
		delete db_stream;
	for (int i = 0; i < NumPages(); i++) {
		delete[] PageAt(i)->pageName;
		delete PageAt(i);
	}
	if (this->db_pages)
		free(this->db_pages);
	if (this->bufferedPage)
		free(bufferedPage);
}