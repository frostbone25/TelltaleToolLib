// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../Meta.hpp"

#ifndef _OBJ_INTRINS
#define _OBJ_INTRINS

template<typename T> struct Curve {//not serialized
	T cf0, cf1, cf2, cf3;
};

struct Vector2 {
	union {
		float x;
		float u;
	};
	union {
		float y;
		float v;
	};
};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pCtx, void* pUserData) {
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector4*>(pObj)->x);
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector4*>(pObj)->y);
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector4*>(pObj)->z);
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector4*>(pObj)->w);
		return eMetaOp_Succeed;
	}

};

struct Vector3 {
	float x;
	float y;
	float z;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pCtx, void* pUserData) {
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector3*>(pObj)->x);
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector3*>(pObj)->y);
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector3*>(pObj)->z);
		return eMetaOp_Succeed;
	}

};

struct Sphere {
	Vector3 mCenter;
	float mRadius;
};

struct Quaternion {
	float x;
	float y;
	float z;
	float w;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pCtx, void* pUserData) {
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Quaternion*>(pObj)->x);
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Quaternion*>(pObj)->y);
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Quaternion*>(pObj)->z);
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Quaternion*>(pObj)->w);
		return eMetaOp_Succeed;
	}

};

struct Transform {
	Quaternion mRot;
	Vector3 mTrans;
	float mPaddingAlign;//to align it since vec3 is 12 bytes and rot is 16, 16 + 16 = 32, a nice one unlike 28
};

#endif