//**********************************************************************//
// 					This file is part of GostxBoard						//
//																		//
//  GostxBoard is free software: you can redistribute it and/or modify	//
//       it under the terms of the Creative Commons [Attribution-		//	
//	NonCommercial-ShareAlike 4.0 International] License as published	// 
//				 by the Ditribution of Creative Commons 				//		
//                Such things are STRONGLY recommended.					//
//    																	//
// 	GostxBoard is distributed in the hope that it will be useful,		//
//  but WITHOUT ANY WARRANTY; without even the implied warranty of		//	
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		//
//  	Creative Commons [BY-NC-SA] License for more details.			//	
//  Do not blame the developper if something goes wrong. You can 		//	
//  either search on the Internet or email the developper to deal 		//
//                             with it.									//
//																		//
//  You should have received a copy of the Creative Commons [BY-NC-SA]	// 
//	             License along with GostxBoard. If not, see 			//
//	  <https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode>. 	//
//																		//
//                              P.A 									//
//                gostxboard_support@whitekernel.fr 					//
//**********************************************************************//



#ifndef GOSTHASH_H
#define GOSTHASH_H

#define gosthash_rotl32(b, shift) ((b << shift) | (b >> (32 - shift)))

#define GOSTHASH_GOST_SBOX_SIZE	16
#define GOSTHASH_GOST_KEYSIZE		32

typedef unsigned char byte;

typedef short gst_word;
typedef int gst_dword;
typedef unsigned short gst_uword;
typedef unsigned int gst_udword;


typedef struct gosthash_s_box
{
	byte k8[GOSTHASH_GOST_SBOX_SIZE];
	byte k7[GOSTHASH_GOST_SBOX_SIZE];
	byte k6[GOSTHASH_GOST_SBOX_SIZE];
	byte k5[GOSTHASH_GOST_SBOX_SIZE];
	byte k4[GOSTHASH_GOST_SBOX_SIZE];
	byte k3[GOSTHASH_GOST_SBOX_SIZE];
	byte k2[GOSTHASH_GOST_SBOX_SIZE];
	byte k1[GOSTHASH_GOST_SBOX_SIZE];
} gosthash_s_box;

typedef struct
{
	gst_dword len;
	gst_udword left;
	byte H[32];
	byte S[32];
	byte remainder[32];
} gost_hash_ctx;

void GOSTHASH_init (gost_hash_ctx *ctx);
void GOSTHASH_add (byte *in, gst_udword len, gost_hash_ctx *ctx);
void GOSTHASH_finalize (gost_hash_ctx *ctx, byte *out);

#endif
