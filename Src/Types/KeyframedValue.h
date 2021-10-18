// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _KFV
#define _KFV

#include "../Meta.hpp"
#include "DCArray.h"
#include "AnimatedValueInterface.h"

struct KeyframedValueInterface{};

enum eTangentModes : int {
	eTangentUnknown = 0,
	eTangentStepped = 1,
	eTangentKnot = 2,
	eTangentSmooth = 3,
	eTangentFlat = 4

};

struct EnumeTangentMode {
	eTangentModes mVal;
};

template<typename T>
struct KeyframedValue : public KeyframedValueInterface , public AnimatedValueInterface<T> {//bases: animatedvalueinterface

	struct Sample {
		float mTime, mRecipTimeToNextSample;
		bool mbInterpolateToNextKey;
		EnumeTangentMode mTangentMode;
		T mValue;
	};

	T mMinVal, mMaxVal;
	DCArray<KeyframedValue<T>::Sample> mSamples;

};

template class KeyframedValue<float>;
template class KeyframedValue<bool>;
template class KeyframedValue<int>;
template class KeyframedValue<u64>;

#endif