// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#pragma once

#include "Meta.hpp"

class ContainerInterface {
public:

	struct Iterator {
		void* mpInternalIter;
		void(*mfpDestroy)(void*);

		~Iterator() {
			if (mfpDestroy)
				mfpDestroy(mpInternalIter);
		}

	};

	ContainerInterface() {}

	

};