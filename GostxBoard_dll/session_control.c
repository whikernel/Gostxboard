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
* \file      session_control.c
* \author	 Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief	 API communication with the driver
* \details   Define functions used by the api to communicate with the driver. 
*
*
* \~French
* \file		session_control.c
* \author	 Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Communication API- Driver
*
* \details	Définit des fonctions utilisées par l'API pour communiquer avec le driver.  
*/

#include "session_control.h"
#include "driver.h"
#include "tools.h"
#include "../Commons/crypto.h"

extern PDEVICE_INFO	pdiDevice = NULL;
extern _inline void PrintEx(char *Message, BYTE bLevel);

//static char * MessagePdb = "Running self encryption tests.";

/**			  
*\~English 	  																	   
*\brief		[API] Allocate structures and check smooth connection with the driver  
*\details 	The function initializes the procedure of connection with the driver. \n 
*			It checks that all parameters and context are matching the expectations. It 
*			also activates the non sleeping mode of the computer, avoiding paging of critical 
*			information on the hard drive. \n
*			This function is API specific and is provided by the DLL.
*
*\param		void
*\return	DWORD  Returned code defined in defines_common
*
*\~French
*\brief		[API] Initialise les structures et vérifie la connection avec le driver	  
*\details	La fonction initialise la procédure de connexion avec le driver. \n 	  
*			Elle vérifie que tous les paramètres et le contexte sont en accords avec 
*			les exigences. Elle initialise aussi le flag de non mise en veille, afin de
*			d'empêcher la pagination d'informations critiques sur le disque. \n
*			Cette fonction est spécifique à l'API et est disponible via la DLL. 
*
*\param		void 
*\return	DWORD 	Code de retour définit dans defines_common
*/
DWORD	GostxBoardInitiateSession(
	PGOSTXBOARD_VERBOSE_CALLBACK VerboseCallback
	)
{
	DWORD				dwReturn	= RETURN_SUCCESS;
	BOOL				bReturn		= TRUE;

		//
		// Initialize structures needed	by the protection module. The memory is 
		//	protected as possible in user-mode memory. 
		//
		pdiDevice = (PDEVICE_INFO)AllocateSafeMemory(&dwReturn, sizeof(DEVICE_INFO), TRUE);
		if (!CODE_SUCCESS(dwReturn))
			goto end;;

		//
		// Check if a verbose callback needs to be registered
		// This callback will be called by the API each time a special event 
		// occurred
		//
		if (VerboseCallback)
			pdiDevice->VerboseCallback = VerboseCallback;

		PRINT_STD("Running self test.");
		dwReturn = GostxBoardRunSelfEncryptionTest();
		pdiDevice->dwLastReturnCode = dwReturn;
		if (!CODE_SUCCESS(dwReturn))
		{
			// SetLastError(ERROR_INVALID_DATA);
			goto end;
		}

		PRINT_STD("Running driver connection tests.");
		//
		// Check availibility of the driver and smooth connection with it
		//
	
		dwReturn = CheckDriverConnection();	 
		
		pdiDevice->dwLastReturnCode = dwReturn;
		if (!CODE_SUCCESS(dwReturn))
		{
			goto end;
		}
		else
		{
			PRINT_STD("Initialization completed with success.");
		}
	
	end:
	
	return dwReturn;
}



/**
*\~English
*\brief		[API] Initiate cipher session with the driver
*\details 	The function initiates the cipher session with the driver. \n 
*			The function dynamically allocates structures for the application's	 
*			credentials. \n	
*			The function #GostxBoardInitCipherSession MUST be called before in order to 
*			ensure safety and smooth running. \n
*			This function is API specific and is provided by the DLL.
*
*\param		void
*\return	DWORD  Returned code defined in defines_common
*
*\~French
*\brief		[API] Initialise la connection avec le driver
*\details	La fonction initialise la session de chiffrement avec le driver. \n	   
*			Elle alloue dynamiquement la structure contenant les clés de chiffrements
*			de l'application. \n
*			La fonction #GostxBoardInitCipherSession DOIT être appelée avant, afin 
*			d'assurer le bon fonctionnement du module de protection. \n
*			Cette fonction est spécifique à l'API et est fournis via la DLL. 
*
*\param		void
*\return	DWORD 	Code de retour définit dans defines_common
*/
DWORD	GostxBoardBeginCipherSession(void)
{
	DWORD				dwReturn	= RETURN_SUCCESS,
						dwBytesRead = 0;
	PPUBLIC_KEY_INIT	pkiPublic	= NULL,
						pkiReturn	= NULL;
	BOOL				bReturn = TRUE;

	__try {
		//
		// Check there were no previous errors
		//
		if (!CODE_SUCCESS(pdiDevice->dwLastReturnCode))
		{
			dwReturn = pdiDevice->dwLastReturnCode;
			__leave;
		}

		//
		//  Check that user has called the initialisation function 
		//	before, in order to  ensure maximum security
		//
		if (pdiDevice->hDevice == INVALID_HANDLE_VALUE)
		{
			dwReturn = ERR_SESSION_NOT_INITIALIZED;
			__leave;
		}

		//
		// The connection might be possible. We initiate required 
		// structures and allocates safe memory as possible. 
		// Lock memory to prevent pagination.
		//
		pkiPublic = (PPUBLIC_KEY_INIT)AllocateSafeMemory(&dwReturn, sizeof(PUBLIC_KEY_INIT), TRUE);
		if (!CODE_SUCCESS(dwReturn) || !PTR_VALID(pkiPublic))
			__leave;

		pkiReturn = (PPUBLIC_KEY_INIT)AllocateSafeMemory(&dwReturn, sizeof(PUBLIC_KEY_INIT), TRUE);
		if (!CODE_SUCCESS(dwReturn) || !PTR_VALID(pkiReturn))
			__leave;


		//
		// Activates the non sleeping mode of the computer, avoiding paging of critical
		// information on the hard drive.
		//
		SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);

		//
		// Fill fields structures sent to the driver
		//
		pkiPublic->dwError = NOERROR;
		pkiPublic->guidApp = APP;
		pkiPublic->bCipherStarted = FALSE;

		//
		// Send the request to the driver 
		// to initiate the cipher session
		//
		bReturn = DeviceIoControl(pdiDevice->hDevice,
			(ULONG)IOCTL_INIT_KEY,
			pkiPublic, sizeof(PUBLIC_KEY_INIT),
			pkiReturn, sizeof(PUBLIC_KEY_INIT),
			&dwBytesRead,
			NULL);
		if (!bReturn)
		{
			CloseHandle(pdiDevice->hDevice);
			dwReturn = ERR_CONNECTION_FAILURE;
			__leave;
		}

		//
		// The driver fills the error code itself
		//
		dwReturn = pkiReturn->dwError;

		//
		// If there is no errors, we save credentials 
		//
		if (CODE_SUCCESS(dwReturn))
		{
			RtlCopyMemory(&pdiDevice->pkiPublic, pkiReturn, sizeof(PUBLIC_KEY_INIT));
			NOTIFY_START(dwReturn);	
		}
	}
	__finally  {
			//
			// Safely cleans the application credentials
			//
			VirtualSafeFreeMemory(pkiPublic, sizeof(PUBLIC_KEY_INIT), TRUE);
			VirtualSafeFreeMemory(pkiReturn, sizeof(PUBLIC_KEY_INIT), TRUE);
			pdiDevice->dwLastReturnCode = dwReturn;
				
	} 
	return dwReturn;
}




DWORD	GostxBoardCloseCipherSession()
{
	return GostxBoardCloseCipherSessionEx(1);
}



/**	 
*\~English
*\brief		[API] Close cipher session with the driver
*\details 	The function closes the cipher session with the driver. \n 
*			The function sends a closing IOCTL to the driver and then 
*			cleans up structures. \n
*			This function is API specific and is provided by the DLL.
*
*\param		void
*\return	DWORD  Returned code defined in defines_common
*
*\~French
*\brief		[API] Termine la connection avec le driver
*\details	La fonction termine la session de chiffrement avec le driver. \n
*			La fonction envoie un IOCTL de fermeture de session au driver puis
*			nettoie les structures comportant les informations de session. \n
*			Cette fonction est spécifique à l'API et est fournis via la DLL.
*
*\param		void
*\return	DWORD 	Code de retour définit dans defines_common
*/
DWORD	GostxBoardCloseCipherSessionEx(
	BYTE	bFlag
	)
{
	DWORD					dwReturn			= RETURN_SUCCESS,
							dwBytesRead			= 0;
	PPUBLIC_CLOSE_SESSION   pcsCloseSession		= NULL,
							pcsCloseReturn		= NULL;
	BOOL					bReturn				= FALSE;

	PRINT_WARN("Cipher session is going to be closed.");
	//
	// Check if session exists 
	//
	if (!pdiDevice->pkiPublic.bCipherStarted)
	{
		goto retn;
	}

	//
	// Allocate return structure which will be sent within IOCTL in order 
	// to notify the driver that session must be closed.
	//
	pcsCloseSession = (PPUBLIC_CLOSE_SESSION)AllocateSafeMemory(&dwReturn, 
		sizeof(PUBLIC_CLOSE_SESSION), FALSE);
	if (!PTR_VALID(pcsCloseSession))
		goto end;
	pcsCloseReturn = (PPUBLIC_CLOSE_SESSION)AllocateSafeMemory(&dwReturn,
		sizeof(PUBLIC_CLOSE_SESSION), FALSE);
	if (!PTR_VALID(pcsCloseReturn))
		goto end;

	//
	// Initiate structure fields before sending it
	//
	pcsCloseSession->bAppisDeleted = FALSE;
	pcsCloseSession->guidApp = APP;
	pcsCloseSession->Unused = NULL;

	//
	// Send the request to the driver
	//
	bReturn = DeviceIoControl(pdiDevice->hDevice,
		(ULONG)IOCTL_CLOSE_KEY,
		pcsCloseSession, sizeof(PUBLIC_CLOSE_SESSION),
		pcsCloseReturn, sizeof(PUBLIC_CLOSE_SESSION),
		&dwBytesRead, NULL);
	if (!bReturn)
	{
		CloseHandle(pdiDevice->hDevice);
		dwReturn = ERR_CONNECTION_FAILURE;
		goto end;
	}

	//
	// Check that session is well closed
	//
	if (!pcsCloseReturn->bAppisDeleted)
	{
		dwReturn = CRIT_ERR_CIPHER_STILL_RUNNING;
	}

	dwReturn = pcsCloseReturn->dwError;

	//
	// Clean the session's credentials 
	//
	CriticalCleanSession();

end:
	//
	// Clean up structures and stop monitoring thread if exists
	//
	VirtualSafeFreeMemory(pcsCloseSession, sizeof(PUBLIC_CLOSE_SESSION), FALSE);
	VirtualSafeFreeMemory(pcsCloseReturn, sizeof(PUBLIC_CLOSE_SESSION), FALSE);
	SetThreadExecutionState(ES_CONTINUOUS);

	if (bFlag && (pdiDevice->hMonitorThread != INVALID_HANDLE_VALUE))
	{
		PRINT_WARN("Monitor thread will be closed");
		// SetEvent(pdiDevice->hExitEvent);
		pdiDevice->hExitEvent = 1;
		WaitForSingleObject(pdiDevice->hMonitorThread, 350);
		if (pdiDevice->hMonitorThread != INVALID_HANDLE_VALUE)
			TerminateThread(pdiDevice->hMonitorThread, EXIT_SUCCESS);
	}

retn:
	PRINT_STD("Session is closed.");
	NOTIFY_STOP(dwReturn);
	pdiDevice->dwLastReturnCode = dwReturn;
	return dwReturn;
}

/**
*\~English
*\brief		[API] Regenerate a cipher session
*\details 	The function closes the current session,resets the
*			streamcipher and then  restart a new session. 
*
*\param		void
*\return	DWORD	Returned code defined in defines_common
*
*\~French
*\brief		[API] Regénére une session courante
*\details	La fonction ferme la sessio courante, réinitilise le 
*			streamcipher et  relance une nouvelle session.
*\param		void
*\return	DWORD	Code de retour définit dans defines_common
*/
DWORD RegenerateSession()
{
	DWORD	dwReturn = 0;

	__try {

		//
		// Close the current session
		//
		dwReturn = GostxBoardCloseCipherSession();
		if (!CODE_SUCCESS(dwReturn))  {
			__leave;
		}

		//
		// Clean all 
		//
		CriticalCleanSession();

		//
		// Restart a new cipher session
		//
		dwReturn = GostxBoardBeginCipherSession();
	}
	__finally {

		pdiDevice->dwLastReturnCode = dwReturn;

	}
	

	return dwReturn;
}



/**
*\~English
*\brief		[API] Clean the current session	  
*\details 	The function cleans the current cipher session by reseting the 
*			streamcipher and safely cleaning the app's credentials.
*			This function is API specific. 
*
*\param		void
*\return	void
*
*\~French
*\brief		[API] Nettoie la session courante 
*\details	La fonction nettoie la session courante en réinitialisant le 
*			streamcipher et en nettoyant de manière sécurisée les 
*			identifiants de l'application. 
*
*\param		void
*\return	void
*/
void  CriticalCleanSession(void)
{
	DWORD			dwIndex		= 0;
	volatile char * cMemBurn	= 0;
	int				dwBurn		= 0;

	//
	// Reinitilize the permutation table and parameters
	//
	ResetShuffleTab();
	StreamCipher(0, 0);
	pdiDevice->dwNumberOfKeys = 0;
	CloseHandle(pdiDevice->hDevice);

	//
	// Safely erase the app's credentials
	//
	RtlSecureZeroMemory(&(pdiDevice->pkiPublic), sizeof(PUBLIC_KEY_INIT));
	do {
		cMemBurn = (volatile char *)(&(pdiDevice->pkiPublic));
		dwBurn = sizeof(PUBLIC_KEY_INIT);
		RtlSecureZeroMemory(&(pdiDevice->pkiPublic), sizeof(PUBLIC_KEY_INIT));
		while (dwBurn--) *cMemBurn++ = 0;
	} while (0);   

	return;
}