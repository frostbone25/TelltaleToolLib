// This file was written by Lucas Saragosa. If you use this code or library,
// I ask you to you give credit to me and the amazing Telltale Games.

#include "TelltaleToolLibrary.h"
#include "Meta.hpp"
#include "MetaInitialize.h"
#include "HashDB/HashDB.h"

i32 TelltaleToolLib_GetGameKeyIndex(const char* pGameID) {
    if (pGameID) {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (!_stricmp(sBlowfishKeys[i].game_id, pGameID)) {
                return i;
            }
        }
    }
    return -1;
}

MetaOpResult TelltaleToolLib_PerformMetaSerialize(MetaClassDescription* pObjectDescription, void* pObject, MetaStream* pUserData) {
    if (!pObjectDescription || !pObject || !pUserData)
        return eMetaOp_Fail;
    return PerformMetaSerializeFull(pUserData, pObject, pObjectDescription);
}

HashDatabase* sgHashDB = NULL;
bool sInitialized = false;

void _DefaultCallback(const char* msg, ErrorSeverity e) {
#ifdef DEBUGMODE
    printf("ERROR: %s: [%s]\n",msg, e == ErrorSeverity::CRITICAL_ERROR ? "CRITICAL" : e == ErrorSeverity::NOTIFY ? "NOTIFY" 
    : e == ErrorSeverity::WARN ? "WARNING" : "ERR");
#endif
    if (e == ErrorSeverity::CRITICAL_ERROR)exit(-1);
}

ErrorCallbackF sDefaultErrorCallback = _DefaultCallback;

void TelltaleToolLib_SetErrorCallback(ErrorCallbackF _Func) {
    sDefaultErrorCallback = _Func;
}

void TelltaleToolLib_RaiseError(const char* _Msg, ErrorSeverity _S) {
    sDefaultErrorCallback(_Msg, _S);
}

void TelltaleToolLib_Free() {
    if (sgHashDB) {
        delete sgHashDB;
        sgHashDB = NULL;
    }
    sDefaultErrorCallback = _DefaultCallback;
}

void TelltaleToolLib_SetGlobalHashDatabaseFromStream(DataStream* stream) {
    if(stream)
        TelltaleToolLib_SetGlobalHashDatabase(new HashDatabase(stream));
}

void TelltaleToolLib_SetGlobalHashDatabase(HashDatabase* db) {
    if (sgHashDB)
        delete sgHashDB;
    sgHashDB = db;
    if(db)
        db->Open();
}

HashDatabase* TelltaleToolLib_GetGlobalHashDatabase() {
    return sgHashDB;
}

u8* TelltaleToolLib_EncryptLencScript(u8* data, u32 size, u32* outsize) {
    u8* ret = (u8*)malloc(size);
    memcpy(ret, data, size);
    TelltaleToolLib_BlowfishEncrypt(ret, size, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
    return ret;
}

u8* TelltaleToolLib_DecryptLencScript(u8* data, u32 size, u32* outsize) {
    u8* ret = (u8*)malloc(size);
    memcpy(ret, data, size);
    TelltaleToolLib_BlowfishDecrypt(ret, size, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
    return ret;
}

u8* TelltaleToolLib_EncryptScript(u8* data, u32 size, u32 *outsize) {
    if (!data || 4 < size)return NULL;
    if (*(int*)data == *(const int*)"\x1BLua") {
        u8* ret = (u8*)malloc(size);
        memcpy(ret, data, size);
        *outsize = size;
        *(int*)ret = 1850035227;//LEn
        TelltaleToolLib_BlowfishEncrypt(ret + 4, size - 4, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
        return ret;
    }
    else if (*(int*)data != *(const int*)"\x1BLEo" && *(int*)data != *(const int*)"\x1BLEn") {
        u8* ret = (u8*)malloc(size + 4);
        memcpy(ret + 4, data, size);
        *outsize = size + 4;
        *(int*)ret = 1866812443;//LEo
        TelltaleToolLib_BlowfishEncrypt(ret + 4, size, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
        return ret;
    }
    u8* ret = (u8*)malloc(size);
    memcpy(ret, data, size);
    *outsize = size;
    return ret;
}

u8* TelltaleToolLib_DecryptScript(u8* data, u32 size, u32* outsize) {
    if (8 >= size || !data)return NULL;
    if (*(char*)data != '\x1B') {
        //plain old text version workaround
        u8* ret = (u8*)malloc(size);
        memcpy(ret, data, size);
        *outsize = size;
        return ret;
    }
    if (*(int*)data == *(const int*)"\x1BLEo") {
        u8* ret = (u8*)malloc(size - 4);
        memcpy(ret, data + 4, size - 4);
        *outsize = size - 4;
        TelltaleToolLib_BlowfishDecrypt(ret, size - 4, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
        return ret;
    }
    u8* ret = (u8*)malloc(size);
    memcpy(ret, data, size);
    if (*(int*)data == *(const int*)"\x1BLEn")
        *(int*)ret = 1635077147;
    TelltaleToolLib_BlowfishDecrypt(ret + 4, size - 4, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
    *outsize = size;
    return ret;
}

const char* TelltaleToolLib_GetMetaClassDescriptionName(MetaClassDescription* pObjDesc) {
    return pObjDesc->mpTypeInfoName;
}

DataStream* TelltaleToolLib_CreateDataStream(const char* fp, DataStreamMode mode) {
    return new DataStreamFileDisc((FileHandle)PlatformSpecOpenFile(fp, mode), mode);
}

void TelltaleToolLib_DeleteDataStream(DataStream* stream) {
    delete stream;//calls destructor
}

char* TelltaleToolLib_Alloc_GetFixed1024ByteStringBuffer() {
    static char buf[1024];
    return &buf[0];
}

char* TelltaleToolLib_Alloc_GetFixed8BytePointerBuffer() {
    static char buf[8];
    return &buf[0];
}

void TelltaleToolLib_GetNextMetaClassDescription(MetaClassDescription** pObjDescPtr) {
    if (pObjDescPtr && *pObjDescPtr) *pObjDescPtr = (*pObjDescPtr)->pNextMetaClassDescription;
}

void TelltaleToolLib_GetNextMetaMemberDescription(MetaMemberDescription** pMemberDescPtr) {
    if (pMemberDescPtr && *pMemberDescPtr)*pMemberDescPtr = (*pMemberDescPtr)->mpNextMember;
}

MetaClassDescription* TelltaleToolLib_FindMetaClassDescription_ByHash(u64 pHash) {
    for (MetaClassDescription* i = TelltaleToolLib_GetFirstMetaClassDescription(); i;) {
        if (i->mHash == pHash)
            return i;
        TelltaleToolLib_GetNextMetaClassDescription(&i);
    }
    return NULL;
}

MetaClassDescription* TelltaleToolLib_FindMetaClassDescription(const char* pStr, bool pIsName) {
    if (pIsName) {
        u64 crc = CRC64_CaseInsensitive(0, pStr);
        for (MetaClassDescription* i = TelltaleToolLib_GetFirstMetaClassDescription(); i;) {          
            if (i->mHash == crc)
                return i;
            TelltaleToolLib_GetNextMetaClassDescription(&i);
        }
    }
    else {
        for (MetaClassDescription* i = TelltaleToolLib_GetFirstMetaClassDescription(); i;) {
            if (!i->mpExt) {//stfu:(cba
                TelltaleToolLib_GetNextMetaClassDescription(&i);
                continue;
            }
            if (!_stricmp(pStr, i->mpExt))
            if (!_stricmp(pStr,i->mpExt))
                return i;
            TelltaleToolLib_GetNextMetaClassDescription(&i);
        }
    }
    return NULL;
}

void TelltaleToolLib_GetMetaMemberDescriptionInfo(MetaMemberDescription* pMemberDesc, void* pDest, MetaMemberDescriptionParam param) {
    switch (param) {
    case MetaMemberDescriptionParam::eMMDP_Name:
        *static_cast<const void**>(pDest) = pMemberDesc->mpName;
        break;
    case MetaMemberDescriptionParam::eMMDP_Offset:
        *static_cast<u64*>(pDest) = pMemberDesc->mOffset;
        break;
    case MetaMemberDescriptionParam::eMMDP_Flags:
        *static_cast<u32*>(pDest) = pMemberDesc->mFlags;
        break;
    case MetaMemberDescriptionParam::eMMDP_HostClass:
        *static_cast<MetaClassDescription**>(pDest) = pMemberDesc->mpHostClass;
        break;
    case MetaMemberDescriptionParam::eMMDP_MemberClassDesc:
        *static_cast<MetaClassDescription**>(pDest) = pMemberDesc->mpMemberDesc;
        break;
    case MetaMemberDescriptionParam::eMMDP_NextMember:
        *static_cast<MetaMemberDescription**>(pDest) = pMemberDesc->mpNextMember;
        break;
    default:
        return;
    }
}

void TelltaleToolLib_GetMetaClassDescriptionInfo(MetaClassDescription* pObj, void* pDest, MetaClassDescriptionParam param) {
    switch (param) {
    case MetaClassDescriptionParam::eMCDP_Extension:
        *static_cast<const char**>(pDest) = pObj->mpExt;
        break;
    case MetaClassDescriptionParam::eMCDP_Name:
        *static_cast<const char**>(pDest) = pObj->mpTypeInfoName;
        break;
    case MetaClassDescriptionParam::eMCDP_Hash:
        *static_cast<u64*>(pDest) = pObj->mHash;
        break;
    case MetaClassDescriptionParam::eMCDP_Flags:
        *static_cast<u32*>(pDest) = pObj->mFlags.mFlags;
        break;
    case MetaClassDescriptionParam::eMCDP_ClassSize:
        *static_cast<u32*>(pDest) = pObj->mClassSize;
        break;
    case MetaClassDescriptionParam::eMCDP_FirstMemberPtr:
        *static_cast<MetaMemberDescription**>(pDest) = pObj->mpFirstMember;
        break;
    case MetaClassDescriptionParam::eMCDP_OperationsList:
        *static_cast<MetaOperationDescription**>(pDest) = pObj->mMetaOperationsList;
        break;
    case MetaClassDescriptionParam::eMCDP_VTable:
        *static_cast<void***>(pDest) = &pObj->mpVTable[0];
        break;
    case MetaClassDescriptionParam::eMCDP_SerializeAccel:
        *static_cast<MetaSerializeAccel**>(pDest) = pObj->mpSerializeAccel;
        break;
    default:
        return;
    }
}

MetaClassDescription* TelltaleToolLib_GetFirstMetaClassDescription() {
    return spFirstMetaClassDescription;
}

const char* TelltaleToolLib_GetVersion() {
	return _VERSION;
}

i32 TelltaleToolLib_GetMetaTypesCount() {
    return sMetaTypesCount;
}

bool TelltaleToolLib_Initialized() {
    return sInitialized;
}

void TelltaleToolLib_SetBlowfishKey(const char* game_id) {
    if (game_id) {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (!_stricmp(sBlowfishKeys[i].game_id, game_id)) {
                sSetKeyIndex = i;
                break;
            }
        }
    }
    else sSetKeyIndex = DEFAULT_BLOWFISH_GAME_KEY;
    //if (spBlowfish) {
    //    spBlowfish->Init(sBlowfishKeys[sSetKeyIndex].game_key);
    //}
}

const char* TelltaleToolLib_GetBlowfishKey() {
    return sBlowfishKeys[sSetKeyIndex].game_id;
}

bool TelltaleToolLib_Initialize(const char* game_id) {
    if (sInitialized)return false;
    if (game_id) {
        const BlowfishKey* k = NULL;
        for (int i = 0; i < KEY_COUNT; i++) {
            if (!_stricmp(sBlowfishKeys[i].game_id, game_id)) {
                k = &sBlowfishKeys[i];
                sSetKeyIndex = i;
                break;
            }
        }
        if (k == NULL)return false;
       // Blowfish::Initialize(k);
    }
  //  else Blowfish::Initialize(&sBlowfishKeys[sSetKeyIndex]);
    MetaInit::Initialize();//init all types
    sInitialized = true;
    return true;
}

//I KNOW ITS BAD FOR ALLOCATIONS BUT IM SORRY I CANT BE ASKED TO CHANGE IT D:
void TelltaleToolLib_MakeInternalTypeName(char** _StringPtr) {
    static std::string _sRepl_A = "struct ";
    static std::string _sRepl_B = "class ";
    static std::string _sRepl_C = "enum ";
    static std::string _sRepl_D = "std:: ";
    static std::string _sNothing = "";
	std::string ReplStr(*_StringPtr);
    size_t start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_A, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_A.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_B, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_B.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_C, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_C.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_D, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_D.length(), _sNothing);
    }
    u32 slen = ReplStr.length();
    char* nbuf = (char*)malloc(slen+1);
    memcpy(nbuf, ReplStr.c_str(), slen);
    //for (int i = 0; i < slen; i++) {
    //    nbuf[i] |= 0b100000;
    //}
    nbuf[slen] = 0i8;
    free(*_StringPtr);
    *_StringPtr = nbuf;
}

void _PrintfDumper(const char* const _Fmt, const char* _ParamA, const char* _ParamB) {
    if (_ParamA && _ParamB) {
        printf(_Fmt, _ParamA, _ParamB);
    }
    else if (_ParamA) {
        printf(_Fmt, _ParamA);
    }
    else {
        printf(_Fmt);
    }
}


void printMembers(int tabs, MetaMemberDescription* mem, DumpClassInfoF _Dumper) {
    while (mem) {
        for (int i = 0; i < tabs; i++)
            _Dumper("\t", NULL, NULL);
        _Dumper("[%s] %s\n", mem->mpMemberDesc->mpTypeInfoName, mem->mpName);
        if (mem->mpMemberDesc->mpFirstMember)
            printMembers(tabs + 1, mem->mpMemberDesc->mpFirstMember,_Dumper);
        mem = mem->mpNextMember;
    }
}

void TelltaleToolLib_DumpClassInfo(DumpClassInfoF _Dumper) {
    MetaClassDescription* clazz = TelltaleToolLib_GetFirstMetaClassDescription();
    for (int i = 0; i < TelltaleToolLib_GetMetaTypesCount(); i++) {
        _Dumper("Class: %s\n", clazz->mpTypeInfoName,NULL);
        if (clazz->mpFirstMember) {
            printMembers(1, clazz->mpFirstMember,_Dumper);
        }
        TelltaleToolLib_GetNextMetaClassDescription(&clazz);
        _Dumper("\n\n",NULL,NULL);
    }
}