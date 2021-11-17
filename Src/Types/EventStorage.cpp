// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "EventStorage.h"

EventStoragePage::~EventStoragePage() {
	for (int i = 0; i < this->mEvents.GetSize(); i++)
		delete mEvents[i];
}

MetaOpResult EventStoragePage::MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDescription, 
	MetaMemberDescription* pContextDescription, void* pUserData) {
	MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
		pContextDescription, pUserData);
	MetaStream* meta = static_cast<MetaStream*>(pUserData);
	EventStoragePage* epage = static_cast<EventStoragePage*>(pObj);
	bool iswrite = meta->mMode == MetaStreamMode::eMetaStream_Write;
	if (r == eMetaOp_Succeed) {
		u32 numevents = epage->mEvents.size();
		meta->serialize_uint32(&numevents);
		meta->BeginObject("Events", NULL);
		meta->SetObjectAsArrayType();
		if (iswrite) {
			std::list<EventLoggerEvent*>::iterator it;
			for (it = epage->mEvents.begin(); it != epage->mEvents.end(); it++) {
				meta->BeginAnonObject(NULL);
				(*it)->Serialize(epage, meta);
				meta->EndAnonObject(NULL);
			}
		}
		else {
			for (int i = 0; i < numevents; i++) {
				EventLoggerEvent* event = new EventLoggerEvent();
				event->Serialize(epage, meta);
				epage->mEvents.push_back(event);
			}
		}
		meta->EndObject("Events");
	}
	return r;
}