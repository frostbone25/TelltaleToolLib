// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.


#include "Meta.hpp"
#include <typeinfo>
#include <bit>

i32 sMetaTypesCount = 0;
MetaClassDescription* spFirstMetaClassDescription = NULL;
char Symbol::smSymbolBuffer[sizeof(u64) * 2 + 1];//1byte= 2 hex chars

void MetaStream::AddVersion(const SerializedVersionInfo* version) {
	for (int i = 0; i < mVersionInfo.size(); i++) {
		if (mVersionInfo[i].mTypeSymbolCrc == version->mTypeSymbolCrc)return;
	}
	mVersionInfo.push_back(MetaVersionInfo{ version->mTypeSymbolCrc, version->mVersionCrc });
}

void MetaStream::SetObjectAsArrayType() {
	//this should be overriden, noramlly in JSON versions this is useful. but in binary mode its not useful so its empty
}

void MetaStream::_WriteHeader() {
	if (!mSection[0].mpStream || !mSection[0].mpStream->IsWrite())return;
	_SetSection(SectionType::eSection_Header);
	u32 magic;
	if (mStreamVersion == 6 || mStreamVersion == 5) {//MSV5 and MSV6 are the same in serialized form. dont know why they needed MSV6,
		//MSV5 was the same lol, thinking its something backend. would need to look into a MSV5 game executable but dont have any pbds for 'em
		magic = GetMetaMagic(6);
		serialize_uint32(&magic);

		u32 default_ = mSection[(int)SectionType::
			eSection_Default].mpStream ? mSection[(int)SectionType::
			eSection_Default].mpStream->GetSize() : 0;
		mSection[(int)SectionType::
			eSection_Default].mCompressedSize = default_;

		u32 debug_ = mSection[(int)SectionType::
			eSection_Debug].mpStream ? mSection[(int)SectionType::
			eSection_Debug].mpStream->GetSize() : 0;
		mSection[(int)SectionType::
			eSection_Debug].mCompressedSize = debug_;

		u32 async_ = mSection[(int)SectionType::
			eSection_Async].mpStream ? mSection[(int)SectionType::
			eSection_Async].mpStream->GetSize() : 0;
		mSection[(int)SectionType::
			eSection_Async].mCompressedSize = async_;

		if (mSection[(int)SectionType::
			eSection_Default].mbCompressed)
			default_ |= 0x80000000;
		if (mSection[(int)SectionType::
			eSection_Debug].mbCompressed)
			debug_ |= 0x80000000;
		if (mSection[(int)SectionType::
			eSection_Async].mbCompressed)
			async_ |= 0x80000000;
		serialize_uint32(&default_);
		serialize_uint32(&debug_);
		serialize_uint32(&async_);
		u32 vers = mVersionInfo.size();
		serialize_uint32(&vers);
		for (int i = 0; i < vers; i++) {
			MetaVersionInfo verinfo = mVersionInfo[i];
			serialize_uint64(&verinfo.mTypeSymbolCrc);
			serialize_uint32(&verinfo.mVersionCrc);
		}
	}
	else {
		const char* errmsg = "Cannot write header with version %d, not supported yet.";
		char endbuf[sizeof(errmsg)+30];
		sprintf(endbuf, errmsg, mStreamVersion);
		throw endbuf;
	}
}

bool MetaStream::_ReadHeader(DataStream* stream, u64 completeStreamSize,
	u64* pOutBytesNeeded) {
	if (!stream->IsRead())return false;
	mCurrentSection = SectionType::eSection_Header;
	mSection[0].mpStream = stream;
	u32 versionmagic, numvers;
	mStreamVersion = 0;
	bool isCompiled = true, encryptstream = false;
	mVersionInfo.clear();
	serialize_uint32(&versionmagic);
	if (versionmagic == GetMetaMagic(1) ||
		versionmagic == GetMetaMagic(2)){//MBIN, MTRE
		mStreamVersion = versionmagic == GetMetaMagic(2) ? 3 : 1;
	}
	else if (versionmagic == GetMetaMagic(4) || versionmagic == GetMetaMagic(6) || versionmagic == GetMetaMagic(5)) {//MSV4,5,6
		u32 default_=0, debug_, async_;
		mStreamVersion = versionmagic == GetMetaMagic(4) ? 4 : versionmagic == GetMetaMagic(5) ? 5 : 6;
		if (mStreamVersion >= 5) 
			serialize_uint32(&default_);
		serialize_uint32(&debug_);
		serialize_uint32(&async_);
		//u32 asyncsomething = 0;
		if (static_cast<i32>(default_) < 0) {//same as & 0x80000000 since if 0x80000000 is set then its negative
			mSection[1].mbCompressed = true;
			default_ &= 0x7FFFFFFF;
		}
		if (debug_ & 0x80000000) {
			mSection[2].mbCompressed = true;
			debug_ &= 0x7FFFFFFF;
		}
		if (async_ & 0x80000000) {
			mSection[2].mbCompressed = true;
			async_ &= 0x7FFFFFFF;
			//asyncsomething = 8 * (async_ >> 12);
		}
		mSection[1].mCompressedSize = default_;
		mSection[2].mCompressedSize = debug_;
		mSection[3].mCompressedSize = async_;
	}
	else {//MBES,MCOM and encrypted streams
		mWriteParams.mbEncryptStream = true;
		if (versionmagic == GetMetaMagic(0)) {//MBES
			mStreamVersion = 1;
		}
		else if (versionmagic == GetMetaMagic(3) || versionmagic == EncrypedVersionHeaders[1]) {//MCOM
			mStreamVersion = 3;
			Advance(4);//some int
			if (versionmagic == GetMetaMagic(3)) {
				mWriteParams.mbSerializeAsCompressVersion = true;
				goto VersionInfo;
			}
		}
		else if (versionmagic == EncrypedVersionHeaders[0]) {
			mStreamVersion = 3;
		}
		else if (versionmagic == EncrypedVersionHeaders[2] || 
				versionmagic == EncrypedVersionHeaders[3] ||
				versionmagic == EncrypedVersionHeaders[4]) {
			mStreamVersion = 2;
		}
		if (!mStreamVersion)return false;
		mWriteParams.mEncryptVersion = mStreamVersion;
		DataStream* nstream = new DataStreamLegacyEncrypted(mSection[0].mpStream, mStreamVersion, stream->GetPosition());
		mSection[0].mpStream = nstream;//dont delete old one, refernced still in metastream::mreadwritestream
		encryptstream = true;
	}
VersionInfo:
	serialize_uint32(&numvers);
	if (numvers > 0 && numvers <= 0x3E8) {
		mVersionInfo.reserve(numvers);
		for (int i = 0; i < numvers; i++) {
			MetaVersionInfo verinfo{ 0 };
			if (mStreamVersion >= 3) {
				serialize_uint64(&verinfo.mTypeSymbolCrc);
			}
			else {
				String typeName;
				serialize_String(&typeName);
				char* buf = (char*)calloc(1, typeName.size()+1);
				if (!buf)return false;
				memcpy(buf, typeName.c_str(), typeName.size());
				TelltaleToolLib_MakeInternalTypeName(&buf);
				typeName = buf;
				free(buf);
				//SubsituteClassNames tf? cba
				verinfo.mTypeSymbolCrc = CRC64_CaseInsensitive(0, typeName.c_str());
			}
			serialize_uint32(&verinfo.mVersionCrc);
			MetaClassDescription* desc = TelltaleToolLib_FindMetaClassDescription_ByHash(verinfo.mTypeSymbolCrc);
			if(desc)SerializedVersionInfo::RetrieveCompiledVersionInfo(desc);
			if (!desc || !desc->mpCompiledVersionSerializedVersionInfo || 
					verinfo.mVersionCrc != desc->mpCompiledVersionSerializedVersionInfo->mVersionCrc)
				isCompiled = false;
			mVersionInfo.push_back(verinfo);
		}
	}
	mSection[0].mCompressedSize = GetPos();
	if (mStreamVersion <= 3) {//MBIN,MBES,MTRE,MCOM (unsectioned streams)
		if(encryptstream)mSection[0].mCompressedSize -= 4;
		mSection[1].mCompressedSize = stream->GetSize() - mSection[0].mCompressedSize;
		mSection[2].mCompressedSize = 0;
		mSection[3].mCompressedSize = 0;
	}
	if (isCompiled) {
		mRuntimeFlags |= (int)RuntimeFlags::eStreamIsCompiledVersion;
	}
	else {
		mRuntimeFlags &= ~((int)RuntimeFlags::eStreamIsCompiledVersion);
	}
	return true;
}

void MetaStream::BeginBlock() {
	SectionInfo& sect = mSection[(int)mCurrentSection];
	BlockInfo b{ GetPos() };
	serialize_uint32(&b.mBlockLength);
	if (mMode == MetaStreamMode::eMetaStream_Read)
		b.mBlockLength += GetPos() - 4;//set block len to end of block offset
	sect.mBlockInfo.push(b);
}

void MetaStream::EndObject(Symbol* name, MetaClassDescription* pDesc, MetaMemberDescription* pContext) {
	if (!(pDesc->mFlags.mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled)
		&& !(pContext->mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled))
		EndBlock();
}

MetaClassDescription* GetMetaClassDescription(const char* typeInfoName) {
	if (!sInitialized)return NULL;
	MetaClassDescription* i = spFirstMetaClassDescription;
	if (typeInfoName == NULL) {
		return NULL;
	}
	else {
		for (i; i; i = i->pNextMetaClassDescription) {
			if (!_strcmpi(typeInfoName, i->mpTypeInfoExternalName))
				return i;
		}
	}
	return NULL;
}

template<typename T> MetaClassDescription* MetaClassDescription_Typed<T>::GetMetaClassDescription(const char* type) {
	if (!sInitialized)return NULL;
	MetaClassDescription* i = spFirstMetaClassDescription;
	if (type == NULL) {
		for (i; i; i = i->pNextMetaClassDescription) {
			if (!_strcmpi(typeid(T).name(), i->mpTypeInfoExternalName))
				return i;
		}
	}
	else {
		for (i; i; i = i->pNextMetaClassDescription) {
			if (!_strcmpi(typeid(T).name(), type))
				return i;
		}
	}
	return NULL;
}

SerializedVersionInfo* SerializedVersionInfo::RetrieveCompiledVersionInfo(MetaClassDescription* pObjDescription) {
	SerializedVersionInfo* result = pObjDescription->mpCompiledVersionSerializedVersionInfo;
	if (!result) {
		result = new SerializedVersionInfo;
		MetaOperation op = pObjDescription->GetOperationSpecialization(MetaOperationDescription::eMetaOpTwentyOne);
		if (op) {
			op(NULL, pObjDescription, NULL, result);
		}
		else {
			Meta::MetaOperation_SerializedVersionInfo(NULL, pObjDescription, NULL, result);
		}
		pObjDescription->mpCompiledVersionSerializedVersionInfo = result;
	}
	return result;
}

i64 MetaStream::BeginObject(Symbol* name, MetaClassDescription* pDesc, MetaMemberDescription* pContext) {
	if (!(pDesc->mFlags.mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled) 
		&& !(pContext->mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled))
		BeginBlock();
	return 0;
}

//used for json
i64 MetaStream::BeginAnonObject(void* data) {
	return 0;
}

//used for json
void MetaStream::EndAnonObject(int id) {
	;
}

void MetaStream::SkipToEndOfCurrentBlock() {//used in read mode really
	SectionInfo& sect = mSection[(int)mCurrentSection];
	if (sect.mBlockInfo.size()) {
		BlockInfo b = sect.mBlockInfo.top();
		SetPos(b.mBlockLength);
		sect.mBlockInfo.pop();
	}
}

void MetaStream::EndBlock() {
	SectionInfo& sect = mSection[(int)mCurrentSection];
	BlockInfo b = sect.mBlockInfo.top();
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		u64 currentpos = GetPos();
		SetPos(b.mBlockLength);
		u32 blocklen = currentpos - b.mBlockLength;
		serialize_uint32(&blocklen);
		SetPos(currentpos);
	}
	else if(GetPos() != b.mBlockLength){
		SetPos(b.mBlockLength);
	}
	sect.mBlockInfo.pop();
}

u64 MetaStream::GetPos() {
	return mSection[(int)mCurrentSection].mpStream->GetPosition();
}

MetaVersionInfo* MetaStream::GetStreamVersion(u64 typeSymbolCrc) {//could use auto
	for (std::vector<MetaVersionInfo>::iterator it = mVersionInfo.begin(); it != mVersionInfo.end(); it++) {
		if (it->mTypeSymbolCrc == typeSymbolCrc)return mVersionInfo.data() + std::distance(mVersionInfo.begin(), it);
	}
	return NULL;
}

MetaVersionInfo* MetaStream::GetStreamVersion(MetaClassDescription *d) { return GetStreamVersion(d->mHash); };

void MetaStream::SetPos(u64 pos) {
	mSection[(int)mCurrentSection].mpStream->SetPosition(pos, DataStreamSeekType::eSeekType_Begin);
}

u64 MetaStream::GetSize() {
	return mSection[(int)mCurrentSection].mpStream->GetSize();
}

u64 MetaStream::GetPartialStreamSize() {
	return mSection[(int)SectionType::eSection_Default].mpStream->GetSize()
		+ mSection[(int)SectionType::eSection_Async].mpStream->GetSize()
		+ mSection[(int)SectionType::eSection_Debug].mpStream->GetSize();
}

bool MetaStream::HasAsyncSection() {
	return mSection[(int)SectionType::eSection_Async].mpStream->GetSize() != 0;
}

bool MetaStream::HasDebugSection() {
	return mSection[(int)SectionType::eSection_Debug].mpStream->GetSize() != 0;
}

void MetaStream::EndAsyncSection() {
	if (mCurrentSection == SectionType::eSection_Async) {
		_SetSection(SectionType::eSection_Default);
	}
}

bool MetaStream::BeginAsyncSection() {
	return mCurrentSection == SectionType::eSection_Default ? _SetSection(SectionType::eSection_Async) : false;
}

void MetaStream::DisableDebugSection() {
	SectionInfo& debug = mSection[(int)SectionType::eSection_Debug];
	debug.mbEnable = false;
	if (debug.mpStream)
		delete debug.mpStream;
	if (mCurrentSection == SectionType::eSection_Debug)
		EndDebugSection();
}

void MetaStream::EndDebugSection() {
	if (mCurrentSection == SectionType::eSection_Debug && mDebugSectionDepth > 0) {
		mDebugSectionDepth--;
		if (!mDebugSectionDepth)
			_SetSection(SectionType::eSection_Default);
	}
}

bool MetaStream::BeginDebugSection() {
	if ((mCurrentSection == SectionType::eSection_Default && !mDebugSectionDepth && _SetSection(SectionType::eSection_Debug))
		|| (mCurrentSection == SectionType::eSection_Debug && mDebugSectionDepth > 0)) {
		mDebugSectionDepth++;
		return true;
	}
	return false;
}

void MetaStream::Advance(int numBytes) {
	mpReadWriteStream->SetPosition(numBytes, DataStreamSeekType::eSeekType_Current);
}

void MetaStream::serialize_Symbol(Symbol* symbol) {
	MetaClassDescription* desc = GetMetaClassDescription(typeid(Symbol).name());
	if (!desc)throw "Not initialized";
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		SerializedVersionInfo* versionInfo = desc->mpCompiledVersionSerializedVersionInfo;
		if (!versionInfo)
			versionInfo = SerializedVersionInfo::RetrieveCompiledVersionInfo(desc);
		AddVersion(desc->mpCompiledVersionSerializedVersionInfo);
	}
	u64 crc = symbol->GetCRC();
	serialize_uint64(&crc);
	symbol->SetCRC(crc);
	if (mStreamVersion < 4 || BeginDebugSection()) {
		u32 i = 0;
		serialize_uint32(&i);//string
		Advance(i);
		if (mStreamVersion >= 4)
			EndDebugSection();
	}
}

MetaStream::MetaStream(const char* Name) {
	MetaStream* v1;
	v1 = this;
	this->mpReadWriteStream = 0i64;
	this->mMode = MetaStreamMode::eMetaStream_Closed;
	this->mStreamVersion = 0;
	v1->mRuntimeFlags.mFlags = 0;
	int len = strlen(Name);
	memcpy(mName, Name, len >= 260 ? 259 : len);
}

void MetaStream::serialize_uint64(u64* param) {
	if (std::endian::native == std::endian::big) {
		if (mMode == MetaStreamMode::eMetaStream_Read)
			ReadData(param, 8);
		u64 p = 0;
		p |= ((*param & 0x00000000000000FF) << 56);
		p |= ((*param & 0x000000000000FF00) << 40);
		p |= ((*param & 0x0000000000FF0000) << 24);
		p |= ((*param & 0x00000000FF000000) <<  8);
		p |= ((*param & 0x000000FF00000000) >>  8);
		p |= ((*param & 0x0000FF0000000000) >> 24);
		p |= ((*param & 0x00FF000000000000) >> 40);
		p |= ((*param & 0xFF00000000000000) >> 56);
		if (mMode == MetaStreamMode::eMetaStream_Write)
			WriteData(&p, 8);
		else
			*param = p;
		return;
	}
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		ReadData(param, 8);
	}
	else {
		WriteData(param, 8);
	}
}


void MetaStream::serialize_uint32(u32* param) {
	if (std::endian::native == std::endian::big) {
		if (mMode == MetaStreamMode::eMetaStream_Read)
			ReadData(param, 4);
		u32 p = 0;
		p |= ((*param & 0xFF000000) >> 24);
		p |= ((*param & 0xFF0000) >> 8);
		p |= ((*param & 0xFF00) << 8);
		p |= ((*param & 0xFF) << 24);
		if (mMode == MetaStreamMode::eMetaStream_Write)
			WriteData(&p, 4);
		else
			*param = p;
		return;
	}
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		ReadData(param, 4);
	}
	else {
		WriteData(param, 4);
	}
}


void MetaStream::serialize_uint16(u16* param) {
	if (std::endian::native == std::endian::big) {
		if (mMode == MetaStreamMode::eMetaStream_Read)
			ReadData(param, 2);
		u16 p = 0;
		p |= ((*param & 0x00FF) << 8);
		p |= ((*param & 0xFF00) >> 8);
		if (mMode == MetaStreamMode::eMetaStream_Write)
			WriteData(&p, 2);
		else
			*param = p;
		return;
	}
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		ReadData(param, 2);
	}
	else {
		WriteData(param, 2);
	}
}

void MetaStream::serialize_int8(char* param) {
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		ReadData(param, 1);
	}
	else {
		WriteData(param, 1);
	}
}

void MetaStream::serialize_float(float* param) {
	serialize_uint32((u32*)param);
}

void MetaStream::serialize_double(long double* param) {
	serialize_uint64((u64*)param);
}

void MetaStream::serialize_bool(bool* param) {
	char val = (*param != 0) + 0x30;
	serialize_bytes((void*)val, 1);
	if ((u8)(val - 0x30) <= 1u) {
		*param = val == 0x31;
	}
	else {
		int x = 00'00'00'00;
		serialize_bytes(&x, 3);
		*param = val || x;
	}
}

void MetaStream::serialize_String(String* param) {
	static char sTempBuffer[512];
	u32 size = param->size();
	serialize_uint32(&size);
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		if (512 >= size) {
			ReadData(sTempBuffer, size);
			param->assign(sTempBuffer, size);
		}
		else {
			void* tempbuffer = malloc(size);
			ReadData(tempbuffer, size);
			param->assign((char*)tempbuffer, size);
			free(tempbuffer);
		}
	}
	else if(size) {
		WriteData((void*)param->c_str(), size);
	}
}

i64 MetaStream::WriteData(void* bytes, u32 size) {
	if (!bytes)return NULL;
	if (!size)return 0;
	DataStream* stream = mSection[(int)mCurrentSection].mpStream;
	if (!stream)return -1;
	if (!stream->IsWrite())return -1;
	if (!stream->Serialize((char*)bytes, size))return -1;
	return size;
}

i64 MetaStream::ReadData(void* bytes, u32 size) {
	if (!bytes)return NULL;
	if (!size)return 0;
	DataStream* stream = mSection[(int)mCurrentSection].mpStream;
	if (!stream)return -1;
	if (!stream->IsRead())return -1;//can convert -1 (signed) from 64 to 32bit without problem as its 0xFFFFFFFF etc
	if (!stream->Serialize((char*)bytes, size))return -1;
	return size;
}

int MetaStream::serialize_bytes(void* bytes, u32 size) {
	return mMode == MetaStreamMode::eMetaStream_Read ? ReadData(bytes, size) : WriteData(bytes, size);
}

//checks if chunks need compressing and compresses
void MetaStream::_FinalizeStream() {
	for (int i = 1; i <= 3; i++) {
		SectionInfo& sect = mSection[i];
		if (mParams.mbCompress && sect.mpStream->GetSize() > 0x8000) {
			throw "Does not support compression or compressed files yet!";
			//TODO compress sect.mpStream into a new stream, once set then set sect.mpStream to compressed.
			//make sure to delete sect.mpStream before its set (the old stream)
			//use datastreamcontainers for the TTCZ header
			sect.mbCompressed = true;
		}
		else {
			sect.mCompressedSize = sect.mpStream ? sect.mpStream->GetSize() : 0;
			sect.mbCompressed = false;
		}
	}
}

bool MetaStream::_SetSection(SectionType s) {
	SectionInfo& sect = mSection[(int)s];
	if (sect.mpStream) {
		mCurrentSection = s;
		sect.mpStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
		return true;
	}
	if (!sect.mbEnable || mMode != MetaStreamMode::eMetaStream_Write)return false;
	sect.mpStream = new DataStreamMemory(0x0ui64, 0x40000ui64);
	mCurrentSection = s;
	return true;
}

MetaStream::~MetaStream() {
	Close();
	if (mpReadWriteStream) {
		if (mpReadWriteStream != mSection[0].mpStream)
			delete mSection[0].mpStream;
		delete mpReadWriteStream;
	}
}


bool MetaStream::Attach(DataStream* stream, MetaStreamMode mode, MetaStreamParams params) {
	if (mode == MetaStreamMode::eMetaStream_Closed || !stream)return false;
	this->mMode = mode;
	this->mpReadWriteStream = stream;
	if (mode != MetaStreamMode::eMetaStream_Read) {
		this->mStreamVersion = 6;//MSV6
		mParams = params;
		mSection[0].mpStream = stream;
		_SetSection(MetaStream::SectionType::eSection_Default);
		return true;
	}
	u64 completeStreamSize = stream->GetSize();
	if (_ReadHeader(stream, completeStreamSize,NULL)) {
		u64 offset = mSection[0].mCompressedSize;
		for (int i = 1; i <= 3; i++) {//for each section (default,async,debug)
			SectionInfo& currentSect = mSection[i];
			if (currentSect.mCompressedSize) {
				//mSection[0].mpStream->SetPosition(offset, DataStreamSeekType::eSeekType_Begin);
				currentSect.mpStream = mSection[0].mpStream->GetSubStream(offset, currentSect.mCompressedSize);
				if (currentSect.mbCompressed) {
					throw "COMPRESSED SECTION! NEED TO LOOK AT THIS FILE AND IMPL";
					//datastreamcontainer::read is called here. I think this function
					//does some sort of decompression? todo. this is found in most 
					//save game files like estore epage etc
					//currentSect.mStreamOffset = 0;//notice this! stream offset is zero.
					//uncompressed sects the offset is the current file offset, but has 0 here because the stream is a seperate
					//decompressed stream
					//currentSect.mStreamSize = currentSect.mpStream->GetSize();
				}
				offset += currentSect.mCompressedSize;
			}
		}
		if (!METASTREAM_ENABLE_DEBUG) {
			//delete debug stream since this version was not built in debug mode, was built in release
			DataStream* debugStream = mSection
				[(int)SectionType::eSection_Debug].mpStream;
			if (debugStream)
				delete debugStream;
			mSection[(int)SectionType::eSection_Debug].mpStream = NULL;
			mSection[(int)SectionType::eSection_Debug].mCompressedSize = 0;
		}
		mCurrentSection = SectionType::eSection_Default;
	}
	else {
		mbErrored = true;
		return false;
	}
	return true;
}



u64 MetaStream::Close() {
	if (this->mMode != MetaStreamMode::eMetaStream_Closed) {
		u64 completeStreamSize = 0;
		if (mMode == MetaStreamMode::eMetaStream_Read && !mbErrored) {
			completeStreamSize = mSection[0].mpStream->GetSize();
			for (int i = 1; i <= 3; i++) {
				if(mSection[i].mpStream)
					completeStreamSize += mSection[i].mpStream->GetSize();
			}
		}
		else if (mMode == MetaStreamMode::eMetaStream_Write) {
			if (!mpReadWriteStream)return 0;
			_FinalizeStream();
			_WriteHeader();
			for (int i = (int)SectionType::eSection_Default; i < (int)SectionType::eSection_Count; i++) {
				if (mSection[i].mpStream) {
					mSection[i].mCompressedSize = mSection[i].mpStream->GetSize();
					mSection[i].mpStream->Transfer(mpReadWriteStream, 0, mSection[i].mCompressedSize);
				}
			}
			completeStreamSize = mpReadWriteStream->GetSize();
		}
		for (int i = 1; i < (int)SectionType::eSection_Count; i++) {
			if (mSection[i].mpStream && mSection[i].mpStream->GetSize()) {
				if (mSection[i].mpStream)
					delete mSection[i].mpStream;
				mSection[i].mpStream = NULL;
				mSection[i].mCompressedSize = 0;
				mSection[i].mbEnable = true;
			}
		}
		return completeStreamSize;
	}
	return 0;
}

void MetaStream::Open(DataStream* stream, MetaStreamMode mode, MetaStreamParams p) {
	//here, the datastream would be loaded from the resource getters, since its not the tool
	//we can just let you pass the file stream in/datastream
	Attach(stream, mode, p);
}

METAOP_FUNC_IMPL_(T3VertexSampleDataBase, SerializeAsync) {
	MetaStream* ms = static_cast<MetaStream*>(pUserData);
	MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
	if (result == MetaOpResult::eMetaOp_Succeed) {
		T3VertexSampleDataBase* db = static_cast<T3VertexSampleDataBase*>(pObj);
		int verts_size = db->mNumVerts * db->mVertSize;
		if (verts_size) {
			if (ms->mMode == MetaStreamMode::eMetaStream_Read) {
				db->mpData = (char*)malloc(verts_size);
			}
			ms->serialize_bytes(db->mpData, verts_size);
		}
	}
	return result;
}

MetaMemberDescription::~MetaMemberDescription() {
	if (this->mFlags & (MetaFlag::MetaFlag_EnumIntType | MetaFlag::MetaFlag_EnumStringType)) {
		MetaEnumDescription* i = this->mpEnumDescriptions;
		while (i) {
			if (i->mpEnumName && (i->mFlags & MetaFlag::MetaFlag_Heap)) {
				free((void*)i->mpEnumName);
				MetaEnumDescription* next = i->mpNext;
				free(i);
				i = next;
				continue;
			}
			else {
				i->mpEnumName = NULL;
				i->mFlags = 0;
				i->mpNext;
			}

			i = i->mpNext;
		}
	}
	else if (this->mFlags & MetaFlag::MetaFlag_FlagType) {
		for (MetaEnumDescription* i = this->mpEnumDescriptions; i; i = i->mpNext) {
			i->mpEnumName = NULL;
			i->mFlags = 0;
		}
	}
}

void* MetaClassDescription::CastToBase(const void* pObj, MetaClassDescription* pBaseClassDesc) {
	if (this == pBaseClassDesc)
		return (void*)pObj;
	void* result = NULL;
	MetaMemberDescription* mem = mpFirstMember;
	if (mem) {
		while (true) {
			if (mem->mFlags & MetaFlag::MetaFlag_BaseClass) {
				if(result=mem->mpMemberDesc->CastToBase(&((char*)pObj)[mem->mOffset], pBaseClassDesc))
					return result;
			}
		}
		mem = mem->mpNextMember;
		if (!mem)return NULL;
	}
}

MetaOperation MetaClassDescription::GetOperationSpecialization(int ID) {
	if (this->mMetaOperationsList) {
		MetaOperationDescription* i = mMetaOperationsList;
		while (i->id != ID) {
			i = i->mpNext;
			if (!i)return NULL;
		}
		return i->mpOpFn;
	}
	else {
		return NULL;
	}
}

void MetaClassDescription::InstallSpecializedMetaOperation(MetaOperationDescription* pNewOperation) {
	if (this->mMetaOperationsList) {
		MetaOperationDescription* i = this->mMetaOperationsList;
		while (i->id != pNewOperation->id) {
			i = i->mpNext;
			if (!i) {
				pNewOperation->mpNext = this->mMetaOperationsList;
				this->mMetaOperationsList = pNewOperation;
				return;
			}
		}
	}
	else {
		pNewOperation->mpNext = this->mMetaOperationsList;//this->mMetaOperationsList is NULL so its just NULL
		this->mMetaOperationsList = pNewOperation;
	}
}

bool MetaClassDescription::IsDerivedFrom(MetaClassDescription* pDesc) {
	if (this == pDesc)return true;
	if (pDesc) {
		for (MetaMemberDescription* i = this->mpFirstMember; i; i = i->mpNextMember) {
			if (i->mFlags & MetaFlag::MetaFlag_BaseClass) {
				if (i->mpMemberDesc->IsDerivedFrom(pDesc))return true;
			}
		}
	}
	return false;
}

void MetaClassDescription::Insert() {
	if (!pNextMetaClassDescription) {
		pNextMetaClassDescription = spFirstMetaClassDescription;
		spFirstMetaClassDescription = this;
	}
	this->mFlags.mFlags |= MetaFlag::Internal_MetaFlag_Initialized;
}

void MetaClassDescription::Initialize(const char* typeInfoName) {
	this->mpTypeInfoName = typeInfoName;
	this->mHash = CRC64_CaseInsensitive(0, typeInfoName);
}

MetaMemberDescription* MetaClassDescription::GetMemberDescription(const char* memberName) {
	for (MetaMemberDescription* i = mpFirstMember; i; i = i->mpNextMember) {
		if (!strcmp(i->mpName, memberName))return i;
	}
	return NULL;
}

void MetaClassDescription::GetDescriptionSymbol(Symbol* sym) {
	String result;
	GetToolDescriptionName(&result);
	Symbol nsym(result.c_str());
	*sym = nsym;
}


METAOP_FUNC_IMPL(Destroy) {
	pObjDescription->Delete(pObj);
	return MetaOpResult::eMetaOp_Succeed;
}

void MetaClassDescription::CopyConstruct(void* pDest, void* pSrc) {
	if (mpVTable && mpVTable[3])
		((FuncCopyConstruct)mpVTable[3])(pDest, pSrc);
}

bool MetaClassDescription::MatchesHash(u64 o) {
	return o == mHash;
}

MetaClassDescription::~MetaClassDescription() {
	if (this->mpSerializeAccel)
		delete[] this->mpSerializeAccel;
	if (this->mpCompiledVersionSerializedVersionInfo)
		delete mpCompiledVersionSerializedVersionInfo;
	if (mbNameIsHeapAllocated)
		free((void*)mpTypeInfoName);
}

void MetaClassDescription::Construct(void* pObj) {
	if (mpVTable && mpVTable[2] && pObj)
		((FuncConstruct)(mpVTable[2]))(pObj);
}

void* MetaClassDescription::New() {
	if (mpVTable && mpVTable[0])
		return ((FuncNew)mpVTable[0])();
	return NULL;
}

void MetaClassDescription::Destroy(void* pObj) {
	if (mpVTable && mpVTable[4] && pObj)
		((FuncDelete)(mpVTable[4]))(pObj);
}

void MetaClassDescription::Delete(void* pObj) {
	if (mpVTable && mpVTable[1] && pObj)
		((FuncDelete)(mpVTable[1]))(pObj);
}

String* MetaClassDescription::GetToolDescriptionName(String* result) {
	int size = strlen(this->mpTypeInfoName);
	char* tmp = (char*)calloc(1, size + 1);
	memcpy(tmp, this->mpTypeInfoName, size);
	TelltaleToolLib_MakeInternalTypeName(&tmp);
	new (result) String(tmp);
	free(tmp);
	return result;
}

METAOP_FUNC_IMPL(EnumerateMembers) {
	Meta::EnumerateMembersInfo* handle = static_cast<Meta::EnumerateMembersInfo*>(pUserData);
	if (!handle)return MetaOpResult::eMetaOp_Fail;
	for (MetaMemberDescription* member = pObjDescription->mpFirstMember; member; member = member->mpNextMember) {
		handle->mpFunc(&((char*)pObj)[member->mOffset], member->mpMemberDesc, member);
	}
	return MetaOpResult::eMetaOp_Succeed;
}

METAOP_FUNC_IMPL(Equivalence) {
	MetaOpResult(*function)(void*, MetaClassDescription*, MetaMemberDescription*, void*);
	Meta::Equivalence* userData = static_cast<Meta::Equivalence*>(pUserData);
	char isEqual;
	if (pObjDescription->mpFirstMember) {
		userData->mbEqual = 1;
		for (MetaMemberDescription* i = pObjDescription->mpFirstMember; i; i = i->mpNextMember) {
			isEqual = false;
			function = i->mpMemberDesc->GetOperationSpecialization(MetaOperationDescription::sIDs::eMetaOpNine);
			if (function) {
				function(&((char*)pObj)[i->mOffset], i->mpMemberDesc, i, &isEqual);
			}
			else {
				Meta::MetaOperation_Equivalence(&((char*)pObj)[i->mOffset], i->mpMemberDesc, i, &isEqual);
			}
			if (!isEqual)
				break;
			if (!i->mpNextMember) return MetaOpResult::eMetaOp_Succeed;
		}
		userData->mbEqual = 0;

	}
	else {
		userData->mbEqual = memcmp(pObj, userData->mpOther, pObjDescription->mClassSize) == 0;
	}
	return MetaOpResult::eMetaOp_Fail;
}

METAOP_FUNC_IMPL(GetObjectName) {
	static String szConstObjName("T3 Object");
	String* pDestStr = static_cast<String*>(pUserData);
	if (pDestStr)
		pDestStr->operator=(szConstObjName.c_str());
	return MetaOpResult::eMetaOp_Succeed;
}

MetaSerializeAccel* MetaSerialize_GenerateAccel(MetaClassDescription* pObj) {
	if (!pObj->mpFirstMember)
		return NULL;
	if (pObj->mpSerializeAccel)return pObj->mpSerializeAccel;
	MetaMemberDescription* member = pObj->mpFirstMember;
	u64 toserialize = 0;
	do {
		bool dont = member->mFlags & MetaFlag::MetaFlag_MetaSerializeDisable || member->mpMemberDesc->mFlags.mFlags & MetaFlag_MetaSerializeDisable;
		member = member->mpNextMember;
		if (!dont)
			toserialize++;
	} while (member);
	if (toserialize > 0) {
		MetaSerializeAccel* accels = new MetaSerializeAccel[toserialize+1i64];
		MetaSerializeAccel* first = accels;
		member = pObj->mpFirstMember;
		do {
			MetaClassDescription* memberDesc = member->mpMemberDesc;
			if (!(memberDesc->mFlags.mFlags & MetaFlag_MetaSerializeDisable) && !(member->mFlags & MetaFlag_MetaSerializeDisable)) {
				accels->mpMemberDesc = member;
				MetaOperation op = memberDesc->GetOperationSpecialization(74);
				accels->mpFunctionAsync = op ? op : Meta::MetaOperation_SerializeAsync;
				op = memberDesc->GetOperationSpecialization(75);
				accels->mpFunctionMain = op ? op : Meta::MetaOperation_SerializeMain;
				accels++;
			}
			member = member->mpNextMember;
		} while (member);
		//reason we have this trailing one is because its the null terminator. bit stupid they could just store a size to spare
		//them 20 or so bytes but telltale ok then
		first[toserialize].mpFunctionAsync = 0;
		first[toserialize].mpFunctionMain = 0;
		first[toserialize].mpMemberDesc = 0;
		pObj->mpSerializeAccel = first;
		return first;
	}
	return NULL;
}

METAOP_FUNC_IMPL(SerializeMain) {
	MetaSerializeAccel* accelerated = pObjDescription->mpSerializeAccel;
	if (accelerated || ((accelerated = MetaSerialize_GenerateAccel(pObjDescription)) != NULL)) {
		while (true) {
			if (!accelerated->mpFunctionMain)return eMetaOp_Succeed;//nothing to serialize
			MetaOpResult result = accelerated->mpFunctionMain(&((char*)pObj)[accelerated->mpMemberDesc->mOffset], 
				accelerated->mpMemberDesc->mpMemberDesc, accelerated->mpMemberDesc, pUserData);
			if (result != eMetaOp_Succeed)return result;
			accelerated++;
			if (!accelerated)
				break;
		}
	}
	return eMetaOp_Succeed;//can still succeed since it could be memberless!
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncuint8) {
	static_cast<MetaStream*>(pUserData)->serialize_bytes(pObj, 1);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncuint16) {
	static_cast<MetaStream*>(pUserData)->serialize_uint16((u16*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncuint32) {
	static_cast<MetaStream*>(pUserData)->serialize_uint32((u32*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncuint64) {
	static_cast<MetaStream*>(pUserData)->serialize_uint64((u64*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncint8) {
	static_cast<MetaStream*>(pUserData)->serialize_bytes(pObj, 1);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncint16) {
	static_cast<MetaStream*>(pUserData)->serialize_uint16((u16*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncint32) {
	static_cast<MetaStream*>(pUserData)->serialize_uint32((u32*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncint64) {
	static_cast<MetaStream*>(pUserData)->serialize_uint64((u64*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncfloat) {
	static_cast<MetaStream*>(pUserData)->serialize_float((float*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncdouble) {
	static_cast<MetaStream*>(pUserData)->serialize_double((long double*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsic1) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, (const char* const)pObj, 1);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsicString) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, ((String*)pObj)->c_str(), ((String*)pObj)->size());
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsic1)
{
	static_cast<Meta::Equivalence*>(pUserData)->mbEqual = (u8*)pObj == (u8*)static_cast<Meta::Equivalence*>(pUserData)->mpOther;
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsic2) {
	static_cast<Meta::Equivalence*>(pUserData)->mbEqual = (u16*)pObj == (u16*)static_cast<Meta::Equivalence*>(pUserData)->mpOther;
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsic4)
{
	static_cast<Meta::Equivalence*>(pUserData)->mbEqual = (u32*)pObj == (u32*)static_cast<Meta::Equivalence*>(pUserData)->mpOther;
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsic8) {
	static_cast<Meta::Equivalence*>(pUserData)->mbEqual = (u64*)pObj == (u64*)static_cast<Meta::Equivalence*>(pUserData)->mpOther;
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsicString) {
	Meta::Equivalence* e = static_cast<Meta::Equivalence*>(pUserData);
	String* s1 = (String*)pObj;
	String* s2 = (String*)e->mpOther;
	e->mbEqual = s1->size() == s2->size() && !memcmp(s1->c_str(), s2->c_str(), s1->size());
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsic2) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, (const char* const)pObj, 2);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsic4) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, (const char* const)pObj, 4);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsic8) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, (const char* const)pObj, 8);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncbool) {
	static_cast<MetaStream*>(pUserData)->serialize_bool((bool*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncString) {
	static_cast<MetaStream*>(pUserData)->serialize_String((String*)pObj);
	return eMetaOp_Succeed;
}


//pObj = object to serialize, pUserData = metastream
METAOP_FUNC_IMPL(SerializeAsync) {
	if(!pObjDescription)return eMetaOp_Fail;
	if (pObjDescription->mFlags.mFlags & (int)MetaFlag::MetaFlag_MetaSerializeDisable ||
		pContextDescription && pContextDescription->mFlags & (int)MetaFlag::MetaFlag_MetaSerializeDisable)
		return eMetaOp_Invalid;
	MetaStream* stream = static_cast<MetaStream*>(pUserData);
	if (pContextDescription && pContextDescription->mpName) {
		stream->BeginObject(pContextDescription->mpName,NULL);
	}
	else stream->BeginAnonObject(NULL);
	if (stream->mMode == MetaStreamMode::eMetaStream_Write) {
		SerializedVersionInfo* ver = SerializedVersionInfo::RetrieveCompiledVersionInfo(pObjDescription);
		if(ver&&!pObjDescription->mbIsIntrinsic)stream->AddVersion(ver);
		MetaSerializeAccel* accel = pObjDescription->mpSerializeAccel;
		if(!accel)accel=MetaSerialize_GenerateAccel(pObjDescription);
		if (accel && accel->mpFunctionAsync) {
			int i = 0; 
			bool blocked = false;
			while (true) {
				if (!accel[i].mpFunctionAsync)break;
				MetaSerializeAccel a = accel[i];
				if (a.mpMemberDesc->mFlags & (int)MetaFlag::MetaFlag_MetaSerializeBlockingDisabled
					|| a.mpMemberDesc->mpMemberDesc->mFlags.mFlags & (int)MetaFlag::MetaFlag_MetaSerializeBlockingDisabled) {
					blocked = false;
				}
				else {
					stream->BeginBlock();
					blocked = true;
				}
				stream->BeginObject(a.mpMemberDesc->mpName, NULL);
				MetaOpResult result = a.mpFunctionAsync(((char*)pObj) + a.mpMemberDesc->mOffset, 
					a.mpMemberDesc->mpMemberDesc, a.mpMemberDesc, pUserData);
				stream->EndObject(a.mpMemberDesc->mpName);
				if (blocked)
					stream->EndBlock();
				if (result != MetaOpResult::eMetaOp_Succeed)break;
				i++;
			}
		}
		if (pContextDescription && pContextDescription->mpName) {
			stream->EndObject(pContextDescription->mpName);
		}
		else stream->EndAnonObject(NULL);
		return eMetaOp_Succeed;
	}
	//if (!(stream->mRuntimeFlags.mFlags & (int)MetaStream::RuntimeFlags::eStreamIsCompiledVersion)) {}
	SerializedVersionInfo* ver = SerializedVersionInfo::RetrieveCompiledVersionInfo(pObjDescription);
	if (!ver)return eMetaOp_Fail;
	for (int i = 0; i < ver->mMembers.size(); i++) {
		SerializedVersionInfo::MemberDesc desc = ver->mMembers[i];
		MetaMemberDescription* member = pObjDescription->GetMemberDescription(&(ver->mMembers[i].mName));
		bool disable = member->mFlags & 1 || member->mpMemberDesc && member->mpMemberDesc->mFlags.mFlags & 1;
		if (disable)continue;
		if (!member)break;
		if (desc.mbBlocked)
			stream->BeginBlock();
		MetaOperation serasync = member->mpMemberDesc->GetOperationSpecialization(74);
		MetaOpResult r;
		if (serasync) {
			r = serasync(((char*)pObj) + member->mOffset, member->mpMemberDesc, member, pUserData);
		}
		else {
			r = MetaOperation_SerializeAsync(((char*)pObj) + member->mOffset, member->mpMemberDesc, member, pUserData);
		}
		if (desc.mbBlocked) {
			stream->EndBlock();
		}
	}
	if (pContextDescription && pContextDescription->mpName) {
		stream->EndObject(pContextDescription->mpName);
	}
	else stream->EndAnonObject(NULL);
	return eMetaOp_Succeed;
}

MetaOpResult PerformMetaSerializeFull(MetaStream* pStream, void* pObj, MetaClassDescription* pDesc) {
	if (!pStream || !pDesc)return eMetaOp_Fail;
	MetaOperation async, main;
	async = pDesc->GetOperationSpecialization(74);
	main = pDesc->GetOperationSpecialization(75);
	if (!async)async = Meta::MetaOperation_SerializeAsync;
	if (!main)main = Meta::MetaOperation_SerializeMain;
	MetaOpResult result = async(pObj, pDesc, NULL, pStream);
	if (result == eMetaOp_Succeed)result = main(pObj, pDesc, NULL, pStream);
	return result;
}

METAOP_FUNC_IMPL(AsyncSave) {
	return PerformMetaSerializeFull(static_cast<MetaStream*>(pUserData), pObj, pObjDescription);
}

METAOP_FUNC_IMPL(SerializedVersionInfo) {
	//if pObj, warn its not needed in the call
	SerializedVersionInfo* ver = static_cast<SerializedVersionInfo*>(pUserData);
	if (ver) {
		if (pObjDescription->mFlags.mFlags & MetaFlag::MetaFlag_MetaSerializeDisable ||
			pContextDescription && pContextDescription->mFlags & MetaFlag::MetaFlag_MetaSerializeDisable)
			return MetaOpResult::eMetaOp_Invalid;//if its not serialized no need for this
		ver->mTypeSymbolCrc = pObjDescription->mHash;
		ver->mSize = pObjDescription->mClassSize;
		char versionCrcBuffer[4], hashBuffer[8];
		u8 crcFlags = ~(unsigned __int8)((unsigned int)pObjDescription->mFlags.
			mFlags >> 1) & MetaFlag::MetaFlag_MetaSerializeDisable;
		ver->mbBlocked = crcFlags;
		versionCrcBuffer[0] = (crcFlags ^ 1) - 1;
		ver->mVersionCrc = CRC32(ver->mVersionCrc, versionCrcBuffer, 4);
		for (MetaMemberDescription* i = pObjDescription->mpFirstMember; i; i = i->mpNextMember) {
			if (i->mFlags & 1)continue;//flag 1 = metaflags:: dont seralized etc
			SerializedVersionInfo::MemberDesc member;
			member.mSize = i->mpMemberDesc->mClassSize;
			member.mTypeNameSymbolCrc = i->mpMemberDesc->mHash;
			member.mbBlocked = !(i->mpMemberDesc->mFlags.mFlags
				& MetaFlag::MetaFlag_MetaSerializeBlockingDisabled)
				&& !(i->mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled);
			member.mName = String(i->mpName);
			member.mTypeName = String(i->mpMemberDesc->mpTypeInfoName);
			ver->mVersionCrc = CRC32(ver->mVersionCrc, member.mName.c_str(),
				member.mName.length());
			memcpy(hashBuffer, &i->mpMemberDesc->mHash, 8);
			ver->mVersionCrc = CRC32(ver->mVersionCrc, hashBuffer, 8);
			memcpy(hashBuffer, &member.mbBlocked, 1);
			ver->mVersionCrc = CRC32(ver->mVersionCrc, hashBuffer, 1);
			ver->mMembers.push_back(member);
		}
	}
	return MetaOpResult::eMetaOp_Succeed;
}