//**********************************************************************//
// 					This file is part of GostxBoard						//
//																		//
//  GostxBoard is free software: you can redistribute it and/or modify	//
//  it under the terms of the GNU General Public License as published	// 
//	by the Free Software Foundation, either version 3 of the License, 	//
//	or (at your option) any later version. 								//
//                Such things are STRONGLY recommended.					//
//    																	//
// 	GostxBoard is distributed in the hope that it will be useful,		//
//  but WITHOUT ANY WARRANTY; without even the implied warranty of		//	
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		//
//  GNU General Public License for more details.						//	
//  Do not blame the developper if something goes wrong. You can 		//	
//  either search on the Internet or email the developper to deal 		//
//  with it.															//
//																		//
//  You should have received a copy of the GNU General Public License 	//
//  along with GostxBoard.  If not, see <http://www.gnu.org/licenses/>. //
//																		//
//                                P.A 									//
//                    driver_support@whitekernel.fr 					//
//**********************************************************************//


/**
* \~English
* \file      tools.c
* \author    P.A
* \version   1.0
* \date      May 21, 2015
* \brief	 API tools
*
* \details   Define functions used by the api in order to run smoothly. \n 
*			 Some API specific encryption functions are in this file. 
*
*
* \~French
* \file		tools.c
* \author	P.A
* \version  1.0
* \date		21 Mai 2015
* \brief	Fonctions utilitaires de l'API
*
* \details	Définit des fonctions utilisées par l'API pour fonctionner correctement. 
*			Certaines fonctions cryptographiques spécifiques à l'API sont pérsentes dans ce fichier.
*/
#include "../Commons/crypto.h"
#include "tools.h"
#include "session_control.h"
#include <ctype.h>

#include "../Commons/defines_common.h"


extern PDEVICE_INFO	pdiDevice; // Pointeur globale


/**
*\~English
*\brief		[API] Allocate memory safely 
*\details 	The function tries to safely allocate memory as it is possible in user-mode.\n
*			Memory execution is prohibited and memory is locked to prevent pagination. \n
*			This function is API specific.
*
*\param		DWORD * dwReturn Return value of tthe fonction 
*\param		size_t size Size of memory to allocate
*\param 	BOOL bLock	If TRUE, the memory is locked to prevent pagination. 
*\return	void 
*
*\~French
*\brief		[API] Alloue de la mémoire sécurisée
*\details	La fonction tente d'allouer de la mémoire de manière sécurisée, si tant est qu'une 
*			telle opération soit possible en user-mode.  \n
*			L'exécution mémoire est interdite et la mémoire allouée est vérouillé pour prévenir la 
*			pagination. \n
*			Cette fonction est spécifique à l'API.
*
*\param		DWORD * dwReturn Return value of tthe fonction
*\param		size_t size Size of memory to allocate
*\param 	BOOL bLock	If TRUE, the memory is locked to prevent pagination.
*\return	void
*/
void * AllocateSafeMemory(
	DWORD * dwReturn,
	size_t size,
	BOOL bLock
	)
{
	BOOL	bReturn		= FALSE;
	void	* ptr		= NULL;

	
		//
		//	Allocation réservée et commitée
		// Allocate reserved and commited memory, with 
		// prohibition of execution.
		//
		ptr = VirtualAlloc(NULL, size,
			MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		if (!PTR_VALID(ptr))
		{
			*dwReturn = ERR_ALLOCATION_FAILURE;
			goto end;
		}

		if (bLock)
		{
			//
			// Memory lock to prevent pagination if 
			// the computer fells in sleep mode (which is normaly 
			// not possible during a cipher session )
			//
			bReturn = VirtualLock(ptr, size);
			if (!bReturn)
			{
				*dwReturn = ERR_LOCK_MEMORY_FAILURE;
				goto end;
			}
		}

		*dwReturn = RETURN_SUCCESS;
end:
	return ptr;
}




/**
*\~English
*\brief		[API]Get code Message
*\details 	The function gets the code message related to the specified return code. \n
*			If the code is unknown, "Unknow Message" is returned. \n
*			This function is API specific and is provided by the DLL.
*
*\param		DWORD dwCode Error or return code to get the message to
*\return	char * A static char containing the message associated with the given code
*
*\~French
*\brief		[API] Récupère le message associé à un code de retour 
*\details	La fonction récupère le message associé au code de retour donnée en paramètre. \n 
*			Si le code est inconnu, "Unkown Message" est retourné. \n 
*			\a Note : Tous les messages sont en anglais. \n
*			Cette fonction est spécifique à l'API et est fournis par la DLL.
*
*\param		DWORD dwCode  Code dont le message doit être retourné 
*\return	char * Un char * contenant le message associé au code donné en paramètre.
*/
char * GostxBoardGetCodeMessage(
	DWORD dwCode
	)
{
	DWORD	dwIndex = 0;

	if (CODE_SUCCESS(dwCode))
	{
		while (CodeReturnMessage[dwIndex].Message != 0)
		{
			if (CodeReturnMessage[dwIndex].dwCode == dwCode)
				return CodeReturnMessage[dwIndex].Message;
			dwIndex++;
		}
	}
	else
	{
		dwIndex = 0;
		while (CodeErrorMessage[dwIndex].Message != 0)
		{
			if (CodeErrorMessage[dwIndex].dwCode == dwCode)
				return CodeErrorMessage[dwIndex].Message;
			dwIndex++;
		}
		return CodeErrorMessage[0].Message;
	}
	return CodeErrorMessage[0].Message;
}



/**
*\~English
*\brief		[API] Erase & Free safe allocated memory
*\details 	The function erase and free a safe pool of memory allocated with the function #AllocateSafeMemory. \n
*			It checks if the given parameters are valid. \n 
*			This function is API specific. 
*
*\param		void * ptr A pointer to the memory to free. After called, this paramter is NULL. 
*\param		size_t	size The size of the memory to free 
*\param		BOOL bLock If TRUE the given memory is unlock before beiing freed.
*\return	DWORD Return code defined in defines_common
*
*\~French
*\brief		[API] Ecrase et libère de la mémoire safe
*\details	La fonction écrase et libère de la mémoire allouée avec la fonction #AllocateSafeMemory. \n 
*			Elle vérifie que les paramètres passés en arguments sont valides. \n
*			Cette fonction est spécifique à l'API.
*
*\param		void * ptr Un pointeur vers la mémoire à libérer. Après appel de la fonction, ce paramètre est NULL. 
*\param		size_t	size La taille de la mémoire à libérer
*\param		BOOL bLock Si vrai, la mémoire est devérouillée avant d'être libérée
*\return	DWORD  Code de retour définit dans defines_common
*/
DWORD VirtualSafeFreeMemory(
	  _In_ void	*	ptr,
	  _In_ size_t	size,
	  _In_ BOOL 	bLock
	)
{
	volatile char * cMemBurn	= 0;
	DWORD			dwBurn		= 0;

	//
	// Check if given pointer is valid
	//
	if(!PTR_VALID(ptr))
	{
		return ERR_INVALID_POINTER;
	}

	//
	// Safely erase the data in memory
	//
	do { 
		cMemBurn = (volatile char *)(ptr); 
		dwBurn = (DWORD)size; 
		RtlSecureZeroMemory (ptr, size); 
		while (dwBurn--) *cMemBurn++ = 0; 
	} while (0);

	//
	// If the memory was locked
	//
	if (bLock)
		VirtualUnlock(ptr, size);

	//
	// Free the memory
	//
	VirtualFree(ptr, 0, MEM_RELEASE);
	ptr = NULL;


	return RETURN_SUCCESS;
}



/**
*\~English
*\brief		[API] Test the encryption system
*\details 	The function tests the encryption system by ciphering some known values with 
*			a know key, and check if the return is as expected. \n
*			This function is API specific and is provided by the DLL. Anyway, the function 
*			#GostxBoardInitiateSession calls this function to test the encryption system. So
*			calling both functions will be redundant and time and resources consumming. 
*
*\param		void
*\return	DWORD Return code defined in defines_common
*
*\~French
*\brief		[API] Test le système de chiffrement
*\details	La fonction test le système de chiffrement en chiffrant des valeurs connues avec une clé
*			connue, et vérifiant que les résultats sont bien ceux attendus. \n
*			La fonction est spécifique à l'API et est disponible via la DLL. La fonction #GostxBoardInitiateSession 
*			fait appel à cette fonction pour tester le système de chiffrement. Faire appel à ces deux fonctions est 
*			donc redondant et inutile. 
*
*\param		void
*\return	DWORD  Code de retour définit dans defines_common
*/
DWORD GostxBoardRunSelfEncryptionTest(void)
{
	DWORD	dwReturn		= RETURN_SUCCESS;
	DWORD	dwIndex			= 0;
	USHORT  usCurrentTest	= 0;


		//
		// Run test vector 1
		//
		// Initiate the public key for test 1
		//
		pdiDevice->pkiPublic.cPublicKey = TEST_VECTOR_1;
		//
		// Run test 1. Each scancode of the test vector is ciphered with the key 
		// and must return the precalculated value. 
		//
		for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
		{
			usCurrentTest = TestVector1[dwIndex].usPlain;
			usCurrentTest = CipherKeyTestMode(usCurrentTest);
			if (usCurrentTest != TestVector1[dwIndex].usCipherScan1)
			{
				dwReturn = CRIT_ERR_TEST_VECTORS_FAILURE;
				goto end;
			}
		}
		//
		// Reset encryption system and
		// Run test vector 2
		//
		StreamCipher(0, 0);
		// Initiate the public key for test 2 
		//
		pdiDevice->pkiPublic.cPublicKey = TEST_VECTOR_2;
		//
		// Run test 2. Again, each scancode of the test vector is ciphered with the key 
		// and must return the precalculated value. 
		//
		for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
		{
			usCurrentTest = TestVector1[dwIndex].usPlain;
			usCurrentTest = CipherKeyTestMode(usCurrentTest);
			if (usCurrentTest != TestVector1[dwIndex].usCipherScan2)
			{
				dwReturn = CRIT_ERR_TEST_VECTORS_FAILURE;
				goto end;
			}
		}
		//
		// Reset encryption system 
		//
		StreamCipher(0, 0);

end:
		pdiDevice->dwLastReturnCode = dwReturn;	
	return dwReturn;
}



/**
*\~English
*\brief		[API] Decipher a char
*\details 	The function decipher a scancode with the current state of the encryption system. \n 
*			It checks if the given pointer is valid and  then cheks if it needs to be deciphered. 
*			In that case, it deciphers the char after having mapped it. \n The value is directly 
*			updated by the function.   \n
*			This function is API specific and is provided by the DLL.
*
*\param		PUCHAR	pChar	A pointer to a char to cipher. At the end of the process, this value is the deciphered one.  
*\return	DWORD	Return code defined in defines_common
*
*\~French
*\brief		[API] Déchiffre un caractère			
*\details	La fonction déchiffre un caractère en fonction de l'état actuel du système de chiffrement. \n 
*			Elle vérifie que le pointeur donné est valide et que le char associé doit-être déchiffré. Si c'est le cas, 
*			elle le déchiffre après l'avoir mappé en scancode. \n 
*			La valeur est directement mise à jour par la fonction.
*
*\param		PUCHAR	pChar	Un pointeur vers un char à déchiffrer. A la fin du processus, la variable pointé contient le code déchiffré.
*\return	DWORD	Code de retour définit dans defines_common
*/
DWORD GostxBoardDecipherChar(
	_In_ PUCHAR	 pChar
	)
{
	DWORD		dwReturn		= 0;
	USHORT		usCurrent		= 0;
	USHORT		VirtualKey		= 0;
	UINT		uDebug = 0;
	BOOL		bUpperCase		= FALSE;
	HKL			KeyboardLayout	= { 0 };

	__try {
		//
		// Check if there were no previous error
		//
		if (!CODE_SUCCESS(pdiDevice->dwLastReturnCode))
			__leave;

		//
		// Check that a cipher session is running. 
		// Else, we just end the function and return the given char.
		//
		if (PTR_VALID(pdiDevice))
		{
			if (!pdiDevice->pkiPublic.bCipherStarted)
			{
				// No session detected.
				dwReturn = ERR_NO_ACTIVE_SESSION;
				__leave;
			}
		}
		else
		{
			dwReturn = ERR_SESSION_NOT_INITIALIZED;
			__leave;
		}

		//
		// Check that the pointer is valid
		// Else, we just end the function and return the given char
		//
		if ((!PTR_VALID(pChar)) || (*pChar == 0))
		{
			// Char is not valid
			dwReturn = ERR_INVALID_CHAR;
			__leave;
		}

		// 
		// Map the the char to obtain a scancode
		//
		KeyboardLayout = pdiDevice->KeyboardLayout;
		usCurrent = VkKeyScanExA(LOBYTE(*pChar), KeyboardLayout);
		if (HIBYTE(usCurrent))
		{
			//
			// This is an uppercase, so clean before process so that we can 
			// compute it as a normal char. We will put it as an uppercase 
			// at the end of the process
			//
			usCurrent = usCurrent << 8;
			usCurrent = usCurrent >> 8;
			bUpperCase = TRUE;
		}
		usCurrent = (USHORT)MapVirtualKeyExA((UINT)LOBYTE(usCurrent), MAPVK_VK_TO_VSC, KeyboardLayout);
		if (!usCurrent)
		{
			// 
			// Non-recoverable error. It shouldn't be such results. 
			// We try to close the session
			//
			dwReturn = GostxBoardCloseCipherSession();
			if (!CODE_SUCCESS(dwReturn))
			{
				//
				// Closing seems to be impossible. Connexion bridge is
				// broken. We try to save what can be save and notifie the 
				// user.
				//
				CriticalCleanSession();
				dwReturn = CRIT_ERR_CIPHER_STILL_RUNNING;
				__leave;
			}
			dwReturn = ERR_STRANGE_SCANCODE_DETECTED;
			__leave;
		}
		//
		// We check if we have to decipher the scancode. Its presence is 
		// check in the whitelist defined in defineKbFr.H
		//
		if (isElementPresent(KEYBOARD_MAP_ALPHAN_FR, AN_TABLE_SIZE, usCurrent)
			!= ERR_CODE_NOT_FOUND)
		{
			//
			// This is a known scancode, so we decipher it. The function 
			// will do it according to the state of the encryption system
			//
			usCurrent = DecipherKey(usCurrent);
			pdiDevice->dwNumberOfKeys++;

			//
			// Check that there is no error
			//
			if (usCurrent == ERR_CODE_NOT_FOUND)
			{
				//
				// We got an error. We try to handle it.
				//
				dwReturn = GostxBoardCloseCipherSession();
				if (!CODE_SUCCESS(dwReturn))
				{
					//
					// Closing seems to be impossible. Connexion bridge is
					// broken. We try to save what can be save and notifie the 
					// user.
					//
					CriticalCleanSession();
					dwReturn = CRIT_ERR_CIPHER_STILL_RUNNING;
					__leave;
				}
				__leave;
			}
			//
			// Map the scancode
			//
			usCurrent = (USHORT)MapVirtualKeyExA(usCurrent, MAPVK_VSC_TO_VK, KeyboardLayout);
			VirtualKey = usCurrent;
			usCurrent = MapVirtualKeyExA(usCurrent, MAPVK_VK_TO_CHAR, KeyboardLayout);

			//
			// If it is an uppercase
			//
			if (HIBYTE(VkKeyScanA((char)usCurrent)) != 1 && bUpperCase)
			{
				#ifdef _DEPRECIATED
					if (__isascii(usCurrent))
					{
						usCurrent = toupper(usCurrent);
					}
					else
					{
						usCurrent = VirtualKey;
					}	 
				#endif
				usCurrent = VirtualKey;

			}
			else if (!bUpperCase && HIBYTE(VkKeyScanA((char)usCurrent)) == 1)
			{
				usCurrent = (USHORT)tolower(usCurrent);
				if (__isascii(usCurrent))
					usCurrent = tolower(usCurrent);
			}

			//
			// Update the puchar given in parameter
			//
			*pChar = (UCHAR)usCurrent;
			RtlSecureZeroMemory(&usCurrent, sizeof(UCHAR));

			//
			// We check the number of ciphered scancodes
			//
			if (pdiDevice->dwNumberOfKeys >= MAX_SCANCODES_BEFORE_REGENERATION)
			{
				//
				// Too much scancodes have been ciphered with the same keys
				// so we need to regenerate the session
				//
				dwReturn = RegenerateSession();
				if (!CODE_SUCCESS(dwReturn))
				{
					dwReturn = ERR_UNABLE_TO_REGENERATE_SESSION;
					__leave;
				}
			}
		}
	} 

	__finally {

	   pdiDevice->dwLastReturnCode = dwReturn;
	} 
	return dwReturn;
}




/**
*\~English
*\brief		[API] Return the last returned code
*\details 	The function returns the last returned code. It can be used by an application to know what's going on 
*			inside the API. \n
*			This function is API specific and is provided by the dll.
*
*\param		void
*\return	DWORD	Return code defined in defines_common
*
*\~French
*\brief		[API] Retourne le dernier code de retour
*\details	La fonction retourne le dernier code de retour. Elle peut-être utilisée par une application pour inspecter ce qu'il se 
*			passe au sein de l'API. \n 
*			Cette fonction est spécifique à l'API et est fournis par la DLL. 
*
*\param		void
*\return	DWORD	Code de retour définit dans defines_common
*/
DWORD GostxBoardGetLastReturnCode(void)
{
	return pdiDevice->dwLastReturnCode;
}




/**
*\~English
*\brief		Call the verbose callback
*\details 	The function calls the registered callback in order to notify the client application. \n
*			This function is API specific.
*
*\param		char * Message Message to send to the application
*\param		BYTE bLevel Severity of the message
*\return	void
*
*\~French
*\brief		Appel le callback verbeux (non je n'ai toujours pas trouvé mieux comme nom.. 42 ? )
*\details	La fonction appelle le callback enregistré de manière à notifier l'application client. \n
*
*\param		char * Message Message à envoyer à l'application
*\return	BYTE bLevel	Sévérité du message
*/
void PrintEx(
	_In_ char * Message,
	_In_ BYTE bLevel
	)
{
	//__try {
		if (pdiDevice->VerboseCallback)
		{
			(*pdiDevice->VerboseCallback)(
				Message,
				pdiDevice->dwLastReturnCode,
				bLevel);
		}
	/*}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// Nothing
	} */
	
	return;
}



