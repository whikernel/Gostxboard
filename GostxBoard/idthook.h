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
//                                P.A 									//
//                    driver_support@whitekernel.fr 					//
//**********************************************************************//

/**
* \~English
* \file      idthook.h
* \author    P.A
* \version   1.0
* \date      May 21, 2015
* \brief     Header of idthook.c
*
*
* \~French
* \file		idthook.h
* \author	P.A
* \version  1.0
* \date		21 Mai 2015
* \brief	Header de idthook.c
*
*/

#ifndef _IDT_HOOK_H
#define _IDT_HOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <windowsx.h>

#include "dispatch.h"



void HookIsr(
	_In_ UINT16	Service ,
	_In_ UINT32	HookAddr
	);

UINT32	GetISRAddr(
	_In_	UINT16		Service
	);

PDESC	 GetDescriptorAddr(
	_In_	UINT16		Service
	);

#pragma warning (disable : 4276 )
void hookroutine();
void DbgPrintHook(UINT32 d);



#endif
