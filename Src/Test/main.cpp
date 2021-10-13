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

	DataStream* src = OpenDataStreamFromDisc("c:/users/lucas/desktop/test.ttarch2", DataStreamMode::eMode_Read);

	TTArchive2 archive;
	archive.Activate(src);
	if (archive.mResources.size()) {

		String str;
		archive.GetResourceName(archive.mResources.data()->mNameCRC, &str);
		printf("Name %s size %d off %llx\n", str.c_str(), archive.mResources.data()->mSize, archive.mResources.data()->mOffset);
		u32 header;
		DataStream* stream = archive.GetResourceStream(archive.mResources.data());
		stream->Serialize((char*)&header, 4);
		printf("Header DWORD %x\n", header);


		printf("Resource count: %llu, version %d\n", archive.mResources.size(),
			archive.mVersion);
		delete stream;
	}

}

void run_ttarch2w() {
	DataStream* dst = OpenDataStreamFromDisc("c:/users/lucas/desktop/test.ttarch2", DataStreamMode::eMode_Write);
	void* buf = calloc(1, 67);
	memcpy(buf, "LUCAS IS AWESOMEEEEEEEE!! THIS IS A TELLTALE DATA STREAM CONTAINER", 67);
	DataStream* src = new DataStreamMemory(buf, 67, DataStreamMode::eMode_Read);
	char* p = new char[sizeof(TTArchive2::ResourceCreateEntry) * 2];
	new (p) TTArchive2::ResourceCreateEntry("AwesomeFile.txt", src);
	new (p + sizeof(TTArchive2::ResourceCreateEntry)) TTArchive2::ResourceCreateEntry("AwesomeFile22.txt", src);
	TTArchive2::Create(dst, (TTArchive2::ResourceCreateEntry*)p, 2, false, false, Compression::Library::OODLE,0);

	delete[]p;
	delete dst;
	delete src;
}

int _RunTestMain(int argn, char** argv) {
	TelltaleToolLib_Initialize("MC2");

	run_ttarch2w();

	printf("Done!");
	_CrtDumpMemoryLeaks();
	return 0;
}

#endif