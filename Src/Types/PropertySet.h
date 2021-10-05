// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../Meta.hpp"
#include "Set.h"
#include "List.h"
#include "HandleObjectInfo.h"

struct PropertyValue {
	MetaClassDescription* mpDataDescription;
	void* pValue;
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
		if ((result = Meta::MetaOperation_SerializeAsync(pObj, pDesc, pCtx, pUserData)) != eMetaOp_Succeed)return result;
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
		PerformMetaSerializeAsync<List<ParentInfo>>(stream, &prop->mParentList);
		if (stream->mMode == MetaStreamMode::eMetaStream_Write) {
				//TODO make list of types from mKeyMap, then write the values
		}
		else if (stream->mMode == MetaStreamMode::eMetaStream_Read) {

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