// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "Blowfish.h"

void Blowfish::Shutdown() {
	if (!spBlowfish)return;
	delete spBlowfish;
	spBlowfish = 0;
}

void Blowfish::Init(const char* password) {
	this->PA[0] = 608135816;
	this->PA[1] = -2052912941;
	this->PA[2] = 320440878;
	this->PA[3] = 57701188;
	this->PA[4] = -1542899678;
	this->PA[5] = 698298832;
	this->PA[6] = 137296536;
	this->PA[7] = -330404727;
	this->PA[8] = 1160258022;
	this->PA[9] = 953160567;
	this->PA[10] = -1101764913;
	this->PA[11] = 887688300;
	this->PA[12] = -1062458953;
	this->PA[13] = -914599715;
	this->PA[14] = 1065670069;
	this->PA[15] = -1253635817;
	this->PA[16] = -1843997223;
	this->PA[17] = -1988494565;
	for (int i = 0; i < 256; i++) {
		this->SB[0][i] = ks0[i];
		this->SB[1][i] = ks1[i];
		this->SB[2][i] = ks2[i];
		this->SB[3][i] = ks3[i];
	}
	this->Gen_Subkeys(password);
	mbInitialized = true;
}

void Blowfish::Gen_Subkeys(const char* Passwd) {
	int len = 0;
	while (Passwd[len]) {
		len++;
	}
	if (len) {
		short j = 0, i = 0,k = 0, N = 16;
		unsigned long data,datal,datar;//32bits

		for (i = 0; i < N + 2; ++i) {
			data = 0x00000000;
			for (k = 0; k < 4; ++k) {
				data = (data << 8) | Passwd[j];
				j = j + 1;
				if (j >= len)j = 0;
			}
			PA[i] = PA[i] ^ data;
		}
		datal = 0;
		datar = 0;
		for (i = 0; i < N + 2; i += 2) {
			BF_En(&datal, &datar);
			PA[i] = datal;
			PA[i + 1] = datar;
		}
		for (i = 0; i < 4; ++i) {
			for (j = 0; j < 256; j += 2) {
				BF_En(&datal, &datar);
				SB[i][j] = datal;
				SB[i][j + 1] = datar;
			}
		}
	}
}

void Blowfish::Encrypt(void* buffer, unsigned int size) {

}

Blowfish::Blowfish(const char* password) : Blowfish() {
	Init(password);
}

Blowfish::Blowfish() : mbInitialized(false){}

Blowfish* Blowfish::Get() {
	return spBlowfish;
}

void Blowfish::Initialize(const BlowfishKey* key) {
	if (!spBlowfish) {
		spBlowfish = new Blowfish();
		spBlowfish->Init(key->game_key);
	}
}

//todo init blowfish in lib init