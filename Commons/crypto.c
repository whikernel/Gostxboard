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
//                              P.A 									//
//                    driver_support@whitekernel.fr 					//
//**********************************************************************//

/**
* \~English
* \file      crypto.c
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Defines common cryptographic functions
*
* \details   Defines the commons crypographic functions used by both 
*			driver and API. Somes functions are only used by one of 
*			them, so there are defines _DRIVER_ and _APP_ defined during
*			generation.
*
* \~French
* \file		crypto.c
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit les fonctions cryptographiques communes
* 
* \details Définit les fonctions cryptographiques communes pour le driver 
*			et l'API. Certaines fonctions sont spécifiques à l'un ou l'autre et 
*			sont donc soumis aux defines _DRIVER_ et _APP_ , définis durant la 
*			compilation.
*/


#include "../Commons/crypto.h"	
#include "../Commons/GostHash.h"
#include "../Commons/defineKbFr.h"
#include "../Commons/defines_common.h"
#include <intrin.h>

#include <ks.h>
#include <time.h>

#if defined(_APP_) || defined(_DLL_)
extern PDEVICE_INFO	pdiDevice;
#endif

extern UCHAR uMemory;


/**
*\~English
*\brief     Cipher flag
*\details   Defines the current state of the cipher - Busy or not
*
*\~French
*\brief		Flag de chiffrement
*\details	Définit l'état actuellement du chiffrement - Occupé ou non
*/
static		DWORD	CipherBusy = 0;

#ifdef _DRIVER_
PAPP_CREDENTIALS	pCurrentApp = NULL;



/**
* \~English
* \brief    Cipher a scancode
* \details  Cipher a scancode with the app's credentials
*			which is connected with the driver
* \param    PUCHAR	MakeCode	Pointer to makecode
* \param    PVOID	Unused		Unused
* \return   NTSTATUS		Status of operation
*
* \~French
* \brief    Chiffre un scan-code
* \details  Chiffre un scan-code grâce aux identifiants 
*			de l'application connectée au driver
* \param    PUCHAR	MakeCode	Pointeur vers le scancode
* \param    PVOID	Unused		Inutilisé
* \return   NTSTATUS			Statut de l'opération
*
*/
NTSTATUS	CipherKey	(
	_In_ PUCHAR					MakeCode,
	_In_ PVOID					Unused
	)
{
	NTSTATUS		ntStatus		= STATUS_SUCCESS;
	USHORT			usTempCipher	= 0;
	USHORT			usChar			= 0;


	#ifdef _DEBUG			    
		DbgPrint("[+] GostxBoard_CipherKey::Entry\n");
	#endif
	UNREFERENCED_PARAMETER( Unused );

	RtlCopyMemory(&usChar, MakeCode, sizeof(UCHAR));

	//
	// Generate the permutation table associated and get the output key
	//
	usTempCipher = ShuffleTab(
		usChar );

	if( usTempCipher == (USHORT)(-1) )
	{
		//
		// Unknown error
		//
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_CipherKey::Error Suffling\n");
			DbgPrint("[+] GostxBoard_CipherKey::Quite Cipher\n");
		#endif
		ntStatus = STATUS_ABANDONED;
		//
		// Reset variable
		//
		RtlSecureZeroMemory(&usTempCipher, sizeof(USHORT) );
		//
		// Return status without key
		//
		return ntStatus;
	}
	
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_CipherKey::Couple ( 0x%x:0x%x )\n", usChar ,
		usTempCipher );
	#endif	

	//
	// Copy the generated key to the i8042prt 
	// variable pointed by MakeCode
	//
	RtlCopyMemory( MakeCode, 
		&(usTempCipher) , sizeof( UCHAR ) );

	//
	// Clean things
	//
	RtlSecureZeroMemory(&usTempCipher, sizeof(USHORT) );
	RtlSecureZeroMemory(&usChar, sizeof(USHORT));

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_CipherKey::Exit\n");
	#endif
	return ntStatus;
}
#endif // driver


/**
* \~English
* \brief    Generates a pseudo-random number
* \details  Generates a pseudo-random number from a seed. \\
*			A rotor of polynom 	1 + x**63 + x**67  is incremented
*			by 64 steps at every call. Register consists of 2 words of 32b.	 \\
*			The return value is the image through a bijection of rotor0.
*					----Credit : E.Filiol - 2002/04/05-----
* \param    unsigned long long	iKey	64bits cipher key
* \param    int			Flags	Flag. 1 means normal cipher, 0 means reset the cipher
* \return   The 64bits number from the stream generated from iKey.
*
* \~French
* \brief    Génère une nombre pseudo-aléatoire
* \details   Permet de générer un pseudo-aléa en fonction
*			d'une graine.
*			Un rotor de polynome 1 + x**63 + x**67 avance
*			de 64 coups à chaque appel. Le registre est 
*			sous la forme de 2 mots de 32b. La valeur 
*			retournée est l'image par une bijection de 
*			rotor0. 
*				----Crédit : E.Filiol - 2002/04/05-----  
* \param    long long	iKey	Clé de chiffrement de 64bits
* \param    int			Flags	Flag. 1 pour le chiffrement, 0 pour la remise à zéro du Stream
* \return   Le nombre de 64bits issue de la suite alatoire, généré à partir de iKey.
*
*/
unsigned long long StreamCipher( 
	_In_ unsigned  long long	iKey, 
	_In_ int					Flags )
{
	static unsigned int					oneway; 
	static unsigned long long			rotor0; 
	static unsigned long long			rotor1;
	static unsigned	long long			feedback;
	unsigned long long					result;

	switch (Flags)
	{
	case 1:
		//
		// Generation of stream
		//
		//
		// STEP0: Initialization during first call
		//
		if(!oneway)
		{
			rotor0 = iKey;
#pragma warning( disable : 4242 4244 )
			rotor1 = iKey * 0xF108E4CD87654321LL;
			oneway = 1;
		}

		//
		// STEP1: 64 incrementation   
		//
#pragma warning( disable : 4293 )
		feedback = (rotor1 << 1) ^ (rotor0 >> 63) ^ (rotor0);
		rotor1 = rotor0;
		rotor0 = feedback;

		//
		// STEP2: Return from image of rotor0 through a bijection
		//
		result = ((rotor0 * 0x1525374657E5F50DLL) + 0x8B459B95879A07F3LL);
#pragma warning( default : 4242 4244 )
#pragma warning( default : 4293 )
		break;

	case 0:
		//
		// Reset cipher for key switching
		//
		oneway		= 0;
		rotor0		= 0;
		rotor1		= 0;
		feedback	= 0;
		result = 0;
	default: 
		result = 0;
		break;
	}

	return result;
}


/**
* \~English
* \brief    Permutate the whitelist table and return the input ciphered scancode 
* \details  Permutate the whitelist table according to the output of the 
*			current state of the streamcipher. After the permutation, the 
*			input parameter ScanCode is ciphered and returned. 
* \param    USHORT		ScanCode	ScanCode to cipher
* \return   The ScanCode ciphered.
*
* \~French
* \brief    Permute la table des scancode et retourne le scan-code d'entrée	chiffré
* \details  Permet d'effectuer une permutation gérée par
*			l'aléa sur l'ensemble E des codes valides. Après la permutation, la valeur du 
*			paramètre d'entré ScanCode est chiffré et retourné.
*
* \param    USHORT		ScanCode	Scancode à chiffrer
* \return   Scancode chiffré.
*
*/
USHORT ShuffleTab(
	_In_	USHORT	ScanCode)
{
	DWORD		iterations = 0;
	DWORD		result = 0;
	KEY_MAP		kmTemp = { 0 };
	USHORT		usTemp = ERR_CODE_NOT_FOUND;


	//
	// Initialize a temporay table from the specified 
	// whitelist table 
	//
	ResetShuffleTab();

#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_SuffleTab::Starting Shuffling \n");
#endif

	for (iterations = 0; iterations < KEYBOARD_MAP_ALPHAN_FR_SIZE; iterations++)
	{

		//
		// Generate a random number modulated at the size of the table.	The entered key of 
		// the streamcipher depends of the context. For the driver, the key is stored in 
		// pCurrentApp and for the API or an application, it is stored in pdiDevice
		//
#ifdef _DRIVER_
		result = (DWORD)(StreamCipher(pCurrentApp->cPublicKey, 1) % (KEYBOARD_MAP_ALPHAN_FR_SIZE - 1));
#elif  defined(_APP_) || defined(_DLL_)
		result = (DWORD)(StreamCipher(pdiDevice->pkiPublic.cPublicKey, 1) % (KEYBOARD_MAP_ALPHAN_FR_SIZE - 1));
#endif
		//
		// Permutation of the elements
		//
		RtlCopyMemory(&kmTemp,
			&KEYBOARD_SHUFFLE_ALPHAN_FR[iterations],
			sizeof(KEY_MAP));
		RtlCopyMemory(&KEYBOARD_SHUFFLE_ALPHAN_FR[iterations],
			&KEYBOARD_SHUFFLE_ALPHAN_FR[result],
			sizeof(KEY_MAP));
		RtlCopyMemory(&KEYBOARD_SHUFFLE_ALPHAN_FR[result],
			&kmTemp,
			sizeof(KEY_MAP));

#if defined(_DRIVER_) 
		//
		// Driver part - The permutation to watch for is from the 
		// original table. 
		//
		if (KEYBOARD_MAP_ALPHAN_FR[iterations].ScanCode == ScanCode)
		{
			//
			// Save the corresponding permuation in order to 
			// find the corresponding scan-code at the end
			//
			usTemp = (USHORT)iterations;
		}

	}
	//
	// Find the corresponding scan code to the index of the permutation saved, 
	// in the suffled table in order to cipher the scan-code
	//
	if (usTemp != ERR_CODE_NOT_FOUND)
		usTemp = KEYBOARD_SHUFFLE_ALPHAN_FR[usTemp].ScanCode;

#elif ( defined(_APP_) || defined(_DLL_) ) 
		//
		// App part -  The permutation to watch for is from the 
		// shuffled table. 
		//
		}
		for( iterations = 0 ; iterations  < KEYBOARD_MAP_ALPHAN_FR_SIZE ; iterations++)
		{ 
			if (KEYBOARD_SHUFFLE_ALPHAN_FR[iterations].ScanCode == ScanCode)
			{
				//
				// Save the corresponding permuation in order to 
				// find the corresponding scan-code at the end
				//
				usTemp = (USHORT)iterations;
				break;
			}
		}
		//
		// Find the corresponding scan code to the index of the permutation saved,
		// in the original table in order to recover the original scan-code
		//
		if (usTemp != ERR_CODE_NOT_FOUND)
			usTemp = KEYBOARD_MAP_ALPHAN_FR[usTemp].ScanCode;

#endif

	//
	// Clean the shuffle table
	//
	RtlSecureZeroMemory( KEYBOARD_SHUFFLE_ALPHAN_FR,
		sizeof( KEYBOARD_SHUFFLE_ALPHAN_FR ) );

	return usTemp;
}



/**
* \~English
* \brief    Reset the shuffle table
* \details  Reset the shuffle table and creata a new one from 
*			original table.
* \param	void
* \return 	void

* \~French
* \brief    Réinitialise la table de permutation 
* \details  Réinitialise la table de permuation à partir de la 
*			table originale
* \param	void 
* \return 	void
*/
VOID ResetShuffleTab()
{
	//
	// Clear the table
	//
	RtlSecureZeroMemory( &KEYBOARD_SHUFFLE_ALPHAN_FR, 
		sizeof(KEYBOARD_SHUFFLE_ALPHAN_FR));
	//
	// Copy the new one, from original table
	//
	RtlCopyMemory( &KEYBOARD_SHUFFLE_ALPHAN_FR, 
		&KEYBOARD_MAP_ALPHAN_FR,
		sizeof(KEYBOARD_MAP_ALPHAN_FR));
}


/**
* \~English
* \brief    Determine if an element is in the whitelist table
* \details  Determine if an element is in the whiteliste table,
*			and if so, return its index in the table.
*			
* \param    KEY_MAP		cIdTab[]	Table in which the function needs to look for
* \param	DWORD		dwSizeOfTab Size of the given table
* \param	USHORT		usElement	Element to look for 
* \return	Index of the element if it is in the whitelist, otherwise ERR_CODE_NOT_FOUND
*
* \~French
* \brief    Détermine si un élement est présent dans la liste blanche
* \details  Détermine si un élement est présent dans la liste blanche, et 
*			dans ce cas retourne son index dans la table.
*
* \param    KEY_MAP		cIdTab[]	Table dans laquelle chercher le scancode
* \param	DWORD		dwSizeOfTab Taille de la table cible 
* \param	USHORT		usElement	Element à chercher
* \return	Index de l'élément dans la table, s'il existe. Sinon le code ERR_CODE_NOT_FOUND
*/
DWORD	isElementPresent ( 
	_In_	KEY_MAP		cIdTab[] ,
	_In_	DWORD		dwSizeOfTab,
	_In_	USHORT		usElement  )
{
	DWORD		dwIndex			= 0;

	//
	// Check if the table exists
	// 
	if( cIdTab == NULL )
	{
		return ERR_NO_SCANCODE_TABLE_FOUND;
	}

	for (dwIndex = 0; dwIndex < dwSizeOfTab; dwIndex++)
	{
		if (cIdTab[dwIndex].ScanCode == usElement)
		{
			//
			// The element is find, return it
			//
			return dwIndex;
		}
	}

	//
	// The element wasn't found in the table, so 
	// we transmit the error 
	//
	return (DWORD)ERR_CODE_NOT_FOUND;
}


#ifdef _DRIVER_
/**
* \~English
* \brief    Initialize the 64bits application's key
* \details  Initialize the application key from a seed and the masterkey. This key 
*			is unique for the application and must often change to ensure maximum
*			security. This is up to the client app.
*
* \param	void	
* \return   The 64bits key for the application
*
* \~French
* \brief    Initialise la clé de 64bits de l'application cliente
* \details  Initialise la clé de 64bits de l'application cliente à partir d'une graine 
*			et de la masterkey. Cette clé est unique à l'application et doit changer 
*			suffisemment souvent pour assurer une sécuritée maximale. 
*
* \param	void
* \return   La clé de 64bits pour l'application.
*/
unsigned long long GenerateKey()
{
	unsigned long long	KeyOut		= 0; 
	unsigned long int	init		= 0;
	unsigned int		random		= 0;
	unsigned int		index		= 0;
	unsigned long long	seed		= 0;
	gost_hash_ctx		ctx			= { 0 };

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_GenerateKey::Entry\n");
		DbgPrint("[+] GostxBoard_GenerateKey::Generating Seed\n");
	#endif

	//
	// Initialization of the random, through the master key and the intrinsic 
	// asm function rdtsc (ReadTimeStampCounter)
	//
	seed = (unsigned long long) __rdtsc();
	seed ^= pCurrentApp->MasterKey ;
	srand((unsigned int)seed);
	init = rand();
	init <<= 16;
	init |= rand();
	init <<= 16;
	init |= rand();
	init <<= 16;
	init |= rand();

	random = (unsigned int)rand() % 16 ;

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_GenerateKey::Generating Key\n");
	#endif
 
	//
	// Generate the stream cipher by calling "random" times
	// the streamcipher function, ensuring a strong key
	//
	for( index = 0; index < random;  index++)
	{
		//
		// Generate the stream
		//
		seed = StreamCipher( init, 1);
	}
	//
	// Reset the stream for later call
	//
	StreamCipher( 0, 0 );

	DbgPrint("[+] GostxBoard_GenerateKey::Hashing generated key..\n");

	//
	//	Hash the key to avoid biais 
	//
	GOSTHASH_init(&ctx);
	GOSTHASH_add((byte *)&seed, sizeof(seed), &ctx);
	GOSTHASH_finalize(&ctx, (byte *)&KeyOut);


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_GenerateKey::Done\n");
		DbgPrint("[+] GostxBoard_GenerateKey::Exit\n");
	#endif

	return KeyOut;
}



/**
* \~English
* \brief    Initialize the 64bits strong MasterKey
* \details  Initialize the 64bits strong MasterKey. This function is called 
*			when the driver is started, at boot time. The MasterKey generated
*			by this function is stored in the APP_CREDENTIAL structure, and is
*			used to generate the client applications key. The generation of this 
*			key is the results of the computation of multiple randoms parameters.
*
* \param	void	
* \return   The 64bits key MasterKey
*
* \~French
* \brief    Initialise la clé forte MasterKey de 64bits
* \details  Initialise la clé forte MasterKey de 64bits. Cette fonction est 
*			appelée à chaque démarrage du driver au boot. La MasterKey générée
*			est enregistrée dans la structure APP_CREDENTIAL, et est utilisée 
*			pour générer les clés des applications clientes. La génération de 
*			cette clé est le résultat de l'agréga de multiples paramètres aléatoires.
*
* \param	void
* \return   La MasterKey de 64bits
*/
unsigned long long InitMasterKey()
{
	unsigned long long		MasterKey = 0;
	unsigned long long		cr = 0;
	LARGE_INTEGER			plInt;

	//
	// Get cpu registers values
	//
	cr = __readcr0() ^ __readcr2() ^ __readcr4();
	cr = cr ^ KeGetCurrentProcessorNumber();
	cr = cr ^ __readeflags();
	cr = cr ^ KeQueryUnbiasedInterruptTime();
	KeQueryPerformanceCounter(&plInt);
	cr = cr ^ plInt.QuadPart;

	//
	// Initialization of the random, with previous seed generated
	//
	srand((unsigned int)cr);
	MasterKey = rand();
	MasterKey <<= 16;
	MasterKey = rand();
	MasterKey <<= 16;
	MasterKey = rand();
	MasterKey <<= 16;
	MasterKey = rand();

	return MasterKey;
}



/**
* \~English
* \brief    Cipher a scancode
* \details  Cipher a scancode received from i8042prt port driver.\n
*			The function checks if the scancode needs to be ciphered and 
*			then calls CipherKey to cipher it.	
*
* \param	PKEYBOARD_INPUT_DATA	pkInKeyData  A pointer to an allocated structure provided by i8042prt
* \param	PUCHAR					DataByte	 A pointer to the scancode to cipher. It resides in i8042prt
* \return   void
*
* \~French
* \brief    Chiffre un scancode
* \details  Chiffre un scancode reçu du driver de port i8042prt. \n
*			La fonction vérifie que le scancode doit-être chiffré, puis 
*			appel la fonction CipherKey pour le faire.

* \param	PKEYBOARD_INPUT_DATA	pkInKeyData  Un pointeur vers une structure allouée et fournis par i8042prt
* \param	PUCHAR					DataByte	 Un pointeur vers le scancode à chiffrer. Il est fournis par le driver i8042prt
* \return   void
*/
void CipherScanCode(
	_In_	PKEYBOARD_INPUT_DATA	pkInKeyData,
	_In_	PUCHAR					DataByte
	)
{
	USHORT		usCurrent	= 0;
	NTSTATUS	ntStatus	= 0;

	UNREFERENCED_PARAMETER(pkInKeyData);

	//
	// Indicate that the cipher is running
	//
	CipherBusy = 1;

	usCurrent = *DataByte;

	//
	// Check if the key needs to be ciphered i.e the client app 
	// is still alive and connected and the scancode is in the whitelist
	//
	if (pCurrentApp->pEprocess != NULL &&
		(isElementPresent(KEYBOARD_MAP_ALPHAN_FR, AN_TABLE_SIZE,
		usCurrent) != ERR_CODE_NOT_FOUND)
		)
	{
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_CipherScanCode::Key need to be ciphered \n");
		#endif
		//
		// Call cipherkey to cipher the scancode according to the state of the 
		// stream cipher
		//
		ntStatus = CipherKey(DataByte, NULL);
		DbgPrint("[+] GostxBoard_CipherScanCode::Return Key : %#x\n", *DataByte);

		if (!NT_SUCCESS(ntStatus))
		{
			#ifdef _DEBUG
				DbgPrint("[+] GostxBoard_CipherScanCode::Key non Ciphered - Error with code 0x%x\n", ntStatus);
			#endif
			return;
		}

	}
	else if (isElementPresent(KEYBOARD_MAP_NUMPAD, NUMPAD_TABLE_SIZE ,usCurrent) 
		!= ERR_CODE_NOT_FOUND)
	{
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_CipherScanCode::NumPad key detected. Correcting to real number. \n", ntStatus);
		#endif
		//
		// Check if given scancode is from numpad. If so, we have to decrement 
		//
		// Get the number associated with the scan code 
		if (usCurrent >= 0x4f)
			usCurrent -= 0x4D;			// 1 - 2 - 3
		else if (usCurrent >= 0x4b)
			usCurrent -= 0x46;			// 3 - 4 - 5 
		else if (usCurrent >= 0x47)
			usCurrent -= 0x3F;			// 5 - 6 - 7
		else
			usCurrent = 0x0;			// Unknown 

		// Save new value
		RtlCopyMemory(DataByte, &usCurrent, sizeof(UCHAR));
		// Call ourselves to recipher the extracted scancode
		CipherScanCode(pkInKeyData, DataByte);
		return;
	}
	else {
		//
		// Check if it is a redundancy ( release key state ). This request is used by 
		// SetWindowsHook to retrieve keystrokes. So we have to cipher it too, in order to ensure 
		// maximal efficency.
		//
		usCurrent &= ~0x80;
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_CipherScanCode::Redundancy check :: 0x%x\n", usCurrent);
		#endif
		if ((isElementPresent(KEYBOARD_MAP_ALPHAN_FR, AN_TABLE_SIZE,usCurrent) !=
			ERR_CODE_NOT_FOUND) && (pCurrentApp->pEprocess != NULL))
		{
			DWORD	dwRand = 0;
			srand((unsigned int)KeQueryUnbiasedInterruptTime());
			// It is a redundancy, we generate noise
			dwRand = rand() % (KEYBOARD_MAP_ALPHAN_FR_SIZE - 1);
			usCurrent = KEYBOARD_MAP_ALPHAN_FR[dwRand].ScanCode;
			usCurrent |= 0x80;
			RtlCopyMemory(DataByte,
				&usCurrent, sizeof(UCHAR));
			#ifdef _DEBUG
				DbgPrint("[+] GostxBoard_CipherScanCode::Redundancy hooked -> 0x%x \n", *DataByte);
			#endif
		}

	}
	uMemory = *DataByte;
	//
	// Free the system encryption
	//
	CipherBusy = 0;
	return;
}


/**
* \~English
* \brief    Test the encryption system
* \details  Test the encryption system. This function test all cryptographics 
*			function of the driver in order to test its integrity. 
*
* \param	void
* \return   DWORD	Status of the operation
*
* \~French
* \brief    Test le chiffrement
* \details  Test le chiffrement du driver. Cette fonction teste toutes les 
*			fonctions cryptographiques du driver pour détecter une erreur 
*			quelconque.
*
* \param	void
* \return   DWORD Status de l'opération
*/
DWORD RunSelfTest(void)
{
	DWORD dwError						= RETURN_SUCCESS;
	DWORD dwIndex						= 0;
	unsigned long long KeyAppOne		= 0;
	unsigned long long KeyAppTwo		= 0;
	unsigned long long MasterKeyTemp	= 0;
	UCHAR usCurrentTest					= 0;


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_SelfTest::Entry\n");
		DbgPrint("[+] GostxBoard_SelfTest::Running MasterKey test... ");
	#endif

	//
	// Check if an application is already using the cipher
	//
	if (pCurrentApp->pEprocess != NULL)
	{
			// An app is already using the cipher
			dwError = CIPHER_BUSY;
#ifdef _DEBUG
			DbgPrint("Error. Cannot run a self test while an application is using the encryption system. \n");
#endif
			goto end;
	}

	//
	// Test MasterKey. The value must not be 0 because 
	// this key is generated at boot time. If it is, we try 
	// to correct it. 
	//
	if (pCurrentApp->MasterKey == 0)
	{
		// Try to correct 
		#ifdef _DEBUG
			DbgPrint("Error\n");
			DbgPrint("[-] GostxBoard_SelfTest::Correcting MasterKey... \n");
		#endif
		bSilent = TRUE;
		pCurrentApp->MasterKey = InitMasterKey();
		bSilent = FALSE;
		if (pCurrentApp->MasterKey == 0)
		{
			// Ciphering is compromised
			dwError = CRIT_ERR_MASTER_KEY_COMPROMISED;
			#ifdef _DEBUG
				DbgPrint("FATAL\n");
				DbgPrint("\n[x] GostxBoard :: Critical Error detected. Cipher is compromised \n\n");
			#endif
			goto end;
		}
	}
	//
	// Check masterkey redundancy
	//
	MasterKeyTemp = InitMasterKey();
	if (MasterKeyTemp == pCurrentApp->MasterKey)
	{
		 // Redundancy detected
		#ifdef _DEBUG
			DbgPrint("Error\n");
			DbgPrint("[-] GostxBoard_SelfTest::Critical Error detected. Cipher is compromised  \n");
		#endif
		dwError = CRIT_ERR_MASTER_KEY_COMPROMISED;
		goto end;
	}
#ifdef _DEBUG
	DbgPrint("0k\n");
#endif

	//
	// Init two applications keys in order to 
	// detect redundancy
	//
#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_SelfTest::Running application keys test... ");
#endif
	bSilent = TRUE;
	KeyAppOne = GenerateKey();
	KeyAppTwo = GenerateKey();
	bSilent = FALSE;
	if (KeyAppOne == KeyAppTwo)
	{
		// Redundancy error
		#ifdef _DEBUG
			DbgPrint("Error.");
			DbgPrint("\n[x] GostxBoard :: Critical Error detected. Cipher is compromised \n\n");
		#endif
		dwError = CRIT_ERR_APP_KEYS_COMPROMISED;
		goto end;
	}
	#ifdef _DEBUG
		DbgPrint("Ok\n");
		DbgPrint("[+] GostxBoard_SelfTest::Running static vectors test 1... ");
	#endif

	//
	// Each key compute MUST corresponds to the 
	// predefined computed key for the cipher key TEST_VECTOR_X
	//
	// Run test vectors 1
	//
	//
	StreamCipher(0,0);
	pCurrentApp->cPublicKey = TEST_VECTOR_1;
	for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
	{
		usCurrentTest = (UCHAR)TestVector1[dwIndex].usPlain;
		bSilent = TRUE;
		CipherKey(&usCurrentTest, NULL);
		bSilent = FALSE;
		if (usCurrentTest != TestVector1[dwIndex].usCipherScan1)
		{
			// Test failed - Cipher is compromised
			#ifdef _DEBUG
				DbgPrint("Error.\n");
				DbgPrint("\n[x] GostxBoard_SelfTest:: Critical error detected. Cipher is compromised \n");
			#endif
			dwError = CRIT_ERR_TEST_VECTORS_FAILURE;
			goto end;
		}
	}
	ResetCipher();
#ifdef _DEBUG
	DbgPrint("Ok\n");
	DbgPrint("[+] GostxBoard_SelfTest::Running static vectors test 2... ");
#endif
	//
	// Run test vectors 2
	//
	pCurrentApp->cPublicKey = TEST_VECTOR_2;
	for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
	{
		usCurrentTest = (UCHAR)TestVector1[dwIndex].usPlain;
		bSilent = TRUE;
		CipherKey(&usCurrentTest, NULL);
		bSilent = FALSE;
		if (usCurrentTest != TestVector1[dwIndex].usCipherScan2)
		{
			// Test failed - Cipher is compromised
			#ifdef _DEBUG
				DbgPrint("Error.\n");
				DbgPrint("\n[x] GostxBoard_SelfTest:: Critical error detected. Cipher is compromised \n");
			#endif
			dwError = CRIT_ERR_TEST_VECTORS_FAILURE;
			goto end;
		}
	}
	ResetCipher();

#ifdef _DEBUG
	DbgPrint("Ok\n");
	DbgPrint("[+] GostxBoard_SelfTest::Encryption system is healthy \n");
#endif

end:
	// Clean things
	RtlSecureZeroMemory(&(pCurrentApp->cPublicKey),
		sizeof(pCurrentApp->cPublicKey));

#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_SelfTest::Exit\n");
#endif
	return dwError;
}



#elif defined(_APP_) || defined(_DLL_) 
/**
* \~English
* \brief    Decipher a scancode
* \details  Decipher a scancode, by calling ShuffleTab function. It does nothing else.
*
* \param	USHORT usInKey ScanCode to decipher
* \return   The clear scancode
*
* \~French
* \brief    Déchiffre un scancode
* \details  Déchiffre un scancode donnée en paramètre. La fonction appelle 
*			ShuffleTab et ne fait rien d'autre. 
*
* \param	USHORT usInKey ScanCode à déchiffrer
* \return   Le scancode déchiffré
*/
USHORT DecipherKey(
	_In_	USHORT				usInKey 
	)
{
	USHORT		usTempCipher = 0;

	//
	// Call ShuffleTab function
	//		
	usTempCipher = ShuffleTab(usInKey);


	return usTempCipher;
}


/**
* \~English
* \brief    Cipher a scancode
* \details  Cipher a scancode. Used by an application for selftest
*
* \param	USHORT usInKey ScanCode to cipher
* \return   The ciphered scan-code
*
* \~French
* \brief    Chiffre un scancode
* \details  Chiffre un scan code. La fonction est utilisée par une application 
*			pour des auto-tests.
*
* \param	USHORT usInKey ScanCode à chiffrer
* \return   Le scancode chiffré
*/
USHORT CipherKeyTestMode(
	_In_	USHORT				usInKey
	)
{
	DWORD		iterations = 0;
	DWORD		result = 0;
	KEY_MAP		kmTemp = { 0 };
	USHORT		usTemp = ERR_CODE_NOT_FOUND;


	//
	// Initialize a temporay table from the specified 
	// whitelist table 
	//
	ResetShuffleTab();

	for (iterations = 0; iterations < KEYBOARD_MAP_ALPHAN_FR_SIZE; iterations++)
	{

		//
		// Generate a random number modulated at the size of the table.	The entered key of 
		// the streamcipher depends of the context. 
		//
		result = (DWORD)(StreamCipher(pdiDevice->pkiPublic.cPublicKey, 1) % (KEYBOARD_MAP_ALPHAN_FR_SIZE - 1));
		//
		// Permutation of the elements
		//
		RtlCopyMemory(&kmTemp,
			&KEYBOARD_SHUFFLE_ALPHAN_FR[iterations],
			sizeof(KEY_MAP));
		RtlCopyMemory(&KEYBOARD_SHUFFLE_ALPHAN_FR[iterations],
			&KEYBOARD_SHUFFLE_ALPHAN_FR[result],
			sizeof(KEY_MAP));
		RtlCopyMemory(&KEYBOARD_SHUFFLE_ALPHAN_FR[result],
			&kmTemp,
			sizeof(KEY_MAP));

		// The permutation to watch for is from the 
		// original table. 
		//
		if (KEYBOARD_MAP_ALPHAN_FR[iterations].ScanCode == usInKey)
		{
			//
			// Save the corresponding permuation in order to 
			// find the corresponding scan-code at the end
			//
			usTemp = (USHORT)iterations;
		}

	}
	//
	// Find the corresponding scan code to the index of the permutation saved, 
	// in the suffled table in order to cipher the scan-code
	//
	if (usTemp != ERR_CODE_NOT_FOUND)
		usTemp = KEYBOARD_SHUFFLE_ALPHAN_FR[usTemp].ScanCode;


#ifdef _DEBUG
	DbgPrint("Couple (%#x :%#x)\n", usInKey, usTemp);
#endif
	//
	// Clean the shuffle table
	//
	RtlSecureZeroMemory(KEYBOARD_SHUFFLE_ALPHAN_FR,
		sizeof(KEYBOARD_SHUFFLE_ALPHAN_FR));

	return usTemp;
}




#ifdef _DEV_MODE
/**
* \~English
* \brief    Regenerate statics test vectors 
* \details  Regenerate test vectors when the whitelist has been changed. This function must only be called 
*			call in development mode. \n 
*			The function print the new table via printf. Displayed values must be replaced in the file 
*			defines_common.h and an update \n
*			The function is API specific and is provided by the DLL if compiled with _DEV_MODE flag on.
*
* \param	void
* \return   void
*
* \~French
* \brief    Regénère les vecteurs de tests statiques. 
* \details  Regénère les vecteurs de tests statiques après une mise à jour de la liste blanche. Cette function doit 
*			seulement être appelée en mode developpement. \n 
*			La fonction imprime via printf les nouveau vecteurs. Les valeurs doivent être reportés dans le 
*			fichier defines_common.h.  \n 
*			Cette fonction est spécifique à l'API et est disponible via la DLL si elle a été compilée avec le flag _DEV_MODE
*			activé.
*
* \param	void
* \return   void
*/
__declspec(dllexport) void GostxBoardRegenerateStaticTestVectors(void)
{
	DWORD dwReturn = 0;
	DWORD dwIndex = 0;
	USHORT usCurrentTest = 0;
	BOOL	bUp = FALSE;
	//
	// If device info does not exists
	//
	if (!PTR_VALID(pdiDevice))
	{
		pdiDevice = malloc(sizeof(DEVICE_INFO));
		bUp = TRUE;
	}

	//
	// Initiate the encryption key
	//
	pdiDevice->pkiPublic.cPublicKey = TEST_VECTOR_1;

	printf("\n\n---BEGIN OF KEYS GENERATION---\n");
	printf("VECTOR 1 :\n");
	for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
	{
		usCurrentTest = TestVector1[dwIndex].usPlain;
		usCurrentTest = CipherKeyTestMode(usCurrentTest);
		printf("{\t%#04x,\t%#04x\t}\n", TestVector1[dwIndex].usPlain, usCurrentTest);
	}
	StreamCipher(0, 0);
	printf("\nVECTOR 2 :\n");
	pdiDevice->pkiPublic.cPublicKey = TEST_VECTOR_2;
	for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
	{
		usCurrentTest = TestVector1[dwIndex].usPlain;
		usCurrentTest = CipherKeyTestMode(usCurrentTest);
		printf("{\t%#04x,\t%#04x\t}\n", TestVector1[dwIndex].usPlain, usCurrentTest);
	}
	printf("\n\n---END OF GENERATION---\n");
	StreamCipher(0, 0);

	//
	// Clean
	//
	if (bUp)
		free(pdiDevice);
	
}
#endif // dev mode


#endif	 // app dll 




