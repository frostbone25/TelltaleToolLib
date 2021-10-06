#ifndef _WINDLL

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "../Meta.hpp"
#include "../Types/PropertySet.h"

void dmp(const char* p, char* b, int s) {
	FILE* t = fopen(p, "wb");
	fwrite(b, s, 1, t);
	fclose(t);
}

void run() {
	DataStream* stream = OpenDataStreamFromDisc("Src/Test/in.prop", READ);

	{
		MetaStream meta("aprop.prop");
		MetaStreamParams params{ 0 };
		meta.Open(stream, MetaStreamMode::eMetaStream_Read, params);

		for (int i = 0; i < meta.mVersionInfo.size(); i++) {
			MetaClassDescription* desc = TelltaleToolLib_FindMetaClassDescription_ByHash(meta.mVersionInfo[i].mTypeSymbolCrc);
			if (desc)
				printf("Found type in file: %s\n", desc->mpTypeInfoName);
		}

		PropertySet prop;
		//TODO test

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