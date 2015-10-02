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
* \file      monitor.c
* \author	 Paul Amicelli
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
* \file		monitor.c
* \author	 Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Suivie du processus par l'API
*
* \details	Définit des fonctions utilisées par l'API pour vérifier que le processus courant est 
*			en premier ou arrière plan afin de réagir au mieux. 
*/


#include "..//Commons/defines_common.h"
#include "monitor.h"
#include "session_control.h"

THREAD_PARAMETERS	tpParams = { 0 };

extern 	PDEVICE_INFO	pdiDevice;


/**
*\~English
*\brief		[API] Monitor the given process
*\details 	The function creates an auxiliary thread used to monitor the process
*			given in parameter. \n
*			This function is API specific and is provided by the DLL.
*
*\param		LPDWORD	lpdwProcessId ID of the process to monitor
*\param		LPDWORD lpdwdwReturn Code returned by the thread's function
*\return	DWORD  Return code defined in defines_common
*
*\~French
*\brief		[API] Monitor un processus donné
*\details	La fonction créée un thread annexe afin de monitorer le processus
*			donné en paramètre. \n
*			Cette fonction est spécifique à l'API et est fournis via la DLL. 
*
*\param		LPDWORD	lpdwProcessId ID du processus à surveiller
*\param		LPDWORD lpdwdwReturn Code de retourné par la fonction du thread
*\return	DWORD  Code de retour définit dans defines_common
*/
DWORD GostxBoardStartMonitorProcessusState(
	_In_	LPDWORD		lpdwProcessId,
	_In_	LPDWORD		lpdwReturn
	)
{
	DWORD				dwReturn		= RETURN_SUCCESS,
						dwThreadID		= 0;
	HANDLE				hThread			= NULL,
						hProcessCaller	= NULL;

	//
	// Check if the pointer is valid
	//
	if (!PTR_VALID(lpdwProcessId))
	{
		dwReturn = ERR_BAD_PARAMETERS;
		goto end;
	}

	//
	// Check the rights on the process are sufficient to 
	// create a new thread
	//
	hProcessCaller = OpenProcess(PROCESS_CREATE_THREAD,
		TRUE, *lpdwProcessId);
	if(!hProcessCaller)
	{
		dwReturn = ERR_NO_CREATE_THREAD_RIGHTS;
		goto end;
	}

	//
	// Initiate thread's structure
	//
	tpParams.lpdwProcessID = lpdwProcessId;
	tpParams.lpdwReturn = lpdwReturn;

	//
	// Create the closing event 
	//
	pdiDevice->hExitEvent = 0;
	tpParams.hExit = &pdiDevice->hExitEvent;

	//
	// Create the annexe thread in order to monitor the 
	// specified process and store the handle for later	use
	//
	PRINT_STD("Starting monitor thread.");
	hThread = CreateThread(NULL,
		0, MonitorProc,
		&tpParams, 0, &dwThreadID);
	if (hThread == INVALID_HANDLE_VALUE)
	{
		dwReturn = ERR_CREATE_THREAD_FAILURE;
		CloseHandle(tpParams.hExit);
	}

	pdiDevice->hMonitorThread = hThread;

end:
	pdiDevice->dwLastReturnCode = dwReturn;
	return dwReturn;
}



/**
*\~English
*\brief		[API] Thread monitoring function
*\details 	The function loops and check every 400ms that the given process is in foreground
*			or background. \n
*			If the process is not in foreground then, the current session is closed. \n 
*			If the process is in foreground and no session is started, then a session is started. \n
*			This function is API specific. 
*
*\param		LPVOID	lpParameter A pointer to the thread parameters
*\return	DWORD   Returned value for thread
*
*\~French
*\brief		[API] Fonction de monitoring du thread
*\details	La fonction boucle et vérifie toutes les 400ms si le processus donné est en background ou 
*			en foreground. \n
*			Si le processus est en arrière plan et qu'une session est ouverte, il la ferme.  \n
*			Si le processus est en premier plan et qu'aucune session n'est ouverte, il en ouvre une. \n
*			Cette fonction est spécifique à l'API.
*
*\param		LPVOID	lpParameter Un pointeur vers les paramètres du thread
*\return	DWORD   Valeur de retour du thread
*/
DWORD WINAPI MonitorProc(
	_In_ LPVOID		lpParameter
	)
{
	DWORD					dwReturn			= RETURN_SUCCESS,
							dwProcessID			= 0,
							dwCurrentProcessID	= 0;
	LPTHREAD_PARAMETERS		ltpParams			= NULL;
	BOOL					bInterrupted		= FALSE;

	ltpParams = (LPTHREAD_PARAMETERS)lpParameter;
	if ( !PTR_VALID(ltpParams)  )
	{
		dwReturn = ERR_BAD_PARAMETERS;
		goto end;
	}

	while(TRUE)
	{
		if (!*(ltpParams->hExit))
		{
			//
			// Check that current processus is the same as the active window one
			//
			GetWindowThreadProcessId(GetForegroundWindow(), &dwProcessID);
			dwCurrentProcessID = *ltpParams->lpdwProcessID;

			if ((dwProcessID != dwCurrentProcessID) && !bInterrupted)
			{
				//
				// The application is not anymore in foreground
				//
				PRINT_WARN("Window focus is lost");
				dwReturn = GostxBoardCloseCipherSessionEx(0);
				if (!CODE_SUCCESS(dwReturn))
				{
					//
					// Closing session seems to be impossible. 
					// This is a criticial error. Cipher on the driver is 
					// probably still running. But we cannot do anything.
					// Just clean and notify. 
					//
					CriticalCleanSession();
					PRINT_WARN("Unable to close session");
					dwReturn = CRIT_ERR_CIPHER_STILL_RUNNING;
					goto end;
				}

				//
				// We clean the session
				//
				CriticalCleanSession();

				//
				// We indicate that the session has been interrupted
				//
				bInterrupted = TRUE;
			}
			else if ((dwProcessID == dwCurrentProcessID) && bInterrupted)
			{
				//
				// The window is active but session is stopped, we need 
				// to retart one.
				//
				PRINT_STD("Session reconnected.");
				dwReturn = GostxBoardBeginCipherSession();
				if (!CODE_SUCCESS(dwReturn))
				{
					//
					// We cannot restart the session
					//
					goto end;
				}
				bInterrupted = FALSE;
			}
		}
		Sleep(300);
	}

end:
	*ltpParams->lpdwReturn = dwReturn;
	return 0;
}