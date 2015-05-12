//******************************************//
//		   GostxBoard for GostCrypt			//
//		     Under GPL v.3 Licence			//
//											//
//    This software is distributed AS IS,	//
//    whithout any warranty and responsa-	//
//	bilities. Do not blame me if something	//
//	   goes wrong using it, but email me.	//
//											//
// Redistribution and modifications whithout//
//	permissions are STRONGLY recommended	//
//											//
//		           P.A						//
//		driver_support@whitekernel.fr		//
//******************************************//


/**
* \~English
* \file      tools.h
* \author    P.A
* \version   1.0
* \date      May 21, 2015
* \brief	 Header of tools.c
*
* \~French
* \file		tools.h
* \author	P.A
* \version  1.0
* \date		21 Mai 2015
* \brief	Header de tools.c
*
*/
#ifndef _TOOLS_H
#define _TOOLS_H

#include "../Commons/defines_common.h"



void * AllocateSafeMemory(DWORD * dwReturn, size_t size, BOOL bLock);
__declspec(dllexport) char * GostxBoardGetCodeMessage(DWORD dwCode);
DWORD VirtualSafeFreeMemory(void* ptr, size_t size, BOOL bLock);
__declspec(dllexport) DWORD GostxBoardRunSelfEncryptionTest(void);
__declspec(dllexport) DWORD GostxBoardDecipherChar(PUCHAR pChar);
_inline void PrintEx(char * Message, BYTE bLevel);


#endif // tools.c