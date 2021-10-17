// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#pragma once
#include "../Meta.hpp"
#include "Container.h"

#ifndef _DYNAMIC_ARRAY
#define _DYNAMIC_ARRAY

template<typename T> class DCArray : public ContainerInterface {
public:
	int mSize;
	int mCapacity;
	T* mpStorage;

	DCArray() : mSize(0), mCapacity(0), mpStorage(NULL) {}

	static MetaOpResult MetaOperation_SerializeMain(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pContext,
		void* pUserData) {
		DCArray<T>* array = static_cast<DCArray<T>*>(pObj);
		MetaClassDescription* vtype = ::GetMetaClassDescription(typeid(T).name());
		if (!array || !vtype)return eMetaOp_Fail;
		MetaOperation op = vtype->GetOperationSpecialization(75);
		if (!op)op = Meta::MetaOperation_SerializeMain;
		for (int i = 0; i < array->mSize; i++) {
			if (op(&array->mpStorage[i], vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
		}
		return eMetaOp_Succeed;
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pContext,
		void* pUserData) {
		DCArray<T>* array = static_cast<DCArray<T>*>(pObj);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		MetaClassDescription* vtype = ::GetMetaClassDescription(typeid(T).name());
		if (!array || !meta || !vtype)return eMetaOp_Fail;
		MetaOperation op = vtype->GetOperationSpecialization(74);
		if (!op)op = Meta::MetaOperation_SerializeAsync;
		u32 size = array->mSize;
		meta->serialize_uint32(&size);
		meta->BeginObject("DCArray", NULL);
		meta->SetObjectAsArrayType();
		MetaOpResult result = eMetaOp_Succeed;
		if (meta->mMode == MetaStreamMode::eMetaStream_Write) {
			for (int i = 0; i < array->mSize; i++) {
				int id = meta->BeginAnonObject(&array->mpStorage[i]);
				if (op(&array->mpStorage[i], vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
				meta->EndAnonObject(id);
			}
		}
		else if(array->Resize(size)) {
			for (int i = 0; i < size; i++) {
				int id = meta->BeginAnonObject(NULL);
					if (op(&array->mpStorage[i], vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
				meta->EndAnonObject(id);
			}
			array->mSize = size;
		}
		else {
			result = eMetaOp_Fail;
		}
		meta->EndObject("DCArray");
		return result;
	}

	virtual int GetSize() override {
		return mSize;
	}

	virtual void RemoveElement(int at_index) override {
		int v2;
		DCArray<bool> arra;
		__int64 v3;
		v2 = this->mSize;
		if (v2)
		{
			if (at_index < v2 - 1)
			{
				v3 = at_index;
				do
				{
					++v3;
					++at_index;
					this->mpStorage[v3 - 1] = this->mpStorage[v3];
					v2 = this->mSize;
				} while (at_index < v2 - 1);
			}
			this->mSize = v2 - 1;
		}
	}

	virtual void ClearElements() override {
		Clear(0);
	}

	void Clear(int from_index) {
		if (from_index >= this->mSize)
		{
			//this->mSize = from_index;
		}
		else
		{
			for (int i = from_index; i < mSize; i++) {
				mpStorage[i].~T();
			}
			Resize(from_index-mSize);
		}
	}

	void SetElement(int at_index, const void* pKeyData, void* pValue) {
		T* elem = &mpStorage[at_index];
		if (pValue)
			*elem = *static_cast<T*>(pValue);
		else
			memset(elem, 0, sizeof(T));
	}

	void AddElement(int at_index, const void* pKeyData, void* pValueToAdd) {
		if (mSize == mCapacity) {
			Resize(4);//bit different to telltale who cares
		}
		if (mSize && !at_index)at_index = mSize;
		SetElement(at_index, pKeyData, pValueToAdd);
		mSize++;
	}

	virtual String* GetElementName(String* result,int index) override {
		static char _Buffer[0x20];
		sprintf(_Buffer, "%d", index);
		if (result)new (result) String(_Buffer);
		return result;
	}

	T& operator[](int index) {
		return mpStorage[index];
	}

	virtual void* GetElement(int index) override{
		return (void*)(mpStorage + index);
	}

	bool ReserveAndResize(int num_to_add_or_remove) {
		if (Resize(num_to_add_or_remove)) {
			mSize = mCapacity;
			return true;
		}
		return false;
	}
	
	//Reserves memory, does not change size
	bool Resize(int num_to_add_or_remove) {
		if (!num_to_add_or_remove)return true;
		T* oldstorage = mpStorage;
		int newcap = mCapacity + num_to_add_or_remove;
		if (newcap > 0) {
			mpStorage = new T[newcap];
			if (mSize >= newcap) {
				memcpy(mpStorage, oldstorage, sizeof(T) * newcap);
				mSize = newcap;
			}
			else {//newcap>size
				memcpy(mpStorage, oldstorage, sizeof(T) * mSize);
			}
			if (oldstorage)delete[] oldstorage;
		}
		else return false;
		mCapacity = newcap;
		return true;
	}

	INLINE virtual bool IsResizable() override { return true; }

	INLINE virtual bool KeyedContainer() override { return false; }

	virtual bool AllocateElements(int total) override {
		bool result;
		if (Resize(total)) {
			result = true;
			mSize = total;
		}
		else {
			result = false;
		}
		return result;
	}

	virtual void* GetKey(int) override {
		return NULL;
	}

	INLINE virtual MetaClassDescription* GetContainerKeyClassDescription() { return NULL; }

	INLINE virtual MetaClassDescription* GetContainerDataClassDescription() {
		return ::GetMetaClassDescription(typeid(T).name());
	}

	~DCArray() {
		if (mpStorage)delete[]mpStorage;
	}

	DCArray& operator=(const DCArray& other) {
		mSize = other.mSize;
		mCapacity = other.mCapacity;
		if (mpStorage)delete[] mpStorage;
		mpStorage = new T[mCapacity];
		memcpy(mpStorage, other.mpStorage, mCapacity);
		return *this;
	}

	DCArray(const DCArray& other) {
		mSize = other.mSize;
		mCapacity = other.mCapacity;
		if (other.mpStorage) {
			mpStorage = new T[mCapacity];
			memcpy(mpStorage, other.mpStorage, sizeof(T) * mCapacity);
		}
		else {
			mpStorage = NULL;
		}
		if (mCapacity && !mpStorage)mpStorage = new T[mCapacity];
	}

	DCArray& operator=(DCArray&& other) {
		mSize = other.mSize;
		mCapacity = other.mCapacity;
		if (mpStorage)delete[] mpStorage;
		mpStorage = other.mpStorage;
		other.mSize = 0;
		other.mCapacity = 0;
		other.mpStorage = NULL;
		return *this;
	}

	DCArray(DCArray&& other) {
		mSize = other.mSize;
		mCapacity = other.mCapacity;
		mpStorage = other.mpStorage;
		other.mSize = 0;
		other.mCapacity = 0;
		other.mpStorage = NULL;
	}

};

//DCArrayNoMeta (not serialized)
template<typename T> using DCArrayNM = DCArray<T>;

#endif