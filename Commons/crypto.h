//**********************************************************************//
// 					This file is part of GostxBoard						//
//																		//
//  GostxBoard is free software: you can redistribute it and/or modify	//
//   it under the terms of the GNU General Public License as published 	//
// 	 by the Free Software Foundation, either version 3 of the licence,  //
//						  or any later version.							//	
//                Such things are STRONGLY recommended.					//
//    																	//
// 	  GostxBoard is distributed in the hope that it will be useful,		//
//   but WITHOUT ANY WARRANTY; without even the implied warranty of		//	
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		//
//  	      GNU General Public License for more details.		    	//	
//  Do not blame the developper if something goes wrong. You can 		//	
//  either search on the Internet or email the developper to deal 		//
//                             with it.									//
//																		//
//    You should have received a copy of GNU General Public License   	// 
//	                along with GostxBoard. If not, see 		    	    //
//	           <http://www.gnu.org/licenses/gpl-3.0.fr.html>.       	//
//																		//
//                              P.A 									//
//                gostxboard_support@whitekernel.fr 					//
//**********************************************************************//

/**
* \~English
* \file      crypto.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Header of crypto.c
*
* \~French
* \file		crypto.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Header de crypto.c
*
*/

#ifndef	_CRYPTO_H
#define	_CRYPTO_H

#include "defines_common.h"
#include "../Commons/defineKbFr.h"
#include <stdlib.h>



#ifdef _DRIVER_
#include "defines.h"
#include "functions.h"
#include "controlfunctions.h"
#elif _APP_
#include <ntstatus.h>
#endif




NTSTATUS CipherKey(PUCHAR MakeCode,PVOID	Unused);
unsigned long long StreamCipher( unsigned long long iKey, int Flags );
USHORT ShuffleTab(USHORT ScanCode);
VOID ResetShuffleTab();
DWORD isElementPresent(KEY_MAP cIdTab[], DWORD dwSizeOfTab, USHORT usElement);


#ifdef _DRIVER_
unsigned long long GenerateKey();
unsigned long long InitMasterKey();
void CipherScanCode( PKEYBOARD_INPUT_DATA pkInKeyData, PUCHAR DataByte);
DWORD RunSelfTest(void);

#elif defined(_APP_) || defined(_DLL_)
USHORT DecipherKey( USHORT usInKey);
USHORT CipherKeyTestMode(USHORT	usInKey);
	#ifdef _DEV_MODE
__declspec(dllexport) void GostxBoardRegenerateStaticTestVectors(void);
	#endif // dev mode
#endif	// APP DLL

#endif	 // crypto . h