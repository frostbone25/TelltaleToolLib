// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "Meta.hpp"
#include "Types/TypeIncludes.h"
#include "TelltaleToolLibrary.h"

//types that use no copy should not be used in .prop since these functions are needed. .scene files use this, so NO scene should be in a prop
#define DEFINETNOCOPY(name_,Ty) }++sMetaTypesCount;static MetaClassDescription meta_##name_; \
if(!(meta_##name_.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name_.mpVTable[0] = NULL;\
meta_##name_.mpVTable[1] = MetaClassDescription_Typed<Ty>::Delete;\
meta_##name_.mpVTable[2] = NULL;\
meta_##name_.mpVTable[3] = NULL;\
meta_##name_.mpVTable[4] = MetaClassDescription_Typed<Ty>::Destroy;\
meta_##name_.mClassSize = sizeof(Ty);\
meta_##name_.mpTypeInfoExternalName = typeid(Ty).name();

#define DEFINET(name_,Ty) }++sMetaTypesCount;static MetaClassDescription meta_##name_; \
if(!(meta_##name_.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name_.mpVTable[0] = MetaClassDescription_Typed<Ty>::New;\
meta_##name_.mpVTable[1] = MetaClassDescription_Typed<Ty>::Delete;\
meta_##name_.mpVTable[2] = MetaClassDescription_Typed<Ty>::Construct;\
meta_##name_.mpVTable[3] = MetaClassDescription_Typed<Ty>::CopyConstruct;\
meta_##name_.mpVTable[4] = MetaClassDescription_Typed<Ty>::Destroy;\
meta_##name_.mClassSize = sizeof(Ty);\
meta_##name_.mpTypeInfoExternalName = typeid(Ty).name();

#define DEFINET2NOCOPY(name,_Ty) DEFINETNOCOPY(name,_Ty);\
meta_##name##.Initialize(typeid(_Ty))

#define DEFINET2(name,_Ty) DEFINET(name,_Ty);\
meta_##name##.Initialize(typeid(_Ty))

#define ADD(name) meta_##name##.Insert()

#define EXT(name,e) meta_##name##.mpExt = #e;

#define DEFINETABS(name_,Ty) }++sMetaTypesCount;static MetaClassDescription meta_##name_; \
if(!(meta_##name_.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name_.mClassSize = sizeof(Ty);\
meta_##name_.mpTypeInfoExternalName = typeid(Ty).name();

#define DEFINESARRAY(type,count) DEFINET(sarray_##type##_##count##, SArray<type SEP count>);\
meta_sarray_##type##_##count##.Initialize(typeid(SArray<type SEP count>));\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeAsync, SArray<type SEP count>::MetaOperation_SerializeAsync);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeMain, SArray<type SEP count>::MetaOperation_SerializeMain);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeMain);\
DEFINEM(sarray_##type##_##count##,baseclass);\
meta_sarray_##type##_##count##.mpFirstMember = &meta_sarray_##type##_##count##_baseclass;\
meta_sarray_##type##_##count##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_sarray_##type##_##count##_baseclass.mOffset = 0;\
meta_sarray_##type##_##count##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_sarray_##type##_##count##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_sarray_##type##_##count##.Insert();

#define DEFINEDCARRAY(type) DEFINET(DCArray_##type##, DCArray<type>);\
meta_DCArray_##type##.Initialize(typeid(DCArray<type>));\
METAOP_CUSTOM(DCArray_##type##, eMetaOpSerializeAsync, DCArray<type>::MetaOperation_SerializeAsync);\
meta_DCArray_##type##.InstallSpecializedMetaOperation(&meta_DCArray_##type##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(DCArray_##type##, eMetaOpSerializeMain, DCArray<type>::MetaOperation_SerializeMain);\
meta_DCArray_##type##.InstallSpecializedMetaOperation(&meta_DCArray_##type##_eMetaOpSerializeMain);\
DEFINEM(DCArray_##type##,baseclass);\
meta_DCArray_##type##.mpFirstMember = &meta_DCArray_##type##_baseclass;\
meta_DCArray_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_DCArray_##type##_baseclass.mOffset = 0;\
meta_DCArray_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_DCArray_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_DCArray_##type##.Insert();

#define DEFINEDCARRAY2(_Ty,type) DEFINET(DCArray_##type##, DCArray<_Ty>);\
meta_DCArray_##type##.Initialize(typeid(DCArray<_Ty>));\
METAOP_CUSTOM(DCArray_##type##, eMetaOpSerializeAsync, DCArray<_Ty>::MetaOperation_SerializeAsync);\
meta_DCArray_##type##.InstallSpecializedMetaOperation(&meta_DCArray_##type##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(DCArray_##type##, eMetaOpSerializeMain, DCArray<_Ty>::MetaOperation_SerializeMain);\
meta_DCArray_##type##.InstallSpecializedMetaOperation(&meta_DCArray_##type##_eMetaOpSerializeMain);\
DEFINEM(DCArray_##type##,baseclass);\
meta_DCArray_##type##.mpFirstMember = &meta_DCArray_##type##_baseclass;\
meta_DCArray_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_DCArray_##type##_baseclass.mOffset = 0;\
meta_DCArray_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_DCArray_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_DCArray_##type##.Insert();

#define DEFINESET(type) DEFINET(Set_##type##, Set<type>);\
meta_Set_##type##.Initialize(typeid(Set<type>));\
METAOP_CUSTOM(Set_##type##, eMetaOpSerializeAsync, Set<type>::MetaOperation_SerializeAsync);\
DEFINEM(Set_##type##,baseclass);\
meta_Set_##type##.mpFirstMember = &meta_Set_##type##_baseclass;\
meta_Set_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Set_##type##_baseclass.mOffset = 0;\
meta_Set_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Set_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Set_##type##.InstallSpecializedMetaOperation(&meta_Set_##type##_eMetaOpSerializeAsync);\
meta_Set_##type##.Insert();

#define DEFINESET_(_Ty,type) DEFINET(Set_##type##, Set<_Ty>);\
meta_Set_##type##.Initialize(typeid(Set<_Ty>));\
METAOP_CUSTOM(Set_##type##, eMetaOpSerializeAsync, Set<_Ty>::MetaOperation_SerializeAsync);\
DEFINEM(Set_##type##,baseclass);\
meta_Set_##type##.mpFirstMember = &meta_Set_##type##_baseclass;\
meta_Set_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Set_##type##_baseclass.mOffset = 0;\
meta_Set_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Set_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Set_##type##.InstallSpecializedMetaOperation(&meta_Set_##type##_eMetaOpSerializeAsync);\
meta_Set_##type##.Insert();

#define DEFINESARRAY_(_Ty,type,count) DEFINET(sarray_##type##_##count##, SArray<_Ty>);\
meta_sarray_##type##_##count##.Initialize(typeid(SArray<_Ty>));\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeAsync, SArray<_Ty>::MetaOperation_SerializeAsync);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeMain, SArray<_Ty>::MetaOperation_SerializeMain);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeMain);\
DEFINEM(sarray_##type##_##count##,baseclass);\
meta_sarray_##type##_##count##.mpFirstMember = &meta_sarray_##type##_##count##_baseclass;\
meta_sarray_##type##_##count##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_sarray_##type##_##count##_baseclass.mOffset = 0;\
meta_sarray_##type##_##count##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_sarray_##type##_##count##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_sarray_##type##_##count##.Insert();

#define DEFINELIST(type) DEFINET(List_##type##, List<type>);\
meta_List_##type##.Initialize(typeid(List<type>));\
METAOP_CUSTOM(List_##type##, eMetaOpSerializeAsync, List<type>::MetaOperation_SerializeAsync);\
DEFINEM(List_##type##,baseclass);\
meta_List_##type##.mpFirstMember = &meta_List_##type##_baseclass;\
meta_List_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_List_##type##_baseclass.mOffset = 0;\
meta_List_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_List_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_List_##type##.InstallSpecializedMetaOperation(&meta_List_##type##_eMetaOpSerializeAsync);\
meta_List_##type##.Insert();

#define DEFINELIST_(_Ty,type) DEFINET(List_##type##, List<_Ty>);\
meta_List_##type##.Initialize(typeid(List<_Ty>));\
METAOP_CUSTOM(List_##type##, eMetaOpSerializeAsync, List<_Ty>::MetaOperation_SerializeAsync);\
DEFINEM(List_##type##,baseclass);\
meta_List_##type##.mpFirstMember = &meta_List_##type##_baseclass;\
meta_List_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_List_##type##_baseclass.mOffset = 0;\
meta_List_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_List_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_List_##type##.InstallSpecializedMetaOperation(&meta_List_##type##_eMetaOpSerializeAsync);\
meta_List_##type##.Insert();

#define DEFINEDEQUE(type) DEFINET(Deque_##type##, Deque<type>);\
meta_Deque_##type##.Initialize(typeid(Deque<type>));\
METAOP_CUSTOM(Deque_##type##, eMetaOpSerializeAsync, Deque<type>::MetaOperation_SerializeAsync);\
DEFINEM(Deque_##type##,baseclass);\
meta_Deque_##type##.mpFirstMember = &meta_Deque_##type##_baseclass;\
meta_Deque_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Deque_##type##_baseclass.mOffset = 0;\
meta_Deque_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Deque_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Deque_##type##.InstallSpecializedMetaOperation(&meta_Deque_##type##_eMetaOpSerializeAsync);\
meta_Deque_##type##.Insert();

#define NEXTMEM(parent, namestr, memberNameInStruct, pathToMember, typeDesc, flags, previousMember) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = memberOffset(&##pathToMember##::##memberNameInStruct##);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##_##previousMember##.mpNextMember =& meta_##parent##_##memberNameInStruct##;

#define FIRSTMEM(parent, namestr, memberNameInStruct, pathToMember, typeDesc, flags) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = memberOffset(&##pathToMember##::##memberNameInStruct##);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##.mpFirstMember = &meta_##parent##_##memberNameInStruct##;

#define NEXTMEM2(parent, memberNameInStruct, pathToMember, typeDesc, flags, previousMember) \
NEXTMEM(parent, #memberNameInStruct, memberNameInStruct, pathToMember, typeDesc, flags, previousMember)

#define FIRSTMEM2(parent, memberNameInStruct, pathToMember, typeDesc, flags) \
FIRSTMEM(parent, #memberNameInStruct, memberNameInStruct, pathToMember, typeDesc, flags)

#define DEFINEMAP(key,value,less) DEFINET(Map_##key##_##value##, Map<key SEP value SEP less>);\
meta_Map_##key##_##value##.Initialize(typeid(Map<key SEP value SEP less>));\
METAOP_CUSTOM(Map_##key##_##value##, eMetaOpSerializeAsync, Map<key SEP value SEP less>::MetaOperation_SerializeAsync);\
DEFINEM(Map_##key##_##value##,baseclass);\
meta_Map_##key##_##value##.mpFirstMember = &meta_Map_##key##_##value##_baseclass;\
meta_Map_##key##_##value##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Map_##key##_##value##_baseclass.mOffset = 0;\
meta_Map_##key##_##value##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Map_##key##_##value##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Map_##key##_##value##.InstallSpecializedMetaOperation(&meta_Map_##key##_##value##_eMetaOpSerializeAsync);\
meta_Map_##key##_##value##.Insert();

#define DEFINEMAP2(key,value,key_var_name, value_var_name,less) DEFINET(Map_##key_var_name##_##value_var_name##, Map<key SEP value SEP less>);\
meta_Map_##key_var_name##_##value_var_name##.Initialize(typeid(Map<key SEP value SEP less>));\
METAOP_CUSTOM(Map_##key_var_name##_##value_var_name##, eMetaOpSerializeAsync, Map<key SEP value SEP less>::MetaOperation_SerializeAsync);\
DEFINEM(Map_##key_var_name##_##value_var_name##,baseclass);\
meta_Map_##key_var_name##_##value_var_name##.mpFirstMember = &meta_Map_##key_var_name##_##value_var_name##_baseclass;\
meta_Map_##key_var_name##_##value_var_name##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Map_##key_var_name##_##value_var_name##_baseclass.mOffset = 0;\
meta_Map_##key_var_name##_##value_var_name##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Map_##key_var_name##_##value_var_name##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Map_##key_var_name##_##value_var_name##.InstallSpecializedMetaOperation(&meta_Map_##key_var_name##_##value_var_name##_eMetaOpSerializeAsync);\
meta_Map_##key_var_name##_##value_var_name##.Insert();

#define DEFINEHANDLE(name_,Ty) DEFINET(Handle##name_, Handle<Ty>) \
meta_Handle##name_##.Initialize(typeid(Handle<Ty>));\
meta_Handle##name_##.mFlags |= 0x20004;\
DEFINEM(Handle##name_, handlebase);\
meta_Handle##name_##_handlebase.mpName = "Baseclass_HandleBase";\
meta_Handle##name_##_handlebase.mOffset = 0;\
meta_Handle##name_##_handlebase.mpMemberDesc = &meta_handlebase;\
meta_Handle##name_##_handlebase.mFlags |= 0x10;\
meta_Handle##name_##.mpFirstMember = &meta_Handle##name_##_handlebase;\
METAOP_CUSTOM(Handle##name_,eMetaOpSerializeAsync, Handle<Ty>::MetaOperation_SerializeAsync);\
meta_Handle##name_##.InstallSpecializedMetaOperation(&meta_Handle##name_##_eMetaOpSerializeAsync);\
meta_Handle##name_##.Insert();\

#define DEFINEHANDLELOCK(name_,Ty) DEFINET(HandleLock##name_, HandleLock<Ty>) \
meta_HandleLock##name_##.Initialize(typeid(HandleLock<Ty>));\
meta_HandleLock##name_##.mFlags |= 0x20004;\
DEFINEM(HandleLock##name_, handlebase);\
meta_HandleLock##name_##_handlebase.mpName = "Baseclass_HandleBase";\
meta_HandleLock##name_##_handlebase.mOffset = 0;\
meta_HandleLock##name_##_handlebase.mpMemberDesc = &meta_handlebase;\
meta_HandleLock##name_##_handlebase.mFlags |= 0x10;\
meta_HandleLock##name_##.mpFirstMember = &meta_HandleLock##name_##_handlebase;\
METAOP_CUSTOM(HandleLock##name_,eMetaOpSerializeAsync, HandleLock<Ty>::MetaOperation_SerializeAsync);\
meta_HandleLock##name_##.InstallSpecializedMetaOperation(&meta_HandleLock##name_##_eMetaOpSerializeAsync);\
meta_HandleLock##name_##.Insert();\

#define DEFINEOP(name, opName,fid,fun)static MetaOperationDescription meta_##name##_##opName; meta_##name##_##opName.id = fid;\
meta_##name##_##opName.mpOpFn = fun;

#define DEFINEM(name,memberName)static MetaMemberDescription meta_##name##_##memberName

#define METAOP_CUSTOM(Parent,ID,Func) static MetaOperationDescription meta_##Parent##_##ID;meta_##Parent##_##ID.id = \
MetaOperationDescription::ID; meta_##Parent##_##ID.mpOpFn = Func; meta_##Parent##_##ID.mpNext = NULL;

#define SEP ,

namespace MetaInit {

	void Initialize2();
	void Initialize3();
	void Initialize4();

	//ONLY TO BE CALLED BY LIBRARY, USE LIBTELLTALETOOL_INIT
	void Initialize() {
		{
			DEFINET(char, char)
				meta_char.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_char.Initialize("int8");
			meta_char.mpTypeInfoExternalName = typeid(char).name();
			meta_char.mbIsIntrinsic = true;
			METAOP_CUSTOM(char, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint8);
			meta_char.InstallSpecializedMetaOperation(&meta_char_eMetaOpSerializeAsync);
			meta_char.Insert();
			DEFINET(ushort, u16)
				meta_ushort.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_ushort.Initialize("uint16");
			METAOP_CUSTOM(ushort, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncuint16);
			meta_ushort.InstallSpecializedMetaOperation(&meta_ushort_eMetaOpSerializeAsync);
			meta_ushort.Insert();
			meta_ushort.mbIsIntrinsic = true;
			DEFINET(short, i16)
				meta_short.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_short.Initialize("int16");
			METAOP_CUSTOM(short, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint16);
			meta_short.InstallSpecializedMetaOperation(&meta_short_eMetaOpSerializeAsync);
			meta_short.Insert();
			meta_short.mbIsIntrinsic = true;
			DEFINET(int, i32)
				meta_int.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_int.Initialize("int");
			METAOP_CUSTOM(int, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint32);
			meta_int.InstallSpecializedMetaOperation(&meta_int_eMetaOpSerializeAsync);
			meta_int.Insert();
			meta_int.mbIsIntrinsic = true;
			DEFINET(long, long)
				meta_long.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled | MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_long.Initialize("long");
			METAOP_CUSTOM(long, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint32);
			meta_long.InstallSpecializedMetaOperation(&meta_long_eMetaOpSerializeAsync);
			meta_long.Insert();
			meta_long.mbIsIntrinsic = true;
			DEFINET(u64, u64)
				meta_u64.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_u64.Initialize("uint64");
			METAOP_CUSTOM(u64, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncuint64);
			meta_u64.InstallSpecializedMetaOperation(&meta_u64_eMetaOpSerializeAsync);
			meta_u64.Insert();
			meta_u64.mbIsIntrinsic = true;

			DEFINET(float, float)
				meta_float.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled | MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_float.Initialize("float");
			METAOP_CUSTOM(float, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncfloat);
			meta_float.InstallSpecializedMetaOperation(&meta_float_eMetaOpSerializeAsync);
			meta_float.Insert();
			meta_float.mbIsIntrinsic = true;
			DEFINET(double, double)
				meta_double.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_double.Initialize("double");
			METAOP_CUSTOM(double, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncdouble);
			meta_double.InstallSpecializedMetaOperation(&meta_double_eMetaOpSerializeAsync);
			meta_double.Insert();
			meta_double.mbIsIntrinsic = true;
			DEFINET(__int64, __int64)
				meta___int64.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta___int64.Initialize("int64");
			METAOP_CUSTOM(__int64, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint64);
			meta___int64.InstallSpecializedMetaOperation(&meta___int64_eMetaOpSerializeAsync);
			meta___int64.Insert();
			meta___int64.mbIsIntrinsic = true;
			DEFINET(__uint32, unsigned __int32)
				meta___uint32.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta___uint32.Initialize("uint32");
			METAOP_CUSTOM(__uint32, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncuint32);
			meta___uint32.InstallSpecializedMetaOperation(&meta___uint32_eMetaOpSerializeAsync);
			meta___uint32.Insert();
			meta___uint32.mbIsIntrinsic = true;
			DEFINET(__int32, __int32)
				meta___int32.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta___int32.Initialize("int32");
			METAOP_CUSTOM(__int32, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint32);
			meta___int32.InstallSpecializedMetaOperation(&meta___int32_eMetaOpSerializeAsync);
			meta___int32.Insert();
			meta___int32.mbIsIntrinsic = true;
			DEFINET(__uint8, unsigned __int8)
				meta___uint8.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta___uint8.Initialize("uint8");
			METAOP_CUSTOM(__uint8, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncuint8);
			meta___uint8.InstallSpecializedMetaOperation(&meta___uint8_eMetaOpSerializeAsync);
			meta___uint8.Insert();
			meta___uint8.mbIsIntrinsic = true;
			DEFINET(symbol, Symbol)
				meta_symbol.mFlags = MetaFlag::MetaFlag_MetaSerializeNonBlockedVariableSize;
			meta_symbol.Initialize("Symbol");
			METAOP_CUSTOM(symbol, eMetaOpSerializeAsync, Symbol::MetaOperation_SerializeAsync);
			meta_symbol.InstallSpecializedMetaOperation(&meta_symbol_eMetaOpSerializeAsync);
			meta_symbol.Insert();
			DEFINET(flags, Flags)
				meta_flags.Initialize(typeid(Flags));
			DEFINEM(flags, mFlags);
			meta_flags_mFlags.mpName = "mFlags";
			meta_flags_mFlags.mFlags |= MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_flags_mFlags.mpMemberDesc = &meta_long;
			meta_flags_mFlags.mOffset = memberOffset(&Flags::mFlags);
			meta_flags.mpFirstMember = &meta_flags_mFlags;
			meta_flags.mFlags.mFlags |= MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_flags.Insert();
			DEFINET(bool, bool)
				meta_bool.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_bool.Initialize("bool");
			METAOP_CUSTOM(bool, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncbool);
			meta_bool.InstallSpecializedMetaOperation(&meta_bool_eMetaOpSerializeAsync);
			meta_bool.Insert();
			meta_bool.mbIsIntrinsic = true;
			DEFINET(ztest, ZTestFunction)
				meta_ztest.mFlags |= MetaFlag_EnumWrapperClass;
			meta_ztest.Initialize(typeid(ZTestFunction));
			DEFINEM(ztest, mZTestType);
			meta_ztest_mZTestType.mpName = "mZTestType";
			meta_ztest_mZTestType.mpMemberDesc = &meta_long;
			meta_ztest_mZTestType.mOffset = memberOffset(&ZTestFunction::mZTestType);
			meta_ztest.mpFirstMember = &meta_ztest_mZTestType;
			meta_ztest.Insert();
			DEFINET(uidowner, UID::Owner)
				meta_uidowner.Initialize(typeid(UID::Owner));
			DEFINEM(uidowner, miUniqueID);
			meta_uidowner_miUniqueID.mpName = "miUniqueID";
			meta_uidowner_miUniqueID.mpMemberDesc = &meta_long;
			meta_uidowner_miUniqueID.mOffset = memberOffset(&UID::Owner::miUniqueID);
			meta_uidowner.mFlags |= MetaFlag_EditorHide;
			meta_uidowner.mpFirstMember = &meta_uidowner_miUniqueID;
			meta_uidowner.Insert();

			//UID::Generator

			DEFINET(uidgen, UID::Generator)
				meta_uidgen.Initialize(typeid(UID::Generator));
			DEFINEM(uidgen, miNextUniqueID);
			meta_uidgen_miNextUniqueID.mpName = "miNextUniqueID";
			meta_uidgen_miNextUniqueID.mpMemberDesc = &meta_long;
			meta_uidgen_miNextUniqueID.mOffset = memberOffset(&UID::Generator::miNextUniqueID);
			meta_uidgen.mpFirstMember = &meta_uidgen_miNextUniqueID;
			meta_uidgen.Insert();

			//T3VertexSampleDataBase

			DEFINET(t3vsdb, T3VertexSampleDataBase);
			meta_t3vsdb.Initialize(typeid(T3VertexSampleDataBase));

			DEFINEOP(t3vsdb, serialize, MetaOperationDescription::sIDs::eMetaOpSerializeAsync, NULL);
			meta_t3vsdb.InstallSpecializedMetaOperation(&meta_t3vsdb_serialize);

			DEFINEM(t3vsdb, numverts);
			meta_t3vsdb.mpFirstMember = &meta_t3vsdb_numverts;
			meta_t3vsdb_numverts.mpName = "mNumVerts";
			meta_t3vsdb_numverts.mOffset = memberOffset(&T3VertexSampleDataBase::mNumVerts);
			meta_t3vsdb_numverts.mpMemberDesc = &meta_long;

			DEFINEM(t3vsdb, vertsize);
			meta_t3vsdb_vertsize.mpName = "mVertSize";
			meta_t3vsdb_vertsize.mOffset = memberOffset(&T3VertexSampleDataBase::mVertSize);
			meta_t3vsdb_vertsize.mpMemberDesc = &meta_long;
			meta_t3vsdb_numverts.mpNextMember = &meta_t3vsdb_vertsize;

			meta_t3vsdb.Insert();

			DEFINET(string, String);
			meta_string.Initialize("String");
			METAOP_CUSTOM(string, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncString);
			meta_string.InstallSpecializedMetaOperation(&meta_string_eMetaOpSerializeAsync);
			meta_string.mFlags.mFlags |= (int)MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_string.Insert();
			meta_string.mbIsIntrinsic = true;

			//ContainerInterface
			DEFINETABS(cinterface, ContainerInterface);
			meta_cinterface.Initialize("ContainerInterface");
			//Override operatoins: collecttyped, objectstate, scriptunlock, scriptlock,
			meta_cinterface.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled | (int)MetaFlag_BaseClass;
			meta_cinterface.Insert();

			//Vector2
			DEFINET(vec2, Vector2);
			meta_vec2.Initialize(typeid(Vector2));
			meta_vec2.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled | (int)MetaFlag_PlaceInAddPropMenu;
			DEFINEM(vec2, y);
			meta_vec2_y.mpName = "y";
			meta_vec2_y.mOffset = 4;
			meta_vec2_y.mpMemberDesc = &meta_float;
			DEFINEM(vec2, x);
			meta_vec2.mpFirstMember = &meta_vec2_x;
			meta_vec2_x.mpName = "x";
			meta_vec2_x.mOffset = 0;
			meta_vec2_x.mpMemberDesc = &meta_float;
			meta_vec2_x.mpNextMember = &meta_vec2_y;
			meta_vec2.Insert();

			DEFINETABS(handlebase, HandleBase);
			meta_handlebase.Initialize(typeid(HandleBase));
			METAOP_CUSTOM(handlebase, eMetaOpSerializeAsync, HandleBase::MetaOperation_SerializeAsync);
			meta_handlebase.InstallSpecializedMetaOperation(&meta_handlebase_eMetaOpSerializeAsync);
			meta_handlebase.Insert();

			//TRange<uint>
			DEFINET(rangeuint, TRange<unsigned int>);
			meta_rangeuint.Initialize(typeid(TRange<unsigned int>));
			meta_rangeuint.mFlags |= 6;//blocking disabled, add to prop menu
			DEFINEM(rangeuint, max);
			meta_rangeuint_max.mpName = "max";
			meta_rangeuint_max.mOffset = memberOffset(&TRange<unsigned int>::max);
			meta_rangeuint_max.mpMemberDesc = &meta___uint32;
			DEFINEM(rangeuint, min);
			meta_rangeuint_min.mpName = "min";
			meta_rangeuint_min.mOffset = memberOffset(&TRange<unsigned int>::min);
			meta_rangeuint_min.mpMemberDesc = &meta___uint32;
			meta_rangeuint_min.mpNextMember = &meta_rangeuint_max;
			meta_rangeuint.mpFirstMember = &meta_rangeuint_min;
			meta_rangeuint.Insert();
			//TRange<float>
			DEFINET(rangef, TRange<float>);
			meta_rangef.Initialize(typeid(TRange<float>));
			meta_rangef.mFlags |= 6;//blocking disabled, add to prop menu
			DEFINEM(rangef, max);
			meta_rangef_max.mpName = "max";
			meta_rangef_max.mOffset = memberOffset(&TRange<float>::max);
			meta_rangef_max.mpMemberDesc = &meta_float;
			DEFINEM(rangef, min);
			meta_rangef_min.mpName = "min";
			meta_rangef_min.mOffset = memberOffset(&TRange<float>::min);
			meta_rangef_min.mpMemberDesc = &meta_float;
			meta_rangef_min.mpNextMember = &meta_rangef_max;
			meta_rangef.mpFirstMember = &meta_rangef_min;
			meta_rangef.Insert();

			//Vector3
			DEFINET(vec3, Vector3);
			meta_vec3.Initialize(typeid(Vector3));
			meta_vec3.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled | (int)MetaFlag_PlaceInAddPropMenu;
			DEFINEM(vec3, z);
			meta_vec3_z.mpName = "z";
			meta_vec3_z.mpMemberDesc = &meta_float;
			meta_vec3_z.mOffset = 8;
			DEFINEM(vec3, y);
			meta_vec3_y.mpName = "y";
			meta_vec3_y.mpMemberDesc = &meta_float;
			meta_vec3_y.mOffset = 4;
			meta_vec3_y.mpNextMember = &meta_vec3_z;
			DEFINEM(vec3, x);
			meta_vec3.mpFirstMember = &meta_vec3_x;
			meta_vec3_x.mpName = "x";
			meta_vec3_x.mpMemberDesc = &meta_float;
			meta_vec3_x.mOffset = 0;
			meta_vec3_x.mpNextMember = &meta_vec3_y;
			meta_vec3.Insert();
			METAOP_CUSTOM(vec3, eMetaOpSerializeAsync, Vector3::MetaOperation_SerializeAsync);
			meta_vec3.InstallSpecializedMetaOperation(&meta_vec3_eMetaOpSerializeAsync);

			//Vector4
			DEFINET(vec4, Vector4);
			meta_vec4.Initialize(typeid(Vector4));
			meta_vec4.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled;
			DEFINEM(vec4, w);
			meta_vec4_w.mpName = "w";
			meta_vec4_w.mpMemberDesc = &meta_float;
			meta_vec4_w.mOffset = 12;
			DEFINEM(vec4, z);
			meta_vec4_z.mpName = "z";
			meta_vec4_z.mpMemberDesc = &meta_float;
			meta_vec4_z.mOffset = 8;
			meta_vec4_z.mpNextMember = &meta_vec4_w;
			DEFINEM(vec4, y);
			meta_vec4_y.mpName = "y";
			meta_vec4_y.mpMemberDesc = &meta_float;
			meta_vec4_y.mOffset = 4;
			meta_vec4_y.mpNextMember = &meta_vec4_z;
			DEFINEM(vec4, x);
			meta_vec4.mpFirstMember = &meta_vec4_x;
			meta_vec4_x.mpName = "x";
			meta_vec4_x.mpMemberDesc = &meta_float;
			meta_vec4_x.mOffset = 0;
			meta_vec4_x.mpNextMember = &meta_vec4_y;
			meta_vec4.Insert();
			METAOP_CUSTOM(vec4, eMetaOpSerializeAsync, Vector4::MetaOperation_SerializeAsync);
			meta_vec4.InstallSpecializedMetaOperation(&meta_vec4_eMetaOpSerializeAsync);

			//Quaternion
			DEFINET(quat, Quaternion);
			meta_quat.Initialize(typeid(Quaternion));
			meta_quat.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled | (int)MetaFlag_PlaceInAddPropMenu;
			DEFINEM(quat, w);
			meta_quat_w.mpName = "w";
			meta_quat_w.mpMemberDesc = &meta_float;
			meta_quat_w.mOffset = 12;
			DEFINEM(quat, z);
			meta_quat_z.mpName = "z";
			meta_quat_z.mpMemberDesc = &meta_float;
			meta_quat_z.mOffset = 8;
			meta_quat_z.mpNextMember = &meta_quat_w;
			DEFINEM(quat, y);
			meta_quat_y.mpName = "y";
			meta_quat_y.mpMemberDesc = &meta_float;
			meta_quat_y.mOffset = 4;
			meta_quat_y.mpNextMember = &meta_quat_z;
			DEFINEM(quat, x);
			meta_quat.mpFirstMember = &meta_quat_x;
			meta_quat_x.mpName = "x";
			meta_quat_x.mpMemberDesc = &meta_float;
			meta_quat_x.mOffset = 0;
			meta_quat_x.mpNextMember = &meta_quat_y;
			meta_quat.Insert();
			METAOP_CUSTOM(quat, eMetaOpSerializeAsync, Quaternion::MetaOperation_SerializeAsync);
			meta_quat.InstallSpecializedMetaOperation(&meta_quat_eMetaOpSerializeAsync);

			DEFINET(transform, Transform);
			meta_transform.Initialize(typeid(Transform));
			DEFINEM(transform, trans);
			meta_transform_trans.mpName = "mTrans";
			meta_transform_trans.mpMemberDesc = &meta_vec3;
			meta_transform_trans.mOffset = memberOffset(&Transform::mTrans);
			DEFINEM(transform, rot);
			meta_transform_rot.mpName = "mRot";
			meta_transform_rot.mpMemberDesc = &meta_quat;
			meta_transform_rot.mOffset = memberOffset(&Transform::mRot);
			meta_transform_rot.mpNextMember = &meta_transform_trans;
			meta_transform.mpFirstMember = &meta_transform_rot;
			meta_transform.Insert();

			DEFINET(rect, Rect);
			meta_rect.Initialize(typeid(Rect));
			meta_rect.mFlags |= (int)MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			DEFINEM(rect,bottom);
			meta_rect_bottom.mpName = "bottom";
			meta_rect_bottom.mpMemberDesc = &meta_long;
			meta_rect_bottom.mOffset = memberOffset(&Rect::bottom);
			DEFINEM(rect, top);
			meta_rect_top.mpName = "top";
			meta_rect_top.mpMemberDesc = &meta_long;
			meta_rect_top.mOffset = memberOffset(&Rect::top);
			meta_rect_top.mpNextMember = &meta_rect_bottom;
			DEFINEM(rect, right);
			meta_rect_right.mpName = "right";
			meta_rect_right.mpMemberDesc = &meta_long;
			meta_rect_right.mOffset = memberOffset(&Rect::right);
			meta_rect_right.mpNextMember = &meta_rect_top;
			DEFINEM(rect, left);
			meta_rect_left.mpName = "left";
			meta_rect_left.mpMemberDesc = &meta_long;
			meta_rect_left.mOffset = memberOffset(&Rect::left);
			meta_rect.mpFirstMember = &meta_rect_left;
			meta_rect_left.mpNextMember = &meta_rect_right;
			meta_rect.Insert();

			DEFINET(sphere, Sphere);
			meta_sphere.Initialize(typeid(Sphere));
			DEFINEM(sphere, radius);
			meta_sphere_radius.mOffset = memberOffset(&Sphere::mRadius);
			meta_sphere_radius.mpName = "mRadius";
			meta_sphere_radius.mpMemberDesc = &meta_float;
			DEFINEM(sphere, center);
			meta_sphere_center.mOffset = memberOffset(&Sphere::mCenter);
			meta_sphere_center.mpName = "mCenter";
			meta_sphere_center.mpMemberDesc = &meta_vec3;
			meta_sphere_center.mpNextMember = &meta_sphere_radius;
			meta_sphere.mpFirstMember = &meta_sphere_center;
			meta_sphere.Insert();

			DEFINET(color, Color);
			meta_color.Initialize(typeid(Color));
			DEFINEM(color, alpha);
			meta_color_alpha.mpName = "a";
			meta_color_alpha.mOffset = memberOffset(&Color::a);
			meta_color_alpha.mpMemberDesc = &meta_float;
			DEFINEM(color, b);
			meta_color_b.mpName = "b";
			meta_color_b.mOffset = memberOffset(&Color::b);
			meta_color_b.mpMemberDesc = &meta_float;
			meta_color_b.mpNextMember = &meta_color_alpha;
			DEFINEM(color, g);
			meta_color_g.mpName = "g";
			meta_color_g.mOffset = memberOffset(&Color::g);
			meta_color_g.mpMemberDesc = &meta_float;
			meta_color_g.mpNextMember = &meta_color_b;
			DEFINEM(color, r);
			meta_color_r.mpName = "r";
			meta_color_r.mOffset = memberOffset(&Color::r);
			meta_color_r.mpMemberDesc = &meta_float;
			meta_color_r.mpNextMember = &meta_color_g;
			meta_color.mpFirstMember = &meta_color_r;
			meta_color.Insert();
			DEFINET(propvalue, PropertyValue);
			meta_propvalue.Initialize(typeid(PropertyValue));
			meta_propvalue.Insert();
			DEFINET(propkeyinfo, PropertySet::KeyInfo);
			meta_propkeyinfo.Initialize(typeid(PropertySet::KeyInfo));
			DEFINEM(propkeyinfo, value);
			meta_propkeyinfo_value.mpName = "mValue";
			meta_propkeyinfo_value.mOffset = memberOffset(&PropertySet::KeyInfo::mValue);
			meta_propkeyinfo_value.mFlags |= (int)MetaFlag::MetaFlag_EditorHide | (int)MetaFlag::MetaFlag_NoPanelCaption;
			meta_propkeyinfo_value.mpMemberDesc = &meta_propvalue;
			DEFINEM(propkeyinfo, flags);
			meta_propkeyinfo_flags.mFlags |= (int)MetaFlag::MetaFlag_MetaSerializeDisable;
			meta_propkeyinfo_flags.mpName = "mFlags";
			meta_propkeyinfo_flags.mpMemberDesc = &meta_flags;
			meta_propkeyinfo_flags.mpNextMember = &meta_propkeyinfo_value;
			meta_propkeyinfo_flags.mOffset = memberOffset(&PropertySet::KeyInfo::mFlags);
			DEFINEM(propkeyinfo, keyname);
			meta_propkeyinfo_keyname.mFlags |= (int)MetaFlag::MetaFlag_EditorHide;
			meta_propkeyinfo_keyname.mpName = "mKeyName";
			meta_propkeyinfo_keyname.mOffset = memberOffset(&PropertySet::KeyInfo::mFlags);
			meta_propkeyinfo_keyname.mpNextMember = &meta_propkeyinfo_flags;
			meta_propkeyinfo_keyname.mpMemberDesc = &meta_symbol;
			meta_propkeyinfo.mpFirstMember = &meta_propkeyinfo_keyname;
			meta_propkeyinfo.Insert();
			DEFINEHANDLE(propset, PropertySet);
			DEFINESARRAY(u32, 3);
			DEFINESARRAY(u8, 32);
			DEFINESARRAY(i32, 4);
			DEFINESARRAY(i32, 3);
			DEFINESARRAY(float, 9);
			DEFINESARRAY(float, 3);
			DEFINESARRAY_(TRange<float> SEP 3, rangefloat, 3);
			DEFINEDCARRAY(i32);
			DEFINEDCARRAY(u16);
			DEFINEDCARRAY(u64);
			DEFINEDCARRAY(u32);
			DEFINEDCARRAY(u8);
			DEFINEDCARRAY(float);
			DEFINEDCARRAY(bool);
			DEFINEDCARRAY(String);
			DEFINESET(i32);
			DEFINESET(String);
			DEFINESET(u32);
			DEFINESET(u64);
			DEFINELIST(Symbol);
			DEFINELIST(i32);
			DEFINELIST(String);
			DEFINEDEQUE(i32);
			DEFINEDEQUE(String);
			DEFINEMAP(Symbol, String, Symbol::CompareCRC);
			DEFINEMAP(Symbol, Symbol, Symbol::CompareCRC);
			DEFINEMAP(Symbol, float, Symbol::CompareCRC);
			DEFINEMAP(String, int, std::less<String>);
			DEFINEMAP(int, Symbol, std::less<int>);
			DEFINEMAP(int, int, std::less<int>);
			DEFINEMAP(int, float, std::less<int>);
			DEFINEMAP2(Symbol, Set<Symbol>, Symbol, setsymbol, Symbol::CompareCRC);
			DEFINEMAP2(String, DCArray<String>, String, dcarraystring, Symbol::CompareCRC);
			DEFINEMAP2(int, Map<int SEP Map<Symbol SEP float SEP Symbol::CompareCRC>>, int, mapintmapsymbolfloat, std::less<int>);
			DEFINEMAP2(int, Map<int SEP int SEP std::less<int>>, int, mapintint, std::less<int>);
			DEFINEMAP(String, String, std::less<String>);
			DEFINEMAP(int, bool, std::less<int>);
			DEFINESET_(PropertySet::KeyInfo,keyinfo);
			DEFINET(prop, PropertySet);
			meta_prop.Initialize(typeid(PropertySet));
			meta_prop.mpExt = "prop";
			METAOP_CUSTOM(prop, eMetaOpSerializeAsync, PropertySet::MetaOperation_SerializeAsync);
			meta_prop.InstallSpecializedMetaOperation(&meta_prop_eMetaOpSerializeAsync);
			DEFINEM(prop, version);
			meta_prop_version.mpName = "mPropVersion";
			meta_prop_version.mOffset = memberOffset(&PropertySet::mPropVersion);
			meta_prop_version.mpMemberDesc = &meta_long;
			meta_prop.mpFirstMember = &meta_prop_version;
			DEFINEM(prop, flags);
			meta_prop_flags.mpName = "mPropertyFlags";
			meta_prop_flags.mpMemberDesc = &meta_flags;
			meta_prop_flags.mOffset = memberOffset(&PropertySet::mPropertyFlags);
			meta_prop_version.mpNextMember = &meta_prop_flags;
			DEFINEM(prop, keymap);
			meta_prop_keymap.mpName = "mKeyMap";
			meta_prop_keymap.mpMemberDesc = &meta_Set_keyinfo;
			meta_prop_keymap.mFlags |= (int)MetaFlag::MetaFlag_MetaSerializeDisable;
			meta_prop_keymap.mOffset = memberOffset(&PropertySet::mKeyMap);
			meta_prop_flags.mpNextMember = &meta_prop_keymap;
			meta_prop.Insert();

			DEFINET(bc, BoneContraints);
			meta_bc.Initialize(typeid(BoneContraints));
			DEFINEM(bc, btype);
			meta_bc_btype.mOffset = memberOffset(&BoneContraints::mBoneType);
			meta_bc_btype.mpName = "mBoneType";
			meta_bc_btype.mpMemberDesc = &meta_long;
			meta_bc_btype.mFlags |= MetaFlag::MetaFlag_EnumIntType;
			static MetaEnumDescription meta_bce_hinge, meta_bce_joint;//joint=ball oops
			meta_bce_hinge.mpEnumName = "eBoneType_Hinge";
			meta_bce_hinge.mEnumIntValue = 0;
			meta_bce_hinge.mpNext = &meta_bce_joint;
			meta_bce_joint.mpEnumName = "eBoneType_Ball";
			meta_bce_joint.mEnumIntValue = 1;
			meta_bc_btype.mpEnumDescriptions = &meta_bce_hinge;
			DEFINEM(bc, axis);
			meta_bc_btype.mpNextMember = &meta_bc_axis;
			meta_bc_axis.mpName = "mHingeAxis";
			meta_bc_axis.mOffset = memberOffset(&BoneContraints::mHingeAxis);
			meta_bc_axis.mFlags |= MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_bc_axis.mpMemberDesc = &meta_vec3;
			DEFINEM(bc, range);
			meta_bc_axis.mpNextMember = &meta_bc_range;
			meta_bc_range.mpName = "mAxisRange";
			meta_bc_range.mOffset = memberOffset(&BoneContraints::mAxisRange);
			meta_bc_range.mpMemberDesc = &meta_sarray_rangefloat_3;
			meta_bc.mpFirstMember = &meta_bc_btype;
			meta_bc.Insert();

			DEFINET(sklentry, Skeleton::Entry);
			meta_sklentry.Initialize(typeid(Skeleton::Entry));
			DEFINEM(sklentry, flags);
			meta_sklentry_flags.mpName = "mFlags";
			meta_sklentry_flags.mpMemberDesc =& meta_flags;
			meta_sklentry_flags.mOffset = memberOffset(&Skeleton::Entry::mFlags);
			DEFINEM(sklentry, constraints);
			meta_sklentry_constraints.mpName = "mConstraints";
			meta_sklentry_constraints.mpMemberDesc =& meta_bc;
			meta_sklentry_constraints.mOffset = memberOffset(&Skeleton::Entry::mConstraints);
			meta_sklentry_constraints.mpNextMember = &meta_sklentry_flags;
			DEFINEM(sklentry, rgm);
			meta_sklentry_rgm.mpName = "mResourceGroupMembership";
			meta_sklentry_rgm.mOffset = memberOffset(&Skeleton::Entry::mResourceGroupMembership);
			meta_sklentry_rgm.mpNextMember = &meta_sklentry_constraints;
			meta_sklentry_rgm.mpMemberDesc = &meta_Map_Symbol_float;
			DEFINEM(sklentry, ats);
			meta_sklentry_ats.mpName = "mAnimTranslationScale";
			meta_sklentry_ats.mOffset = memberOffset(&Skeleton::Entry::mAnimTranslationScale);
			meta_sklentry_ats.mpNextMember = &meta_sklentry_rgm;
			meta_sklentry_ats.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, lts);
			meta_sklentry_lts.mpName = "mLocalTranslationScale";
			meta_sklentry_lts.mOffset = memberOffset(&Skeleton::Entry::mLocalTranslationScale);
			meta_sklentry_lts.mpNextMember = &meta_sklentry_ats;
			meta_sklentry_lts.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, gts);
			meta_sklentry_gts.mpName = "mGlobalTranslationScale";
			meta_sklentry_gts.mOffset = memberOffset(&Skeleton::Entry::mGlobalTranslationScale);
			meta_sklentry_gts.mpNextMember = &meta_sklentry_lts;
			meta_sklentry_gts.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, restx);
			meta_sklentry_restx.mpName = "mRestXForm";
			meta_sklentry_restx.mOffset = memberOffset(&Skeleton::Entry::mRestXform);
			meta_sklentry_restx.mpNextMember = &meta_sklentry_gts;
			meta_sklentry_restx.mpMemberDesc = &meta_transform;
			DEFINEM(sklentry, localq);
			meta_sklentry_localq.mpName = "mLocalQuat";
			meta_sklentry_localq.mOffset = memberOffset(&Skeleton::Entry::mLocalQuat);
			meta_sklentry_localq.mpNextMember = &meta_sklentry_restx;
			meta_sklentry_localq.mpMemberDesc = &meta_quat;
			DEFINEM(sklentry, localp);
			meta_sklentry_localp.mpName = "mLocalPos";
			meta_sklentry_localp.mOffset = memberOffset(&Skeleton::Entry::mLocalPos);
			meta_sklentry_localp.mpNextMember = &meta_sklentry_localq;
			meta_sklentry_localp.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, blen);
			meta_sklentry_blen.mpName = "mBoneLength";
			meta_sklentry_blen.mOffset = memberOffset(&Skeleton::Entry::mBoneLength);
			meta_sklentry_blen.mpNextMember = &meta_sklentry_localp;
			meta_sklentry_blen.mpMemberDesc = &meta_float;
			meta_sklentry_blen.mFlags |= MetaFlag::MetaFlag_MetaSerializeDisable;
			DEFINEM(sklentry, mblen);
			meta_sklentry_mblen.mpName = "mMirrorBoneIndex";
			meta_sklentry_mblen.mOffset = memberOffset(&Skeleton::Entry::mMirrorBoneIndex);
			meta_sklentry_mblen.mpNextMember = &meta_sklentry_blen;
			meta_sklentry_mblen.mpMemberDesc = &meta_long;
			DEFINEM(sklentry, mbn);
			meta_sklentry_mbn.mpName = "mMirrorBoneName";
			meta_sklentry_mbn.mOffset = memberOffset(&Skeleton::Entry::mMirrorBoneName);
			meta_sklentry_mbn.mpNextMember = &meta_sklentry_mblen;
			meta_sklentry_mbn.mpMemberDesc = &meta_symbol;
			DEFINEM(sklentry, pi);
			meta_sklentry_pi.mpName = "mParentIndex";
			meta_sklentry_pi.mOffset = memberOffset(&Skeleton::Entry::mParentIndex);
			meta_sklentry_pi.mpNextMember = &meta_sklentry_mbn;
			meta_sklentry_pi.mpMemberDesc = &meta_long;
			DEFINEM(sklentry, pn);
			meta_sklentry_pn.mpName = "mParentName";
			meta_sklentry_pn.mOffset = memberOffset(&Skeleton::Entry::mParentName);
			meta_sklentry_pn.mpNextMember = &meta_sklentry_pi;
			meta_sklentry_pn.mpMemberDesc = &meta_symbol;
			DEFINEM(sklentry, jn);
			meta_sklentry_jn.mpName = "mJointName";
			meta_sklentry_jn.mOffset = memberOffset(&Skeleton::Entry::mJointName);
			meta_sklentry_jn.mpNextMember = &meta_sklentry_pn;
			meta_sklentry_jn.mpMemberDesc = &meta_symbol;
			meta_sklentry.mpFirstMember = &meta_sklentry_jn;
			meta_sklentry.Insert();

			DEFINEDCARRAY2(Skeleton::Entry,sklentry);

			DEFINET(skl, Skeleton);
			meta_skl.Initialize(typeid(Skeleton));
			meta_skl.mpExt = "skl";
			DEFINEM(skl, entries);
			meta_skl_entries.mpName = "mEntries";
			meta_skl_entries.mOffset = memberOffset(&Skeleton::mEntries);
			meta_skl_entries.mpMemberDesc = &meta_DCArray_sklentry;
			meta_skl.mpFirstMember = &meta_skl_entries;
			meta_skl.Insert();

			DEFINET(aam, ActorAgentMapper);
			meta_aam.Initialize(typeid(ActorAgentMapper));
			meta_aam.mpExt = "aam";
			DEFINEM(aam, aam);
			meta_aam_aam.mpName = "mActorAgentMap";
			meta_aam_aam.mpMemberDesc = &meta_prop;
			meta_aam_aam.mOffset = memberOffset(&ActorAgentMapper::mActorAgentMap);
			meta_aam.mpFirstMember = &meta_aam_aam;
			DEFINEM(aam, aa);
			meta_aam_aa.mpName = "mActionActors";
			meta_aam_aa.mpMemberDesc = &meta_Set_String;
			meta_aam_aa.mOffset = memberOffset(&ActorAgentMapper::mActionActors);
			meta_aam_aam.mpNextMember = &meta_aam_aa;
			meta_aam.Insert();

			DEFINET(amape, AgentMap::AgentMapEntry);
			meta_amape.Initialize(typeid(AgentMap::AgentMapEntry));
			DEFINEM(amape, name);
			meta_amape_name.mpName = "mzName";
			meta_amape_name.mOffset = memberOffset(&AgentMap::AgentMapEntry::mzName);
			meta_amape_name.mpMemberDesc = &meta_string;

			DEFINEM(amape, Actor);
			meta_amape_Actor.mpName = "mzActor";
			meta_amape_Actor.mOffset = memberOffset(&AgentMap::AgentMapEntry::mzActor);
			meta_amape_Actor.mpMemberDesc = &meta_string;
			meta_amape_name.mpNextMember = &meta_amape_Actor;

			DEFINEM(amape, name2);
			meta_amape_name2.mpName = "mazModels";
			meta_amape_name2.mOffset = memberOffset(&AgentMap::AgentMapEntry::mazModels);
			meta_amape_name2.mpMemberDesc = &meta_Set_String;
			meta_amape_Actor.mpNextMember = &meta_amape_name2;
		
			DEFINEM(amape, name3);
			meta_amape_name3.mpName = "mazGuides";
			meta_amape_name3.mOffset = memberOffset(&AgentMap::AgentMapEntry::mazGuides);
			meta_amape_name3.mpMemberDesc = &meta_Set_String;
			meta_amape_name2.mpNextMember = &meta_amape_name3;

			DEFINEM(amape, name4);
			meta_amape_name4.mpName = "mazStyleIdles";
			meta_amape_name4.mOffset = memberOffset(&AgentMap::AgentMapEntry::mazStyleIdles);
			meta_amape_name4.mpMemberDesc = &meta_Set_String;
			meta_amape_name3.mpNextMember = &meta_amape_name4;

			meta_amape.mpFirstMember = &meta_amape_name;
			meta_amape.Insert();

			DEFINEMAP2(String, AgentMap::AgentMapEntry, str, amape, std::less<String>);

			DEFINET(amap, AgentMap);
			meta_amap.Initialize(typeid(AgentMap));
			meta_amap.mpExt = "amap";
			DEFINEM(amap, agents);
			meta_amap_agents.mpName = "maAgents";
			meta_amap_agents.mOffset = memberOffset(&AgentMap::maAgents);
			meta_amap_agents.mpMemberDesc = &meta_Map_str_amape;
			meta_amap.mpFirstMember = &meta_amap_agents;
			meta_amap.Insert();

			DEFINET(acol, AssetCollection);
			meta_acol.Initialize(typeid(AssetCollection));
			meta_acol.mpExt = "acol";
			DEFINEM(acol, inc);
			DEFINEM(acol, exl);
			DEFINEM(acol, pre);
			meta_acol_inc.mpMemberDesc = &meta_DCArray_String;
			meta_acol_exl.mpMemberDesc = &meta_DCArray_String;
			meta_acol_pre.mpMemberDesc = &meta_string;
			meta_acol_inc.mOffset = memberOffset(&AssetCollection::mIncludeMasks);
			meta_acol_exl.mOffset = memberOffset(&AssetCollection::mExcludeMasks);
			meta_acol_pre.mOffset = memberOffset(&AssetCollection::mPreFilter);
			meta_acol_inc.mpName = "mIncludeMasks";
			meta_acol_exl.mpName = "mExcludeMasks";
			meta_acol_pre.mpName = "mPreFilter";
			meta_acol_pre.mFlags |= MetaFlag::MetaFlag_EditorHide;
			meta_acol.mpFirstMember = &meta_acol_inc;
			meta_acol_inc.mpNextMember = &meta_acol_exl;
			meta_acol_exl.mpNextMember = &meta_acol_pre;
			meta_acol.Insert();

			DEFINEMAP2(String, SoundBusSystem::BusDescription, str, bd, std::less<String>);//we can define since it doesnt ref the mcd yet
	
			DEFINET(busd, SoundBusSystem::BusDescription);
			meta_busd.Initialize(typeid(SoundBusSystem::BusDescription));
			FIRSTMEM(busd, "fVolumedB", fVolumedB, SoundBusSystem::BusDescription, float, 0);
			NEXTMEM(busd, "fReverbWetLeveldB", fReverbWetLeveldB, SoundBusSystem::BusDescription, float, 0, fVolumedB);
			NEXTMEM(busd, "fLowPassFrequencyCutoff", fLowPassFrequencyCutoff, SoundBusSystem::BusDescription, float, 0, fReverbWetLeveldB);
			NEXTMEM(busd, "fHighPassFrequencyCutoff", fHighPassFrequencyCutoff, SoundBusSystem::BusDescription, float, 0, fLowPassFrequencyCutoff);
			NEXTMEM(busd, "bEnableLowPass", bEnableLowPass, SoundBusSystem::BusDescription, bool, 0, fHighPassFrequencyCutoff);
			NEXTMEM(busd, "bEnableHighPass", bEnableHighPass, SoundBusSystem::BusDescription, bool, 0, bEnableLowPass);
			NEXTMEM(busd, "children", children, SoundBusSystem::BusDescription, Map_str_bd, 0, bEnableHighPass);
			NEXTMEM(busd, "autoAssignPatternCollection", autoAssignPatternCollection, SoundBusSystem::BusDescription, acol, 0, children);
			meta_busd.Insert();

			DEFINET(bus, SoundBusSystem::BusHolder);
			meta_bus.Initialize(typeid(SoundBusSystem::BusHolder));
			meta_bus.mpExt = "audiobus";
			DEFINEM(bus, mbus);
			meta_bus_mbus.mpName = "masterBus";
			meta_bus_mbus.mOffset = memberOffset(&SoundBusSystem::BusHolder::masterBus);
			meta_bus_mbus.mpMemberDesc = &meta_busd;
			DEFINEM(bus, am);
			meta_bus_am.mpName = "assetMap";
			meta_bus_mbus.mpNextMember = &meta_bus_am;
			meta_bus_am.mOffset = memberOffset(&SoundBusSystem::BusHolder::assetMap);
			meta_bus_am.mpMemberDesc = &meta_Map_Symbol_Symbol;
			meta_bus_am.mFlags |= MetaFlag::MetaFlag_EditorHide;
			meta_bus.mpFirstMember = &meta_bus_mbus;
			meta_bus.Insert();

			DEFINET2(imapm, InputMapper::EventMapping);
			FIRSTMEM(imapm, "mInputCode", mInputCode, InputMapper::EventMapping, long, MetaFlag::MetaFlag_EnumIntType);
			NEXTMEM2(imapm, mEvent, InputMapper::EventMapping, long, MetaFlag::MetaFlag_EnumIntType, mInputCode);
			NEXTMEM2(imapm, mScriptFunction, InputMapper::EventMapping, string, 0, mEvent);
			NEXTMEM2(imapm, mControllerIndexOverride, InputMapper::EventMapping, long, 0, mScriptFunction);
			ADD(imapm);

			DEFINEDCARRAY2(InputMapper*, imapptr);//telltale made a mistake! would not need to serialize ptrs? has overriden metaop tho

			DEFINET2(imapr, InputMapper::RawEvent);
			FIRSTMEM2(imapr, mKey, InputMapper::RawEvent, long, MetaFlag::MetaFlag_EnumIntType);
			NEXTMEM2(imapr, mType, InputMapper::RawEvent, long, MetaFlag::MetaFlag_EnumIntType, mKey);
			NEXTMEM2(imapr, mX, InputMapper::RawEvent, float, 0, mType);
			NEXTMEM2(imapr, mY, InputMapper::RawEvent, float, 0, mX);
			NEXTMEM2(imapr, mController, InputMapper::RawEvent, long, 0, mY);
			NEXTMEM2(imapr, mIMAPFilter, InputMapper::RawEvent, DCArray_imapptr, MetaFlag::MetaFlag_NoPanelCaption | MetaFlag::MetaFlag_BaseClass, mController);
			ADD(imapr);

			DEFINEDCARRAY2(InputMapper::EventMapping, eventmapping);

			DEFINET2(imap, InputMapper);
			EXT(imap, imap);
			METAOP_CUSTOM(imap, eMetaOpSerializeAsync, InputMapper::MetaOperation_SerializeAsync);
			FIRSTMEM2(imap, mName, InputMapper, string, 0);
			NEXTMEM2(imap, mMappedEvents, InputMapper, DCArray_eventmapping, 0, mName);
			ADD(imap);

			DEFINET2(pmapm, PlatformInputMapper::EventMapping);
			FIRSTMEM2(pmapm, mPlatformInputCode, PlatformInputMapper::EventMapping, long, 0);
			NEXTMEM2(pmapm, mInputCode, PlatformInputMapper::EventMapping, long, 0, mPlatformInputCode);
			ADD(pmapm);

			DEFINEHANDLELOCK(scenelock, Scene);
			DEFINEDCARRAY2(HandleLock<Scene>, hlscene);

			DEFINET2(sceneagent, Scene::AgentInfo);
			FIRSTMEM2(sceneagent, mAgentName, Scene::AgentInfo, string, 0);
			NEXTMEM2(sceneagent, mAgentSceneProps, Scene::AgentInfo, prop, 0, mAgentName);
			ADD(sceneagent);

			DEFINELIST_(Scene::AgentInfo*, agentinfoptr);

			DEFINET2NOCOPY(scene, Scene);
			METAOP_CUSTOM(scene, eMetaOpSerializeAsync, Scene::MetaOperation_SerializeAsync);
			FIRSTMEM2(scene, mTimeScale, Scene, float, 1);
			NEXTMEM2(scene, mbActive, Scene, bool, 1, mTimeScale);
			NEXTMEM2(scene, mbHidden, Scene, bool, 0, mbActive);
			NEXTMEM2(scene, mName, Scene, string, 0, mbHidden);
			NEXTMEM2(scene, mAgentList, Scene, List_agentinfoptr, 1, mName);
			NEXTMEM2(scene, mReferencedScenes, Scene, DCArray_hlscene, 0, mAgentList);
			EXT(scene, scene);
			ADD(scene);

		}
		Initialize2();
		Initialize3();
		Initialize4();
	}

	//The rest of the initialize functions would be for that they say but it would take long to search each member up so its in initialize()

	void Initialize2() {



		//HANDLES
	}

	void Initialize3() {



		//ANIM VALUE INTERFACES
	}

	void Initialize4() {



		//CONTAINERS 
	}

}

