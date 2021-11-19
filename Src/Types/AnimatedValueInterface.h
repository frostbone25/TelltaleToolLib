// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ANIMATED_VALUE_INTERFACE
#define _ANIMATED_VALUE_INTERFACE

#include "../Meta.hpp"
#include "../TelltaleToolLibrary.h"

struct AnimationValueInterfaceBase {

	enum Flags {
		eDisabled = 0x1,
		eTimeBehavior = 0x2,
		eWeightBehavior = 0x4,
		eMoverAnim = 0x10,
		ePropertyAnimation = 0x20,
		eTextureMatrixAnimation = 0x40,
		eAudioDataAnimation = 0x80,
		eDontOptimize = 0x100,
		eHomogeneous = 0x200,
		eMixerScaled = 0x400,
		eMixerHomogeneous = 0x800,
		eStyleAnimation = 0x1000,
		ePropForceUpdate = 0x2000,
		eMixerOwned = 0x4000,
		eMixerDirty = 0x8000, 
		eAdditive = 0x10000,
		eExternallyOwned = 0x20000,
		eDontMixPausedControllers = 0x40000,
		eRuntimeAnimation = 0x80000,
		eTransientAnimation = 0x100000,
		eToolOnly = 0x200000,
		eKeyedAttachmentAnimation = 0x400000,
		eMixerWeightedBlend = 0x800000,
		eValueKind = 0xFF000000,
	};

	Symbol mName;
	int mFlags;



};

template<typename T> struct AnimatedValueInterface : public AnimationValueInterfaceBase {};

#endif