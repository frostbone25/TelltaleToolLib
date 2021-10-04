#ifndef _WINDLL

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "../Meta.hpp"
#include "../Types/DCArray.h"
#include "../Types/Set.h"

void dmp(const char* p, char* b, int s) {
	FILE* t = fopen(p, "wb");
	fwrite(b, s, 1, t);
	fclose(t);
}

void run() {
	DataStream* stream = OpenDataStreamFromDisc("Src/Test/testing.prop", READ);

	{
		MetaStream meta("aprop.prop");
		MetaStreamParams params{ 0 };
		meta.Open(stream, MetaStreamMode::eMetaStream_Read, params);

		for (int i = 0; i < meta.mVersionInfo.size(); i++) {
			MetaClassDescription* desc = TelltaleToolLib_FindMetaClassDescription_ByHash(meta.mVersionInfo[i].mTypeSymbolCrc);
			if (desc)
				printf("Found type in file: %s\n", desc->mpTypeInfoName);
		}

		Set<i32> array;
		MetaOpResult r = PerformMetaSerializeFull(&meta, &array, 
			GetMetaClassDescription(typeid(Set<i32>).name()));
		printf("%d\n", r);
		for (int i = 0; i < array.GetSize(); i++) {
			printf("Found %d\n", array[i]);
		}

	}
}

int main(int argn, char** argv) {
	TelltaleToolLib_Initialize("MCSM");
	run();

	printf("Done!");
	//_CrtDumpMemoryLeaks();
	return 0;
}

#endif