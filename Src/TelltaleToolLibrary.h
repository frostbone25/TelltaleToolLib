// This file was written by Lucas Saragosa. If you use this code or library,
// I ask you to you give credit to me and the amazing Telltale Games.


#ifndef _TTToolLib
#define _TTToolLib

#include <type_traits>
#include <string>

#define INLINE __inline
#define FORCE_INLINE __forceinline

#define _VERSION "1.1"

#define _TTToolLib_Exp __declspec(dllexport)

typedef std::make_unsigned<__int64>::type u64;
typedef std::make_unsigned<__int32>::type u32;
typedef std::make_unsigned<__int16>::type u16;
typedef std::make_unsigned<__int8> ::type u8;
typedef __int8  i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;
typedef std::basic_string<i8,std::char_traits<i8>, std::allocator<i8>> String;

struct Flags {
	u32 mFlags;
};

const char* _TTToolLib_Exp TelltaleToolLib_GetVersion();
//allocate below function param using calloc/malloc
void _TTToolLib_Exp TelltaleToolLib_MakeInternalTypeName(char**);


#endif
