#ifndef _TTARCHIVE
#define _TTARCHIVE

#include "TelltaleToolLibrary.h"
#include <vector>
#include "Meta.hpp"

struct ResourceInfo {
	u64 mOffset;
	u64 mSize;
	//mSourceType,mCreationTime,mLastModifiedTime
};

//.TTARCH2 files. Takes ownership (deletes when done) of passed in stream when reading
class TTArchive2 {
public:

	struct ResourceEntry {
		u32 mNameCRC;
		u64 mOffset;
		u32 mSize;
		u16 mNamePageIndex, mNamePageOffset;
	};

	DataStream* mpNameStream;
	DataStream* mpResourceStream;
	std::vector<TTArchive2::ResourceEntry> mResources;
	u32 mVersion;
	char* mpNamePageCache;
	i32 mNamePageCacheIndex;
	u32 mNamePageCount;
	bool mbActive;//by lib, for lib
	DataStream* mpInStream;//for lib

	void Activate(DataStream* inArchiveStream);//nullable (assumes creation)
	bool HasResource(const Symbol&);
	String* GetResourceName(const Symbol&, String* result);
	bool GetResourceInfo(const Symbol&, ResourceInfo*);
	DataStreamSubStream&& GetResourceStream(ResourceEntry*);
	void Deactivate();

	TTArchive2() : mbActive(false), mpInStream(NULL), mNamePageCacheIndex(-1)
	, mpNamePageCache(NULL), mpNameStream(NULL), mpResourceStream(NULL) {}

	~TTArchive2() {
		if (mbActive)
			Deactivate();
	}

private:

	ResourceEntry* _FindResource(const Symbol&);

};

#endif