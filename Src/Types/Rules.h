// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _RULES
#define _RULES

#include "../Meta.hpp"
#include "HandleObjectInfo.h"
#include "PropertySet.h"
#include "Map.h"
#include "DCArray.h"

struct LogicGroup {

	struct LogicItem : PropertySet {
		String mName;
		Map<Symbol, bool, std::less<Symbol>> mKeyNegateList;
		Map<Symbol, int, std::less<Symbol>> mKeyComparisonList, mKeyActionList;
		DCArray<String> mReferenceKeyList;
	};

	long mOperator;
	Map<String, LogicItem, std::less<String>> mItems;
	DCArray<LogicGroup> mLogicGroups;
	long mGroupOperator;
	long mType;
	String mName;

};

struct Rule {

	String mName;
	String mRuntimePropName;
	Flags mFlags;
	LogicGroup mConditions, mActions, mElse;
	String mAgentCategory;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {

		}
		return r;
	}

};

//.RULES FILES
struct Rules {
	Flags mFlags;
	Handle<PropertySet> mhLogicProps;
	Map<String, Rule*, std::less<String>> mRuleMap;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {

		}
		return r;
	}

};

#endif