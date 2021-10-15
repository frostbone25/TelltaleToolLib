// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _HOI
#define _HOI

#include "../TelltaleToolLibrary.h"
#include "../Meta.hpp"
#include <algorithm>
#include "../HashDB/HashDB.h"

struct HandleObjectInfo {
	Symbol mObjectName;
	Flags mFlags;
};

struct HandleBase {

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDescription, MetaMemberDescription* pCtx,
		void* pUserData) {
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		HandleBase* handle = static_cast<HandleBase*>(pObj);
		if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
			if (stream->mStreamVersion >= 5) {
				stream->serialize_Symbol(&handle->mHandleObjectInfo.mObjectName);
			}
			else {
				String str;
				stream->serialize_String(&str);
				handle->mHandleObjectInfo.mObjectName = str.c_str();
			}
		}
		else {
			stream->serialize_Symbol(&handle->mHandleObjectInfo.mObjectName);
		}
		return eMetaOp_Succeed;
	}

	//HandleObjectInfo* mpHandleObjectInfo; //normally a pointer in the engine
	//maybe to do with a buffer of them? but anyway
	HandleObjectInfo mHandleObjectInfo;
};

template<typename T> struct Handle : public HandleBase {

	Handle<T>& operator=(const Symbol& o) {
		mHandleObjectInfo.mObjectName = o;
		return *this;
	}

	String GetObjectName() {
		static const String _NF = "<NotFound>";
		HashDatabase* db = TelltaleToolLib_GetGlobalHashDatabase();
		MetaClassDescription* desc = GetMetaClassDescription(typeid(T).name());
		if (!desc || !desc->mpExt)return _NF;
		if (!db)return _NF;
		HashDatabase::Page* page = NULL;
		String pagen = "Files_";
		pagen += desc->mpExt;
		pagen += '_';
		String gameid = sBlowfishKeys[sSetKeyIndex].game_id;
		std::transform(gameid.begin(), gameid.end(), gameid.begin(), ::toupper);
		pagen += gameid;
		page = db->FindPage(pagen.c_str());
		if (!page)return _NF;
		String ret;
		db->FindEntry(page, mHandleObjectInfo.mObjectName.GetCRC(), &ret);
		if (ret.size() == 0)
			ret = _NF;
		return ret;
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDescription, MetaMemberDescription* pCtx,
		void* pUserData) {
		return HandleBase::MetaOperation_SerializeAsync(pObj, pObjDescription, pCtx, pUserData);//originally this overloaded one
		//would have have extra code here specific to loading type T but we dont need it so just do the norm.
	}

};

template<typename T> struct HandleLock : public Handle<T> {

};

template<typename T> INLINE Handle<T> CreateHandle(const char* fileName) {
	Handle<T> ret;
	ret.mHandleObjectInfo.mObjectName.SetCRC(CRC64_CaseInsensitive(0, fileName));
	return ret;
}

#endif
