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

struct JiraRecord {

};

struct JiraRecordManager {
	Map<String, JiraRecord*, std::less<String>> mRecords;

	~JiraRecordManager() {
		for (int i = 0; i < mRecords.GetSize(); i++) {
			delete mRecords[i].second;
		}
		mRecords.ClearElements();
	}

};

struct DlgObjID {
	Symbol mID;
};

struct DlgObjIDOwner {
	DlgObjID mDlgID;
};

//.DLOG FILES
struct Dlg : DlgObjIDOwner, UID::Generator {//UID im not 100% sure since its only in older games, looks like a UID 

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

		return eMetaOp_Succeed;
	}

};

#endif