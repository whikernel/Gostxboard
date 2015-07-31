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
* \file      monitor.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Header of monitor.c
* \~French
* \file		monitor.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Header de monitor.c
*
*/

#ifndef _MONITOR_H
#define _MONITOR_H

#include "../Commons/defines_common.h"

__declspec(dllexport) DWORD GostxBoardStartMonitorProcessusState( LPDWORD lpdwProcessId, LPDWORD lpdwReturn);
DWORD WINAPI MonitorProc(LPVOID lpParameter);


typedef struct _THREAD_PARAMETERS_ {
	LPDWORD lpdwProcessID;						/*<! \~English A pointer to the ID of the process to monitor. \~French Un pointeur vers l'ID du thread à monitorer. */
	LPDWORD	lpdwReturn;							/*<! \~English A pointer to the returned values of the thread's functions. \~French Un pointeur vers les valeurs de retour des fonctions du thread. */
	LPBYTE	hExit;								/*!< \~English Event on exit signal. This is used by the API to stop the monitoring thread. \~French Evénement sur le signal de sortie. Il est utilisé par l'API  */
} THREAD_PARAMETERS, *LPTHREAD_PARAMETERS;

#endif // Monitor