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

#define ADDFLAGS(name, flags) meta_##name.mFlags |= flags

#define DEFINESARRAY_(_Ty,type,count) DEFINET(sarray_##type##_##count##, SArray<_Ty SEP count>);\
meta_sarray_##type##_##count##.Initialize(typeid(SArray<_Ty SEP count>));\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeAsync, SArray<_Ty SEP count>::MetaOperation_SerializeAsync);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeMain, SArray<_Ty SEP count>::MetaOperation_SerializeMain);\
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

#define FIRSTMEM1(parent, namestr, memberNameInStruct, memberAlias, pathToMember, typeDesc, flags) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberAlias);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##.mpFirstMember = &meta_##parent##_##memberNameInStruct##;

#define NEXTMEM1(parent, namestr, memberNameInStruct, memberAlias, pathToMember, typeDesc, flags, previousMember) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberAlias);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##_##previousMember##.mpNextMember =& meta_##parent##_##memberNameInStruct##;


#define NEXTMEM4(parent, memberNameInStruct, pathToMember, typeDesc, flags, previousMember, minVersion, maxVersion) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = #memberNameInStruct;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##_##previousMember##.mpNextMember =& meta_##parent##_##memberNameInStruct##;\
meta_##parent##_##memberNameInStruct##.mGameIndexVersionRange.min = minVersion;\
meta_##parent##_##memberNameInStruct##.mGameIndexVersionRange.max = maxVersion;

#define NEXTMEM(parent, namestr, memberNameInStruct, pathToMember, typeDesc, flags, previousMember) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##_##previousMember##.mpNextMember =& meta_##parent##_##memberNameInStruct##;


#define FIRSTMEM(parent, namestr, memberNameInStruct, pathToMember, typeDesc, flags) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberNameInStruct);\
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

#define FIRSTENUM(parent,child,enumName, enumIndex, flags) static MetaEnumDescription meta_##parent##_##child##_##enumName##;\
meta_##parent##_##child##_##enumName##.mpEnumName = #enumName;\
meta_##parent##_##child##_##enumName##.mEnumIntValue = enumIndex;\
meta_##parent##_##child##_##enumName##.mFlags |= flags;\
meta_##parent##_##child##.mpEnumDescriptions = &meta_##parent##_##child##_##enumName##;

#define NEXTENUM(parent,child,enumName, enumIndex, flags, previous) static MetaEnumDescription meta_##parent##_##child##_##enumName##;\
meta_##parent##_##child##_##enumName##.mpEnumName = #enumName;\
meta_##parent##_##child##_##enumName##.mEnumIntValue = enumIndex;\
meta_##parent##_##child##_##enumName##.mFlags |= flags;\
meta_##parent##_##child##_##previous.mpNext = &meta_##parent##_##child##_##enumName##;

#define FIRSTENUM2(parent,child,enumNameStr, enumName, enumIndex, flags) static MetaEnumDescription meta_##parent##_##child##_##enumName##;\
meta_##parent##_##child##_##enumName##.mpEnumName = enumNameStr;\
meta_##parent##_##child##_##enumName##.mEnumIntValue = enumIndex;\
meta_##parent##_##child##_##enumName##.mFlags |= flags;\
meta_##parent##_##child##.mpEnumDescriptions = &meta_##parent##_##child##_##enumName##;

#define NEXTENUM2(parent,child,enumNameStr, enumName, enumIndex, flags, previous) static MetaEnumDescription meta_##parent##_##child##_##enumName##;\
meta_##parent##_##child##_##enumName##.mpEnumName = enumNameStr;\
meta_##parent##_##child##_##enumName##.mEnumIntValue = enumIndex;\
meta_##parent##_##child##_##enumName##.mFlags |= flags;\
meta_##parent##_##child##_##previous.mpNext = &meta_##parent##_##child##_##enumName##;

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

#define SERIALIZER(name,_Ty) \
METAOP_CUSTOM(name, eMetaOpSerializeAsync, _Ty##::MetaOperation_SerializeAsync);\
meta_##name##.InstallSpecializedMetaOperation(&meta_##name##_eMetaOpSerializeAsync)

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

#define DEFINEANMVALUEI(_name, _Ty) DEFINET2(anmi_##_name##, AnimatedValueInterface<_Ty>);\
DEFINEM(anmi_##_name##, base);\
meta_anmi_##_name##_base.mpName = "Baseclass_AnimationValueInterfaceBase";\
meta_anmi_##_name##_base.mpMemberDesc = &meta_anminterface;\
meta_anmi_##_name##_base.mFlags |= 0x10;\
meta_anmi_##_name##_base.mOffset = (i64)((AnimationValueInterfaceBase*)((AnimatedValueInterface<_Ty>*)NULL));\
meta_anmi_##_name##.mpFirstMember = &meta_anmi_##_name##_base;\
ADD(anmi_##_name##);

#define DEFINEKEYFRAMEDVALUE(_name,_Ty,_TyMetaVar) \
DEFINET2(kfv_##_name##_sample, KeyframedValue<_Ty>::Sample);\
FIRSTMEM2(kfv_##_name##_sample, mTime, KeyframedValue<_Ty>::Sample, float, 0);\
NEXTMEM2(kfv_##_name##_sample, mbInterpolateToNextKey, KeyframedValue<_Ty>::Sample, bool, 0, mTime);\
NEXTMEM2(kfv_##_name##_sample, mTangentMode, KeyframedValue<_Ty>::Sample, long, 0, mbInterpolateToNextKey);\
FIRSTENUM(kfv_##_name##_sample, mTangentMode, eTangentUnknown, 0, 0);\
NEXTENUM(kfv_##_name##_sample, mTangentMode, eTangentStepped, 1, 0, eTangentUnknown);\
NEXTENUM(kfv_##_name##_sample, mTangentMode, eTangentKnot, 2, 0, eTangentStepped);\
NEXTENUM(kfv_##_name##_sample, mTangentMode, eTangentSmooth, 3, 0, eTangentKnot);\
NEXTENUM(kfv_##_name##_sample, mTangentMode, eTangentFlat, 4, 0, eTangentSmooth);\
NEXTMEM2(kfv_##_name##_sample, mValue, KeyframedValue<_Ty>::Sample, _TyMetaVar, 0, mTangentMode);\
NEXTMEM2(kfv_##_name##_sample, mRecipTimeToNextSample, KeyframedValue<_Ty>::Sample, float, 0x2021, mValue);\
ADD(kfv_##_name##_sample);\
DEFINEDCARRAY2(KeyframedValue<_Ty>::Sample, kfv_##_name##_sample);\
DEFINEANMVALUEI(_name, _Ty);\
DEFINET2(kfv_##_name##, KeyframedValue<_Ty>);\
DEFINEM(kfv_##_name##, animatedvalueinterface);\
meta_kfv_##_name##_animatedvalueinterface.mpName = "Baseclass_AnimatedValueInterface<T>";\
meta_kfv_##_name##_animatedvalueinterface.mpMemberDesc = &meta_anmi_##_name##;\
meta_kfv_##_name##_animatedvalueinterface.mFlags |= 0x10;\
meta_kfv_##_name##_animatedvalueinterface.mOffset = (i64)((AnimatedValueInterface<_Ty>*)((KeyframedValue<_Ty>*)NULL));\
DEFINEM(kfv_##_name##, keyframedvalueinterface);\
meta_kfv_##_name##_keyframedvalueinterface.mpName = "Baseclass_KeyframedValueInterface";\
meta_kfv_##_name##_keyframedvalueinterface.mpMemberDesc = &meta_keyframedvalueinterface;\
meta_kfv_##_name##_keyframedvalueinterface.mFlags |= 0x10;\
meta_kfv_##_name##_keyframedvalueinterface.mOffset = (i64)((KeyframedValueInterface*)((KeyframedValue<_Ty>*)NULL));\
meta_kfv_##_name##_animatedvalueinterface.mpNextMember = &meta_kfv_##_name##_keyframedvalueinterface;\
meta_kfv_##_name##.mpFirstMember = &meta_kfv_##_name##_animatedvalueinterface;\
NEXTMEM2(kfv_##_name##, mMinVal, KeyframedValue<_Ty>, _TyMetaVar, 0, animatedvalueinterface);\
NEXTMEM2(kfv_##_name##, mMaxVal, KeyframedValue<_Ty>, _TyMetaVar, 0, mMinVal);\
NEXTMEM2(kfv_##_name##, mSamples, KeyframedValue<_Ty>, DCArray_kfv_##_name##_sample, 0, mMaxVal);\
ADD(kfv_##_name##);

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
			meta_flags_mFlags.mOffset = offsetof(Flags, mFlags);
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
			meta_ztest_mZTestType.mOffset = offsetof(ZTestFunction, mZTestType);
			meta_ztest.mpFirstMember = &meta_ztest_mZTestType;
			meta_ztest.Insert();
			DEFINET(uidowner, UID::Owner)
				meta_uidowner.Initialize(typeid(UID::Owner));
			DEFINEM(uidowner, miUniqueID);
			meta_uidowner_miUniqueID.mpName = "miUniqueID";
			meta_uidowner_miUniqueID.mpMemberDesc = &meta_long;
			meta_uidowner_miUniqueID.mOffset = offsetof(UID::Owner, miUniqueID);
			meta_uidowner.mFlags |= MetaFlag_EditorHide;
			meta_uidowner.mpFirstMember = &meta_uidowner_miUniqueID;
			meta_uidowner.Insert();

			//UID::Generator

			DEFINET(uidgen, UID::Generator)
				meta_uidgen.Initialize(typeid(UID::Generator));
			DEFINEM(uidgen, miNextUniqueID);
			meta_uidgen_miNextUniqueID.mpName = "miNextUniqueID";
			meta_uidgen_miNextUniqueID.mpMemberDesc = &meta_long;
			meta_uidgen_miNextUniqueID.mOffset = offsetof(UID::Generator, miNextUniqueID);
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
			meta_t3vsdb_numverts.mOffset = offsetof(T3VertexSampleDataBase, mNumVerts);
			meta_t3vsdb_numverts.mpMemberDesc = &meta_long;

			DEFINEM(t3vsdb, vertsize);
			meta_t3vsdb_vertsize.mpName = "mVertSize";
			meta_t3vsdb_vertsize.mOffset = offsetof(T3VertexSampleDataBase, mVertSize);
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
			meta_rangeuint_max.mOffset = offsetof(TRange<unsigned int>, max);
			meta_rangeuint_max.mpMemberDesc = &meta___uint32;
			DEFINEM(rangeuint, min);
			meta_rangeuint_min.mpName = "min";
			meta_rangeuint_min.mOffset = offsetof(TRange<unsigned int>, min);
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
			meta_rangef_max.mOffset = offsetof(TRange<float>, max);
			meta_rangef_max.mpMemberDesc = &meta_float;
			DEFINEM(rangef, min);
			meta_rangef_min.mpName = "min";
			meta_rangef_min.mOffset = offsetof(TRange<float>, min);
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
			meta_transform_trans.mOffset = offsetof(Transform, mTrans);
			DEFINEM(transform, rot);
			meta_transform_rot.mpName = "mRot";
			meta_transform_rot.mpMemberDesc = &meta_quat;
			meta_transform_rot.mOffset = offsetof(Transform, mRot);
			meta_transform_rot.mpNextMember = &meta_transform_trans;
			meta_transform.mpFirstMember = &meta_transform_rot;
			meta_transform.Insert();

			DEFINET(rect, Rect);
			meta_rect.Initialize(typeid(Rect));
			meta_rect.mFlags |= (int)MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			DEFINEM(rect, bottom);
			meta_rect_bottom.mpName = "bottom";
			meta_rect_bottom.mpMemberDesc = &meta_long;
			meta_rect_bottom.mOffset = offsetof(Rect, bottom);
			DEFINEM(rect, top);
			meta_rect_top.mpName = "top";
			meta_rect_top.mpMemberDesc = &meta_long;
			meta_rect_top.mOffset = offsetof(Rect, top);
			meta_rect_top.mpNextMember = &meta_rect_bottom;
			DEFINEM(rect, right);
			meta_rect_right.mpName = "right";
			meta_rect_right.mpMemberDesc = &meta_long;
			meta_rect_right.mOffset = offsetof(Rect, right);
			meta_rect_right.mpNextMember = &meta_rect_top;
			DEFINEM(rect, left);
			meta_rect_left.mpName = "left";
			meta_rect_left.mpMemberDesc = &meta_long;
			meta_rect_left.mOffset = offsetof(Rect, left);
			meta_rect.mpFirstMember = &meta_rect_left;
			meta_rect_left.mpNextMember = &meta_rect_right;
			meta_rect.Insert();

			DEFINET(sphere, Sphere);
			meta_sphere.Initialize(typeid(Sphere));
			DEFINEM(sphere, radius);
			meta_sphere_radius.mOffset = offsetof(Sphere, mRadius);
			meta_sphere_radius.mpName = "mRadius";
			meta_sphere_radius.mpMemberDesc = &meta_float;
			DEFINEM(sphere, center);
			meta_sphere_center.mOffset = offsetof(Sphere, mCenter);
			meta_sphere_center.mpName = "mCenter";
			meta_sphere_center.mpMemberDesc = &meta_vec3;
			meta_sphere_center.mpNextMember = &meta_sphere_radius;
			meta_sphere.mpFirstMember = &meta_sphere_center;
			meta_sphere.Insert();

			DEFINET(color, Color);
			meta_color.Initialize(typeid(Color));
			DEFINEM(color, alpha);
			meta_color_alpha.mpName = "a";
			meta_color_alpha.mOffset = offsetof(Color, a);
			meta_color_alpha.mpMemberDesc = &meta_float;
			DEFINEM(color, b);
			meta_color_b.mpName = "b";
			meta_color_b.mOffset = offsetof(Color, b);
			meta_color_b.mpMemberDesc = &meta_float;
			meta_color_b.mpNextMember = &meta_color_alpha;
			DEFINEM(color, g);
			meta_color_g.mpName = "g";
			meta_color_g.mOffset = offsetof(Color, g);
			meta_color_g.mpMemberDesc = &meta_float;
			meta_color_g.mpNextMember = &meta_color_b;
			DEFINEM(color, r);
			meta_color_r.mpName = "r";
			meta_color_r.mOffset = offsetof(Color, r);
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
			meta_propkeyinfo_value.mOffset = offsetof(PropertySet::KeyInfo, mValue);
			meta_propkeyinfo_value.mFlags |= (int)MetaFlag::MetaFlag_EditorHide | (int)MetaFlag::MetaFlag_NoPanelCaption;
			meta_propkeyinfo_value.mpMemberDesc = &meta_propvalue;
			DEFINEM(propkeyinfo, flags);
			meta_propkeyinfo_flags.mFlags |= (int)MetaFlag::MetaFlag_MetaSerializeDisable;
			meta_propkeyinfo_flags.mpName = "mFlags";
			meta_propkeyinfo_flags.mpMemberDesc = &meta_flags;
			meta_propkeyinfo_flags.mpNextMember = &meta_propkeyinfo_value;
			meta_propkeyinfo_flags.mOffset = offsetof(PropertySet::KeyInfo, mFlags);
			DEFINEM(propkeyinfo, keyname);
			meta_propkeyinfo_keyname.mFlags |= (int)MetaFlag::MetaFlag_EditorHide;
			meta_propkeyinfo_keyname.mpName = "mKeyName";
			meta_propkeyinfo_keyname.mOffset = offsetof(PropertySet::KeyInfo, mKeyName);
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
			DEFINESARRAY_(TRange<float>, rangefloat, 3);
			DEFINEDCARRAY(i32);
			DEFINEDCARRAY(u16);
			DEFINEDCARRAY(u64);
			DEFINEDCARRAY(u32);
			DEFINEDCARRAY(u8);
			DEFINEDCARRAY(float);
			DEFINEDCARRAY(Symbol);
			DEFINEDCARRAY(bool);
			DEFINEDCARRAY(String);
			if (meta_DCArray_String.mbNameIsHeapAllocated) {
				free((void*)meta_DCArray_String.mpTypeInfoName);
				meta_DCArray_String.mbNameIsHeapAllocated = false;
			}
			meta_DCArray_String.mpTypeInfoName = "DCArray<String>";//string is std::basic_string<...> etc
			meta_DCArray_String.mHash = CRC64_CaseInsensitive(0, meta_DCArray_String.mpTypeInfoName);
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
			DEFINEMAP(Symbol, int, Symbol::CompareCRC);
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
			DEFINESET_(PropertySet::KeyInfo, keyinfo);
			DEFINET(prop, PropertySet);
			meta_prop.Initialize(typeid(PropertySet));
			meta_prop.mpExt = "prop";
			METAOP_CUSTOM(prop, eMetaOpSerializeAsync, PropertySet::MetaOperation_SerializeAsync);
			meta_prop.InstallSpecializedMetaOperation(&meta_prop_eMetaOpSerializeAsync);
			DEFINEM(prop, version);
			meta_prop_version.mpName = "mPropVersion";
			meta_prop_version.mOffset = offsetof(PropertySet, mPropVersion);
			meta_prop_version.mpMemberDesc = &meta_long;
			meta_prop.mpFirstMember = &meta_prop_version;
			DEFINEM(prop, flags);
			meta_prop_flags.mpName = "mPropertyFlags";
			meta_prop_flags.mpMemberDesc = &meta_flags;
			meta_prop_flags.mOffset = offsetof(PropertySet, mPropertyFlags);
			meta_prop_version.mpNextMember = &meta_prop_flags;
			DEFINEM(prop, keymap);
			meta_prop_keymap.mpName = "mKeyMap";
			meta_prop_keymap.mpMemberDesc = &meta_Set_keyinfo;
			meta_prop_keymap.mFlags |= (int)MetaFlag::MetaFlag_MetaSerializeDisable;
			meta_prop_keymap.mOffset = offsetof(PropertySet, mKeyMap);
			meta_prop_flags.mpNextMember = &meta_prop_keymap;
			meta_prop.Insert();

			DEFINET(bc, BoneContraints);
			meta_bc.Initialize(typeid(BoneContraints));
			DEFINEM(bc, btype);
			meta_bc_btype.mOffset = offsetof(BoneContraints, mBoneType);
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
			meta_bc_axis.mOffset = offsetof(BoneContraints, mHingeAxis);
			meta_bc_axis.mFlags |= MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_bc_axis.mpMemberDesc = &meta_vec3;
			DEFINEM(bc, range);
			meta_bc_axis.mpNextMember = &meta_bc_range;
			meta_bc_range.mpName = "mAxisRange";
			meta_bc_range.mOffset = offsetof(BoneContraints, mAxisRange);
			meta_bc_range.mpMemberDesc = &meta_sarray_rangefloat_3;
			meta_bc.mpFirstMember = &meta_bc_btype;
			meta_bc.Insert();

			DEFINET(sklentry, Skeleton::Entry);
			meta_sklentry.Initialize(typeid(Skeleton::Entry));
			DEFINEM(sklentry, flags);
			meta_sklentry_flags.mpName = "mFlags";
			meta_sklentry_flags.mpMemberDesc = &meta_flags;
			meta_sklentry_flags.mOffset = offsetof(Skeleton::Entry, mFlags);
			DEFINEM(sklentry, constraints);
			meta_sklentry_constraints.mpName = "mConstraints";
			meta_sklentry_constraints.mpMemberDesc = &meta_bc;
			meta_sklentry_constraints.mOffset = offsetof(Skeleton::Entry, mConstraints);
			meta_sklentry_constraints.mpNextMember = &meta_sklentry_flags;
			DEFINEM(sklentry, rgm);
			meta_sklentry_rgm.mpName = "mResourceGroupMembership";
			meta_sklentry_rgm.mOffset = offsetof(Skeleton::Entry, mResourceGroupMembership);
			meta_sklentry_rgm.mpNextMember = &meta_sklentry_constraints;
			meta_sklentry_rgm.mpMemberDesc = &meta_Map_Symbol_float;
			DEFINEM(sklentry, ats);
			meta_sklentry_ats.mpName = "mAnimTranslationScale";
			meta_sklentry_ats.mOffset = offsetof(Skeleton::Entry, mAnimTranslationScale);
			meta_sklentry_ats.mpNextMember = &meta_sklentry_rgm;
			meta_sklentry_ats.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, lts);
			meta_sklentry_lts.mpName = "mLocalTranslationScale";
			meta_sklentry_lts.mOffset = offsetof(Skeleton::Entry, mLocalTranslationScale);
			meta_sklentry_lts.mpNextMember = &meta_sklentry_ats;
			meta_sklentry_lts.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, gts);
			meta_sklentry_gts.mpName = "mGlobalTranslationScale";
			meta_sklentry_gts.mOffset = offsetof(Skeleton::Entry, mGlobalTranslationScale);
			meta_sklentry_gts.mpNextMember = &meta_sklentry_lts;
			meta_sklentry_gts.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, restx);
			meta_sklentry_restx.mpName = "mRestXForm";
			meta_sklentry_restx.mOffset = offsetof(Skeleton::Entry, mRestXform);
			meta_sklentry_restx.mpNextMember = &meta_sklentry_gts;
			meta_sklentry_restx.mpMemberDesc = &meta_transform;
			DEFINEM(sklentry, localq);
			meta_sklentry_localq.mpName = "mLocalQuat";
			meta_sklentry_localq.mOffset = offsetof(Skeleton::Entry, mLocalQuat);
			meta_sklentry_localq.mpNextMember = &meta_sklentry_restx;
			meta_sklentry_localq.mpMemberDesc = &meta_quat;
			DEFINEM(sklentry, localp);
			meta_sklentry_localp.mpName = "mLocalPos";
			meta_sklentry_localp.mOffset = offsetof(Skeleton::Entry, mLocalPos);
			meta_sklentry_localp.mpNextMember = &meta_sklentry_localq;
			meta_sklentry_localp.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, blen);
			meta_sklentry_blen.mpName = "mBoneLength";
			meta_sklentry_blen.mOffset = offsetof(Skeleton::Entry, mBoneLength);
			meta_sklentry_blen.mpNextMember = &meta_sklentry_localp;
			meta_sklentry_blen.mpMemberDesc = &meta_float;
			meta_sklentry_blen.mFlags |= MetaFlag::MetaFlag_MetaSerializeDisable;
			DEFINEM(sklentry, mblen);
			meta_sklentry_mblen.mpName = "mMirrorBoneIndex";
			meta_sklentry_mblen.mOffset = offsetof(Skeleton::Entry, mMirrorBoneIndex);
			meta_sklentry_mblen.mpNextMember = &meta_sklentry_blen;
			meta_sklentry_mblen.mpMemberDesc = &meta_long;
			DEFINEM(sklentry, mbn);
			meta_sklentry_mbn.mpName = "mMirrorBoneName";
			meta_sklentry_mbn.mOffset = offsetof(Skeleton::Entry, mMirrorBoneName);
			meta_sklentry_mbn.mpNextMember = &meta_sklentry_mblen;
			meta_sklentry_mbn.mpMemberDesc = &meta_symbol;
			DEFINEM(sklentry, pi);
			meta_sklentry_pi.mpName = "mParentIndex";
			meta_sklentry_pi.mOffset = offsetof(Skeleton::Entry, mParentIndex);
			meta_sklentry_pi.mpNextMember = &meta_sklentry_mbn;
			meta_sklentry_pi.mpMemberDesc = &meta_long;
			DEFINEM(sklentry, pn);
			meta_sklentry_pn.mpName = "mParentName";
			meta_sklentry_pn.mOffset = offsetof(Skeleton::Entry, mParentName);
			meta_sklentry_pn.mpNextMember = &meta_sklentry_pi;
			meta_sklentry_pn.mpMemberDesc = &meta_symbol;
			DEFINEM(sklentry, jn);
			meta_sklentry_jn.mpName = "mJointName";
			meta_sklentry_jn.mOffset = offsetof(Skeleton::Entry, mJointName);
			meta_sklentry_jn.mpNextMember = &meta_sklentry_pn;
			meta_sklentry_jn.mpMemberDesc = &meta_symbol;
			meta_sklentry.mpFirstMember = &meta_sklentry_jn;
			meta_sklentry.Insert();

			DEFINEDCARRAY2(Skeleton::Entry, sklentry);

			DEFINET(skl, Skeleton);
			meta_skl.Initialize(typeid(Skeleton));
			meta_skl.mpExt = "skl";
			DEFINEM(skl, entries);
			meta_skl_entries.mpName = "mEntries";
			meta_skl_entries.mOffset = offsetof(Skeleton, mEntries);
			meta_skl_entries.mpMemberDesc = &meta_DCArray_sklentry;
			meta_skl.mpFirstMember = &meta_skl_entries;
			meta_skl.Insert();

			DEFINET(aam, ActorAgentMapper);
			meta_aam.Initialize(typeid(ActorAgentMapper));
			meta_aam.mpExt = "aam";
			DEFINEM(aam, aam);
			meta_aam_aam.mpName = "mActorAgentMap";
			meta_aam_aam.mpMemberDesc = &meta_prop;
			meta_aam_aam.mOffset = offsetof(ActorAgentMapper, mActorAgentMap);
			meta_aam.mpFirstMember = &meta_aam_aam;
			DEFINEM(aam, aa);
			meta_aam_aa.mpName = "mActionActors";
			meta_aam_aa.mpMemberDesc = &meta_Set_String;
			meta_aam_aa.mOffset = offsetof(ActorAgentMapper, mActionActors);
			meta_aam_aam.mpNextMember = &meta_aam_aa;
			meta_aam.Insert();

			DEFINET(amape, AgentMap::AgentMapEntry);
			meta_amape.Initialize(typeid(AgentMap::AgentMapEntry));
			DEFINEM(amape, name);
			meta_amape_name.mpName = "mzName";
			meta_amape_name.mOffset = offsetof(AgentMap::AgentMapEntry, mzName);
			meta_amape_name.mpMemberDesc = &meta_string;

			DEFINEM(amape, Actor);
			meta_amape_Actor.mpName = "mzActor";
			meta_amape_Actor.mOffset = offsetof(AgentMap::AgentMapEntry, mzActor);
			meta_amape_Actor.mpMemberDesc = &meta_string;
			meta_amape_name.mpNextMember = &meta_amape_Actor;

			DEFINEM(amape, name2);
			meta_amape_name2.mpName = "mazModels";
			meta_amape_name2.mOffset = offsetof(AgentMap::AgentMapEntry, mazModels);
			meta_amape_name2.mpMemberDesc = &meta_Set_String;
			meta_amape_Actor.mpNextMember = &meta_amape_name2;

			DEFINEM(amape, name3);
			meta_amape_name3.mpName = "mazGuides";
			meta_amape_name3.mOffset = offsetof(AgentMap::AgentMapEntry, mazGuides);
			meta_amape_name3.mpMemberDesc = &meta_Set_String;
			meta_amape_name2.mpNextMember = &meta_amape_name3;

			DEFINEM(amape, name4);
			meta_amape_name4.mpName = "mazStyleIdles";
			meta_amape_name4.mOffset = offsetof(AgentMap::AgentMapEntry, mazStyleIdles);
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
			meta_amap_agents.mOffset = offsetof(AgentMap, maAgents);
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
			meta_acol_inc.mOffset = offsetof(AssetCollection, mIncludeMasks);
			meta_acol_exl.mOffset = offsetof(AssetCollection, mExcludeMasks);
			meta_acol_pre.mOffset = offsetof(AssetCollection, mPreFilter);
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
			meta_bus_mbus.mOffset = offsetof(SoundBusSystem::BusHolder, masterBus);
			meta_bus_mbus.mpMemberDesc = &meta_busd;
			DEFINEM(bus, am);
			meta_bus_am.mpName = "assetMap";
			meta_bus_mbus.mpNextMember = &meta_bus_am;
			meta_bus_am.mOffset = offsetof(SoundBusSystem::BusHolder, assetMap);
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
			meta_imap.InstallSpecializedMetaOperation(&meta_imap_eMetaOpSerializeAsync);
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
			meta_scene.InstallSpecializedMetaOperation(&meta_scene_eMetaOpSerializeAsync);
			FIRSTMEM2(scene, mTimeScale, Scene, float, 1);
			NEXTMEM2(scene, mbActive, Scene, bool, 1, mTimeScale);
			NEXTMEM2(scene, mbHidden, Scene, bool, 0, mbActive);
			NEXTMEM2(scene, mName, Scene, string, 0, mbHidden);
			NEXTMEM2(scene, mAgentList, Scene, List_agentinfoptr, 1, mName);
			NEXTMEM2(scene, mReferencedScenes, Scene, DCArray_hlscene, 0, mAgentList);
			EXT(scene, scene);
			ADD(scene);

			DEFINET2(dlgprops1, DlgObjectPropsMap);
			SERIALIZER(dlgprops1, DlgObjectPropsMap);
			ADD(dlgprops1);

			DEFINET2(dlgdef, DlgObjectPropsMap::GroupDefinition);
			SERIALIZER(dlgdef, DlgObjectPropsMap::GroupDefinition);
			ADDFLAGS(dlgdef, MetaFlag::MetaFlag_NoPanelCaption);
			DEFINEM(dlgdef, uidowner);
			meta_dlgdef_uidowner.mpName = "Baseclass_UID::Owner";
			meta_dlgdef_uidowner.mpMemberDesc = &meta_uidowner;
			meta_dlgdef.mpFirstMember = &meta_dlgdef_uidowner;
			meta_dlgdef_uidowner.mOffset = 0;
			NEXTMEM2(dlgdef, mGroupCat, DlgObjectPropsMap::GroupDefinition, long, MetaFlag::MetaFlag_EditorHide, uidowner);
			NEXTMEM2(dlgdef, mVer, DlgObjectPropsMap::GroupDefinition, long, MetaFlag::MetaFlag_EditorHide, mGroupCat);
			NEXTMEM2(dlgdef, mhProps, DlgObjectPropsMap::GroupDefinition, Handlepropset, MetaFlag::MetaFlag_EditorHide, mVer);

			ADD(dlgdef);

			DEFINET2(dss, DlgSystemSettings);
			EXT(dss, dss);
			FIRSTMEM2(dss, mPropsMapUser, DlgSystemSettings, dlgprops1, 0);
			NEXTMEM2(dss, mPropsMapProduction, DlgSystemSettings, dlgprops1, 0, mPropsMapUser);
			ADD(dss);

			DEFINET2(bb, BinaryBuffer);
			FIRSTMEM2(bb, mDataSize, BinaryBuffer, long, 0);
			SERIALIZER(bb, BinaryBuffer);
			ADD(bb);

			DEFINET2(enlsig, EnlightenSignature);
			FIRSTMEM2(enlsig, mSignature, EnlightenSignature, __int64, 0);
			NEXTMEM2(enlsig, mMagicNumber, EnlightenSignature, long, 0, mSignature);
			NEXTMEM2(enlsig, mVersion, EnlightenSignature, long, 0, mMagicNumber);
			ADD(enlsig);

			DEFINET2(enlprobe, EnlightenProbeData);
			FIRSTMEM2(enlprobe, mEnvTileName, EnlightenProbeData, symbol, 0);
			NEXTMEM2(enlprobe, mRadProbeSetCore, EnlightenProbeData, bb, 0, mEnvTileName);
			ADD(enlprobe);

			DEFINET2(enlsys, EnlightenSystemData);
			FIRSTMEM2(enlsys, mName, EnlightenSystemData, symbol, 0);
			NEXTMEM2(enlsys, mEnvTileName, EnlightenSystemData, symbol, 0, mName);
			NEXTMEM2(enlsys, mRadSystemCore, EnlightenSystemData, bb, 0, mEnvTileName);
			NEXTMEM2(enlsys, mInputWorkspace, EnlightenSystemData, bb, 0, mRadSystemCore);
			NEXTMEM2(enlsys, mClusterAlbedoWorkspaceMaterial, EnlightenSystemData, bb, 0, mInputWorkspace);
			NEXTMEM2(enlsys, mPrecomputedVisibility, EnlightenSystemData, bb, 0, mClusterAlbedoWorkspaceMaterial);
			ADD(enlsys);

			DEFINEDCARRAY(EnlightenSystemData);
			DEFINEDCARRAY(EnlightenProbeData);

			DEFINET2(enl, EnlightenData);
			EXT(enl, enl);
			FIRSTMEM2(enl, mSignature, EnlightenData, enlsig, 0);
			NEXTMEM2(enl, mName, EnlightenData, string, 0, mSignature);
			NEXTMEM2(enl, mSystemData, EnlightenData, DCArray_EnlightenSystemData, 0, mName);
			NEXTMEM2(enl, mProbeData, EnlightenData, DCArray_EnlightenProbeData, 0, mSystemData);
			//no need for overriden metaops since they just call meta::x
			ADD(enl);

			DEFINET2(reverb, SoundReverbDefinition);
			EXT(reverb, reverb);
			FIRSTMEM2(reverb, mbEnabled, SoundReverbDefinition, bool, 0);
			NEXTMEM2(reverb, mfRoomEffectLevel, SoundReverbDefinition, float, 0, mbEnabled);
			NEXTMEM2(reverb, mfRoomEffectLevelHighFrequency, SoundReverbDefinition, float, 0, mfRoomEffectLevel);
			NEXTMEM2(reverb, mfRoomEffectLevelLowFrequency, SoundReverbDefinition, float, 0, mfRoomEffectLevelHighFrequency);
			NEXTMEM2(reverb, mfDecayTime, SoundReverbDefinition, float, 0, mfRoomEffectLevelLowFrequency);
			NEXTMEM2(reverb, mfDecayHighFrequencyRatio, SoundReverbDefinition, float, 0, mfDecayTime);
			NEXTMEM2(reverb, mfReflections, SoundReverbDefinition, float, 0, mfDecayHighFrequencyRatio);
			NEXTMEM2(reverb, mfReflectionsDelay, SoundReverbDefinition, float, 0, mfReflections);
			NEXTMEM2(reverb, mfReverb, SoundReverbDefinition, float, 0, mfReflectionsDelay);
			NEXTMEM2(reverb, mfReverbDelay, SoundReverbDefinition, float, 0, mfReverb);
			NEXTMEM2(reverb, mfHighFrequencyReference, SoundReverbDefinition, float, 0, mfReverbDelay);
			NEXTMEM2(reverb, mfLowFrequencyReference, SoundReverbDefinition, float, 0, mfHighFrequencyReference);
			NEXTMEM2(reverb, mfDiffusion, SoundReverbDefinition, float, 0, mfLowFrequencyReference);
			NEXTMEM2(reverb, mfDensity, SoundReverbDefinition, float, 0, mfDiffusion);
			ADD(reverb);

			DEFINET2(anminterface, AnimationValueInterfaceBase);
			FIRSTMEM2(anminterface, mName, AnimationValueInterfaceBase, symbol, 0);
			NEXTMEM2(anminterface, mFlags, AnimationValueInterfaceBase, long, 0, mName);
			ADD(anminterface);

			DEFINET2(keyframedvalueinterface, KeyframedValueInterface);
			ADDFLAGS(keyframedvalueinterface, MetaFlag::MetaFlag_MetaSerializeDisable | MetaFlag::MetaFlag_SkipObjectState);
			ADD(keyframedvalueinterface);

			DEFINET2(sampler, T3SamplerStateBlock);
			FIRSTMEM2(sampler, mData, T3SamplerStateBlock, long, 0);
			ADD(sampler);

			DEFINET2(pt, EnumPlatformType);
			FIRSTMEM2(pt, mVal, EnumPlatformType, long, 0);
			ADDFLAGS(pt, MetaFlag::MetaFlag_NoPanelCaption | MetaFlag::MetaFlag_EnumWrapperClass);
			FIRSTENUM(pt, mVal, ePlatform_None, 0, 0);
			NEXTENUM(pt, mVal, ePlatform_All, 1, 0, ePlatform_None);
			NEXTENUM(pt, mVal, ePlatform_PC, 2, 0, ePlatform_All);
			NEXTENUM(pt, mVal, ePlatform_Wii, 3, 0, ePlatform_PC);
			NEXTENUM(pt, mVal, ePlatform_Xbox, 4, 0, ePlatform_Wii);
			NEXTENUM(pt, mVal, ePlatform_PS3, 5, 0, ePlatform_Xbox);
			NEXTENUM(pt, mVal, ePlatform_Mac, 6, 0, ePlatform_PS3);
			NEXTENUM(pt, mVal, ePlatform_iPhone, 7, 0, ePlatform_Mac);
			NEXTENUM(pt, mVal, ePlatform_Android, 8, 0, ePlatform_iPhone);
			NEXTENUM(pt, mVal, ePlatform_Vita, 9, 0, ePlatform_Android);
			NEXTENUM(pt, mVal, ePlatform_Linux, 10, 0, ePlatform_Vita);
			NEXTENUM(pt, mVal, ePlatform_PS4, 11, 0, ePlatform_Linux);
			NEXTENUM(pt, mVal, ePlatform_XBOne, 12, 0, ePlatform_PS4);
			NEXTENUM(pt, mVal, ePlatform_WiiU, 13, 0, ePlatform_XBOne);
			NEXTENUM(pt, mVal, ePlatform_Win10, 14, 0, ePlatform_WiiU);
			NEXTENUM(pt, mVal, ePlatform_NX, 15, 0, ePlatform_Win10);
			ADDFLAGS(pt_mVal, MetaFlag::MetaFlag_EnumIntType);
			ADD(pt);

			DEFINET2(tp, ToolProps);
			SERIALIZER(tp, ToolProps);
			ADDFLAGS(tp, MetaFlag::MetaFlag_MetaSerializeNonBlockedVariableSize);
			FIRSTMEM2(tp, mbHasProps, ToolProps, bool, 0);
			ADD(tp);

			DEFINET2(t3gr, T3ToonGradientRegion);
			FIRSTMEM2(t3gr, mColor, T3ToonGradientRegion, color, 0);
			NEXTMEM2(t3gr, mSize, T3ToonGradientRegion, float, 0, mColor);
			NEXTMEM2(t3gr, mGradientSize, T3ToonGradientRegion, float, 0, mSize);
			ADD(t3gr);

			DEFINEDCARRAY(T3ToonGradientRegion);

			DEFINET2(swizzle, RenderSwizzleParams);
			SERIALIZER(swizzle, RenderSwizzleParams);
			DEFINEM(RenderSwizzleParams, s0);
			size_t start = offsetof(RenderSwizzleParams, mSwizzle);
			meta_RenderSwizzleParams_s0.mOffset = start + 0;
			meta_RenderSwizzleParams_s0.mpName = "mSwizzle[0]";
			meta_RenderSwizzleParams_s0.mpMemberDesc = &meta_char;
			meta_swizzle.mpFirstMember = &meta_RenderSwizzleParams_s0;
			DEFINEM(RenderSwizzleParams, s1);
			meta_RenderSwizzleParams_s1.mOffset = start + 1;
			meta_RenderSwizzleParams_s1.mpName = "mSwizzle[1]";
			meta_RenderSwizzleParams_s1.mpMemberDesc = &meta_char;
			meta_RenderSwizzleParams_s0.mpNextMember = &meta_RenderSwizzleParams_s1;
			DEFINEM(RenderSwizzleParams, s2);
			meta_RenderSwizzleParams_s2.mOffset = start + 2;
			meta_RenderSwizzleParams_s2.mpName = "mSwizzle[2]";
			meta_RenderSwizzleParams_s2.mpMemberDesc = &meta_char;
			meta_RenderSwizzleParams_s1.mpNextMember = &meta_RenderSwizzleParams_s2;
			DEFINEM(RenderSwizzleParams, s3);
			meta_RenderSwizzleParams_s3.mOffset = start + 3;
			meta_RenderSwizzleParams_s3.mpName = "mSwizzle[3]";
			meta_RenderSwizzleParams_s3.mpMemberDesc = &meta_char;
			meta_RenderSwizzleParams_s2.mpNextMember = &meta_RenderSwizzleParams_s3;
			ADD(swizzle);

			DEFINET2(tex, T3Texture);
			EXT(tex, d3dtx);
			FIRSTMEM2(tex, mVersion, T3Texture, long, MetaFlag::MetaFlag_EditorHide);
			NEXTMEM2(tex, mSamplerState, T3Texture, sampler, 0, mVersion);
			NEXTMEM2(tex, mPlatform, T3Texture, pt, 0, mSamplerState);
			NEXTMEM2(tex, mName, T3Texture, string, 0, mPlatform);
			NEXTMEM2(tex, mImportName, T3Texture, string, 0, mName);
			NEXTMEM2(tex, mImportScale, T3Texture, float, 0, mImportName);
			NEXTMEM4(tex, mImportSpecularPower, T3Texture, float, 0, mImportScale, -1, TelltaleToolLib_GetGameKeyIndex("MCSM"));
			NEXTMEM2(tex, mToolProps, T3Texture, tp, MetaFlag::MetaFlag_EditorHide, mImportSpecularPower);
			NEXTMEM2(tex, mNumMipLevels, T3Texture, long, 0, mToolProps);
			NEXTMEM2(tex, mWidth, T3Texture, long, 0, mNumMipLevels);
			NEXTMEM2(tex, mHeight, T3Texture, long, 0, mWidth);
			NEXTMEM4(tex, mDepth, T3Texture, long, 0, mHeight, TelltaleToolLib_GetGameKeyIndex("BATMAN"), -1);
			NEXTMEM4(tex, mArraySize, T3Texture, long, 0, mDepth, TelltaleToolLib_GetGameKeyIndex("BATMAN"), -1);
			NEXTMEM2(tex, mSurfaceFormat, T3Texture, long, 0, mArraySize);
			FIRSTENUM(tex, mSurfaceFormat, eSurface_Unknown, -1, 0);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB8, 0, 0, eSurface_Unknown);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB16, 0x1, 0, eSurface_ARGB8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGB565, 0x2, 0, eSurface_ARGB16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB1555, 0x3, 0, eSurface_RGB565);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB4, 0x4, 0, eSurface_ARGB1555);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB2101010, 0x5, 0, eSurface_ARGB4);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R16, 0x6, 0, eSurface_ARGB2101010);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG16, 0x7, 0, eSurface_R16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA16, 0x8, 0, eSurface_RG16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG8, 0x9, 0, eSurface_RGBA16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA8, 0xA, 0, eSurface_RG8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R32, 0xB, 0, eSurface_RGBA8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG32, 0xC, 0, eSurface_R32);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA32, 0xD, 0, eSurface_RG32);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA8S, 0xF, 0, eSurface_RGBA32);
			NEXTENUM(tex, mSurfaceFormat, eSurface_A8, 0x10, 0, eSurface_RGBA8S);
			NEXTENUM(tex, mSurfaceFormat, eSurface_L8, 0x11, 0, eSurface_A8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_AL8, 0x12, 0, eSurface_L8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_L16, 0x13, 0, eSurface_AL8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG16S, 0x14, 0, eSurface_L16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA16S, 0x15, 0, eSurface_RG16S);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R16UI, 0x16, 0, eSurface_RGBA16S);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG16UI, 0x17, 0, eSurface_R16UI);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R16F, 0x20, 0, eSurface_RG16UI);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG16F, 0x21, 0, eSurface_R16F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA16F, 0x22, 0, eSurface_RG16F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R32F, 0x23, 0, eSurface_RGBA16F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG32F, 0x24, 0, eSurface_R32F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA32F, 0x25, 0, eSurface_RG32F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA1010102F, 0x26, 0, eSurface_RGBA32F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGB111110F, 0x27, 0, eSurface_RGBA1010102F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGB9E5F, 0x28, 0, eSurface_RGB111110F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DepthPCF16, 0x30, 0, eSurface_RGB9E5F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DepthPCF24, 0x31, 0, eSurface_DepthPCF16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth16, 0x32, 0, eSurface_DepthPCF24);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth24, 0x33, 0, eSurface_Depth16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DepthStencil32, 0x34, 0, eSurface_Depth24);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth32F, 0x35, 0, eSurface_DepthStencil32);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth32F_Stencil8, 0x36, 0, eSurface_Depth32F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth24F_Stencil8, 0x37, 0, eSurface_Depth32F_Stencil8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXT1, 64, 0, eSurface_Depth24F_Stencil8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXT3, 65, 0, eSurface_DXT1);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXT5, 66, 0, eSurface_DXT3);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXT5A, 67, 0, eSurface_DXT5);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXTN, 68, 0, eSurface_DXT5A);
			NEXTENUM(tex, mSurfaceFormat, eSurface_CTX1, 69, 0, eSurface_DXTN);
			NEXTENUM(tex, mSurfaceFormat, eSurface_BC6, 70, 0, eSurface_CTX1);
			NEXTENUM(tex, mSurfaceFormat, eSurface_BC7, 71, 0, eSurface_BC6);
			NEXTENUM(tex, mSurfaceFormat, eSurface_PVRTC2, 80, 0, eSurface_BC7);
			NEXTENUM(tex, mSurfaceFormat, eSurface_PVRTC4, 81, 0, eSurface_PVRTC2);
			NEXTENUM(tex, mSurfaceFormat, eSurface_PVRTC2a, 82, 0, eSurface_PVRTC4);
			NEXTENUM(tex, mSurfaceFormat, eSurface_PVRTC4a, 83, 0, eSurface_PVRTC2a);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ATC_RGB, 96, 0, eSurface_PVRTC4a);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ATC_RGB1A, 97, 0, eSurface_ATC_RGB);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ATC_RGBA, 98, 0, eSurface_ATC_RGB1A);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC1_RGB, 112, 0, eSurface_ATC_RGBA);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_RGB, 113, 0, eSurface_ETC1_RGB);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_RGB1A, 114, 0, eSurface_ETC2_RGB);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_RGBA, 115, 0, eSurface_ETC2_RGB1A);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_R, 116, 0, eSurface_ETC2_RGBA);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_RG, 117, 0, eSurface_ETC2_R);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ATSC_RGBA_4x4, 128, 0, eSurface_ETC2_RG);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R8, 14, 0, eSurface_ATSC_RGBA_4x4);
			ADDFLAGS(tex_mSurfaceFormat, MetaFlag::MetaFlag_EnumIntType);
			NEXTMEM2(tex, mTextureLayout, T3Texture, long, 0, mSurfaceFormat);
			ADDFLAGS(tex_mTextureLayout, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mTextureLayout, eTextureLayout_Unknown, -1, 0);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_2D, 0, 0, eTextureLayout_Unknown);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_Cube, 1, 0, eTextureLayout_2D);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_3D, 2, 0, eTextureLayout_Cube);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_2DArray, 3, 0, eTextureLayout_3D);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_CubeArray, 4, 0, eTextureLayout_2DArray);
			NEXTMEM2(tex, mSurfaceGamma, T3Texture, long, 0, mTextureLayout);
			ADDFLAGS(tex_mSurfaceGamma, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mSurfaceGamma, eSurfaceGamma_Unknown, -1, 0);
			NEXTENUM(tex, mSurfaceGamma, eSurfaceGamma_Linear, 0, 0, eSurfaceGamma_Unknown);
			NEXTENUM(tex, mSurfaceGamma, eSurfaceGamma_sRGB, 1, 0, eSurfaceGamma_Linear);
			NEXTMEM4(tex, mSurfaceMultisample, T3Texture, long, 0, mSurfaceGamma, TelltaleToolLib_GetGameKeyIndex("BATMAN"), -1);
			ADDFLAGS(tex_mSurfaceMultisample, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_None, 0, 0);
			NEXTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_2x, 1, 0, eSurfaceMultisample_None);
			NEXTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_4x, 2, 0, eSurfaceMultisample_None);
			NEXTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_8x, 3, 0, eSurfaceMultisample_None);
			NEXTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_16x, 4, 0, eSurfaceMultisample_None);
			NEXTMEM2(tex, mResourceUsage, T3Texture, long, 0, mSurfaceMultisample);
			ADDFLAGS(tex_mResourceUsage, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mResourceUsage, eResourceUsage_Static, 0, 0);
			NEXTENUM(tex, mResourceUsage, eResourceUsage_Dynamic, 1, 0, eResourceUsage_Static);
			NEXTENUM(tex, mResourceUsage, eResourceUsage_System, 2, 0, eResourceUsage_Dynamic);
			NEXTMEM2(tex, mType, T3Texture, long, 0, mResourceUsage);
			ADDFLAGS(tex_mType, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mType, eTxUnknown, 0, 0);
			NEXTENUM(tex, mType, eTxLightmap_V0, 0x1, 0, eTxUnknown);
			NEXTENUM(tex, mType, eTxBumpmap, 0x2, 0, eTxLightmap_V0);
			NEXTENUM(tex, mType, eTxNormalMap, 0x3, 0, eTxBumpmap);
			NEXTENUM(tex, mType, eTxUNUSED1, 0x4, 0, eTxNormalMap);
			NEXTENUM(tex, mType, eTxUNUSED0, 0x5, 0, eTxUNUSED1);
			NEXTENUM(tex, mType, eTxSubsurfaceScatteringMap_V0, 0x6, 0, eTxUNUSED0);
			NEXTENUM(tex, mType, eTxSubsurfaceScatteringMap, 0x7, 0, eTxSubsurfaceScatteringMap_V0);
			NEXTENUM(tex, mType, eTxDetailMap, 0x8, 0, eTxSubsurfaceScatteringMap);
			NEXTENUM(tex, mType, eTxStaticShadowMap, 0x9, 0, eTxDetailMap);
			NEXTENUM(tex, mType, eTxLightmapHDR, 0xA, 0, eTxStaticShadowMap);
			NEXTENUM(tex, mType, eTxSDFDetailMap, 0xB, 0, eTxLightmapHDR);
			NEXTENUM(tex, mType, eTxEnvMap, 0xC, 0, eTxSDFDetailMap);
			NEXTENUM(tex, mType, eTxSpecularColor, 0xD, 0, eTxEnvMap);
			NEXTENUM(tex, mType, eTxToonLookup, 0xE, 0, eTxSpecularColor);
			NEXTENUM(tex, mType, eTxStandard, 0xF, 0, eTxToonLookup);
			NEXTENUM(tex, mType, eTxOutlineDiscontinuity, 0x10, 0, eTxStandard);
			NEXTENUM(tex, mType, eTxLightmapHDRScaled, 0x11, 0, eTxOutlineDiscontinuity);
			NEXTENUM(tex, mType, eTxEmissiveMap, 0x12, 0, eTxLightmapHDRScaled);
			NEXTENUM(tex, mType, eTxParticleProperties, 0x13, 0, eTxEmissiveMap);
			NEXTENUM(tex, mType, eTxBrushNormalMap, 0x14, 0, eTxParticleProperties);
			NEXTENUM(tex, mType, eTxUNUSED2, 0x15, 0, eTxBrushNormalMap);
			NEXTENUM(tex, mType, eTxNormalGlossMap, 0x16, 0, eTxUNUSED2);
			NEXTENUM(tex, mType, eTxLookup, 0x17, 0, eTxNormalGlossMap);
			NEXTENUM(tex, mType, eTxAmbientOcclusion, 0x18, 0, eTxLookup);
			NEXTENUM(tex, mType, eTxPrefilteredEnvCubeMapHDR, 0x19, 0, eTxAmbientOcclusion);
			NEXTENUM(tex, mType, eTxBrushLookupMap, 0x1A, 0, eTxPrefilteredEnvCubeMapHDR);
			NEXTENUM(tex, mType, eTxVector2Map, 0x1B, 0, eTxBrushLookupMap);
			NEXTENUM(tex, mType, eTxNormalDxDyMap, 0x1C, 0, eTxVector2Map);
			NEXTENUM(tex, mType, eTxPackedSDFDetailMap, 0x1D, 0, eTxNormalDxDyMap);
			NEXTENUM(tex, mType, eTxSingleChannelSDFDetailMap, 0x1E, 0, eTxPackedSDFDetailMap);
			NEXTENUM(tex, mType, eTxLightmapDirection, 0x1F, 0, eTxSingleChannelSDFDetailMap);
			NEXTENUM(tex, mType, eTxLightmapStaticShadows, 0x20, 0, eTxLightmapDirection);
			NEXTENUM(tex, mType, eTxLightStaticShadowMapAtlas, 0x21, 0, eTxLightmapStaticShadows);
			NEXTENUM(tex, mType, eTxLightStaticShadowMap, 0x22, 0, eTxLightStaticShadowMapAtlas);
			NEXTENUM(tex, mType, eTxPrefilteredEnvCubeMapHDRScaled, 0x23, 0, eTxLightStaticShadowMap);
			NEXTENUM(tex, mType, eTxLightStaticShadowVolume, 0x24, 0, eTxPrefilteredEnvCubeMapHDRScaled);
			NEXTENUM(tex, mType, eTxLightmapAtlas, 0x25, 0, eTxLightStaticShadowVolume);
			NEXTENUM(tex, mType, eTxNormalXYMap, 0x26, 0, eTxLightmapAtlas);
			NEXTENUM(tex, mType, eTxLightmapFlatAtlas, 0x27, 0, eTxNormalXYMap);
			NEXTENUM(tex, mType, eTxLookupXY, 0x28, 0, eTxLightmapFlatAtlas);
			NEXTENUM(tex, mType, eTxObjectNormalMap, 0x29, 0, eTxLookupXY);
			NEXTMEM2(tex, mSwizzle, T3Texture, swizzle, MetaFlag::MetaFlag_MetaSerializeBlockingDisabled, mType);
			NEXTMEM2(tex, mSpecularGlossExponent, T3Texture, float, 0, mSwizzle);
			NEXTMEM2(tex, mHDRLightmapScale, T3Texture, float, 0, mSpecularGlossExponent);
			NEXTMEM2(tex, mToonGradientCutoff, T3Texture, float, 0, mHDRLightmapScale);
			NEXTMEM2(tex, mAlphaMode, T3Texture, long, 0, mToonGradientCutoff);
			FIRSTENUM(tex, mAlphaMode, eTxAlphaUnkown, -1, 0);
			NEXTENUM(tex, mAlphaMode, eTxNoAlpha, 0, 0, eTxAlphaUnkown);
			NEXTENUM(tex, mAlphaMode, eTxAlphaTest, 1, 0, eTxNoAlpha);
			NEXTENUM(tex, mAlphaMode, eTxAlphaBlend, 2, 0, eTxAlphaTest);
			NEXTMEM2(tex, mColorMode, T3Texture, long, 0, mAlphaMode);
			FIRSTENUM(tex, mColorMode, eTxColorUnknown, -1, 0);
			NEXTENUM(tex, mColorMode, eTxColorFull, 0, 0, eTxColorUnknown);
			NEXTENUM(tex, mColorMode, eTxColorBlack, 1, 0, eTxColorFull);
			NEXTENUM(tex, mColorMode, eTxColorGrayscale, 2, 0, eTxColorBlack);
			NEXTENUM(tex, mColorMode, eTxColorGrayscaleAlpha, 3, 0, eTxColorGrayscale);
			NEXTMEM2(tex, mUVOffset, T3Texture, vec2, 0, mColorMode);
			NEXTMEM2(tex, mUVScale, T3Texture, vec2, 0, mUVOffset);
			NEXTMEM2(tex, mNumMipLevelsAllocated, T3Texture, long, MetaFlag::MetaFlag_MetaSerializeDisable, mUVScale);
			NEXTMEM2(tex, mNumSurfacesRequested, T3Texture, long, MetaFlag::MetaFlag_MetaSerializeDisable, mNumMipLevelsAllocated);
			NEXTMEM2(tex, mNumSurfacesRequired, T3Texture, long, MetaFlag::MetaFlag_MetaSerializeDisable, mNumSurfacesRequested);
			NEXTMEM2(tex, mNumSurfacesLoaded, T3Texture, long, MetaFlag::MetaFlag_MetaSerializeDisable, mNumSurfacesRequired);
			NEXTMEM4(tex, mArrayFrameNames, T3Texture, DCArray_Symbol, 0, mNumSurfacesLoaded, TelltaleToolLib_GetGameKeyIndex("BATMAN"),-1);
			NEXTMEM2(tex, mToonRegions, T3Texture, DCArray_T3ToonGradientRegion, MetaFlag::MetaFlag_EditorHide, mArrayFrameNames);
			SERIALIZER(tex, T3Texture);
			ADD(tex);

			DEFINET2(theader, T3Texture::StreamHeader);
			FIRSTMEM2(theader, mRegionCount, T3Texture::StreamHeader, long, 0);
			NEXTMEM2(theader, mAuxDataCount, T3Texture::StreamHeader, long, 0, mRegionCount);
			NEXTMEM2(theader, mTotalDataSize, T3Texture::StreamHeader, long, 0, mAuxDataCount);
			ADD(theader);

			DEFINET2(rsh, T3Texture::RegionStreamHeader);
			FIRSTMEM2(rsh, mFaceIndex, T3Texture::RegionStreamHeader, long, 0);
			NEXTMEM2(rsh, mMipIndex, T3Texture::RegionStreamHeader, long, 0, mFaceIndex);
			NEXTMEM2(rsh, mMipCount, T3Texture::RegionStreamHeader, long, 0, mMipIndex);
			NEXTMEM2(rsh, mDataSize, T3Texture::RegionStreamHeader, long, 0, mMipCount);
			NEXTMEM2(rsh, mPitch, T3Texture::RegionStreamHeader, long, 0, mDataSize);
			NEXTMEM4(rsh, mSlicePitch, T3Texture::RegionStreamHeader, long, 0, mPitch, TelltaleToolLib_GetGameKeyIndex("BATMAN"),-1);
			ADD(rsh);

			DEFINET2(aux, T3Texture::AuxilaryData);
			FIRSTMEM2(aux, mType, T3Texture::AuxilaryData, symbol, 0);
			NEXTMEM2(aux, mData, T3Texture::AuxilaryData, bb, 0, mType);
			ADD(aux);

			DEFINET2(locreg, LocalizationRegistry);
			EXT(locreg, locreg);
			FIRSTMEM2(locreg, mFlagIndexMap, LocalizationRegistry, Map_Symbol_int, 0);
			NEXTMEM2(locreg, mFlagIndexMapReverse, LocalizationRegistry, Map_int_Symbol, 0, mFlagIndexMap);
			NEXTMEM2(locreg, mToolProps, LocalizationRegistry, tp, 0, mFlagIndexMapReverse);
			meta_locreg_mToolProps.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("wdc");
			ADD(locreg);

			DEFINEHANDLE(dlg, Dlg);

			DEFINET2(dlgset, LanguageLookupMap::DlgIDSet);
			FIRSTMEM2(dlgset, mIDRange, LanguageLookupMap::DlgIDSet, rangeuint, 0);
			NEXTMEM2(dlgset, mAdditionalIDs, LanguageLookupMap::DlgIDSet, Set_u32, 0, mIDRange);
			NEXTMEM2(dlgset, mhDlg, LanguageLookupMap::DlgIDSet, Handledlg, 0, mAdditionalIDs);
			ADD(dlgset);

			DEFINEDCARRAY2(LanguageLookupMap::DlgIDSet, dlgsetarr);

			DEFINET2(llm, LanguageLookupMap);
			EXT(llm, llm);
			FIRSTMEM2(llm, mIDSets, LanguageLookupMap, DCArray_dlgsetarr, 0);
			ADD(llm);

			DEFINEKEYFRAMEDVALUE(float, float, float);

			DEFINET2(trm, TransitionRemapper);
			FIRSTMEM2(trm, mRemapKeys, TransitionRemapper, kfv_float, 0);
			ADD(trm);

			DEFINET2(tmi, TransitionMap::TransitionMapInfo);
			FIRSTMEM2(tmi, mRemapper, TransitionMap::TransitionMapInfo, trm, 0);
			ADD(tmi);

			DEFINEMAP2(Symbol, TransitionMap::TransitionMapInfo, symbol, tmapinfo, Symbol::CompareCRC);

			DEFINET2(tmap, TransitionMap);
			EXT(tmap, tmap);
			FIRSTMEM2(tmap, mTransitionRemappers, TransitionMap, Map_symbol_tmapinfo, 0);
			ADD(tmap);

			DEFINET2(enumbase, EnumBase);
			ADDFLAGS(enumbase, 0x21);
			ADD(enumbase);

			DEFINET2(soundmat, SoundFootsteps::EnumMaterial);
			ADDFLAGS(soundmat, 0x8008);
			FIRSTMEM2(soundmat, mVal, SoundFootsteps::EnumMaterial, long, 0x40);
			FIRSTENUM(soundmat, mVal, Default, SoundFootsteps::Default, 0);
			NEXTENUM(soundmat, mVal, Carpet, 2, 0, Default);
			NEXTENUM(soundmat, mVal, Concrete, 3, 0, Carpet);
			NEXTENUM2(soundmat, mVal, "Concrete (Wet)", Concrete_Wet, 4, 0, Concrete);
			NEXTENUM2(soundmat, mVal, "Dirt", Dirt, 5, 0, Concrete_Wet);
			NEXTENUM2(soundmat, mVal, "Grass", Grass, 6, 0, Dirt);
			NEXTENUM2(soundmat, mVal, "Grass (Tall)", Grass_Tall, 7, 0, Grass);
			NEXTENUM2(soundmat, mVal, "Gravel", Gravel, 8, 0, Grass_Tall);
			NEXTENUM2(soundmat, mVal, "Leaves", Leaves, 9, 0, Gravel);
			NEXTENUM2(soundmat, mVal, "Linoleum", Linoleum, 10, 0, Leaves);
			NEXTENUM2(soundmat, mVal, "Metal (Thick)", Metal_Thick, 11, 0, Linoleum);
			NEXTENUM2(soundmat, mVal, "Metal (Thin)", Metal_Thin, 12, 0, Metal_Thick);
			NEXTENUM2(soundmat, mVal, "Mud", Mud, 13, 0, Metal_Thin);
			NEXTENUM2(soundmat, mVal, "Puddle", Puddle, 14, 0, Mud);
			NEXTENUM2(soundmat, mVal, "Sand", Sand, 15, 0, Puddle);
			NEXTENUM2(soundmat, mVal, "Snow", Snow, 16, 0, Sand);
			NEXTENUM2(soundmat, mVal, "Tile (Hard)", Tile_Hard, 17, 0, Snow);
			NEXTENUM2(soundmat, mVal, "Wood", Wood, 18, 0, Tile_Hard);
			DEFINEM(soundmat, base);
			meta_soundmat_base.mpName = "Baseclass_EnumBase";
			meta_soundmat_base.mOffset = 0;
			meta_soundmat_base.mpMemberDesc = &meta_enumbase;
			meta_soundmat_mVal.mpNextMember = &meta_soundmat_base;
			ADD(soundmat);

			DEFINET2(wboxedge, WalkBoxes::Edge);
			FIRSTMEM2(wboxedge, mV1, WalkBoxes::Edge, long, 0);
			NEXTMEM2(wboxedge, mV2, WalkBoxes::Edge, long, 0, mV1);
			NEXTMEM2(wboxedge, mEdgeDest, WalkBoxes::Edge, long, 0, mV2);
			NEXTMEM2(wboxedge, mEdgeDestEdge, WalkBoxes::Edge, long, 0, mEdgeDest);
			NEXTMEM2(wboxedge, mEdgeDir, WalkBoxes::Edge, long, 0, mEdgeDestEdge);
			NEXTMEM2(wboxedge, mMaxRadius, WalkBoxes::Edge, float, 0, mEdgeDir);
			ADD(wboxedge);

			DEFINET2(wboxquad, WalkBoxes::Quad);
			FIRSTMEM2(wboxquad, mVerts, WalkBoxes::Quad, sarray_i32_4, 0);
			ADD(wboxquad);

			DEFINET2(wboxvert, WalkBoxes::Vert);
			FIRSTMEM2(wboxvert, mFlags, WalkBoxes::Vert, flags, 0);
			NEXTMEM2(wboxvert, mPos, WalkBoxes::Vert, vec3, 0, mFlags);
			ADD(wboxvert);

			DEFINESARRAY_(WalkBoxes::Edge, wboxedge, 3);

			DEFINET2(wboxtri, WalkBoxes::Tri);
			FIRSTMEM2(wboxtri, mFootstepMaterial, WalkBoxes::Tri, soundmat, 0);
			NEXTMEM2(wboxtri, mFlags, WalkBoxes::Tri, flags, 0, mFootstepMaterial);
			NEXTMEM2(wboxtri, mNormal, WalkBoxes::Tri, long, 0, mFlags);
			NEXTMEM2(wboxtri, mQuadBuddy, WalkBoxes::Tri, long, 0, mNormal);
			NEXTMEM2(wboxtri, mMaxRadius, WalkBoxes::Tri, float, 0, mQuadBuddy);
			NEXTMEM2(wboxtri, mVerts, WalkBoxes::Tri, sarray_i32_3, 0, mMaxRadius);
			NEXTMEM2(wboxtri, mEdgeInfo, WalkBoxes::Tri, sarray_wboxedge_3, 0, mVerts);
			NEXTMEM2(wboxtri, mVertOffsets, WalkBoxes::Tri, sarray_i32_3, 0, mEdgeInfo);
			NEXTMEM2(wboxtri, mVertScales, WalkBoxes::Tri, sarray_float_3, 0, mVertOffsets);
			ADD(wboxtri);

			DEFINEDCARRAY2(WalkBoxes::Tri, tri);
			DEFINEDCARRAY2(WalkBoxes::Vert, vert);
			DEFINEDCARRAY2(WalkBoxes::Quad, quad);
			DEFINEDCARRAY2(Vector3, vec3);

			DEFINET2(wbox, WalkBoxes);
			EXT(wbox, wbox);
			SERIALIZER(wbox, WalkBoxes);
			FIRSTMEM2(wbox, mName, WalkBoxes, string, 0);
			NEXTMEM2(wbox, mTris, WalkBoxes, DCArray_tri, 0, mName);
			NEXTMEM2(wbox, mVerts, WalkBoxes, DCArray_vert, 0, mTris);
			NEXTMEM2(wbox, mNormals, WalkBoxes, DCArray_vec3, 0, mVerts);
			NEXTMEM2(wbox, mQuads, WalkBoxes, DCArray_quad, 0x20, mNormals);
			ADD(wbox);

			DEFINET2(sebmp, SoundEventBankMap);
			EXT(sebmp, soundeventbankmap);
			FIRSTMEM2(sebmp, mBankMap, SoundEventBankMap, Map_String_dcarraystring, 0);
			NEXTMEM2(sebmp, mbLoadAllBanksGlobally, SoundEventBankMap, bool, 0, mBankMap);
			ADD(sebmp);

			DEFINEHANDLE(hanm, Animation);
			DEFINEHANDLE(hchore, Chore);

			DEFINET2(animorchore, AnimOrChore);
			ADDFLAGS(animorchore, MetaFlag::MetaFlag_PlaceInAddPropMenu);
			FIRSTMEM2(animorchore, mhAnim, AnimOrChore, Handlehanm, 0);
			NEXTMEM2(animorchore, mhChore, AnimOrChore, Handlehchore, 0, mhAnim);
			ADD(animorchore);

			DEFINET2(resgroups, ResourceGroups);
			ADDFLAGS(resgroups, MetaFlag::MetaFlag_EditorHide);
			FIRSTMEM2(resgroups, mGroups, ResourceGroups, Map_Symbol_float, 0);
			ADD(resgroups);

			DEFINET2(propo, ActingOverridablePropOwner);
			FIRSTMEM2(propo, mSerializationFlags, ActingOverridablePropOwner, flags, 0);
			SERIALIZER(propo, ActingOverridablePropOwner);
			ADD(propo);

			DEFINET2(tanmode, EnumeTangentModes);
			FIRSTMEM2(tanmode, mVal, EnumeTangentModes, long, 0);
			FIRSTENUM(tanmode, mVal, eTangentUnknown, eTangentUnknown, 0);
			NEXTENUM(tanmode, mVal, eTangentStepped, eTangentStepped, 0, eTangentUnknown);
			NEXTENUM(tanmode, mVal, eTangentKnot, eTangentKnot, 0, eTangentStepped);
			NEXTENUM(tanmode, mVal, eTangentSmooth, eTangentSmooth, 0, eTangentKnot);
			NEXTENUM(tanmode, mVal, eTangentFlat, eTangentFlat, 0, eTangentSmooth);
			NEXTMEM1(tanmode, "Baseclass_EnumBase", BASE_CLASS, mVal,
				EnumeTangentModes, enumbase, 0, mVal);
			ADD(tanmode);

			DEFINET2(actres, ActingResource);
			FIRSTMEM(actres, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, ActingResource, propo, 0);
			NEXTMEM2(actres, mResource, ActingResource, animorchore, MetaFlag::MetaFlag_EditorHide, mSerializationFlags);
			NEXTMEM2(actres, mValidIntensityRange, ActingResource, rangef, 0, mResource);
			ADD(actres);

			DEFINET2(actdur, ActingPalette::EnumActiveDuring);
			ADDFLAGS(actdur, 0x8008);
			FIRSTMEM2(actdur, mVal, ActingPalette::EnumActiveDuring, long, 0);
			FIRSTENUM(actdur, mVal, always, ActingPalette::ActiveDuring::always, 0);
			NEXTENUM(actdur, mVal, talking, ActingPalette::ActiveDuring::talking, 0, always);
			NEXTENUM(actdur, mVal, listening, ActingPalette::ActiveDuring::listening, 0, talking);
			NEXTMEM1(actdur, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingPalette::EnumActiveDuring, enumbase, 0x10, mVal);
			ADD(actdur);

			DEFINET2(actrun1, ActingAccentPalette::EnumOverrun);
			ADDFLAGS(actrun1, 0x8008);
			FIRSTMEM2(actrun1, mVal, ActingAccentPalette::EnumOverrun, long, 0);
			FIRSTENUM(actrun1, mVal, disallowed, ActingAccentPalette::Overrun::disallowed, 0);
			NEXTENUM(actrun1, mVal, allowed, ActingAccentPalette::Overrun::allowed, 0, disallowed);
			NEXTMEM1(actrun1, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingAccentPalette::EnumOverrun, enumbase, 0x10, mVal);
			ADD(actrun1);

			DEFINET2(actrun, ActingPalette::EnumOverrun);
			ADDFLAGS(actrun, 0x8008);
			FIRSTMEM2(actrun, mVal, ActingPalette::EnumOverrun, long, 0);
			FIRSTENUM(actrun, mVal, disallowed, ActingPalette::Overrun::disallowed, 0);
			NEXTENUM(actrun, mVal, allowed, ActingPalette::Overrun::allowed, 0, disallowed);
			NEXTMEM1(actrun, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingPalette::EnumOverrun, enumbase, 0x10, mVal);
			ADD(actrun);

			DEFINET2(actrel, ActingPalette::EnumEndRelativeTo);
			ADDFLAGS(actrel, 0x8008);
			FIRSTMEM2(actrel, mVal, ActingPalette::EnumEndRelativeTo, long, 0);
			FIRSTENUM(actrel, mVal, beginning, ActingPalette::EndRelativeTo::beginning, 0);
			NEXTENUM(actrel, mVal, end, ActingPalette::EndRelativeTo::end, 0, beginning);
			NEXTENUM(actrel, mVal, transition, ActingPalette::EndRelativeTo::transition, 0, end);
			NEXTMEM1(actrel, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingPalette::EnumEndRelativeTo, enumbase, 0x10, mVal);
			ADD(actrel);

			DEFINET2(aresp, ActingResource*);
			ADD(aresp);
			DEFINEDCARRAY2(ActingResource*, actresp);

			DEFINET2(actp, ActingPalette);
			FIRSTMEM(actp, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, ActingPalette, propo, 0x10);
			NEXTMEM(actp, "Baseclass_UID::Owner", miUniqueID, ActingPalette, uidowner, 0x10, mSerializationFlags);
			SERIALIZER(actp, ActingPalette);
			NEXTMEM2(actp, mName, ActingPalette, string, 0x20, miUniqueID);
			NEXTMEM2(actp, mActiveDuring, ActingPalette, actdur, 0, mName);
			NEXTMEM2(actp, mTimeBetweenActions, ActingPalette, rangef, 0, mActiveDuring);
			NEXTMEM2(actp, mFirstActionDelayRange, ActingPalette, rangef, 0, mTimeBetweenActions);
			NEXTMEM2(actp, mSpilloutBufPreRange, ActingPalette, rangef, 0x20, mFirstActionDelayRange);
			NEXTMEM2(actp, mSpilloutBufPostRange, ActingPalette, rangef, 0, mSpilloutBufPreRange);
			NEXTMEM2(actp, mLatestStartOffsetRange, ActingPalette, rangef, 0, mSpilloutBufPostRange);
			NEXTMEM2(actp, mValidIntensityRange, ActingPalette, rangef, 0, mLatestStartOffsetRange);
			NEXTMEM2(actp, mResourcePtrs, ActingPalette, DCArray_actresp, 0x21, mValidIntensityRange);
			NEXTMEM2(actp, mGroupMembershipUID, ActingPalette, long, 0x20, mResourcePtrs);
			NEXTMEM2(actp, mFlags, ActingPalette, flags, 0x20, mGroupMembershipUID);
			NEXTMEM2(actp, mFirstActionEndRel, ActingPalette, actrel, 0, mFlags);
			NEXTMEM2(actp, mEndOffsetRel, ActingPalette, actrel, 0, mFirstActionEndRel);
			NEXTMEM2(actp, mLatestStartOffsetRel, ActingPalette, actrel, 0, mEndOffsetRel);
			NEXTMEM2(actp, mOverrunAllowed, ActingPalette, actrun, 0, mLatestStartOffsetRel);
			NEXTMEM2(actp, mMoodOverrunAllowed, ActingPalette, actrun, 0, mOverrunAllowed);
			NEXTMEM2(actp, mDisableAct, ActingPalette, bool, 0, mMoodOverrunAllowed);
			NEXTMEM2(actp, mJunket, ActingPalette, long, 0, mDisableAct);
			ADD(actp);


			DEFINET2(actap, ActingAccentPalette);
			ADDFLAGS(actap, 0x40);
			SERIALIZER(actap, ActingAccentPalette);
			FIRSTMEM(actap, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, ActingAccentPalette, propo, 0x10);
			NEXTMEM(actap, "Baseclass_UID::Owner", miUniqueID, ActingAccentPalette, uidowner, 0x10, mSerializationFlags);
			NEXTMEM2(actap, mName, ActingAccentPalette, string, 0, miUniqueID);
			NEXTMEM2(actap, mStartOffsetRange, ActingAccentPalette, rangef, 0, mName);
			NEXTMEM2(actap, mMoodOverrunAllowed, ActingAccentPalette, actrun1, 0, mStartOffsetRange);
			NEXTMEM2(actap, mDisableAct, ActingAccentPalette, bool, 0, mMoodOverrunAllowed);
			NEXTMEM2(actap, mValidIntensityRange, ActingAccentPalette, rangef, 0, mDisableAct);
			NEXTMEM2(actap, mSpilloutBufPostRange, ActingAccentPalette, rangef, 0, mValidIntensityRange);
			NEXTMEM2(actap, mRandomChance, ActingAccentPalette, float, 0, mSpilloutBufPostRange);
			NEXTMEM2(actap, mTrackID, ActingAccentPalette, long, 0, mRandomChance);
			FIRSTENUM2(actap, mTrackID, "Body", Body, 1, 0);
			NEXTENUM2(actap, mTrackID, "Face", Face, 2, 0, Body);
			NEXTENUM2(actap, mTrackID, "Head 1", Head1, 3, 0, Face);
			NEXTENUM2(actap, mTrackID, "Head 2", Head2, 4, 0, Head1);
			NEXTMEM2(actap, mResourcePtrs, ActingAccentPalette, DCArray_actresp, 0x21, mTrackID);
			NEXTMEM2(actap, mGroupMembershipUID, ActingAccentPalette, long, 0x20, mResourcePtrs);
			NEXTMEM2(actap, mFlags, ActingAccentPalette, flags, 0x20, mGroupMembershipUID);
			NEXTMEM2(actap, mVersion, ActingAccentPalette, long, 0x20, mFlags);
			ADD(actap);

			DEFINEHANDLE(tmap, TransitionMap);

			DEFINET2(idlet, ActingPaletteGroup::EnumIdleTransition);
			ADDFLAGS(idlet, 0x8008);
			FIRSTMEM(idlet, "mVal", mVal, ActingPaletteGroup::EnumIdleTransition, long, 0);
			FIRSTENUM(idlet, mVal, transitionLinear, 1, 0);
			NEXTENUM(idlet, mVal, transitionEaseInOut, 2, 0, transitionLinear);
			NEXTENUM(idlet, mVal, transitionUnused, 3, 0, transitionEaseInOut);
			NEXTMEM1(idlet, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingPalette::EnumEndRelativeTo, enumbase, 0x10, mVal);
			ADD(idlet);

			DEFINET2(actpt, ActingPaletteGroup::ActingPaletteTransition);
			FIRSTMEM2(actpt, mTransition, ActingPaletteGroup::ActingPaletteTransition, string, 0);
			NEXTMEM2(actpt, mTransitionIn, ActingPaletteGroup::ActingPaletteTransition, animorchore, 0, mTransition);
			NEXTMEM2(actpt, mCenterOffset, ActingPaletteGroup::ActingPaletteTransition, float, 0, mTransitionIn);
			NEXTMEM2(actpt, mPreDelay, ActingPaletteGroup::ActingPaletteTransition, float, 0, mCenterOffset);
			NEXTMEM2(actpt, mPostDelay, ActingPaletteGroup::ActingPaletteTransition, float, 0, mPreDelay);
			NEXTMEM2(actpt, mFadeTime, ActingPaletteGroup::ActingPaletteTransition, float, 0, mPostDelay);
			ADD(actpt);

			DEFINELIST_(ActingPaletteGroup::ActingPaletteTransition, acttrans);

			DEFINET2(actg, ActingPaletteGroup);
			SERIALIZER(actg, ActingPaletteGroup);
			FIRSTMEM(actg, "Baseclass_UID::Owner", miUniqueID, ActingPaletteGroup, uidowner, 0x10, mSerializationFlags);
			NEXTMEM2(actg, mName, ActingPaletteGroup, string, 0x20, miUniqueID);
			NEXTMEM2(actg, mIdle, ActingPaletteGroup, animorchore, 0, mName);
			NEXTMEM2(actg, mTalkingIdle, ActingPaletteGroup, animorchore, 0x20, mIdle);
			NEXTMEM2(actg, mMumbleMouth, ActingPaletteGroup, animorchore, 0x20, mTalkingIdle);
			NEXTMEM2(actg, mWeight, ActingPaletteGroup, float, 0x20, mMumbleMouth);
			NEXTMEM2(actg, mTransitionIn, ActingPaletteGroup, animorchore, 0x20, mWeight);
			NEXTMEM2(actg, mTransitionOut, ActingPaletteGroup, animorchore, 0x20, mTransitionIn);
			NEXTMEM2(actg, mTransitions, ActingPaletteGroup, List_acttrans, 0x20, mTransitionOut);
			NEXTMEM2(actg, mIdleTransitionTimeOverride, ActingPaletteGroup, float, 0, mTransitions);
			NEXTMEM2(actg, mhIdleTransitionMap, ActingPaletteGroup, Handletmap, 0, mIdleTransitionTimeOverride);
			meta_actg_mhIdleTransitionMap.mMinMetaVersion = 6;//only in new
			NEXTMEM2(actg, mIdleTransitionKind, ActingPaletteGroup, idlet, 0, mhIdleTransitionMap);
			NEXTMEM2(actg, mRandomAutoMin, ActingPaletteGroup, float, 0, mIdleTransitionKind);
			NEXTMEM2(actg, mRandomAutoMax, ActingPaletteGroup, float, 0, mRandomAutoMin);
			ADD(actg);

			DEFINET2(actpp, ActingPalette*);
			ADD(actpp);

			DEFINEDCARRAY2(ActingPalette*, actpp);

			DEFINET2(actapp, ActingAccentPalette*);
			ADD(actapp);

			DEFINEDCARRAY2(ActingAccentPalette*, actapp);

			DEFINET2(actppg, ActingPaletteGroup*);
			ADD(actppg);

			DEFINEDCARRAY2(ActingPaletteGroup*, actppg);

			DEFINET2(actc, ActingPaletteClass);
			SERIALIZER(actc, ActingPaletteClass);
			FIRSTMEM(actc, "Baseclass_UID::Generator", miNextUniqueID, ActingPaletteClass, uidgen, 0x10);
			NEXTMEM(actc, "Baseclass_UID::Owner", miUniqueID, ActingPaletteClass, uidowner, 0x10, miNextUniqueID);
			NEXTMEM(actc, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, ActingPaletteClass, propo, 0x10, miUniqueID);
			NEXTMEM2(actc, mName, ActingPaletteClass, string, 0, mSerializationFlags);
			NEXTMEM2(actc, mPalettePtrs, ActingPaletteClass, DCArray_actpp, 0x21, mName);
			NEXTMEM2(actc, mAccentPalettePtrs, ActingPaletteClass, DCArray_actapp, 0x21, mPalettePtrs);
			NEXTMEM2(actc, mPaletteGroupPtrs, ActingPaletteClass, DCArray_actppg, 0x21, mAccentPalettePtrs);
			NEXTMEM2(actc, mAlternateNames, ActingPaletteClass, DCArray_String, 0, mPaletteGroupPtrs);
			NEXTMEM2(actc, mDefaultPaletteGroupID, ActingPaletteClass, long, 0x20, mAlternateNames);
			NEXTMEM2(actc, mFlags, ActingPaletteClass, flags, 0x20, mDefaultPaletteGroupID);
			NEXTMEM2(actc, mInstantChange, ActingPaletteClass, bool, 0x20, mFlags);
			ADD(actc);

			DEFINEDCARRAY2(ActingPaletteClass*, actcp);
			DEFINEDCARRAY2(ActingPaletteClass, actc);
			DEFINET2(sg, StyleGuide);
			SERIALIZER(sg, StyleGuide);
			EXT(sg, "style");
			FIRSTMEM(sg, "Baseclass_UID::Generator", miNextUniqueID, StyleGuide, uidgen, 0x10);
			NEXTMEM(sg, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, StyleGuide, propo, 0x10, miNextUniqueID);
			NEXTMEM2(sg, mDefPaletteClassID, StyleGuide, long, 0, mSerializationFlags);
			NEXTMEM2(sg, mbGeneratesLookAts, StyleGuide, bool, 0, mDefPaletteClassID);
			NEXTMEM2(sg, mPaletteClassPtrs, StyleGuide, DCArray_actcp, 1, mbGeneratesLookAts);
			NEXTMEM2(sg, mFlags, StyleGuide, flags, 0, mPaletteClassPtrs);
			NEXTMEM2(sg, mPaletteClasses, StyleGuide, DCArray_actc, 0x20, mFlags);
			NEXTMEM2(sg, mDefPaletteClassIndex, StyleGuide, long, 0x20, mPaletteClasses);
			ADD(sg);

			DEFINEHANDLE(sound, SoundData);

			DEFINET(ptrbase, void*);
			meta_ptrbase.Initialize("Ptr<PtrBase>");
			meta_ptrbase.mFlags |= 1u;
			ADD(ptrbase);

			DEFINET2(lr, LanguageResource);
			SERIALIZER(lr, LanguageResource);
			EXT(lr, langres);
			FIRSTMEM2(lr, mId, LanguageResource, long, 0);
			NEXTMEM2(lr, mPrefix, LanguageResource, string, 0, mId);
			NEXTMEM2(lr, mText, LanguageResource, string, 0, mPrefix);
			NEXTMEM2(lr, mhAnimation, LanguageResource, Handlehanm, 0, mText);
			NEXTMEM2(lr, mhVoiceData, LanguageResource, Handlesound, 0, mhAnimation);
			NEXTMEM2(lr, mShared, LanguageResource, bool, 0, mhVoiceData);
			NEXTMEM2(lr, mAllowSharing, LanguageResource, bool, 0, mShared);
			NEXTMEM2(lr, mbNoAnim, LanguageResource, bool, 0, mAllowSharing);
			NEXTMEM2(lr, mpLipsyncAnimation, LanguageResource, ptrbase, 0, mbNoAnim);
			NEXTMEM2(lr, mFlags, LanguageResource, flags, 0, mpLipsyncAnimation);
			static MetaFlagDescription synth;
			synth.mpFlagName = "Synthesized";
			synth.mFlagValue = 1;
			static MetaFlagDescription synth1;
			synth1.mpFlagName = "Lipsync ignore text";
			synth1.mFlagValue = 2;
			synth1.mpNext = &synth;
			static MetaFlagDescription synth2;
			synth2.mpFlagName = "Hide Subtitles";
			synth2.mFlagValue = 4;
			synth2.mpNext = &synth1;
			static MetaFlagDescription synth3;
			synth3.mpFlagName = "Is Voiced";
			synth3.mFlagValue = 8;
			synth3.mpNext = &synth2;
			static MetaFlagDescription synth4;
			synth4.mpFlagName = "Is Silent";
			synth4.mFlagValue = 16;
			synth4.mpNext = &synth3;
			meta_lr_mFlags.mpFlagDescriptions = &synth4;
			meta_lr_mFlags.mFlags = MetaFlag::MetaFlag_FlagType;
			ADD(lr);

			DEFINEMAP(int, LanguageResource, std::less<int>);

			DEFINET2(langdb, LanguageDatabase);
			EXT(langdb, langdb);
			SERIALIZER(langdb, LanguageDatabase);
			FIRSTMEM2(langdb, mLanguageResources, LanguageDatabase,
				Map_int_LanguageResource, 0);
			NEXTMEM2(langdb, mName, LanguageDatabase, string, 0, mLanguageResources);
			ADD(langdb);

			DEFINET2(locali, LocalizeInfo);
			FIRSTMEM2(locali, mFlags, LocalizeInfo, flags, 0);
			ADD(locali);

			DEFINET2(langresl, LanguageResLocal);
			ADDFLAGS(langresl, MetaFlag::MetaFlag_NoPanelCaption);
			FIRSTMEM2(langresl, mPrefix, LanguageResLocal, string, 0);
			NEXTMEM2(langresl, mText, LanguageResLocal, string, 0, mPrefix);
			NEXTMEM2(langresl, mLocalInfo, LanguageResLocal, locali, 0x20, mText);
			ADD(langresl);

			DEFINEDCARRAY2(LanguageResLocal, lresl);

			DEFINET2(pidp, ProjectDatabaseIDPair);
			FIRSTMEM2(pidp, mProjectID, ProjectDatabaseIDPair, long, 0);
			NEXTMEM2(pidp, mDBID, ProjectDatabaseIDPair, long, 0, mProjectID);
			ADD(pidp);

			DEFINET2(recs, RecordingUtils::EnumRecordingStatus);
			FIRSTMEM2(recs, mVal, RecordingUtils::EnumRecordingStatus, long, 0);
			FIRSTENUM2(recs, mVal, "Not Recorded", notrec, 0, 0);
			NEXTENUM2(recs, mVal, "Sent To Studio", sent, 0, 1, notrec);
			NEXTENUM2(recs, mVal, "Recorded", rec, 0, 2, sent);
			NEXTENUM2(recs, mVal, "Delivered", del, 0, 3, rec);
			NEXTMEM1(recs, "Baseclass_EnumBase", BASE_CLASS, mVal,
				RecordingUtils::EnumRecordingStatus, enumbase, 0x10, mVal);
			ADD(recs);

			DEFINET2(lres, LanguageRes);
			FIRSTMEM2(lres, mResName, LanguageRes, symbol, 0);
			NEXTMEM2(lres, mID, LanguageRes, long, 0x20, mResName);
			NEXTMEM2(lres, mIDAlias, LanguageRes, long, 0, mID);
			NEXTMEM2(lres, mhAnimation, LanguageRes, Handlehanm, 0, mIDAlias);
			NEXTMEM2(lres, mhVoiceData, LanguageRes, Handlesound, 0, mhAnimation);
			NEXTMEM2(lres, mLocalData, LanguageRes, DCArray_lresl, 0x20, mhVoiceData);
			NEXTMEM2(lres, mLengthOverride, LanguageRes, float, 0x20, mLocalData);
			NEXTMEM2(lres, mResolvedLocalData, LanguageRes, langresl, 0x20, mLengthOverride);
			NEXTMEM2(lres, mRecordingStatus, LanguageRes, recs, 0x20, mResolvedLocalData);
			NEXTMEM2(lres, mFlags, LanguageRes, flags, 0x20, mRecordingStatus);
			static MetaFlagDescription asynth;
			asynth.mpFlagName = "Synthesized";
			asynth.mFlagValue = 1;
			static MetaFlagDescription asynth1;
			asynth1.mpFlagName = "Lipsync ignore text";
			asynth1.mFlagValue = 2;
			asynth1.mpNext = &asynth;
			static MetaFlagDescription asynth2;
			asynth2.mpFlagName = "Hide Subtitles";
			asynth2.mFlagValue = 4;
			asynth2.mpNext = &asynth1;
			static MetaFlagDescription asynth3;
			asynth3.mpFlagName = "Is Voiced";
			asynth3.mFlagValue = 8;
			asynth3.mpNext = &asynth2;
			static MetaFlagDescription asynth4;
			asynth4.mpFlagName = "Is Silent";
			asynth4.mFlagValue = 16;
			asynth4.mpNext = &asynth3;
			static MetaFlagDescription asynth5;
			asynth5.mpFlagName = "No Anim";
			asynth5.mFlagValue = 32;
			asynth5.mpNext = &asynth4;
			meta_lres_mFlags.mpFlagDescriptions = &asynth5;
			meta_lres_mFlags.mFlags = MetaFlag::MetaFlag_FlagType;
			EXT(lres, lang);
			ADD(lres);

			DEFINET2(lanreg, LanguageRegister);
			FIRSTMEM(lanreg, "Baseclass_UID::Generator", miNextUniqueID, LanguageRegister, uidgen, 0x10);
			EXT(lanreg, lanreg);
			ADD(lanreg);

			DEFINEDCARRAY2(ProjectDatabaseIDPair, pdbidp);

			DEFINEMAP2(unsigned int, LanguageRes, uint, langres, std::less<unsigned int>);

			DEFINET2(landb, LanguageDB);
			EXT(landb, landb);
			SERIALIZER(landb,LanguageDB);
			FIRSTMEM(landb, "Baseclass_UID::Owner", miUniqueID, LanguageDB, uidowner, 0x10);
			NEXTMEM(landb, "Baseclass_UID::Generator", miNextUniqueID, LanguageDB, uidgen, 0x10, miUniqueID);
			NEXTMEM2(landb, mLanguageResources, LanguageDB, Map_uint_langres, 0, miNextUniqueID);
			NEXTMEM2(landb, mRegistry, LanguageDB, locreg, 0, mLanguageResources);
			NEXTMEM2(landb, mFlags, LanguageDB, flags, 0, mRegistry);
			NEXTMEM2(landb, mProjectID, LanguageDB, long, 0, mFlags);
			NEXTMEM2(landb, mExpandedIDRanges, LanguageDB, DCArray_pdbidp, 0, mProjectID);
			ADD(landb);

			DEFINEHANDLE(sprite, ParticleSprite);
			DEFINEHANDLE(tex, T3Texture);
			DEFINEHANDLE(font, Font);

			DEFINET2(ovsp, T3OverlaySpriteParams);
			FIRSTMEM2(ovsp, mhSprite, T3OverlaySpriteParams, Handlesprite, 0);
			NEXTMEM2(ovsp, mInitialPosition, T3OverlaySpriteParams, vec2, 0, mhSprite);
			NEXTMEM2(ovsp, mSize, T3OverlaySpriteParams, vec2, 0, mInitialPosition);
			NEXTMEM2(ovsp, mAnimation, T3OverlaySpriteParams, symbol, 0, mSize);
			NEXTMEM2(ovsp, mAnimationSpeed, T3OverlaySpriteParams, float, 0, mAnimation);
			NEXTMEM2(ovsp, mFlags, T3OverlaySpriteParams, flags, MetaFlag::MetaFlag_FlagType, mAnimationSpeed);
			static MetaFlagDescription ovspf;
			ovspf.mpFlagName = "eOverlaySprite_AnimationLoop";
			ovspf.mFlagValue = 1;
			ovspf.mpNext = NULL;
			meta_ovsp_mFlags.mpFlagDescriptions = &ovspf;
			ADD(ovsp);

			DEFINET2(oodtp, T3OverlayTextParams);
			FIRSTMEM2(oodtp, mhFont, T3OverlayTextParams, Handlefont, 0);
			NEXTMEM2(oodtp, mhDlg, T3OverlayTextParams, Handledlg, 0, mhFont);
			NEXTMEM2(oodtp, mDlgNodeName, T3OverlayTextParams, symbol, 0, mhDlg);
			NEXTMEM2(oodtp, mText, T3OverlayTextParams, string, 0, mDlgNodeName);
			NEXTMEM2(oodtp, mInitialPosition, T3OverlayTextParams, vec2, 0, mText);
			ADD(oodtp);

			DEFINET2(oods, T3OverlayObjectData_Sprite);
			FIRSTMEM2(oods, mName, T3OverlayObjectData_Sprite, symbol, 0);
			NEXTMEM2(oods, mParams, T3OverlayObjectData_Sprite, ovsp, 0, mName);
			ADD(oods);

			DEFINET2(oodt, T3OverlayObjectData_Text);
			FIRSTMEM2(oodt, mName, T3OverlayObjectData_Text, symbol, 0);
			NEXTMEM2(oodt, mParams, T3OverlayObjectData_Text, oodtp, 0, mName);
			ADD(oodt);

			DEFINET2(op, T3OverlayParams);
			FIRSTMEM2(op, mhBackgroundTexture, T3OverlayParams, Handletex, 0);
			NEXTMEM2(op, mhChore, T3OverlayParams, Handlehchore, 0, mhBackgroundTexture);
			NEXTMEM2(op, mMinDisplayTime, T3OverlayParams, float, 0, mhChore);
			NEXTMEM2(op, mFadeTime, T3OverlayParams, float, 0, mMinDisplayTime);
			ADD(op);

			DEFINEDCARRAY2(T3OverlayObjectData_Text, overlaytext);
			DEFINEDCARRAY2(T3OverlayObjectData_Sprite, overlaysprite);

			DEFINET2(overlay, T3OverlayData);
			EXT(overlay, overlay);
			FIRSTMEM2(overlay, mName, T3OverlayData, string, 0x20);
			NEXTMEM2(overlay, mSpriteObjects, T3OverlayData, DCArray_overlaysprite, 0, mName);
			NEXTMEM2(overlay, mTextObjects, T3OverlayData, DCArray_overlaytext, 0, mSpriteObjects);
			NEXTMEM2(overlay, mParams, T3OverlayData, op, 0, mTextObjects);
			ADD(overlay);

			DEFINET2(blendmode, BlendMode);
			FIRSTMEM2(blendmode, mVal, BlendMode, long, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM2(blendmode, mVal, "Normal", normal, T3BlendMode::eBlendModeNormal, 0);
			NEXTENUM2(blendmode, mVal, "Default", default, T3BlendMode::eBlendModeDefault, 0, normal);
			NEXTENUM2(blendmode, mVal, "Alpha", alpha, 1, 0, default);
			NEXTENUM2(blendmode, mVal, "Alpha Alpha Test", aat, 2, 0, alpha);
			NEXTENUM2(blendmode, mVal, "Alpha Test", at, 3, 0, aat);
			NEXTENUM2(blendmode, mVal, "Inverse Alpha Test", iat, 4, 0, at);
			NEXTENUM2(blendmode, mVal, "Add", add, 5, 0, iat);
			NEXTENUM2(blendmode, mVal, "Multiply", mul, 6, 0, add);
			NEXTENUM2(blendmode, mVal, "InverseMultiply", imul, 7, 0, mul);
			NEXTENUM2(blendmode, mVal, "Alpha Add", aadd, 8, 0, imul);
			NEXTENUM2(blendmode, mVal, "Alpha Subtract", asub, 9, 0, aadd);
			NEXTENUM2(blendmode, mVal, "Alpha Inverse Alpha Test", aiat, 10, 0, asub);
			NEXTENUM2(blendmode, mVal, "Add Alpha Test", aat1,11, 0, aiat);
			NEXTENUM2(blendmode, mVal, "Add Inverse Alpha Test", aaiat, 12, 0, aat1);
			NEXTENUM2(blendmode, mVal, "Multiply Alpha Test", mat, 13, 0, aaiat);
			NEXTENUM2(blendmode, mVal, "Multiply Inverse Alpha Test", miat, 14, 0,mat);
			NEXTMEM1(blendmode, "Baseclass_EnumBase", BASE_CLASS, mVal, BlendMode, enumbase, MetaFlag::MetaFlag_BaseClass, mVal);
			ADD(blendmode);

			DEFINESARRAY_( Handle<T3Texture>, handletex, 1);

			DEFINET2(spriteanm, ParticleSprite::Animation);
			FIRSTMEM2(spriteanm, mName, ParticleSprite::Animation, symbol, 0);
			NEXTMEM2(spriteanm, mStartFrame, ParticleSprite::Animation, long, 0, mName);
			NEXTMEM2(spriteanm, mFrameCount, ParticleSprite::Animation, long, 0, mStartFrame);
			ADD(spriteanm);

			DEFINEDCARRAY2(ParticleSprite::Animation, spriteanm);

			DEFINET2(sprite, ParticleSprite);
			EXT(sprite, sprite);
			SERIALIZER(sprite, ParticleSprite);
			FIRSTMEM2(sprite, mName, ParticleSprite, string, 0x20);
			NEXTMEM2(sprite, mhTexture, ParticleSprite, sarray_handletex_1, 0, mName);
			NEXTMEM2(sprite, mTextureX, ParticleSprite, long, 0, mhTexture);
			NEXTMEM2(sprite, mTextureY, ParticleSprite, long, 0, mTextureX);
			NEXTMEM2(sprite, mSpriteSize, ParticleSprite, vec2, 0, mTextureY);
			NEXTMEM2(sprite, mBlendMode, ParticleSprite, blendmode, 0, mSpriteSize);
			NEXTMEM2(sprite, mAnimations, ParticleSprite, DCArray_spriteanm, 0, mBlendMode);
			ADD(sprite);

			DEFINET2(rectf, TRect<float>);
			ADDFLAGS(rectf, MetaFlag::MetaFlag_MetaSerializeBlockingDisabled);
			FIRSTMEM2(rectf, left, TRect<float>, float, 0);
			NEXTMEM2(rectf, right, TRect<float>, float, 0,left);
			NEXTMEM2(rectf, top, TRect<float>, float, 0, right);
			NEXTMEM2(rectf, bottom, TRect<float>, float, 0, top);
			ADD(rectf);

			DEFINET2(glyph, Font::GlyphInfo);
			FIRSTMEM2(glyph, mTexturePage, Font::GlyphInfo, long, 0);
			NEXTMEM2(glyph, mChannel, Font::GlyphInfo, long, 0, mTexturePage);
			NEXTMEM2(glyph, mGlyph, Font::GlyphInfo, rectf, 0, mChannel);
			NEXTMEM2(glyph, mWidth, Font::GlyphInfo, float, 0, mGlyph);
			NEXTMEM2(glyph, mHeight, Font::GlyphInfo, float, 0, mWidth);
			NEXTMEM2(glyph, mXOffset, Font::GlyphInfo, float, 0, mHeight);
			NEXTMEM2(glyph, mYOffset, Font::GlyphInfo, float, 0, mXOffset);
			NEXTMEM2(glyph, mXAdvance, Font::GlyphInfo, float, 0, mYOffset);
			NEXTMEM4(glyph, mGradientSize, Font::GlyphInfo, float, 0, mXAdvance, -1, TelltaleToolLib_GetGameKeyIndex("MICHONNE"));
			ADD(glyph);

			DEFINEMAP2(unsigned int, Font::GlyphInfo, uint,
				glyphinf, std::less<unsigned int>);

			DEFINEDCARRAY(T3Texture);
			DEFINEDCARRAY2(unsigned int, uint);

			DEFINET2(font, Font);
			ADDFLAGS(font, MetaFlag::MetaFlag_RenderResource);
			EXT(font, font);
			SERIALIZER(font, Font);
			FIRSTMEM2(font, mName, Font, string, 0);
			NEXTMEM2(font, mbUnicode, Font, bool, 0, mName);
			NEXTMEM2(font, mHeight, Font, float, 0, mbUnicode);
			NEXTMEM2(font, mBase, Font, float, 0, mHeight);
			//WII SCALE, FONT SCALE
			NEXTMEM4(font, mWiiScale, Font, float, 0, mBase, -1, TelltaleToolLib_GetGameKeyIndex("BATMAN2"));
			NEXTMEM4(font, mFontScale, Font, float, 0, mWiiScale, -1, TelltaleToolLib_GetGameKeyIndex("BATMAN2"));
			//-------
			NEXTMEM2(font, mGlyphInfo, Font, Map_uint_glyphinf, 0, mFontScale);
			NEXTMEM2(font, mTexturePages, Font, DCArray_T3Texture, 0, mGlyphInfo);
			NEXTMEM2(font, mIsDistanceField, Font, bool, 0, mTexturePages);
			//NEWER GAMES
			NEXTMEM4(font, mIsRuntime, Font, bool, 0, mIsDistanceField, TelltaleToolLib_GetGameKeyIndex("WD4"),-1);
			NEXTMEM4(font, mIsFiltered, Font, bool, 0, mIsRuntime, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			NEXTMEM4(font, mTtfData, Font, bb, 0, mIsFiltered, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			NEXTMEM4(font, mBasePointSize, Font, float, 0, mTtfData, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			NEXTMEM4(font, mPreferredPointSizes, Font, DCArray_uint, 0, mBasePointSize, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			//------
			ADD(font);

			DEFINET2(dlgid, DlgObjID);
			FIRSTMEM2(dlgid, mID, DlgObjID, symbol, 0);
			ADD(dlgid);

			DEFINET2(startnodeoff, PreloadPackage::StartNodeOffset);
			FIRSTMEM2(startnodeoff, mStartNodeChain, PreloadPackage::StartNodeOffset, dlgid, 0);
			NEXTMEM2(startnodeoff, fStartTimeSeconds, PreloadPackage::StartNodeOffset, float, 0,mStartNodeChain);
			NEXTMEM2(startnodeoff, mfMinDurationToPreload, PreloadPackage::StartNodeOffset, float, 0, fStartTimeSeconds);
			ADD(startnodeoff);

			DEFINET2(idandnodeoff, PreloadPackage::RuntimeDataDialog::DlgObjIdAndStartNodeOffset);
			FIRSTMEM2(idandnodeoff, mID, PreloadPackage::RuntimeDataDialog::DlgObjIdAndStartNodeOffset, dlgid, 0);
			NEXTMEM2(idandnodeoff, mOffset, PreloadPackage::RuntimeDataDialog::DlgObjIdAndStartNodeOffset, startnodeoff, 0, mID);
			ADD(idandnodeoff);

			DEFINESET_(Symbol SEP Symbol::CompareCRC, symbol);

			DEFINET2(seen, PreloadPackage::ResourceSeenTimes);
			FIRSTMEM2(seen, mfEarliest, PreloadPackage::ResourceSeenTimes, float, 0);
			NEXTMEM2(seen, mfLatest, PreloadPackage::ResourceSeenTimes, float, 0, mfEarliest);
			NEXTMEM2(seen, mAdditionalScenes, PreloadPackage::ResourceSeenTimes, Set_symbol,0, mfLatest);
			ADD(seen);

			DEFINET2(bitsetbase1, BitSetBase<1>);
			SERIALIZER(bitsetbase1, BitSetBase<1>);
			ADD(bitsetbase1);

			DEFINET2(reskey, PreloadPackage::ResourceKey);
			FIRSTMEM2(reskey, mResourceName, PreloadPackage::ResourceKey, symbol, 0);
			NEXTMEM2(reskey, mMetaClassDescriptionCrc, PreloadPackage::ResourceKey, __int64, 0, mResourceName);
			//NEWER GAMES
			NEXTMEM4(reskey, mRenderQualities, PreloadPackage::ResourceKey, bitsetbase1, 0x20, mMetaClassDescriptionCrc
				,TelltaleToolLib_GetGameKeyIndex("BATMAN"),-1);
			NEXTMEM4(reskey, mVisible, PreloadPackage::ResourceKey, bool, 
				0, mRenderQualities, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			NEXTMEM4(reskey, mPrefix, PreloadPackage::ResourceKey, string,
				0, mVisible, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			//--
			ADD(reskey);

			DEFINEDCARRAY2(PreloadPackage::ResourceKey, reskey);

			DEFINET2(ppkgs, PreloadPackage::RuntimeDataScene);
			EXT(ppkgs, preloadpackagerts);
			FIRSTMEM2(ppkgs, mResources, PreloadPackage::RuntimeDataScene, DCArray_reskey, 0);
			ADD(ppkgs);

			DEFINET2(dlgresinf, PreloadPackage::RuntimeDataDialog::DialogResourceInfo);
			FIRSTMEM2(dlgresinf, mResourceKey, PreloadPackage::RuntimeDataDialog::DialogResourceInfo, reskey, 0);
			NEXTMEM2(dlgresinf, mResourceSeenTimes, PreloadPackage::RuntimeDataDialog::DialogResourceInfo, seen, 0, mResourceKey);
			ADD(dlgresinf);

			DEFINEDCARRAY2(PreloadPackage::RuntimeDataDialog::DialogResourceInfo, resinfodlg);

			DEFINET2(idandvec, PreloadPackage::RuntimeDataDialog::DlgObjIdAndResourceVector);
			FIRSTMEM2(idandvec, mID, PreloadPackage::RuntimeDataDialog::DlgObjIdAndResourceVector, dlgid, 0);
			NEXTMEM2(idandvec, mVector, PreloadPackage::RuntimeDataDialog::DlgObjIdAndResourceVector, DCArray_resinfodlg, 0, mID);
			ADD(idandvec);

			DEFINEDCARRAY2(PreloadPackage::RuntimeDataDialog::DlgObjIdAndResourceVector, resvec);
			DEFINEDCARRAY2(PreloadPackage::RuntimeDataDialog::DlgObjIdAndStartNodeOffset, startnodeoffandid);

			DEFINET2(ppkgd, PreloadPackage::RuntimeDataDialog);
			EXT(ppkgd, preloadpackagertd);
			FIRSTMEM2(ppkgd, mDialogResourceVectors, PreloadPackage::RuntimeDataDialog, DCArray_resvec, 0);
			NEXTMEM2(ppkgd, mStartNodeOffsets, PreloadPackage::RuntimeDataDialog, DCArray_startnodeoffandid, 0, mDialogResourceVectors);
			ADD(ppkgd);

			DEFINET2(sounddata, SoundData);
			EXT(sounddata, wav);
			ADD(sounddata);

			DEFINET2(pathbase, PathBase);
			ADDFLAGS(pathbase, MetaFlag::MetaFlag_Memberless);
			ADD(pathbase);

			//DEFINET2(walkpath, WalkPath);
			//SERIALIZER(walkpath, WalkPath);
			//FIRSTMEM2(walkpath, mName, WalkPath, string, 0);
			//ADD(walkpath);

			DEFINEMAP(Symbol, bool, Symbol::CompareCRC);

			DEFINET2(litem, LogicGroup::LogicItem);
			FIRSTMEM(litem, "Baseclass_PropertySet", mPropVersion,LogicGroup::LogicItem, prop, 0);
			NEXTMEM2(litem, mName, LogicGroup::LogicItem, string, 0, mPropVersion);
			NEXTMEM2(litem, mKeyNegateList, LogicGroup::LogicItem, Map_Symbol_bool, 0, mName);
			NEXTMEM2(litem, mKeyComparisonList, LogicGroup::LogicItem, Map_Symbol_int, 0, mKeyNegateList);
			NEXTMEM2(litem, mKeyActionList , LogicGroup::LogicItem, Map_Symbol_int, 0, mKeyComparisonList);
			NEXTMEM2(litem, mReferenceKeyList, LogicGroup::LogicItem, DCArray_String, 0, mKeyActionList);
			ADD(litem);

			DEFINEDCARRAY(LogicGroup);
			DEFINEMAP2(String, LogicGroup::LogicItem, string, logicitem, std::less<String>);

			DEFINET2(lgroup, LogicGroup);
			FIRSTMEM2(lgroup, mOperator, LogicGroup, long, 0);
			NEXTMEM2(lgroup, mItems, LogicGroup, Map_string_logicitem, 0, mOperator);
			NEXTMEM2(lgroup, mLogicGroups, LogicGroup,DCArray_LogicGroup, 0, mItems);
			NEXTMEM2(lgroup, mGroupOperator, LogicGroup, long, 0, mLogicGroups);
			NEXTMEM2(lgroup, mType, LogicGroup, long, 0, mGroupOperator);
			NEXTMEM2(lgroup, mName, LogicGroup, string, 0, mType);
			ADD(lgroup);

			DEFINET2(rule, Rule);
			ADDFLAGS(rule, MetaFlag::MetaFlag_ScriptTransient | MetaFlag::MetaFlag_PlaceInAddPropMenu);
			SERIALIZER(rule, Rule);
			FIRSTMEM2(rule, mName, Rule, string, 0);
			NEXTMEM2(rule, mRuntimePropName, Rule, string, 0, mName);
			NEXTMEM2(rule, mFlags, Rule, flags, 0, mRuntimePropName);
			NEXTMEM2(rule, mConditions, Rule, lgroup, 0, mFlags);
			NEXTMEM2(rule, mActions, Rule, lgroup, 0, mConditions);
			NEXTMEM2(rule, mElse, Rule, lgroup, 0, mActions);
			NEXTMEM2(rule, mAgentCategory, Rule, string, 0, mElse);
			ADD(rule);

			DEFINEMAP2(String, Rule*, string, ruleptr, std::less<String>);

			DEFINET2(rules, Rules);
			SERIALIZER(rules, Rules);
			EXT(rules, rules);
			FIRSTMEM2(rules, mFlags, Rules, flags, 0);
			NEXTMEM2(rules, mhLogicProps, Rules, Handlepropset, 0, mFlags);
			NEXTMEM2(rules, mRuleMap, Rules, Map_string_ruleptr, MetaFlag::MetaFlag_MetaSerializeDisable, mhLogicProps);
			ADD(rules);

			DEFINET2(eloge, EventLoggerEvent);
			FIRSTMEM2(eloge, mEventID, EventLoggerEvent, long, 0);
			NEXTMEM2(eloge, mMaxSeverity, EventLoggerEvent, long, 0, mEventID);
			ADD(eloge);

			DEFINET2(epage, EventStoragePage);
			SERIALIZER(epage, EventStoragePage);
			EXT(epage, epage);
			FIRSTMEM2(epage, mVersion, EventStoragePage, long, 0);
			NEXTMEM2(epage, mSessionID,EventStoragePage, __int64, 0, mVersion);
			NEXTMEM4(epage, mFlushedNameOnDisk, EventStoragePage, string, 0, mSessionID, TelltaleToolLib_GetGameKeyIndex("BORDERLANDS"), -1);
			ADD(epage);

			DEFINEDCARRAY2(EventStorage::PageEntry, estoreentry);
			DEFINEHANDLE(estorepage, EventStoragePage);

			DEFINET2(estoree, EventStorage::PageEntry);
			ADDFLAGS(estoree, MetaFlag::MetaFlag_Handle | MetaFlag::MetaFlag_PlaceInAddPropMenu);
			FIRSTMEM2(estoree, mhPage, EventStorage::PageEntry, Handleestorepage, 0);
			NEXTMEM2(estoree, mMaxEventID, EventStorage::PageEntry, long, 0, mhPage);
			ADD(estoree);

			DEFINET2(estore, EventStorage);
			EXT(estore, estore);
			SERIALIZER(estore, EventStorage);
			FIRSTMEM2(estore, mVersion, EventStorage, long, 0);
			NEXTMEM2(estore, mSessionID, EventStorage, __int64, 0, mVersion);
			NEXTMEM2(estore, mPages, EventStorage, DCArray_estoreentry, 0, mSessionID);
			NEXTMEM2(estore, mName, EventStorage, string, 0, mPages);
			NEXTMEM2(estore, mLastEventID, EventStorage, long, 0, mName);
			NEXTMEM2(estore, mEventStoragePageSize, EventStorage, long, 0, mLastEventID);
			ADD(estore);

			DEFINET2(pentry, PhonemeTable::PhonemeEntry);
			FIRSTMEM2(pentry, mAnimation, PhonemeTable::PhonemeEntry, animorchore, 0);
			NEXTMEM2(pentry, mContributionScalar, PhonemeTable::PhonemeEntry, float, 0, mAnimation);
			NEXTMEM2(pentry, mTimeScalar, PhonemeTable::PhonemeEntry, float, 0, mContributionScalar);
			ADD(pentry);

			DEFINEMAP2(Symbol, PhonemeTable::PhonemeEntry, symbol, pentry, Symbol::CompareCRC);

			DEFINET2(ptable, PhonemeTable);
			EXT(ptable, ptable);
			SERIALIZER(ptable, PhonemeTable);
			FIRSTMEM2(ptable, mName, PhonemeTable, string, 0);
			NEXTMEM2(ptable, mContributionScaler, PhonemeTable, float, 0, mName);
			NEXTMEM2(ptable, mAnimations, PhonemeTable, Map_symbol_pentry, 0, mContributionScaler);
			ADD(ptable);

			DEFINET2(cstate, Procedural_LookAt::EnumLookAtComputeStage);
			FIRSTMEM(cstate, "Baseclass_EnumBase", mVal, 
				Procedural_LookAt::EnumLookAtComputeStage, enumbase, 0);
			NEXTMEM1(cstate, "mVal", ALAIS,mVal,Procedural_LookAt::EnumLookAtComputeStage,
				long, 0, mVal);
			FIRSTENUM2(cstate, ALAIS, "Idle Look At", idlelook, 0,0);
			NEXTENUM2(cstate, ALAIS, "Dialog Chore Look At", dlglook, 1, 0, idlelook);
			NEXTENUM2(cstate, ALAIS, "Final Look At", flook, 2, 0, dlglook);
			ADD(cstate);

			DEFINET2(lookat, Procedural_LookAt);
			ADDFLAGS(lookat, MetaFlag::MetaFlag_Memberless);
			EXT(lookat, look);
			FIRSTMEM2(lookat, mHostNode, Procedural_LookAt, string, 1);
			NEXTMEM2(lookat, mTargetAgent, Procedural_LookAt, string, 1, mHostNode);
			NEXTMEM2(lookat, mTargetNode, Procedural_LookAt, string, 1, mTargetAgent);
			NEXTMEM2(lookat, mTargetOffset, Procedural_LookAt, vec3, 1, mTargetNode);
			NEXTMEM2(lookat, mbUsePrivateNode, Procedural_LookAt, bool, 1, mTargetOffset);
			NEXTMEM2(lookat, mhXAxisChore, Procedural_LookAt, animorchore, 1, mbUsePrivateNode);
			NEXTMEM2(lookat, mhYAxisChore, Procedural_LookAt, animorchore, 1, mhXAxisChore);
			NEXTMEM2(lookat, mbRotateHostNode, Procedural_LookAt, bool, 1, mhYAxisChore);
			NEXTMEM2(lookat, mLastLRAngle, Procedural_LookAt, float, 1, mbRotateHostNode);
			NEXTMEM2(lookat, mLastUDAngle, Procedural_LookAt, float, 1, mLastLRAngle);
			NEXTMEM2(lookat, mLastLRWeight, Procedural_LookAt, float, 1, mLastUDAngle);
			NEXTMEM2(lookat,mLastUDWeight, Procedural_LookAt, float, 1, mLastLRWeight);
			NEXTMEM2(lookat, mLookAtComputeStage, Procedural_LookAt, cstate, 1,mLastUDWeight);
			ADD(lookat);

			DEFINET2(pbase, PathBase);
			ADDFLAGS(pbase, MetaFlag::MetaFlag_Memberless);
			ADD(pbase);

			DEFINET2(pseg, PathSegment);
			FIRSTMEM(pseg, "Baseclass_PathBase", mStart, PathSegment, pbase, 0x10);
			NEXTMEM1(pseg, "mStart", ALAIS, mStart, PathSegment, vec3, 0, mStart);
			NEXTMEM2(pseg, mEnd, PathSegment, vec3, 0, ALAIS);
			NEXTMEM2(pseg, mStartNodeId, PathSegment, long, 0, mEnd);
			NEXTMEM2(pseg, mEndNodeId, PathSegment, long, 0, mStartNodeId);
			SERIALIZER(pseg, PathSegment);
			ADD(pseg);

			DEFINET2(psegh, HermiteCurvePathSegment);
			FIRSTMEM(psegh, "Baseclass_PathBase", mStart, HermiteCurvePathSegment, pbase, 0x10);
			NEXTMEM1(psegh, "mStart", ALAIS, mStart, HermiteCurvePathSegment, vec3, 0, mStart);
			NEXTMEM2(psegh, mEnd, HermiteCurvePathSegment, vec3, 0, ALAIS);
			NEXTMEM2(psegh, mStartDir, HermiteCurvePathSegment, vec3, 0, mEnd);
			NEXTMEM2(psegh, mEndDir, HermiteCurvePathSegment, vec3, 0, mStartDir);
			NEXTMEM2(psegh, mStartNodeId, HermiteCurvePathSegment, long, 0, mEndDir);
			NEXTMEM2(psegh, mEndNodeId, HermiteCurvePathSegment, long, 0, mStartNodeId);
			SERIALIZER(psegh, HermiteCurvePathSegment);
			ADD(psegh);

			DEFINET2(cstate1, AnimationDrivenPathSegment::EnumAnimatedPathSegmentType);
			FIRSTMEM(cstate1, "Baseclass_EnumBase", mVal,
				AnimationDrivenPathSegment::EnumAnimatedPathSegmentType, enumbase, 0);
			NEXTMEM1(cstate1, "mVal", ALAIS, mVal, AnimationDrivenPathSegment::EnumAnimatedPathSegmentType,
				long, 0, mVal);
			FIRSTENUM2(cstate1, ALAIS, "eStartType", idlelook, 0, 0);
			NEXTENUM2(cstate1, ALAIS, "eLoopType", dlglook, 1, 0, idlelook);
			NEXTENUM2(cstate1, ALAIS, "eStopType", flook, 2, 0, dlglook);
			ADD(cstate1);

			DEFINET2(psegha, AnimationDrivenPathSegment);
			FIRSTMEM(psegha, "Baseclass_PathBase", mStart, AnimationDrivenPathSegment, pbase, 0x10);
			NEXTMEM1(psegha, "mStart", ALAIS, mStart, AnimationDrivenPathSegment, vec3, 0, mStart);
			NEXTMEM2(psegha, mEnd, AnimationDrivenPathSegment, vec3, 0, ALAIS);
			NEXTMEM2(psegha, mStartDirection, AnimationDrivenPathSegment, vec3, 0, mEnd);
			NEXTMEM2(psegha, mEndDirection, AnimationDrivenPathSegment, vec3, 0, mStartDirection);
			NEXTMEM2(psegha, mAnimType, AnimationDrivenPathSegment, cstate1, 0, mEndDirection);
			SERIALIZER(psegha, AnimationDrivenPathSegment);
			ADD(psegha);

			DEFINET2(wpath, WalkPath);
			FIRSTMEM2(wpath, mName, WalkPath, string, 0);
			SERIALIZER(wpath, WalkPath);
			ADD(wpath);

			DEFINET2(dloader, DependencyLoader<1>);
			SERIALIZER(dloader, DependencyLoader<1>);
			ADDFLAGS(dloader, MetaFlag::MetaFlag_Memberless);
			ADD(dloader);

			DEFINET2(aas, AutoActStatus);
			FIRSTMEM2(aas, m_Status, AutoActStatus, 
				long, MetaFlag::MetaFlag_EnumIntType);
			ADD(aas);

			DEFINET2(aab, ActorAgentBinding);
			FIRSTMEM2(aab, mActorName, ActorAgentBinding, string, 0);
			ADD(aab);

			DEFINEMAP(Symbol, WalkPath, Symbol::CompareCRC);

			DEFINET2(chore, Chore);
			EXT(chore, chore);
			SERIALIZER(chore, Chore);
			FIRSTMEM2(chore, mName, Chore, string, 0);
			NEXTMEM2(chore, mFlags, Chore, flags, 0, mName);
			NEXTMEM2(chore, mLength, Chore, float, 0, mFlags);
			NEXTMEM2(chore, mNumResources, Chore, long, 0, mLength);
			NEXTMEM2(chore, mNumAgents, Chore, long, 0, mNumResources);
			NEXTMEM2(chore, mEditorProps, Chore, prop, 
				MetaFlag::MetaFlag_SkipObjectState, mNumAgents);
			NEXTMEM2(chore, mChoreSceneFile, Chore, string, 0, mEditorProps);
			NEXTMEM2(chore, mRenderDelay, Chore, long, MetaFlag::MetaFlag_SkipObjectState,
				mChoreSceneFile);
			NEXTMEM2(chore, mSynchronizedToLocalization, Chore, locali, 0, mRenderDelay);
			NEXTMEM2(chore, mDependencies, Chore, dloader,
				MetaFlag::MetaFlag_SkipObjectState, mSynchronizedToLocalization);
			NEXTMEM2(chore, mToolProps, Chore, tp, MetaFlag::MetaFlag_SkipObjectState
				| MetaFlag::MetaFlag_EditorHide, mDependencies);
			NEXTMEM2(chore, mWalkPaths, Chore, Map_Symbol_WalkPath, 0, mToolProps);
			ADD(chore);

			DEFINET2(cattach, ChoreAgent::Attachment);
			FIRSTMEM2(cattach, mbDoAttach, ChoreAgent::Attachment, bool, 0);
			NEXTMEM2(cattach, mAttachTo, ChoreAgent::Attachment, string, MetaFlag::MetaFlag_SelectAgentType,mbDoAttach);
			NEXTMEM2(cattach, mAttachToNode, ChoreAgent::Attachment, string, 0, mAttachTo);
			NEXTMEM2(cattach, mAttachPos, ChoreAgent::Attachment, vec3, 0, mAttachToNode);
			NEXTMEM2(cattach, mAttachQuat, ChoreAgent::Attachment, quat, 0, mAttachPos);
			NEXTMEM2(cattach, mbAttachPreserveWorldPos, ChoreAgent::Attachment, bool, 0, mAttachQuat);
			NEXTMEM2(cattach, mbLeaveAttachedWhenComplete, ChoreAgent::Attachment, bool, 0, mbAttachPreserveWorldPos);
			ADD(cattach);

			DEFINET2(cagent, ChoreAgent);
			SERIALIZER(cagent, ChoreAgent);
			FIRSTMEM2(cagent, mpChore, ChoreAgent, ptrbase, 0);
			NEXTMEM2(cagent, mAgentName, ChoreAgent, string, 0, mpChore);
			NEXTMEM2(cagent, mFlags, ChoreAgent, flags, 0, mAgentName);
			NEXTMEM2(cagent, mResources, ChoreAgent, DCArray_i32, 0, mFlags);
			NEXTMEM2(cagent, mAttachment, ChoreAgent, cattach, 0, mResources);
			NEXTMEM2(cagent, mAABinding, ChoreAgent, aab, 0, mAttachment);
			NEXTMEM2(cagent, mAgentEnabledRule, ChoreAgent, rule, 0, mAABinding);
			ADD(cagent);

			DEFINET2(cblock, ChoreResource::Block);
			FIRSTMEM2(cblock, mStartTime, ChoreResource::Block, float, 0);
			NEXTMEM2(cblock, mEndTime, ChoreResource::Block, float, 0, mStartTime);
			NEXTMEM2(cblock, mbLoopingBlock, ChoreResource::Block, bool, 0, mEndTime);
			NEXTMEM2(cblock, mScale, ChoreResource::Block, float, 0, mbLoopingBlock);
			NEXTMEM2(cblock, mbSelected, ChoreResource::Block, bool, 1, mScale);
			ADD(cblock);

			DEFINEDCARRAY2(ChoreResource::Block, choreblock);

			DEFINEDCARRAY2(AnimationValueInterfaceBase*, anminterfacebase);

			DEFINET2(anm, Animation);
			EXT(anm, anm);
			SERIALIZER(anm, Animation);
			FIRSTMEM2(anm, mVersion, Animation, long, 0);
			NEXTMEM2(anm, mFlags, Animation, long, 0, mVersion);
			NEXTMEM2(anm, mName, Animation, symbol, 0, mFlags);
			NEXTMEM2(anm, mLength, Animation, float, 0, mName);
			NEXTMEM2(anm, mAdditiveMask, Animation, float, 0, mLength);
			NEXTMEM2(anm, mValues, Animation, DCArray_anminterfacebase, MetaFlag::MetaFlag_MetaSerializeDisable, mAdditiveMask);
			NEXTMEM2(anm, mToolProps, Animation, tp, MetaFlag::MetaFlag_EditorHide | MetaFlag::MetaFlag_SkipObjectState, mValues);
			ADD(anm);

			DEFINET2(cres, ChoreResource);
			SERIALIZER(cres, ChoreResource);
			FIRSTMEM2(cres, mpChore, ChoreResource, ptrbase, 0);
			NEXTMEM2(cres, mVersion, ChoreResource, long, 0, mpChore);
			NEXTMEM2(cres, mResName, ChoreResource, symbol, 0, mVersion);
			NEXTMEM2(cres, mResLength, ChoreResource, float, 0, mResName);
			NEXTMEM2(cres, mPriority, ChoreResource, long, 0, mResLength);
			NEXTMEM2(cres, mFlags, ChoreResource, flags, 0, mPriority);
			NEXTMEM2(cres, mResourceGroup, ChoreResource, string, 0, mFlags);
			NEXTMEM2(cres, mhObject, ChoreResource, handlebase, 0, mResourceGroup);
			NEXTMEM2(cres, mControlAnimation, ChoreResource, anm, 0, mhObject);
			NEXTMEM2(cres, mBlocks, ChoreResource, DCArray_choreblock, 0, mControlAnimation);
			NEXTMEM2(cres, mbNoPose, ChoreResource, bool, 0, mBlocks);
			NEXTMEM2(cres, mbEmbedded, ChoreResource, bool, 0, mbNoPose);
			NEXTMEM2(cres, mbEnabled, ChoreResource, bool, 0, mbEmbedded);
			NEXTMEM2(cres, mbIsAgentResource, ChoreResource, bool, 0, mbEnabled);
			NEXTMEM2(cres, mbViewGraphs, ChoreResource, bool,0, mbIsAgentResource);
			NEXTMEM2(cres, mbViewEmptyGraphs, ChoreResource, bool, 0, mbViewGraphs);
			NEXTMEM2(cres, mbViewProperties, ChoreResource, bool, 0, mbViewEmptyGraphs);
			NEXTMEM2(cres, mbViewResourceGroups, ChoreResource, bool, 0, mbViewProperties);
			NEXTMEM2(cres, mResourceProperties, ChoreResource, prop, 0, mbViewResourceGroups);
			NEXTMEM2(cres, mResourceGroupInclude, ChoreResource, Map_Symbol_float, 0, mResourceProperties);
			NEXTMEM2(cres, mAAStatus, ChoreResource, aas, 0, mResourceGroupInclude);
			NEXTMEM1(cres, "mAAStatus", ALIAS, mAAStatus,
				ChoreResource, long, 0, mAAStatus);
			meta_cres_ALIAS.mGameIndexVersionRange.max = 
				TelltaleToolLib_GetGameKeyIndex("MCSM");
			meta_cres_mAAStatus.mGameIndexVersionRange.min = 
				TelltaleToolLib_GetGameKeyIndex("MICHONNE");
			ADD(cres);

			DEFINET2(pkey, PhonemeKey);
			FIRSTMEM2(pkey, mPhoneme, PhonemeKey, symbol, 0);
			NEXTMEM2(pkey, mFadeInTime, PhonemeKey, float, 0, mPhoneme);
			NEXTMEM2(pkey, mHoldTime, PhonemeKey, float, 0, mFadeInTime);
			NEXTMEM2(pkey, mFadeOutTime, PhonemeKey, float, 0, mHoldTime);
			NEXTMEM2(pkey, mTargetContribution, PhonemeKey, float, 0, mFadeOutTime);
			SERIALIZER(pkey, PhonemeKey);
			ADD(pkey);

			DEFINEKEYFRAMEDVALUE(Vector3,Vector3,vec3);
			DEFINEKEYFRAMEDVALUE(Transform, Transform, transform);
			DEFINEKEYFRAMEDVALUE(bool, bool, bool);
			DEFINEKEYFRAMEDVALUE(String, String, string);
			DEFINEKEYFRAMEDVALUE(PhonemeKey, PhonemeKey, pkey);

			if (meta_kfv_String_sample.mbNameIsHeapAllocated) {
				free((void*)meta_kfv_String_sample.mpTypeInfoName);
				meta_kfv_String_sample.mbNameIsHeapAllocated = false;
			}
			meta_kfv_String_sample.mpTypeInfoName = "KeyframedValue<String>::Sample";//typedef string
			meta_kfv_String_sample.mHash = CRC64_CaseInsensitive(0, 
				meta_kfv_String_sample.mpTypeInfoName);
			
			if (meta_anmi_String.mbNameIsHeapAllocated) {
				free((void*)meta_anmi_String.mpTypeInfoName);
				meta_anmi_String.mbNameIsHeapAllocated = false;
			}
			meta_anmi_String.mpTypeInfoName = "AnimatedValueInterface<String>";//typedef string
			meta_anmi_String.mHash = CRC64_CaseInsensitive(0, meta_anmi_String.mpTypeInfoName);


			if (meta_kfv_String.mbNameIsHeapAllocated) {
				free((void*)meta_kfv_String.mpTypeInfoName);
				meta_kfv_String.mbNameIsHeapAllocated = false;
			}
			meta_kfv_String.mpTypeInfoName = "KeyframedValue<String>";//typedef string
			meta_kfv_String.mHash = CRC64_CaseInsensitive(0, meta_kfv_String.mpTypeInfoName);
			DEFINET2(spose, SkeletonPose);
			ADD(spose);

			DEFINET2(sklk, CompressedSkeletonPoseKeys);
			FIRSTMEM(sklk, "Baseclass_AnimationValueInterfaceBase", mName, 
				CompressedSkeletonPoseKeys, anminterface, MetaFlag::MetaFlag_BaseClass);
			meta_sklk_mName.mOffset = (i64)((AnimationValueInterfaceBase*)((CompressedSkeletonPoseKeys*)NULL));
			NEXTMEM2(sklk, mDataSize, CompressedSkeletonPoseKeys, long, 0, mName);
			ADD(sklk);

			DEFINET2(sklk2, CompressedSkeletonPoseKeys2);
			FIRSTMEM(sklk2, "Baseclass_AnimationValueInterfaceBase", mName,
				CompressedSkeletonPoseKeys2, anminterface, MetaFlag::MetaFlag_BaseClass);
			meta_sklk2_mName.mOffset = (i64)((AnimationValueInterfaceBase*)((CompressedSkeletonPoseKeys2*)NULL));
			NEXTMEM2(sklk2, mDataSize, CompressedSkeletonPoseKeys2, long, 0, mName);
			ADD(sklk2);

			DEFINET2(sbentry, SkeletonPoseValue::BoneEntry);
			FIRSTMEM2(sbentry, mName, SkeletonPoseValue::BoneEntry, symbol, 0);
			NEXTMEM2(sbentry, mFlags, SkeletonPoseValue::BoneEntry, long, 0, mName);
			ADD(sbentry);
			
			DEFINEDCARRAY(Transform);

			DEFINET2(sample, SkeletonPoseValue::Sample);
			FIRSTMEM2(sample, mTime, SkeletonPoseValue::Sample, float, 0);
			NEXTMEM2(sample, mRecipTimeToNextSample, SkeletonPoseValue::Sample,
				float, 0, mTime);
			NEXTMEM2(sample, mValues, SkeletonPoseValue::Sample, DCArray_Transform,
				0, mRecipTimeToNextSample);
			NEXTMEM2(sample, mTangents, SkeletonPoseValue::Sample, DCArray_i32, 0,
				mValues);
			ADD(sample);

			DEFINEDCARRAY2(SkeletonPoseValue::Sample, sample);
			DEFINEDCARRAY2(SkeletonPoseValue::BoneEntry, bentry);

			DEFINET2(pose, SkeletonPoseValue);
			FIRSTMEM(pose, "Baseclass_AnimationValueInterfaceBase", mName,
				SkeletonPoseValue , anminterface, MetaFlag::MetaFlag_BaseClass);
			meta_pose_mName.mOffset = (i64)((AnimationValueInterfaceBase*)
				((SkeletonPoseValue*)NULL));
			NEXTMEM2(pose, mBones, SkeletonPoseValue, DCArray_bentry, 0, mName);
			NEXTMEM2(pose, mSamples, SkeletonPoseValue, DCArray_sample, 0, mBones);
			ADD(pose);

			DEFINET2(dlgidowner, DlgObjIDOwner);
			FIRSTMEM2(dlgidowner, mDlgObjID, DlgObjIDOwner, dlgid, 0x20);
			ADD(dlgidowner);

			DEFINET2(dlgprops, DlgObjectProps);
			ADDFLAGS(dlgprops, MetaFlag::MetaFlag_NoPanelCaption);
			SERIALIZER(dlgprops, DlgObjectProps);
			FIRSTMEM2(dlgprops, mFlags, DlgObjectProps, flags, 0x20);
			ADD(dlgprops);

			DEFINET2(dlgpropo, DlgObjectPropsOwner);
			FIRSTMEM2(dlgpropo, mDlgObjectProps, DlgObjectPropsOwner, dlgprops, 8);
			ADD(dlgpropo);

			DEFINET2(dlglink, DlgNodeLink);
			FIRSTMEM(dlglink, "Baseclass_DlgObjIDOwner", mDlgObjID, DlgNodeLink, dlgidowner, 0x10);
			meta_dlglink_mDlgObjID.mOffset = (i64)dynamic_cast<DlgObjIDOwner*>((DlgNodeLink*)NULL);
			NEXTMEM2(dlglink, mRequiredCCType, DlgNodeLink, long, 0, mDlgObjID);
			ADD(dlglink);

			DEFINET2(dlghead, DlgChainHead);
			FIRSTMEM(dlghead, "Baseclass_DlgObjIDOwner", mDlgObjID, DlgChainHead, dlgidowner, 0x10);
			meta_dlghead_mDlgObjID.mOffset = (i64)dynamic_cast<DlgObjIDOwner*>((DlgChainHead*)NULL);
			NEXTMEM2(dlghead, mLink, DlgChainHead, dlglink, 0, mDlgObjID);
			ADD(dlghead);

			DEFINET2(dlgdown, DlgDownstreamVisibilityConditions);
			FIRSTMEM2(dlgdown, mNodeTypeFlags, DlgDownstreamVisibilityConditions, flags, 0);
			NEXTMEM2(dlgdown, mMaxNumNodeEvals, DlgDownstreamVisibilityConditions, long, 0, mNodeTypeFlags);
			ADD(dlgdown);

			DEFINET2(dlgvcond, DlgVisibilityConditions);
			FIRSTMEM2(dlgvcond, mbDiesOff, DlgVisibilityConditions, bool, 0);
			NEXTMEM2(dlgvcond, mFlags, DlgVisibilityConditions, flags, 0, mbDiesOff);
			NEXTMEM2(dlgvcond, mDownstreamVisCond, DlgVisibilityConditions,dlgdown, 0, mFlags);
			NEXTMEM2(dlgvcond, mScriptVisCond, DlgVisibilityConditions, string, 0, mDownstreamVisCond);
			ADD(dlgvcond);

			DEFINET2(dlgvowner, DlgVisibilityConditionsOwner);
			ADDFLAGS(dlgvowner, 8);
			FIRSTMEM2(dlgvowner, mVisCond, DlgVisibilityConditionsOwner, dlgvcond, 0);
			ADD(dlgvowner);

			DEFINET2(dlgchild, DlgChild);
			ADDFLAGS(dlgchild, 8);
			FIRSTMEM(dlgchild, "Baseclass_DlgChainHead", mLink, DlgChainHead, dlghead, 0x10);
			NEXTMEM2(dlgchild, mName, DlgChild, symbol, 0, mLink);
			NEXTMEM(dlgchild, "Baseclass_DlgVisibilityConditionsOwner", mVisCond, DlgChild, dlgvowner, 0x10, mName);
			NEXTMEM(dlgchild, "Baseclass_DlgObjectPropsOwner", 
				mDlgObjectProps, DlgChild, dlgpropo, 0x10, mVisCond);
			NEXTMEM2(dlgchild, mParent, DlgChild, dlglink, 0x20, mDlgObjectProps);
			ADD(dlgchild);

			DEFINEDCARRAY2(DlgChild*, childptr);

			DEFINET2(dlgchildset, DlgChildSet);
			FIRSTMEM2(dlgchildset, mChildren, DlgChildSet, DCArray_childptr, 0x20);
			NEXTMEM2(dlgchildset, mParent, DlgChildSet, dlglink, 0x20, mChildren);
			SERIALIZER(dlgchildset, DlgChildSet);
			ADD(dlgchildset);

			DEFINET2(dfolder, DlgFolder);
			ADDFLAGS(dfolder, 8);
			FIRSTMEM(dfolder, "Baseclass_DlgObjIDOwner", mDlgObjID, DlgFolder, dlgidowner, 0x30);
			//meta_dfolder_mDlgObjID.mOffset = (i64)dynamic_cast<DlgObjIDOwner*>((DlgFolder*)NULL);
			NEXTMEM(dfolder, "Baseclass_DlgObjectPropsOwner", mDlgObjectProps, DlgFolder, dlgpropo, 0x10, mDlgObjID);
			//meta_dfolder_mDlgObjectProps.mOffset = (i64)dynamic_cast<DlgObjectPropsOwner*>((DlgFolder*)NULL);
			NEXTMEM(dfolder, "Baseclass_DlgChildSet", mChildren, DlgFolder, dlgchildset, 0x10, mDlgObjectProps);
			//meta_dfolder_mChildren.mOffset = (i64)dynamic_cast<DlgChildSet*>((DlgFolder*)NULL);
			NEXTMEM(dfolder, "Baseclass_UID::Owner", miUniqueID, DlgFolder, uidowner, 0x10, mChildren);
			//meta_dfolder_miUniqueID.mOffset = (i64)dynamic_cast<UID::Owner*>((DlgFolder*)NULL);
			meta_dfolder_miUniqueID.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			NEXTMEM2(dfolder, mName, DlgFolder, symbol, 0, miUniqueID);
			NEXTMEM2(dfolder, mProdReportProps, DlgFolder, prop, 0, mName);
			ADD(dfolder);

			DEFINET2(dnode, DlgNode);
			ADDFLAGS(dnode, 8);
			FIRSTMEM(dnode, "Baseclass_DlgObjIDOwner", mDlgObjID, DlgNode, dlgidowner, 0x30);
			//meta_dnode_mDlgObjID.mOffset = (i64)dynamic_cast<DlgObjIDOwner*>((DlgNode*)NULL);
			NEXTMEM(dnode, "Baseclass_DlgVisibilityConditionsOwner", mVisCond, DlgNode, dlgvowner, 0x10, mDlgObjID);
			//meta_dnode_mVisCond.mOffset = (i64)dynamic_cast<DlgVisibilityConditionsOwner*>((DlgNode*)NULL);
			NEXTMEM(dnode, "Baseclass_DlgObjectPropsOwner", mDlgObjectProps, DlgNode, dlgpropo, 0x10, mVisCond);
			//meta_dnode_mDlgObjectProps.mOffset = (i64)dynamic_cast<DlgObjectPropsOwner*>((DlgNode*)NULL);
			NEXTMEM(dnode, "Baseclass_UID::Owner", miUniqueID, DlgNode, uidowner, 0x10, mDlgObjID);
			//meta_dnode_miUniqueID.mOffset = (i64)dynamic_cast<UID::Owner*>((DlgNode*)NULL);
			meta_dnode_miUniqueID.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			NEXTMEM2(dnode, mPrev, DlgNode, dlglink, 0x20, miUniqueID);
			NEXTMEM2(dnode, mNext, DlgNode, dlglink, 0x20, mPrev);
			NEXTMEM2(dnode, mName, DlgNode, string, 0x20, mNext);
			NEXTMEM2(dnode, mFlags, DlgNode, flags, 0x20, mName);
			NEXTMEM2(dnode, mChainContextTypeID, DlgNode, long, 0x20, mFlags);
			ADD(dnode);

			DEFINET2(date, DateStamp);
			FIRSTMEM2(date, mSec, DateStamp, __uint8, 0);
			NEXTMEM2(date, mMin, DateStamp, __uint8, 0, mSec);
			NEXTMEM2(date, mHour, DateStamp, __uint8, 0, mMin);
			NEXTMEM2(date, mMday, DateStamp, __uint8, 0, mHour);
			NEXTMEM2(date, mMon, DateStamp, __uint8, 0, mMday);
			NEXTMEM2(date, mYear, DateStamp, __uint8, 0, mMon);
			NEXTMEM2(date, mWday, DateStamp, __uint8, 0, mYear);
			NEXTMEM2(date, mYday, DateStamp, ushort, 0, mWday);
			NEXTMEM2(date, mIsdst, DateStamp, __uint8, 0, mYday);
			ADD(date);

			DEFINET2(notee, Note::Entry);
			FIRSTMEM(notee, "Baseclass_UID::Owner", miUniqueID, Note::Entry, uidowner, 0x10);
			meta_notee_miUniqueID.mOffset = (i64)dynamic_cast<UID::Owner*>((Note::Entry*)NULL);
			NEXTMEM(notee, "Baseclass_DlgObjIDOwner", mDlgObjID, Note::Entry, dlgidowner, 0x10, miUniqueID);
			meta_notee_mDlgObjID.mOffset = (i64)dynamic_cast<DlgObjIDOwner*>((Note::Entry*)NULL);
			NEXTMEM2(notee, mAuthor, Note::Entry, string, 0, mDlgObjID);
			NEXTMEM2(notee, mStamp, Note::Entry, date, 0, mAuthor);
			NEXTMEM2(notee, mCategory, Note::Entry, string, 0, mStamp);
			NEXTMEM2(notee, mText, Note::Entry, string, 0, mCategory);
			ADD(notee);

			DEFINEDCARRAY2(Note::Entry*, noteentryp);

			DEFINET2(note, Note);
			SERIALIZER(note, Note);
			EXT(note, note);
			FIRSTMEM2(note, mEntries, Note, DCArray_noteentryp, 1);
			NEXTMEM2(note, mName, Note, string, 0, mEntries);
			ADD(note);

			DEFINEMAP2(int, Note*, int, notep, std::less<int>);

			DEFINET2(notec, NoteCollection);
			FIRSTMEM(notec, "Baseclass_UID::Generator", miNextUniqueID, NoteCollection, uidgen, 0x10);
			meta_notec_miNextUniqueID.mOffset = (i64)dynamic_cast<UID::Generator*>((NoteCollection*)NULL);
			NEXTMEM2(notec, mNotes, NoteCollection, Map_int_notep, 1, miNextUniqueID);
			SERIALIZER(notec, NoteCollection);
			ADD(notec);

			DEFINET2(testt, DlgNodeCriteria::EnumTestT);
			FIRSTMEM1(testt, "Baseclass_EnumBase", ALAIS, mVal,
				DlgNodeCriteria::EnumTestT, enumbase, 0x10);
			NEXTMEM2(testt, mVal, DlgNodeCriteria::EnumTestT, long, 0, ALAIS);
			FIRSTENUM2(testt, mVal, "eRequired", r, 1, 0);
			NEXTENUM2(testt, mVal, "eForbidden", f, 2, 0, r);
			ADDFLAGS(testt, 0x8008);
			ADD(testt);

			DEFINET2(thresht, DlgNodeCriteria::EnumThresholdT);
			FIRSTMEM1(thresht, "Baseclass_EnumBase", ALAIS, mVal,
				DlgNodeCriteria::EnumThresholdT, enumbase, 0x10);
			NEXTMEM2(thresht, mVal, DlgNodeCriteria::EnumThresholdT, long, 0, ALAIS);
			FIRSTENUM2(thresht, mVal, "eAny", r, 1, 0);
			NEXTENUM2(thresht, mVal, "eAll", f, 2, 0, r);
			ADDFLAGS(thresht, 0x8008);
			ADD(thresht);

			DEFINET2(deft, DlgNodeCriteria::EnumDefaultResultT);
			FIRSTMEM1(deft, "Baseclass_EnumBase", ALAIS, mVal,
				DlgNodeCriteria::EnumDefaultResultT, enumbase, 0x10);
			NEXTMEM2(deft, mVal, DlgNodeCriteria::EnumDefaultResultT, long, 0, ALAIS);
			FIRSTENUM2(deft, mVal, "eDefaultToPass", r, 1, 0);
			NEXTENUM2(deft, mVal, "eDefaultToNotPass", f, 2, 0, r);
			NEXTENUM2(deft, mVal, "eDefaultToNotPassUnlessTransparent", g, 3, 0, f);
			ADDFLAGS(deft, 0x8008);
			ADD(deft);

			DEFINET2(crit, DlgNodeCriteria);
			FIRSTMEM2(crit, mTestType, DlgNodeCriteria, testt, 0);
			NEXTMEM2(crit, mFlagsThreshold, DlgNodeCriteria, thresht, 0, mTestType);
			NEXTMEM2(crit, mCriteriaThreshold, DlgNodeCriteria, thresht, 0, mFlagsThreshold);
			NEXTMEM2(crit, mDefaultResult, DlgNodeCriteria, deft, 0, mCriteriaThreshold);
			NEXTMEM2(crit, mClassFlags, DlgNodeCriteria, flags, 0, mDefaultResult);
			NEXTMEM2(crit, mClassIDs, DlgNodeCriteria, Set_i32, 0, mClassFlags);
			ADD(crit);

			DEFINET2(res, LanguageResProxy);
			FIRSTMEM2(res, mID, LanguageResProxy, long, 0);
			ADD(res);

			DEFINET2(jrecord, JiraRecord);
			ADD(jrecord);

			DEFINEMAP2(String, JiraRecord*, String, jrecord, std::less<String>);

			DEFINET2(jira, JiraRecordManager);
			SERIALIZER(jira, JiraRecordManager);
			FIRSTMEM2(jira, mRecords, JiraRecordManager, Map_String_jrecord, 0);
			ADD(jira);

			DEFINET2(line, DlgLine);
			FIRSTMEM1(line, "Baseclass_UID::Owner", ALAISOWNER, miUniqueID,
				DlgLine, uidowner, 0x10);
			meta_line_ALAISOWNER.mOffset = (i64)
				dynamic_cast<UID::Owner*>((DlgLine*)NULL);
			NEXTMEM1(line, "Baseclass_DlgObjIDOwner", ALAISDLG, mDlgObjID,
				DlgLine, dlgidowner, 0x30, ALAISOWNER);
			meta_line_ALAISDLG.mOffset = (i64)
				dynamic_cast<DlgObjIDOwner*>((DlgLine*)NULL);
			NEXTMEM2(line, mLangResProxy, DlgLine, res, 0, ALAISDLG);
			ADD(line);

			DEFINEMAP(int, DlgLine, std::less<int>);

			DEFINET2(lcol, DlgLineCollection);
			FIRSTMEM1(lcol, "Baseclass_UID::Generator", ALAISGEN, miNextUniqueID,
				DlgLineCollection, uidgen, 0x10);
			meta_lcol_ALAISGEN.mOffset = (i64)
				dynamic_cast<UID::Generator*>((DlgLineCollection*)NULL);
			NEXTMEM2(lcol, mLines, DlgLineCollection, Map_int_DlgLine, 0,
				ALAISGEN);
			ADD(lcol);

			DEFINET2(folderc, DlgFolderChild);
			FIRSTMEM1(folderc, "Baseclass_DlgChild", CHILDALAIS,mName, 
				DlgChild, dlgchild, 0x10);
			//meta_folderc_CHILDALAIS.mOffset = (i64)
			//	dynamic_cast<DlgChild*>((DlgFolderChild*)NULL);
			ADD(folderc);

			DEFINET2(dlg, Dlg);
			SERIALIZER(dlg,Dlg);
			FIRSTMEM1(dlg, "Baseclass_DlgObjIDOwner", BASE, mDlgObjID,
				Dlg, dlgidowner, 0x10);
			NEXTMEM1(dlg, "Baseclass_UID::Generator", BASE1,
				miNextUniqueID, Dlg, uidgen, 0x10, BASE);
			meta_dlg_BASE1.mGameIndexVersionRange.max = 
				TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			NEXTMEM2(dlg, mName, Dlg, string, 0x20, BASE1);
			NEXTMEM2(dlg, mVersion, Dlg, long, 0x20, mName);
			NEXTMEM2(dlg, mDefFolderID, Dlg, dlgid, 0x20, mVersion);
			NEXTMEM2(dlg, mLangDB, Dlg, landb, 0x20, mDefFolderID);
			NEXTMEM2(dlg, mProjectID, Dlg, long, 0x20, mLangDB);
			NEXTMEM2(dlg, mResourceLocationID, Dlg, symbol, 0x20, mProjectID);
			NEXTMEM2(dlg, mChronology, Dlg, long, 0, mResourceLocationID);
			NEXTMEM2(dlg, mFlags, Dlg, flags, 0x20, mChronology);
			NEXTMEM2(dlg, mDependencies, Dlg, dloader, 0x20, mFlags);
			NEXTMEM2(dlg, mProdReportProps, Dlg, prop, 0, mDependencies);
			NEXTMEM2(dlg, mJiraRecordManager, Dlg, jira, 0x20, mProdReportProps);
			NEXTMEM2(dlg, mbHasToolOnlyData, Dlg, bool, 0x20, mJiraRecordManager);
			EXT(dlg, dlog);
			ADD(dlg);



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
