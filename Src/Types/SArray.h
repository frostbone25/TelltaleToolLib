// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../Meta.hpp"

#ifndef _STATIC_ARRAY
#define _STATIC_ARRAY

template<typename T, int N> class SArray {
public:
	T mData[N];

	static MetaOpResult MetaOperation_SerializeMain(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pContext,
		void* pUserData) {
		SArray<T, N>* array = static_cast<SArray<T, N>*>(pObj);
		MetaClassDescription* vtype = GetMetaClassDescription(typeid(T).name());
		if (!array || !vtype)return eMetaOp_Fail;
		MetaOperation op = vtype->GetOperationSpecialization(75);
		if (!op)op = Meta::MetaOperation_SerializeMain;
		for (int i = 0; i < N; i++) {
			if (op(&array->mData[i], vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
		}
		return eMetaOp_Succeed;
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pContext,
		void* pUserData) {
		SArray<T, N>* array = static_cast<SArray<T, N>*>(pObj);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		MetaClassDescription* vtype = GetMetaClassDescription(typeid(T).name());
		if (!array || !meta || !vtype)return eMetaOp_Fail;
		MetaOperation op = vtype->GetOperationSpecialization(74);
		if (!op)op = Meta::MetaOperation_SerializeAsync;
		meta->BeginObject("SArray", NULL);
		meta->SetObjectAsArrayType();
		for (int i = 0; i < N; i++) {
			int id = meta->BeginAnonObject(&array->mData[i]);
			if (op(&array->mData[i], vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
			meta->EndAnonObject(id);
		}
		meta->EndObject("SArray");
		return eMetaOp_Succeed;
	}

};

#endif