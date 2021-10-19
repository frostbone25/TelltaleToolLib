// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _STYLE_GUIDE
#define _STYLE_GUIDE

#include "../Meta.hpp"
#include "TRange.h"
#include "DCArray.h"
#include "HandleObjectInfo.h"
#include "TransitionMap.h"
#include "List.h"
#include "AnimOrChore.h"


struct ActingOverridablePropOwner {
	Flags mSerializationFlags;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingOverridablePropOwner* data = static_cast<ActingOverridablePropOwner*>(pObj);
	}

};

struct ActingResource : public ActingOverridablePropOwner{
	AnimOrChore mResource;
	Flags mRuntimeFlags;
	TRange<float> mValidIntensityRange;
};

struct ActingPalette : public ActingOverridablePropOwner, public UID::Owner {

	enum ActiveDuring {
		always = 1,
		talking = 2,
		listening = 3
	};

	struct EnumActiveDuring {
		ActiveDuring mVal;
	};
	
	enum EndRelativeTo {
		beginning = 1,
		end = 2,
		transition = 3
	};

	struct EnumEndRelativeTo {
		EndRelativeTo mVal;
	};

	enum Overrun {
		dissallowed = 0,
		allowed = 1
	};

	struct EnumOverrun {
		Overrun mVal;
	};

	String mName;
	EnumActiveDuring mActiveDuring;
	TRange<float> mTimeBetweenActions;
	TRange<float> mFirstActionDelayRange;
	TRange<float> mSpilloutBufPreRange;
	TRange<float> mSpilloutBufPostRange;
	TRange<float> mLatestStartOffsetRange;
	TRange<float> mValidIntensityRange;
	DCArray<ActingResource*> mResourcePtrs;
	long mGroupMembershipUID;
	Flags mFlags;
	EnumEndRelativeTo mFirstActionEndRel, mEndOffsetRel, mLatestStartOffsetRel;
	EnumOverrun mOverrunAllowed, mMoodOverrunAllowed;
	bool mDisableAct;
	long mJunket;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingPalette* data = static_cast<ActingPalette*>(pObj);
	}

};

struct ActingAccentPalette : public ActingOverridablePropOwner, public UID::Owner {

	enum Overrun {
		dissallowed = 0,
		allowed = 1
	};

	struct EnumOverrun {
		Overrun mVal;
	};

	String mName;
	TRange<float> mStartOffsetRange;
	EnumOverrun mMoodOverrunAllowed;
	bool mDisableAct;
	TRange<float> mValidIntensityRange;
	TRange<float> mSpilloutBufPostRange;
	float mRandomChance;
	long mTrackID;//ENUM: 1= Body, 2= Face, 3 = Head 1, 4 = Head 2
	DCArray<ActingResource*> mResourcePtrs;
	long mGroupMembershipUID;
	Flags mFlags;
	long mVersion;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingAccentPalette* data = static_cast<ActingAccentPalette*>(pObj);
	}

};

struct ActingPaletteGroup : public UID::Owner {

	struct ActingPaletteTransition {
		String mTransition;
		AnimOrChore mTransitionIn;
		float mCenterOffset;
		float mPreDelay;
		float mPostDelay;
		float mFadeTime;
	};

	enum IdleTransition {
		transitionLinear = 1,
		transitionEaseInOut = 2,
		transitionUnused = 3
	};

	struct EnumIdleTransition {
		IdleTransition mVal;
	};

	String mName;
	AnimOrChore mIdle;
	AnimOrChore mTalkingIdle;
	AnimOrChore mMumbleMouth;
	float mWeight;
	AnimOrChore mTransitionIn;
	AnimOrChore mTransitionOut;
	List<ActingPaletteTransition> mTransitions;
	float mIdleTransitionTimeOverride;
	Handle<TransitionMap> mhIdleTransitionMap;
	EnumIdleTransition mIdleTransitionKind;
	float mRandomAutoMin;
	float mRandomAutoMax;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingPaletteGroup* data = static_cast<ActingPaletteGroup*>(pObj);
	}

};

struct ActingPaletteClass : public UID::Generator , public UID::Owner, public ActingOverridablePropOwner {
	String mName;
	DCArray<ActingPalette*> mPalettePtrs;
	DCArray<ActingAccentPalette*> mAccentPalettePtrs;
	DCArray<ActingPaletteGroup*> mPaletteGroupPtrs;
	DCArray<String> mAlternateNames;
	long mDefaultPaletteGroupID;
	Flags mFlags;
	bool mInstantChange;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingPaletteClass* data = static_cast<ActingPaletteClass*>(pObj);
	}

};

//.STYLE FILES
struct StyleGuide : public UID::Generator, public ActingOverridablePropOwner {
	long mDefPaletteClassID;
	bool mbGeneratesLookAts;
	DCArray<ActingPaletteClass*> mPaletteClassPtrs;
	Flags mFlags;
	DCArray<ActingPaletteClass> mPaletteClasses;
	long mDefPaletteClassIndex;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		StyleGuide* data = static_cast<StyleGuide*>(pObj);
	}

};

#endif