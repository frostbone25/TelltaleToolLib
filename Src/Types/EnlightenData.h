// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ENL
#define _ENL

#include "DCArray.h"
#include <emmintrin.h>
#include "../Meta.hpp"
#include "../TelltaleToolLibrary.h"

struct EnlightenSignature {
	u64 mSignature;
	u32 mMagicNumber, mVersion;
};

namespace Enlighten {

	struct RadProbeSetMetaData {
		Geo::GeoGuid m_ProbeSetId;
		int m_NumProbes;
		int m_NumProbesOutput;
		int m_SHOrder;
		int m_SHOverEnvVis;
		int m_MaxBudget;
		int m_MaxProbeSize;
		int m_RequiredWorkspaceSize;
		int m_RequiredTemporalCoherenceBufferSize;
		int m_EnvironmentResolution;
		unsigned __int16 m_IsRegularGrid;
		unsigned __int16 m_RegularGridResolutionX;
		unsigned __int16 m_RegularGridResolutionY;
		unsigned __int16 m_RegularGridResolutionZ;
		unsigned int m_Padding;
		__m128d m_BoundingBoxOrigin;
		__m128d m_BoundingBoxBasisX;
		__m128d m_BoundingBoxBasisY;
		__m128d m_BoundingBoxBasisZ;
	};

	enum eRadDataType : unsigned __int16 {
		RDT_INVALID = 0x0,
		RDT_RAD_CORE = 0x1,
		RDT_RAD_PROJECTION_DATA = 0x2,
		RDT_INPUT_WORKSPACE = 0x4,
		RDT_PROBE_SET_CORE = 0x5,
		RDT_CAW_CLUSTER_MATERIALS = 0x6,
		RDT_TAW_MATERIAL_RUNS = 0x7,
		RDT_AW_MATERIAL_GUIDS = 0x8,
		RDT_INPUT_LIGHTING_BUFFER = 0x9,
		RDT_RAD_CUBE_MAP_CORE = 0xA, 
		RDT_INPUT_LIGHTING_CUBE_MAP_CORE = 0xB,
		RDT_INPUT_PROJECTED_POINTS = 0xF,
		RDT_INPUT_SOURCE_MESH_DATA = 0x10,
		RDT_PROBE_SET_OCTREE = 0x11,
		RDT_PROBE_SET_INTERPOLATION = 0x12,
		RDT_PROBE_SET_VISIBILITY = 0x13,
		RDT_PROBE_SET_OCTREE_VISIBILITY = 0x14,
		RDT_INPUT_HQ_BOUNCE_COORDINATES = 0x15,
		RDT_INPUT_CLUSTER_PROBE_OFFSETS = 0x16,
		RDT_INPUT_CLUSTER_PROBE_BOUNCE_DATA = 0x17,
		RDT_ENTIRE_PROBE_SET_CORE = 0x18,
		RDT_CLUSTER_ALBEDO_WORKSPACE = 0x19,
		RDT_PROBE_SET_DEBUG_DATA = 0x1A,
		RDT_RAD_RESAMPLING_DATA = 0x1B,
		RDT_ALBEDO_BUFFER = 0x1C,
		RDT_EMISSIVE_BUFFER = 0x1D,
		RDT_INCIDENT_LIGHTING_BUFFER = 0x1E,
		RDT_DYNAMIC_MATERIAL_WORKSPACE = 0x1F,
		RDT_TRANSPARENCY_BUFFER = 0x20, 
		RDT_RAD_CORE_BOUNCE = 0x21,
	};

	enum eSolverType : unsigned __int16 {
		SOLVER_REFERENCE = 0,
		SOLVER_SEE = 1,
		SOLVEN_NEON = 2
	};

	struct RadDataBlock {
		union {
			void* m_DataStart;
			unsigned __int64 m_DataStartU64;
		};
		int m_Length;
		eRadDataType m_DataType;
		eSolverType m_SolverType;
	};

	struct RadProbeSetCore {
		RadProbeSetMetaData m_MetaData;
		RadDataBlock m_ProbeSetPrecomp;
		RadDataBlock m_EntireProbeSetPrecomp;
		RadDataBlock m_InterpolationData;
		RadDataBlock m_VisibilityData;
		RadDataBlock m_DebugData;
	};

}

struct EnlightenSystemData {
	Symbol mName;
	Symbol mEnvTileName;
	//u64 mpRadCore;
	//u64 mpInputWorkspace;
	//u64 mpCAWMaterialData;
	//u64 mpPrecomputedVisibilityData;
	BinaryBuffer mRadSystemCore;
	BinaryBuffer mInputWorkspace;
	BinaryBuffer mClusterAlbedoWorkspaceMaterial;
	BinaryBuffer mPrecomputedVisibility;
};

struct EnlightenProbeData {
	Symbol mEnvTileName;
	Enlighten::RadProbeSetCore mpRadProbeSetCore;//ptr but not in lib. this is the format of the data below
	BinaryBuffer mRadProbeSetCore;
};

//.ENL FILES
struct EnlightenData {
	EnlightenSignature mSignature;
	String mName;
	DCArray<EnlightenSystemData> mSystemData;
	DCArray<EnlightenProbeData> mProbeData;
};

#endif