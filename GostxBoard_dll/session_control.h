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
* \file      session_control.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief	 API monitoring of a process
*
* \details   Define functions used by the api to check if the current running process is
*			 in foreground or background, allowing it to stop the cipher session depending
*			 on the state.
*
*
* \~French
* \file		session_control.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Suivie du processus par l'API
*
* \details	Définit des fonctions utilisées par l'API pour vérifier que le processus courant est
*			en premier ou arrière plan afin de réagir au mieux.
*/

#ifndef _SESSION_H
#define _SESSION_H

#include "../Commons/defines_common.h"


__declspec(dllexport) DWORD	GostxBoardIniateSession(void);
__declspec(dllexport) DWORD	GostxBoardBeginCipherSession(void);
DWORD	GostxBoardCloseCipherSessionEx(BYTE bFlag);
__declspec(dllexport) DWORD	GostxBoardCloseCipherSession(void);
DWORD RegenerateSession();
void  CriticalCleanSession(void);




#endif
