// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "Compression.h"

bool Compression::ZlibCompress(void* pDst, unsigned int* pDstLength, const void* pSrc, unsigned int srcLength) {
	z_stream_s stream;
	stream.next_in = (Bytef*)pSrc;
	stream.avail_in = srcLength;
	stream.avail_out = *pDstLength;
	stream.next_out = (Bytef*)pDst;
	if (!deflateInit2_(&stream, Z_BEST_SPEED, Z_DEFLATED, -15, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY, "1.2.8", sizeof(stream))) {
		if (deflate(&stream, Z_FINISH) == 1) {//z finish
			*pDstLength = stream.total_out;
			return deflateEnd(&stream) == Z_OK;
		}
		deflateEnd(&stream);
	}
	return false;
}