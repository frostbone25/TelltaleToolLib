#ifndef _WINDLL

#include "../Meta.hpp"

void dmp(const char* p, char* b, int s) {
	FILE* t = fopen(p, "wb");
	fwrite(b, s, 1, t);
	fclose(t);
}

int main(int argn, char** argv) {

	TelltaleToolLib_Initialize("MCSM");

	DataStream* stream = OpenDataStreamFromDisc("c:/users/lucas/desktop/in.prop", READ);

	{
		MetaStream meta("flag.bin");
		MetaStreamParams params{ 0 };
		meta.Open(stream, MetaStreamMode::eMetaStream_Read, params);

		for (int i = 0; i < meta.mVersionInfo.size(); i++) {
			MetaClassDescription* desc = TelltaleToolLib_FindMetaClassDescription_ByHash(meta.mVersionInfo[i].mTypeSymbolCrc);
			if (desc)
				printf("Found type in file: %s\n", desc->mpTypeInfoName);
		}

	}

	printf("Done!");
	return 0;
}

#endif