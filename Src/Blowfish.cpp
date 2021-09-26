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
	//TODO
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