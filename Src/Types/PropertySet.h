// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../TelltaleToolLibrary.h"
#include "../HashDB/HashDB.h"
#include "Set.h"
#include "List.h"
#include "Map.h"
#include <memory>
#include "HandleObjectInfo.h"
#include <algorithm>

#ifndef _PROP
#define _PROP

struct PropertyValue {
	MetaClassDescription* mpDataDescription;
	void* mpValue;
	//union {
	//	char mStaticBuf[40]; //cool way of saving memory allocations by telltale! (like small string optimization!)
	//	char* mpBuf;
	//};

	void ClearData() {
		if (mpDataDescription && mpValue)
			mpDataDescription->Delete(mpValue);
		mpValue = NULL;
	}

	template<typename T> T* CastValue() const {
		if (mpDataDescription && (mpDataDescription == GetMetaClassDescription(typeid(T).name()) ||
			(GetMetaClassDescription(typeid(T).name())->mFlags.mFlags & 0x200) && (mpDataDescription->mFlags.mFlags & 0x200))) {
			//if descriptions are the same or they are both script enums
			return (T*)mpValue;
		}
		else return NULL;
	}

	void SetData(void* pSrcObj, MetaClassDescription* pDescription) {
		if (!pDescription) {
			if (!mpDataDescription)
				return;
			pDescription = mpDataDescription;
		}
		ClearData();
		mpDataDescription = pDescription;
		if (!(pDescription->mpVTable[0]) || !(pDescription->mpVTable[2]) || !(pDescription->mpVTable[3])) {
			TelltaleToolLib_RaiseError("Cannot add non trivial virtual types as a property as its not a concrete type!", ErrorSeverity::ERR);
			return;
		}
		mpValue = operator new(pDescription->mClassSize);
		if (pSrcObj) {
			pDescription->CopyConstruct(mpValue, pSrcObj);
		}
		else {
			pDescription->Construct(mpValue);
		}
	}

	PropertyValue(PropertyValue&& other) {
		mpDataDescription = other.mpDataDescription;
		mpValue = other.mpValue;
		other.mpValue = NULL;
		other.mpDataDescription = NULL;
	}

	PropertyValue& operator=(const PropertyValue& other) {
		SetData(other.mpValue, other.mpDataDescription);
		return *this;
	}

	PropertyValue(const PropertyValue& other) {
		mpDataDescription = other.mpDataDescription;
		if (other.mpValue && mpDataDescription) {
			mpValue = operator new(mpDataDescription->mClassSize);
			mpDataDescription->CopyConstruct(mpValue, other.mpValue);
		}
		else mpValue = NULL;
	}

	PropertyValue() : mpValue(NULL), mpDataDescription(NULL) {}

	~PropertyValue() {
		if(mpDataDescription && mpValue)
			mpDataDescription->Delete(mpValue);
	}

};

//.PROP FILES
class PropertySet {
public:

	static inline const char* kPropEntered = "Trigger Entered Callback";
	static inline const char* kPropExited = "Trigger Exited Callback";
	static inline const char* kPropTargetName = "Trigger Target Name";
	static inline const char* kPropEnabled = "Trigger Enabled";

	enum eClearContext {
		eClearContext_DEFAULT = 0,
		eClearContext_FOR_DESTRUCTOR = 1
	};

	enum GetKeyInfoFlags {
		eNone = 1,
		eCreateKeyIfNecessary = 0x2,
		eSearchParentsForKey = 0x4
	};

	enum eParentSearchMode {
		eSearchParents = 0x1,
		eDontSearchParents = 0x2
	};

	enum PropertyFlags { 
		eResetNavCamsOnExit = 0x1, 
		eBackgroundFade = 0x2, 
		eBackgroundLoop = 0x4, 
		eEndPause = 0x8, 
		eAdditive_Depreciated = 0x10, 
		eEmbedded = 0x20, 
		eChoreCut = 0x40,
	};

	enum PropertyLocalKeyOptions { eKeepLocalKeys = 0x0, eDiscardLocalKeys = 0x1, ePrompt = 0x2, };

	enum PropertyModifications { 
		eInModifiedList = 0x1, 
		eKeyAdded = 0x2, 
		eKeyRemoved = 0x4, 
		eKeyModified = 0x8, 
		eParentAdded = 0x10, 
		eParentRemoved = 0x20, 
		eKeyCallbacksPending = 0x40, 
		eChangedSinceSave = 0x80, 
		eForDestructor = 0x100, 
		eAnyChangeMask = 0x3E, };

	struct KeyInfo {
		Flags mFlags;
		Symbol mKeyName;
		std::shared_ptr<PropertyValue> mpValue;

		KeyInfo() {
			mFlags.mFlags = 0;
			mKeyName = {};
			mpValue = std::make_shared<PropertyValue>();
		}

		KeyInfo(const KeyInfo& o) {
			mFlags = o.mFlags;
			mKeyName = o.mKeyName;
			mpValue = o.mpValue;
		}

		inline bool operator<(const KeyInfo& other) const {
			return mKeyName.GetCRC() < other.mKeyName.GetCRC();
		}

		KeyInfo& operator=(const KeyInfo& other) {
			mFlags = other.mFlags;
			mKeyName = other.mKeyName;
			mpValue = other.mpValue;
			return *this;
		}

	};

	PropertySet() {
		this->mKeyMap = List<KeyInfo>();
		mParentList = List<ParentInfo>();
		mPropVersion = 0;
		mPropertyFlags.mFlags = 0;
		mHOI.mFlags.mFlags = 0;
		mEmbeddedToolProps = NULL;
		mHOI.mObjectName.SetCRC(0);
	}

	PropertySet(const PropertySet& rhs) {
		mPropVersion = rhs.mPropVersion;
		mPropertyFlags = rhs.mPropertyFlags;
		mKeyMap = rhs.mKeyMap;
		mParentList = rhs.mParentList;
		if (rhs.mEmbeddedToolProps) {
			mEmbeddedToolProps = new PropertySet;
			*mEmbeddedToolProps = *rhs.mEmbeddedToolProps;
		}
		else {
			mEmbeddedToolProps = NULL;
		}
		mHOI = rhs.mHOI;
	}

	PropertySet& operator=(const PropertySet& rhs) {
		mPropVersion = rhs.mPropVersion;
		mPropertyFlags = rhs.mPropertyFlags;
		mKeyMap = rhs.mKeyMap;
		mParentList = rhs.mParentList;
		mHOI = rhs.mHOI;
		if (rhs.mEmbeddedToolProps) {
			if (mEmbeddedToolProps)
				delete mEmbeddedToolProps;
			mEmbeddedToolProps = new PropertySet;
			*mEmbeddedToolProps = *rhs.mEmbeddedToolProps;
		}
		return *this;
	}

	struct ParentInfo {
		Handle<PropertySet> mhParent;
	};

	int mPropVersion;
	Flags mPropertyFlags;
	//Flags mModifiedFlags; //NOT SERIALIZED! could be useful?
	List<PropertySet::KeyInfo> mKeyMap;//type=>value
	List<ParentInfo> mParentList;//list of parent property set handle file references. in the engine these could be files (eTTArch) or just
	//memory references (eMemory). not implemented in this lib but useful to know for loaded .props 
	HandleObjectInfo mHOI;
	//in the prop reading code this was read into a temp prop and then forgotten. tool props?
	PropertySet* mEmbeddedToolProps;

	~PropertySet() {
		if (mEmbeddedToolProps)
			delete mEmbeddedToolProps;
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pCtx, void* pUserData) {
		if (!pUserData || !pDesc || !pObj)return eMetaOp_Fail;
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		PropertySet* prop = static_cast<PropertySet*>(pObj);
		if (stream->mMode == MetaStreamMode::eMetaStream_Write) {
			if(!(prop->mPropVersion == 1 || prop->mPropVersion == 2))
				prop->mPropVersion = 2;//LATEST VERSION!
			prop->mPropertyFlags.mFlags &= 0xFFBFFFFF;
		}
		MetaOpResult result = eMetaOp_Succeed;
		bool flag1 = (prop->mPropertyFlags.mFlags >> 12) & 1;
		bool flag2 = (prop->mPropertyFlags.mFlags >> 13) & 1;
		bool flag3 = ((prop->mPropertyFlags.mFlags & 0xFFFFCFFF) >> 15) & 1;
		if (prop->mEmbeddedToolProps)
			prop->mPropertyFlags.mFlags |= 0x400;
		if ((result = ::Meta::MetaOperation_SerializeAsync(pObj, pDesc, pCtx, pUserData)) != eMetaOp_Succeed)return result;
		if (prop->mPropVersion > 2)
			prop->mPropVersion = 1;
		/*if (stream->mMode == MetaStreamMode::eMetaStream_Read)
			prop->mPropertyFlags &= 0xFDFF7FFF;
		if (flag1)
			prop->mPropertyFlags |= 0x1000;
		else
			prop->mPropertyFlags &= 0xFFFFEFFF;
		if (flag2)
			prop->mPropertyFlags |= 0x2000;
		else
			prop->mPropertyFlags |= 0xFFFFDFFF;*/
		stream->BeginBlock();
		u32 parents = prop->mParentList.GetSize();
		stream->serialize_uint32(&parents);
		if (stream->mMode == MetaStreamMode::eMetaStream_Write) {
			HashDatabase* db = TelltaleToolLib_GetGlobalHashDatabase();
			if (!db && prop->mPropVersion == 1)return eMetaOp_SymbolNotFoundInDB;
			HashDatabase::Page* page = NULL;
			if (prop->mPropVersion == 1) {
				String pagen = "Files_PROP_";
				String gameid = sBlowfishKeys[sSetKeyIndex].game_id;
				std::transform(gameid.begin(), gameid.end(), gameid.begin(), ::toupper);
				pagen += gameid;
				page = db->FindPage(pagen.c_str());
				if (!page)return eMetaOp_SymbolNotFoundInDB;
			}
			for (int i = 0; i < parents; i++) {
				/*if (prop->mPropVersion == 1) {
					Symbol sym = prop->mParentList[i].
						mhParent.mHandleObjectInfo.mObjectName;
					String str;
					db->FindEntry(page, sym.GetCRC(), &str);
					if (str.empty())return eMetaOp_SymbolNotFoundInDB;
					stream->serialize_String(&str);
				}
				else if (prop->mPropVersion == 2) {
					Symbol sym = prop->mParentList[i].
						mhParent.mHandleObjectInfo.mObjectName;
					stream->serialize_Symbol(&sym);
				}*/
				Symbol sym = prop->mParentList[i].
					mhParent.mHandleObjectInfo.mObjectName;
				stream->serialize_Symbol(&sym);
			}
			if (prop->mPropVersion == 1) {
				stream->EndBlock();
				stream->BeginBlock();
			}
			Map<Symbol, List<KeyInfo>, Symbol::CompareCRC> typeMap;
			for (int i = 0; i < prop->mKeyMap.size(); i++) {
				KeyInfo mapping = prop->mKeyMap[i];
				Symbol typeSymbol = mapping.mpValue->mpDataDescription->GetDescriptionSymbol();
				auto it = typeMap.find(typeSymbol);
				if (it == typeMap.end()) {
					List<KeyInfo> l;
					l.AddElement(0, NULL, &mapping);
					typeMap.AddElement(0, &typeSymbol, &l);//copies
				}
				else {
					it->second.AddElement(0, NULL, &mapping);
				}
			}
			u32 size = typeMap.GetSize();
			stream->serialize_uint32(&size);
			for (auto it = typeMap.begin(); it != typeMap.end(); it++) {
				Symbol type = it->first;
				stream->serialize_Symbol(&type);
				List<KeyInfo> list = it->second;
				size = list.GetSize();
				stream->serialize_uint32(&size);
				for (int i = 0; i < size; i++) {
					KeyInfo keyInfo = list[i];
					stream->serialize_Symbol(&keyInfo.mKeyName);
					PerformMetaSerializeFull(stream, keyInfo.mpValue->mpValue,
						keyInfo.mpValue->mpDataDescription); 
				}
			}
			if (prop->mEmbeddedToolProps)
				result=PerformMetaSerializeFull(stream, prop->mEmbeddedToolProps, pDesc);
		}
		else if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
			for (int i = 0; i < parents; i++) {
				/*if (prop->mPropVersion == 1) {
					String str;
					stream->serialize_String(&str);
					Symbol sym(str.c_str());
					prop->mParentList.AddElement(0, NULL, &sym);
				}
				else if (prop->mPropVersion == 2) {
					Symbol sym;
					stream->serialize_Symbol(&sym);
					prop->mParentList.AddElement(0, NULL, &sym);
				}*/
				Symbol sym;
				stream->serialize_Symbol(&sym);
				prop->mParentList.AddElement(0, NULL, &sym);
			}
			if (prop->mPropVersion == 1) {
				stream->EndBlock();
				stream->BeginBlock();
			}
			u32 numtypes = 0;
			u32 numvalues = 0;
			stream->serialize_uint32(&numtypes);
			for (int i = 0; i < numtypes; i++) {
				Symbol typeSymbol;
				stream->serialize_Symbol(&typeSymbol);
				MetaClassDescription* typeDesc = 
					TelltaleToolLib_FindMetaClassDescription_ByHash
						(typeSymbol.GetCRC());
				if (!typeDesc) {
#ifdef DEBUGMODE
					printf("Property class type not found for type with symbol"
						" %llX at (default section) : 0x%X!\n", typeSymbol.GetCRC(),stream->GetPos());
#endif
					TelltaleToolLib_RaiseError("Property type in property set not supported (run in debug for to print the CRC)", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
				stream->serialize_uint32(&numvalues);
				MetaOpResult opres = eMetaOp_Succeed;
				void* (*metaTypedNew)(void);
				for (int i = 0; i < numvalues; i++) {
					KeyInfo property{};
					stream->serialize_Symbol(&property.mKeyName);
					property.mpValue->mpDataDescription = typeDesc;
					metaTypedNew = (void*(*)(void))typeDesc->mpVTable[0];
					if (!metaTypedNew)return eMetaOp_Fail;//ABSTRACT! no
					property.mpValue->mpValue = metaTypedNew();
					if((opres=PerformMetaSerializeFull(stream, 
						property.mpValue->mpValue,typeDesc)) 
						!= eMetaOp_Succeed) return opres;
					prop->mKeyMap.AddElement(0, NULL, &property);
				}
			}
			if (prop->mPropertyFlags.mFlags & 0x400) {
				if (!prop->mEmbeddedToolProps)
					prop->mEmbeddedToolProps = new PropertySet;
				result=PerformMetaSerializeFull(stream, prop->mEmbeddedToolProps, pDesc);
			}
		}
		stream->EndBlock();
		if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
			if (flag3)
				prop->mPropertyFlags.mFlags |= 0x8000;
			else
				prop->mPropertyFlags.mFlags &= 0xFFFF7FFF;
		}
		return result;
	}

	void ClearKeys() {
		mKeyMap.clear();
	}

	void ClearParents() {
		mParentList.ClearElements();
	}

	void RemoveParent(Handle<PropertySet> handle) {
		for (auto it = mParentList.begin(); it != mParentList.end(); it++) {
			if (it->mhParent.mHandleObjectInfo.mObjectName == handle.mHandleObjectInfo.mObjectName) {
				mParentList.erase(it);
				return;
			}
		}
	}

	void AddParent(Handle<PropertySet> handle) {
		ParentInfo info;
		info.mhParent = handle;
		mParentList.AddElement(0, NULL, &info);
	}

	bool HasProperty(u64 crc) {
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc)return true;
		}
		return false;
	}

	bool HasProperty(const char* keyName) {
		u64 crc = CRC64_CaseInsensitive(0, keyName);
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc)return true;
		}
		return false;
	}

	void* GetProperty(u64 crc) {
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc)
				return it->mpValue->mpValue;
		}
		return NULL;
	}

	void SetProperty(u64 crc, void* val) {
		for (List<PropertySet::KeyInfo>::iterator it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc) {
				it->mpValue->SetData(val, NULL);
			}
		}
	}

	void* GetProperty(const char* keyName) {
		u64 crc = CRC64_CaseInsensitive(0, keyName);
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc)
				return it->mpValue->mpValue;
		}
		return NULL;
	}

	template<typename T> T* GetProperty(const Symbol keyName) {
		u64 crc = keyName.GetCRC();
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc)
				return it->mpValue->CastValue<T>();
		}
		return NULL;
	}

	template<typename T> T* GetProperty(const char* keyName) {
		u64 crc = CRC64_CaseInsensitive(0, keyName);
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc)
				return it->mpValue->CastValue<T>();
		}
		return NULL;
	}

	u32 GetNumKeys() {
		return mKeyMap.size();
	}

	u32 GetNumPropertiesOfType(MetaClassDescription* type) {
		u32 ret = 0;
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mpValue->mpDataDescription && it->mpValue->mpDataDescription == type)
				ret++;
		}
		return ret;
	}


	template<typename T> u32 GetNumPropertiesOfTypeT() {
		MetaClassDescription* desc = GetMetaClassDescription(typeid(T).name());
		if (!desc)return 0;
		return GetNumPropertiesOfType(desc);
	}

	void AddProperty(u64 crc, MetaClassDescription* desc, void* value) {
		if (!desc)return;
		KeyInfo k;
		k.mKeyName = crc;
		k.mpValue->SetData(value, desc);
		mKeyMap.AddElement(0, NULL, &k);
	}

	bool ExistsKey(const Symbol& pKeyName, bool pSearchParents) {
		for (int i = 0; i < mKeyMap.GetSize(); i++) {
			if (mKeyMap[i].mKeyName == pKeyName)
				return true;
		}
		if (pSearchParents) {
			TelltaleToolLib_RaiseError("Cannot search parents,"
				" cannot load external file", ErrorSeverity::NOTIFY);
		}
		return false;
	}

	void CreateKey(const Symbol& KeyName, MetaClassDescription* pDesc) {
		KeyInfo k;
		k.mpValue->SetData(NULL, pDesc);
		k.mKeyName = KeyName;
		mKeyMap.AddElement(0, NULL, &k);
	}

	void AddProperty(const char* keyName, MetaClassDescription* desc, void* value) {
		if (!keyName || !desc)return;
		KeyInfo k;
		k.mKeyName = CRC64_CaseInsensitive(0, keyName);
		k.mpValue->SetData(value, desc);
		mKeyMap.AddElement(0, NULL, &k);
	}

	template<typename T> void AddProperty(const char* keyName, T* value) {
		MetaClassDescription* desc = GetMetaClassDescription(typeid(T).name());
		if (!desc) {
#ifdef DEBUGMODE
			printf("Could not find meta class description for type '%s'\n", typeid(T).name());
#endif
			return;
		}
		AddProperty(keyName, desc, value);
	}

	bool RemoveProperty(u64 crc) {
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc) {
				mKeyMap.erase(it);
				return true;
			}
		}
		return false;
	}

	bool RemoveProperty(const char* keyName) {
		u64 crc = CRC64_CaseInsensitive(0, keyName);
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mKeyName.GetCRC() == crc) {
				mKeyMap.erase(it);
				return true;
			}
		}
		return false;
	}

	List<KeyInfo> GetPropertiesOfType(MetaClassDescription* pObjDesc) {
		if (!pObjDesc)return List<KeyInfo>();
		List<KeyInfo> keyInfo;
		for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++) {
			if (it->mpValue->mpDataDescription && it->mpValue->mpDataDescription == pObjDesc)
				keyInfo.push_back(*it);
		}
		return keyInfo;
	}

	template<typename T> List<KeyInfo> GetPropertiesOfTypeT() {
		return GetPropertiesOfType(GetMetaClassDescription(typeid(T).name()));
	}

};

#endif