// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _META
#define _META

#include <string>
#include <typeinfo>
#include <vector>
#include "MetaStream.h"

enum MetaFlag {
	MetaFlag_MetaSerializeDisable = 1,
	MetaFlag_MetaSerializeBlockingDisabled = 2,
	MetaFlag_PlaceInAddPropMenu = 4,
	MetaFlag_NoPanelCaption = 8,
	MetaFlag_BaseClass = 0x10,
	MetaFlag_EditorHide = 0x20,
	MetaFlag_EnumIntType = 0x40,
	MetaFlag_EnumStringType = 0x80,
	MetaFlag_ContainerType = 0x100,
	MetaFlag_ScriptEnum = 0x200,
	MetaFlag_Heap = 0x400,
	MetaFlag_ScriptTransient = 0x800,
	MetaFlag_SelectAgentType = 0x1000,
	MetaFlag_SkipObjectState = 0x2000,
	MetaFlag_NotCacheable = 0x4000,
	MetaFlag_EnumWrapperClass = 0x8000,
	MetaFlag_TempDescription = 0x10000,
	MetaFlag_Handle = 0x20000,
	MetaFlag_FlagType = 0x40000,
	MetaFlag_SelectFolderType = 0x80000,
	MetaFlag_Memberless = 0x100000,
	MetaFlag_RenderResource = 0x200000,
	MetaFlag_MetaSerializeNonBlockedVariableSize = 0x400002,
	MetaFlag_EmbeddedCacheableResource = 0x800000,
	MetaFlag_VirtualResource = 0x1000000,
	MetaFlag_DontAsyncLoad = 0x2000000,
	MetaFlag_IsNotMetaFile = 0x4000000,
	Internal_MetaFlag_Initialized = 0x20000000,
};

/*
* 
00000050 ReadData        dq ? ; offset 80
00000058 WriteData       dq ? ; offset 88
00000060 ReadDataStream  dq ? ; offset 96
00000068 BeginAsyncSection dq ? ; offset 104
00000070 EndAsyncSection dq ? ; offset 112
00000078 HasAsyncSection dq ? ; offset 120
00000080 BeginDebugSection dq ? ; offset 128
00000088 EndDebugSection dq ? ; offset 136
00000090 HasDebugSection dq ? ; offset 144
00000098 GetSize         dq ? ; offset 152
000000A0 GetPos          dq ? ; offset 160
000000A8 SetPos          dq ? ; offset 168
000000B0 Advance         dq ? ; offset 176
000000B8 BeginBlock      dq ? ; offset 184
000000C0 EndBlock        dq ? ; offset 192
000000C8 SkipToEndOfCurrentBlock dq ? ; offset 200
000000D0 BeginObject     dq ? ; offset 208
000000E8 EndObject       dq ? ; offset 232
00000100 BeginAnonObject dq ? ; offset 256
00000108 EndAnonObject   dq ? ; offset 264
00000110 SetObjectAsArrayType dq ? ; offset 272
00000118 AddVersion      dq ? ; offset 280
00000120 serialize_float dq ? ; offset 288
00000128 serialize_double dq ? ; offset 296
00000130 serialize_int   dq ? ; offset 304
00000138 serialize_uint  dq ? ; offset 312
00000140 serialize_int8  dq ? ; offset 320
00000148 serialize_uint8 dq ? ; offset 328
00000150 serialize_int16 dq ? ; offset 336
00000158 serialize_uint16 dq ? ; offset 344
00000160 serialize_int32 dq ? ; offset 352
00000168 serialize_uint32 dq ? ; offset 360
00000170 serialize_int64 dq ? ; offset 368
00000178 serialize_uint64 dq ? ; offset 376
00000180 serialize_String dq ? ; offset 384
00000188 serialize_Symbol dq ? ; offset 392
00000190 serialize_bool  dq ? ; offset 400
00000198 serialize_bytes dq ? ; offset 408*/

enum MetaOpResult {
	eMetaOp_Fail = 0x0,
	eMetaOp_Succeed = 0x1,
	eMetaOp_Invalid = 0x2,
	eMetaOp_OutOfMemory = 0x3,
	eMetaOp_MAX = 0x4,
};

struct SerializedVersionInfo {

	struct MemberDesc {
		String mName;
		String mTypeName;
		u64 mTypeNameSymbolCrc;
		bool mbBlocked;
		u32 mSize;
		u32 mVersionCrc;
	};

	String mFileName;
	u64 mTypeSymbolCrc;
	u32 mVersionCrc;
	u32 mSize;
	bool mbBlocked;
	std::vector<MemberDesc> mMembers;//DCArrayNM<MemberDesc> mMembers;
};

struct MetaMemberDescription;
struct MetaClassDescription;

struct MetaSerializeAccel {
	MetaOpResult(__cdecl* mpFunctionAsync)(void*, MetaClassDescription*,
		MetaMemberDescription*, void*); 
	MetaOpResult(__cdecl* mpFunctionMain)(void*, MetaClassDescription*, 
		MetaMemberDescription*, void*);
	MetaMemberDescription* mpMemberDesc;
};

typedef MetaOpResult(*MetaOperation)(void*, MetaClassDescription*, MetaMemberDescription*, void*);

struct MetaOperationDescription {
	u32 id;
	MetaOperation mpOpFn;
	MetaOperationDescription* mpNext;
};

struct MetaClassDescription {
	const char* mpExt;
	const char* mpTypeInfoName;
	u64 mHash;
	Flags mFlags;
	u32 mClassSize;
	SerializedVersionInfo* mpCompiledVersionSerializedVersionInfo;//atomic
	MetaMemberDescription* mpFirstMember;
	MetaOperationDescription* mMetaOperationsList; 
	MetaClassDescription* pNextMetaClassDescription; 
	//const void** mpVTable; 
	MetaSerializeAccel* mpSerializeAccel;//atomic
};

struct MetaEnumDescription {
	const char* mpEnumName;
	i32 mFlags;
	i32 mEnumIntValue;
	MetaEnumDescription* mpNext;
};

struct MetaFlagDescription {
	const char* mpFlagName;
	i32 mFlagValue;
	MetaFlagDescription* mpNext;
};

struct MetaMemberDescription {
	const char* mpName;
	i64 mOffset;
	i32 mFlags;
	MetaClassDescription* mpHostClass;
	MetaMemberDescription* mpNextMember;
	union {
		MetaEnumDescription* mpEnumDescriptions;
		MetaFlagDescription* mpFlagDescriptions;
	};
	MetaClassDescription* mpMemberDesc;
};

namespace Meta {

	//set the version crc in the serializedversioninfo
	static MetaOpResult MetaOperation_SerializedVersionInfo(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pContextDesc,
		void* pUserData) {
		//if pObj, warn its not needed in the call
		SerializedVersionInfo* ver = static_cast<SerializedVersionInfo*>(pUserData);
		if (ver) {
			if (pObjDescription->mFlags.mFlags & MetaFlag::MetaFlag_MetaSerializeDisable || 
				pContextDesc && pContextDesc->mFlags & MetaFlag::MetaFlag_MetaSerializeDisable	)
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

	struct Equivalence {
		bool mbEqual;
		void* mpOther;
	};

	struct ConvertFromInfo {
		const void* mpFromObject;
		MetaClassDescription* mpFromObjDescription;
	};

	static MetaOpResult MetaOperation_ConvertFrom(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription *pContextDescription,
		void * pUserData);

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pCtxDesc, void* pUserData);

	static MetaOpResult MetaOperation_Invalid(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pContextDescription,
		void* pUserData) {
		return MetaOpResult::eMetaOp_Invalid;
	}

	static MetaOpResult MetaOperation_SerializeMain(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pCtxDesc, void* pUserData);


};

namespace MetaHelpers {

	template<typename T>
	MetaClassDescription* GetMetaClassDescription() {
		return NULL;
	}

}









#endif
