#include "HashDB.h"

const u32 MAGIC = 0x54544c42;

HashDatabase::HashDatabase(DataStream* stream) {
	if (!stream)throw "bad stream";
	this->db_stream = stream;
	this->db_pages = NULL;
	this->flags = 0;
	this->numPages = 0;
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
		this->db_stream->SetPosition(page->offset, DataStreamSeekType::eSeekType_Begin);
		for (int i = 0; i < page->count; i++) {
			u64 c;
			db_stream->Serialize((char*)&c, 8);
			if (c == crc) {
				u32 p;
				db_stream->Serialize((char*)&p, 4);
				db_stream->SetPosition((page->count * 12) + page->offset + p, DataStreamSeekType::eSeekType_Begin);
				char ch = 0;
				while ((db_stream->Serialize(&ch, 1)) != 0) {
					result->operator+=(ch);
				}
				return;
			}
			else db_stream->Serialize((char*)&c, 4);
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
	if (this->db_stream)delete db_stream;
	for (int i = 0; i < NumPages(); i++) {
		delete[] PageAt(i)->pageName;
	}
	if (!this->db_pages)free(this->db_pages);
}