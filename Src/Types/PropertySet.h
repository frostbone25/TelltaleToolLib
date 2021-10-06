// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../Meta.hpp"
#include "Set.h"
#include "List.h"
#include "Map.h"
#include "HandleObjectInfo.h"

#ifndef _PROP
#define _PROP

struct PropertyValue {
	MetaClassDescription* mpDataDescription;
	void* mpValue;
	//union {
	//	char mStaticBuf[40];
	//	char* mpBuf;
	//};
};

//.PROP FILES
class PropertySet {
public:

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
		PropertyValue mValue;

		inline bool operator<(const KeyInfo& other) const {
			return mKeyName.GetCRC() < other.mKeyName.GetCRC();
		}

	};

	struct ParentInfo {
		Handle<PropertySet> mhParent;
	};

	int mPropVersion;
	Flags mPropertyFlags;
	//Flags mModifiedFlags; //NOT SERIALIZED! could be useful?
	Set<PropertySet::KeyInfo> mKeyMap;//type=>value
	List<ParentInfo> mParentList;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pCtx, void* pUserData) {
		if (!pUserData || !pDesc || !pObj)return eMetaOp_Fail;
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		PropertySet* prop = static_cast<PropertySet*>(pObj);
		if (stream->mMode == MetaStreamMode::eMetaStream_Write) {
			prop->mPropVersion = 2;//LATEST VERSION!
			prop->mPropertyFlags.mFlags &= 0xFFBFFFFF;
		}
		MetaOpResult result = eMetaOp_Succeed;
		bool flag1 = (prop->mPropertyFlags.mFlags >> 12) & 1;
		bool flag2 = (prop->mPropertyFlags.mFlags >> 13) & 1;
		bool flag3 = ((prop->mPropertyFlags.mFlags & 0xFFFFCFFF) >> 15) & 1;
		if ((result = ::Meta::MetaOperation_SerializeAsync(pObj, pDesc, pCtx, pUserData)) != eMetaOp_Succeed)return result;
		if (prop->mPropVersion > 2)
			prop->mPropVersion = 1;
		if (stream->mMode == MetaStreamMode::eMetaStream_Read)
			prop->mPropertyFlags &= 0xFDFF7FFF;
		if (flag1)
			prop->mPropertyFlags |= 0x1000;
		else
			prop->mPropertyFlags &= 0xFFFFEFFF;
		if (flag2)
			prop->mPropertyFlags |= 0x2000;
		else
			prop->mPropertyFlags |= 0xFFFFDFFF;
		stream->BeginBlock();
		u32 parents = prop->mParentList.GetSize();
		stream->serialize_uint32(&parents);
		if (stream->mMode == MetaStreamMode::eMetaStream_Write) {
			for (int i = 0; i < parents; i++) {
				Symbol sym = prop->mParentList[i].
					mhParent.mHandleObjectInfo.mObjectName;
				stream->serialize_Symbol(&sym);
			}
			Map<Symbol, List<KeyInfo>> typeMap;
			for (int i = 0; i < prop->mKeyMap.size(); i++) {
				KeyInfo mapping = prop->mKeyMap[i];
				Symbol typeSymbol = mapping.mValue.mpDataDescription->GetDescriptionSymbol();
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
					PerformMetaSerializeFull(stream, keyInfo.mValue.mpValue,
						keyInfo.mValue.mpDataDescription); 
				}
			}
		}
		else if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
			for (int i = 0; i < parents; i++) {
				Symbol sym;
				stream->serialize_Symbol(&sym);
				prop->mParentList.AddElement(0, NULL, &sym);
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
				if (!typeDesc)return eMetaOp_Fail;
				stream->serialize_uint32(&numvalues);
				MetaOpResult opres = eMetaOp_Succeed;
				void* (*metaTypedNew)(void);
				for (int i = 0; i < numvalues; i++) {
					KeyInfo property;
					stream->serialize_Symbol(&property.mKeyName);
					property.mValue.mpDataDescription = typeDesc;
					metaTypedNew = (void*(*)(void))typeDesc->mpVTable[0];
					if (!metaTypedNew)return eMetaOp_Fail;//ABSTRACT! no
					property.mValue.mpValue = metaTypedNew();
					if((opres=PerformMetaSerializeFull(stream, 
						property.mValue.mpValue,typeDesc)) 
						!= eMetaOp_Succeed) return opres;
					prop->mKeyMap.AddElement(0, NULL, &property);
				}
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

};

#endif