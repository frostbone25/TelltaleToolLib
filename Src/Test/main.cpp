#ifndef _WINDLL

#include "../TelltaleToolLibrary.h"


int main(int argn, char** argv) {
	DataStream* stream = new DataStreamFileDisc(
		PlatformSpecOpenFile("c:/users/lucas/desktop/test.bin", 
		DataStreamMode::eMode_Write),
		DataStreamMode::eMode_Write);
	DataStreamMemory* memstream = new DataStreamMemory(100, 10);
	printf("Created stream\n");
	//TODO fix memory::Serialize, doesnt serialize chunks properyls
	for(int i = 0; i < 10; i++)memstream->SerializeStringWrite("hiLucasLucasNONO");
	memstream->Transfer(stream, 0, memstream->GetSize());

	getchar();
	if (stream)delete stream;
	if (memstream)delete memstream;
	printf("Deleted\n");
	return 0;
}

#endif