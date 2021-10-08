#ifndef _WINDLL

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include "../TTArchive2.hpp"
#include "../Meta.hpp"
#include "../Types/PropertySet.h"

void dmp(const char* p, char* b, int s) {
	FILE* t = fopen(p, "wb");
	fwrite(b, s, 1, t);
	fclose(t);
}

void run_write() {
	DataStream* stream = OpenDataStreamFromDisc("Src/Test/out.prop", WRITE);

	{
		MetaStream meta("aprop.prop");
		meta.Open(stream, MetaStreamMode::eMetaStream_Write, { 0 });

		PropertySet prop;

		Handle<PropertySet> testValue = CreateHandle<PropertySet>("ReferencedProp.prop");
		prop.AddProperty("Test Prop - Reference", &testValue);

		MetaOpResult result = PerformMetaSerializeFull(&meta, &prop);
		printf("Result: %d\n", result);

	}
}

void print_version_info(MetaStream* meta) {
	for (int i = 0; i < meta->mVersionInfo.size(); i++) {
		MetaClassDescription* desc = TelltaleToolLib_FindMetaClassDescription_ByHash(meta->mVersionInfo[i].mTypeSymbolCrc);
		if (desc)
			printf("Found type in file: %s\n", desc->mpTypeInfoName);
	}
}

void run() {
	DataStream* stream = OpenDataStreamFromDisc("Src/Test/in.prop", READ);

	{
		MetaStream meta("aprop.prop");
		meta.Open(stream, MetaStreamMode::eMetaStream_Read, { 0 });

		print_version_info(&meta);

		PropertySet prop;
		MetaOpResult result = PerformMetaSerializeFull(&meta, &prop);
		printf("Result: %d\n", result);

	}
}

void run_ttarch2() {

	const char* home_pc = "d:/games/minecraft - story mode/archives/"
		"MCSM_pc_Minecraft101_voice.ttarch2";
	const char* laptop = "c:/users/lucas/desktop/TTArch/"
		"MC2_pc_Menu_anichore.ttarch2";

	DataStream* src = OpenDataStreamFromDisc(laptop, DataStreamMode::eMode_Read);

	TTArchive2 archive;
	archive.Activate(src);
	DataStreamSubStream&& stream = archive.GetResourceStream(
		archive.mResources.data());
	char header[5];
	header[4] = 0;
	stream.Serialize(header, 4);

	printf("Header %s\n", header);

	printf("Resource count: %llu, version %d\n", archive.mResources.size(),
		archive.mVersion);

}

int main(int argn, char** argv) {
	TelltaleToolLib_Initialize("MC2");

	run_ttarch2();

	printf("Done!");
	_CrtDumpMemoryLeaks();
	return 0;
}

#endif