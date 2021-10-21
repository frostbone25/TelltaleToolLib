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
#include "PropertySet.h"
#include "TransitionMap.h"
#include "List.h"
#include "AnimOrChore.h"

constexpr Symbol styleIdlesOnKey{ 0x225F4A10E80B37AD };
constexpr Symbol styleBaseIdleKey{ 0x1DB0F9232E38E12F };
constexpr Symbol styleIdleKey1{ 0x16637976BEE9313D };
constexpr Symbol styleGuideKey1{ 0x3A19C2CC84E6A163 };
constexpr Symbol styleIdleKey2{ 0x0E630E104C6B0DA01 };
constexpr Symbol styleGuideKey2{ 0x8BA4000EFA24BABE };
constexpr Symbol styleIdleKey3{ 0x0A4C000EF6F5AEC92 };
constexpr Symbol styleGuideKey3{ 0x0C954E1E553CE8C2D };
constexpr Symbol styleIdleKey4{ 0x2AE244969B265BF8 };
constexpr Symbol styleGuideKey4{ 0x4776A59CA7B23B47 };
constexpr Symbol styleIdleKey5{ 0x6812A57D32CC6D6B };
constexpr Symbol styleGuideKey5{ 0x58644770E580DD4 };
constexpr Symbol styleUser1Key{ 0x0BCE3F22B573AE08F };
constexpr Symbol styleUser2Key{ 0x7BF2D017AD04BB3A };
constexpr Symbol styleUser3Key{ 0x390231FC04EE8DA9 };
constexpr Symbol styleUser4Key{ 0x0B7207585F0923AC3 };
constexpr Symbol styleUser5Key{ 0x0F5D0946E59780C50 };

struct ActingOverridablePropOwner {
	Flags mSerializationFlags;
	PropertySet* mpOverridableValues;
	Handle<PropertySet> mhParent;

	ActingOverridablePropOwner() : mpOverridableValues(NULL) {}

	ActingOverridablePropOwner(Handle<PropertySet>& pHandle){
		mhParent = pHandle;
		mpOverridableValues = NULL;
	}

	~ActingOverridablePropOwner() {
		if (mpOverridableValues)
			delete mpOverridableValues;
	}

	void CreateOverridableValuesPropertySet(bool pAddParents) {
		if (!this->mpOverridableValues) {
			mpOverridableValues = new PropertySet();
		}
	}

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