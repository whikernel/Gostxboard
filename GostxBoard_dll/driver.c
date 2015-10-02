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
* \file      driver.c
* \author	 Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief	 Some functions used by the API to handle driver's connection
*
*
* \~French
* \file		driver.c
* \author	 Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit quelques fonctions utilisées par l'API pour gérer la connexion avce le driver
*
*/

#include "driver.h"
#include "tools.h"
#include "../Commons/crypto.h"

#include <time.h>
#include <SetupAPI.h>

#include "../Commons/defineKbFr.h"


extern	PDEVICE_INFO	pdiDevice;


/**
*\~English
*\brief		[API] Check the smooth connection with the driver	
*\details 	The function checks the availability and smooth connection with the driver. \n 
*			It especially checks the rights of the current process. \n
*			This function is API specific.
*
*\param		void 
*\return	DWORD  Return code defined in defines_common
*
*\~French
*\brief		[API] Vérifie la bonne connection avec le driver
*\details	La fonction vérifie la présence et la bonne connection avec le driver. \n 
*			Elle vérifie notamment les doroits du processus actuel. \n
*			Cette fonction est spécifique à l'API.
*
*\param		void
*\return	DWORD  Code de retour définit dans defines_common
*/
DWORD CheckDriverConnection(void)
{
	DWORD								dwReturn						= RETURN_SUCCESS,
										dwBytesRead						= 0,
										dwIndex							= 0,
										dwRand							= 0;
	PPUBLIC_TEST_CONNECTION				ptcTestConnection				= NULL,
										ptcTestReturn					= NULL;
	PSP_DEVICE_INTERFACE_DETAIL_DATA	devInterfaceDetailData			= NULL;
	HANDLE								hDevice							= NULL;
	BOOL								bResult							= FALSE;
	unsigned long long					ullApplicationKey				= 0;
	TEST_VECTOR							TempVector[TEST_VECTOR_SIZE]	= { 0 };


	//
	// Get the path to the device interface provided by the driver, if exists
	//
	devInterfaceDetailData = GetDeviceInterfaceDetails();
	if (!devInterfaceDetailData)
	{
		//
		// No interface was found. Rise an error
		//
		dwReturn = ERR_DRIVER_NOT_FOUND;
		goto end;
	}

	//
	// Test driver connexion by opening the interface found
	//
	hDevice = CreateFileA((LPCSTR)devInterfaceDetailData->DevicePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED,
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		//
		// We cannot connect with the driver. This might be a rights issue. 
		// We check that the current thread has enought rights to do so. 
		// Admi rights are required to connect the app to the driver
		//
		if (!IsCurrentUserLocalAdministrator())
		{
			//
			// Not enoughts rights. Rise an error. 
			//
			dwReturn = ERR_INSUFFICIENT_RIGHTS;
			FREE(devInterfaceDetailData, sizeof(PSP_DEVICE_INTERFACE_DETAIL_DATA));
			goto end;
		}
		else
		{
			//
			// This is not a rights issue. 
			// No interface was found. 
			//
			dwReturn = ERR_INTERFACE_NOT_FOUND;
			FREE(devInterfaceDetailData, sizeof(PSP_DEVICE_INTERFACE_DETAIL_DATA));
			goto end;
		}
	}

	//
	// We send an ioctl to check the connection and the health
	// of the driver. 
	// We allocate memory for send and return structure
	//
	ptcTestConnection	= (PPUBLIC_TEST_CONNECTION)AllocateSafeMemory(&dwReturn, sizeof(PUBLIC_TEST_CONNECTION), TRUE);
	if (!CODE_SUCCESS(dwReturn))
	{
		FREE(devInterfaceDetailData, sizeof(PSP_DEVICE_INTERFACE_DETAIL_DATA));
		goto end;
	}
	ptcTestReturn		= (PPUBLIC_TEST_CONNECTION)AllocateSafeMemory(&dwReturn, sizeof(PUBLIC_TEST_CONNECTION), TRUE);
	if (!CODE_SUCCESS(dwReturn))
	{
		FREE(devInterfaceDetailData, sizeof(PSP_DEVICE_INTERFACE_DETAIL_DATA));
		VirtualSafeFreeMemory(ptcTestConnection, sizeof(PUBLIC_TEST_CONNECTION), TRUE);
		goto end;
	}

	//
	// Populate test vector1. Test vector2 is unused
	// Initiate random with current time
	//
	srand((unsigned int)time(NULL));
	StreamCipher(0, 0);

	//
	// Initiate 64bits application key 
	//
	ullApplicationKey = rand();
	ullApplicationKey <<= 16;
	ullApplicationKey |= rand();
	ullApplicationKey <<= 16;
	ullApplicationKey |= rand();
	ullApplicationKey <<= 16;
	ullApplicationKey |= rand();
	ptcTestConnection->TestKey = ullApplicationKey;
	pdiDevice->pkiPublic.cPublicKey = ullApplicationKey;
	for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
	{
		//
		// Generate random scancode from whitelist
		//
		dwRand = rand() % (KEYBOARD_MAP_ALPHAN_FR_SIZE - 1);
		ptcTestConnection->TestVector[dwIndex].usPlain = 
			KEYBOARD_MAP_ALPHAN_FR[dwRand].ScanCode;
		TempVector[dwIndex].usPlain =  KEYBOARD_MAP_ALPHAN_FR[dwRand].ScanCode;;
		//
		// Save ciphered scancodes
		//
		TempVector[dwIndex].usCipherScan1 = 
			CipherKeyTestMode(KEYBOARD_MAP_ALPHAN_FR[dwRand].ScanCode);
	}
	StreamCipher(0, 0);

	bResult = DeviceIoControl(hDevice,
		(ULONG)IOCTL_TEST_CONNECTION,
		ptcTestConnection, sizeof(PUBLIC_TEST_CONNECTION),
		ptcTestReturn, sizeof(PUBLIC_TEST_CONNECTION),
		&dwBytesRead, NULL);
	if (!bResult)
	{
		VirtualSafeFreeMemory(ptcTestConnection, sizeof(PUBLIC_TEST_CONNECTION), TRUE);
		VirtualSafeFreeMemory(ptcTestReturn, sizeof(PUBLIC_TEST_CONNECTION), TRUE);
		CloseHandle(hDevice);
		dwReturn = ERR_CONNECTION_FAILURE;
		goto end;
	}

	//
	// We have return from IOCTL
	//
	dwReturn = ptcTestReturn->dwError;
	if (!CODE_SUCCESS(dwReturn))
		goto free;

	//
	// Checks the returned encrypted vector
	//
	for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
	{
		if (ptcTestReturn->TestVector[dwIndex].usPlain != TempVector[dwIndex].usCipherScan1)
		{
			// Encryption error
			dwReturn = CRIT_ERR_TEST_VECTORS_FAILURE;
			goto free;
		}
	}

	//
	// Saved the handle to later operation, like 
	// starting a cipher session
	//
	pdiDevice->hDevice = hDevice;

	//
	// Saved current keyboard layout in order to map scancodes to chars
	//
	pdiDevice->KeyboardLayout = GetKeyboardLayout(0);

	free:
	//
	// Free memory
	//
	FREE(devInterfaceDetailData, sizeof(PSP_DEVICE_INTERFACE_DETAIL_DATA));
	VirtualSafeFreeMemory(ptcTestConnection, sizeof(PUBLIC_TEST_CONNECTION), TRUE);
	VirtualSafeFreeMemory(ptcTestReturn, sizeof(PUBLIC_TEST_CONNECTION), TRUE);

end:
	pdiDevice->dwLastReturnCode = dwReturn;
	return dwReturn;
}


/**
*\~English
*\brief		[API] Get driver device interface
*\details 	The function retrieves the path to the device interface provided by the driver. \n
*			This function is API specific.
*
*\param		void 
*\return	PSP_DEVICE_INTERFACE_DETAIL_DATA  A pointer to a structure containing informations about the device interface founded. \n 
*			If an error occured the returned pointer is NULL.  
*
*\~French
*\brief		[API] récupère le chemin d'accès l'interface du périphérique du driver
*\details	La fonction récupère le chemin d'accès à l'interface du périphérique fournis par le driver.  \n
*			Cette fonction est spécifique à l'API.
*
*\param		void 
*\return	PSP_DEVICE_INTERFACE_DETAIL_DATA Un pointeur vers une structure contenant les informations liées à l'interface du périphérique trouvé. \n
*			Si une erreur survient, le pointeur retourné est NULL. 
*/
PSP_DEVICE_INTERFACE_DETAIL_DATA  GetDeviceInterfaceDetails(void)
{
	PSP_DEVICE_INTERFACE_DETAIL_DATA    devInterfaceDetailData	= NULL;
	HDEVINFO							hardDeviceInfo			= { 0 };
	SP_DEVICE_INTERFACE_DATA			devInterfaceData		= { 0 };
	DWORD								dwIndex					= 0;
	ULONG								ulLengthRequired		= 0,
										ulLengthPredicted		= 0;
	//
	//
	// Open an handle on the device interface given by the GUID, 
	// in order to retrieve all instance of the interface.
	//
	hardDeviceInfo = SetupDiGetClassDevs(
		(LPGUID)&GUID_DEVINTERFACE_GOSTXBOARD,
		NULL, NULL,
		(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if ( hardDeviceInfo == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	//
	// Initiate size of the interface structure
	devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	//
	// Enumerate all instances
	//
	do
	{
		if (SetupDiEnumDeviceInterfaces(hardDeviceInfo,
			0, (LPGUID)&GUID_DEVINTERFACE_GOSTXBOARD,
			dwIndex,
			&devInterfaceData)) {
			//
			// If the structure is already allocated, free it 
			// in order to reuse it. 
			if (devInterfaceDetailData) {
				free(devInterfaceDetailData);
				devInterfaceDetailData = NULL;
			}

			//
			// nAllocate a structure used to retrieve specific information from the device we 
			// are looking for. This information contains the path to the device interface which 
			// wil allow us to talk with our driver
			//
			if (!SetupDiGetDeviceInterfaceDetail(
				hardDeviceInfo,
				&devInterfaceData,
				NULL, 0,
				&ulLengthRequired,
				NULL))
			{
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
				{
					SetupDiDestroyDeviceInfoList(hardDeviceInfo);
					return NULL;
				}
			}


			//
			// Allocate structure to retrieve inforamtions with 
			// previous retrieved size
			//
			ulLengthPredicted = ulLengthRequired;
			devInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(ulLengthPredicted);
			if (devInterfaceDetailData)
			{
				devInterfaceDetailData->cbSize =
					sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			}
			else
			{
				SetupDiDestroyDeviceInfoList(hardDeviceInfo);
				return NULL;
			}

			// 
			// Get details of our device in previous allocated structure
			//
			if (!SetupDiGetDeviceInterfaceDetail(
				hardDeviceInfo, &devInterfaceData,
				devInterfaceDetailData, ulLengthPredicted,
				&ulLengthRequired, NULL))
			{
				SetupDiDestroyDeviceInfoList(hardDeviceInfo);
				free(devInterfaceDetailData);
				return NULL;
			}
			dwIndex++;
		}
		else if (GetLastError() != ERROR_NO_MORE_ITEMS)
		{
			free(devInterfaceDetailData);
			devInterfaceDetailData = NULL;
			continue;
		}
		else
			break;

	} WHILE(TRUE);

	//
	// Destroy storage structure of hardware informations. No raison 
	// to keeps it. 
	SetupDiDestroyDeviceInfoList(hardDeviceInfo);

	return devInterfaceDetailData;

}


//****
/**
*\~English
*\brief		[API] Checks admin rights
*\details 	The function checks if the current thread has the adminitrators rights. This is necessary for the 
*			connection with the driver. 
*
*\param		void
*\return	BOOL If TRUE, the current thread is running as administrator. 
*
*\~French
*\brief		[API] Vérifie les droits administrateurs.
*\details	La fonction vérifie que le thread actuel s'exécute avec les droits administrateurs, ce qui est nécessaire 
*			pour la connexion avec le driver. 
*
*\param		void
*\return	BOOL Si vrai, le thread actuel s'exécute en administrateur. 
*/
BOOL IsCurrentUserLocalAdministrator(void)
{
	BOOL						fReturn				= FALSE;
	DWORD						dwStatus			= 0,
								dwAccessMask		= 0,
								dwAccessDesired		= 0,
								dwACLSize			= 0,
								dwStructureSize		= sizeof(PRIVILEGE_SET);
	PACL						pACL				= NULL;
	PSID						psidAdmin			= NULL;
	HANDLE						hToken				= NULL;
	HANDLE						hImpersonationToken = NULL;
	PRIVILEGE_SET				ps					;
	GENERIC_MAPPING				GenericMapping		;
	PSECURITY_DESCRIPTOR		psdAdmin			= NULL;
	SID_IDENTIFIER_AUTHORITY	SystemSidAuthority	= SECURITY_NT_AUTHORITY;

	const DWORD ACCESS_READ = 1;
	const DWORD ACCESS_WRITE = 2;

	//
	// Check that current thread is running in the context of a user member of the 
	// local administrators group. The function creates a security descriptor that contains
	// a DACL (Access control list ) containing an ACE ( Access control entry ) which authorize
	// only local adminisrators. AccessCheck will then tells us if the current thread can access this object 
	// or not.
	//
	__try
	{
		//
		// Duplicate token's current thread and impersonnate. The token is no own 
		// by the thread anymore 
		//
		if (!OpenThreadToken(GetCurrentThread(), TOKEN_DUPLICATE | TOKEN_QUERY,
				TRUE, &hToken))
		{
			if (GetLastError() != ERROR_NO_TOKEN)
				__leave;
			if (!OpenProcessToken(GetCurrentProcess(),
				TOKEN_DUPLICATE | TOKEN_QUERY, &hToken))
				__leave;
		}
		if (!DuplicateToken(hToken, SecurityImpersonation,
			&hImpersonationToken))
			__leave;

		//
		// Create a binary representation of the SID representing administrator local group
		//
		if (!AllocateAndInitializeSid(&SystemSidAuthority, 2,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0, &psidAdmin))
			__leave;

		psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (psdAdmin == NULL)
			__leave;
		//
		// Create the DACL containing the ACE et applicate rights
		//
		if (!InitializeSecurityDescriptor(psdAdmin,
			SECURITY_DESCRIPTOR_REVISION))
			__leave;

		dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) +
			GetLengthSid(psidAdmin) - sizeof(DWORD);

		pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
		if (pACL == NULL)
			__leave;

		if (!InitializeAcl(pACL, dwACLSize, ACL_REVISION2))
			__leave;

		dwAccessMask = ACCESS_READ | ACCESS_WRITE;

		if (!AddAccessAllowedAce(pACL, ACL_REVISION2, dwAccessMask,

			psidAdmin))
			__leave;

		if (!SetSecurityDescriptorDacl(psdAdmin, TRUE, pACL, FALSE))
			__leave;

		SetSecurityDescriptorGroup(psdAdmin, psidAdmin, FALSE);
		SetSecurityDescriptorOwner(psdAdmin, psidAdmin, FALSE);

		if (!IsValidSecurityDescriptor(psdAdmin))
			__leave;

		dwAccessDesired = ACCESS_READ;
		GenericMapping.GenericRead = ACCESS_READ;
		GenericMapping.GenericWrite = ACCESS_WRITE;
		GenericMapping.GenericExecute = 0;
		GenericMapping.GenericAll = ACCESS_READ | ACCESS_WRITE;

		//.
		// AccessCheck to check if the local user has the permission or not
		//
		//
		if (!AccessCheck(psdAdmin, hImpersonationToken, dwAccessDesired,
			&GenericMapping, &ps, &dwStructureSize, &dwStatus,
			&fReturn))
		{
			fReturn = FALSE;
			__leave;
		}
	}
	__finally
	{
		//
		// Clean structures 
		//
		if (pACL) 
			LocalFree(pACL);
		if (psdAdmin) 
			LocalFree(psdAdmin);
		if (psidAdmin) 
			FreeSid(psidAdmin);
		if (hImpersonationToken) 
			CloseHandle(hImpersonationToken);
		if (hToken) 
			CloseHandle(hToken);
	}

	return fReturn;
}