#ifndef _WINDLL

#include "../Meta.hpp"

void dmp(const char* p, char* b, int s) {
	FILE* t = fopen(p, "wb");
	fwrite(b, s, 1, t);
	fclose(t);
}

int main(int argn, char** argv) {

	TelltaleToolLib_Initialize("boneville");

	DataStream* stream = OpenDataStreamFromDisc("c:/users/lucas/desktop/msv5.prop", READ);
	
	{
		MetaStream meta("prefs.prop");
		MetaStreamParams params{ 0 };
		meta.Open(stream, MetaStreamMode::eMetaStream_Read, params);
		
		printf("Stream Version: %d, Is Compiled: %s\n", meta.mStreamVersion, meta.mRuntimeFlags.mFlags &
			(int)MetaStream::RuntimeFlags::eStreamIsCompiledVersion ? "true" : "false");

		for (int i = 0; i < meta.mVersionInfo.size(); i++) {
			MetaVersionInfo inf = meta.mVersionInfo[i];
			MetaClassDescription* tdesc = TelltaleToolLib_FindMetaClassDescription_ByHash(inf.mTypeSymbolCrc);
			if (tdesc)
				printf("Found meta class description: %s\n", tdesc->mpTypeInfoName);
		}

	}

	printf("Done!");
	return 0;
}

#endif