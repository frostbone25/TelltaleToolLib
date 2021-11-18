// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _CHORE
#define _CHORE

#include "../Meta.hpp"
#include "DCArray.h"
#include "Map.h"
#include "LocalizationRegistry.h"
#include "PropertySet.h"
#include "ObjectSpaceIntrinsics.h"
#include "Rules.h"
#include "ToolProps.h"
#include "Animation.h"

//.LOOK FILES
struct Procedural_LookAt {

	struct EnumLookAtComputeStage : EnumBase {
		long mVal;//0 = idle look at, 1 = dialog chore look at, 2 = final look at
	};

	String mHostNode;
	String mTargetAgent;
	String mTargetNode;
	Vector3 mTargetOffset;
	bool mbUsePrivateNode;
	AnimOrChore mhXAxisChore;
	AnimOrChore mhYAxisChore;
	bool mbRotateHostNode;
	float mLastLRAngle;
	float mLastUDAngle;
	float mLastLRWeight;
	float mLastUDWeight;
	EnumLookAtComputeStage mLookAtComputeStage;

};

struct WalkPath {
	String mName;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(WalkPath, path);
		//TODO WALKPATH
		return eMetaOp_Succeed;
	}

};

template<int N>
struct DependencyLoader {
	DCArray<String>* mpResNames;

	DependencyLoader() {
		mpResNames = NULL;
	}

	~DependencyLoader() {
		if (mpResNames)
			delete mpResNames;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(DependencyLoader<N>, loader);
		bool b = loader->mpResNames != NULL;
		meta->serialize_bool(&b);
		static MetaClassDescription* _DCArrayString = NULL;
		if (!_DCArrayString) {
			_DCArrayString = GetMetaClassDescription<DCArray<String>>();
			if (!_DCArrayString) {
				TelltaleToolLib_RaiseError("Cannot serialize DependencyLoader<1>: Meta uninitialized (MCD for DCArray<String> not found)",
					ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
		}
		if (b) {
			if (meta->IsWrite()) {
				meta->serialize_uint64(_DCArrayString->mHash);
				PerformMetaSerializeFull(meta, loader->mpResNames, _DCArrayString);
			}
			else {
				u64 metacrc = 0;
				meta->serialize_uint64(&metacrc);
				MetaClassDescription* mcd = TelltaleToolLib_FindMetaClassDescription_ByHash(metacrc);
				if (mcd == NULL) {
					TelltaleToolLib_RaiseError("Meta class type not found in DependencyLoader<1>", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
				if (loader->mpResNames)
					delete loader->mpResNames;
				void* newArray = mcd->New();
				if (!newArray)
					return eMetaOp_OutOfMemory;
				if (PerformMetaSerializeFull(meta, newArray, mcd) == eMetaOp_Succeed) {
					loader->mpResNames = static_cast<DCArray<String>*>(mcd->CastToBase(newArray, _DCArrayString));
				}
			}
		}
		return eMetaOp_Succeed;
	}

};

struct ActorAgentBinding {
	String mActorName;

	ActorAgentBinding() = default;
	~ActorAgentBinding() = default;

	void Clear() {
		mActorName.clear();
	}

};

struct AutoActStatus {
	long m_Status;

	bool operator!=(const AutoActStatus& other) {
		return other.m_Status != m_Status;
	}

	bool operator==(const AutoActStatus& other) {
		return other.m_Status == m_Status;
	}

	int Lev1Status() {
		return m_Status > 2 ? 1 : m_Status;
	}

};

//.CHORE FILES
struct Chore {

	struct ChoreResource {

		struct Block {
			float mStartTime;
			float mEndTime;
			bool mbLoopingBlock;
			float mScale;
			bool mbSelected;//not serialized
		};

		Chore* mpChore;
		long mVersion;
		Symbol mResName;
		float mResLength;
		long mPriority;
		Flags mFlags;
		String mResourceGroup;
		HandleBase mhObjec;
		//by lib, for embedded data
		void* mhObjectEmbedded;
		MetaClassDescription* mhObjectDesc;
		//
		Animation mControlAnimation;
		DCArray<Block> mBlocks;
		bool mbNoPose;
		bool mbEmbedded;
		bool mbEnabled;
		bool mbIsAgentResource;
		bool mbViewGraphs;
		bool mbViewEmptyGraphs;
		bool mbViewProperties;
		bool mbViewResourceGroups;
		PropertySet mResourceProperties;
		Map<Symbol, float, Symbol::CompareCRC> mResourceGroupInclude;
		AutoActStatus mAAStatus;

		enum ChoreFlags {
			eFilterMoverAnimation = 1,
			eForceInclusionOfMoverAnimation = 2,
			eIsThisChore = 4,
			eMoodIdlePersists = 8,
			e_LibInterpreted_IsChoreCut = 0x1000
		};

		Flags mFlags;

		ChoreResource() {
			mhObjectEmbedded = NULL;
			mhObjectDesc = NULL;
		}

		INLINE void SetPriority(int iPriority) {
			mPriority = iPriority;
		}

		INLINE void SetIsAgentResource(bool bIsAgentResource) {
			mbIsAgentResource = bIsAgentResource;
		}
		
		INLINE void SetChore(Chore* pChore) {
			mpChore = pChore;
		}

		INLINE void SetEmbedded(Symbol embeddedName) {
			if (embeddedName.GetCRC()) {
				mbEmbedded = true;
				SetResourceName(embeddedName, NULL, false);
			}
			else {
				mbEmbedded = false;
			}
		}

		INLINE void SetResourceHandle(void* data, MetaClassDescription* desc, bool bDeleteOldData) {
			if (bDeleteOldData && mhObjectEmbedded && mhObjectDesc)
				mhObjectDesc->Delete(mhObjectEmbedded);
			mhObjectEmbedded = data;
			mhObjectDesc = desc;
		}

		void SetResourceName(Symbol newName, MetaClassDescription* pDesc, bool bSetResourceNameOnly) {
			mResName = newName;
		}

		static METAOP_FUNC_IMPL__(SerializeAsync) {
			MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
			MetaStream* meta = static_cast<MetaStream*>(pUserData);
			ChoreResource* choreres = static_cast<ChoreResource*>(pObj);
			if (r == eMetaOp_Succeed) {
				if (meta->IsWrite() && choreres->mbEmbedded) {
					MetaClassDescription* desc = choreres->mhObjectDesc;
					meta->serialize_uint64(&desc->mHash);
					meta->serialize_uint64(&desc->mHash);
					r=PerformMetaSerializeFull(meta, choreres->mhObjectEmbedded, choreres->mhObjectDesc);
					if (r != eMetaOp_Succeed)
						return r;
				}
				u64 hash = 0;
				MetaClassDescription* embedt = NULL;
				if (!meta->IsRead())
					return r;//done writing
				if (!choreres->mbEmbedded)
					return r;//no embed, we are done here
				meta->serialize_uint64(&hash);
				meta->serialize_uint64(&hash);
				if (choreres->mVersion) {
					embedt = TelltaleToolLib_FindMetaClassDescription_ByHash(hash);
				}
				else {
					String s;
					meta->serialize_String(&s);
					embedt = TelltaleToolLib_FindMetaClassDescription_ByHash(CRC64_CaseInsensitive(0, s.c_str()));
				}
				if (!embedt) {
					TelltaleToolLib_RaiseError("Could not locate meta class description from CRC in embedded chore data", ErrorSeverity::ERR);
					return eMetaOp_Invalid;
				}
				void* obj = embedt->New();
				if (!obj) {
					return eMetaOp_OutOfMemory;
				}
				if (choreres->mVersion >= 2 || embedt != GetMetaClassDescription<Procedural_LookAt>()) {
					PerformMetaSerializeFull(meta, obj, embedt);
				}
				else {
					Animation temp;
					MetaClassDescription* anmdesc = GetMetaClassDescription<Animation>();
					PerformMetaSerializeFull(meta, &temp, anmdesc);
					meta->mRuntimeFlags.mFlags |= 1u;
				}
				choreres->SetResourceHandle(obj, embedt, true);
			}
			return r;
		}

	};

	struct ChoreAgent {

		struct Attachment {
			bool mbDoAttach;
			String mAttachTo;
			String mAttachToNode;
			Vector3 mAttachPos;
			Quaternion mAttachQuat;
			bool mbAttachPreserveWorldPos;
			bool mbLeaveAttachedWhenComplete;
		};

		Chore* mpChore;
		String mAgentName;
		Flags mFlags;
		DCArray<int> mResources;
		Attachment mAttachment;
		ActorAgentBinding mAABinding;
		Rule mAgentEnabledRule;

		void SetIsThisChore(bool onOff) {
			int f = mFlags.mFlags | 4;
			if (!onOff)
				f = mFlags.mFlags & 0xFFFFFFFB;
			mFlags.mFlags = f;
		}

		INLINE int GetNumResources() {
			return mResources.GetSize();
		}

		INLINE int GetResourceID(int index) {
			return *(mResources.mpStorage + index);
		}

		INLINE void SetChore(Chore* pChore) {
			mpChore = pChore;
		}

		INLINE String* GetAgentName() {
			return &mAgentName;
		}

		INLINE void SetAgentName(const String& agentName) {
			mAgentName.assign(agentName);
		}

		static METAOP_FUNC_IMPL__(SerializeAsync) {
			MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
			return r;
		}

	};

	String mName;
	Flags mFlags;
	float mLength;
	long mNumResources;
	long mNumAgents;
	PropertySet mEditorProps;
	String mChoreSceneFile;
	long mRenderDelay;
	LocalizeInfo mSynchronizedToLocalization;
	DependencyLoader<1> mDependencies;
	ToolProps mToolProps;
	Map<Symbol, WalkPath, Symbol::CompareCRC> mWalkPaths;
	//DO NOT ADD TO THIS, USE FUNCTIONS
	DCArray<ChoreResource*> mPtrResources;
	//DO NOT ADD TO THIS, USE FUNCTIONS
	DCArray<ChoreAgent*> mPtrAgents;
	//If you want to add to this, make sure you add a resource from the return value of AddResource
	DCArray<ChoreResource*> mChoreCutResources;

	enum AddResBehaviour {
		eCreateAgents = 1,
		eAddToAgents = 2,
		eQueryUser = 4,
		eSkipIfExists = 8,
		eIsAgentResource = 0x10,
		eCreateDefaultAgent = 0x20,
		eAddEmbedded = 0x40,
		eAddClip = 0x80
	};

	INLINE int FindAgent(Symbol agentName) {
		Symbol compare{};
		for (int i = 0; i < mPtrAgents.GetSize(); i++) {
			compare.SetCRC(CRC64_CaseInsensitive(0,(*(mPtrAgents.mpStorage + i))->GetAgentName()->c_str()));
			if (compare.GetCRC() == agentName.GetCRC())
				return i;
		}
		return -1;
	}

	u32 GetNumResources() {
		return mNumResources = mPtrResources.GetSize();
	}

	INLINE constexpr float GetChoreFadeTime() {
		return 0.25f;
	}

	bool IsValidResource(int id) {
		return id >= 0 && id < mPtrResources.GetSize();
	}

	ChoreResource* GetResource(int id) {
		return *(mPtrResources.mpStorage + id);
	}

	ChoreAgent* GetAgent(int id) {
		return *(mPtrAgents.mpStorage + id);
	}

	ChoreAgent* AddAgent(const String& agentName) {
		if (FindAgent(Symbol(agentName)) != -1)
			return NULL;
		ChoreAgent* agent = new ChoreAgent();
		mPtrAgents.AddElement(0, NULL, &agent);
		agent->SetChore(this);
		agent->SetAgentName(agentName);
		return agent;
	}

	//see flags in addresbehaviour for behaviour
	ChoreResource* AddResource(Symbol resName, MetaClassDescription* pDesc, Flags behaviour, void* hEmbeddedData, MetaClassDescription* hEmbeddedType) {
		ChoreResource* ret = new ChoreResource();
		mPtrResources.AddElement(0, NULL, &ret);
		ret->SetChore(this);
		ret->SetIsAgentResource((behaviour.mFlags & AddResBehaviour::eIsAgentResource) != 0);
		if ((behaviour.mFlags & AddResBehaviour::eAddEmbedded) != 0) {
			ret->SetResourceHandle(hEmbeddedData, hEmbeddedType, false);
			ret->SetEmbedded(resName);
		}
		ret->SetResourceName(resName, pDesc, false);
		//if (((behaviour.mFlags & AddResBehaviour::eCreateAgents) != 0) && pDesc == GetMetaClassDescription<Chore>()) {
//
		//}
		return ret;
	}

	void _DeletePtrs() {
		if (mPtrResources.GetSize() > 0) {
			for (int i = 0; i < mPtrResources.GetSize(); i++) {
				delete mPtrResources[i];
			}
			mPtrResources.~DCArray();
			mPtrResources = DCArray<ChoreResource*>();
		}
		if (mPtrAgents.GetSize() > 0) {
			for (int i = 0; i < mPtrAgents.GetSize(); i++) {
				delete mPtrAgents[i];
			}
			mPtrAgents.~DCArray();
			mPtrAgents = DCArray<ChoreAgent*>();
		}
		for (int i = 0; i < mChoreCutResources.GetSize(); i++) {
			*(mChoreCutResources.mpStorage + i) = NULL;
		}
	}

	~Chore() {
		_DeletePtrs();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		Chore* chore = static_cast<Chore*>(pObj);
		if (r == eMetaOp_Succeed) {
			if (meta->IsWrite()) {
				chore->mNumAgents = chore->mPtrAgents.GetSize();
				chore->mNumResources = chore->mPtrResources.GetSize();
				for (int i = 0; i < chore->mChoreCutResources.GetSize(); i++) {
					chore->mChoreCutResources[i]->mFlags.mFlags |= ChoreResource::ChoreFlags::e_LibInterpreted_IsChoreCut;
				}
			}
			for (int i = 0; i < chore->mNumResources; i++) {
				ChoreResource* resource;
				if (meta->IsRead()) {
					resource = new ChoreResource();
					resource->SetChore(chore);
					chore->mPtrResources.AddElement(0, NULL, &resource);
				}
				else {
					resource = chore->mPtrResources[i];
				}
				r=PerformMetaSerializeAsync<ChoreResource>(meta, resource);
				if (r != eMetaOp_Succeed)
					return r;
				if (meta->IsRead() && ((resource->mFlags.mFlags & ChoreResource::ChoreFlags::e_LibInterpreted_IsChoreCut) != 0)) {
					chore->mChoreCutResources.AddElement(0, NULL, &resource);
				}
			}
			for (int i = 0; i < chore->mNumAgents; i++) {
				ChoreAgent* Agent;
				if (meta->IsRead()) {
					Agent = new ChoreAgent();
					Agent->SetChore(chore);
					chore->mPtrAgents.AddElement(0, NULL, &Agent);
				}
				else {
					Agent = chore->mPtrAgents[i];
				}
				r = PerformMetaSerializeAsync<ChoreAgent>(meta, Agent);
				if (r != eMetaOp_Succeed)
					return r;
			}
			//if (!(chore->mFlags.mFlags & 0x60)) {}
		}
		return r;
	}

};

#endif