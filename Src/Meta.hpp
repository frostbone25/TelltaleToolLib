// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _META
#define _META

#include "TelltaleToolLibrary.h"
#include <string>
#include <math.h>
#include <typeinfo>
#include <vector>
#include <stack>
#include "Types/TRange.h"
#include "HashManager.h"
#include "HashDB/HashDB.h"

//set to false to disallow the debug section of meta stream files to be loaded. default true
#define METASTREAM_ENABLE_DEBUG true
//Default to false, if true then data will be written to the debug section. The debug section is never read in the release
//mode of games (so all telltale games) so this does not really affect anything.
#define METASTREAM_ENABLE_DEBUGSECTION_WRITE false

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 0
#elif defined(_MSC_VER) 
#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 0
#else
#error "Could not detect endian"
#endif

#define METAOP_FUNC_DEF(_FuncName) MetaOpResult MetaOperation_##_FuncName(void *,MetaClassDescription*,MetaMemberDescription*,void*);
#define METAOP_FUNC_IMPL(_FuncName) MetaOpResult Meta::MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)

#define METAOP_FUNC_IMPL_(NS,_FuncName) MetaOpResult NS::MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)

#define METAOP_FUNC_IMPL__(_FuncName) MetaOpResult MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)

struct MetaClassDescription;
struct MetaMemberDescription;
class Symbol; 

struct EnumBase {};

const u32 EncrypedVersionHeaders[] = {
	0x64AFDEAA,//version 3
	0x64AFDEBB,//version 3 (encrypted MCOM)
	0xFB4A1764,//version 2
	0xEB794091,//version 2
	0x64AFDEFB,//version 2
};

//reason they are reversed is because in the executables these are stored as INTS and because of the endian they are reversed
constexpr const char VersionHeaders[][5] = {
#if LITTLE_ENDIAN == 1
	"SEBM",//version 0, Meta Binary Encrypted Stream
	"NIBM",//version 0, Meta BINary
	"ERTM",//version 3, Meta Type REference (I think, more like meta try reverse engineer (assholes, god im lonely)) maybe MT in MTRE is meta
	"MOCM",//version 3, Meta COMpressed, compressed version of MTRE. this might be wrong, im thinking its more meta communication?
	//maybe for example transporting stuff in the engine?
	"4VSM",//version 4, Meta Stream Version 4
	"5VSM",//version 5
	"6VSM",//version 6
#else
	"MBES",//version 0, Meta Binary Encrypted Stream
	"MBIN",//version 0, Meta BINary
	"MTRE",//version 3, no clue
	"MCOM",//version 3, Meta COMpressed
	"MSV4",//version 4, Meta Stream Version 4
	"MSV5",//version 5
	"MSV6",//version 6
#endif
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

	static SerializedVersionInfo* RetrieveCompiledVersionInfo(MetaClassDescription* pObjDescription);

	//Originally would save to <Tool>/Meta/<file> This saves in .vers format. This writes everything (including header).
	//Vers file (serialized versions) names are in the format %s1(%s2).vers , where %s1 is the type name, 
	//and %s2 is the base 36 of the version CRC. Returns a datastream pointer, which you need to delete
	DataStream* Save(const char* versName);

	//TAKES OWNERSHIP OF STREAM PARAM, IT WILL BE DELETED AFTER USE
	//Loads this version information from a .vers.
	//There should be three parameters (type name crc, version crc) and the engine would load the .vers from that
	void RetrieveVersionInfo(/*u64 typeNameCrc, u32 typeVersionCrc, */ const char* versFileName, DataStream* stream);

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
	eMetaOp_SymbolNotFoundInDB = 0x4,
	eMetaOp_MAX = 0x5,//normally 4
};

namespace UID {
	typedef int idT;

	struct __declspec(align(8)) Generator {
		static constexpr idT msUninitID = -1;
		idT miNextUniqueID;

		Generator() : miNextUniqueID(1) {}

		static idT UninitID() {
			return 0xFFFFFFFF;
		}

		inline void Reset() {
			miNextUniqueID = 1;
		}

		inline bool ValidID(idT id) {
			return id >= 1 && id < this->miNextUniqueID;
		}

		inline idT GetNextUniqueID(bool bIncrement) {
			idT _t = miNextUniqueID;
			if (miNextUniqueID == 0xFFFFFFFF) {
				miNextUniqueID = 1;
				_t = 1;
			}
			else if (bIncrement)
				miNextUniqueID++;
			return _t;
		}
	};

	struct __declspec(align(8)) Owner {
		idT miUniqueID;

		Owner() : miUniqueID(-1) {}

		Owner(Generator& gen) {
			miUniqueID = gen.GetNextUniqueID(true);
		}

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

	static METAOP_FUNC_DEF(SerializeAsync);

};

constexpr const char* spVersNameFormat = "%s(%s).vers";

struct ZTestFunction {
	enum zFuncTypes {
		eNever = 0x1, eLess = 0x2, eEqual = 0x3, eLessEqual = 0x4, eGreater = 0x5, eNotEqual = 0x6, eGreaterEqual = 0x7, eAlways = 0x8,
	};
	zFuncTypes mZTestType;
};

struct Color {
	float r, g, b, a;
};

struct ColorHDR {
	float r, g, b, intensity;
};

struct Meta {

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

	static METAOP_FUNC_DEF(GetObjectName)

	static METAOP_FUNC_DEF(EnumerateMembers)

	static METAOP_FUNC_DEF(Destroy)

	static METAOP_FUNC_DEF(SerializeAsync)

	static METAOP_FUNC_DEF(SerializeMain)

	static METAOP_FUNC_DEF(Equivalence)

	static METAOP_FUNC_DEF(AsyncSave)

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

//If you have opened and it errored on Open, then you have to reset the input stream to its offset when first passed into Open,
//can implement child class MetaStream_JSON
class MetaStream {

public:

	enum class RuntimeFlags {
		eWriteback = 0x1,
		eStreamIsCompiledVersion = 0x2,
		eIsUndo = 0x4
	};

	enum SectionType : int {
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

	struct WriteParams {//not in telltale tool, used for this library to write meta streams
		bool mbEncryptStream : 1;
		u8 mEncryptVersion : 7;//1 2 or 3
		bool mbSerializeAsCompressVersion;//MCOM. not supported (unknown int, and no files of this type in public, must be engine private).
		
		WriteParams() : mbEncryptStream(false), mEncryptVersion(3), mbSerializeAsCompressVersion(false) {}

	};

	//normally is separated into SubStreamInfo, each one could be a metastream or just data. for simplicity just done it like this

	SectionInfo mSection[(int)SectionType::eSection_Count];
	std::vector<MetaVersionInfo> mVersionInfo;
	MetaStreamParams mParams;
	int mDebugSectionDepth;
	SectionType mCurrentSection;
	WriteParams mWriteParams;

	struct MissingMemberCallbackInfo {
		bool(*mMissingMemberCallback)(SerializedVersionInfo::MemberDesc*, void*);
		void* mpUserData;
	};

	std::vector<MissingMemberCallbackInfo> mMissingMemberCallbacks;

	/*
	* Stream Versions:
	* 1: MBIN (and MBES if encrypted)
	* 2: Encrypted MBIN (weird headers)
	* 3: MTRE (if the meta stream header is not MTRE its a weird encrypted header)
	* 4: MSV4 - No default section, just async and debug
	* 5: MSV5 
	* 6: MSV6
	*/
	u32 mStreamVersion;
	DataStream* mpReadWriteStream;
	MetaStreamMode mMode;
	//Blowfish* mpBlowfish;
	Flags mRuntimeFlags;//flag values: RuntimeFlags enum
	//char mName[260];

	bool mbDontDeleteStream = false;//by lib

	bool mbErrored = false;

	INLINE virtual MetaStream::StreamType GetStreamType() { return StreamType::eStream_Binary; }
	u64 Close();//returns size of stream
	bool Attach(DataStream*, MetaStreamMode, MetaStreamParams);
	//IMPORTANT: THIS CLASS TAKES *OWNERSHIP* OF THE DATASTREAM AND WILL DELETE IT WHEN DONE!
	void Open(DataStream*, MetaStreamMode, MetaStreamParams);
	void DisableDebugSection();
	u64 GetPartialStreamSize();
	void PushMissingMemberCallback(bool(*cb)(
		SerializedVersionInfo::MemberDesc*, void*),
	void* mpUserData);
	void PopMissingMemberCallback();
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
	virtual void BeginObject(void*) {}
	virtual void EndObject(void*) {}
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

	virtual void SwitchToMode(MetaStreamMode newMode,DataStream* inputOrOutputStream);//by lib, a useful function

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

	struct CompareCRC {
		constexpr bool operator()(const Symbol& _Left, const Symbol& _Right) const {
			return _Left.mCrc64 < _Right.mCrc64;
		}
	};

	Symbol(void) : mCrc64(0) {}
	Symbol(char const* pString) {
		mCrc64 = pString ? CRC64_CaseInsensitive(0, pString) : 0;
	}
	Symbol(const String& pString) {
		mCrc64 = CRC64_CaseInsensitive(0, pString.c_str());
	}

	constexpr Symbol(u64 crc) : mCrc64(crc) {}

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
		mCrc64 = CRC64_CaseInsensitive(mCrc64, tName.c_str());
		return *this;
	}

	INLINE u64 GetCRC() const {
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
		static_cast<MetaStream*>(pUserData)->serialize_Symbol(static_cast<Symbol*>(pObj));
		return eMetaOp_Succeed;
	}

	static const Symbol kEmptySymbol;
	static const Symbol kTransitionMapKey;
	static const Symbol kAnimGroupArmLeft;
	static const Symbol kAnimGroupArmRight;
	static const Symbol kAnimGroupBodyLower;
	static const Symbol kAnimGroupBodyUpper;
	static const Symbol kAnimGroupBrowLeft;
	static const Symbol kAnimGroupBrowRight;
	static const Symbol kAnimGroupBrows;
	static const Symbol kAnimGroupCheekLeft;
	static const Symbol kAnimGroupCheekRight;
	static const Symbol kAnimGroupCheeks;
	static const Symbol kAnimGroupExtra;
	static const Symbol kAnimGroupEyeLids;
	static const Symbol kAnimGroupEyes;
	static const Symbol kAnimGroupFace;
	static const Symbol kAnimGroupFingersLeft;
	static const Symbol kAnimGroupFingersRight;
	static const Symbol kAnimGroupFootLeft;
	static const Symbol kAnimGroupFootRight;
	static const Symbol kAnimGroupHair;
	static const Symbol kAnimGroupHandRight;
	static const Symbol kAnimGroupHandLeft;
	static const Symbol kAnimGroupHead;
	static const Symbol kAnimGroupIndexFLeft;
	static const Symbol kAnimGroupIndexFRight;
	static const Symbol kAnimGroupLegLeft;
	static const Symbol kAnimGroupLegRight;
	static const Symbol kAnimGroupMouth;
	static const Symbol kAnimGroupNeck;
	static const Symbol kAnimGroupRoot;
	static const Symbol kAnimGroupShoulderLeft;
	static const Symbol kAnimGroupShoulderRight;
	static const Symbol kAnimGroupGroupSkirt;
	static const Symbol kAnimGroupGroupTail;
	static const Symbol kAnimGroupThumbLeft;
	static const Symbol kAnimGroupThumbRight;

	static const Symbol kPropKeyViewportOrigin;
	static const Symbol kPropKeyViewportSize;
	static const Symbol kPropKeyViewportCamera;
	static const Symbol kPropKeyViewportRelative;
	static const Symbol kPropKeyAllowInterpolation;
	static const Symbol kBGMStartPos;

	static const Symbol sAssertSymbol;
	static const Symbol sAssertTimeStampSymbol;
	static const Symbol sAssertFileSymbol;
	static const Symbol sAssertLineSymbol;
	static const Symbol sGeneralSymbol;
	static const Symbol sChoreSymbol;
	static const Symbol sLuaSymbol;
	static const Symbol sGraphicsSymbol;
	static const Symbol sDialogSymbol;
	static const Symbol sAudioSymbol;
	static const Symbol sQtSymbol;//telltale use qt in the tool!!!
	static const Symbol sExportSymbol;
	static const Symbol sCoreSymbol;

	static const Symbol kPropKeySource;
	static const Symbol kPropKeyWidth;
	static const Symbol kPropKeyHeight;
	static const Symbol kPropKeyPause;
	static const Symbol kPropKeyVolume;
	static const Symbol kPropKeyMediaTimeout;
	static const Symbol kPropKeyVolumeMaster;
	static const Symbol kPropKeyVolumeMusic;
	static const Symbol kPropKeyVolumeSound;
	static const Symbol kPropKeyVolumeVoice;
	static const Symbol kPropKeyMuteVoices;
	static const Symbol kPropKeyVolumeAmbient;
	static const Symbol kPropKeyMixMaster;
	static const Symbol kPropKeyMixMusic;
	static const Symbol kPropKeyMixSound;
	static const Symbol kPropKeyMixVoice;
	static const Symbol kPropKeyMixAmbient;
	static const Symbol kPropKeySoundMode;
	static const Symbol kCompressAnimations;
	static const Symbol kEncryptMetaStreams;
	static const Symbol kPropKeyROMRuntimeLoad;
	static const Symbol kPropKeyROMErrorDialogResource;
	static const Symbol kPropKeyROMErrorTitle;
	static const Symbol kPropKeyROMErrorMessage;
	static const Symbol kPropKeyROMPackSubDir;
	static const Symbol kPropKeyTextMinSec;
	static const Symbol kPropKeyTextSecPerLetter;
	static const Symbol kPropKeyUserSpaceAsArchive;
	static const Symbol kPropKeyDisableDevFolders;
	static const Symbol kPropKeyUsageOS;
	static const Symbol kPropKeyUsageCPU;
	static const Symbol kPropKeyUsageGPU;
	static const Symbol kPropKeyUsageDeviceType;
	static const Symbol kPropKeyUsageDeviceModel;
	static const Symbol kPropKeyUsageDeviceName;
	static const Symbol kPropKeyUsageLaunchCount;
	static const Symbol kPropKeyUsageShutdownCount;
	static const Symbol kPropKeyUsageSessionExecutionTime;
	static const Symbol kPropKeyUsageTotalExecutionTime;
	static const Symbol kChoreAgentGroupFilterIncludeNonSkeleton;
	static const Symbol kPropOpenLocalizations;
	static const Symbol kHDScreenShotResolution;
	static const Symbol kBlocking;
	static const Symbol kBlockingOffset;
	static const Symbol kBlockingOffsetContribution;
	static const Symbol kResourceNameSuffix;
	static const Symbol kLookAtBlockingAnimation;
	static const Symbol kLookAtBlockingOffsetAnimation;
	static const Symbol kLookAtBlockingOffsetContributionAnimation;

};

template<typename T, typename U> constexpr size_t memberOffset(U T::* member)
{
	return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

template<typename Base, typename Derived> constexpr size_t parentOffset() {
	return (reinterpret_cast<char*>(static_cast<Base*>(reinterpret_cast<Derived*>(0x10000000))) - reinterpret_cast<char*>(0x10000000));
}

MetaClassDescription* GetMetaClassDescription(const char* typeInfoName);

//would have used to be inclined getmetaaclassdescription functions, this would have had template specializations for all types.
template<typename T>
struct MetaClassDescription_Typed {

	static MetaClassDescription* GetMetaClassDescription() {
		return GetMetaClassDescription(NULL);
	}

	static MetaClassDescription* GetMetaClassDescription
	(const char* type);//originally this would be specialized

	static void* New(void) {
		void* ptr = operator new(sizeof(T));
		new (ptr) T();
		return ptr;
	}

	static void Destroy(void* pObj) {
		static_cast<T*>(pObj)->~T();
	}

	static void Construct(void* pObj) {
		new(pObj) T();
	}

	static void CopyConstruct(void* pDest, void* pSrc) {
		Construct(pDest);
		*static_cast<T*>(pDest) = *static_cast<T*>(pSrc);
	}

	static void Delete(void* pObj) {
		static_cast<T*>(pObj)->~T();
		operator delete(pObj);
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

enum MetaFlag : int {
	//Dont serialize this type
	MetaFlag_MetaSerializeDisable = 1,
	//Dont put a block size infront of this type
	MetaFlag_MetaSerializeBlockingDisabled = 2,
	//Place in game engine property menu
	MetaFlag_PlaceInAddPropMenu = 4,
	//No caption in the panel its in
	MetaFlag_NoPanelCaption = 8,
	//This type is a base class
	MetaFlag_BaseClass = 0x10,
	//Dont show this in the game engine editor
	MetaFlag_EditorHide = 0x20,
	//Is an enum, of type int
	MetaFlag_EnumIntType = 0x40,
	//Is an enum of type string
	MetaFlag_EnumStringType = 0x80,
	//Is a container type (eg a map, list, array)
	MetaFlag_ContainerType = 0x100,
	//Is a script enum type (enum used in lua scripts)
	MetaFlag_ScriptEnum = 0x200,
	//The name of this type (in meta member descriptions) is allocated on the heap
	MetaFlag_Heap = 0x400,
	//Serialized or created from lua scripts
	MetaFlag_ScriptTransient = 0x800,
	//Not seen this used yet
	MetaFlag_SelectAgentType = 0x1000,
	//Not seen this used yet, just object state is a meta operation
	MetaFlag_SkipObjectState = 0x2000,
	//Not seen this used yet
	MetaFlag_NotCacheable = 0x4000,
	//This type wraps an enum. Eg, this type is a struct EnumPlatformType{PlatformType mType}, where PlatformType is an enum
	//Reason its like this is because the enumplatformtype struct inherits from EnumBase and that has a seperate description
	MetaFlag_EnumWrapperClass = 0x8000,
	//Not seen this used yet
	MetaFlag_TempDescription = 0x10000,
	//This type is a handle (reference to a file). If serialized this is a CRC
	MetaFlag_Handle = 0x20000,
	//This type has a list of flags present with it (eg FlagsT3LightEnvGroupSet)
	MetaFlag_FlagType = 0x40000,
	//Not seen this used yet
	MetaFlag_SelectFolderType = 0x80000,
	//This type has no members, so doesnt need to be serialized
	MetaFlag_Memberless = 0x100000,
	//This type is a renderable resource (eg a texture)
	MetaFlag_RenderResource = 0x200000,
	//If this type has a block around it but the size of the serialized version is not always one value
	MetaFlag_MetaSerializeNonBlockedVariableSize = 0x400002,
	//Not seen this used yet
	MetaFlag_EmbeddedCacheableResource = 0x800000,
	//Not seen this used yet
	MetaFlag_VirtualResource = 0x1000000,
	//Not seen this used yet
	MetaFlag_DontAsyncLoad = 0x2000000,
	//If this type is not a meta file
	MetaFlag_IsNotMetaFile = 0x4000000,
	//If this type has been initialized
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

struct MetaMemberDescription;
struct MetaClassDescription;

struct MetaSerializeAccel {

	MetaSerializeAccel() : mpFunctionMain(NULL), mpFunctionAsync(NULL), mpMemberDesc(NULL) {}

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
		eMetaOpTwo = 0x2,//AddToChore
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
		eMetaOpEleven = 0x0B,//GetLength
		eMetaOpTwelve = 0x0C,
		eMetaOpThirteen = 0x0D,
		eMetaOpFourteen = 0x0E,
		eMetaOpFifteen = 0x0F,//ObjectState
		eMetaOpSixteen = 0x10,//OpenEditor
		eMetaOpSeventeen = 0x11,
		eMetaOpEighteen = 0x12,//ScriptLock
		eMetaOpNineteen = 0x13,//ScriptUnlock
		eMetaOpTwenty = 0x14,
		eMetaOpTwentyOne = 0x15,//SerializedVersionInfo
		eMetaOpTwentyTwo = 0x16,//SetObjectName
		eMetaOpTwentyThree = 0x17,//ToString
		eMetaOpTwentyFour = 0x18,//GetName
		eMetaOpTwentyFive = 0x19,
		eMetaOpTwentySix = 0x1A,
		eMetaOpTwentySeven = 0x1B,
		eMetaOpTwentyEight = 0x1C,//CollectTyped
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
		eMetaOpFiftyFour = 0x36,//PreloadDependentResources
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
	const char* mpTypeInfoExternalName;//for lib
	u64 mHash;
	Flags mFlags;
	u32 mClassSize;
	SerializedVersionInfo* mpCompiledVersionSerializedVersionInfo;//atomic, lazily eval
	MetaMemberDescription* mpFirstMember;
	MetaOperationDescription* mMetaOperationsList;
	MetaClassDescription* pNextMetaClassDescription;
	void* mpVTable[5/*6*/];
	MetaSerializeAccel* mpSerializeAccel;//atomic
	bool mbNameIsHeapAllocated;//created by lib
	bool mbIsIntrinsic;//created by lib, intrinsics arent added to header. where does this filter?? no clue, so i have to add this
	//oh wait for the value above, nevermind just realised metaoperation_serializeasync is not ever called on intrinsics.
	//its only called other objects. since we check for the overloaded specialization function for serialize which is set.

	String* GetToolDescriptionName(String* result);
	void Delete(void* pObj);
	void Destroy(void* pObj);
	void* New();
	void Construct(void*);
	void CopyConstruct(void*, void*);
	MetaClassDescription() : mbNameIsHeapAllocated(false), mbIsIntrinsic(false), mpExt(NULL) {}
	~MetaClassDescription();
	bool MatchesHash(u64 hash);
	void GetDescriptionSymbol(Symbol*);
	Symbol GetDescriptionSymbol();
	MetaMemberDescription* GetMemberDescription(const char* memberName);
	INLINE MetaMemberDescription* GetMemberDescription(String* _Str) {
		return GetMemberDescription(_Str->c_str());
	}
	INLINE void Initialize(const std::type_info& info) {
		//i know its slow but it doesnt need to be super  fast and i cba to change this (heap allocations/deallocations)
		char* buf = (char*)calloc(1, strlen(info.name()) + 1);
		memcpy(buf, info.name(), strlen(info.name()));
		TelltaleToolLib_MakeInternalTypeName(&buf);
		mbNameIsHeapAllocated = true;
		Initialize(buf);
	}
	//DO NOT USE typeid(type).name(), THIS IS NOT THE RIGHT FORMAT, use typeid(type) and use the overloaded function!!
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
	i32 mMinMetaVersion;//by lib for other game support
	TRange<int> mGameIndexVersionRange;
	inline MetaMemberDescription() {
		mMinMetaVersion = -1;
		mpName = NULL;
		mOffset = 0;
		mFlags = 0;
		mpHostClass = NULL;
		mpNextMember = NULL;
		mpMemberDesc = NULL;
		mpEnumDescriptions = NULL;
		mGameIndexVersionRange.min = -1;
		mGameIndexVersionRange.max = -1;
	}
	~MetaMemberDescription();
};

METAOP_FUNC_DEF(SerializeIntrinsicAsyncuint8)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncuint16)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncuint32)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncuint64)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncint8)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncint16)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncint32)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncint64)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncfloat)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncdouble)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncbool)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncString)

//Object state meta operations are like minecrafts 'getmetaint' or whatever from block classes, gets info as an int (defults to CRC32)
METAOP_FUNC_DEF(ObjectStateIntrinsic1);
METAOP_FUNC_DEF(ObjectStateIntrinsic2);
METAOP_FUNC_DEF(ObjectStateIntrinsic4);
METAOP_FUNC_DEF(ObjectStateIntrinsic8);
METAOP_FUNC_DEF(ObjectStateIntrinsicString);

METAOP_FUNC_DEF(EquivalenceIntrinsic1);
METAOP_FUNC_DEF(EquivalenceIntrinsic2);
METAOP_FUNC_DEF(EquivalenceIntrinsic4);
METAOP_FUNC_DEF(EquivalenceIntrinsic8);
METAOP_FUNC_DEF(EquivalenceIntrinsicString);

//There are more like Arithmetic, Comparison, Interpolate, but this library doesnt need them until needed.

//Serialize or deserialize the given type. Use PerformMetaSerializeAsync which gets the meta class description for you.
//If you are using this library not as a template library (but a compiled one) then use this!
MetaOpResult PerformMetaSerializeFull(MetaStream* pStream, void* pObj, MetaClassDescription* pDesc);

template<typename T> MetaOpResult PerformMetaSerializeAsync(MetaStream* pStream, T* pObj) {
	MetaClassDescription* desc = GetMetaClassDescription(typeid(T).name());
	if (!desc || !pStream)return eMetaOp_Fail;
	return PerformMetaSerializeFull(pStream, pObj, desc);
}

struct BinaryBuffer {

	char* mpData;
	int mDataSize;

	BinaryBuffer() : mpData(NULL), mDataSize(0) {}

	~BinaryBuffer() {
		if (mpData)
			delete[] mpData;
	}

	bool SetData(int dataSize, const void* pData) {
		if (mpData)
			delete[] mpData;
		if (dataSize > 0) {
			mpData = (char*)operator new[](dataSize);
			if (mpData) {
				mDataSize = dataSize;
				if (pData)
					memmove(mpData, pData, mDataSize);
				else
					memset(mpData, 0, mDataSize);
			}
			else return false;
		}
		return true;
	}

	void Memset(char v) {
		if (mpData)
			memset(mpData, v, mDataSize);
	}

	void Swap(BinaryBuffer& other) {
		char* my = mpData;
		mpData = other.mpData;
		other.mpData = my;
	}

	void Clear() {
		if (mpData)
			delete[] mpData;
	}

	bool AllocData(/*u64*/ int dataSize) {
		if (mpData)
			delete[] mpData;
		if (dataSize > 0) {
			mpData = (char*)operator new[](dataSize);
			if (mpData) {
				mDataSize = dataSize;
			}
			else return false;
		}
		return true;
	}

	BinaryBuffer(const BinaryBuffer& other) {
		mpData = NULL;
		mDataSize = 0;
		SetData(other.mDataSize, other.mpData);
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDesc, 
			MetaMemberDescription* pCtx, void* pUserData) {
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		BinaryBuffer* bb = static_cast<BinaryBuffer*>(pObj);
		u32 size = bb->mDataSize;
		stream->serialize_uint32(&size);
		bb->mDataSize = size;
		if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
			if (bb->mpData)
				delete[] bb->mpData;
			bb->mpData = new char[size];
			if (!bb->mpData) {
				stream->Advance(size);
				return eMetaOp_OutOfMemory;
			}
			memset(bb->mpData, 0, size);
		}
		stream->serialize_bytes(bb->mpData, size);
		return eMetaOp_Succeed;
	}

};

#include "Types/TRange.h"


namespace Acting {

	constexpr TRange<float> kSecBetweenLinesRange{ 0.25,0.35 };
	constexpr TRange<float> kProcLookatSpeakerDelayRange{ 0.0f, 0.15f };
	constexpr TRange<float> kProcLookAtDefaultBeginDelayRange{ 0.25f,0.75f };
	constexpr TRange<float> kProcLookAtDefaultEndDelayRange{ 0.08f,0.25f };
	constexpr TRange<float> kProcLookAtFinalEndDelayRange{ 0.0f,0.2f };
	constexpr TRange<float> sContributionScaleRangeDef{ 1.0f,1.0f };
	constexpr TRange<float> sAnimScaleRangeDef{ 1.0f,1.0f };
	constexpr Symbol kBaseLookAtPriKey{ 0x79E37DC8F4EA91E5 };
	constexpr Symbol kLookAtPriIncrementKey{ 0x0A398C78DEBB2D503 };
	constexpr Symbol kLookAtFadeTimeKey{ 0x31E84EFD0F4F164A };
	constexpr Symbol kLookAtGenerateInProjectKey{ 0x3619B30FCD406D18 };
	constexpr Symbol kPrefUseLegacyLookAtMaxAngle{ 0x9906F5914B07805F };
	constexpr Symbol kNoMoverDataInIdlesKey{ 0x85E2318F7535F345 };
	constexpr Symbol kTalkingDefaultMinInitialDelayKey{ 0x987C4BFD3365F7B4 };
	constexpr Symbol kListeningDefaultMinInitialDelayKey{ 0x6F44D84461CEEE36 };
	constexpr Symbol kAlwaysDefaultMinInitialDelayKey{ 0x37DB3E605513B158 };
	constexpr Symbol kTalkingDefaultMaxInitialDelayKey{ 0x6F001ADEABBDD403 };
	constexpr Symbol kListeningDefaultMaxInitialDelayKey{ 0x9D3C011E171914F7 };
	constexpr Symbol kAlwaysDefaultMaxInitialDelayKey{ 0x7DE1C03C3E2E54EE };
	constexpr Symbol kAnimFadeTimeDefKey{ 0x3AE879EB1625184D };
	constexpr Symbol kLookatStrengthKey{ 0x0EE55C02999ED1F98 };
	constexpr Symbol kAnimPreDelayDefKey{ 0x17C95DFE007ECEB2 };
	constexpr Symbol kAnimPostDelayDefKey{ 0x9A627A4316571A81 };
	constexpr Symbol kAnimPriorityDefKey{ 0x0E204FD2E718CCA1B };
	constexpr Symbol kAnimBlendingDefKey{ 0x0C1E314190D01F26 };
	constexpr Symbol kAnimScaleRangeDefKey{ 0x632C08E64BE46194 };
	constexpr Symbol kContributionScaleRangeDefKey{ 0x6DA3E623CF54498A };
	constexpr Symbol kPauseCommandDefKey{ 0x0FB9EDBBE2BE479B2 };
	constexpr Symbol kValidateEmbeddedCommandsKey{ 0x1CD8B1FCD7E493C8 };
	constexpr Symbol kStyleIdleTransitionTimeKey{ 0x6BDE8E8578A2708F };
	constexpr Symbol kStyleIdleTransitionInTimeOverrideKey{ 0x0A88B3F9F7218A160 };
	constexpr Symbol kStyleIdleTransitionOutTimeOverrideKey{ 0x0B8DEF12AFA1DAB4A };
	constexpr Symbol kStyleBaseIdleTransitionTimeKey{ 0x3974E6A76464C68A };
	constexpr Symbol kValidateVoicePreAAKey{ 0x0EA4E8DD73A00249D };
	constexpr Symbol kUseNewActingKey{ 0x1D7F3F7C5E624254 };
	constexpr Symbol kDisplayLookAtDebugDataKey{ 0x0FB304862D67CE4A4 };
	constexpr Symbol kDisplayDebugPathKey{ 0x0DDE767E8D498D4D1 };
	constexpr Symbol kSetDefaultIntensityKey{ 0x0E29CC88BC83A9DA8 };
	constexpr Symbol kDefaultIntensityValue{ 0x8B0C232138FF0D8A };
	constexpr Symbol kSetDefaultAccentTags{ 0x7F3B16B6CB2F79FE };
	constexpr Symbol kFixPopInAdditiveIdleTransition{ 0x0B72FDFD28B8C051F };
	constexpr Symbol kTalkingIdleTransitionTimeKey{ 0x12C9352DE9F6D9B3 };
	constexpr Symbol kActingIntensityKey{ 0x0DE48259EB8929B6 };
	constexpr Symbol kActingFaceAccentKey{ 0xCBAE23CE3F2B0CCE };
	constexpr Symbol kActingBodyAccentKey{ 0xEFFC9824AB71EF3D };
	constexpr Symbol kActingHead1AccentKey{ 0x7D37DAB358DDE48C };
	constexpr Symbol kActingHead2AccentKey{ 0xC86CE44964FFF54B };
	constexpr Symbol kLookAtStopKey{ 0x66E772BA8E023A4D };
	constexpr Symbol kGenerateAccentsFromLanguageResources{ 0x73AF59848641A183 };
	constexpr Symbol kEnableNewLookats{ 0x69967C61AE2C7675 };
	constexpr Symbol kUseCurvedPath{ 0x8A29C4B8ACC1A104 };
	constexpr Symbol kLegacyEnableTargetedBlockingOnAttachments{ 0x6B8EE0DEAD524B98 };
	constexpr Symbol kLegacyUseOldBGMIdleBehavior{ 0x0E35173FA6BB35AD1 };
	constexpr Symbol kPriorityKey{ 0x19E1083970E1804D };
	constexpr Symbol kFadeTimeKey{ 0x5252B0398028EEE7 };
	constexpr Symbol kPreDelayKey{ 0x49AE373C8672B756 };
	constexpr Symbol kPostDelayKey{ 0x5EAA376E1C28BFEB };
	constexpr Symbol kBlendingKey{ 0x8CD42C9E0BD6AD43 };
	constexpr Symbol kScaleRangeKey{ 0x0D07B6742C1C50C1F };
	constexpr Symbol kStyleMumbleMouthKey{ 0x7D10D6DE5CE9F014 };
	constexpr Symbol kContributionRangeKey{ 0x151FE38B3A415A9D };
	constexpr Symbol kResourceGroupsKey{ 0x89CC3329DF3EC278 };
	constexpr Symbol kStartOffsetRangeKey{ 0x54BF2651351D34B9 };
	constexpr Symbol kStyleTransitionTimeKey{ 0x0B7535A357B22778 };
	constexpr Symbol kPropertyKeyBlockOverrunPercentage{ 0x38AA965019501AF9 };
	constexpr Symbol kPropertyKeyIntensityContributionMultiplierRange{ 0x9525F5C22FC0F5D1 };
	constexpr Symbol kPropertyKeyIntensityScaleMultiplierRange{ 0x272FA01DF8115368 };
	constexpr Symbol kPropertyKeyValidIntensityRange{ 0x6C92A5EB51A06379 };
	constexpr Symbol kPropertyKeyPaletteValidIntensityRange{ 0xDB68FF2D8AE9697B };
	constexpr Symbol kPropertyKeyIntensityTimeBetweenActionsMultiplierRange{ 0x8A21C545508159E };
	constexpr Symbol kRuntimeApplyChoreGenConflictToAllKey{ 0x6890F50FAF563600 };
	constexpr Symbol kRuntimeChoreGenConflictActionKey{ 0x345FF590FDEC8D01 };
}

template<int N>
struct __BitSet_BaseN {
	static constexpr int _TyN = (N / 32) + (N % 32 == 0 ? 0 : 1);
};

template<int N>
struct BitSetBase {
	u32 mWords[N];

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		for (int i = 0; i < N; i++) {
			((MetaStream*)pUserData)->serialize_uint32(((BitSetBase<N>*)pObj)->mWords + i);
		}
		return eMetaOp_Succeed;
	}

};

template<typename T, int N, int NoClueLol>
struct BitSet : BitSetBase<__BitSet_BaseN<N>::_TyN> {

	T operator[](int _Index) const {
		return static_cast<T>(this.mWords[_Index]);
	}

};

/*
* Tries to find the symbol name for the given symbol. Make sure the global hash database is set. This is will try search all pages in the 
* current game set (using set blowfish key). This can be slow! If the value is found, the outpageref pointer is set to the page it was
* found in in case you want to find more symbol names in that page. Returns <NotFound> if not found, or <Empty> if the CRC is 0.
* Use once the library is initialized.
*/
String FindSymbolName(const Symbol&, HashDatabase::Page*& outpageref);

/*
* Gets the constant list of file extension strings that the library currently supports.
*/
const std::vector<const char*>* GetMetaFileExtensions();

namespace RecordingUtils {

	enum ScriptType {
		STScript = 1,
		STActor = 2,
		STEngineer = 3,
		STTakeSheet = 4
	};

	enum RecordingStatus {
		eRecordingStatus_None = 0,
		eRecordingStatus_AtStudio = 1,
		eRecordingStatus_Recorded = 2,
		eRecordingStatus_Delivered = 3
	};

	struct EnumRecordingStatus : public EnumBase {
		RecordingStatus mVal;
	};

	struct EnumScriptType : public EnumBase {
		ScriptType mVal;
	};

}

struct PathBase {};

template<typename T>
MetaClassDescription* GetMetaClassDescription() {
	MetaClassDescription* clazz = TelltaleToolLib_GetFirstMetaClassDescription();
	const char* tn = typeid(T).name();
	while (clazz != NULL) {
		if (!_stricmp(tn, clazz->mpTypeInfoExternalName))
			return clazz;
		TelltaleToolLib_GetNextMetaClassDescription(&clazz);
	}
	return NULL;
}

/*struct WalkPath {
	Set<int, std::less<int>> mTrianglesInPath;
	Handle<WalkBoxes> mhBoxes;
	Vector3 mRequestedStart, mRequestedEnd, mPathStart, mPathEnd,
		mPathStartDir, mPathEndDir;
	String mName;
	mutable DCArray<PathBase> mPaths;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj,
			pObjDescription, pContextDescription, pUserData);
		if (r != eMetaOp_Succeed)
			return r;
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		WalkPath* o = (WalkPath*)pObj;
		bool iswrite = meta->mMode == MetaStreamMode::eMetaStream_Write;
		u32 paths = o->mPaths.GetSize();
		MetaClassDescription* pathBaseDescription = GetMetaClassDescription<PathBase>();
		meta->serialize_uint32(&paths);
		for (int i = 0; i < paths; i++) {
			if (iswrite) {
				meta->serialize_uint64(&pathBaseDescription->mHash);
				r = PerformMetaSerializeFull(meta, o->mPaths.mpStorage + i, pathBaseDescription);
				if (r != eMetaOp_Succeed)
					return r;
			}
			else {
				u64 crc = 0;
				meta->serialize_uint64(&crc);
				if (crc != pathBaseDescription->mHash) {
					TelltaleToolLib_RaiseError("PathBase ", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
			}
		}
		return eMetaOp_Succeed;
	}

};*/

#endif
