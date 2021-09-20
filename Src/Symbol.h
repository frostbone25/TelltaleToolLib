// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#pragma once
#include "TelltaleToolLibrary.h"
#include "Meta.h"
#include "HashManager.h"

class Symbol {
private:
	static char smSymbolBuffer[sizeof(u64) * 2 + 1];//1byte= 2 hex chars
	u64 mCrc64;
public:

	Symbol(void) : mCrc64(0) {}
	Symbol(char const* pString) {
		mCrc64 = pString ? CRC64_CaseInsensitive(0, pString) : 0;
	}
	Symbol(const String& pString) {
		mCrc64 = CRC64_CaseInsensitive(0, pString.c_str());
	}
	Symbol(u64 crc) : mCrc64(crc) {}

	Symbol& operator=(const Symbol& rhs) {
		this->mCrc64 = rhs.mCrc64;
		return *this;
	}

	INLINE const char* CRCAsCstr() const {//not async
		sprintf(smSymbolBuffer, "%llx", mCrc64);
		return smSymbolBuffer;
	}

	INLINE void SetCRC(u64 crc) { mCrc64 = crc; }

	Symbol& Concat(const String& tName) {
		mCrc64 = CRC64_CaseInsensitive(0, tName.c_str());
		return *this;
	}
	//AsConcat isnt needed

	INLINE operator const char* () const { return CRCAsCstr(); }

	static MetaOpResult MetaOperation_Equivalence(void* pObj,
		MetaClassDescription* pClassDescription, MetaMemberDescription* pContextDescription,
		void* pUserData) {//userdata=meta::equivalence
		static_cast<Meta::Equivalence*>(pUserData)->mbEqual = static_cast<Symbol*>(pObj)->mCrc64 ==
			static_cast<Symbol*>(static_cast<Meta::Equivalence*>(pUserData)->mpOther)->mCrc64;
		return MetaOpResult::eMetaOp_Succeed;
	}

	static MetaOpResult MetaOperation_ConvertFrom(void* pObj, MetaClassDescription* pClassDescription,
		MetaMemberDescription* pContextDescription, void* pUserData) {
		if (static_cast<Meta::ConvertFromInfo*>(pUserData)->mpFromObjDescription ==
			MetaHelpers::GetMetaClassDescription<String>()) {
			static_cast<Symbol*>(pObj)->mCrc64 = CRC64_CaseInsensitive(0, 
				static_cast<const char*>(static_cast<Meta::ConvertFromInfo*>(pUserData)->mpFromObject));
			return eMetaOp_Succeed;
		}
		return Meta::MetaOperation_ConvertFrom(pObj, pClassDescription, pContextDescription, pUserData);
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pClassDescription,
		MetaMemberDescription* pContextDescription, void* pUserData) {
		static_cast<MetaStream*>(pUserData)->serialize_Symbol(static_cast<Symbol*>(pObj));
		return eMetaOp_Succeed;
	}

	static const Symbol EmptySymbol;

};