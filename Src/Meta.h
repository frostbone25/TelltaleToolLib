// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _META
#define _META

#include <string>
#include <typeinfo>
#include <vector>
#include "HandleObjectInfo.h"
#include "MetaStream.h"

#define METAOP_FUNC_DEF(_FuncName) static MetaOpResult MetaOperation_##_FuncName(void *,MetaClassDescription*,MetaMemberDescription*,void*);
#define METAOP_FUNC_IMPL(_FuncName) static MetaOpResult Meta::MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)


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

typedef void* (*FuncNew)(void);
typedef void (*FuncDestroy)(void*);
typedef void (*FuncDelete)(void*);
typedef void (*FuncCopyConstruct)(void*, void*);
typedef void (*FuncConstruct)(void*);
typedef void (*FuncCastToConcreteObject)(void**, MetaClassDescription**);

struct MetaSerializeAccel {
	MetaOpResult(__cdecl* mpFunctionAsync)(void*, MetaClassDescription*,
		MetaMemberDescription*, void*); 
	MetaOpResult(__cdecl* mpFunctionMain)(void*, MetaClassDescription*, 
		MetaMemberDescription*, void*);
	MetaMemberDescription* mpMemberDesc;
};

typedef MetaOpResult(*MetaOperation)(void*, MetaClassDescription*, MetaMemberDescription*, void*);

struct MetaOperationDescription {

	enum sIDs {
		eMetaOpZero = 0x0,
		eMetaOpOne = 0x1,
		eMetaOpTwo = 0x2,
		eMetaOpThree = 0x3,
		eMetaOpFour = 0x4,
		eMetaOpFive = 0x5,
		eMetaOpSix = 0x6,
		eMetaOpSeven = 0x7,
		eMetaOpEight = 0x8,
		eMetaOpNine = 0x9,
		eMetaOpTen = 0x0A,
		eMetaOpEleven = 0x0B,
		eMetaOpTwelve = 0x0C,
		eMetaOpThirteen = 0x0D,
		eMetaOpFourteen = 0x0E,
		eMetaOpFifteen = 0x0F,
		eMetaOpSixteen = 0x10,
		eMetaOpSeventeen = 0x11,
		eMetaOpEighteen = 0x12,
		eMetaOpNineteen = 0x13,
		eMetaOpTwenty = 0x14,
		eMetaOpTwentyOne = 0x15,
		eMetaOpTwentyTwo = 0x16,
		eMetaOpTwentyThree = 0x17,
		eMetaOpTwentyFour = 0x18,
		eMetaOpTwentyFive = 0x19,
		eMetaOpTwentySix = 0x1A,
		eMetaOpTwentySeven = 0x1B,
		eMetaOpTwentyEight = 0x1C,
		eMetaOpTwentyNine = 0x1D,
		eMetaOpThirty = 0x1E,
		eMetaOpThirtyOne = 0x1F,
		eMetaOpThirtyTwo = 0x20,
		eMetaOpThirtyThree = 0x21,
		eMetaOpThirtyFour = 0x22,
		eMetaOpThirtyFive = 0x23,
		eMetaOpThirtySix = 0x24,
		eMetaOpThirtySeven = 0x25,
		eMetaOpThirtyEight = 0x26,
		eMetaOpThirtyNine = 0x27,
		eMetaOpForty = 0x28,
		eMetaOpFortyOne = 0x29,
		eMetaOpFortyTwo = 0x2A,
		eMetaOpFortyThree = 0x2B,
		eMetaOpFortyFour = 0x2C,
		eMetaOpFortyFive = 0x2D,
		eMetaOpFortySix = 0x2E,
		eMetaOpFortySeven = 0x2F,
		eMetaOpFortyEight = 0x30,
		eMetaOpFortyNine = 0x31,
		eMetaOpFifty = 0x32,
		eMetaOpFiftyOne = 0x33,
		eMetaOpFiftyTwo = 0x34,
		eMetaOpFiftyThree = 0x35,
		eMetaOpFiftyFour = 0x36,
		eMetaOpFiftyFive = 0x37,
		eMetaOpFiftySix = 0x38,
		eMetaOpFiftySeven = 0x39,
		eMetaOpFiftyEight = 0x3A,
		eMetaOpFiftyNine = 0x3B,
		eMetaOpSixty = 0x3C,
		eMetaOpSixtyOne = 0x3D,
		eMetaOpSixtyTwo = 0x3E,
		eMetaOpGetAssetCost = 0x3F,
		eMetaOpFromUiString = 0x40,
		eMetaOpToUiString = 0x41,
		eMetaOpWidgetFromClass = 0x42,
		eMetaOpUserOpen = 0x43,
		eMetaOpSave = 0x44,
		eMetaOpGetVersion = 0x45,
		eMetaOpCopy = 0x46,
		eMetaOpDelete = 0x47,
		eMetaOpAddToCache = 0x48,
		eMetaOpRemoveFromCache = 0x49,
		eMetaOpSerializeAsync = 0x4A,
		eMetaOpSerializeMain = 0x4B,
		eMetaOpDestroy = 0x4C,
		eMetaOpGetEstimatedVramUsage = 0x4D,
		eMetaOpEnumerateMembers = 0x4E,
		eMetaOpSetupPropertyValue = 0x4F,
		eMetaOpSavePropertyValue = 0x50,
		eMetaOpRecursiveVersionCRC = 0x51,
		eMetaOpNewResource = 0x52,
		eNumMetaOps = 0x53,
	};

	sIDs id;
	MetaOperation mpOpFn;
	MetaOperationDescription* mpNext;
};

struct MetaClassDescription {
	static MetaClassDescription* spFirstMetaClassDescription;
	const char* mpExt;
	const char* mpTypeInfoName;
	u64 mHash;
	Flags mFlags;
	u32 mClassSize;
	SerializedVersionInfo* mpCompiledVersionSerializedVersionInfo;//atomic
	MetaMemberDescription* mpFirstMember;
	MetaOperationDescription* mMetaOperationsList; 
	MetaClassDescription* pNextMetaClassDescription; 
	const void** mpVTable; 
	MetaSerializeAccel* mpSerializeAccel;//atomic

	String* GetToolDescriptionName(String* result);
	void Delete(void* pObj);
	void Destroy(void* pObj);
	void* New();
	void Construct(void*);
	void CopyConstruct(void*, void*);
	~MetaClassDescription();
	bool MatchesHash(u64 hash);
	void GetDescriptionSymbol(Symbol*);
	MetaMemberDescription* GetMemberDescription(const char* memberName);
	INLINE MetaMemberDescription* GetMemberDescription(String* _Str) {
		return GetMemberDescription(_Str->c_str());
	}
	INLINE void Initialize(std::type_info* info) {
		Initialize(info->name());
	}
	void Initialize(const char*);
	void Insert();

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
		void* pUserData);

	struct Equivalence {
		bool mbEqual;
		void* mpOther;
	};

	typedef void (*EnumerateMembersFunc)(void*,MetaClassDescription*, MetaMemberDescription*);

	struct EnumerateMembersInfo {
		EnumerateMembersFunc mpFunc;
		std::vector<void*> mArgs;//DCArray<void*>
	};

	struct ConvertFromInfo {
		const void* mpFromObject;
		MetaClassDescription* mpFromObjDescription;
	};

	METAOP_FUNC_DEF(GetObjectName)

	METAOP_FUNC_DEF(EnumerateMembers)

	METAOP_FUNC_DEF(Destroy)

	METAOP_FUNC_DEF(SerializeAsync)

	METAOP_FUNC_DEF(SerializeMain)

	//the goto function!
	METAOP_FUNC_DEF(AsyncSave)

	static INLINE MetaOpResult MetaOperation_Invalid(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pContextDescription,
		void* pUserData) {
		return MetaOpResult::eMetaOp_Invalid;
	}
	

};

namespace MetaHelpers {

	template<typename T>
	MetaClassDescription* GetMetaClassDescription() {
		return NULL;
	}

}









#endif
