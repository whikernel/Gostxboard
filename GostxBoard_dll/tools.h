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
* \file      tools.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief	 Header of tools.c
*
* \~French
* \file		tools.h
* \author	Paul Amicelli
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