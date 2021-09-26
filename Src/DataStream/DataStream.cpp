// This file was written by Lucas Saragosa. Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "DataStream.h"
#include <utility>

DataStream::DataStream(DataStream&& o)  {
	mMode = o.mMode;
	o.mMode = DataStreamMode::eMode_Unset;
}

DataStream& DataStream::operator=(DataStream&& o)  {
	this->mMode = o.mMode;
	o.mMode = DataStreamMode::eMode_Unset;
	return *this;
}

bool DataStreamSubStream::SetPosition(signed __int64 pos, DataStreamSeekType type) {
	unsigned __int64 final = 0;
	switch (type) {
	case DataStreamSeekType::eSeekType_Begin:
		final = pos;
		break;
	case DataStreamSeekType::eSeekType_Current:
		final = mOffset + pos;
		break;
	case DataStreamSeekType::eSeekType_End:
		final = mSize - pos;
		break;
	}
	if (final > mSize)return false;
	mOffset = final;
	return true;
}

bool DataStream::SetMode(DataStreamMode mode) {
	if (mSubStreams && mode != DataStreamMode::eMode_Read)return false;
	mMode = mode;
	return true;
}

bool DataStreamMemory::Serialize(void* buffer, unsigned __int64 bufsize) {
	if (!IsWrite())return false;
	if (!buffer && bufsize)return false;
	if (!bufsize)return true;
	int rem = mSize % mPageSize;
	char* buf = (char*)buffer;
	int size = bufsize;
	if (rem) {
		if(rem >= bufsize){
			memcpy(mPageTable[mPageTable.size() - 1] + rem, buf, bufsize);
			mSize += bufsize;
			return true;
		}
		else {
			memcpy(mPageTable[mPageTable.size() - 1] + rem, buf, mPageSize-rem);
			bufsize -= rem;
			buf += rem;
		}
	}
	int newpgs = bufsize / mPageSize;
	for (int i = 0; i < newpgs; i++) {
		char* newbuf = (char*)calloc(1, mPageSize);
		memcpy(newbuf, buf, mPageSize);
		buf += mPageSize;
		bufsize -= mPageSize;
		mPageTable.push_back(newbuf);
	}
	if (bufsize) {
		char* newbuf = (char*)calloc(1, mPageSize);
		memcpy(newbuf, buf, bufsize);
		mPageTable.push_back(newbuf);
	}
	mSize = size;
	return true;
}

DataStreamMemory::~DataStreamMemory() {
	int pages = mPageTable.size();
	for (int i = 0; i < pages; i++) {
		delete[] mPageTable[i];
	}
}

DataStreamMemory::DataStreamMemory(unsigned __int64 pageSize) : DataStream(DataStreamMode::eMode_Write) {
	this->mPageSize = pageSize;
	this->mSize = 0;
}

DataStreamMemory& DataStreamMemory::operator=(DataStreamMemory&& other) {
	mPageTable = std::move(other.mPageTable);
	mPageSize = other.mPageSize;
	mSize = other.mSize;
	other.mPageTable.clear();
	other.mSize = 0;
	other.mPageSize = 0;
	return *this;
}

DataStreamMemory::DataStreamMemory(DataStreamMemory&& other) : mPageTable(std::move(other.mPageTable)), mPageSize(other.mPageSize),
	DataStream(DataStreamMode::eMode_Write), mSize(other.mSize){
	other.mPageTable.clear();
	other.mSize = 0;
	other.mPageSize = 0;
}

bool DataStreamSubStream::Serialize(void* buf, unsigned __int64 bufsize) {
	if (!bufsize)return true;
	if ((!buf && bufsize) || (mOffset + bufsize > mSize))return false;
	if (!mpBase->SetPosition(mStreamOffset + mOffset, DataStreamSeekType::eSeekType_Begin))return false;
	if(!mpBase->Serialize(buf, bufsize))return false;
	mOffset += bufsize;
	return true;
}

DataStreamSubStream* DataStream::GetSubStream(unsigned __int64 off, unsigned __int64 size) {
	if (!IsRead())return NULL;
	if (GetPosition() + size > GetSize()) return NULL;
	return new DataStreamSubStream(this, size, off);
}

DataStreamSubStream* DataStreamSubStream::GetSubStream(unsigned __int64 off, unsigned __int64 size) {
	if (!IsRead())return NULL;
	if (this->mOffset + size > mSize) return NULL;
	return new DataStreamSubStream(this, size, off);
}

DataStreamSubStream::DataStreamSubStream(DataStream* base, unsigned __int64 size,
	unsigned __int64 off) : DataStream(DataStreamMode::eMode_Read), mpBase(base),
mSize(size), mStreamOffset(off), mOffset(0) {
	if (!base)throw "No base passed";
	if (off > base->GetSize())
		off = base->GetSize();
	mpBase->mSubStreams++;
}


DataStreamSubStream::DataStreamSubStream(DataStream* base, unsigned __int64 size) : DataStream(DataStreamMode::eMode_Read), mpBase(base),
	mSize(size), mStreamOffset(0), mOffset(0) {
	if (!base)throw "No base passed";
	mOffset = base->GetPosition();
	mpBase->mSubStreams++;
}

DataStreamSubStream& DataStreamSubStream::operator=(DataStreamSubStream&& o) {
	DataStream::operator=(std::move(o));
	this->mOffset = o.mOffset;
	this->mSize = o.mSize;
	this->mpBase = o.mpBase;
	o.mpBase = NULL;
	o.mSize = 0;
	o.mOffset = 0;
	return *this;
}

DataStreamSubStream::DataStreamSubStream(DataStreamSubStream&& o) : DataStream(std::move(o)){
	this->mOffset = o.mOffset;
	this->mSize = o.mSize;
	this->mpBase = o.mpBase;
	o.mpBase = NULL;
	o.mSize = 0;
	o.mOffset = 0;
}

DataStreamSubStream::~DataStreamSubStream() {
	mpBase->mSubStreams--;
}

bool DataStreamFile_Win::Truncate(unsigned __int64 newSize) {
	static char _TruncateBuffer[0x1000];
	if (!IsWrite())return false;
	if (newSize == mStreamSize)return true;
	if (newSize > mStreamSize) {//add bytes
		unsigned __int64 off = GetPosition();
		fseek(mHandle, mStreamSize, SEEK_SET);
		unsigned __int64 diff = newSize - mStreamSize;
		unsigned __int64 blocks = diff / 0x1000;
		for (int i = 0; i < blocks; i++)
			fwrite(_TruncateBuffer, 0x1000, 1, mHandle);
		fwrite(_TruncateBuffer, diff % 0x1000, 1, mHandle);
		fseek(mHandle, off, SEEK_SET);
	}
	else {//remove bytes, windows version can add bytes but just in case for other platforms :D
		PlatformSpecTrunc(mHandle, newSize);
		mStreamSize = newSize;
		if (mStreamOffset > mStreamSize)
			mStreamOffset = mStreamSize;
	}
	return true;
}

bool DataStreamFile_Win::SetPosition(signed __int64 pos, DataStreamSeekType type) {
	unsigned __int64 final = 0;
	switch (type) {
	case DataStreamSeekType::eSeekType_Begin:
		final = pos;
		break;
	case DataStreamSeekType::eSeekType_Current:
		final = mStreamOffset + pos;
		break;
	case DataStreamSeekType::eSeekType_End:
		final = mStreamSize - pos;
		break;
	}
	if (final > mStreamSize)return false;
	fseek(mHandle, final, SEEK_SET);
	return true;
}

bool DataStreamFile_Win::Serialize(void* buf, unsigned __int64 bufsize) {
	if (!bufsize)return true;
	if (IsInvalid() || !buf && bufsize)return false;
	fseek(mHandle, mStreamOffset, SEEK_SET);
	if (IsWrite()) {
		if (bufsize != fwrite(buf, bufsize, 1, mHandle)) return false;
		mStreamSize += bufsize;
		mStreamOffset += bufsize;
	}
	else {
		if (mStreamOffset + bufsize > mStreamSize)return false;
		fread(buf, bufsize, 1, mHandle);
		mStreamOffset += bufsize;
	}
	return true;
}

DataStreamFile_Win::DataStreamFile_Win(FileHandle handle,DataStreamMode m) : mHandle{ handle }, DataStream(m) {
	mStreamOffset = ftell(handle);
	fseek(handle, 0, SEEK_END);
	mStreamSize = ftell(handle);
	fseek(handle, mStreamOffset, SEEK_SET);
}

DataStreamFile_Win::DataStreamFile_Win(FileHandle handle) : mHandle{ handle }, DataStream() {
	mStreamOffset = ftell(handle);
	fseek(handle, 0, SEEK_END);
	mStreamSize = ftell(handle);
	fseek(handle, mStreamOffset, SEEK_SET);
}

DataStreamFile_Win& DataStreamFile_Win::operator=(DataStreamFile_Win&& o) {
	DataStream::operator=(std::move(o));
	this->mHandle = o.mHandle;
	this->mStreamOffset = o.mStreamOffset;
	this->mStreamSize = o.mStreamSize;
	o.mHandle = NULL;
	o.mStreamOffset = 0;
	o.mStreamSize = 0;
	return *this;
}

DataStreamFile_Win::DataStreamFile_Win(DataStreamFile_Win&& o) : DataStream(std::move(o))  {
	this->mHandle = o.mHandle;
	this->mStreamOffset = o.mStreamOffset;
	this->mStreamSize = o.mStreamSize;
	o.mHandle = NULL;
	o.mStreamOffset = 0;
	o.mStreamSize = 0;
}