// This file was written by Lucas Saragosa. If you use this code or library,
// I ask you to you give credit to me and the amazing Telltale Games.


#ifndef _TTToolLib
#define _TTToolLib

#define _VERSION "6.1.2"

#include <type_traits>
#include <string>
#include "DataStream/DataStream.h"
#include "Blowfish.h"

//64 BIT! ONLY!

#ifdef _MSC_VER
#ifdef _DEBUG
#define DEBUGMODE
#else
#define RELEASEMODE
#endif
#else
#error "Please implement debug macro check for this compiler!"
#endif

#define INLINE __inline
#define FORCE_INLINE __forceinline

#define _TTToolLib_Exp extern "C" __declspec(dllexport)

#define _TTTLib_PrintMetaClasses() TelltaleToolLib_DumpClassInfo(_PrintfDumper)

typedef std::make_unsigned<__int64>::type u64;
typedef std::make_unsigned<__int32>::type u32;
typedef std::make_unsigned<__int16>::type u16;
typedef std::make_unsigned<__int8> ::type u8;
typedef __int8  i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;
typedef std::basic_string<i8,std::char_traits<i8>, std::allocator<i8>> String;
struct Flags;
template<typename T> class Ptr;
struct MetaClassDescription;
struct MetaMemberDescription;
enum MetaOpResult;
class MetaStream;

enum MetaMemberDescriptionParam {
	eMMDP_Name = 1,//stores 8 bytes (ptr)
	eMMDP_Offset = 2,//stores 8 bytes
	eMMDP_Flags = 3,//stores 4 bytes
	eMMDP_HostClass = 4,//stores 8 bytes (ptr)
	eMMDP_MemberClassDesc = 5,//stores 8 bytes (ptr)
	eMMDP_NextMember = 6,//stores 8 bytes (ptr)
};

enum MetaClassDescriptionParam {
	eMCDP_Extension = 1,//stores 8 bytes (ptr)
	eMCDP_Name = 2,//same as calling getmetaclassdescriptionname
	eMCDP_Hash = 3,//stores 8 bytes
	eMCDP_Flags = 4,//stores 4 bytes
	eMCDP_ClassSize = 5,//stores 4 bytes
	eMCDP_FirstMemberPtr = 6,//stores 8 bytes (ptr)
	eMCDP_OperationsList = 7,//stores 8 bytes (ptr)
	eMCDP_VTable = 8,//stores 8 bytes (ptr to array of ptrs)
	eMCDP_SerializeAccel = 9, //stores 8 bytes (ptr)
};

typedef void (*DumpClassInfoF)(const char* const _Fmt, const char* _ParamA, const char* _ParamB);

void _PrintfDumper(const char* const _Fmt, const char* _ParamA, const char* _ParamB);

/*
* Gets the version of the TelltaleTool library as a string.
*/
_TTToolLib_Exp const char* TelltaleToolLib_GetVersion();

/*
* Dumps class information in a tabbed format
*/
_TTToolLib_Exp void TelltaleToolLib_DumpClassInfo(DumpClassInfoF _Dumper);

/*
* Makes the type name of the given string pointer to one which is ready to be hashed. 
* The parameter needs to be a pointer to a string which is allocated with calloc/malloc.
* This string is freed and the new string (allocated with calloc) is set to the parameter pointer's value.
*/
_TTToolLib_Exp void TelltaleToolLib_MakeInternalTypeName(char**);

/*
* Initialize the library. This must be called before you call any file reading and writing functions.
* Must pass a game id for the game that you are going to be working with files from. This is used for decryption keys.
* See TelltaleToolLib_SetBlowfishKey. The game id *can* be NULL, where it will be set to the default game id which can be 
* found in the Blowfish translation unit. If you pass an invalid game id, this will return false and fail. Passing NULL succeeds 
* with the default ID, this goes aswell for the SetBlowfishKey function.
*/
_TTToolLib_Exp bool TelltaleToolLib_Initialize(const char* game_id);

/*
* Returns the first meta class description in the list of all meta class description types.
*/
_TTToolLib_Exp MetaClassDescription* TelltaleToolLib_GetFirstMetaClassDescription();

/*
* Inserts the next meta class description in the list into the parameter, or NULL is inserted if there are no more after it.
*/
_TTToolLib_Exp void TelltaleToolLib_GetNextMetaClassDescription(MetaClassDescription**);

/*
* Gets various information parameters from the given MCD and stores them in the function params
*/
_TTToolLib_Exp void TelltaleToolLib_GetMetaClassDescriptionInfo(MetaClassDescription*, void*,MetaClassDescriptionParam);

/*
* Gets various information parameters from the given MMD and stores them in the function params (middle one)
*/
_TTToolLib_Exp void TelltaleToolLib_GetMetaMemberDescriptionInfo(MetaMemberDescription*, void*, MetaMemberDescriptionParam);

/*
* Gets the next member in the meta class description from the parameter and stores it in the parameter. If there is none after the passed
* one then NULL is inserted.
*/
_TTToolLib_Exp void TelltaleToolLib_GetNextMetaMemberDescription(MetaMemberDescription**);

/*
* Sets the current game ID which is used for decryption of files when needed. Game IDs can be found in the github repo.
*/
_TTToolLib_Exp void TelltaleToolLib_SetBlowfishKey(const char* game_id);

/*
* Gets the game id of the currently set blowfish key
*/
_TTToolLib_Exp const char* TelltaleToolLib_GetBlowfishKey();

/*
* Searches for the given meta class description file extension or name. If the second parameter is false, then its by extension
* else its by the name. For example, for telltale texture files, use ("T3Texture",true) or ("d3dtx",false) to get the class for it.
*/
_TTToolLib_Exp MetaClassDescription* TelltaleToolLib_FindMetaClassDescription(const char*, bool bByNameOrExt);

/*
* Same as TelltaleToolLib_FindMetaClassDescription, but searches by hash.
*/
_TTToolLib_Exp MetaClassDescription* TelltaleToolLib_FindMetaClassDescription_ByHash(u64);

/*
* Gets the name of the given meta class description
*/
_TTToolLib_Exp const char* TelltaleToolLib_GetMetaClassDescriptionName(MetaClassDescription*);

/*
* When initialize is called (else this would return 0), this returns the amount of meta types that are initialized.
*/
_TTToolLib_Exp i32 TelltaleToolLib_GetMetaTypesCount();

/*
* Returns if this library is initialized yet.
*/
_TTToolLib_Exp bool TelltaleToolLib_Initialized();

/*
* Creates a datastream for the given file path.
*/
_TTToolLib_Exp DataStream* TelltaleToolLib_CreateDataStream(const char* fp, DataStreamMode);

/*
* Deletes the given data stream. Only use this if you know what you're doing, normally its deleted internally.
*/
_TTToolLib_Exp void TelltaleToolLib_DeleteDataStream(DataStream*);

/*
* A static 1024 byte char buffer for strings. This can be used by other programs to store strings passed in between the library.
*/
_TTToolLib_Exp char* TelltaleToolLib_Alloc_GetFixed1024ByteStringBuffer();

/*
* A static 8 byte buffer to store pointers. Useful when using for loops which need to get the next class/member and can store the 
* pointer to it in this buffer to save memory allocations.
*/
_TTToolLib_Exp char* TelltaleToolLib_Alloc_GetFixed8BytePointerBuffer();

//These next two family of functions return a buffer allocated with MALLOC! So use FREE. Returns NULL if a problem occurred.
//These only work for LUA files.

_TTToolLib_Exp u8* TelltaleToolLib_EncryptScript(u8* data, u32 size, u32* outsize);
_TTToolLib_Exp u8* TelltaleToolLib_DecryptScript(u8* data, u32 size, u32* outsize);

//These next two family of functions return a buffer allocated with MALLOC! So use FREE. Returns NULL if a problem occurred.
//These only work for LENC files.

_TTToolLib_Exp u8* TelltaleToolLib_EncryptLencScript(u8* data, u32 size, u32 *outsize);
_TTToolLib_Exp u8* TelltaleToolLib_DecryptLencScript(u8* data, u32 size, u32* outsize);

class HashDatabase;
class DataStream;

/*
* Sets the global hash database used to search CRCs. It is very important that its set before you read or write most types (eg PropertySet)
* If one is already set, then this deletes the old use using operator delete. Can be set to NULL.
*/
_TTToolLib_Exp void TelltaleToolLib_SetGlobalHashDatabase(HashDatabase*);

/*
* Does the same as the normal set global hash db, but creates it for you. Pass in the reading file stream to it. This stream 
* is deleted by the library so DONT DELETE IT!
*/
_TTToolLib_Exp void TelltaleToolLib_SetGlobalHashDatabaseFromStream(DataStream*);

/*
* Gets the game index of the given game id. This is used to match when a game is released with others. Used internally make sure the 
* current file is being read correctly with the correct version range
*/
_TTToolLib_Exp i32 TelltaleToolLib_GetGameKeyIndex(const char* pGameID);

/*
* Gets the global hash database, or NULL if its not been set.
*/
_TTToolLib_Exp HashDatabase* TelltaleToolLib_GetGlobalHashDatabase();

/*
* Frees all non-static memory related to this library. 
*/
_TTToolLib_Exp void TelltaleToolLib_Free();

enum ErrorSeverity {
	NOTIFY,
	WARN,
	ERR,
	CRITICAL_ERROR
};

typedef void (*ErrorCallbackF)(const char* _Msg, ErrorSeverity _Severity);

/*
* Sets the error callback which will be called when any error occurs.
*/
_TTToolLib_Exp void TelltaleToolLib_SetErrorCallback(ErrorCallbackF _Func);

_TTToolLib_Exp void TelltaleToolLib_RaiseError(const char* _Msg, ErrorSeverity _S);

_TTToolLib_Exp MetaOpResult
TelltaleToolLib_PerformMetaSerialize(MetaClassDescription* pObjectDescription, void* pObject, MetaStream* pUserData);

extern bool sInitialized;

struct Flags {
	u32 mFlags;

	Flags(u32 i) : mFlags(i) {}

	Flags() {}

	INLINE Flags& operator=(u32 i) {
		mFlags = i;
		return *this;
	}
	INLINE Flags& operator|=(u32 i) {
		mFlags |= i;
		return *this;
	}
	INLINE Flags& operator&=(u32 i) {
		mFlags &= i;
		return *this;
	}
};

template<int Bits, typename T>
struct TTBitArray {
	enum NoInit {
		eNoInit = 0
	};

	T mBits[Bits / 8];

};

namespace Geo {

	struct GeoGuid {
		u64 _Id[2];
	};

}

template<typename T> void PtrModifyRefCount(T*, int delta) {}//deprecated in engine/not needed for shipping

//telltale impl of pointers, mainly its calls to the ref count 
template<typename T>
class Ptr {
private:
	T* mpData;
public:

	Ptr(const T* pObj) : mpData(pObj) {}

	Ptr(const Ptr<T>& other) : mpData(other.mpData) {}

	~Ptr() {}

	Ptr<T> operator=(const Ptr<T>& pObj) {
		mpData = pObj.mpData;
	}

	Ptr<T> operator=(T* pOther) {
		mpData = pOther;
	}

	void Assign(const T* pObj) {
		mpData = pObj;
	}

	void DeleteObject() {
		if (mpData) {
			delete mpData;
		}
	}

};

#endif
