//Taken from my old Telltale lib, LibTelltale!

#ifndef _HASHDB
#define _HASHDB

#include "../TelltaleToolLibrary.h"

#define HASHDB_EXT HashDB

const extern u32 MAGIC;

/*
* A database of hashes which the library uses to search for its CRCs. Similar to RTBs, except more complex with pages of entries to make quicker searching
* Use this to find a CRC to string mapping
*/
class HashDatabase {
public:
	struct Page {
		char* pageName;
		u16 flags;
		u32 count;
		u64 offset;
		u64 size;
	};

	~HashDatabase();
	//TAKES OWNERSHIP OF STREAM!
	HashDatabase(DataStream* db);
	bool Open();
	u32 NumPages();
	u32 NumEntries();
	u32 Flags();
	Page* PageAt(int index);
	Page** Pages();
	Page* FindPage(const char* name);
	//page can be NULL, if so it will search all pages. CALL DELETE ON RETURN VALUE
	void FindEntry(Page* page, u64 crc,String* result);

protected:
	DataStream* db_stream;
	Page** db_pages;
	int numPages;
	int flags;
};

#endif