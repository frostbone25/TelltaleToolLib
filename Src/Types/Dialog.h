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

struct DlgFolder {

	MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgFolder>();
	}

};

struct DlgNode {
	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgNode>();
	}
};

struct DlgObjIDOwner {
	DlgObjID mDlgID;
};

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

#endif