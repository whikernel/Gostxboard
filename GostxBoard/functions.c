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

const WCHAR CurrentDriverName[] = L"\\Driver\\GostProtect";
const WCHAR I8042DriverName[] = L"\\Driver\\i8042prt";
const WCHAR ACPIDriverName[] = L"\\Driver\\ACPI";



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
	} 
	else 
	{
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
* \brief    Monitoring de la sécurité
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
	DWORD	dwIndex = 0;
	PDEVICE_OBJECT pRootDevice = NULL;
	PDEVICE_OBJECT pNextDevice = NULL;
	PDRIVER_OBJECT pCurrDriver = NULL;

	DbgPrint("[+] GostxBoard_CheckDriverSecurityContext:: Check has begun.. \n");

	DbgPrint("[+] GostxBoard_CheckDriverSecurityContext:: Analysing stack.. ");
	pRootDevice =
		WdfDeviceWdmGetPhysicalDevice(pCurrentApp->wdfCurrentDevice);
	pRootDevice = IoGetAttachedDeviceReference(pRootDevice);
	pNextDevice = pRootDevice;

	while (pNextDevice != NULL)
	{
		// Get driver from device object 
		pCurrDriver = pRootDevice->DriverObject;
		switch (dwIndex)
		{
		case 0:	  
			if (wcscmp(pCurrDriver->DriverName.Buffer, CurrentDriverName) != 0)
				// Here we are and here we have another name.. Strange things happen here.. Obviously
				dwReturn = ERR_INVALID_DRIVER_NAME;
			break;
		case 1:
			if (wcscmp(pCurrDriver->DriverName.Buffer, I8042DriverName) != 0 )
				// i8042prt should be below us.. And he's not
				dwReturn = ERR_INVALID_I8042_PLACE;
			break;
		case 2: 
			if (wcscmp(pCurrDriver->DriverName.Buffer, ACPIDriverName) != 0)
				// acpi should be below i8042prt.. And he's not
				dwReturn = ERR_INVALD_ACPI_PLACE;
		default: 
			break;

		}
		// == DEBUG == 
		DbgPrint("Driver %d [%ws]  [",  dwIndex++, pCurrDriver->DriverName.Buffer);
		if (!CODE_SUCCESS(dwReturn))
		{
			// One of the watched driver is not at its place
			DbgPrint("Failed]");
			goto end;
		}
		else
		{
			DbgPrint("Ok] - ");
		}
		pNextDevice = IoGetLowerDeviceObject(pRootDevice);
		pRootDevice = pNextDevice;
	}
	

	end:
	DbgPrint(" @ End of analyze\n");
	return dwReturn;
}