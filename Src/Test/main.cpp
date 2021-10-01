#ifndef _WINDLL

#include "../Meta.hpp"


int main(int argn, char** argv) {

	TelltaleToolLib_Initialize("mc2");

	DataStream* stream = OpenDataStreamFromDisc("c:/users/lucas/desktop/obj.prop", READ);
	
	{
		MetaStream meta("obj.prop");
		MetaStreamParams params{ 0 };
		meta.Open(stream, MetaStreamMode::eMetaStream_Read, params);
		
	}

	printf("Done!");
	return 0;
}

#endif