#include "TTArchive2.hpp"

bool TTArchive2::GetResourceInfo(const Symbol& s, ResourceInfo* i) {
	if (!i)return false;
	TTArchive2::ResourceEntry* e = _FindResource(s);
	if (!e)return false;
	i->mOffset = e->mOffset;
	i->mSize = e->mSize;
	return true;
}

void TTArchive2::Activate(DataStream* inArchiveStream) {
#define getint(dest,size)inArchiveStream->Serialize((char*)dest, size);
	if (mbActive)Deactivate();
	DataStreamContainerParams params;
	params.mpSrcStream = inArchiveStream;
	DataStreamContainer* c;
	inArchiveStream = c = new DataStreamContainer(params);
	mpInStream = inArchiveStream;
	u64 version;
	c->Read(params.mpSrcStream->GetPosition(), &version);
	inArchiveStream->Serialize((char*)&version, 4);
	if (version == 1414807858) {//TTA2: Telltale Archive 2
		version = 0;
		goto v3;
	}
	else if (version == 1414807859) {//TTA3
		version = 1;
	v3:
		u32 i;
		getint(&i, 4);
		if (i > 15)return;
	}
	else if (version != 1414807860) {//if not TTA4
		return;
	}
	else version = 2;
	u32 namesize, resources;
	getint(&namesize, 4);
	if (namesize > 0x10000000)return;
	getint(&resources, 4);
	if (resources > 0xFFFFF)return;
	mResources.reserve(resources);
	//int* buffer = version < 1 ? new int[resources] : NULL;
	for (int i = 0; i < resources; i++) {
		ResourceEntry entry;
		getint(&entry.mNameCRC, 8);
		getint(&entry.mOffset, 8);
		if (version < 1) {//if v2
			//getint(buffer + i, 4);
			inArchiveStream->SetPosition(4, DataStreamSeekType::eSeekType_Current);
		}
		getint(&entry.mSize, 4);
		inArchiveStream->SetPosition(4, DataStreamSeekType::eSeekType_Current);//skipped
		getint(&entry.mNamePageIndex, 2);
		getint(&entry.mNamePageOffset, 2);
		mResources.push_back(entry);
	}
	u32 startoff = inArchiveStream->GetPosition() + namesize;
	mpNameStream = inArchiveStream->GetSubStream(startoff - namesize, namesize);
	mpResourceStream = inArchiveStream->GetSubStream(startoff,
		inArchiveStream->GetSize() - startoff);
	//delete[] buffer;
	mbActive = true;
	mVersion = version;
}

DataStreamSubStream&& TTArchive2::GetResourceStream(TTArchive2::ResourceEntry* entry) {
	return DataStreamSubStream(mpResourceStream, entry->mSize, entry->mOffset);
}

String* TTArchive2::GetResourceName(const Symbol& name, String* result) {
	if (!mbActive)return NULL;
	TTArchive2::ResourceEntry* e = _FindResource(name);
	if (!e)return NULL;
	if (!mpNamePageCache)
		mpNamePageCache = (char*)malloc(0x10000);
	if (!mpNamePageCache)
		return NULL;
	if (mNamePageCacheIndex != e->mNamePageIndex) {
		mpNameStream->SetPosition((e->mNamePageIndex << 16) | e->mNamePageOffset,
			DataStreamSeekType::eSeekType_Begin);
		mpNameStream->Serialize(mpNamePageCache, 0x10000);
	}
	result->assign(mpNamePageCache + e->mNamePageOffset);
	return result;
}

bool TTArchive2::HasResource(const Symbol& sym) {
	return _FindResource(sym) != NULL;
}

TTArchive2::ResourceEntry* TTArchive2::_FindResource(const Symbol& sym) {
	if (mbActive) {
		int i = 0;
		for (auto it = mResources.begin(); it != mResources.end(); it++, i++) {
			if (sym.GetCRC() == it->mNameCRC)
				return mResources.data() + i;
		}
	}
	else return NULL;
}

void TTArchive2::Deactivate() {
	if (mbActive) {
		mNamePageCacheIndex = -1;
		if (mpResourceStream)
			delete mpResourceStream;
		if (mpNameStream)
			delete mpNameStream;
		if (mResources.size())
			mResources.clear();
		if (mpInStream)
			delete mpInStream;
		if (mpNamePageCache)
			free(mpNamePageCache);
		mbActive = false;
	}
}