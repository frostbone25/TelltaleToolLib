// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _BLOWFISH
#define _BLOWFISH

//Not implemented fully. You can if you want to. Maybe i will in the future..

#define KEY_COUNT 66

//Default game key is set to The Walking Dead: Collection
#define DEFAULT_BLOWFISH_GAME_KEY 62


typedef struct {
	const bool isNewEncryption;
	const char* game_id;
	const char* game_key;
} BlowfishKey;


static unsigned int bfp[] =
{
	0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344,
	0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89,
	0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c,
	0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
	0x9216d5d9, 0x8979fb1b,
};

static unsigned int ks0[] =
{
	0xd1310ba6, 0x98dfb5ac, 0x2ffd72db, 0xd01adfb7,
	0xb8e1afed, 0x6a267e96, 0xba7c9045, 0xf12c7f99,
	0x24a19947, 0xb3916cf7, 0x0801f2e2, 0x858efc16,
	0x636920d8, 0x71574e69, 0xa458fea3, 0xf4933d7e,
	0x0d95748f, 0x728eb658, 0x718bcd58, 0x82154aee,
	0x7b54a41d, 0xc25a59b5, 0x9c30d539, 0x2af26013,
	0xc5d1b023, 0x286085f0, 0xca417918, 0xb8db38ef,
	0x8e79dcb0, 0x603a180e, 0x6c9e0e8b, 0xb01e8a3e,
	0xd71577c1, 0xbd314b27, 0x78af2fda, 0x55605c60,
	0xe65525f3, 0xaa55ab94, 0x57489862, 0x63e81440,
	0x55ca396a, 0x2aab10b6, 0xb4cc5c34, 0x1141e8ce,
	0xa15486af, 0x7c72e993, 0xb3ee1411, 0x636fbc2a,
	0x2ba9c55d, 0x741831f6, 0xce5c3e16, 0x9b87931e,
	0xafd6ba33, 0x6c24cf5c, 0x7a325381, 0x28958677,
	0x3b8f4898, 0x6b4bb9af, 0xc4bfe81b, 0x66282193,
	0x61d809cc, 0xfb21a991, 0x487cac60, 0x5dec8032,
	0xef845d5d, 0xe98575b1, 0xdc262302, 0xeb651b88,
	0x23893e81, 0xd396acc5, 0x0f6d6ff3, 0x83f44239,
	0x2e0b4482, 0xa4842004, 0x69c8f04a, 0x9e1f9b5e,
	0x21c66842, 0xf6e96c9a, 0x670c9c61, 0xabd388f0,
	0x6a51a0d2, 0xd8542f68, 0x960fa728, 0xab5133a3,
	0x6eef0b6c, 0x137a3be4, 0xba3bf050, 0x7efb2a98,
	0xa1f1651d, 0x39af0176, 0x66ca593e, 0x82430e88,
	0x8cee8619, 0x456f9fb4, 0x7d84a5c3, 0x3b8b5ebe,
	0xe06f75d8, 0x85c12073, 0x401a449f, 0x56c16aa6,
	0x4ed3aa62, 0x363f7706, 0x1bfedf72, 0x429b023d,
	0x37d0d724, 0xd00a1248, 0xdb0fead3, 0x49f1c09b,
	0x075372c9, 0x80991b7b, 0x25d479d8, 0xf6e8def7,
	0xe3fe501a, 0xb6794c3b, 0x976ce0bd, 0x04c006ba,
	0xc1a94fb6, 0x409f60c4, 0x5e5c9ec2, 0x196a2463,
	0x68fb6faf, 0x3e6c53b5, 0x1339b2eb, 0x3b52ec6f,
	0x6dfc511f, 0x9b30952c, 0xcc814544, 0xaf5ebd09,
	0xbee3d004, 0xde334afd, 0x660f2807, 0x192e4bb3,
	0xc0cba857, 0x45c8740f, 0xd20b5f39, 0xb9d3fbdb,
	0x5579c0bd, 0x1a60320a, 0xd6a100c6, 0x402c7279,
	0x679f25fe, 0xfb1fa3cc, 0x8ea5e9f8, 0xdb3222f8,
	0x3c7516df, 0xfd616b15, 0x2f501ec8, 0xad0552ab,
	0x323db5fa, 0xfd238760, 0x53317b48, 0x3e00df82,
	0x9e5c57bb, 0xca6f8ca0, 0x1a87562e, 0xdf1769db,
	0xd542a8f6, 0x287effc3, 0xac6732c6, 0x8c4f5573,
	0x695b27b0, 0xbbca58c8, 0xe1ffa35d, 0xb8f011a0,
	0x10fa3d98, 0xfd2183b8, 0x4afcb56c, 0x2dd1d35b,
	0x9a53e479, 0xb6f84565, 0xd28e49bc, 0x4bfb9790,
	0xe1ddf2da, 0xa4cb7e33, 0x62fb1341, 0xcee4c6e8,
	0xef20cada, 0x36774c01, 0xd07e9efe, 0x2bf11fb4,
	0x95dbda4d, 0xae909198, 0xeaad8e71, 0x6b93d5a0,
	0xd08ed1d0, 0xafc725e0, 0x8e3c5b2f, 0x8e7594b7,
	0x8ff6e2fb, 0xf2122b64, 0x8888b812, 0x900df01c,
	0x4fad5ea0, 0x688fc31c, 0xd1cff191, 0xb3a8c1ad,
	0x2f2f2218, 0xbe0e1777, 0xea752dfe, 0x8b021fa1,
	0xe5a0cc0f, 0xb56f74e8, 0x18acf3d6, 0xce89e299,
	0xb4a84fe0, 0xfd13e0b7, 0x7cc43b81, 0xd2ada8d9,
	0x165fa266, 0x80957705, 0x93cc7314, 0x211a1477,
	0xe6ad2065, 0x77b5fa86, 0xc75442f5, 0xfb9d35cf,
	0xebcdaf0c, 0x7b3e89a0, 0xd6411bd3, 0xae1e7e49,
	0x00250e2d, 0x2071b35e, 0x226800bb, 0x57b8e0af,
	0x2464369b, 0xf009b91e, 0x5563911d, 0x59dfa6aa,
	0x78c14389, 0xd95a537f, 0x207d5ba2, 0x02e5b9c5,
	0x83260376, 0x6295cfa9, 0x11c81968, 0x4e734a41,
	0xb3472dca, 0x7b14a94a, 0x1b510052, 0x9a532915,
	0xd60f573f, 0xbc9bc6e4, 0x2b60a476, 0x81e67400,
	0x08ba6fb5, 0x571be91f, 0xf296ec6b, 0x2a0dd915,
	0xb6636521, 0xe7b9f9b6, 0xff34052e, 0xc5855664,
	0x53b02d5d, 0xa99f8fa1, 0x08ba4799, 0x6e85076a
};
static unsigned int ks1[] =
{
	0x4b7a70e9, 0xb5b32944, 0xdb75092e, 0xc4192623,
	0xad6ea6b0, 0x49a7df7d, 0x9cee60b8, 0x8fedb266,
	0xecaa8c71, 0x699a17ff, 0x5664526c, 0xc2b19ee1,
	0x193602a5, 0x75094c29, 0xa0591340, 0xe4183a3e,
	0x3f54989a, 0x5b429d65, 0x6b8fe4d6, 0x99f73fd6,
	0xa1d29c07, 0xefe830f5, 0x4d2d38e6, 0xf0255dc1,
	0x4cdd2086, 0x8470eb26, 0x6382e9c6, 0x021ecc5e,
	0x09686b3f, 0x3ebaefc9, 0x3c971814, 0x6b6a70a1,
	0x687f3584, 0x52a0e286, 0xb79c5305, 0xaa500737,
	0x3e07841c, 0x7fdeae5c, 0x8e7d44ec, 0x5716f2b8,
	0xb03ada37, 0xf0500c0d, 0xf01c1f04, 0x0200b3ff,
	0xae0cf51a, 0x3cb574b2, 0x25837a58, 0xdc0921bd,
	0xd19113f9, 0x7ca92ff6, 0x94324773, 0x22f54701,
	0x3ae5e581, 0x37c2dadc, 0xc8b57634, 0x9af3dda7,
	0xa9446146, 0x0fd0030e, 0xecc8c73e, 0xa4751e41,
	0xe238cd99, 0x3bea0e2f, 0x3280bba1, 0x183eb331,
	0x4e548b38, 0x4f6db908, 0x6f420d03, 0xf60a04bf,
	0x2cb81290, 0x24977c79, 0x5679b072, 0xbcaf89af,
	0xde9a771f, 0xd9930810, 0xb38bae12, 0xdccf3f2e,
	0x5512721f, 0x2e6b7124, 0x501adde6, 0x9f84cd87,
	0x7a584718, 0x7408da17, 0xbc9f9abc, 0xe94b7d8c,
	0xec7aec3a, 0xdb851dfa, 0x63094366, 0xc464c3d2,
	0xef1c1847, 0x3215d908, 0xdd433b37, 0x24c2ba16,
	0x12a14d43, 0x2a65c451, 0x50940002, 0x133ae4dd,
	0x71dff89e, 0x10314e55, 0x81ac77d6, 0x5f11199b,
	0x043556f1, 0xd7a3c76b, 0x3c11183b, 0x5924a509,
	0xf28fe6ed, 0x97f1fbfa, 0x9ebabf2c, 0x1e153c6e,
	0x86e34570, 0xeae96fb1, 0x860e5e0a, 0x5a3e2ab3,
	0x771fe71c, 0x4e3d06fa, 0x2965dcb9, 0x99e71d0f,
	0x803e89d6, 0x5266c825, 0x2e4cc978, 0x9c10b36a,
	0xc6150eba, 0x94e2ea78, 0xa5fc3c53, 0x1e0a2df4,
	0xf2f74ea7, 0x361d2b3d, 0x1939260f, 0x19c27960,
	0x5223a708, 0xf71312b6, 0xebadfe6e, 0xeac31f66,
	0xe3bc4595, 0xa67bc883, 0xb17f37d1, 0x018cff28,
	0xc332ddef, 0xbe6c5aa5, 0x65582185, 0x68ab9802,
	0xeecea50f, 0xdb2f953b, 0x2aef7dad, 0x5b6e2f84,
	0x1521b628, 0x29076170, 0xecdd4775, 0x619f1510,
	0x13cca830, 0xeb61bd96, 0x0334fe1e, 0xaa0363cf,
	0xb5735c90, 0x4c70a239, 0xd59e9e0b, 0xcbaade14,
	0xeecc86bc, 0x60622ca7, 0x9cab5cab, 0xb2f3846e,
	0x648b1eaf, 0x19bdf0ca, 0xa02369b9, 0x655abb50,
	0x40685a32, 0x3c2ab4b3, 0x319ee9d5, 0xc021b8f7,
	0x9b540b19, 0x875fa099, 0x95f7997e, 0x623d7da8,
	0xf837889a, 0x97e32d77, 0x11ed935f, 0x16681281,
	0x0e358829, 0xc7e61fd6, 0x96dedfa1, 0x7858ba99,
	0x57f584a5, 0x1b227263, 0x9b83c3ff, 0x1ac24696,
	0xcdb30aeb, 0x532e3054, 0x8fd948e4, 0x6dbc3128,
	0x58ebf2ef, 0x34c6ffea, 0xfe28ed61, 0xee7c3c73,
	0x5d4a14d9, 0xe864b7e3, 0x42105d14, 0x203e13e0,
	0x45eee2b6, 0xa3aaabea, 0xdb6c4f15, 0xfacb4fd0,
	0xc742f442, 0xef6abbb5, 0x654f3b1d, 0x41cd2105,
	0xd81e799e, 0x86854dc7, 0xe44b476a, 0x3d816250,
	0xcf62a1f2, 0x5b8d2646, 0xfc8883a0, 0xc1c7b6a3,
	0x7f1524c3, 0x69cb7492, 0x47848a0b, 0x5692b285,
	0x095bbf00, 0xad19489d, 0x1462b174, 0x23820e00,
	0x58428d2a, 0x0c55f5ea, 0x1dadf43e, 0x233f7061,
	0x3372f092, 0x8d937e41, 0xd65fecf1, 0x6c223bdb,
	0x7cde3759, 0xcbee7460, 0x4085f2a7, 0xce77326e,
	0xa6078084, 0x19f8509e, 0xe8efd855, 0x61d99735,
	0xa969a7aa, 0xc50c06c2, 0x5a04abfc, 0x800bcadc,
	0x9e447a2e, 0xc3453484, 0xfdd56705, 0x0e1e9ec9,
	0xdb73dbd3, 0x105588cd, 0x675fda79, 0xe3674340,
	0xc5c43465, 0x713e38d8, 0x3d28f89e, 0xf16dff20,
	0x153e21e7, 0x8fb03d4a, 0xe6e39f2b, 0xdb83adf7
};
static unsigned int ks2[] =
{
	0xe93d5a68, 0x948140f7, 0xf64c261c, 0x94692934,
	0x411520f7, 0x7602d4f7, 0xbcf46b2e, 0xd4a20068,
	0xd4082471, 0x3320f46a, 0x43b7d4b7, 0x500061af,
	0x1e39f62e, 0x97244546, 0x14214f74, 0xbf8b8840,
	0x4d95fc1d, 0x96b591af, 0x70f4ddd3, 0x66a02f45,
	0xbfbc09ec, 0x03bd9785, 0x7fac6dd0, 0x31cb8504,
	0x96eb27b3, 0x55fd3941, 0xda2547e6, 0xabca0a9a,
	0x28507825, 0x530429f4, 0x0a2c86da, 0xe9b66dfb,
	0x68dc1462, 0xd7486900, 0x680ec0a4, 0x27a18dee,
	0x4f3ffea2, 0xe887ad8c, 0xb58ce006, 0x7af4d6b6,
	0xaace1e7c, 0xd3375fec, 0xce78a399, 0x406b2a42,
	0x20fe9e35, 0xd9f385b9, 0xee39d7ab, 0x3b124e8b,
	0x1dc9faf7, 0x4b6d1856, 0x26a36631, 0xeae397b2,
	0x3a6efa74, 0xdd5b4332, 0x6841e7f7, 0xca7820fb,
	0xfb0af54e, 0xd8feb397, 0x454056ac, 0xba489527,
	0x55533a3a, 0x20838d87, 0xfe6ba9b7, 0xd096954b,
	0x55a867bc, 0xa1159a58, 0xcca92963, 0x99e1db33,
	0xa62a4a56, 0x3f3125f9, 0x5ef47e1c, 0x9029317c,
	0xfdf8e802, 0x04272f70, 0x80bb155c, 0x05282ce3,
	0x95c11548, 0xe4c66d22, 0x48c1133f, 0xc70f86dc,
	0x07f9c9ee, 0x41041f0f, 0x404779a4, 0x5d886e17,
	0x325f51eb, 0xd59bc0d1, 0xf2bcc18f, 0x41113564,
	0x257b7834, 0x602a9c60, 0xdff8e8a3, 0x1f636c1b,
	0x0e12b4c2, 0x02e1329e, 0xaf664fd1, 0xcad18115,
	0x6b2395e0, 0x333e92e1, 0x3b240b62, 0xeebeb922,
	0x85b2a20e, 0xe6ba0d99, 0xde720c8c, 0x2da2f728,
	0xd0127845, 0x95b794fd, 0x647d0862, 0xe7ccf5f0,
	0x5449a36f, 0x877d48fa, 0xc39dfd27, 0xf33e8d1e,
	0x0a476341, 0x992eff74, 0x3a6f6eab, 0xf4f8fd37,
	0xa812dc60, 0xa1ebddf8, 0x991be14c, 0xdb6e6b0d,
	0xc67b5510, 0x6d672c37, 0x2765d43b, 0xdcd0e804,
	0xf1290dc7, 0xcc00ffa3, 0xb5390f92, 0x690fed0b,
	0x667b9ffb, 0xcedb7d9c, 0xa091cf0b, 0xd9155ea3,
	0xbb132f88, 0x515bad24, 0x7b9479bf, 0x763bd6eb,
	0x37392eb3, 0xcc115979, 0x8026e297, 0xf42e312d,
	0x6842ada7, 0xc66a2b3b, 0x12754ccc, 0x782ef11c,
	0x6a124237, 0xb79251e7, 0x06a1bbe6, 0x4bfb6350,
	0x1a6b1018, 0x11caedfa, 0x3d25bdd8, 0xe2e1c3c9,
	0x44421659, 0x0a121386, 0xd90cec6e, 0xd5abea2a,
	0x64af674e, 0xda86a85f, 0xbebfe988, 0x64e4c3fe,
	0x9dbc8057, 0xf0f7c086, 0x60787bf8, 0x6003604d,
	0xd1fd8346, 0xf6381fb0, 0x7745ae04, 0xd736fccc,
	0x83426b33, 0xf01eab71, 0xb0804187, 0x3c005e5f,
	0x77a057be, 0xbde8ae24, 0x55464299, 0xbf582e61,
	0x4e58f48f, 0xf2ddfda2, 0xf474ef38, 0x8789bdc2,
	0x5366f9c3, 0xc8b38e74, 0xb475f255, 0x46fcd9b9,
	0x7aeb2661, 0x8b1ddf84, 0x846a0e79, 0x915f95e2,
	0x466e598e, 0x20b45770, 0x8cd55591, 0xc902de4c,
	0xb90bace1, 0xbb8205d0, 0x11a86248, 0x7574a99e,
	0xb77f19b6, 0xe0a9dc09, 0x662d09a1, 0xc4324633,
	0xe85a1f02, 0x09f0be8c, 0x4a99a025, 0x1d6efe10,
	0x1ab93d1d, 0x0ba5a4df, 0xa186f20f, 0x2868f169,
	0xdcb7da83, 0x573906fe, 0xa1e2ce9b, 0x4fcd7f52,
	0x50115e01, 0xa70683fa, 0xa002b5c4, 0x0de6d027,
	0x9af88c27, 0x773f8641, 0xc3604c06, 0x61a806b5,
	0xf0177a28, 0xc0f586e0, 0x006058aa, 0x30dc7d62,
	0x11e69ed7, 0x2338ea63, 0x53c2dd94, 0xc2c21634,
	0xbbcbee56, 0x90bcb6de, 0xebfc7da1, 0xce591d76,
	0x6f05e409, 0x4b7c0188, 0x39720a3d, 0x7c927c24,
	0x86e3725f, 0x724d9db9, 0x1ac15bb4, 0xd39eb8fc,
	0xed545578, 0x08fca5b5, 0xd83d7cd3, 0x4dad0fc4,
	0x1e50ef5e, 0xb161e6f8, 0xa28514d9, 0x6c51133c,
	0x6fd5c7e7, 0x56e14ec4, 0x362abfce, 0xddc6c837,
	0xd79a3234, 0x92638212, 0x670efa8e, 0x406000e0
};
static unsigned int ks3[] =
{
	0x3a39ce37, 0xd3faf5cf, 0xabc27737, 0x5ac52d1b,
	0x5cb0679e, 0x4fa33742, 0xd3822740, 0x99bc9bbe,
	0xd5118e9d, 0xbf0f7315, 0xd62d1c7e, 0xc700c47b,
	0xb78c1b6b, 0x21a19045, 0xb26eb1be, 0x6a366eb4,
	0x5748ab2f, 0xbc946e79, 0xc6a376d2, 0x6549c2c8,
	0x530ff8ee, 0x468dde7d, 0xd5730a1d, 0x4cd04dc6,
	0x2939bbdb, 0xa9ba4650, 0xac9526e8, 0xbe5ee304,
	0xa1fad5f0, 0x6a2d519a, 0x63ef8ce2, 0x9a86ee22,
	0xc089c2b8, 0x43242ef6, 0xa51e03aa, 0x9cf2d0a4,
	0x83c061ba, 0x9be96a4d, 0x8fe51550, 0xba645bd6,
	0x2826a2f9, 0xa73a3ae1, 0x4ba99586, 0xef5562e9,
	0xc72fefd3, 0xf752f7da, 0x3f046f69, 0x77fa0a59,
	0x80e4a915, 0x87b08601, 0x9b09e6ad, 0x3b3ee593,
	0xe990fd5a, 0x9e34d797, 0x2cf0b7d9, 0x022b8b51,
	0x96d5ac3a, 0x017da67d, 0xd1cf3ed6, 0x7c7d2d28,
	0x1f9f25cf, 0xadf2b89b, 0x5ad6b472, 0x5a88f54c,
	0xe029ac71, 0xe019a5e6, 0x47b0acfd, 0xed93fa9b,
	0xe8d3c48d, 0x283b57cc, 0xf8d56629, 0x79132e28,
	0x785f0191, 0xed756055, 0xf7960e44, 0xe3d35e8c,
	0x15056dd4, 0x88f46dba, 0x03a16125, 0x0564f0bd,
	0xc3eb9e15, 0x3c9057a2, 0x97271aec, 0xa93a072a,
	0x1b3f6d9b, 0x1e6321f5, 0xf59c66fb, 0x26dcf319,
	0x7533d928, 0xb155fdf5, 0x03563482, 0x8aba3cbb,
	0x28517711, 0xc20ad9f8, 0xabcc5167, 0xccad925f,
	0x4de81751, 0x3830dc8e, 0x379d5862, 0x9320f991,
	0xea7a90c2, 0xfb3e7bce, 0x5121ce64, 0x774fbe32,
	0xa8b6e37e, 0xc3293d46, 0x48de5369, 0x6413e680,
	0xa2ae0810, 0xdd6db224, 0x69852dfd, 0x09072166,
	0xb39a460a, 0x6445c0dd, 0x586cdecf, 0x1c20c8ae,
	0x5bbef7dd, 0x1b588d40, 0xccd2017f, 0x6bb4e3bb,
	0xdda26a7e, 0x3a59ff45, 0x3e350a44, 0xbcb4cdd5,
	0x72eacea8, 0xfa6484bb, 0x8d6612ae, 0xbf3c6f47,
	0xd29be463, 0x542f5d9e, 0xaec2771b, 0xf64e6370,
	0x740e0d8d, 0xe75b1357, 0xf8721671, 0xaf537d5d,
	0x4040cb08, 0x4eb4e2cc, 0x34d2466a, 0x0115af84,
	0xe1b00428, 0x95983a1d, 0x06b89fb4, 0xce6ea048,
	0x6f3f3b82, 0x3520ab82, 0x011a1d4b, 0x277227f8,
	0x611560b1, 0xe7933fdc, 0xbb3a792b, 0x344525bd,
	0xa08839e1, 0x51ce794b, 0x2f32c9b7, 0xa01fbac9,
	0xe01cc87e, 0xbcc7d1f6, 0xcf0111c3, 0xa1e8aac7,
	0x1a908749, 0xd44fbd9a, 0xd0dadecb, 0xd50ada38,
	0x0339c32a, 0xc6913667, 0x8df9317c, 0xe0b12b4f,
	0xf79e59b7, 0x43f5bb3a, 0xf2d519ff, 0x27d9459c,
	0xbf97222c, 0x15e6fc2a, 0x0f91fc71, 0x9b941525,
	0xfae59361, 0xceb69ceb, 0xc2a86459, 0x12baa8d1,
	0xb6c1075e, 0xe3056a0c, 0x10d25065, 0xcb03a442,
	0xe0ec6e0e, 0x1698db3b, 0x4c98a0be, 0x3278e964,
	0x9f1f9532, 0xe0d392df, 0xd3a0342b, 0x8971f21e,
	0x1b0a7441, 0x4ba3348c, 0xc5be7120, 0xc37632d8,
	0xdf359f8d, 0x9b992f2e, 0xe60b6f47, 0x0fe3f11d,
	0xe54cda54, 0x1edad891, 0xce6279cf, 0xcd3e7e6f,
	0x1618b166, 0xfd2c1d05, 0x848fd2c5, 0xf6fb2299,
	0xf523f357, 0xa6327623, 0x93a83531, 0x56cccd02,
	0xacf08162, 0x5a75ebb5, 0x6e163697, 0x88d273cc,
	0xde966292, 0x81b949d0, 0x4c50901b, 0x71c65614,
	0xe6c6c7bd, 0x327a140a, 0x45e1d006, 0xc3f27b9a,
	0xc9aa53fd, 0x62a80f00, 0xbb25bfe2, 0x35bdd2f6,
	0x71126905, 0xb2040222, 0xb6cbcf7c, 0xcd769c2b,
	0x53113ec0, 0x1640e3d3, 0x38abbd60, 0x2547adf0,
	0xba38209c, 0xf746ce76, 0x77afa1c5, 0x20756060,
	0x85cbfe4e, 0x8ae88dd8, 0x7aaaf9b0, 0x4cf9aa7e,
	0x1948c25c, 0x02fb8a8c, 0x01c36ae4, 0xd6ebe1f9,
	0x90d4f869, 0xa65cdea0, 0x3f09252d, 0xc208e69f,
	0xb74e6132, 0xce77e25b, 0x578fdfe3, 0x3ac372e6
};

struct Blowfish {
	unsigned long long S[4][256];
	unsigned long long P[18];
};

extern "C" __declspec(dllexport) void TelltaleToolLib_BlowfishEncrypt(unsigned char* data, unsigned int size, bool n, unsigned char* k);
extern "C" __declspec(dllexport) void TelltaleToolLib_BlowfishDecrypt(unsigned char* data, unsigned int size, bool n, unsigned char* k);
extern "C" __declspec(dllexport) const char* TelltaleToolLib_GetRawBlowfishKey(const char* gameID);
extern "C" __declspec(dllexport) bool TelltaleToolLib_DoesGameUseModifiedEncryption(const char* gameID);

/*
struct Blowfish {//singleton

	struct WordByte {
		int three : 8;
		int two : 8;
		int one : 8;
		int zero : 8;
	};

	union Word {
		unsigned int word;
		WordByte byte;
	};

	struct DWord {
		Word word1;
		Word word2;
	};

	unsigned int PA[18];
	unsigned int SB[4][256];
	bool mbInitialized;

	Blowfish();
	Blowfish(const char*);
	void Decrypt(void*, unsigned int, bool _New) {};
	void Encrypt(void*, unsigned int, bool _New) {};
	void Gen_Subkeys(const char*);
	void Init(const char*, bool _New);

	static Blowfish* Get();
	static void Initialize(const BlowfishKey*);
	static void Shutdown();

};*/

//extern Blowfish* spBlowfish;
extern unsigned int sSetKeyIndex;

const BlowfishKey sBlowfishKeys[KEY_COUNT] = {
	{ false,"texasholdem","" },//No key needed
	{ false,"boneville","\x82\xa3\x89\x88\x89\xd8\x9f\xb7\xd3\xd8\xda\xc0\x82\xd7\xc2\xc1\xce\x8d\xa1\xea\x99\xb7\xa5\xdd\xca\x52\xe5\x87\x69\xc8\xa4\x6e\xbb\x99\x97\xbb\xcd\xd7\x9a\xd8\xda\xd0\xc8\xde\xa6\x9c\xb7\xd2\xb9\xd6\x82\x75\xdf\xe0\xa4" },
	{ false,"csi3dimensions","" },//No key needed
	{ false,"cowrace","\x81\xD8\x9B\x99\x56\xE2\x65\x73\xB4\xDB\xE3\xC9\x64\xDB\x85\x87\xAB\x99\x9B\xDC\x6F\xEB\x68\x9F\xA7\x90\xDD\xBA\x6B\xE2\x93\x64\xA1\xB4\xA0\xB4\x93\xD9\x6B\x9C\xB7\xE3\xE6\xD1\x69\xA8\x84\x9F\x87\xD2\x94\x98\xA2\xE8\x71" },
	{ false,"sammax101","\x92\xCA\x9A\x81\x85\xE4\x64\x73\xA3\xBF\xD6\xD1\x7F\xC6\xCB\x88\x99\x5B\x80\xD8\xAA\xC2\x97\xE7\x96\x51\xA0\xA8\x9A\xD9\xAE\x95\xD7\x76\x62\x80\xB4\xC4\xA6\xB9\xD6\xEC\xA9\x9C\x68\x85\xB3\xDC\x92\xC4\x9E\x64\xA0\xA3\x92" },
	{ false,"sammax102","\x92\xCA\x9A\x81\x85\xE4\x64\x73\xA4\xBF\xD6\xD1\x7F\xC6\xCB\x88\x99\x01\x80\xD8\xAA\xC2\x97\xE7\x96\x51\xA1\xA8\x9A\xD9\xAE\x95\xD7\x76\x62\x81\xB4\xC4\xA6\xB9\xD6\xEC\xA9\x9C\x69\x85\xB3\xDC\x92\xC4\x9E\x64\xA0\xA4\x92" },
	{ false,"sammax103","\x92\xca\x9a\x81\x85\xe4\x64\x73\xa5\xbf\xd6\xd1\x7f\xc6\xcb\x88\x99\x5d\x80\xd8\xaa\xc2\x97\xe7\x96\x51\xa2\xa8\x9a\xd9\xae\x95\xd7\x76\x62\x82\xb4\xc4\xa6\xb9\xd6\xec\xa9\x9c\x6a\x85\xb3\xdc\x92\xc4\x9e\x64\xa0\xa5\x92" },
	{ false,"sammax104","\x92\xCA\x9A\x81\x85\xE4\x64\x73\xA6\xBF\xD6\xD1\x7F\xC6\xCB\x88\x99\x5E\x80\xD8\xAA\xC2\x97\xE7\x96\x51\xA3\xA8\x9A\xD9\xAE\x95\xD7\x76\x62\x83\xB4\xC4\xA6\xB9\xD6\xEC\xA9\x9C\x6B\x85\xB3\xDC\x92\xC4\x9E\x64\xA0\xA6\x92" },
	{ false,"sammax105","\x92\xCA\x9A\x81\x85\xE4\x64\x73\xA7\xBF\xD6\xD1\x7F\xC6\xCB\x88\x99\x5F\x80\xD8\xAA\xC2\x97\xE7\x96\x51\xA4\xA8\x9A\xD9\xAE\x95\xD7\x76\x62\x84\xB4\xC4\xA6\xB9\xD6\xEC\xA9\x9C\x6C\x85\xB3\xDC\x92\xC4\x9E\x64\xA0\xA7\x92" },
	{ false,"sammax106","\x92\xCA\x9A\x81\x85\xE4\x64\x73\xA8\xBF\xD6\xD1\x7F\xC6\xCB\x88\x99\x60\x80\xD8\xAA\xC2\x97\xE7\x96\x51\xA5\xA8\x9A\xD9\xAE\x95\xD7\x76\x62\x85\xB4\xC4\xA6\xB9\xD6\xEC\xA9\x9C\x6D\x85\xB3\xDC\x92\xC4\x9E\x64\xA0\xA8\x92" },
	{ false,"csihard","\x82\xbc\x76\x68\x67\xbf\x7c\x77\xb5\xbf\xbe\x98\x75\xb8\x9c\x8b\xac\x7d\x76\xab\x80\xc8\x7f\xa3\xa8\x74\xb8\x89\x7c\xbf\xaa\x68\xa2\x98\x7b\x83\xa4\xb6\x82\xa0\xb8\xc7\xc1\xa0\x7a\x85\x9b\xa3\x88\xb6\x6f\x67\xb3\xc5\x88" },
	{ false,"sammax201","\x92\xca\x9a\x81\x85\xe4\x65\x73\xa3\xbf\xd6\xd1\x7f\xc6\xcb\x89\x99\x5b\x80\xd8\xaa\xc2\x97\xe7\x97\x51\xa0\xa8\x9a\xd9\xae\x95\xd7\x77\x62\x80\xb4\xc4\xa6\xb9\xd6\xec\xaa\x9c\x68\x85\xb3\xdc\x92\xc4\x9e\x65\xa0\xa3\x92" },
	{ false,"sammax202","\x92\xCA\x9A\x81\x85\xE4\x65\x73\xA4\xBF\xD6\xD1\x7F\xC6\xCB\x89\x99\x01\x80\xD8\xAA\xC2\x97\xE7\x97\x51\xA1\xA8\x9A\xD9\xAE\x95\xD7\x77\x62\x81\xB4\xC4\xA6\xB9\xD6\xEC\xAA\x9C\x69\x85\xB3\xDC\x92\xC4\x9E\x65\xA0\xA4\x92" },
	{ false,"sammax203","\x92\xCA\x9A\x81\x85\xE4\x65\x73\xA5\xBF\xD6\xD1\x7F\xC6\xCB\x89\x99\x5D\x80\xD8\xAA\xC2\x97\xE7\x97\x51\xA2\xA8\x9A\xD9\xAE\x95\xD7\x77\x62\x82\xB4\xC4\xA6\xB9\xD6\xEC\xAA\x9C\x6A\x85\xB3\xDC\x92\xC4\x9E\x65\xA0\xA5\x92" },
	{ false,"sammax204","\x92\xCA\x9A\x81\x85\xE4\x65\x73\xA6\xBF\xD6\xD1\x7F\xC6\xCB\x89\x99\x5E\x80\xD8\xAA\xC2\x97\xE7\x97\x51\xA3\xA8\x9A\xD9\xAE\x95\xD7\x77\x62\x83\xB4\xC4\xA6\xB9\xD6\xEC\xAA\x9C\x6B\x85\xB3\xDC\x92\xC4\x9E\x65\xA0\xA6\x92" },
	{ false,"sammax205","\x92\xca\x9a\x81\x85\xe4\x65\x73\xa7\xbf\xd6\xd1\x7f\xc6\xcb\x89\x99\x5f\x80\xd8\xaa\xc2\x97\xe7\x97\x51\xa4\xa8\x9a\xd9\xae\x95\xd7\x77\x62\x84\xb4\xc4\xa6\xb9\xd6\xec\xaa\x9c\x6c\x85\xb3\xdc\x92\xc4\x9e\x65\xa0\xa7\x92" },
	{ false,"sbcg4ap101","\x87\xD8\x9A\x99\x97\xE0\x94\xB5\xA3\x9C\xA6\xAC\xA1\xD2\xB8\xCA\xDD\x8B\x9F\xA8\x6D\xA6\x7E\xDE\xD2\x86\xE2\xC9\x9A\xDE\x92\x64\x90\x8D\xA1\xBC\xC6\xD6\xAD\xCD\xE7\xA5\xA8\x9D\x7F\xA1\xBF\xD4\xB8\xD7\x87\xA5\xA1\xA2\x70" },//strong bad 101
	{ false,"sbcg4ap102","\x87\xd8\x9a\x99\x97\xe0\x94\xb5\xa3\x9c\xa7\xac\xa1\xd2\xb8\xca\xdd\x8b\x9f\xa8\x6d\xa7\x7e\xde\xd2\x86\xe2\xc9\x9a\xde\x92\x64\x91\x8d\xa1\xbc\xc6\xd6\xad\xcd\xe7\xa5\xa8\x9e\x7f\xa1\xbf\xd4\xb8\xd7\x87\xa5\xa1\xa2\x71" },
	{ false,"sbcg4ap103","\x87\xD8\x9A\x99\x97\xE0\x94\xB5\xA3\x9C\xA8\xAC\xA1\xD2\xB8\xCA\xDD\x8B\x9F\xA8\x6D\xA8\x7E\xDE\xD2\x86\xE2\xC9\x9A\xDE\x92\x64\x92\x8D\xA1\xBC\xC6\xD6\xAD\xCD\xE7\xA5\xA8\x9F\x7F\xA1\xBF\xD4\xB8\xD7\x87\xA5\xA1\xA2\x72" },
	{ false,"sbcg4ap104","\x87\xd8\x9a\x99\x97\xe0\x94\xb5\xa3\x9c\xa9\xac\xa1\xd2\xb8\xca\xdd\x8b\x9f\xa8\x6d\xa9\x7e\xde\xd2\x86\xe2\xc9\x9a\xde\x92\x64\x93\x8d\xa1\xbc\xc6\xd6\xad\xcd\xe7\xa5\xa8\xa0\x7f\xa1\xbf\xd4\xb8\xd7\x87\xa5\xa1\xa2\x73" },
	{ false,"sbcg4ap105","\x87\xd8\x9a\x99\x97\xe0\x94\xb5\xa3\x9c\xaa\xac\xa1\xd2\xb8\xca\xdd\x8b\x9f\xa8\x6d\xaa\x7e\xde\xd2\x86\xe2\xc9\x9a\xde\x92\x64\x94\x8d\xa1\xbc\xc6\xd6\xad\xcd\xe7\xa5\xa8\xa1\x7f\xa1\xbf\xd4\xb8\xd7\x87\xa5\xa1\xa2\x74" },
	{ false,"wag101","\x96\xCA\x99\xA0\x85\xCF\x98\x8A\xE4\xDB\xE2\xCD\xA6\x96\x83\x88\xC0\x8B\x99\xE3\x9E\xD8\x9B\xB6\xD7\x90\xDC\xBE\xAD\x9D\x91\x65\xB6\xA6\x9E\xBB\xC2\xC6\x9E\xB3\xE7\xE3\xE5\xD5\xAB\x63\x82\xA0\x9C\xC4\x92\x9F\xD1\xD5\xA4" },
	{ false,"wag102","\x96\xCA\x99\xA0\x85\xCF\x98\x8A\xE4\xDB\xE2\xCD\xA6\x96\x83\x89\xC0\x8B\x99\xE3\x9E\xD8\x9B\xB6\xD7\x90\xDC\xBE\xAD\x9D\x91\x66\xB6\xA6\x9E\xBB\xC2\xC6\x9E\xB3\xE7\xE3\xE5\xD5\xAB\x63\x82\xA1\x9C\xC4\x92\x9F\xD1\xD5\xA4" },
	{ false,"wag103","\x96\xCA\x99\xA0\x85\xCF\x98\x8A\xE4\xDB\xE2\xCD\xA6\x96\x83\x8A\xC0\x8B\x99\xE3\x9E\xD8\x9B\xB6\xD7\x90\xDC\xBE\xAD\x9D\x91\x67\xB6\xA6\x9E\xBB\xC2\xC6\x9E\xB3\xE7\xE3\xE5\xD5\xAB\x63\x82\xA2\x9C\xC4\x92\x9F\xD1\xD5\xA4" },
	{ false,"wag104","\x96\xCA\x99\xA0\x85\xCF\x98\x8A\xE4\xDB\xE2\xCD\xA6\x96\x83\x8B\xC0\x8B\x99\xE3\x9E\xD8\x9B\xB6\xD7\x90\xDC\xBE\xAD\x9D\x91\x68\xB6\xA6\x9E\xBB\xC2\xC6\x9E\xB3\xE7\xE3\xE5\xD5\xAB\x63\x82\xA3\x9C\xC4\x92\x9F\xD1\xD5\xA4" },
	{ false,"monkeyisland101","\x8C\xD8\x9B\x9F\x89\xE5\x7C\xB6\xDE\xCD\xE3\xC8\x63\x95\x84\xA4\xD8\x98\x98\xDC\xB6\xBE\xA9\xDB\xC6\x8F\xD3\x86\x69\x9D\xAE\xA3\xCD\xB0\x97\xC8\xAA\xD6\xA5\xCD\xE3\xD8\xA9\x9C\x68\x7F\xC1\xDD\xB0\xC8\x9F\x7C\xE3\xDE\xA0" },//note for me: edit ttarchive source if id is changed
	{ false,"monkeyisland102","\x8C\xD8\x9B\x9F\x89\xE5\x7C\xB6\xDE\xCD\xE3\xC8\x63\x95\x85\xA4\xD8\x98\x98\xDC\xB6\xBE\xA9\xDB\xC6\x8F\xD3\x86\x69\x9E\xAE\xA3\xCD\xB0\x97\xC8\xAA\xD6\xA5\xCD\xE3\xD8\xA9\x9C\x69\x7F\xC1\xDD\xB0\xC8\x9F\x7C\xE3\xDE\xA0" },
	{ false,"monkeyisland103","\x8C\xD8\x9B\x9F\x89\xE5\x7C\xB6\xDE\xCD\xE3\xC8\x63\x95\x86\xA4\xD8\x98\x98\xDC\xB6\xBE\xA9\xDB\xC6\x8F\xD3\x86\x69\x9F\xAE\xA3\xCD\xB0\x97\xC8\xAA\xD6\xA5\xCD\xE3\xD8\xA9\x9C\x6A\x7F\xC1\xDD\xB0\xC8\x9F\x7C\xE3\xDE\xA0" },
	{ false,"monkeyisland104","\x8c\xd8\x9b\x9f\x89\xe5\x7c\xb6\xde\xcd\xe3\xc8\x63\x95\x87\xa4\xd8\x98\x98\xdc\xb6\xbe\xa9\xdb\xc6\x8f\xd3\x86\x69\xa0\xae\xa3\xcd\xb0\x97\xc8\xaa\xd6\xa5\xcd\xe3\xd8\xa9\x9c\x6b\x7f\xc1\xdd\xb0\xc8\x9f\x7c\xe3\xde\xa0" },
	{ false,"monkeyisland105","\x8c\xd8\x9b\x9f\x89\xe5\x7c\xb6\xde\xcd\xe3\xc8\x63\x95\x88\xa4\xd8\x98\x98\xdc\xb6\xbe\xa9\xdb\xc6\x8f\xd3\x86\x69\xa1\xae\xa3\xcd\xb0\x97\xc8\xaa\xd6\xa5\xcd\xe3\xd8\xa9\x9c\x6c\x7f\xc1\xdd\xb0\xc8\x9f\x7c\xe3\xde\xa0" },
	{ false,"csideadly","\x82\xBC\x76\x69\x54\x9C\x86\x90\xD7\xDA\xEA\xA7\x85\xAE\x88\x87\x99\x7D\x7A\xDC\xAB\xEA\x79\xC2\xAE\x56\x9F\x85\x8C\xB9\xC6\xA2\xD4\x88\x85\x98\x96\x93\x69\xBF\xC2\xD9\xE6\xE1\x7A\x85\x9B\xA4\x75\x93\x79\x80\xD5\xE0\xB4" },
	{ false,"hector101","\x87\xCE\x90\xA8\x93\xDE\x64\x73\xA3\xB4\xDA\xC7\xA6\xD4\xC5\x88\x99\x5B\x75\xDC\xA0\xE9\xA5\xE1\x96\x51\xA0\x9D\x9E\xCF\xD5\xA3\xD1\x76\x62\x80\xA9\xC8\x9C\xE0\xE4\xE6\xA9\x9C\x68\x7A\xB7\xD2\xB9\xD2\x98\x64\xA0\xA3\x87" },
	{ false,"hector102","\x87\xCE\x90\xA8\x93\xDE\x64\x73\xA4\xB4\xDA\xC7\xA6\xD4\xC5\x88\x99\x01\x75\xDC\xA0\xE9\xA5\xE1\x96\x51\xA1\x9D\x9E\xCF\xD5\xA3\xD1\x76\x62\x81\xA9\xC8\x9C\xE0\xE4\xE6\xA9\x9C\x69\x7A\xB7\xD2\xB9\xD2\x98\x64\xA0\xA4\x87" },
	{ false,"hector103","\x87\xCE\x90\xA8\x93\xDE\x64\x73\xA5\xB4\xDA\xC7\xA6\xD4\xC5\x88\x99\x5D\x75\xDC\xA0\xE9\xA5\xE1\x96\x51\xA2\x9D\x9E\xCF\xD5\xA3\xD1\x76\x62\x82\xA9\xC8\x9C\xE0\xE4\xE6\xA9\x9C\x6A\x7A\xB7\xD2\xB9\xD2\x98\x64\xA0\xA5\x87" },
	{ false,"sammax301","\x92\xCA\x9A\x81\x85\xE4\x66\x73\xA3\xBF\xD6\xD1\x7F\xC6\xCB\x8A\x99\x5B\x80\xD8\xAA\xC2\x97\xE7\x98\x51\xA0\xA8\x9A\xD9\xAE\x95\xD7\x78\x62\x80\xB4\xC4\xA6\xB9\xD6\xEC\xAB\x9C\x68\x85\xB3\xDC\x92\xC4\x9E\x66\xA0\xA3\x92" },
	{ false,"sammax302","\x92\xCA\x9A\x81\x85\xE4\x66\x73\xA4\xBF\xD6\xD1\x7F\xC6\xCB\x8A\x99\x01\x80\xD8\xAA\xC2\x97\xE7\x98\x51\xA1\xA8\x9A\xD9\xAE\x95\xD7\x78\x62\x81\xB4\xC4\xA6\xB9\xD6\xEC\xAB\x9C\x69\x85\xB3\xDC\x92\xC4\x9E\x66\xA0\xA4\x92" },
	{ false,"sammax303","\x92\xCA\x9A\x81\x85\xE4\x66\x73\xA5\xBF\xD6\xD1\x7F\xC6\xCB\x8A\x99\x5D\x80\xD8\xAA\xC2\x97\xE7\x98\x51\xA2\xA8\x9A\xD9\xAE\x95\xD7\x78\x62\x82\xB4\xC4\xA6\xB9\xD6\xEC\xAB\x9C\x6A\x85\xB3\xDC\x92\xC4\x9E\x66\xA0\xA5\x92" },
	{ false,"sammax304","\x92\xCA\x9A\x81\x85\xE4\x66\x73\xA6\xBF\xD6\xD1\x7F\xC6\xCB\x8A\x99\x5E\x80\xD8\xAA\xC2\x97\xE7\x98\x51\xA3\xA8\x9A\xD9\xAE\x95\xD7\x78\x62\x83\xB4\xC4\xA6\xB9\xD6\xEC\xAB\x9C\x6B\x85\xB3\xDC\x92\xC4\x9E\x66\xA0\xA6\x92" },
	{ false,"sammax305","\x92\xCA\x9A\x81\x85\xE4\x66\x73\xA7\xBF\xD6\xD1\x7F\xC6\xCB\x8A\x99\x5F\x80\xD8\xAA\xC2\x97\xE7\x98\x51\xA4\xA8\x9A\xD9\xAE\x95\xD7\x78\x62\x84\xB4\xC4\xA6\xB9\xD6\xEC\xAB\x9C\x6C\x85\xB3\xDC\x92\xC4\x9E\x66\xA0\xA7\x92" },
	{ false,"grickle101","\x86\xDB\x96\x97\x8F\xD8\x98\x74\xA2\x9D\xBC\xD6\x9B\xC8\xBE\xC3\xCE\x5B\x5D\xA8\x84\xE7\x9F\xD2\xD0\x8D\xD4\x86\x69\x9D\xA8\xA6\xC8\xA8\x9D\xBB\xC6\x94\x69\x9D\xBC\xE6\xE1\xCF\xA2\x9E\xB7\xA0\x75\x94\x6D\xA5\xD9\xD5\xAA" },//puzzle agent 1 (scoggins)
	{ false,"csifatal","\x82\xBC\x76\x6A\x54\x9C\x76\x96\xBB\xA2\xA5\x94\x75\xB8\x9C\x8D\x99\x5A\x70\xCA\x86\xAB\x66\x9F\xA8\x74\xB8\x8B\x69\x9C\xA4\x87\xA8\x7B\x62\x7F\xA4\xB6\x82\xA2\xA5\xA4\xBB\xBF\x80\x68\x82\x9F\x88\xB6\x6F\x69\xA0\xA2\x82" },
	{ false,"celebritypoker","\x82\xCE\x99\x99\x86\xDE\x9C\xB7\xEB\xBC\xE4\xCF\x97\xD7\x96\xBC\xD5\x8F\x8F\xE9\xA6\xE9\xAF\xBF\xD4\x8C\xD4\xC7\x7C\xD1\xCD\x99\xC1\xB7\x9B\xC3\xDA\xB3\xA8\xD7\xDA\xE6\xBB\xD1\xA3\x97\xB4\xE1\xAE\xD7\x9F\x83\xDF\xDD\xA4" },//Poker Night at the inv
	{ false,"bttf101","\x81\xCA\x90\x9F\x78\xDB\x87\xAB\xD7\xB2\xEA\xD8\xA7\xD7\xB8\x88\x99\x5B\x6F\xD8\xA0\xE0\x8A\xDE\xB9\x89\xD4\x9B\xAE\xE0\xD6\xA6\xC4\x76\x62\x80\xA3\xC4\x9C\xD7\xC9\xE3\xCC\xD4\x9C\x78\xC7\xE3\xBA\xD5\x8B\x64\xA0\xA3\x81" },
	{ false,"bttf102","\x81\xca\x90\x9f\x78\xdb\x87\xab\xd7\xb2\xea\xd8\xa7\xd7\xb8\x88\x99\x01\x6f\xd8\xa0\xe0\x8a\xde\xb9\x89\xd4\x9b\xae\xe0\xd6\xa6\xc4\x76\x62\x81\xa3\xc4\x9c\xd7\xc9\xe3\xcc\xd4\x9c\x78\xc7\xe3\xba\xd5\x8b\x64\xa0\xa4\x81" },
	{ false,"bttf103","\x81\xCA\x90\x9F\x78\xDB\x87\xAB\xD7\xB2\xEA\xD8\xA7\xD7\xB8\x88\x99\x5D\x6F\xD8\xA0\xE0\x8A\xDE\xB9\x89\xD4\x9B\xAE\xE0\xD6\xA6\xC4\x76\x62\x82\xA3\xC4\x9C\xD7\xC9\xE3\xCC\xD4\x9C\x78\xC7\xE3\xBA\xD5\x8B\x64\xA0\xA5\x81" },
	{ false,"bttf104","\x81\xCA\x90\x9F\x78\xDB\x87\xAB\xD7\xB2\xEA\xD8\xA7\xD7\xB8\x88\x99\x5E\x6F\xD8\xA0\xE0\x8A\xDE\xB9\x89\xD4\x9B\xAE\xE0\xD6\xA6\xC4\x76\x62\x83\xA3\xC4\x9C\xD7\xC9\xE3\xCC\xD4\x9C\x78\xC7\xE3\xBA\xD5\x8B\x64\xA0\xA6\x81" },
	{ false,"bttf105","\x81\xCA\x90\x9F\x78\xDB\x87\xAB\xD7\xB2\xEA\xD8\xA7\xD7\xB8\x88\x99\x5F\x6F\xD8\xA0\xE0\x8A\xDE\xB9\x89\xD4\x9B\xAE\xE0\xD6\xA6\xC4\x76\x62\x84\xA3\xC4\x9C\xD7\xC9\xE3\xCC\xD4\x9C\x78\xC7\xE3\xBA\xD5\x8B\x64\xA0\xA7\x81" },
	{ false,"grickle102","\x86\xDB\x96\x97\x8F\xD8\x98\x74\xA2\x9E\xBC\xD6\x9B\xC8\xBE\xC3\xCE\x5B\x5D\xA9\x84\xE7\x9F\xD2\xD0\x8D\xD4\x86\x69\x9E\xA8\xA6\xC8\xA8\x9D\xBB\xC6\x94\x69\x9E\xBC\xE6\xE1\xCF\xA2\x9E\xB7\xA0\x75\x95\x6D\xA5\xD9\xD5\xAA" },//Puzzle agent 2
	{ true,"jurassicpark","\x89\xde\x9f\x95\x97\xdf\x9c\xa6\xc2\xcd\xe7\xcf\x63\x95\x83\xa1\xde\x9c\x8e\xea\xb0\xde\x99\xbf\xc6\x93\xda\x86\x69\x9c\xab\xa9\xd1\xa6\xa5\xc2\xca\xc6\x89\xcd\xe7\xdf\xa9\x9c\x67\x7c\xc7\xe1\xa6\xd6\x99\x9c\xd3\xc2\xa0" },
	{ false,"lawandorder","\x8B\xCA\xA4\x75\x92\xD0\x82\xB5\xD6\xD1\xE7\x95\x62\x95\x9F\xB8\xE0\x6B\x9B\xDB\x8C\xE7\x9A\xD4\xD7\x52\x9F\x85\x85\xCD\xD8\x75\xCD\xA9\x81\xC1\xC5\xC8\xAB\x9D\xA5\xA4\xC4\xCD\xAE\x73\xC0\xD3\x94\xD5\x8A\x98\xE2\xA3\x6F" },
	{ false,"twd1","\x96\xca\x99\x9f\x8d\xda\x9a\x87\xd7\xcd\xd9\x95\x62\x95\xaa\xb8\xd5\x95\x96\xe5\xa4\xb9\x9b\xd0\xc9\x52\x9f\x85\x90\xcd\xcd\x9f\xc8\xb3\x99\x93\xc6\xc4\x9d\x9d\xa5\xa4\xcf\xcd\xa3\x9d\xbb\xdd\xac\xa7\x8b\x94\xd4\xa3\x6f" },//Walking Dead new day, not for the series! this is an old one
	{ false,"celebritypoker2","\x82\xCE\x99\x99\x86\xDE\x9C\xB7\xEB\xBC\xE4\xCF\x97\xD7\x85\x9A\xCE\x96\x92\xD9\xAF\xDE\xAA\xE8\xB5\x90\xDA\xBA\xAB\x9E\xA4\x99\xCB\xAA\x94\xC1\xCA\xD7\xB2\xBC\xE4\xDF\xDD\xDE\x69\x75\xB7\xDB\xAA\xC5\x98\x9C\xE4\xEB\x8F" },//Poker Night 2
	{ true,"fables","\x85\xca\x8f\xa0\x89\xdf\x64\x73\xa2\xb2\xd6\xc6\x9e\xca\xc6\x88\x99\x5a\x73\xd8\x9f\xe1\x9b\xe2\x96\x51\x9f\x9b\x9a\xce\xcd\x99\xd2\x76\x62\x7f\xa7\xc4\x9b\xd8\xda\xe7\xa9\x9c\x67\x78\xb3\xd1\xb1\xc8\x99\x64\xa0\xa2\x85" },//wolf among us
	{ true,"wd2","\x96\xCA\x99\x9F\x8D\xDA\x9A\x87\xD7\xCD\xD9\x96\x62\x95\xAA\xB8\xD5\x95\x96\xE5\xA4\xB9\x9B\xD0\xC9\x53\x9F\x85\x90\xCD\xCD\x9F\xC8\xB3\x99\x93\xC6\xC4\x9D\x9E\xA5\xA4\xCF\xCD\xA3\x9D\xBB\xDD\xAC\xA7\x8B\x94\xD4\xA4\x6F" },//walking dead season 2
	{ true,"borderlands","\x81\xD8\x9F\x98\x89\xDE\x9F\xA4\xE0\xD0\xE8\x95\x62\x95\x95\xC6\xDB\x8E\x92\xE9\xA9\xD6\xA4\xD3\xD8\x52\x9F\x85\x7B\xDB\xD3\x98\xC4\xB7\x9E\xB0\xCF\xC7\xAC\x9D\xA5\xA4\xBA\xDB\xA9\x96\xB7\xE1\xB1\xC4\x94\x97\xE3\xA3\x6F" },
	{ true,"thrones","\x86\xCA\x9A\x99\x73\xD2\x87\xAB\xE4\xDB\xE3\xC9\xA5\x96\x83\x87\xB0\x8B\x9A\xDC\x8C\xDB\x8A\xD7\xD7\x90\xDD\xBA\xAC\x9D\x91\x64\xA6\xA6\x9F\xB4\xB0\xC9\x8D\xD4\xE7\xE3\xE6\xD1\xAA\x63\x82\x9F\x8C\xC4\x93\x98\xBF\xD8\x93" },
	{ true,"mcsm","\x8c\xd2\x9b\x99\x87\xde\x94\xa9\xe6\x9d\xa5\x94\x7f\xce\xc1\xbc\xcc\x9c\x8e\xdd\xb1\xa6\x66\x9f\xb2\x8a\xdd\xba\x9c\xde\xc2\x9a\xd3\x76\x62\x7f\xae\xcc\xa7\xd1\xd8\xe6\xd9\xd2\xab\x63\x82\x9f\x92\xcc\x94\x98\xd3\xe4\xa0" },
	{ true,"michonne","\x96\xca\x99\x9f\x8d\xda\x9a\x87\xd7\xcd\xd9\xb1\x63\x95\x83\xae\xca\x96\x98\xe0\xab\xdc\x7a\xd4\xc6\x85\xbc\x86\x69\x9c\xb8\x95\xcb\xb0\x9b\xbd\xc8\xa7\x9e\xcd\xd9\xc1\xa9\x9c\x67\x89\xb3\xdb\xb0\xcc\x94\x9a\xb4\xd7\xa0" },
	{ true,"batman","\x81\xca\xa1\xa1\x85\xda\x64\x73\xa2\xae\xd6\xd8\x9f\xc6\xc1\x88\x99\x5a\x6f\xd8\xb1\xe2\x97\xdd\x96\x51\x9f\x97\x9a\xe0\xce\x95\xcd\x76\x62\x7f\xa3\xc4\xad\xd9\xd6\xe2\xa9\x9c\x67\x74\xb3\xe3\xb2\xc4\x94\x64\xa0\xa2\x81" },
	{ true,"wd3","\x96\xca\x99\x9f\x8d\xda\x9a\x87\xd7\xcd\xd9\x97\x62\x95\xaa\xb8\xd5\x95\x96\xe5\xa4\xb9\x9b\xd0\xc9\x54\x9f\x85\x90\xcd\xcd\x9f\xc8\xb3\x99\x93\xc6\xc4\x9d\x9f\xa5\xa4\xcf\xcd\xa3\x9d\xbb\xdd\xac\xa7\x8b\x94\xd4\xa5\x6f" },//Walking Dead - A New Frontier
	{ true,"marvel","\x86\xDE\x8E\xA6\x88\xD5\x94\xB1\xE5\x9D\xA5\x94\x79\xDA\xB4\xC9\xCD\x93\x8E\xE5\xB0\xA6\x66\x9F\xAC\x96\xD0\xC7\x9D\xD5\xC2\xA2\xD2\x76\x62\x7F\xA8\xD8\x9A\xDE\xD9\xDD\xD9\xDA\xAA\x63\x82\x9F\x8C\xD8\x87\xA5\xD4\xDB\xA0" },
	{ true,"mc2","\x8c\xd2\x9b\x99\x87\xde\x94\xa9\xe6\x9e\xa5\x94\x7f\xce\xc1\xbc\xcc\x9c\x8e\xdd\xb1\xa7\x66\x9f\xb2\x8a\xdd\xba\x9c\xde\xc2\x9a\xd3\x77\x62\x7f\xae\xcc\xa7\xd1\xd8\xe6\xd9\xd2\xab\x64\x82\x9f\x92\xcc\x94\x98\xd3\xe4\xa0" },// mcsm - season 2
	{ true,"batman2","\x81\xCA\xA1\xA1\x85\xDA\x65\x73\xA2\xAE\xD6\xD8\x9F\xC6\xC1\x89\x99\x5A\x6F\xD8\xB1\xE2\x97\xDD\x97\x51\x9F\x97\x9A\xE0\xCE\x95\xCD\x77\x62\x7F\xA3\xC4\xAD\xD9\xD6\xE2\xAA\x9C\x67\x74\xB3\xE3\xB2\xC4\x94\x65\xA0\xA2\x81" },//enemy within
	{ true,"wd4", "\x96\xCA\x99\x9F\x8D\xDA\x9A\x87\xD7\xCD\xD9\x98\x62\x95\xAA\xB8\xD5\x95\x96\xE5\xA4\xB9\x9B\xD0\xC9\x55\x9F\x85\x90\xCD\xCD\x9F\xC8\xB3\x99\x93\xC6\xC4\x9D\xA0\xA5\xA4\xCF\xCD\xA3\x9D\xBB\xDD\xAC\xA7\x8B\x94\xD4\xA6\x6F"},//walking dead final season
	{ true,"wdc","\x96\xCA\x99\x9F\x8D\xDA\x9A\x87\xD7\xCD\xD9\xBB\x93\xD1\xBE\xC0\xD7\x91\x71\xDC\x9E\xD9\x8D\xD0\xD1\x8C\xD8\xC3\xA0\xB0\xC6\x95\xC3\x9C\x93\xBB\xCC\xCC\xA7\xD3\xB9\xD9\xD9\xD0\x8E\x93\xBE\xDA\xAE\xD1\x8D\x77\xD5\xD3\xA3" },//Walking Dead Collection / Definitive
	{ true,"sammaxremaster", "\x92\xCA\x9A\x81\x85\xE4\x64\x73\xA2\xBF\xD6\xD1\x7F\xC6\xCB\x88\x99\x5A\x80\xD8\xAA\xC2\x97\xE7\x96\x51\x9F\xA8\x9A\xD9\xAE\x95\xD7\x76\x62\x7F\xB4\xC4\xA6\xB9\xD6\xEC\xA9\x9C\x67\x85\xB3\xDC\x92\xC4\x9E\x64\xA0\xA2\x92"},
};

#endif