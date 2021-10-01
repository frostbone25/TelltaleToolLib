// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _META
#define _META

#include "TelltaleToolLibrary.h"
#include <string>
#include <typeinfo>
#include <vector>
#include <stack>
#include "HashManager.h"

//set to false to disallow the debug section of meta stream files to be loaded. default true
#define METASTREAM_ENABLE_DEBUG true

#define METAOP_FUNC_DEF(_FuncName) static MetaOpResult MetaOperation_##_FuncName(void *,MetaClassDescription*,MetaMemberDescription*,void*);
#define METAOP_FUNC_IMPL(_FuncName) MetaOpResult Meta::MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)

#define METAOP_FUNC_IMPL_(NS,_FuncName) MetaOpResult NS::MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)

struct MetaClassDescription;
struct MetaMemberDescription;
class SerializedVersionInfo;
class Symbol; 

constexpr const char VersionHeaders[][5] = {
	"SEBM",//version 0, meta binary encrypted stream
	"NIBM",//version 0, meta binary
	"ERTM",//version 3, no clue
	"MOCM",//version 3, meta compressed
	"4VSM",//version 4, meta stream version 4
	"5VSM",//version 5
	"6VSM",//version 6
};

static constexpr u32 GetMetaMagic(int versionID) {
	if (versionID >= 0 && versionID <= 6)return *((u32*)VersionHeaders[versionID]);
	return 0;
}

enum MetaOpResult {
	eMetaOp_Fail = 0x0,
	eMetaOp_Succeed = 0x1,
	eMetaOp_Invalid = 0x2,
	eMetaOp_OutOfMemory = 0x3,
	eMetaOp_MAX = 0x4,
};

namespace UID {
	typedef int idT;
	
	struct __declspec(align(8)) Owner {
		idT miUniqueID;
	};

	struct __declspec(align(8)) Generator {
		idT miNextUniqueID;
	};

};

struct T3VertexSampleDataBase {
	//mAllocator, mRefCount
	int mNumVerts;
	int mVertSize;
	char* mpData;

	T3VertexSampleDataBase() : mpData(NULL) {}

	~T3VertexSampleDataBase() {
		if (mpData)
			free(mpData);
	}

	METAOP_FUNC_DEF(SerializeAsync);

};

struct ZTestFunction {
	enum zFuncTypes {
		eNever = 0x1, eLess = 0x2, eEqual = 0x3, eLessEqual = 0x4, eGreater = 0x5, eNotEqual = 0x6, eGreaterEqual = 0x7, eAlways = 0x8,
	};
	zFuncTypes mZTestType;
};

struct Meta {

	//Types
	static void Initialize();
	//Handle<T>
	static void Initialize2();
	//AnimatedValueInterfaceBases
	static void Initialize3();
	//Containers
	static void Initialize4();

	//set the version crc in the serializedversioninfo ( IS IMPLEMENTED, intellisense my ass)
	static MetaOpResult MetaOperation_SerializedVersionInfo(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pContextDesc,
		void* pUserData);

	struct Equivalence {
		bool mbEqual;
		void* mpOther;
	};

	typedef void (*EnumerateMembersFunc)(void*, MetaClassDescription*, MetaMemberDescription*);

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

	METAOP_FUNC_DEF(Equivalence)

	//the goto function!
	METAOP_FUNC_DEF(AsyncSave)

};

const struct MetaStreamParams {
	bool mbCompress;
};

enum class MetaStreamMode {
	eMetaStream_Closed = 0x0, eMetaStream_Read = 0x1, eMetaStream_Write = 0x2,
};

struct MetaVersionInfo {
	u64 mTypeSymbolCrc;
	u32 mVersionCrc;
};

class MetaStream {//can implement child class MetaStream_JSON

public:

	enum class RuntimeFlags {
		eWriteback = 0x1,
		eStreamIsCompiledVersion = 0x2,
		eIsUndo = 0x4
	};

	enum class SectionType {
		eSection_Header = 0x0,
		eSection_Default = 0x1,
		eSection_Debug = 0x2,
		eSection_Async = 0x3, 
		eSection_Count = 0x4
	};

	enum class StreamType {
		eStream_Binary = 0x0,
		eStream_JSON = 0x1
	};

	struct BlockInfo {
		u32 mBlockLength;//or start pos
	};

	struct SectionInfo {
		DataStream* mpStream;//mpDataStream
		u64 mCompressedSize;
		std::stack<BlockInfo> mBlockInfo;//LIFO
		bool mbEnable;
		bool mbCompressed;
		//mReadBuffer, dont need it

		//u64 mStreamSize, mStreamOffset, mStreamPosition;

		SectionInfo() {
			mbEnable = true;
		}

	};

	//normally is separated into SubStreamInfo, each one could be a metastream or just data. for simplicity just done it like this

	SectionInfo mSection[(int)SectionType::eSection_Count];
	std::vector<MetaVersionInfo> mVersionInfo;
	MetaStreamParams mParams;
	int mDebugSectionDepth;
	SectionType mCurrentSection;

	u32 mStreamVersion;
	DataStream* mpReadWriteStream;
	MetaStreamMode mMode;
	//Blowfish* mpBlowfish;
	Flags mRuntimeFlags;//flag values: RuntimeFlags enum
	char mName[260];

	bool mbErrored = false;

	INLINE virtual const char* GetName() { return mName; }
	INLINE virtual MetaStream::StreamType GetStreamType() { return StreamType::eStream_Binary; }
	u64 Close();//returns size of stream
	bool Attach(DataStream*, MetaStreamMode, MetaStreamParams);
	//IMPORTANT: THIS CLASS TAKES *OWNERSHIP* OF THE DATASTREAM AND WILL DELETE IT WHEN DONE!
	void Open(DataStream*, MetaStreamMode, MetaStreamParams);
	void DisableDebugSection();
	u64 GetPartialStreamSize();
	virtual i64 ReadData(void*, u32);
	virtual i64 WriteData(void*, u32);
	virtual bool BeginAsyncSection();
	virtual void EndAsyncSection();
	virtual bool HasAsyncSection();
	virtual bool BeginDebugSection();
	virtual void EndDebugSection();
	virtual bool HasDebugSection();
	virtual u64 GetSize();
	virtual u64 GetPos();
	virtual void SetPos(u64);
	virtual void Advance(int numBytes);
	virtual void BeginBlock();
	virtual void EndBlock();
	virtual void SkipToEndOfCurrentBlock();
	virtual void BeginObject(Symbol*, void*) {};
	virtual void EndObject(Symbol*) {};
	virtual void BeginObject(const char*, void*) {};
	virtual void EndObject(const char*) {};
	virtual i64 BeginAnonObject(void*);
	virtual void EndAnonObject(int id);
	virtual i64 BeginObject(Symbol*, MetaClassDescription*, MetaMemberDescription*);
	virtual void EndObject(Symbol*, MetaClassDescription*, MetaMemberDescription*);
	virtual void SetObjectAsArrayType();
	virtual void AddVersion(const SerializedVersionInfo*);
	virtual MetaVersionInfo* GetStreamVersion(u64 typeSymbolCrc);
	virtual MetaVersionInfo* GetStreamVersion(MetaClassDescription* d);
	virtual void serialize_String(String*);
	virtual void serialize_Symbol(Symbol*);
	virtual void serialize_bool(bool*);
	virtual int serialize_bytes(void*, u32);
	bool _ReadHeader(DataStream* partial, u64, u64* pOutBytesNeeded);
	void _WriteHeader();
	void _FinalizeStream();
	bool _SetSection(SectionType);
	virtual void serialize_double(long double*);
	virtual void serialize_float(float*);
	virtual void serialize_uint16(u16*);
	virtual void serialize_uint32(u32*);
	virtual void serialize_uint64(u64*);
	virtual void serialize_int8(char*);

	MetaStream(const char* Name);
	~MetaStream();

};

typedef void* (*FuncNew)(void);
typedef void (*FuncDestroy)(void*);
typedef void (*FuncDelete)(void*);
typedef void (*FuncCopyConstruct)(void*, void*);
typedef void (*FuncConstruct)(void*);
typedef void (*FuncCastToConcreteObject)(void**, MetaClassDescription**);

extern i32 sMetaTypesCount;

class Symbol {
private:
	static char smSymbolBuffer[sizeof(u64) * 2 + 1];//1byte= 2 hex chars
	u64 mCrc64;
public:

	Symbol(void) : mCrc64(0) {}
	Symbol(char const* pString) {
		mCrc64 = pString ? CRC64_CaseInsensitive(0, pString) : 0;
	}
	Symbol(const String& pString) {
		mCrc64 = CRC64_CaseInsensitive(0, pString.c_str());
	}
	Symbol(u64 crc) : mCrc64(crc) {}

	Symbol& operator=(const Symbol& rhs) {
		this->mCrc64 = rhs.mCrc64;
		return *this;
	}

	INLINE const char* CRCAsCstr() const {//not async
		sprintf(smSymbolBuffer, "%llx", mCrc64);
		return smSymbolBuffer;
	}

	INLINE void SetCRC(u64 crc) { mCrc64 = crc; }

	Symbol& Concat(const String& tName) {
		mCrc64 = CRC64_CaseInsensitive(0, tName.c_str());
		return *this;
	}

	INLINE u64 GetCRC() {
		return mCrc64;
	}

	//AsConcat isnt needed

	INLINE operator const char* () const { return CRCAsCstr(); }

	static MetaOpResult MetaOperation_Equivalence(void* pObj,
		MetaClassDescription* pClassDescription, MetaMemberDescription* pContextDescription,
		void* pUserData) {//userdata=meta::equivalence
		static_cast<Meta::Equivalence*>(pUserData)->mbEqual = static_cast<Symbol*>(pObj)->mCrc64 ==
			static_cast<Symbol*>(static_cast<Meta::Equivalence*>(pUserData)->mpOther)->mCrc64;
		return MetaOpResult::eMetaOp_Succeed;
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pClassDescription,
		MetaMemberDescription* pContextDescription, void* pUserData) {
		//static_cast<MetaStream*>(pUserData)->serialize_Symbol(static_cast<Symbol*>(pObj));---------------------------------------
		return eMetaOp_Succeed;
	}

	static const Symbol EmptySymbol;

};

template<typename T, typename U> constexpr size_t memberOffset(U T::* member)
{
	return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

template<typename T>
struct MetaClassDescription_Typed {

	static MetaClassDescription* GetMetaClassDescription() {
		return GetMetaClassDescription(NULL);
	}

	static MetaClassDescription* GetMetaClassDescription(const char* type);//originally this would be specialized

	static void* New(void) {
		return operator new(sizeof(T));
	}

	static void Destroy(void* pObj) {
		static_cast<T*>(pObj)->~T();
	}

	static void Delete(void* pObj) {
		static_cast<T*>(pObj)->~T();
		operator delete(pObj);
	}

	static void CopyConstruct(void* pDest, void* pSrc) {
		new (pDest) T(*static_cast<T*>(pSrc));
	}

	static void Construct(void* pObj) {
		new (pObj) T();
	}

};

enum VTableFunction {
	eVTableNew = 0,
	eVTableDelete = 1,
	eVTableConstruct = 2,
	eVTableCopyConstruct = 3,
	eVTableDestroy = 4,
	//eVTableCastToConcreteObject = 5,
	eVTableCount = 5//6
};

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

	static SerializedVersionInfo* RetrieveCompiledVersionInfo(MetaClassDescription* pObjDescription);

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

	enum sIDs {
		eMetaOpZero = 0x0,
		eMetaOpOne = 0x1,
		eMetaOpTwo = 0x2,
		eMetaOpThree = 0x3,
		//add to panel
		eMetaOpFour = 0x4,
		eMetaOpFive = 0x5,
		//convert
		eMetaOpSix = 0x6,
		eMetaOpSeven = 0x7,
		eMetaOpEight = 0x8,
		//equivalence
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
		eMetaOpTwentyOne = 0x15,//MetaOperation_SerializedVersionInfo
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
	const char* mpExt;
	const char* mpTypeInfoName;
	u64 mHash;
	Flags mFlags;
	u32 mClassSize;
	SerializedVersionInfo* mpCompiledVersionSerializedVersionInfo;//atomic
	MetaMemberDescription* mpFirstMember;
	MetaOperationDescription* mMetaOperationsList;
	MetaClassDescription* pNextMetaClassDescription;
	void* mpVTable[5/*6*/];
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
	bool IsDerivedFrom(MetaClassDescription* pDesc);
	void InstallSpecializedMetaOperation(MetaOperationDescription*);
	MetaOperation GetOperationSpecialization(int ID);
	//void CastToConcreteObject(void** pObj, MetaClassDescription** pDesc); //doesnt exist
	void* CastToBase(const void* pObj, MetaClassDescription* pBaseClassDesc);

};

extern MetaClassDescription* spFirstMetaClassDescription;

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

MetaSerializeAccel* MetaSerialize_GenerateAccel(MetaClassDescription*);

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
	~MetaMemberDescription();
};

#endif
