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
	DataStream* src = OpenDataStreamFromDisc("d:/games/minecraft - story mode/archives/"
		"MCSM_pc_Minecraft101_voice.ttarch2", DataStreamMode::eMode_Read);

	u64 size;
	char header[5];
	memset(header, 0, 5);
	DataStreamContainerParams params;
	params.mpSrcStream = src;

	DataStreamContainer container(params);
	container.Read(0, &size);

	container.SetPosition(0, DataStreamSeekType::eSeekType_Begin);
	container.Serialize(header, 4);

	printf("Total size: %llu, Header: %s\n", size,header);

	delete src;
}

int main(int argn, char** argv) {
	TelltaleToolLib_Initialize("MCSM");

	run_ttarch2();

	printf("Done!");
	//_CrtDumpMemoryLeaks();
	return 0;
}

#endif