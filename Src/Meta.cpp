// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.


#include "Meta.hpp"
#include <typeinfo>

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
			meta_flags.Initialize(typeid(Flags).name());
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
		meta_ztest.Initialize(typeid(ZTestFunction).name());
		DEFINEM(ztest, mZTestType);
		meta_ztest_mZTestType.mpName = "mZTestType";
		meta_ztest_mZTestType.mpMemberDesc = &meta_long;
		meta_ztest_mZTestType.mOffset = memberOffset(&ZTestFunction::mZTestType);
		meta_ztest.mpFirstMember = &meta_ztest_mZTestType;
		meta_ztest.Insert();
		DEFINET(uidowner, UID::Owner)
			meta_uidowner.Initialize(typeid(UID::Owner).name());
		DEFINEM(uidowner, miUniqueID);
		meta_uidowner_miUniqueID.mpName = "miUniqueID";
		meta_uidowner_miUniqueID.mpMemberDesc = &meta_long;
		meta_uidowner_miUniqueID.mOffset = memberOffset(&UID::Owner::miUniqueID);
		meta_uidowner.mFlags |= MetaFlag_EditorHide;
		meta_uidowner.mpFirstMember = &meta_uidowner_miUniqueID;
		meta_uidowner.Insert();

		//UID::Generator

		DEFINET(uidgen, UID::Generator)
			meta_uidgen.Initialize(typeid(UID::Generator).name());
		DEFINEM(uidgen, miNextUniqueID);
		meta_uidgen_miNextUniqueID.mpName = "miNextUniqueID";
		meta_uidgen_miNextUniqueID.mpMemberDesc = &meta_long;
		meta_uidgen_miNextUniqueID.mOffset = memberOffset(&UID::Generator::miNextUniqueID);
		meta_uidgen.mpFirstMember = &meta_uidgen_miNextUniqueID;
		meta_uidgen.Insert();

		//T3VertexSampleDataBase

		DEFINET(t3vsdb, T3VertexSampleDataBase);
		meta_t3vsdb.Initialize(typeid(T3VertexSampleDataBase).name());

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

MetaStream::MetaStream(const char* Name) {
	MetaStream* v1;
	v1 = this;
	this->mpWriteStream = 0i64;
	this->mMode = MetaStreamMode::eMetaStream_Closed;
	v1->mRuntimeFlags.mFlags = 0;
	int len = strlen(Name);
	memcpy(mName, Name, len >= 260 ? 259 : len);
}

MetaStream::~MetaStream() {
	Close();
	if (mpWriteStream) {
		delete mpWriteStream;
	}
}


bool MetaStream::Attach(DataStream* stream, MetaStreamMode mode, MetaStreamParams params) {
	if (mode == MetaStreamMode::eMetaStream_Closed || !stream)return false;
	this->mMode = mode;
	if (mode != MetaStreamMode::eMetaStream_Read) {
		this->mStreamVersion = 6;//MSV6
		mParams = params;
		this->mpWriteStream = stream;
		_SetSection(MetaStream::SectionType::eSection_Default);
		return true;
	}
	u64 completeStreamSize = stream->GetSize();
	if (_ReadHeader(stream, completeStreamSize,NULL)) {
		u64 offset = mSection[(int)SectionType::eSection_Header].mStreamSize;
		for (int i = 1; i <= 3; i++) {//for each section (default,async,debug)
			SectionInfo currentSect = mSection[i];
			if (currentSect.mCompressedSize) {
				if (currentSect.mbCompressed) {
					throw "COMPRESSED SECTION! NEED TO LOOK AT THIS FILE AND IMPL";
					//datastreamcontainer::read is called here. I think this function
					//does some sort of decompression? todo. this is found in most 
					//save game files like estore epage etc
					currentSect.mpDataStream = stream->GetSubStream
						(offset, currentSect.mCompressedSize);
					offset += currentSect.mCompressedSize;
					currentSect.mStreamOffset = 0;//notice this! stream offset is zero.
					//uncompressed sects the offset is the current file offset, but has 0 here because the stream is a seperate
					//decompressed stream
					currentSect.mStreamSize = currentSect.mpDataStream->GetSize();
				}
				else {
					currentSect.mStreamOffset = offset;
					currentSect.mStreamSize = currentSect.mCompressedSize;
					currentSect.mpDataStream = NULL;
				}
				offset += currentSect.mCompressedSize;
			}
		}
		if (!METASTREAM_ENABLE_DEBUG) {
			//delete debug stream since this version was not built in debug mode, was built in release
			DataStream* debugStream = mSection
				[(int)SectionType::eSection_Debug].mpDataStream;
			if (debugStream)
				delete debugStream;
			mSection[(int)SectionType::eSection_Debug].mpDataStream = NULL;
			mSection[(int)SectionType::eSection_Debug].mStreamOffset = 0;
			mSection[(int)SectionType::eSection_Debug].mStreamSize = 0;
			mSection[(int)SectionType::eSection_Debug].mCompressedSize = 0;
		}
		mCurrentSection = SectionType::eSection_Default;
	}
	else return false;
	return true;
}



u64 MetaStream::Close() {
	if (this->mMode != MetaStreamMode::eMetaStream_Closed) {
		u64 completeStreamSize = 0;
		if (mMode == MetaStreamMode::eMetaStream_Read) {
			completeStreamSize = mSection[0].mStreamSize + mSection[1].mStreamSize + mSection[2].mStreamSize + mSection[3].mStreamSize;
		}
		else if (mMode == MetaStreamMode::eMetaStream_Write) {
			if (!mpWriteStream)return;
			_FinalizeStream();
			_WriteHeader();
			for (int i = (int)SectionType::eSection_Default; i < (int)SectionType::eSection_Count; i++) {
				if(mSection[i].mpDataStream)
					mSection[i].mpDataStream->Transfer(mpWriteStream, 0, mSection[i].mpDataStream->GetSize());
			}
			completeStreamSize = mpWriteStream->GetSize();
		}
		for (int i = 0; i < (int)SectionType::eSection_Count; i++) {
			if (mSection[i].mStreamSize) {
				if (mSection[i].mpDataStream)
					delete mSection[i].mpDataStream;
				mSection[i].mpDataStream = NULL;
				mSection[i].mStreamOffset = 0;
				mSection[i].mStreamPosition = 0;
				mSection[i].mStreamSize = 0;
				mSection[i].mCompressedSize = 0;
				mSection[i].mBlockInfo.clear();
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
				free(i);
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
			SerializedVersionInfo::MemberDesc member{ 0 };
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