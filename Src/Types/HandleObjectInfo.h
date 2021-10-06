// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _HOI
#define _HOI

#include "../Meta.hpp"

struct HandleObjectInfo {
	Symbol mObjectName;
	Flags mFlags;
};

struct HandleBase {
	//HandleObjectInfo* mpHandleObjectInfo; //normally a pointer in the engine
	//maybe to do with a buffer of them? but anyway
	HandleObjectInfo mHandleObjectInfo;
};

template<typename T> struct Handle : public HandleBase {

};

template<typename T> struct HandleLock : public Handle<T> {

};

#endif
