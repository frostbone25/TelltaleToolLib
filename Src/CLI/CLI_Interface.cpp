// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../TelltaleToolLibrary.h"

#include "../Types/DCArray.h"
#include <iostream>
#include "dirent.h"//free impl!
#include <filesystem>
#include "../Types/PropertySet.h"
#include "../TTArchive2.hpp"

#define _MAX(x,y) max(x,y)
#define _MIN(x,y) min(x,y)

namespace TEditorCLI {

	bool _FileExists(const char* fp) {
		return std::filesystem::is_regular_file(fp);
	}

	bool _DirExists(const char* dp) {
		return std::filesystem::is_directory(dp);
	}

	void _PrintUsage() {
		//"\tTEditor_CLI.exe\n\n"
		//creating archive wont work since it would open to many file streams (512 max) for lots of files
		printf("Usage:\n"
			"<> are required arguments, [] are non required, ':' seperates input strings. \n\n\tUsage: TEditor_CLI.exe <game_id> "
			"[options] \n\t\t<extractarc:createarc:dump:import:export> [args]\n\nSee the GameIDs.txt for game IDs.\n\nOperation Usage Examples:\n"
			"\tTEditor_CLI.exe MC2 extractarc \"c:/my_archive.ttarch2\" \"c:/output_folder\"\n"
			"\tTEditor_CLI.exe MCSM -c -l oodle -e -f *.d3dmesh,*.d3dtx,adv_.* importarc \"c:/input_folder\" \"c:/my_desc_archive.ttarch2\"\n"
			"\tTEditor_CLI.exe WDC dump \"c:/path/to/my_input_meta_file\" \"c:/path/to/output_file.txt\"\n"
			"\tTEditor_CLI.exe WD3 import \"c:/path/to/input_script_or_file\" \"c:/path/to/output_script_or_meta_file\"\n"
			"\tTEditor_CLI.exe WD3 export \"c:/path/to/input_script_or_meta_stream_file\" \"c:/path/to/output_script_or_file\"\n"
			"\nOptions:\n\t-l <oodle:zlib> : Specify the compression library to compress the archive with (used in importarc)\n\t"
			"-e : Encrypt the archive (used in import arc)\n\t"
			"-c : Compress the archive (used in import arc)\n\t"
			"-v : Specify the output archive version (used in import arc. Version 1 is for TTA3 (wolf among us), version 2 for TTA4 (rest))\n\t"
			"\tNote that version 2 is the default version!\n\t"
			"-f <exp> : Filter which files types/names to import or export from or to an archive. Examples: '*.lua','obj_objectName.*'\n\t"
			"-s <header:default:debug:async> : Dump only the specified section of the file. (used im dump)\n\t"
			"-h : Use with any operation (eg extractarc/import/dump etc) to only print out how to use this operation or print what it does.\n\t");
	}

	void TEditor_MakeArchive_Callback(const char* msg, float p) {
		if (msg)
			printf("[%f%%] %s...\n",p,msg);
		else
			printf("%f%%...\n",p);
	}

	int TEditor_Main(int argn, char* argv[]) {
		printf("need to fix! CLI not available at the moment. please contact me (github) if you feel I should remake it");
		exit(1);
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
		u32 version = 2;
		int index = 0;
		MetaStream::SectionType dumponly = MetaStream::SectionType::eSection_Count;
		int argi = 0;
		bool b = true;
		char* type, * matchstr, * sect;
		printf("\n\tTellale Games File Editor v1.0.1 By Lucas Saragosa\n\n");
		for (argi; argi < args.mSize; argi++) {
			char* arg = args[argi];
			if (*arg == '-') {//OPT
				switch (arg[1]) {
				case 'l':
					if (argi + 1 == args.mSize) {
						printf("Error at argument -l, you need to specify the compression type after it!\n");
						args.mpStorage = NULL;
						return 1;
					}
					++argi;
					type = args[argi];
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
					if (argi + 1 == args.mSize) {
						printf("Error at argument -f, you need to specify a comma separated list of file name wildcards!\n");
						args.mpStorage = NULL;
						return 1;
					}
					argi++;
					matchstr = strtok(args[argi], ",");
					if (matchstr) {
						while (matchstr) {
						addmatch:
							std::string match(matchstr);
							size_t dot, star;
							if ((dot = match.find('.')) == std::string::npos || (star = match.find('*')) == std::string::npos) {
								args.mpStorage = NULL;
								printf("Error at argument -f, wildcard '%s' not valid. Must contain a star for the unknown, "
									"and dot for the extenstion separator!", matchstr);
								return 1;
							}
							if (match.size() == 2) {
								printf("- Ignoring generic wildcard '%s'\n", matchstr);
								if (b)
									matchstr = strtok(NULL, ",");
								else
									matchstr = NULL;
								continue;
							}
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
							else
								matchstr = NULL;
						}
					}
					else {
						b = false;
						matchstr = args[argi];
						goto addmatch;
					}
					break;
				case 'c':
					compress = true;
					break;
				case 's':
					if (argi + 1 == args.mSize) {
						printf("Error at argument -s, you need to specify the section type after it!\n");
						args.mpStorage = NULL;
						return 1;
					}
					argi++;
					sect = args[argi];
					if (!_stricmp(sect, "header")) {
						dumponly = MetaStream::SectionType::eSection_Header;
					}
					else if (!_stricmp(sect, "default")) {
						dumponly = MetaStream::SectionType::eSection_Default;
					}
					else if (!_stricmp(sect, "async")) {
						dumponly = MetaStream::SectionType::eSection_Async;
					}
					else if (!_stricmp(sect, "debug")) {
						dumponly = MetaStream::SectionType::eSection_Debug;
					}
					else {
						printf("Error at argument -s, invalid section type '%s'!\n", sect);
						args.mpStorage = NULL;
						return 1;
					}
					break;
				case 'v':
					if (argi + 1 == args.mSize) {
						printf("Error at argument -v, you need to specify a number after it!\n");
						args.mpStorage = NULL;
						return 1;
					}
					argi++;
					version = atoi(args[argi]);
					if (version > 2) {
						printf("Error at argument -v %d, the version you entered is not 0, 1 or 2!\n", version);
						args.mpStorage = NULL;
						return 1;
					}
					break;
				case 'h':
					help = true;
					break;
				default:
					printf("Error at argument '%s': Option not recognised!\n", arg);
					args.mpStorage = NULL;
					return 1;
				}
			}
			else break;
		}
		if (!help) {
			if (argi > args.GetSize() - 2) {
				_PrintUsage();
				printf("Incorrect usage! See the usage above. \n");
				args.mpStorage = NULL;
				return 1;
			}
			TelltaleToolLib_SetBlowfishKey(args[argi++]);
			printf("- Set Game ID to %s\n", sBlowfishKeys[sSetKeyIndex].game_id);
		}
		else {
			if (argi <= args.GetSize() - 2) {
				argi++;
			}else if (argi > args.GetSize() - 1) {
				printf("Incorrect usage! You must specify a command for help with:\n");
				args.mpStorage = NULL;
				return 1;
			}
		}
		char* op = args[argi++];
		if (starts_with.GetSize() || ends_in.GetSize()) {
			printf("- Set all wildcards:\n");
			if (starts_with.GetSize()) {
				printf("\tFiles starting with:\n");
				for (int i = 0; i < starts_with.GetSize(); i++)
					printf("\t\t%s.*\n", static_cast<std::string*>(starts_with.GetElement(i))->c_str());
			}
			if (ends_in.GetSize()) {
				printf("\tFiles ending in:\n");
				for (int i = 0; i < ends_in.GetSize(); i++)
					printf("\t\t*.%s\n", static_cast<std::string*>(ends_in.GetElement(i))->c_str());
			}
			printf("\n");
		}
		int _Ret = 0;
#define inoutfiles char* in = args[argi++], *out = args[argi++];
		if (!_stricmp(op, "importarc")) {
			if (help) {
				printf("Usage: [options] importarc input_folder output_file. This creates a .ttarch2 archive."
					" Please note that this wont create files bigger than 4GB!\n");
				goto endhelp;
			}
			if (argi > args.GetSize() - 2) {
				_Ret = 1;
				_PrintUsage();
				printf("Incorrect usage! See the usage above. \n");
			}
			else {
				inoutfiles;
				if (!_DirExists(in)) {
					printf("Could not locate input directory %s\n", in);
					args.mpStorage = 0;
					return 1;
				}
				FILE* file = fopen(out, "wb");
				if (!file) {
					printf("Could not find or create output file %s!\n", in);
					args.mpStorage = 0;
					return 1;
				}
				DataStreamFileDisc d(file, DataStreamMode::eMode_Write);
				std::vector<TTArchive2::ResourceCreateEntry> ent;
				
				struct dirent* dp;
				DIR* dir = opendir(in);
				std::string f;
				while (dp = readdir(dir)) {
					if (dp->d_type == DT_REG) {
						f = in;
						if (!(f.ends_with('/')|| f.ends_with('\\')))
							f += '/';
						f += dp->d_name;
						TTArchive2::ResourceCreateEntry e;
						e.mName = new char[dp->d_namlen+1];
						e.mName[dp->d_namlen] = 0;
						memcpy(e.mName, dp->d_name, dp->d_namlen);
						e.mNameLen = dp->d_namlen;
						e.mpStream = _OpenDataStreamFromDisc(
							f.c_str(), DataStreamMode::eMode_Read);
						ent.push_back(e);
					}
				}
				closedir(dir);
				printf("Importing and creating archive... (Please wait)\n");
				bool b = TTArchive2::Create(TEditor_MakeArchive_Callback,&d, ent.data(), 
					ent.size(), encrypt, compress, lib, version);
				for (int i = 0; i < ent.size(); i++) {
					delete (ent.data() + i)->mpStream;
					delete[](ent.data() + i)->mName;
				}
				if (!b) {
					printf("Failed to create archive. Please contact me!\n");
					_Ret = 2;
				}
			}
		}
		else if (!_stricmp(op, "import")) {
			if (help) {
				printf("Usage: [options] import input_file output_file. This imports a general file type to a telltale one"
					". For example, this can convert from a .obj to .d3dmesh, or png/dds to .d3dtx, etc\n");
				goto endhelp;
			}
			if (argi > args.GetSize() - 2) {
				_Ret = 1;
				_PrintUsage();
				printf("Incorrect usage! See the usage above. \n");
			}
			else {
				inoutfiles;
			}
		}
		else if (!_stricmp(op, "export")) {
			if (help) {
				printf("Usage: [options] export input_file output_file. This exports a telltale file as a general file type"
					". General file types are general for its use, for example a general texture file is .dds, .obj for meshes. etc\n");
				goto endhelp;
			}
			if (argi > args.GetSize() - 2) {
				_Ret = 1;
				_PrintUsage();
				printf("Incorrect usage! See the usage above. \n");
			}
			else {
				inoutfiles;
			}
		}
		else if (!_stricmp(op, "dump")) {
			if (help) {
				printf("Usage: [options] dump input_file output_file. This dumps the file data from the meta stream. Find out what this "
					"looks like for yourself! The output file is a text file.\n");
				goto endhelp;
			}
			if (argi > args.GetSize() - 2) {
				_Ret = 1;
				_PrintUsage();
				printf("Incorrect usage! See the usage above. \n");
			}
			else {
				inoutfiles;
			}
		}
		else {
			if (!_stricmp(op, "extractarc")) {
				if (help) {
					printf("Usage: [options] exportarc input_file output_folder. This extracts a .ttarch2 archive.\n");
					goto endhelp;
				}
				if (argi > args.GetSize() - 2) {
					_Ret = 1;
					_PrintUsage();
					printf("Incorrect usage! See the usage above. \n");
				}
				else {
					inoutfiles;
					if (!_FileExists(in)) {
						printf("Could not locate input file %s\n",in);
						args.mpStorage = 0;
						return 1;
					}
					if (!_DirExists(out)) {
						printf("Could not locate output directory %s\n", out);
						args.mpStorage = 0;
						return 1;
					}
					TTArchive2 arc;
					arc.Activate(_OpenDataStreamFromDisc(in, DataStreamMode::eMode_Read));
					printf("\n\nArchive Details: \n\tFile Count: %d\n\tVersion: %d\n\tCompression: %s\n\tEncrypted: %s\n",
						arc.mResources.size(), arc.mVersion, static_cast<DataStreamContainer*>(arc.mpInStream)->IsCompressed()
						? static_cast<DataStreamContainer*>(arc.mpInStream)->GetCompressionLibrary() == Compression::Library::OODLE
						? "Oodle" : "ZLib" : "None", static_cast<DataStreamContainer*>(arc.mpInStream)->IsEncrypted() ? "Yes" : "No");
					char* nt = new  char[arc.mNamePageCount * 0x10000];
					arc.mpNameStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
					arc.mpNameStream->Serialize(nt, arc.mNamePageCount * 0x10000);
					int len = 0;
					char* cur = NULL;
					std::string s;
					for (int i = 0; i < arc.mResources.size(); i++) {
						TTArchive2::ResourceEntry* entry = arc.mResources.data() + i;
						std::string path(out);//ik this can be optimized idc
						if (!path.ends_with("/")||!path.ends_with("\\"))
							path += '/';
						path += (cur = nt + (entry->mNamePageIndex * 0x10000) + entry->mNamePageOffset);
						if (starts_with.GetSize()) {
							s = cur;
							for (int x = 0; x < starts_with.mSize; x++) {
								if (s.starts_with(starts_with[x]))
									goto endsw;
							}
							continue;
						}
					endsw:
						if (ends_in.GetSize()) {
							s = cur;
							for (int x = 0; x < ends_in.mSize; x++) {
								if (s.ends_with(ends_in[x]))
									goto endi;
							}
							continue;
						}
					endi:
						printf("Extract: %s...\n", cur);
						DataStreamFileDisc ostream = _OpenDataStreamFromDisc_(path.c_str(), DataStreamMode::eMode_Write);
						DataStream* istream = arc.GetResourceStream(entry);
						istream->Copy(&ostream, 0, 0, istream->GetSize());
						delete istream;
					}
					delete[] nt;
				}
			}
			else {
				printf("Unknown operation '%s'.\n\n", op);
				_PrintUsage();
				_Ret = 1;
			}
		}
		if (!_Ret) {
			printf("\nThank you for using the Telltale Editor (CLI)! By Lucas Saragosa.\n");
		}
		endhelp:
		args.mpStorage = NULL;
		return _Ret;
	}

}

#include "../Meta.hpp"
#include "../HashDB/HashDB.h"

struct _LeakSlave {
	~_LeakSlave() {
		_CrtDumpMemoryLeaks();
	}
};

void _anm_chk() {
	TelltaleToolLib_Initialize("FABLES");

	TelltaleToolLib_SetGlobalHashDatabaseFromStream(
		_OpenDataStreamFromDisc("c:/users/lucas/desktop/My Stuff/Projects/HashDB Creator/LibTelltale DB/LibTelltale.HashDB",
			DataStreamMode::eMode_Read));

	struct dirent* dp;
	static char dir1[1000];
	DIR* dir = opendir("D:/games/telltale archives/tales from the borderlands");
	const char* s = "D:/games/telltale archives/tales from the borderlands/";//TODO check games
	int l = strlen(s);
	memcpy(dir1, s, l);
	std::string f;
	static u64 array[0x1000];
	while (dp = readdir(dir)) {
		if (dp->d_type == DT_REG && dp->d_name[dp->d_namlen-1] == 'g' && dp->d_name[dp->d_namlen - 2] == 'o'
			&& dp->d_name[dp->d_namlen - 3] == 'l' && dp->d_name[dp->d_namlen - 4] == 'd') {
			MetaStream meta{};
			memcpy(dir1 + l, dp->d_name, dp->d_namlen + 1);
			meta.Open(_OpenDataStreamFromDisc(dir1, DataStreamMode::eMode_Read), MetaStreamMode::eMetaStream_Read, { 0 });
			printf("- done %s\n",dir1);
			meta.SetPos(0x10);
			u64 d = 0;
			meta.serialize_uint64(&d);
			if (d != 8) {
				printf("%s !!!!!!!!!!!!!!!\n",dir1);
			}
		}
	}
	closedir(dir);
	printf("- done\n");
	u64 c;
	int i = 0;
	while ((c = array[i]) != 0) {
		printf("Type: %llx\n", c);
		i++;
	}
}

#include "../Types/Dialog.h"

void _testing_func() {
	TelltaleToolLib_Initialize("MC2");

	TelltaleToolLib_SetGlobalHashDatabaseFromStream(
		_OpenDataStreamFromDisc("c:/users/lucas/desktop/My Stuff/Projects/HashDB Creator/LibTelltale DB/LibTelltale.HashDB",
			DataStreamMode::eMode_Read));

	DataStream* stream = OpenDataStreamFromDisc(
		"D:/games/telltale archives/minecraft story mode - season 1 & 2"
		"/env_underneathFredTown.dlog", READ);
	{
		MetaStream meta{};
		meta.Open(stream, MetaStreamMode::eMetaStream_Read, { 0 });
		Dlg dlg{};
		MetaOpResult r = PerformMetaSerializeAsync(&meta, &dlg);
		printf("info %s %d\n", dlg.mName.c_str(), (i32)r);
		printf("-done\n");
	}
}

int main(int argn, char* argv[]) {

	_LeakSlave _s;

	{
		_testing_func();
	}

	return 0;
	//return TEditorCLI::TEditor_Main(argn, argv);
}
