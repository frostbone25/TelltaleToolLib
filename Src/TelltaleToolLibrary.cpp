// This file was written by Lucas Saragosa. If you use this code or library,
// I ask you to you give credit to me and the amazing Telltale Games.

#include "TelltaleToolLibrary.h"

const char* TelltaleToolLib_GetVersion() {
	return _VERSION;
}

void TelltaleToolLib_MakeInternalTypeName(char** _StringPtr) {
    static std::string _sRepl_A = "struct ";
    static std::string _sRepl_B = "class ";
    static std::string _sRepl_C = "enum ";
    static std::string _sRepl_D = "std:: ";
    static std::string _sNothing = "";
	std::string ReplStr(*_StringPtr);
    size_t start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_A, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_A.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_B, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_B.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_C, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_C.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_D, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_D.length(), _sNothing);
    }
    u32 slen = ReplStr.length();
    char* nbuf = (char*)malloc(slen+1);
    memcpy(nbuf, ReplStr.c_str(), slen);
    nbuf[slen] = 0i8;
    free(*_StringPtr);
    *_StringPtr = nbuf;
}