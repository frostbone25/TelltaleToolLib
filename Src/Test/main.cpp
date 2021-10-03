#ifndef _WINDLL

#include "../Meta.hpp"
#include "../Types/DCArray.h"

void dmp(const char* p, char* b, int s) {
	FILE* t = fopen(p, "wb");
	fwrite(b, s, 1, t);
	fclose(t);
}

int main(int argn, char** argv) {

	TelltaleToolLib_Initialize("MCSM");

	DataStream* stream = OpenDataStreamFromDisc("c:/users/lucas/desktop/testing.prop", READ);

	{
		MetaStream meta("flag.bin");
		MetaStreamParams params{ 0 };
		meta.Open(stream, MetaStreamMode::eMetaStream_Read, params);

		for (int i = 0; i < meta.mVersionInfo.size(); i++) {
			MetaClassDescription* desc = TelltaleToolLib_FindMetaClassDescription_ByHash(meta.mVersionInfo[i].mTypeSymbolCrc);
			if (desc)
				printf("Found type in file: %s\n", desc->mpTypeInfoName);
		}

		DCArray<i32> array;
		MetaOpResult r = PerformMetaSerializeFull(&meta, &array, GetMetaClassDescription(typeid(DCArray<i32>).name()));
		printf("%d\n", r);
		for (int i = 0; i < array.GetSize(); i++) {
			printf("Found %d\n", array[i]);
		}

	}

	printf("Done!");
	return 0;
}

#endif