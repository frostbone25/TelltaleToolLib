// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.


#include "Meta.h"

#define DEFINET(name) }static MetaClassDescription meta_##name; \
if(!(meta_##name.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){

void Meta::Initialize() {{
	DEFINET(char)
	meta_char.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
	meta_char.Initialize("int8");
	meta_char.mClassSize = 1;
	meta_char.Insert();
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

void MetaClassDescription::CastToConcreteObject(void** pObj, MetaClassDescription** pDesc) {
	if (this->mpVTable) {
		void(* func)(void**, MetaClassDescription**);
		func = (void(*)(void**, MetaClassDescription**))this->mpVTable[5];
		if (func)
			func(pObj, pDesc);
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
		free(mpSerializeAccel);
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
	EnumerateMembersInfo* handle = static_cast<EnumerateMembersInfo*>(pUserData);
	if (!handle)return MetaOpResult::eMetaOp_Fail;
	for (MetaMemberDescription* member = pObjDescription->mpFirstMember; member; member = member->mpNextMember) {
		handle->mpFunc(&((char*)pObj)[member->mOffset], member->mpMemberDesc, member);
	}
	return MetaOpResult::eMetaOp_Succeed;
}

METAOP_FUNC_IMPL(GetObjectName) {
	static String szConstObjName("T3 Object");
	String* pDestStr = static_cast<String*>(pUserData);
	if (pDestStr)
		pDestStr->operator=(szConstObjName.c_str());
	return MetaOpResult::eMetaOp_Succeed;
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