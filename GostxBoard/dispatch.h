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
/**
* \~English
* \file      dispatch.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Header of dispatch.c
*
* \~French
* \file		dispatch.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Header de dispatch.c
*
*/

#ifndef _DISPATCH_H
#define _DISPATCH_H

#include <ntifs.h>
#include <intrin.h>
#include <malloc.h>
#include <Ntddkbd.h>
#include <kbdmou.h>

#include "sha.h"
#include "defines.h"
#include "../Commons/defineKbFr.h"
#include "../Commons/defines_common.h"

#ifdef _DEPRECIATED

VOID GostxBoard_EvtIoRead(
	_In_  WDFQUEUE Queue,
	_In_  WDFREQUEST Request,
	_In_  size_t Length
	);

VOID GostxBoard_CompletionReadRoutine (
	_In_  WDFREQUEST Request,
	_In_  WDFIOTARGET Target,
	_In_  PWDF_REQUEST_COMPLETION_PARAMS Params,
	_In_  WDFCONTEXT Context
	);

#endif // depreciated 


#endif