// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _LOCREG
#define _LOCREG

#include "../Meta.hpp"
#include "Map.h"
#include "ToolProps.h"

//.LOCREG FILES
struct LocalizationRegistry {

	Map<Symbol, int, Symbol::CompareCRC> mFlagIndexMap;
	Map<int, Symbol, std::less<int>> mFlagIndexMapReverse;
	ToolProps mToolProps;

};

#endif