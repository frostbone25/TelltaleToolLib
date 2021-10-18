// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _WBOX
#define _WBOX

#include "../Meta.hpp"
#include "ObjectSpaceIntrinsics.h"
#include "DCArray.h"
#include "SArray.h"

//.WBOX FILES
struct WalkBoxes {

	struct Tri {

	};

	struct Edge {
		Flags mFlags;
		int mV1, V2;
		int mEdgeDest, mEdgeDestEdge, mEdgeDir;
		float mMaxRadius;
	};

	struct Vert {
		Flags mFlags;
		Vector3 mPos;
	};

	struct Quad {
		SArray<int, 4> mVerts;
	};

	String mName;
	DCArray<Tri> mTris;
	DCArray<Vert> mVerts;
	DCArray<Vector3> mNormals;
	DCArray<Quad> mQuads;

};

#endif