// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _DLG
#define _DLG

#include "../Meta.hpp"
#include "LanguageDB.h"
#include "PropertySet.h"
#include "Map.h"
#include "HandleObjectInfo.h"
#include "Chore.h"
#include "Rules.h"

//SEE https://www.atlassian.com/software/jira - telltale API for using it
struct JiraRecord {
	//contains runtime data, not needed
	size_t _empty;
};

//SEE https://www.atlassian.com/software/jira - telltale API for using it
struct JiraRecordManager {
	Map<String, JiraRecord*, std::less<String>> mRecords;

	void _DeleteData() {
		for (int i = 0; i < mRecords.GetSize(); i++) {
			delete mRecords[i].second;
		}
		mRecords.ClearElements();
	}

	~JiraRecordManager() {
		_DeleteData();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(JiraRecordManager, jira);
		if (meta->BeginDebugSection()) {
			if (meta->IsRead())
				jira->_DeleteData();
			u32 num = jira->mRecords.GetSize();
			meta->serialize_uint32(&num);
			if (meta->IsRead()) {
				String str{};
				JiraRecord *record = new JiraRecord;
				for (int i = 0; i < num; i++) {
					meta->serialize_String(&str);
					jira->mRecords.AddElement(NULL, &str, &record);
				}
			}
			else {
				String p;
				for (int i = 0; i < num; i++) {
					p = jira->mRecords[i].first;
					meta->serialize_String(&p);
				}
			}
			meta->EndDebugSection();
		}
		return eMetaOp_Succeed;
	}

};

struct DlgObjID {
	Symbol mID;
};

struct DlgObjIDOwner {

	DlgObjID mDlgID;

};


struct DlgObjectProps {

	enum PropsTypeT {
		eUserProps = 1,
		eProductionProps = 2,
		eToolProps = 4
	};
	
	PropertySet* mpUserProps, *mpProductionProps, *mpToolProps;
	Flags mFlags;//PropsTypeT, makes sense to be an enum oh well

	DlgObjectProps() {
		mpUserProps = NULL;
		mpProductionProps = NULL;
		mpToolProps = NULL;
	}

	void _DeleteData() {
		if (mpUserProps)
			delete mpUserProps;
		if (mpProductionProps)
			delete mpProductionProps;
		if (mpToolProps)
			delete mpToolProps;
		mpUserProps = NULL;
		mpProductionProps = NULL;
		mpToolProps = NULL;
		mFlags.mFlags = 0;
	}

	~DlgObjectProps() {
		_DeleteData();
	}

};

struct DlgObjectPropsOwner {
	DlgObjectProps mDlgObjectProps;
};

namespace DlgConstants {
	enum ChainContextTypeID {
		eCCUnspecified = 1,
		eCCAction = 2,
		eCCData = 3
	};
	enum DlgNodeClassID {
		eNodeClassStart = 0,
		eNodeChoices = 1,
		eNodeConditional = 2,
		eNodeChore = 3,
		eNodeExchange = 4,
		eNodeExit = 5,
		eNodeIdle = 6,
		eNodeJump = 7,
		eNodeLogic = 8,
		eNodeScript = 9,
		eNodeSequence = 10,
		eNodeStart = 11,
		eNodeText = 12,
		eNodeWait = 13,
		eNodeNotes = 14,
		eNodeChancelChoices = 15,
		eNodeParallel = 0x10,
		eNodeMarker = 0x11,
		eNodeStoryboard = 0x12,
		eNodeStats = 0x13,
		eNodeClassLast = 0x14,
		eNodeClassInvalid = 0x15,
		eNodeClassAll = 0x16,
		eChildChoices = 0x17,
		eChildConditional = 0x18
	};
	enum DlgChildClassID {
		eChildClassStart = 0x63,
		eChildClassChoice = 0x64,
		eChildClassChoicesChildPre = 0x65,
		eChildClassChoicesChildPost = 0x66,
		eChildClassElement = 0x67,
		eChildClassCase = 0x68,
		eChildClassFolderChild = 0x69,
		eChildClassPElement = 0x6A,
		eChildClassCohort = 0x6B,
		eChildClassLast = 0x6C
	};
	//enum DlgClassAIID {
		//typedefd! D: cant see values
	//};
	enum DlgObjectClassFlag {
		eObjClassInvalid = 0,
		eObjClassDialog = 1,
		eObjClassFolder = 2,
		eObjClassNodeChoices = 4,
		eObjClassNodeConditional = 8,
		eObjClassNodeChore = 16,
		eObjClassNodeExchange = 32,
		eObjClassNodeExit = 64,
		eObjClassNodeIdle = 128,
		eObjClassNodeJump = 256,
		eObjClassNodeLogic = 512,
		eObjClassNodeScript = 1024,
		eObjClassNodeSequence = 2048,
		eObjClassNodeStart = 4096,
		eObjClassNodeText = 8192,
		eObjClassNodeWait = 16384,
		eObjClassNodeNotes = 0x8000,
		eObjClassNodeChancelChoices = 0x10000,
		eObjClassNodeParallel = 0x20000,
		eObjClassNodeMarker = 0x40000,
		eObjClassNodeStoryboard = 0x80000,
		eObjClassNodeStats = 0x100000,
		eObjClassChildChoices = 0x200000,
		eObjClassChildChoicesPre = 0x400000,
		eObjClassChildChoicesPost = 0x800000,
		eObjClassChildSequence = 0x1000000,
		eObjClassChildConditional = 0x2000000,
		eObjClassChildFolder = 0x4000000,
		eObjClassChildParallel = 0x8000000,
		eObjClassChildStats = 0x10000000
	};
}

struct DlgNodeLink : DlgObjIDOwner {//no UID
	DlgConstants::ChainContextTypeID mRequiredCCType;//chain context type
};

struct DlgChainHead : DlgObjIDOwner {
	DlgNodeLink mLink;
};

struct DlgDownstreamVisibilityConditions {
	Flags mNodeTypeFlags;
	long mMaxNumNodeEvals;
};

struct DlgVisibilityConditions {
	bool mbDiesOff;
	Flags mFlags;
	DlgDownstreamVisibilityConditions mDownstreamVisCond;
	String mScriptVisCond;//script function (LUA)
};

struct DlgVisibilityConditionsOwner {
	DlgVisibilityConditions mVisCond;
};

struct DlgChild : DlgChainHead, DlgVisibilityConditionsOwner, DlgObjectPropsOwner{
	DlgNodeLink mParent;
};

struct DlgChildSet {
	DCArray<DlgChild*> mChildren;
	DlgNodeLink mParent;

	void _DeleteData() {
		for (int i = 0; i < mChildren.GetSize(); i++) {
			delete mChildren[i];
		}
		mChildren.ClearElements();
	}

	~DlgChildSet() {
		_DeleteData();
	}	

};

struct DlgFolder : DlgObjIDOwner, DlgObjectPropsOwner, DlgChildSet, UID::Owner {

	Symbol mName;
	PropertySet mProdReportProps;//production report properties

	MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgFolder>();
	}

};

struct DlgNode : DlgObjIDOwner, DlgVisibilityConditionsOwner, DlgObjectPropsOwner, UID::Owner {

	DlgNodeLink mPrev, mNext;
	Symbol mName;
	Flags mFlags;
	DlgConstants::ChainContextTypeID mChainContextTypeID;

	virtual ~DlgNode() {}

	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgNode>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() {
		return DlgConstants::DlgNodeClassID::eNodeClassInvalid;
	}

};

struct DlgNodeStart : DlgNode {

	PropertySet mProdReportProps;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeStart>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeStart;
	}

};

struct DlgNodeCancelChoices : DlgNode {

	enum CancelGroupT {
		eAllActiveChoices = 1,
	};

	CancelGroupT mCancelGroup;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeCancelChoices>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeChancelChoices;
	}

};

//TYPES TODO 

struct DlgChildSetChoice {

};

struct DlgChildSetChoicesChildPre {

};

struct DlgChildSetChoicesChildPost {

};

struct DlgChildSetConditionalCase {

};

struct NoteCollection {//add to its own header if its a file type with extension (i think its .notes)

};

struct DlgLineCollection {

};

// --
//note dlgnodejump is below Dlg struct since it uses it handle

//start from notes


struct DlgNodeLogic : DlgNode {

	Rule mRule;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeLogic>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeLogic;
	}

};

struct DlgNodeIdle : DlgNode {

	enum OverrideOption {
		eUseDefaults = 1,
		eOverride = 2
	};

	struct EnumOverrideOption : EnumBase {
		OverrideOption mVal;
	};

	Handle<Chore> mhIdle;
	float mTransitionTimeOverride;
	int mTransitionStyleOverride;
	int mIdleSlot;
	EnumOverrideOption mOverrideOptionTime, mOverrideOptionStyle;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeIdle>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeIdle;
	}

};

struct DlgNodeExit : DlgNode {

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeExit>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeExit;
	}

};

struct DlgNodeExchange : DlgNode {

	enum EntryType {
		eLine = 1,
		eNote = 2
	};

	struct Entry {
		int mID;
		EntryType mType;
	};
	
	Handle<Chore> mhChore;
	NoteCollection* mpNotes;
	DlgLineCollection* mpLines;
	DCArray<Entry> mEntries;

	DlgNodeExchange() {
		mpNotes = NULL;
		mpLines = NULL;
	}

	virtual ~DlgNodeExchange() {
		if (mpLines)
			delete mpLines;
		if (mpNotes)
			delete mpNotes;
		mpNotes = NULL;
		mpLines = NULL;
	}

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeExchange>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeExchange;
	}

};

struct DlgNodeChore : DlgNode {

	Handle<Chore> mhChore;
	int mPriority;
	bool mLooping;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeChore>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeChore;
	}

};

struct DlgNodeConditional : DlgNode {

	DlgChildSetConditionalCase mCases;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeConditional>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeConditional;
	}

};

struct DlgNodeChoices : DlgNode {

	DlgChildSetChoice mChoices;
	DlgChildSetChoicesChildPre mPreChoice;
	DlgChildSetChoicesChildPost mPostChoice;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeChoices>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeChoices;
	}

};

/*
struct DlgNode : DlgNode {

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNode>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNode;
	}

};*/

//.DLOG FILES
struct Dlg : DlgObjIDOwner, UID::Generator {//UID im not 100% sure since its only in older games, looks like a UID 

	DCArray<DlgFolder*> mFolders;
	DCArray<DlgNode*> mNodes;

	void _DeleteData() {
		for (int i = 0; i < mFolders.GetSize(); i++)
			delete mFolders[i];
		mFolders.ClearElements();
		for (int i = 0; i < mNodes.GetSize(); i++)
			delete mNodes[i];
		mNodes.ClearElements();
	}

	~Dlg() {
		_DeleteData();
	}
	
	String mName;
	long mVersion;
	DlgObjID mDefFolderID;
	LanguageDB mLangDB;
	unsigned long mProjectID;
	Symbol mResourceLocationID;
	long mChronology;
	Flags mFlags;//values in DlgNode::ClassFlags
	DependencyLoader<1> mDependencies;
	PropertySet mProdReportProps;
	JiraRecordManager mJiraRecordManager;
	bool mbHasToolOnlyData;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(Dlg, dlog);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
			pContextDescription, pUserData);
		MetaClassDescription* nodeDesc = ::GetMetaClassDescription<DlgNode>();
		if (!nodeDesc) {
			TelltaleToolLib_RaiseError(
				"Cannot serialize dialog, meta not initialized (node meta not found)",
				ErrorSeverity::ERR
			);
			return eMetaOp_Fail;
		}
		if (r == eMetaOp_Succeed) {
			if (meta->IsRead())
				dlog->_DeleteData();
			u32 folders = dlog->mFolders.GetSize();
			meta->serialize_uint32(&folders);
			for (int i = 0; i < folders; i++) {
				if (meta->IsWrite()) {
					PerformMetaSerializeFull(meta, dlog->mFolders[i],
						dlog->mFolders[i]->GetMetaClassDescription());
				}
				else {
					DlgFolder* f = new DlgFolder;
					PerformMetaSerializeAsync<DlgFolder>(meta, f);
					dlog->mFolders.AddElement(0, NULL, &f);
				}
			}
			u32 nodes = dlog->mNodes.GetSize();
			meta->serialize_uint32(&nodes);
			u64 hash;
			MetaClassDescription* clazz;
			if (meta->IsRead()) {
				for (int i = 0; i < nodes; i++) {
					meta->serialize_uint64(&hash);
					clazz = TelltaleToolLib_FindMetaClassDescription_ByHash(hash);
					if (!clazz) {
						TelltaleToolLib_RaiseError(
							"Dialog contains unknown subclass of dialog node",
							ErrorSeverity::ERR);
						return eMetaOp_Fail;
					}
					void* inst = clazz->New();
					if (!inst)
						return eMetaOp_OutOfMemory;
					DlgNode* base = (DlgNode*)clazz->CastToBase(inst, nodeDesc);
					if (!base) {
						TelltaleToolLib_RaiseError(
							"Dialog node in dialog does not subclass from DlgNode",
							ErrorSeverity::ERR);
						return eMetaOp_Fail;
					}
					r=PerformMetaSerializeFull(meta, inst, clazz);
					if (r != eMetaOp_Succeed)
						return r;
					dlog->mNodes.AddElement(0, NULL, &base);
				}
			}
			else {
				for (int i = 0; i < nodes; i++) {
					DlgNode* node = dlog->mNodes[i];
					PerformMetaSerializeFull(meta,
						node, node->GetMetaClassDescription());
				}
			}
		}
		return r;
	}

};


struct DlgNodeJump : DlgNode {

	enum JumpBehaviour {
		eJumpAndExecute = 1,
		eJumpExecuteAndReturn = 2,
		eReturn = 3
	};

	enum JumpTargetClass {
		eToName = 1,
		eToParent = 2,
		eToNodeAfterParentWaitNode = 3
	};

	enum VisibilityBehaviour {
		eIgnoreVisibility = 1,
		eObeyVisibility = 2,
	};

	struct EnumJumpTargetClass :EnumBase {
		JumpTargetClass mVal;
	};

	struct EnumJumpBehaviour : EnumBase {
		JumpBehaviour mVal;
	};

	struct EnumVisibilityBehaviour : EnumBase {
		VisibilityBehaviour mVal;
	};

	DlgNodeLink mJumpToLink;
	Symbol mJumpToName;
	EnumJumpTargetClass mJumpTargetClass;
	EnumJumpBehaviour mJumpBehaviour;
	EnumVisibilityBehaviour mVisibilityBehaviour;
	int mChoiceTransparency;
	Handle<Dlg> mhJumpToDlg;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeJump>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeJump;
	}

};

#endif