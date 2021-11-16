// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _EVENTSTORE
#define _EVENTSTORE

#include "../Meta.hpp"
#include "DCArray.h"
#include "HandleObjectInfo.h"
#include "List.h"

struct EventLoggerEvent;

//.EPAGE FILES
struct EventStoragePage {
	long mVersion;
	u64 mSessionID;
	String mFlushedNameOnDisk;
	List<EventLoggerEvent*> mEvents;
	EventStoragePage(int pagesize) {

	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
			pContextDescription, pUserData);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		EventStoragePage* epage = static_cast<EventStoragePage*>(pObj);
		bool iswrite = meta->mMode == MetaStreamMode::eMetaStream_Write;
		if (r == eMetaOp_Succeed) {
			u32 numevents = epage->mEvents.size();
			meta->serialize_uint32(&numevents);
			meta->BeginObject("Events",NULL);
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

};

struct EventLoggerEvent {

	long mEventID;
	long mMaxSeverity;

	void Serialize(EventStoragePage* storage, MetaStream* stream) {
		Meta::MetaOperation_SerializeAsync(this,
			GetMetaClassDescription<EventLoggerEvent>(), NULL, stream);
		stream->BeginBlock();
		//eventloggerevent serialize
		stream->EndBlock();
	}

};

//.ESTORE FILES
struct EventStorage {

	struct PageEntry {
		Handle<EventStoragePage> mhPage;
		u32 mMaxEventID;
	};

	long mVersion;
	u64 mSessionID;
	DCArray<PageEntry> mPages;
	String mName;
	u32 mLastEventID;
	u32 mEventStoragePageSize;
	EventStoragePage* mpCurrentPage;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
			pContextDescription, pUserData);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		EventStorage* estore = static_cast<EventStorage*>(pObj);
		if (r == eMetaOp_Succeed) {
			bool hasPage = estore->mpCurrentPage != NULL;
			meta->serialize_bool(&hasPage);
			if (hasPage) {
				if (estore->mpCurrentPage == NULL)
					estore->mpCurrentPage = new EventStoragePage(0x2000);
				r=PerformMetaSerializeAsync<EventStoragePage>
					(meta, estore->mpCurrentPage);
			}
		}
		return r;
	}

};

#endif