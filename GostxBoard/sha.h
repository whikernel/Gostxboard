//**********************************************************************//
// 					This file is part of GostxBoard						//
//																		//
//  GostxBoard is free software: you can redistribute it and/or modify	//
//  it under the terms of the GNU General Public License as published	// 
//	by the Free Software Foundation, either version 3 of the License, 	//
//	or (at your option) any later version. 								//
//                Such things are STRONGLY recommended.					//
//    																	//
// 	GostxBoard is distributed in the hope that it will be useful,		//
//  but WITHOUT ANY WARRANTY; without even the implied warranty of		//	
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		//
//  GNU General Public License for more details.						//	
//  Do not blame the developper if something goes wrong. You can 		//	
//  either search on the Internet or email the developper to deal 		//
//  with it.															//
//																		//
//  You should have received a copy of the GNU General Public License 	//
//  along with GostxBoard.  If not, see <http://www.gnu.org/licenses/>. //
//																		//
//                              P.A 									//
//                    driver_support@whitekernel.fr 					//
//**********************************************************************//
/**
* \~English
* \file      sha.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Defines for SHA256 computation
*
* \details   Defines for SHA256 computation. \n 
*			\a NB : The author of the driver is not the author of this file. Thanks to "stranger" for this implementation.
*
*
* \~French
* \file		sha.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Defines pour le calcul SHA256.
*
* \details	Defines pour le calcul SHA256. \n
*			\a NB : L'auteur du driver n'est pas l'auteur de ce fichier. Mer ci à "inconnu" pour cette implémentation.
*/


// Signed variables are for wimps 
#define uchar unsigned char // 8-bit byte
#define uint unsigned long // 32-bit word

typedef struct {
   uchar data[64];
   uint datalen;
   uint bitlen[2];
   uint state[8];
} SHA256_CTX;



void sha256_init(SHA256_CTX *ctx);

void sha256_update(SHA256_CTX *ctx, uchar data[], uint len);

void sha256_final(SHA256_CTX *ctx, uchar hash[]);