// This file was written by Lucas Saragosa. Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "DataStream.h"
#include <vector>
#include <utility>
#include "../Blowfish.h"

DataStream::DataStream(DataStream&& o)  {
	mMode = o.mMode;
	o.mMode = DataStreamMode::eMode_Unset;
}

DataStream& DataStream::operator=(DataStream&& o)  {
	this->mMode = o.mMode;
	o.mMode = DataStreamMode::eMode_Unset;
	return *this;
}

bool DataStreamLegacyEncrypted::Serialize(char* buffer, unsigned __int64 size) {
	if (!mpBase->IsRead() || !buffer && size || size + mOffset > mSize)
		return false;
	if (!size)return true;
	if (mSize) {
		int blocks = mSize / mEncryptSize;
		int sizerem = size;
		if (mOffset >= blocks * mEncryptSize) {//end padded block
			mpBase->Serialize(buffer, size);
		}
		else {
			int startblock = mOffset / mEncryptSize;
			int endblock = (mOffset + size) / mEncryptSize;
			for (int i = startblock; i <= endblock; i++) {
				if (mCurrentBlock != i) {
					SetPosition(i * mEncryptSize, DataStreamSeekType::eSeekType_Begin);
					if (blocks > i){
						mpBase->Serialize(mBuf, mEncryptSize);
						if (i % mEncryptInterval) {
							if (i % mEncryptSkip) {
								for (int x = 0; x < mEncryptSize; x++)
									mBuf[x] = ~mBuf[x];
							}
						}
						else {
							LibTelltaleTool_BlowfishDecrypt((unsigned char*)mBuf, mEncryptSize, false,
								(unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
						}
					}
					else {
						mpBase->Serialize(mBuf, mpBase->GetSize() - mpBase->GetPosition());
					}
				}
				if (i == startblock) {
					int startblockoff = mOffset % mEncryptSize;
					int rem = mEncryptSize - startblockoff;
					if (rem > size) {
						memcpy(buffer, mBuf + startblockoff, size);
						//end
					}
					else {
						memcpy(buffer, mBuf + startblockoff, rem);
						buffer += rem;
						size -= rem;
					}
				}
				else if (i == endblock){
					memcpy(buffer, mBuf, size);
					//end
				}
				else {
					memcpy(buffer, mBuf, mEncryptSize);
					buffer += mEncryptSize;
					size -= mEncryptSize;
				}
			}
			mCurrentBlock = endblock;
		}
		mOffset += sizerem;
	}
	return true;
}

DataStreamLegacyEncrypted::DataStreamLegacyEncrypted(DataStream* base, int version, unsigned int header) : 
	mHeader(header), mpBase(base),  mOffset(0), mSize(base->GetSize()-header), mCurrentBlock(-1),DataStream(DataStreamMode::eMode_Read) {
	if (version == 1)
	{
		mEncryptSize = 64;
		mEncryptInterval = 64;
		mEncryptSkip = 100;
	}
	else if (version == 2)
	{
		mEncryptSize = 128;//0x80
		mEncryptInterval = 32;//0x20
		mEncryptSkip = 80;//0x50
	}
	else // 64AFDEAA
	{
		mEncryptSize = 256;
		mEncryptInterval = 8;
		mEncryptSkip = 24;
	}
}

bool DataStreamContainer::Serialize(char* dest, unsigned __int64 size) {
	if (mStreamPosition + size > mStreamSize)return false;
	if (mParams.mbCompress) {
		//pos = 255, window = 100, size = 50
		int chunkoff = mStreamPosition % mParams.mWindowSize;//55
		int rem = mParams.mWindowSize - chunkoff;//45
		if (rem >= size) {
			memcpy(dest, mpCachedPage + chunkoff, size);
			return true;
		}
		if (rem) {
			memcpy(dest, mpCachedPage + chunkoff, rem);
			dest += rem;
			size -= rem;
		}
		mStreamPosition += rem;
		//size = 5, pos = 300, windows = 100
		//int blocks = size / mStrea
	}
	else {
		bool r = mParams.mpSrcStream->Serialize(dest, size);
		if (!r)return r;
		mStreamPosition += size;
		return true;
	}
	return true;
}

void DataStreamContainer::Read(unsigned __int64 offset, unsigned __int64* pContainerSize) {
	mParams.mpSrcStream->SetPosition(offset, DataStreamSeekType::eSeekType_Begin);
	unsigned __int32 type = 0;
	mParams.mpSrcStream->Serialize((char*)&type, 4);
	if (type == 0x5454434E) {//TTNC telltale not compressed
		mParams.mbCompress = false;
		mParams.mbEncrypt = false;
		mParams.mpSrcStream->Serialize((char*)pContainerSize, 8);//total size
		mStreamSize = *pContainerSize;
		mStreamOffset = mParams.mpSrcStream->GetPosition();
		mStreamPosition = 0;
		return;
	}
	else {
		mParams.mbCompress = true;
		switch (type) {
		case 1414808389: //TTCE
			mParams.mbEncrypt = true;
			mParams.mCompressionLibrary = Compression::Library::ZLIB;
			break;
		case 1414808410: //TTCZ
			mParams.mbEncrypt = false;
			mParams.mCompressionLibrary = Compression::Library::ZLIB;
			break;
		case 1414808421: //TTCe
		case 1414808442: //TTCz
			mParams.mbEncrypt = type == 1414808421;
			unsigned __int32 libtype;
			mParams.mpSrcStream->Serialize((char*)&libtype, 4);
			if (libtype > 1)return;
			mParams.mCompressionLibrary = (Compression::Library)libtype;
			break;
		default:
			return;
		}
	}
	mParams.mpSrcStream->Serialize((char*)&mParams.mWindowSize, 4);
	mParams.mpSrcStream->Serialize((char*)&mNumPages, 4);
	mNumPages++;
	mPageOffsets = (unsigned __int64*)calloc(1, mNumPages * 8);
	mParams.mpSrcStream->Serialize((char*)mPageOffsets, mNumPages * 8);
	*pContainerSize = (--mNumPages) * mParams.mWindowSize;
	mStreamSize = *pContainerSize;
	mStreamOffset = mParams.mpSrcStream->GetPosition();
	mStreamPosition = 0;
	//mCacheablePages = 0x10 > mNumPages ? mNumPages : 0x10;
	mpCachedPage= (char*)malloc(mParams.mWindowSize);
	mpReadTransitionBuf = (char*)calloc(1, mParams.mWindowSize + 0x50);//add 80 bytes in case it goes over (bad compression!)
}

DataStreamContainer::~DataStreamContainer() {
	if (mPageOffsets)
		free(mPageOffsets);
	if (mpCachedPage)
		free(mpCachedPage);
	if (mpReadTransitionBuf)
		free(mpReadTransitionBuf);
}

void DataStreamContainer::GetChunk(unsigned __int64 index) {
	unsigned __int64 offset = mPageOffsets[index];
	unsigned __int64 size = GetCompressedPageSize(index);
	mParams.mpSrcStream->SetPosition(mStreamOffset + offset, DataStreamSeekType::eSeekType_Begin);
	mParams.mpSrcStream->Serialize(mpReadTransitionBuf, size);
	if (mParams.mbEncrypt) {
		LibTelltaleTool_BlowfishDecrypt((unsigned char*)mpReadTransitionBuf, size, true, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
	}
	if (mParams.mCompressionLibrary == Compression::Library::ZLIB) {
		unsigned int destl = mParams.mWindowSize;
		bool r = Compression::ZlibDecompress(mpCachedPage, &destl, mpReadTransitionBuf, size);
#if defined(_MSC_VER) && defined(_DEBUG)
		if (destl != mParams.mWindowSize || !r)
			printf("Decompression failed: %s", destl != mParams.mWindowSize ? "Dest len not equal" : "ZDecompress returned false.");
#endif
	}else if (mParams.mCompressionLibrary == Compression::Library::OODLE) {
		throw "OODLE NOT IMPLEMENTED YET!";
	}
}

/*struct _range {
	unsigned int mMin;
	unsigned int mMax;

	inline bool contains(int value) {
		return value >= mMin && mMax >= value;
	}

	_range common(const _range& o) {
		_range ret;
		ret.mMin = o.mMin >= mMin ? o,mMin : mMin;
		ret.mMax = o.mMax <= mMax ? o.mMax : mMax;
		return ret;
	}

	int diff() {
		return mMax - mMin;
	}

};*/


inline unsigned __int64 DataStreamContainer::GetCompressedPageSize(unsigned __int32 index) {
	return mPageOffsets[index + 1] - mPageOffsets[index];
}

bool DataStreamContainer::SetPosition(signed __int64 pos, DataStreamSeekType type) {
	unsigned __int64 final = 0;
	switch (type) {
	case DataStreamSeekType::eSeekType_Begin:
		final = pos;
		break;
	case DataStreamSeekType::eSeekType_Current:
		final = mStreamPosition + pos;
		break;
	case DataStreamSeekType::eSeekType_End:
		final = mStreamSize - pos;
		break;
	}
	if (final > mStreamSize)return false;
	if (mParams.mbCompress) {
		unsigned __int64 tot = 0;
		int blocks = pos / mParams.mWindowSize;
		if (blocks != (mStreamPosition / mParams.mWindowSize)) {
			GetChunk(blocks);
		}
	}
	else {
		mParams.mpSrcStream->SetPosition(mStreamOffset + final, DataStreamSeekType::eSeekType_Begin);
	}
	mStreamPosition = final;
	return true;
}

bool DataStreamLegacyEncrypted::SetPosition(signed __int64 pos, DataStreamSeekType type) {
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
	return mpBase->SetPosition(pos + mHeader, type);
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

bool DataStreamMemory::Serialize(char* buffer, unsigned __int64 bufsize) {
	if (!buffer && bufsize)return false;
	if (!bufsize)return true;
	if (IsRead()) {
		if (bufsize + mOffset > mSize)return false;
		memcpy(buffer, (char*)mMemoryBuffer + mOffset, bufsize);
		mOffset += bufsize;
	}
	else if(IsWrite()) {
		int memorybufsize = mSize;
		if (mSize % mGFact)memorybufsize += mGFact - (mSize % mGFact);
		int numchunks = memorybufsize / mGFact;
		if (mSize > mOffset) {
			int rem = mSize - mOffset;
			if (rem >= bufsize) {
				memcpy((char*)mMemoryBuffer + mOffset, buffer, bufsize);
				mOffset += bufsize;
				return true;
			}
			memcpy((char*)mMemoryBuffer + mOffset, buffer, rem);
			buffer += rem;
			bufsize -= rem;
			mOffset = mSize;
		}
		int rem = memorybufsize - mSize;
		if (rem >= bufsize) {
			memcpy((char*)mMemoryBuffer + mOffset, buffer, bufsize);
			mOffset += bufsize;
			mSize = mOffset;
			return true;
		}
		memcpy((char*)mMemoryBuffer + mOffset, buffer, rem);
		mOffset += rem;
		mSize = mOffset;
		buffer += rem;
		bufsize -= rem;
		int appendchunkz = bufsize / mGFact;
		if (bufsize % mGFact)appendchunkz++;
		void* nb = realloc(mMemoryBuffer, memorybufsize + appendchunkz * mGFact);
		if (!nb)return false;
		mMemoryBuffer = nb;
		memcpy((char*)mMemoryBuffer + memorybufsize, buffer, bufsize);
		mSize += bufsize;
		mOffset = mSize;
	}
	return true;
}

bool DataStreamMemory::SetPosition(signed __int64 pos, DataStreamSeekType type) {
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

DataStreamMemory::~DataStreamMemory() {
	if (mMemoryBuffer) {
		free(mMemoryBuffer);
	}
}

DataStreamMemory& DataStreamMemory::operator=(DataStreamMemory&& other) {
	mSize = other.mSize;
	mOffset = other.mOffset;
	mGFact = other.mGFact;
	mMemoryBuffer = other.mMemoryBuffer;
	other.mSize = 0;
	other.mOffset = 0;
	other.mGFact = DEFAULT_GROWTH_FACTOR;
	other.mMemoryBuffer = calloc(1, other.mGFact);//could be made faster with some kinda check etc idk i cba this is a telltale lib not memlib
	return *this;
}

DataStreamMemory::DataStreamMemory(unsigned __int64 initial, unsigned __int64 growth) : DataStreamMemory(initial) {
	mGFact = growth;
}

DataStreamMemory::DataStreamMemory(unsigned __int64 initial) : mOffset(0), mSize(initial),
	DataStream(DataStreamMode::eMode_Write) {
	if (mGFact > initial) {
		mMemoryBuffer = calloc(1, mGFact);
	}
	else {
		unsigned __int64 memorybufsize = mSize;
		if (mSize % mGFact)memorybufsize += mGFact - (mSize % mGFact);
		mMemoryBuffer = calloc(1, memorybufsize);
	}
}

DataStreamMemory::DataStreamMemory(DataStreamMemory&& other) : mOffset(other.mOffset), mGFact(other.mGFact), mMemoryBuffer(other.mMemoryBuffer),
	DataStream(other.mMode), mSize(other.mSize){
	other.mSize = 0;
	other.mOffset = 0;
	other.mGFact = DEFAULT_GROWTH_FACTOR;
	other.mMemoryBuffer = calloc(1, other.mGFact);
}

bool DataStreamSubStream::Serialize(char* buf, unsigned __int64 bufsize) {
	if (!bufsize)return true;
	if ((!buf && bufsize) || (mOffset + bufsize > mSize))return false;
	if (!mpBase->SetPosition(mStreamOffset + mOffset, DataStreamSeekType::eSeekType_Begin))return false;
	if(!mpBase->Serialize(buf, bufsize))return false;
	mOffset += bufsize;
	return true;
}

DataStreamSubStream* DataStream::GetSubStream(unsigned __int64 off, unsigned __int64 size) {
	if (!IsRead())return NULL;
	if (off + size > GetSize()) return NULL;
	return new DataStreamSubStream(this, size, off);
}

DataStreamSubStream* DataStreamSubStream::GetSubStream(unsigned __int64 off, unsigned __int64 size) {
	if (!IsRead())return NULL;
	if (this->mOffset + size > mSize) return NULL;
	return new DataStreamSubStream(this, size, off);
}
																
bool DataStreamMemory::Truncate(unsigned __int64 newsize) {
	if (!IsWrite())return false;
	int memorybufsize = mSize;
	if (mSize % mGFact)memorybufsize += mGFact - (mSize % mGFact);
	if (newsize == mSize)return true;
	if (newsize < memorybufsize) {
		memset((char*)mMemoryBuffer + newsize, 0, memorybufsize - newsize);
		mSize = newsize;
		if (mOffset > mSize)mOffset = mSize;
	}
	else {
		int newchunkcount = newsize / mGFact;
		if (newsize % mGFact)newchunkcount++;
		void* nb = realloc(mMemoryBuffer, memorybufsize = newchunkcount * mGFact);
		if (!nb)return false;
		mMemoryBuffer = nb;
		mSize = newsize;
	}
	return true;
}

bool DataStreamMemory::Transfer(DataStream* dst, unsigned __int64 off, unsigned __int64 size) {
	if (off + size > mSize || !dst)return false;
	bool settoread = false;
	if (dst->mMode == DataStreamMode::eMode_Read)settoread = true;
	dst->mMode = DataStreamMode::eMode_Write;
	if (!dst->Serialize((char*)mMemoryBuffer + off, size))return false;
	if (settoread)dst->mMode = DataStreamMode::eMode_Read;
	return true;
}

bool DataStreamSubStream::Transfer(DataStream* dst, unsigned __int64 off, unsigned __int64 size) {
	if (off + size > mSize || !dst)return false;
	return mpBase->Transfer(dst, mStreamOffset + off, size);
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
	mSize(size), mOffset(0) {
	if (!base)throw "No base passed";
	mStreamOffset = base->GetPosition();
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
	this->mStreamOffset = final;
	return true;
}

bool DataStreamFile_Win::Serialize(char* buf, unsigned __int64 bufsize) {
	if (!bufsize)return true;
	if (IsInvalid() || !buf && bufsize)return false;
	fseek(mHandle, mStreamOffset, SEEK_SET);
	if (IsWrite()) {
		if (1 != fwrite(buf, bufsize, 1, mHandle)) return false;//if we couldnt write 1 element
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