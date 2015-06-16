//**********************************************************************//
// 					This file is part of GostCrypt						//
//																		//
//  GostxCrypt is free software: you can redistribute it and/or modify	//
//  it under the terms of the TrueCrypt Collective License.				//
//                Such things are STRONGLY recommended.					//
//    																	//
// 	 GostCrypt is distributed in the hope that it will be useful,		//
//  but WITHOUT ANY WARRANTY; without even the implied warranty of		//	
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		//
//  GNU General Public License for more details.						//	
//  Do not blame the developpers if something goes wrong. You can 		//	
//  either search on the Internet or email the developpers to deal 		//
//  with it.															//
//																		//
//  You should have received a copy of the TrueCrypt Public License 	//
//    along with GostCrypt.  If not, see <http://gostcrypt.org/>.		//
//																		//
//                              P.A 									//
//						support@gostcrypt.org 							//
//**********************************************************************//

/**
* \~English
* \file      functions.c
* \author    GostCrypt Foundation - P.A
* \version   1.0
* \date      May 21, 2015
* \brief     Defines some usefull functions
*
* \details   Defines some usefull functions like tools and so on.
*
* \~French
* \file		functions.c
* \author	GostCrypt Foundation - P.A
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit des fonctions utilitaires
*
* \details	Définit des fonctions utilitaires pour le driver.
*/

#include "functions.h"
#include "../Commons/crypto.h"

extern PAPP_CREDENTIALS	pCurrentApp;



/**
* \~English
* \brief    SHA256 Hash Function
* \details  Compute the SHA256 hash of the cKey given. 
*
* \param	char * cKey Key to hash
* \param	int keyLength	Size of the key to hash
* \param    usigned char usResult[32]	Output hash
* \return   void
*
* \~French
* \brief    Fonction de hashage SHA256
* \details  Calcule le hash de la clé cKey passé en paramètre.
*
* \param	char * cKey Clé à hasher
* \param	int keyLength	Taille de la clé
* \param    usigned char usResult[32]	Hash de la clé
* \return   void
*
*/
/*
VOID HashKey( char * cKey, 
			 int keyLength, 
			 unsigned char usResult[32]  )
{
	SHA256_CTX	ctx;
	#ifdef _DEBUG
		DbgPrint("[+] KbdCrypt_HashKey::Entry \n");
	#endif

	#ifdef _DEBUG
		DbgPrint("[+] KbdCrypt_HashKey::Initiate \n");
	#endif
	sha256_init(&ctx);
	#ifdef _DEBUG
		DbgPrint("[+] KbdCrypt_HashKey::Update \n");
	#endif
	sha256_update( &ctx, 
		(unsigned char *) cKey,
		keyLength );
	#ifdef _DEBUG
		DbgPrint("[+] KbdCrypt_HashKey::End\n");
	#endif
	sha256_final( &ctx, usResult);

	#ifdef _DEBUG
		DbgPrint("[+] KbdCrypt_HashKey::Hash Generated \n");
	#endif
	
}
*/

/**
* \~English
* \brief    Process Watcher
* \details  This function is called each time a process is created or 
*			is dying. This allows us to monitor the client application 
*			and detect if it dies unexpectedly.
*
* \param	HANDLE	parentId		An handle on the parent's process
* \param	HANDLE	processId		An handle on the process
* \param	BOOLEAN	create			A boolean telling if this is a creating or a death.
* \return   void
*
* \~French
* \brief    Monitoring des processus
* \details  Cette fonction est appelée à chaque fois qu'un processus est 
*			créé ou meurt. Cela permet de monitorer l'application cliente 
*			et détecter si elle meurt de manière inattendue.
*
* \param	HANDLE	parentId		Un handle sur le processus parent
* \param	HANDLE	processId		Un handle sur le processus
* \param	BOOLEAN	create			Un booléen indiquant la création ou la mort du processus 
* \return   void
*
*/
VOID	ProcessNotifyRoutine (
	_In_ HANDLE		parentId,
	_In_ HANDLE		processId,
	_In_ BOOLEAN	create )
{
	UNREFERENCED_PARAMETER( parentId);
	if( create )
	{
		//
		// Creation of the process
		//
	} else {
		//
		// Check if this is the application to monitor
		//
		if ( processId == pCurrentApp->hProcessId )
		{
			//
			// This is our application, which is now dead. We quickly reset all and 
			// stop the cipher session
			//
			ResetCipher();
			#ifdef _DEBUG
				DbgPrint("[+] KbdCrypt_ProcessNotify::Client app with PID %d seems to be dead.\n", processId);
				DbgPrint("[+] KbdCrypt_ProcessNotify::Cleaning cipher session.\n");
			#endif
		}

	}
	return;
}

/**
* \~English
* \brief    Security Watcher
* \details  This function is called each time the driver wants to know the security 
*			contexte in which it is running. 
*
* \param	void
* \return   DWORD	Return code defined in defines_common
*
* \~French
* \brief    Monitoring des processus
* \details  Cette fonction est appelée à chaque fois que le driver veut vérifier 
*			le contexte de sécurité dans lequel il s'exécute. 
*
* \param	void
* \return   DWORD	Code de retour définit dans defines_common
*
*/
DWORD	CheckDriverSecurityContext(void)
{
	DWORD	dwReturn = RETURN_SUCCESS;
	PDEVICE_OBJECT pRootDevice = NULL;
	PDEVICE_OBJECT pNextDevice = NULL;
	PDRIVER_OBJECT pCurrDriver = NULL;

	DbgPrint("[+] CheckDriverSecurityContext:: Check has begun.. \n");

	// 
	// First, get the current pointer device object 
	//
	pRootDevice = 
		WdfDeviceWdmGetPhysicalDevice(pCurrentApp->wdfCurrentDevice);
	if (pRootDevice == NULL)
	{
		DbgPrint("[x] CheckDriverSecurityContext::Error :  Unable to get current device object ! \n");
		return ERR_PDO_FROM_WDFDEVICE_FAILED;
	}

	pNextDevice = pRootDevice;

	//
	// Walk the keyboard stack to check if something's wrong.. 
	//
	while (pNextDevice != NULL)
	{
		// Get driver from device object 
		pCurrDriver = pRootDevice->DriverObject;

		// Check name of the driver
		pCurrDriver->DriverName.Buffer

		pNextDevice = IoGetLowerDeviceObject(pRootDevice);
		pRootDevice = pNextDevice;
	}

	return dwReturn;
}