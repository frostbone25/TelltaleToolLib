// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "Meta.hpp"
#include "Types/TypeIncludes.h"
#include "TelltaleToolLibrary.h"

#define DEFINET(name_,Ty) }++sMetaTypesCount;static MetaClassDescription meta_##name_; \
if(!(meta_##name_.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name_.mpVTable[0] = MetaClassDescription_Typed<Ty>::New;\
meta_##name_.mpVTable[1] = MetaClassDescription_Typed<Ty>::Delete;\
meta_##name_.mpVTable[2] = MetaClassDescription_Typed<Ty>::Construct;\
meta_##name_.mpVTable[3] = MetaClassDescription_Typed<Ty>::CopyConstruct;\
meta_##name_.mpVTable[4] = MetaClassDescription_Typed<Ty>::Destroy;\
meta_##name_.mClassSize = sizeof(Ty);\
meta_##name_.mpTypeInfoExternalName = typeid(Ty).name();

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

