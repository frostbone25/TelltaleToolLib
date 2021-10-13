// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../TelltaleToolLibrary.h"

// Copyright Lucas Saragosa - Part of the Telltale Editor project. Some Job semantics from Telltale. Implementation is by me.

// This header should not be included in any files.

#include "TEditor.h"
#include "../Types/DCArray.h"
#include <iostream>

#define _MAX(x,y) max(x,y)
#define _MIN(x,y) min(x,y)

void _PrintUsage() {
	//"\tTEditor_CLI.exe\n\n"
	printf("Usage:\n"
		"<> are required arguments, [] are non required, ':' seperates input strings. \n\n\tUsage: TEditor_CLI.exe <game_id> "
		"[options] \n\t\t<extractarc:createarc:dump:import:export> [args]\n\nSee the GameIDs.txt for game IDs.\n\nOperation Usage Examples:\n\n"
		"\tTEditor_CLI.exe MC2 extractarc \"c:/my_archive.ttarch2\" \"c:/output_folder\"\n\n"
		"\tTEditor_CLI.exe MCSM -c -l oodle -e -f *.d3dmesh,*.d3dtx,adv_.* importarc \"c:/input_folder\" \"c:/my_desc_archive.ttarch2\"\n\n"
		"\tTEditor_CLI.exe WDC dump \"c:/path/to/my_input_meta_file\" \"c:/path/to/output_file.txt\"\n\n"
		"\tTEditor_CLI.exe WD3 import \"c:/path/to/input_script_or_file\" \"c:/path/to/output_script_or_meta_file\"\n\n"
		"\tTEditor_CLI.exe WD3 export \"c:/path/to/input_script_or_meta_stream_file\" \"c:/path/to/output_script_or_file\"\n\n"
		"\nOptions:\n\t-d \"c:/dump_dest_file.txt\" : Dumps all output written from console the specified text file\n\t"
		"-l <oodle:zlib> : Specify the compression library to compress the archive with (used in importarc)\n\t"
		"-e : Encrypt the archive (used in import arc)\n\t"
		"-c : Compress the archive (used in import arc)\n\t"
		"-v : Specify the output archive version (used in import arc. Version 1 is for TTA3 (wolf among us), version 2 for TTA4 (rest))\n\t"
		"\tNote that version 2 is the default version!\n\t"
		"-f <exp> : Filter which files types/names to import or export from or to an archive. Examples: '*.lua','obj_objectName.*'\n\t"
		"-s <header:default:debug:async> : Dump only the specified section of the file. (used im dump)\n\t"
		"-h : Use with any operation (eg extractarc/import/dump etc) to only print out how to use this operation or print what it does.\n\t");
}

int TEditor_Main(int argn, char *argv[]) {
	DCArrayNM<char*> args;
	args.mpStorage = argv + 1;
	args.mSize = argn - 1;
	if (!args.GetSize()) {
		_PrintUsage();
		args.mpStorage = NULL;
		return 1;
	}
	DCArray<std::string> ends_in, starts_with;
	bool compress = false, encrypt = false, help = false;
	Compression::Library lib = Compression::Library::ZLIB;
	char in[265], out[265];
	u32 version = 2;
	int index = 0;
	char** filters = NULL;
	MetaStream::SectionType dumponly = MetaStream::SectionType::eSection_Count;
	int i = 0;
	bool b = true;
	char* type,*matchstr,*sect;
	printf("\n\tTellale Games File Editor v1.0.1 By Lucas Saragosa\n\n");
	for (i; i < args.mSize; i++) {
		char* arg = args[i];
		if (*arg == '-') {//OPT
			switch (arg[1]) {
			case 'l':
				if (i + 1 == args.mSize) {
					printf("Error at argument -l, you need to specify the compression type after it!\n");
					args.mpStorage = NULL;
					return 1;
				}
				++i;
				type = args[i];
				if (!_stricmp(type, "oodle")) {
					lib = Compression::Library::OODLE;
				}
				else if (!_stricmp(type, "zlib")) {
					lib = Compression::Library::ZLIB;
				}
				else {
					printf("Error at argument -l, %s is not a valid compression for archives (only oodle and zlib)\n", type);
					args.mpStorage = NULL;
					return 1;
				}
				break;//etc
			case 'e':
				encrypt = true;
				break;
			case 'f':
				if (i + 1 == args.mSize) {
					printf("Error at argument -f, you need to specify a comma separated list of file name wildcards!\n");
					args.mpStorage = NULL;
					return 1;
				}
				i++;
				matchstr = strtok(args[i], ",");
				if (matchstr) {
					while (matchstr) {
					addmatch:
						std::string match(matchstr);
						size_t dot, star;
						if ((dot=match.find('.')) == std::string::npos || (star=match.find('*')) == std::string::npos) {
							args.mpStorage = NULL;
							printf("Error at argument -f, wildcard '%s' not valid. Must contain a star for the unknown,"
								"and dot for the extenstion separator!",matchstr);
							return 1;
						}
						if (match.size() == 2)continue;
						if (_MAX(dot, star) - _MIN(dot, star) != 1) {
							args.mpStorage = NULL;
							printf("Error at argument -f, wildcard '%s' not valid. Extension separator and unknown "
								"string not sequential!", matchstr);
							return 1;
						}
						if (dot > star && star) {//*.
							args.mpStorage = NULL;
							printf("Error at argument -f, wildcard '%s' not valid.", matchstr);//*. at start only
							return 1;
						}
						if (star > dot && star != match.size() - 1) {//.*
							args.mpStorage = NULL;
							printf("Error at argument -f, wildcard '%s' not valid.", matchstr);//.* at end only
							return 1;
						}
						if (dot > star) {
							match = match.substr(2);//could be optimized
							ends_in.AddElement(0, NULL, &match);
						}
						else {
							match = match.substr(0, match.size() - 2);
							starts_with.AddElement(0, NULL, &match);
						}
						if (b)
							matchstr = strtok(NULL, ",");
					}
				}
				else {
					b = false;
					matchstr = args[i];
					goto addmatch;
				}
				break;
			case 'c':
				compress = true;
				break;
			case 's':
				if (i + 1 == args.mSize) {
					printf("Error at argument -s, you need to specify the section type after it!\n");
					args.mpStorage = NULL;
					return 1;
				}
				i++;
				sect = args[i];
				if (!_stricmp(sect, "header")) {
					dumponly = MetaStream::SectionType::eSection_Header;
				}else if (!_stricmp(sect, "default")) {
					dumponly = MetaStream::SectionType::eSection_Default;
				}
				else if (!_stricmp(sect, "async")) {
					dumponly = MetaStream::SectionType::eSection_Async;
				}
				else if (!_stricmp(sect, "debug")) {
					dumponly = MetaStream::SectionType::eSection_Debug;
				}
				else {
					printf("Error at argument -s, invalid section type '%s'!\n",sect);
					args.mpStorage = NULL;
					return 1;
				}
				break;
			case 'v':
				if (i + 1 == args.mSize) {
					printf("Error at argument -v, you need to specify a number after it!\n");
					args.mpStorage = NULL;
					return 1;
				}
				i++;
				version = atoi(args[i]);
				if (version > 2) {
					printf("Error at argument -v %d, the version you entered is not 0, 1 or 2!\n",version);
					args.mpStorage = NULL;
					return 1;
				}
				break;
			case 'h':
				help = true;
				break;
			default:
				printf("Error at argument '%s': Option not recognised!\n",arg);
				args.mpStorage = NULL;
				return 1;
			}
		}
		else break;
	}
	if (i >= args.GetSize() - 2) {
		printf("Incorrect usage! See the usage:\n\n");
		_PrintUsage();
		args.mpStorage = NULL;
		return 1;
	}
	//TODO get game id then op
	if (starts_with.GetSize() || ends_in.GetSize()) {
		printf("Set all wildcards:\n");
		if (starts_with.GetSize()) {
			printf("\tFiles starting with:\n");
			for (int i = 0; i < starts_with.GetSize(); i++)
				printf("\t\t%s.*\n",static_cast<std::string*>(starts_with.GetElement(i))->c_str());
		}
		if (ends_in.GetSize()) {
			printf("\tFiles ending in:\n");
			for (int i = 0; i < ends_in.GetSize(); i++)
				printf("\t\t*.%s\n", static_cast<std::string*>(ends_in.GetElement(i))->c_str());
		}
		printf("\n\n");
	}
	delete[] filters;
	args.mpStorage = NULL;
	return 0;
}

int main(int argn, char* argv[]) {
	return TEditor_Main(argn, argv);
}
