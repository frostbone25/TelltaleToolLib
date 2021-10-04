// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../Meta.hpp"
#include "Container.h"
#include <set>

template<typename T> class Set : public ContainerInterface, public std::set<T,std::less<T>> {
public:
	virtual int GetSize() {
		return this->size();
	}

	INLINE virtual bool KeyedContainer() { return false; }
	INLINE virtual void* GetKey(int) { return NULL; }

	virtual MetaClassDescription* GetContainerKeyClassDescription() {
		return NULL;
	}

	virtual MetaClassDescription* GetContainerDataClassDescription() {
		return ::GetMetaClassDescription(typeid(T).name());
	}

	INLINE virtual bool IsResizable() { return true; }

	virtual bool AllocateElements(int total) {
		return false;
	}

	virtual void ClearElements() {
		this->clear();
	}

	virtual void RemoveElement(int at_index) {
		auto it = this->begin();
		it += at_index;
		this->erase(it);
	}

	virtual void AddElement(int at_index, const void* pKeyData, void* pValueToAdd) {
		this->insert(*(T*)pValueToAdd);
	}

	//DO NOT USE THIS IN SETS!
	virtual void SetElement(int at_index, const void* pKeyData, void* pValue) {}

	//DO NOT USE THIS IN SETS!
	virtual String* GetElementName(String* result, int index) {
		return NULL;
	}

	//DO NOT USE THIS IN SETS!
	virtual void* GetElement(int at_index) {
		return NULL;
	}

};