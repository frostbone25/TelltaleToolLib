// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _CHORE
#define _CHORE

#include "../Meta.hpp"
#include "DCArray.h"
#include "LocalizationRegistry.h"
#include "PropertySet.h"

//.CHORE FILES
struct Chore {

	String mName;
	Flags mFlags;
	float mLength;
	long mNumResources;
	long mNumAgents;
	PropertySet mEditorProps;
	String mChoreSceneFile;
	long mRenderDelay;
	LocalizeInfo mSynchronizedToLocalization;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		
	}

};

#endif