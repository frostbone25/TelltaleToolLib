// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ANM
#define _ANM

#include "../Meta.hpp"
#include "ToolProps.h"
#include "DCArray.h"
#include "AnimatedValueInterface.h"

//TODO , note cant use chore included, prefine and code in src file
struct Animation {

	long mVersion;
	Flags mFlags;
	Symbol mName;
	float mLength;
	float mAdditiveMask;
	DCArray<AnimationValueInterfaceBase*> mValues;//not serialized/see spec
	ToolProps mToolProps;
	void* mpValueBuffer;//no ser
	u32 mValueBufferSize;//no ser
	void* mpValueDataBuffer;//no set

	void _DeleteData() {
		for (int i = 0; i < mValues.GetSize(); i++)
			delete mValues[i];
		mValues.ClearElements();
		if (mpValueBuffer)
			delete mpValueBuffer;
		if (mpValueDataBuffer)
			delete mpValueDataBuffer;
		mValueBufferSize = 0;
		mpValueBuffer = NULL;
		mpValueDataBuffer = NULL;
	}

	Animation() {
		mValueBufferSize = 0;
		mpValueBuffer = NULL;
		mpValueDataBuffer = NULL;
	}

	~Animation() {
		_DeleteData();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(Animation, anm);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj,
			pObjDescription, pContextDescription, pUserData);
		if (r != eMetaOp_Succeed)
			return r;
		meta->BeginBlock();
		u32 interfaces = anm->mValues.GetSize();
		meta->serialize_uint32(&interfaces);
		if (meta->IsWrite()) {
			//telltale didnt have any writing code (probably made from maya)
			//so ill wing it latr
		}
		else {
			MetaClassDescription* interfaceDesc = 
				GetMetaClassDescription<AnimationValueInterfaceBase>();
			if (!interfaceDesc) {
				TelltaleToolLib_RaiseError("Meta not initialized: "
					"cannot read anm: no metaclass for value interface base", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
			anm->_DeleteData();
			anm->mValues.Resize(interfaces);
			if (anm->mVersion >= 4) {
				u32 dataBufferSize = 0, animValueTypes = 0;
				meta->serialize_uint32(&dataBufferSize);
				meta->serialize_uint32(&animValueTypes);//anim value type count
				char* classDescriptions = (char*)calloc(1,16 * animValueTypes);
				char* typeCounts = classDescriptions + 8 * animValueTypes;
				char* tempBuffer3 = classDescriptions + 12 * animValueTypes;
				u64 crc = 0,total=0;
				MetaClassDescription* desc = NULL;
				u16 valuesOfType = 0, d = 0;
				for (int i = 0; i < animValueTypes; i++) {
					meta->serialize_uint64(&crc);
					desc = TelltaleToolLib_FindMetaClassDescription_ByHash(crc);
					if (!desc) {
						static char temp[100];
						sprintf(temp, 
							"Could not find anm value description SYM: %PRIx64", crc);
						TelltaleToolLib_RaiseError(temp, ErrorSeverity::ERR);
						meta->EndBlock();
						return eMetaOp_SymbolNotFound;
					}
					meta->serialize_uint16(&valuesOfType);
					meta->serialize_uint16(&d);
					memcpy(classDescriptions + 8 * i, &desc, 8);
					memcpy(typeCounts + 4 * i, &valuesOfType, 2);
					memcpy(tempBuffer3 + 4 * i, &d, 2);
					total += desc->mClassSize * valuesOfType;
				}
				if (total > 0) {
					anm->mValueBufferSize = total;
					anm->mpValueBuffer = ::operator new(total);
				}
				if (dataBufferSize > 0) {
					anm->mpValueDataBuffer = ::operator new(dataBufferSize);
					if (!anm->mpValueDataBuffer) {
						free(classDescriptions);
						TelltaleToolLib_RaiseError("Could not allocate "
							"animation value data buffers", 
							ErrorSeverity::ERR);
						return eMetaOp_OutOfMemory;
					}
				}
				if (animValueTypes > 0) {
					for (int i = 0; i < animValueTypes; i++) {
						MetaClassDescription* desc = 
							(MetaClassDescription*)classDescriptions[i * 8];
						int count = ((int*)typeCounts)[i];
						int x = ((int*)tempBuffer3)[i];
						//void* valueData = anm->mpValueBuffer
					}
				}
				free(classDescriptions);
			}
		}
		meta->EndBlock();
		return r;
	}

};

#endif