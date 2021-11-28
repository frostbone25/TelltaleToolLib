// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _BLENDGRAPH
#define _BLENDGRAPH

#include "AnimOrChore.h"
#include "../Meta.hpp"
#include "HandleObjectInfo.h"
#include "DCArray.h"
#include "KeyframedValue.h"

struct CorrespondencePoint {
	Flags mFlags;
	Flags mEaseOutStartFlags;
	Flags mEaseOutEndFlags;
	Flags mEaseInStartFlags;
	Flags mEaseInEndFlags;
	Flags mSteeringFlags;
	Flags mTransitionFlags;
	float mfTime;
	String mComment;
};

struct BlendEntry {
	Vector3 mParameterValues;
	AnimOrChore mAnimOrChore;
	DCArray<CorrespondencePoint> mCorrespondencePoints;
	float mfAnimOrChoreLength;
	String mComment;
};

//.BGH FILES
struct BlendGraph {

	enum BlendGraphType {
		eBlendgraph_Looping = 0,
		eBlendgraph_NonLooping = 1
	};

	struct EnumBlendGraphType : EnumBase {
		BlendGraphType mVal;
	};

	BlendGraph() {
		mpGeometry = NULL;
	}

	DCArray<Symbol> mParameters, mFrozenParameterNames;//frozen params removed in WD3 only, fuck knows why
	DCArray<float> mDampeningConstants;
	DCArray<BlendEntry> mEntries;
	EnumBlendGraphType mBlendGraphType;
	bool mDampen;
	float mfTimeScale;
	String mComment;
	void* mpGeometry;
	Handle<Chore> mhBlendGraphAuxiliaryChore;
	long mbInvertParameters;
	long mVersion;
	long mNumGeometryDimensions;
	long mNumDimensions;
	DCArray<int> mParameterOrder;

	//static void ConvertEaseInFlagsToV6(int v5Flags, CorrespondencePoint* cp) {}

	//static void ConvertEaseOutFlagsToV6(int v5Flags, CorrespondencePoint* cp) {}

	inline static const float kDefaultConstants[4] = { 6.0f,2.0f,2.0f,0.0f };

	INLINE void ClearGeometry() {
		if (mpGeometry) {
			if (mNumGeometryDimensions == 1) {
				delete (KeyframedValue<int>*)mpGeometry;
			}
			else if (mNumGeometryDimensions == 2) {
				delete (Map<float, KeyframedValue<int>>*)mpGeometry;
			}
			else if (mNumGeometryDimensions == 3) {
				delete (Map<float, Map<float, KeyframedValue<int>>>*)mpGeometry;
			}
			else
				delete mpGeometry;
		}
	}

	~BlendGraph() {
		ClearGeometry();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(BlendGraph, bg);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			long ver = bg->mVersion;
			if (ver < 4)
				bg->mNumGeometryDimensions = bg->mNumDimensions;
			if (ver < 5 && bg->mBlendGraphType.mVal == BlendGraphType::eBlendgraph_NonLooping)
				bg->mFrozenParameterNames = bg->mParameters;
			bool hasgeom = bg->mpGeometry != NULL;
			meta->serialize_bool(&hasgeom);
			if (meta->IsRead())
				bg->ClearGeometry();
			if (hasgeom) {
				MetaClassDescription* geom = NULL;
				if (bg->mNumGeometryDimensions == 1) {
					if (meta->IsRead())
						bg->mpGeometry = new KeyframedValue<int>;
					geom = ::GetMetaClassDescription<KeyframedValue<int>>();
				}
				else if (bg->mNumGeometryDimensions == 2) {
					if (meta->IsRead())
						bg->mpGeometry = new Map<float, KeyframedValue<int>>;
					geom = ::GetMetaClassDescription<Map<float, KeyframedValue<int>>>();
				}
				else {
					if (bg->mNumGeometryDimensions != 3)
						return eMetaOp_Fail;
					if (meta->IsRead())
						bg->mpGeometry = new Map<float, Map<float, KeyframedValue<int>>>;
					geom = ::GetMetaClassDescription<Map<float, Map<float, KeyframedValue<int>>>>();
				}
				r = PerformMetaSerializeFull(meta, bg->mpGeometry, geom);
				if (r != eMetaOp_Succeed)
					return r;
			}

		}
		return r;
	}

};

//.BGM FILES
struct BlendGraphManager {
	float mfTransitionTime;
	AnimOrChore mIdleAnimOrChore;
	bool mbUseAnimationMoverData;
	Handle<BlendGraph> mhFreewalkStartGraph, mhFreewalkLoopGraph, mhFreewalkStopGraph, mhTurnToFaceGraph,//turn to face in newer
		mhChoredMovementStartGraph, mhChoredMovementLoopGraph, mhChoredMovementStopGraph;//older games this line
	long mVersion;
	bool mbUseAlgorithmicHeadTurn;
	float mfMaxManualSteeringVelocityInDegrees, mfMinManualSteeringVelocityInDegrees;
	float mfMaxLeanInPercentVelocity, mfMinLeanInPercentVelocity;
	float mfWalkSpeedScale;
	float mfRunSpeedScale;
};

#endif