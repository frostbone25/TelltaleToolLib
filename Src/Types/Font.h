// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _FONT
#define _FONT

#include "../Meta.hpp"
#include "DCArray.h"
#include "T3Texture.h"
#include "Map.h"
#include "TRect.h"

//.FONT FILES
struct Font {

	struct GlpyhInfo {
		long mTexturePage;
		long mChannel;
		TRect<float> mGlyph;
		float mWidth;
		float mHeight;
		float mXOffset;
		float mYOffset;
		float mXAdvance;
	};

	//TODO REMEMBER TO NOTE THE VARS WHICH ARE IN OLDER GAMES ETC . WHEN ON PC WITH ALL GAMES CHECK WHICH GAMES HAVE THE VARS (.EXE)

	String mName;
	bool mbUnicode;
	float mHeight;
	float mBase;

	//OLDER GAMES --- (before WD4, exclusive)
	float mWiiScale;
	float mFontScale;
	//-----

	Map<unsigned int, Font::GlpyhInfo> mGlyphInfo;
	DCArray<T3Texture> mTexturePages;
	bool mIsDistanceField;

	//-- NEWER DATA BELOW 
	bool mIsRuntime;
	bool mIsFiltered;
	BinaryBuffer mTtfData;
	float mBasePointSize;
	DCArray<unsigned int> mPreferredPointSizes;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		return Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
	}

};

#endif