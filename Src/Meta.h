#ifndef _META
#define _META

#include <string>
#include <typeinfo>
#include "MetaStream.h"

enum MetaOpResult {
	eMetaOp_Fail = 0x0,
	eMetaOp_Succeed = 0x1,
	eMetaOp_Invalid = 0x2,
	eMetaOp_OutOfMemory = 0x3,
	eMetaOp_MAX = 0x4,
};

class SerializedVersionInfo {

};

class MetaSerializeAccel {

};

struct MetaMemberDescription;
struct MetaClassDescription;

typedef MetaOpResult(*MetaOperation)(void*, MetaClassDescription*, MetaMemberDescription*, void*);

struct MetaOperationDescription {
	u32 id;
	MetaOperation mpOpFn;
	MetaOperationDescription* mpNext;
};

struct MetaClassDescription {
	const char* mpExt;
	const char* mpTypeInfoName;
	u64 mHash;
	u32 mClassSize;
	SerializedVersionInfo* mpCompiledVersionSerializedVersionInfo;//atomic
	MetaMemberDescription* mpFirstMember;
	MetaOperationDescription* mMetaOperationsList; 
	MetaClassDescription* pNextMetaClassDescription; 
	//const void** mpVTable; 
	MetaSerializeAccel* mpSerializeAccel;//atomic
};

struct MetaEnumDescription {

};

struct MetaFlagDescription {

};

struct MetaMemberDescription {
	const char* mpName;
	i64 mOffset;
	i32 mFlags;
	MetaClassDescription* mpHostClass;
	MetaMemberDescription* mpNextMember;
	union {
		MetaEnumDescription mpEnumDescriptions;
		MetaFlagDescription* mpFlagDescriptions;
	};
	MetaClassDescription* mpMemberDesc;
};

namespace Meta {

	struct Equivalence {
		bool mbEqual;
		void* mpOther;
	};

	struct ConvertFromInfo {
		const void* mpFromObject;
		MetaClassDescription* mpFromObjDescription;
	};

	static MetaOpResult MetaOperation_ConvertFrom(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription *pContextDescription,
		void * pUserData);

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pCtxDesc, void* pUserData);

	static MetaOpResult MetaOperation_Invalid(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pContextDescription,
		void* pUserData) {
		return MetaOpResult::eMetaOp_Invalid;
	}

	static MetaOpResult MetaOperation_SerializeMain(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pCtxDesc, void* pUserData);


};

namespace MetaHelpers {

	template<typename T>
	MetaClassDescription* GetMetaClassDescription() {
		return NULL;
	}

}









#endif
