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
#include <windowsx.h>

extern PAPP_CREDENTIALS	pCurrentApp;

#define HKEY_LOCAL_MACHINE                  (( HKEY ) (ULONG_PTR)((LONG)0x80000002) )


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
	BOOL	bUpdateNeeded = FALSE;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT pRootDevice = NULL;
	PDEVICE_OBJECT pNextDevice = NULL;
	PDRIVER_OBJECT pCurrDriver = NULL;
	

	DbgPrint("[+] GostxBoard_CheckDriverSecurityContext:: Check has begun.. \n");

	DbgPrint("[+] GostxBoard_CheckDriverSecurityContext:: Analysing stack.. Drivers ");

	//
	// Get the current device_object then,  
	// Get the top of the keyboard stack. Kbdclass isn't in it. 
	// A classic keyboard stack contains in the order : 
	//		- Filter(us) 
	//		- i8042prt (driver port)
	//		- ACPI 
	//
	pRootDevice =
		WdfDeviceWdmGetPhysicalDevice(pCurrentApp->wdfCurrentDevice);
	pRootDevice = IoGetAttachedDeviceReference(pRootDevice);
	pNextDevice = pRootDevice;

	while (pNextDevice != NULL)
	{
		//
		// Get driver from device object 
		//
		pCurrDriver = pRootDevice->DriverObject;
		switch (dwIndex)
		{
		case 0:	  
			if (!RtlEqualUnicodeString(&pCurrDriver->DriverName, &pCurrentApp->DriverName, FALSE))
				// Here we are and here we have another name.. Strange things happened here.. Obviously
				dwReturn = ERR_INVALID_DRIVER_NAME;
			break;
		case 1:
			if (!RtlEqualUnicodeString(&pCurrDriver->DriverName, &pCurrentApp->i8042Name, FALSE))
				// i8042prt should be below us.. And he's not
				dwReturn = ERR_INVALID_I8042_PLACE;
			break;
		case 2: 
			if (!RtlEqualUnicodeString(&pCurrDriver->DriverName, &pCurrentApp->apciName, FALSE))
				// acpi should be below i8042prt.. And he's not
				dwReturn = ERR_INVALD_ACPI_PLACE;
		default: 
			break;

		}
		// == DEBUG == //
		DbgPrint(" %d [",  dwIndex++, pCurrDriver->DriverName);
		if (!CODE_SUCCESS(dwReturn))
		{
			// One of the watched driver is not at its place
			DbgPrint("Failed]");
			goto end_analyze;
		}
		else
		{
			DbgPrint("Ok] - ");
		}
		pNextDevice = IoGetLowerDeviceObject(pRootDevice);
		pRootDevice = pNextDevice;
	}
	

	end_analyze:
	DbgPrint(" @ End of analyze\n");
	DbgPrint("[+] GostxBoard_CheckDriverSecurityContext::Checking registry.\n");
	ntStatus = GostxBoard_CheckRegistry(&bUpdateNeeded);
	if (bUpdateNeeded)
	{
		DbgPrint("Update of registry is needed");
		GostxBoard_UpdateRegistry();
	}

	if (!NT_SUCCESS(ntStatus))
	{
		DbgPrint("[-] GostxBoard_CheckDriverSecurityContext::Unable to get registry key. Code : %#x \n", ntStatus);
	}
		   
 
	return dwReturn;		
}



/**
* \~English
* \brief	Check keyboard filter registry value
* \details  This function is called each time the driver wants to check if the registry value 
*			for the keyboard filters have not changed. This key specifies to the system the level of the keyboard filter 
*			driver.	The protection driver has to be the lowest in the driver stack, and so the first in the key. If it's not
*			hte case, a rewrite is done. 
*
* \param	void
* \return   DWORD	Return code defined in defines_common
*
* \~French
* \brief	Vérification de la valeur de registre des filtres claviers 
* \details  Cette fonction est appelée à chaque fois que le driver veut vérifier que la valeur 
*			de registre des filtres claviers n'a pas changé. Cette clé de registre permet de spécifier au 
*			système l'altitude du filtre clavier. Le driver de protection doit être le plus bas dans la pile 
*			des drivers, et donc le premier dans la clé. Si ce n'est pas le cas, une réécriture est effectuée. 
*
* \param	void
* \return   DWORD	Code de retour définit dans defines_common
*
*/	
#pragma warning(disable: 4702)
NTSTATUS GostxBoard_CheckRegistry(
	BOOL * UpdateNeeded
	)
{
	NTSTATUS							ntStatus		= STATUS_SUCCESS;
	UNICODE_STRING						name			;	
	PKEY_VALUE_PARTIAL_INFORMATION		data			= NULL;
	ULONG								i				= 0;
	
	//
	// Init registry path
	//
	RtlInitUnicodeString(&name, L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e96b-e325-11ce-bfc1-08002be10318}");

	DbgPrint("[+] Gostxboard_ReadRegistery::Reading registry.. \n");

	//
	// Get data from registry value
	//
	ntStatus = GostxBoard_ReadRegistryKey(&name, L"UpperFilters", &data);
	if (NT_SUCCESS(ntStatus))
	{
		//
		// For the length of the key
		//

		if ((data->Type == REG_MULTI_SZ) && (data->DataLength >= 11 * sizeof(wchar_t)))
		{
			//
			// We search the string "GostProtect". It must be in first position
			// 

			for (i = 0; i <= (data->DataLength - 11 * sizeof(wchar_t)); ++i)
			{
				if (memcmp(data->Data + i, L"GostProtect", 11 * sizeof(wchar_t)) == 0)
				{
					DbgPrint(">>>Registry active<<<\n");
					*UpdateNeeded = FALSE;
					ntStatus = STATUS_SUCCESS;
					break;
				}
				else
				{
					DbgPrint("\nRegistry value non found in first position. Update is needed \n");
					*UpdateNeeded = TRUE;
					ntStatus = STATUS_INVALID_DISPOSITION;
					break;
				}
			}
		}
		else
		{
			DbgPrint("\nRegistry value not valid. Update is needed \n");
			*UpdateNeeded = TRUE;
			ntStatus = STATUS_INVALID_DISPOSITION;
		}
#		pragma warning( default : 4702 )
	}

	return ntStatus;
}		


/**
* \~English
* \brief	Check keyboard filter registry value
* \details  This function is called each time the driver wants to check if the registry value
*			for the keyboard filters have not changed. This key specifies to the system the level of the keyboard filter
*			driver.	The protection driver has to be the lowest in the driver stack, and so the first in the key. If it's not
*			hte case, a rewrite is done.
*
* \param	void
* \return   DWORD	Return code defined in defines_common
*
* \~French
* \brief	Vérification de la valeur de registre des filtres claviers
* \details  Cette fonction est appelée à chaque fois que le driver veut vérifier que la valeur
*			de registre des filtres claviers n'a pas changé. Cette clé de registre permet de spécifier au
*			système l'altitude du filtre clavier. Le driver de protection doit être le plus bas dans la pile
*			des drivers, et donc le premier dans la clé. Si ce n'est pas le cas, une réécriture est effectuée.
*
* \param	void
* \return   DWORD	Code de retour définit dans defines_common
*
*/
#pragma warning(disable: 4702)
NTSTATUS GostxBoard_UpdateRegistry(void)
{
	//UNICODE_STRING	name;
	//RtlInitUnicodeString(&name, L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e96b-e325-11ce-bfc1-08002be10318}");
	//GostxBoard_WriteRegistryKey(&name, L"UpperFilters", REG_MULTI_SZ, &data,  )

	DbgPrint("[+] GostxBoard_UpdateRegistry::Updated..\n");
	return STATUS_SUCCESS;
}


/**
* \~English
* \brief	Read a registry key 
* \details  This function is used to read a registry key. 
*
* \param	PUNICODE_STRING keyPath Path to the key 
* \param	wchar_t  * keyValueName Pointer to the name of the value
* \param	PKEY_VALUE_PARTIAL_INFORMATION *keyData Pointer to a data structure which will contain information of the key 
* \return   DWORD	Return code defined in defines_common
*
* \~French
* \brief	Lit une clé de registre
* \details  Cette fonction lit une clé de registre. 
*
* \param	PUNICODE_STRING keyPath Chemin vers la clé
* \param	wchar_t  * keyValueName Pointeur vers le nom de la valeur
* \param	PKEY_VALUE_PARTIAL_INFORMATION *keyData Pointeur vers une structure qui va contenir les informations sur la clé.
* \return   DWORD	Code de retour définit dans defines_common
*
*/
NTSTATUS GostxBoard_ReadRegistryKey(
	_In_ PUNICODE_STRING keyPath, 
	_In_ wchar_t *keyValueName, 
	_In_ PKEY_VALUE_PARTIAL_INFORMATION *keyData
	)
{
	OBJECT_ATTRIBUTES		regObjAttribs		= { 0 };
	HANDLE					regKeyHandle		= NULL;
	NTSTATUS				status				= STATUS_SUCCESS;	
	UNICODE_STRING			valName				;
	ULONG					size				= 0;
	ULONG					resultSize			= 0 ;

	//
	// Init unicode string value
	//
	InitializeObjectAttributes(
		&regObjAttribs,
		keyPath, 
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
		NULL, 
		NULL
		);

	status = ZwOpenKey(
		&regKeyHandle, 
		KEY_READ, 
		&regObjAttribs
		);

	if (!NT_SUCCESS(status))
		return status;

	RtlInitUnicodeString(
		&valName, 
		keyValueName
		);

	//
	// Get the size of the registry key 
	//
	status = ZwQueryValueKey(
		regKeyHandle, 
		&valName, 
		KeyValuePartialInformation, 
		NULL, 
		0, 
		&size
		);

	if (!NT_SUCCESS(status) && status != STATUS_BUFFER_OVERFLOW && status != STATUS_BUFFER_TOO_SMALL)
	{
		ZwClose(regKeyHandle);
		return status;
	}

	if (size == 0)
	{
		ZwClose(regKeyHandle);
		return STATUS_NO_DATA_DETECTED;
	}

	//
	// Allocate a buffer to get the registry value 
	//
	*keyData = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePoolWithTag(PagedPool, size, 0);
	if (!*keyData)
	{
		ZwClose(regKeyHandle);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	//
	// Query the value 
	//
	status = ZwQueryValueKey(
		regKeyHandle, 
		&valName, 
		KeyValuePartialInformation, 
		*keyData, 
		size, 
		&resultSize);

	ZwClose(regKeyHandle);
	return status;
}




/**
* \~English
* \brief	Write a registry key
* \details  This function is used to write a registry key.
*
* \param	PUNICODE_STRING keyPath Path to the key
* \param	wchar_t  * keyValueName Pointer to the name of the value
* \param	ULONG	keyValueType Type of value
* \param	void * ValueData Pointer to the data value
* \return   DWORD	Return code defined in defines_common
*
* \~French
* \brief	Ecrit une clé de registre
* \details  Cette fonction écrit une clé de registre.
*
* \param	PUNICODE_STRING keyPath Chemin vers la clé
* \param	wchar_t  * keyValueName Pointeur vers le nom de la valeur
* \param	ULONG keyValueType Type de valeurs		
* \param	void * ValueData Pointeur vers la la donée à écrire 
* \return   DWORD	Code de retour définit dans defines_common
*
*/
NTSTATUS GostxBoard_WriteRegistryKey(
	_In_ PUNICODE_STRING keyPath, 
	_In_ wchar_t *keyValueName, 
	_In_ ULONG keyValueType, 
	_In_ void *valueData, 
	_In_ ULONG valueSize
	)
{
	OBJECT_ATTRIBUTES		regObjAttribs		= { 0 };
	HANDLE					regKeyHandle		= NULL;
	NTSTATUS				status				= STATUS_SUCCESS;
	UNICODE_STRING			valName;

	//
	// Init registry path 
	//
	InitializeObjectAttributes(&regObjAttribs, keyPath, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	//
	// Open the registry key 
	//
	status = ZwOpenKey(&regKeyHandle, KEY_READ | KEY_WRITE, &regObjAttribs);
	if (!NT_SUCCESS(status))
		return status;

	RtlInitUnicodeString(&valName, keyValueName);

	//
	// Set the value of the key 
	//
	status = ZwSetValueKey(regKeyHandle, &valName, 0, keyValueType, valueData, valueSize);

	ZwClose(regKeyHandle);
	return status;
}




