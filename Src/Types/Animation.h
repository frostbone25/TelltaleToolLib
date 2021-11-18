// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ANM
#define _ANM

#include "../Meta.hpp"

//TODO , note cant use chore included, prefine and code in src file
struct Animation {

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(Animation, a);
		meta->SkipToEndOfCurrentBlock();//for chore
		return eMetaOp_Succeed;
	}

};

#endif