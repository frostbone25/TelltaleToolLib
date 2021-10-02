// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.


#include "Meta.hpp"
#include <typeinfo>
#include <bit>

#define DEFINET(name,Ty) }++sMetaTypesCount;static MetaClassDescription meta_##name; \
if(!(meta_##name.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name.mpVTable[0] = MetaClassDescription_Typed<Ty>::New;\
meta_##name.mpVTable[1] = MetaClassDescription_Typed<Ty>::Delete;\
meta_##name.mpVTable[2] = MetaClassDescription_Typed<Ty>::Construct;\
meta_##name.mpVTable[3] = MetaClassDescription_Typed<Ty>::CopyConstruct;\
meta_##name.mpVTable[4] = MetaClassDescription_Typed<Ty>::Destroy;\
meta_##name.mClassSize = sizeof(Ty);

#define DEFINEOP(name, opName,fid,fun)static MetaOperationDescription meta_##name##_##opName; meta_##name##_##opName.id = fid;\
meta_##name##_##opName.mpOpFn = fun;

#define DEFINEM(name,memberName)static MetaMemberDescription meta_##name##_##memberName

i32 sMetaTypesCount = 0;
MetaClassDescription* spFirstMetaClassDescription = NULL;

void Meta::Initialize() {
	{
		DEFINET(char, char)
			meta_char.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta_char.Initialize("int8");
		meta_char.Insert();
		DEFINET(ushort, u16)
			meta_ushort.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta_ushort.Initialize("uint16");
		meta_ushort.Insert();
		DEFINET(short, i16)
			meta_short.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta_short.Initialize("int16");
		meta_short.Insert();
		DEFINET(int, i32)
			meta_int.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta_int.Initialize("int");
		meta_int.Insert();
		DEFINET(long, i32)
			meta_long.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled | MetaFlag::MetaFlag_PlaceInAddPropMenu;
		meta_long.Initialize("long");
		meta_long.Insert();
		DEFINET(u64, u64)
			meta_u64.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta_u64.Initialize("uint64");
		meta_u64.Insert();
		DEFINET(float, float)
			meta_float.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled | MetaFlag::MetaFlag_PlaceInAddPropMenu;
		meta_float.Initialize("float");
		meta_float.Insert();
		DEFINET(double, double)
			meta_double.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta_double.Initialize("double");
		meta_double.Insert();
		DEFINET(__int64, __int64)
			meta___int64.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta___int64.Initialize("int64");
		meta___int64.Insert();
		DEFINET(__uint32, unsigned __int32)
			meta___uint32.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta___uint32.Initialize("uint32");
		meta___uint32.Insert();
		DEFINET(__int32, __int32)
			meta___int32.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta___int32.Initialize("int32");
		meta___int32.Insert();
		DEFINET(__uint8, unsigned __int8)
			meta___uint8.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta___uint8.Initialize("uint8");
		meta___uint8.Insert();
		DEFINET(symbol, Symbol)
			meta_symbol.mFlags = MetaFlag::MetaFlag_MetaSerializeNonBlockedVariableSize;
		meta_symbol.Initialize("Symbol");
		meta_symbol.Insert();
		DEFINET(flags, Flags)
			meta_flags.Initialize(typeid(Flags));
		DEFINEM(flags, mFlags);
		meta_flags_mFlags.mpName = "mFlags";
		meta_flags_mFlags.mFlags |= MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta_flags_mFlags.mpMemberDesc = &meta_long;
		meta_flags_mFlags.mOffset = memberOffset(&Flags::mFlags);
		meta_flags.mpFirstMember = &meta_flags_mFlags;
		meta_flags.Insert();
		DEFINET(bool, bool)
			meta_bool.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
		meta_bool.Initialize("bool");
		meta_bool.Insert();
		DEFINET(ztest, ZTestFunction)
			meta_ztest.mFlags |= MetaFlag_EnumWrapperClass;
		meta_ztest.Initialize(typeid(ZTestFunction));
		DEFINEM(ztest, mZTestType);
		meta_ztest_mZTestType.mpName = "mZTestType";
		meta_ztest_mZTestType.mpMemberDesc = &meta_long;
		meta_ztest_mZTestType.mOffset = memberOffset(&ZTestFunction::mZTestType);
		meta_ztest.mpFirstMember = &meta_ztest_mZTestType;
		meta_ztest.Insert();
		DEFINET(uidowner, UID::Owner)
			meta_uidowner.Initialize(typeid(UID::Owner));
		DEFINEM(uidowner, miUniqueID);
		meta_uidowner_miUniqueID.mpName = "miUniqueID";
		meta_uidowner_miUniqueID.mpMemberDesc = &meta_long;
		meta_uidowner_miUniqueID.mOffset = memberOffset(&UID::Owner::miUniqueID);
		meta_uidowner.mFlags |= MetaFlag_EditorHide;
		meta_uidowner.mpFirstMember = &meta_uidowner_miUniqueID;
		meta_uidowner.Insert();

		//UID::Generator

		DEFINET(uidgen, UID::Generator)
			meta_uidgen.Initialize(typeid(UID::Generator));
		DEFINEM(uidgen, miNextUniqueID);
		meta_uidgen_miNextUniqueID.mpName = "miNextUniqueID";
		meta_uidgen_miNextUniqueID.mpMemberDesc = &meta_long;
		meta_uidgen_miNextUniqueID.mOffset = memberOffset(&UID::Generator::miNextUniqueID);
		meta_uidgen.mpFirstMember = &meta_uidgen_miNextUniqueID;
		meta_uidgen.Insert();

		//T3VertexSampleDataBase

		DEFINET(t3vsdb, T3VertexSampleDataBase);
		meta_t3vsdb.Initialize(typeid(T3VertexSampleDataBase));

		DEFINEOP(t3vsdb, serialize, MetaOperationDescription::sIDs::eMetaOpSerializeAsync, NULL);
		meta_t3vsdb.InstallSpecializedMetaOperation(&meta_t3vsdb_serialize);

		DEFINEM(t3vsdb, numverts);
		meta_t3vsdb.mpFirstMember = &meta_t3vsdb_numverts;
		meta_t3vsdb_numverts.mpName = "mNumVerts";
		meta_t3vsdb_numverts.mOffset = memberOffset(&T3VertexSampleDataBase::mNumVerts);
		meta_t3vsdb_numverts.mpMemberDesc = &meta_long;

		DEFINEM(t3vsdb, vertsize);
		meta_t3vsdb_vertsize.mpName = "mVertSize";
		meta_t3vsdb_vertsize.mOffset = memberOffset(&T3VertexSampleDataBase::mVertSize);
		meta_t3vsdb_vertsize.mpMemberDesc = &meta_long;
		meta_t3vsdb_numverts.mpNextMember = &meta_t3vsdb_vertsize;

		meta_t3vsdb.Insert();

	}
	Initialize2();
	Initialize3();
	Initialize4();
}

void Meta::Initialize2() {

}

void Meta::Initialize3() {

}

void Meta::Initialize4() {

}

void MetaStream::AddVersion(const SerializedVersionInfo* version) {
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
			eSection_Default].mpStream->GetSize();
		mSection[(int)SectionType::
			eSection_Default].mCompressedSize = default_;
		u32 debug_ = mSection[(int)SectionType::
			eSection_Debug].mpStream->GetSize();
		mSection[(int)SectionType::
			eSection_Debug].mCompressedSize = debug_;
		u32 async_ = mSection[(int)SectionType::
			eSection_Async].mpStream->GetSize();
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
		char endbuf[sizeof(errmsg)+3];
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

template<typename T> MetaClassDescription* MetaClassDescription_Typed<T>::GetMetaClassDescription(const char* type) {
	MetaClassDescription* i = spFirstMetaClassDescription;
	if (type == NULL) {
		for (i; i; i = i->pNextMetaClassDescription) {
			if (!_strcmpi(typeid(T).name(), i->mpTypeInfoName))
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
	MetaClassDescription* desc = MetaClassDescription_Typed<Symbol>::GetMetaClassDescription();
	if (!desc)throw "Not initialized";
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		SerializedVersionInfo* versionInfo = desc->mpCompiledVersionSerializedVersionInfo;
		if (!versionInfo)
			versionInfo = SerializedVersionInfo::RetrieveCompiledVersionInfo(desc);
		AddVersion(desc->mpCompiledVersionSerializedVersionInfo);
	}
	u64 crc = symbol->GetCRC();
	serialize_uint64(&crc);
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
			sect.mCompressedSize = sect.mpStream->GetSize();
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
	sect.mpStream = new DataStreamMemory(0ui64, 0x40000ui64);
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
				if(mSection[i].mpStream)
					mSection[i].mpStream->Transfer(mpReadWriteStream, 0, mSection[i].mpStream->GetSize());
			}
			completeStreamSize = mpReadWriteStream->GetSize();
		}
		for (int i = 1; i < (int)SectionType::eSection_Count; i++) {
			if (mSection[i].mpStream && mSection[i].mpStream->GetSize()) {
				if (mSection[i].mpStream)
					delete mSection[i].mpStream;
				//not needed below but why not
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
			if (!(memberDesc->mFlags.mFlags & MetaFlag_MetaSerializeDisable) && (!member->mFlags & MetaFlag_MetaSerializeDisable)) {
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

METAOP_FUNC_IMPL(SerializeAsync) {
//DO AFTER META STRAEM IMPL
	return eMetaOp_Succeed;
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