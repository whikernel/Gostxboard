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
* \file      main.c
* \author	 Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Define the entry point of the DLL 
*
* \details   The file contains the entry point of the DLL and  the major function 
*			used oto decipher a scancode.
*
* \~French
* \file		main.c
* \author	 Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit le point d'entrée de la DLL
*
* \details	Ce fichier contient le point d'entrée de la DLL et la fonction principale
*			permettant de déchiffrer un scan-code.
*/

#include <stdio.h>
#include <stdlib.h>
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS

#include "../Commons/defines_common.h"
#include "../Commons/crypto.h"
#include "session_control.h"


extern PDEVICE_INFO	pdiDevice;


BOOL APIENTRY DllMain(	
		_In_	HANDLE hModule,
		_In_	DWORD ul_reason_for_call,
		_In_	LPVOID lpReserved
	)
{
	//
	// We have nothing to do here..
	//
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

