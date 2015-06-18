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
* \file      defines_common.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Defines common defines for the module's parts
*
* \details   Defines the commons defines for both driver and applications.\n
*			Some of theses defines are only used by one of the part of the module,
*			So, there are some module specifics defines.
*
* \~French
* \file		defines_commons.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit les defines communs aux modules
*
* \details Définit les defines communs aux différentes parties du module de protection.\n
*		   Certains de ces defines sont seulement utilisés par l'un des module. Il y a donc 
*		   des defines spécifiques.
*/


#ifndef _COMMON_DEFINES_H
#define _COMMON_DEFINES_H

/**
*\~English
*\brief		Debug flag 
*\details	This flag MUST NOT be used in release versions. \n
*			Some criticals values like credentials can be displayed 
*			when debug flag is set !
*\~French
*\brief		Flag de débug
*\details	Ce flag ne DOIT PAS être utilisé dans les versions release. \n
*			Son activation peut entrainer l'affichage de certaines variables 
*			critiques comme les clés de chiffrements.
*/
#define _DEBUG

/**
*\~English
*\brief		Depreciated flag
*\details	This flag is used to include depreciated functions during the compilation.\n
*			The concerned functions are left in files ONLY for educational purpose. \nThe flag must 
*			be turned off for better efficiency of the protection module. Indeed, some of theses
*			functions are time and resources consuming, increasing the time of the process of cipher.\n
*			Furthermore, the functions concerned by the depreciated flag can cause bug checks and BSOD
*			since the driver is constantly improved and these functions are not maintened anymore. 
*
*\~French
*\brief		Flag de dépréciation
*\details	Ce flag est utilisé pour inclure des fonctions dépréciés lors de la compilation.\n
*			Ces fonctions sont laissés dans les fichiers dans un but SEULEMENT éducatif.\nLe flag doit être 
*			désactivé pour une meilleur optimisation du module. En effet, certaines de ces fonctions 
*			sont gourmandes en ressources et temps, augmentant le temps de chiffrement. \n
*			De plus, les fonctions concernés par ce flag peuvent améner le système à lever des bugchecks et 
*			BSOD car le driver est en constante évolultion et ces fonctions ne sont plus tenues à jour.  
*/
//#define _DEPRECIATED


/**
*\~English
*\brief		Development mode flag
*\details	This flag is used to include some functions in the generation that might be usefull to develop 
*			the driver. These functions are not meant to be in the release version of the module. \n 
*
*\~French
*\brief		Flag de développement
*\details	Ce flag est utilisé pour inclure des fonctions qui pourraient être utiles au developpement du driver. \n
*			Ces fonctions ne devraient pas se retrouver dans des versions finales du module. \n
*/
#define _DEV_MODE



//
// Includes for driver compilation
//
#ifdef _DRIVER_

#define __VERSION__ 	"2.0"

#include "../GostxBoard/driver.h"
#include <kbdmou.h>
#include <Ntdd8042.h>
#include <wmilib.h>
#include <wdf.h>

#elif defined(_APP_) || defined(_DLL_) 
//
// Includes for app and dll compilation 
//
#define WIN32_NO_STATUS		 // Against ntstatus.h redefinition
#include <windows.h>
#undef WIN32_NO_STATUS
#include <stdio.h>
#include <initguid.h> 

#endif 


BOOL bSilent;

#if defined(_APP_) || defined(_DLL_)
	#ifndef _DLL_
		#define DbgPrint(_sc_, ... )  printf(_sc_, __VA_ARGS__)
	#elif _DLL_
		#define DbgPrint(_sc_, ... )  __nop()	// can be used to debug -> printf(_sc_, __VA_ARGS__)//
	#endif
#endif

#if defined(_DEBUG) && defined(_DRIVER_)
#define DbgPrint(_sc_, ... )	if(!bSilent) DbgPrint(_sc_, __VA_ARGS__)
#endif


/**
*\~English
*\brief		Defines number of cipher before changing the key application
*\details	This definee is used to determine the number of key ciphered before
*			the key application must be changed.\n The more the key changed, the better
*			is the security, but the slower is the process.
*
*\~French
*\brief		Définit le nombre de clé chiffrés avant de changer la clé de l'application
*\details	Ce define est utilisé pour déterminer le nombre de clé avant que la clé de l'application
*			ne soit être changée. \n Plus la clé change, meilleure est la protection mais plus lent est
*			le processus.
*
*/
#define		MAX_SCANCODES_BEFORE_REGENERATION	21 // * 2	



//
// -----------  COMMONS CODES  ------------/
//
/**
*\~English
* \brief    Code Message
* \details  Correspondance table of error or success code.
*
*\~French
*\brief		Correspondance code - messages
*\details	Table de correspondance code d'erreurs et de succès
*/
typedef struct _CodeMessageList_ {
	DWORD dwCode;					/*!< \~English Message code return by functions \~French Code du message retourné par les fonctions */
	char * Message;					/*!< \~English Associated string message \~French Message associé au code*/
}CodeMessageList, *PCodeMessageList;


/** \~English Generate error code. Value is >= 0xff.\n #CODE_SUCCESS macro will help to determine the type of code \~French Génère les codes d'erreurs. Leurs valeurs est >= 0xff.\n Le macro #CODE_SUCCESS pourra aider à déterminer le type de code retourné. */
#define ERROR_CODE(_code_)				0xff + (_code_)
/** \~English Unknown error \~French Erreur inconnu */
#define UNKNOWN_ERROR						ERROR_CODE(0)
/** \~English Driver is not available. Try to reboot or reinstall it. \~French Driver non disponible pour une session de chiffrement. Il faut essayer de redémarrer ou réinstaller le driver. */
#define CIPHER_BUSY							ERROR_CODE(1)
/** \~English Specified scancode was not found in the whitelist \~French Scan-code non trouvé dans la liste blanche  */
#define ERR_CODE_NOT_FOUND					ERROR_CODE(2)
/** \~English Driver not found \~French Driver introuvable */
#define ERR_DRIVER_NOT_FOUND				ERROR_CODE(3)
/** \~English Insufficient rights for specified action  \~French Droits insuffisants pour l'action demandée */
#define ERR_INSUFFICIENT_RIGHTS				ERROR_CODE(4)
/** \~English Interface not found \~French Aucune interface du driver trouvée	*/
#define ERR_INTERFACE_NOT_FOUND				ERROR_CODE(5)
/** \~English Driver connection failed \~French Connexion avec le driver impossible. Echec de l'envoie de l'ioctl	 */
#define ERR_CONNECTION_FAILURE				ERROR_CODE(6)
/** \~English Unable to allocate memory \~French Impossibilité d'allouer de la mémoire */
#define ERR_ALLOCATION_FAILURE				ERROR_CODE(7)
/** \~English Unable to lock memory \~French Impossibilité de verrouiller la mémoire */
#define ERR_LOCK_MEMORY_FAILURE				ERROR_CODE(8)
/** \~English Invalid pointer \~French Pointeur mémoire invalide */
#define ERR_INVALID_POINTER					ERROR_CODE(9)
/** \~English Critical error.\n Cipher session cannot be closed.\n Cipher session is still running \~French Erreur critique.\n Impossible de terminer la session.\n La session est toujours active. */
#define CRIT_ERR_CIPHER_STILL_RUNNING		ERROR_CODE(10)
/** \~English Given char is not valid \~French Char fournis non valide	*/
#define ERR_INVALID_CHAR					ERROR_CODE(11)
/** \~English Attempt to cipher while no active session was found \~French Tentative de chiffrement alors qu'il n'y a pas de session active  */
#define ERR_NO_ACTIVE_SESSION				ERROR_CODE(12)
/** \~English No whitelist table was found.\n This is probably a compilation error. \~French Aucune table liste blanche trouvée. Il s'agit surement d'une erreur de compilation  */
#define ERR_NO_SCANCODE_TABLE_FOUND			ERROR_CODE(13)
/** \~English Unable to create a new thread in current process \~French Impossible de créer un nouveau thread dans le processus actuel  */
#define ERR_CREATE_THREAD_FAILURE			ERROR_CODE(14)	
/** \~English Insufficient rights to create a new thread in specified process\~French Les droits sont insuffisants pour créer un nouveau thread dans le processus spécifié */
#define ERR_NO_CREATE_THREAD_RIGHTS			ERROR_CODE(15)	
/** \~English Some given parameters are not valid \~French Certains paramètres fournis ne sont pas valides */
#define ERR_BAD_PARAMETERS					ERROR_CODE(16)	
/** \~English Attempt to start an unitialized cipher session. \~French Tentative de démarrage d'une session non initialisée*/
#define ERR_SESSION_NOT_INITIALIZED			ERROR_CODE(17)	
/** \~English Critical error\n A self test has been made and generation of keys's applications appears to be not safe. \~French Erreur critique.\n Un auto-test a été effectué sur la génération des clés applicatives et a échoué. \n Les clés ne sont pas sûr. */
#define CRIT_ERR_APP_KEYS_COMPROMISED		ERROR_CODE(18)	
/** \~English Critical error\n A self test has been made and MasterKey generation appears to be not safe.\n The problem cannot be auto-resolved. \~French Erreur critique.\n Un auto-test a été effectué sur la génération de la masterkey et a échoué.\nLe problème ne peut-être résolu  */
#define CRIT_ERR_MASTER_KEY_COMPROMISED		ERROR_CODE(19)	
/** \~English A self test has been made and vectors test has failed.\n Ciphering is not sure. \~French Le test des vecteurs a échoué. Le chiffrement n'est pas sûr. */
#define CRIT_ERR_TEST_VECTORS_FAILURE		ERROR_CODE(20)	
/** \~English A strange scancode has been received. \~French Un scancode étrange a été reçu.*/
#define ERR_STRANGE_SCANCODE_DETECTED		ERROR_CODE(21)
/** \~English Too much scancodes were ciphered with the same key and the API has attempted to regenerate the session but something goes wrong. \~French Trop de scancodes ont étés chiffrés avec la même clé et l'API a tenté de regénérer la session sans succès.*/
#define ERR_UNABLE_TO_REGENERATE_SESSION	ERROR_CODE(22)
/** \~English Unable to retrieve PDO from current wdfdevice. \~French Impossibilité de récupérer le PDO du wdfdevice courant. */
#define ERR_PDO_FROM_WDFDEVICE_FAILED		ERROR_CODE(23)
/** \~English THe driver name is not as expected. This error realy should'nt happen. \~French Le nom du driver n'est pas celui attendu. Cette erreur ne devrait vraiment pas arriver. */
#define ERR_INVALID_DRIVER_NAME			ERROR_CODE(24)
/** \~English Place of i8042prt driver is as excpected. It is maybe a hook. \~French La place du driver i8042prt n'est pas celle attendu. Il s'agit peut-être d'une attaque. */
#define ERR_INVALID_I8042_PLACE				ERROR_CODE(25)
/** \~English Place of ACPI driver is not as excpected. It is maybe a hook. \~French La place du driver acpi n'est pas celle attendue. Il s'agit peut-être d'un hook. */
#define ERR_INVALD_ACPI_PLACE				ERROR_CODE(26) 

/**\~English Static #CodeMessageList structure containing equivalents error codes - messages \~French #CodeMessageList structure contenant les équivalences code d'erreurs - messages */
static CodeMessageList CodeErrorMessage[] = {
		{ UNKNOWN_ERROR, "Unknown Error" },
		{ CIPHER_BUSY, "Error cipher busy" },
		{ ERR_CODE_NOT_FOUND, "ScanCode not found" },
		{ ERR_DRIVER_NOT_FOUND, "Driver not found" },
		{ ERR_INSUFFICIENT_RIGHTS, "Insufficient rights" },
		{ ERR_INTERFACE_NOT_FOUND, "Driver interface not found" },
		{ ERR_CONNECTION_FAILURE, "Driver connection failed" },
		{ ERR_ALLOCATION_FAILURE, "Unable to allocate memory" },
		{ ERR_LOCK_MEMORY_FAILURE, "Unable to lock memory" },
		{ ERR_INVALID_POINTER, "Memory pointer not valid" },
		{ CRIT_ERR_CIPHER_STILL_RUNNING, "Critical error - Unable to terminate session - Cipher is still running" },
		{ ERR_INVALID_CHAR, "Given char not valid" },
		{ ERR_NO_ACTIVE_SESSION, "No active session found" },
		{ ERR_NO_SCANCODE_TABLE_FOUND, "No whitelist of scancode has been found - Compilation Error " },
		{ ERR_CREATE_THREAD_FAILURE, "Unable to create a new thread in specified process" },
		{ ERR_NO_CREATE_THREAD_RIGHTS, "Current rights are not sufficients to create a new thread in specified process" },
		{ ERR_BAD_PARAMETERS, "Bad parameters given" },
		{ ERR_SESSION_NOT_INITIALIZED, "Initiate function must be called before begining a cipher session" },
		{ CRIT_ERR_APP_KEYS_COMPROMISED, "App keys test has failed" },
		{ CRIT_ERR_MASTER_KEY_COMPROMISED, "MasterKey test has failed. Unable to correct." },
		{ CRIT_ERR_TEST_VECTORS_FAILURE, "Vectors test failed. Cipher is compromised. " },
		{ ERR_STRANGE_SCANCODE_DETECTED, "A strange, unhandled scancode has been detected. " },
		{ ERR_UNABLE_TO_REGENERATE_SESSION, "Unable to regenerate the session." },
		{ ERR_PDO_FROM_WDFDEVICE_FAILED, "Unable to retrieve PDO from current WdfDevice for security checks. " },
		{ ERR_INVALID_DRIVER_NAME, "Driver name is not as expected. Possible hack.. " },
		{ ERR_INVALID_I8042_PLACE, "I8042 driver place is not as expected. Possible hack.. " },
		{ ERR_INVALD_ACPI_PLACE, "ACPI driver place is not as expected. Possible hack.. " },
		{0,0}
};



//
// Commons returns codes
//
/** \~English Generate return codes. Value is >= 0x00 && <= 0xff.\n #CODE_SUCCESS macro will help to determine the type of code. \~French Génère les codes de retour succès. Leurs valeurs est >= 0x00 && <= 0xff.\n Le macro #CODE_SUCCESS pourra aider à déterminer le type de code retourné. */
#define RETURN_CODE(_code_)			0x00 + (_code_)
/** \~English The operation was successfully ended.\~French L'opération s'est terminée avec succès. */
#define RETURN_SUCCESS				RETURN_CODE(0)
/** \~English The driver is available and healthy.\~French Le driver est disponible et en bonne santé. */
#define DRIVER_AVAILABLE			RETURN_CODE(1)	
/** \~English A cipher session has been successfully started\~French Une session de chiffrement s'est démarrée avec succès */
#define SESSION_STARTED				RETURN_CODE(2)	
/** \~English A cipher session has been successfully closed\~French Une session de chiffrement s'est terminée avec succès */
#define SESSION_CLOSED				RETURN_CODE(3)

/**\~English Static #CodeMessageList structure containing equivalents success codes - messages \~French #CodeMessageList structure contenant les équivalences code de succès - messages */
static CodeMessageList CodeReturnMessage[] = {
		{ RETURN_SUCCESS, "Success" },
		{ DRIVER_AVAILABLE, "Driver available" },
		{ SESSION_STARTED, "Session started" },
		{ SESSION_CLOSED, "Session closed"},
		{ 0, 0 }
};



//
// -----------  MACROS  ------------/
//
/**\~English Check if the given code is a success code. Return #TRUE if so, else #FALSE. \~French Vérifie si le code donné en paramètre est un code succès et renvoie #TRUE dans ce cas, sinon #FALSE. */
#define CODE_SUCCESS(_code_)		(_code_ < 0xff)
/**\~English Check if the given handle is valid. Return #TRUE if so, else #FALSE. \~French Vérifie si le handle donné en paramètre est valide et renvoie #TRUE dans ce cas, sinon #FALSE. */
#define HANDLE_VALID(_handle_)		(_handle_ != INVALID_HANDLE_VALUE) 
/**\~English Check if the given pointer is a valid. Return #TRUE if so, else #FALSE. \~French Vérifie si le pointeur donné en paramètre est valide et renvoie #TRUE dans ce cas, sinon #FALSE. */
#define PTR_VALID(_ptr_)			(_ptr_) //? TRUE : FALSE

//
// Tools macros
//
/**\~English Redefinition of while loop in order to allows the use of constant. \~French Redéfintion du while pour pouvoir utiliser une constante */
#define WHILE(_constant_)			 __pragma(warning(disable: 4127)) while(_constant_); __pragma(warning(default: 4127))

//
// Memory macros
//
/**\~English Free a memory pool after cleaning it.\n The returned pointer is NULL. \~French Libère un espace mémoire après l'avoir réinitialisé.\n Le pointeur de retour est nul. */
#define FREE(_ptr_, _size_)			RtlSecureZeroMemory(_ptr_, _size_); free(_ptr_); _ptr_ = NULL;
/**\~English Free a memory pool locked, after unlocking and cleaning it.\n The returned pointer is NULL. \~French Libère un espace mémoire verouillé, après l'avoir déverouillé et réinitialisé.\n Le pointeur de retour est nul. */
#define VIRTUAL_FREE(_ptr_,_size_)	RtlSecureZeroMemory(_ptr_, _size_); VirtualUnlock(_ptr_, _size_); VirtualFree(_ptr_, 0 , MEM_RELEASE); _ptr_ = NULL;




//
// -----------  VECTORS TESTS  ------------/
//
/**
*\~English
* \brief    Test Vectors										 
* \details  Structure containing data needed for tests vectors. \n 
*			Theses tests check if ciphering a pool of data, with known 
*			keys give the expected results.
*
*\~French
*\brief		Vecteurs de test
*\details	Structure contenant les données nécessaires aux tests par vecteurs. \n
*			Ces tests vérifie que le chiffrement sur un ensemble de données, avec un clé 
*			connue, donne le résultat attendu. 
*/
typedef struct _TEST_VECTOR {
	USHORT	usPlain;				/*!< \~English Plain text reference value \~French Valeur en clair de référence */
	USHORT	usCipherScan1;		    /*!< \~English Ciphered value with #TEST_VECTOR_1 \~French Valeur chiffré grâce à la clé #TEST_VECTOR_1 */
	USHORT	usCipherScan2;			/*!< \~English Ciphered value with #TEST_VECTOR_2 \~French Valeur chiffré grâce à la clé #TEST_VECTOR_2 */
} TEST_VECTOR, *PTEST_VECTOR;

/**
*\~English Size of each vector to test. \n This value must be changed if values are added or removed to the vectors.  
*\~French Représente la taille des vecteurs de tests.\n Cette valeur doit-être changé si des données sont ajoutés ou retirés des vecteurs. 
*/
#define TEST_VECTOR_SIZE	10
/**
*\~English First test key, used to generate the first pool of vectors. \n This key must be only used to check the smooth running of the cipher engine, and only for this. 
*\~French Première clé de test, utilisée pour générer le premier vecteur de test. \n Cette clé doit-être seulement utilisée pour vérifier le bon fonctionnement du chiffrement, et seulement pour ça.
*/
#define TEST_VECTOR_1		0xE588E4CD87564321ULL;
/**
*\~English Second test key, used to generate the second pool of vectors. \n This key must be only used to check the smooth running of the cipher engine, and only for this.
*\~French Deuxième clé de test, utilisée pour générer le second vecteur de test. \n Cette clé doit-être seulement utilisée pour vérifier le bon fonctionnement du chiffrement, et seulement pour ça.
*/
#define TEST_VECTOR_2		0xD4459B95879A07F3ULL;
/**
*\~English Test vectors pre-generated. \n The first columns contains the plain text values, the seconds are values ciphered with #TEST_VECTOR_1 key, the third are values ciphered with #TEST_VECTOR_2 key.\n For tests, one must ensure that 
*	each ciphered value of vectors 1 and 2 gives the plain text value when computed. \n
*	Carefull ! Each change in the whitelist involves the vectors tests to be recomputed. For this, it is recommended to use the function GostxBoardRegenerateTestVector (flag DEV_MODE has to be enabled). The function has to be run, 
*	the vectors values replaced with displayed ones and then the driver and the API recompiled.\n
*	Really, it has to be done, otherwise security tests will failed.
*	
*\~French Vecteurs de tests précalculés.\n La première colonne contient les valeurs en clair, la deuxième représente les données chiffrées avec la clé #TEST_VECTOR_1, et la troisième représente les données chiffrés avec la clé #TEST_VECTOR_2.\n 
*	Pour les tests, il faut s'assurer que chaque valeurs chiffrés des vecteurs 1 et 2 redonne bien la valeur déchiffrée lorsqu'elle est traité par l'algorithme. \n
*	Attention ! Chaque changement de la liste blanche implique un recalcule des vecteurs de tests. Pour cela, il est conseillé d'utiliser la fonction GostxBoardRegenerateTestVector utilisable via le flag DEV_MODE. Il faut exécuter la fonction, 
*	remplacer les valeurs obtenues dans ce tableau, puis recompiler le driver et l'API. \n 
*	Si cela n'est pas fait, les tests de sécurités échoueront. 
*/
static TEST_VECTOR TestVector1[] = {
//		PLAIN		VECTOR1		VECTOR2
		{ 0x35,		0x26,		0x11 },
		{ 0x12,		0x19,		0x2b },
		{ 0x14,		0x16,		0x35 },
		{ 0x16,		0x09,		0x32 },
		{ 0x18,		0x09,		0x0d },
		{ 0x20,		0x03,		0x08 },
		{ 0x23,		0x26,		0x14 },
		{ 0x25,		0x25,		0x10 },
		{ 0x2e,		0x2c,		0x1f },
		{ 0x35,		0x06,		0x39 }
};



//
// -----------  GUID DEFINES  ------------/
//
/**
*\~English Interface GUID provided by the driver for a client application who wants to connected itself with the driver. \n The application must look for this GUID in order to find the interface. \n The API automatically does this. 
*\~French GUID de l'interface fournis par le driver pour une application qui souhaite se connecter au driver. \n L'application doit chercher l'interface correspondante au GUID se connecter. \n L'API réalise ceci de manière transparente.  
*/
DEFINE_GUID(GUID_DEVINTERFACE_GOSTXBOARD,			 
0x3fb7299d, 0x6847, 0x4490, 0xb0, 0xc9, 0x99, 0xe0, 0x98, 0x6a, 0xb8, 0x86);
// {3FB7299D-6847-4490-B0C9-99E0986AB886}
// Keyboard ID
#ifdef _DRIVER_
#define  KBFILTR_DEVICE_ID L"{A65C87F9-BE02-4ed9-92EC-012D416169FA}\\KeyboardFilter\0"
// {40AC4921-16C7-4CF3-8E22-BCBAB4603FDB}
#elif _APP_
static const GUID APP =
{ 0x40ac4921, 0x16c7, 0x4cf3, { 0x8e, 0x22, 0xbc, 0xba, 0xb4, 0x60, 0x3f, 0xdb } };
#endif

//
// -----------  COMMON DEFINES  ------------/
//
#define MAX_REGISTRY_VALUE_SIZE		4096

//
// -----------  IOCTL DEFINES  ------------/
//
#define SIOCTL_TYPE		40000
/**
*\~English Define custom IOCTL used by the driver and the application to communicate together. 
*\~French GDéfinit des IOCTL custom utilisé par le driver et l'application pour communiquer entre-eux.
*/
#define DEF_IOCTL(_code_) (CTL_CODE (FILE_DEVICE_UNKNOWN, 0x800 + (_code_),			 \
				METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA))

/**
*\~English This IOCTL allows an application to send to the driver the #PUBLIC_KEY_INIT structure which will contains credentials to begin a cipher session. \n Structures sent MUST be allocated before. 
*\~French Cette IOCTL permet à une application d'envoyer au driver la structure #PUBLIC_KEY_INIT qui contiendra les identifiants utilisées pour démarrer une session de chiffrement. \n Les structures DOIVENT être initialisées avant d'être envoyées.
*/
#define IOCTL_INIT_KEY			DEF_IOCTL(0)
/**
*\~English This IOCTL allows an application to send to the driver the #PUBLIC_CLOSE_SESSION structure which is used to end a cipher session. \n Structures sent MUST be allocated before.
*\~French Cette IOCTL permet à une application d'envoyer au driver la structure #PUBLIC_CLOSE_SESSION, utilisée pour terminer une session de chiffrement. \n Les structures DOIVENT être initialisées avant d'être envoyése.
*/
#define	IOCTL_CLOSE_KEY			DEF_IOCTL(1)
/**
*\~English This IOCTL allows an application to send to the driver the #PUBLIC_TEST_SESSION structure which is used to test the availability and health of the driver. \n Structures sent MUST be allocated before.
*\~French Cette IOCTL permet à une application d'envoyer au driver la structure #PUBLIC_TEST_SESSION, utilisée pour tester la disponibilité et la santé du driver.\n Les structures DOIVENT être initialisées avant d'être envoyés.
*/
#define IOCTL_TEST_CONNECTION	DEF_IOCTL(2)


//
// -----------  STRUCTURES  ------------/
//
/**
*\~English
* \brief    Init Session Structure
* \details  This structure is used to begin a cipher session beetwen a client application and the driver. \n
*			It MUST be allocated by the application in safe memory pool. The structure is designed to be sent over the #IOCTL_INIT_KEY ioctl. \n
*			API is allocating and sending this structure automatically in a safe way.  
*
*\~French
*\brief		Structure d'initialisation d'une session
*\details	Cette structure est utilisée pour démarrer une session de chiffrement entre une application cliente et le driver. \n 
*			Elle DOIT être allouée de manière sécurisée dans l'espace mémoire de l'application. Cette structure est faite pour être envoyé via l'ioctl #IOCTL_INIT_KEY. \n
*			L'API alloue et envoie automatiquement de façon sécurisé cette structure.
*/
typedef struct _PUBLIC_KEY_INIT {
	GUID				guidApp;			 /*!< \~English [Unused] GUID used to identify calling application.\n This field is unused in current versions of the driver. \~French [Inutilisé] GUID utilisé pour identifier l'application appelante.\nCe champs n'est pas utilisé dans les versions actuelles du driver */
	ULONGLONG			cPublicKey;			 /*!< \~English Cipher key used to cipher scancode.\n This key is unique to the cliente application, and is meant to be changed frequently to ensure maximal security \~French Clé de chiffrement utilisé pour chiffrer les scancodes.\n Cette clé est unique à l'application, et est prévue pour changer fréquement. */
	DWORD				dwError;			 /*!< \~English Error code returned by the driver.\n This code tells you if something goes wrong. \~French Code d'erreur retourné par le driver.\n Ce code permet de savoir si quelque chose ne va pas */
	BYTE 				cHashPubKey[33];	 /*!< \~English [Unused] SHA256 hash of the cPublicKey\~French [Inutilisé] Hash SHA256 de la clé cPublicKey  */
	BOOL				bCipherStarted;		 /*!< \~English Boolean used to indicate that the session is started. \~French Booléen utilisé pour indiquer que la session de chiffrement à démarrer */		
} PUBLIC_KEY_INIT, *PPUBLIC_KEY_INIT ;


/**
*\~English
* \brief    Close Session Structure
* \details  This structure is used to close a cipher session beetwen a client application and the driver. \n
*			It MUST be allocated by the application in safe memory pool. The structure is designed to be sent over the #IOCTL_CLOSE_SESSION ioctl. \n
*			API is allocating and sending this structure automatically in a safe way.
*
*\~French
*\brief		Structure de fin d'une session
*\details	Cette structure est utilisée pour terminer une session de chiffrement entre une application cliente et le driver. \n
*			Elle DOIT être allouée de manière sécurisée dans l'espace mémoire de l'application. Cette structure est faite pour être envoyé via l'ioctl #IOCTL_CLOSE_KEY. \n
*			L'API alloue et envoie automatiquement de façon sécurisé cette structure.
*/
typedef	struct _PUBLIC_CLOSE_SESSION {
	GUID		guidApp;					/*!< \~English [Unused] GUID used to identify calling application.\n This field is unused in current versions of the driver. \~French [Inutilisé] GUID utilisé pour identifier l'application appelante.\nCe champs n'est pas utilisé dans les versions actuelles du driver */
	BOOL		bAppisDeleted;				/*!< \~English Boolean used to indicate that the session is closed. \~French Booléen utilisé pour indiquer que la session de chiffrement est terminée. */
	DWORD		dwError;					/*!< \~English Error code returned by the driver.\n This code tells you if something goes wrong. \~French Code d'erreur retourné par le driver.\n Ce code permet de savoir si quelque chose ne va pas */
	PVOID		Unused;						/*!< \~English [Unused] Unused field. \~French [Inutilisé] Champ inutilisé */
} PUBLIC_CLOSE_SESSION, *PPUBLIC_CLOSE_SESSION;



/**
*\~English
* \brief    Test Session Structure
* \details  This structure is used to test a cipher session beetwen a client application and the driver. \n
*			It MUST be allocated by the application in safe memory pool. The structure is designed to be sent over the #IOCTL_TEST_SESSION ioctl. \n
*			API is allocating and sending this structure automatically in a safe way.
*
*\~French
*\brief		Structure de test d'une session
*\details	Cette structure est utilisée pour tester une session de chiffrement entre une application cliente et le driver. \n
*			Elle DOIT être allouée de manière sécurisée dans l'espace mémoire de l'application. Cette structure est faite pour être envoyé via l'ioctl #IOCTL_TEST_CONNECTION. \n
*			L'API alloue et envoie automatiquement de façon sécurisé cette structure.
*/
typedef struct _PUBLIC_TEST_CONNECTION {
	BOOL		DriverAvailable;				/*!< \~English Boolean used to indicate that the driver is available and healthy. \~French Booléen utilisé pour indiquer que le driver est disponible et en bonne santé. */
	PUCHAR		usCurrentTest;					/*!< \~English Pointer to an uchar used to test the cipher. \n Return value and processed value must be the same, so that the application konws that cipher is smoothly working. \~French Pointer vers un uchar utilisé pour tester le chiffrement. \n La veleur de retour et la valeur calculée par l'application doivent être la même pour certifier le bon fonctionnement du driver. */
	DWORD		dwError;						/*!< \~English Error code returned by the driver.\n This code tells you if something goes wrong. \~French Code d'erreur retourné par le driver.\n Ce code permet de savoir si quelque chose ne va pas */
	TEST_VECTOR TestVector[TEST_VECTOR_SIZE];	/*!< \~English A test vector initiated by the API and used to checks if the encryption system is smoothly working. \~French Un vecteur de test initialisé par l'API, et utilisé pour vérifier si le système de chiffrement fonctionne correctement. */
	unsigned long long TestKey;					/*!< \~English A 64bits test key used to encrypt the test vector. \~French Un clé de test de 64bits utilisée pour chiffrer le vecteur de test.*/
} PUBLIC_TEST_CONNECTION, *PPUBLIC_TEST_CONNECTION;




#ifdef _DLL_

extern _inline 	void PrintEx(char * Message, BYTE bLevel);

#define EXEC(_ret_,_fct_)		_ret_ = _fct_; pdiDevice->dwLastReturnCode = _ret_;

/**
*\~English
* \brief	Api verbose callback structure
* \details  This structure is specific to the API and is used by the callback VerboseCallback to indicate the client application what's going on 
*			inside the API. 
*
*\~French
*\brief		Structure du callback verbose
*\details	Cette structure est spécifique à l'API et est utilisée par le callback VerboseCallback pour indiquer à l'application cliente ce qu'il se passe 
*			au sein de l'API.
*/
typedef struct _VERBOSE_INFO {
	char *		Message;						/*!< \~English Message wich explains the cause of the call of the function. \~French Message expliquant la raison de l'appel de la fonction. */
	DWORD		dwLastReturnCode;				/*!< \~English Last return code which has trigger the call. \~French Dernier code de retour renvoyé, qui a declenché l'appel. */
	BYTE		bLevel;							/*!< \~English Severity of the call.\n 0 means a standard output. Flag [CALL_STD]. \n 1 means a warning. Flag [CALL_WARN].\n 2 means an error.Flag [CALL_ERR]. \n 3 means a critical error. Flag [CALL_CRIT]. \
													\~French Sévérité de l'appel.\n 0 signifie une sortie standard. Flag [CALL_STD].\n 1 signifie un warning. Flag [CALL_WARN].\n 2 signifie une erreur. Flag [CALL_ERR].\n 3 signifie une erreur critique. Flag [CALL_CRIT].*/
} VERBOSE_INFO, *PVERBOSE_INFO;

#define CALL_STD	0x0
#define CALL_WARN 	0x1
#define CALL_ERR	0x2
#define CALL_CRIT	0x3
#define CALL_STOP	0x4
#define CALL_START	0x5

#define PRINT_STD(_sc_)			PrintEx(_sc_, CALL_STD)
#define PRINT_WARN(_sc_)		PrintEx(_sc_, CALL_WARN)
#define PRINT_ERR(_sc_)			PrintEx(_sc_, CALL_ERR)
#define PRINT_CRIT(_sc_)		PrintEx(_sc_, CALL_CRIT)
#define NOTIFY_START(_ret_)		if(CODE_SUCCESS(_ret_)) PrintEx(NULL, CALL_START)
#define NOTIFY_STOP(_ret_)		if(CODE_SUCCESS(_ret_)) PrintEx(NULL, CALL_STOP)


/**
*\~English
* \brief	Verbose callback
* \details  A cliente application can register this callback to be notify each time something special occurs inside the API. 
*
*\~French
*\brief		Callback verbeux (oui je n'ai pas trouvé mieux comme nom.. )
*\details	Une application cliente peut enregistrer ce callback pour être notifié à chaque fois que quelque chose de spécial 
*			se passe au seins de l'API. 
*/
typedef void(*PGOSTXBOARD_VERBOSE_CALLBACK) (
	_In_ char *		Message,
	_In_ DWORD 		dwLastReturnCode,
	_In_ BYTE		bLevel
);


/**
*\~English
* \brief    Device Info Structure
* \details  This structure is specific to the API and is used to store the informations of the interface provided by the driver and the credentials. \n
*			It MUST be allocated by the application in safe memory pool.\n
*			API is allocating this structure automatically in a safe way.
*
*\~French
*\brief		Structure d'informations du périphérique
*\details	Cette structure est spécifique à l'API et est utilisée pour sauvegarder les informations liés à l'interface fournis par le driver and the credentials. \n
*			Elle DOIT être allouée de manière sécurisée dans l'espace mémoire de l'application.\n
*			L'API alloue automatiquement de façon sécurisé cette structure.
*/
typedef struct _DEVICE_INFO	{
	HANDLE				hDevice;					/*!< \~English Handle on the device interface provided by the driver.\n This handle is used by the API to connect itself with the driver. \~French Handle sur l'interface périphérique fournis par le driver.\n Ce handle est utilisé par l'API pour se connecter au driver. */
	PUBLIC_KEY_INIT		pkiPublic;					/*!< \~English Structure used to store application credentials. \~French Structure utilisée pour sauvegarder les identifiants de l'application */
	DWORD				dwNumberOfKeys;				/*!< \~English Number of keys encrypted with the same key. \~French Nombre de touches chiffrés avec une même clé. */
	DWORD               dwLastReturnCode;			/*!< \~English Last code returned. \~French Dernier code de retour retourné. */
	HKL					KeyboardLayout;				/*!< \~English Define the current keyboard layout. It is used to remape scancodes to chars. \~French Définit le standard clavier actuel. Il est utilisé pour remapper les scancodes en chars.*/
	HANDLE				hMonitorThread;				/*!< \~English Handle on the monitoring thread if started. \~French Handle sur le thread de monitoring, si démarré. */
	BYTE				hExitEvent;					/*!< \~English Exit event used by the monitoring thread. \~French Evenement de srotie du thread de monitoring. */
	PGOSTXBOARD_VERBOSE_CALLBACK VerboseCallback;	/*!< \~English The client application can register this callback. It will be called each time special event occured. \~French Une application cliente peut enregistrer ce callback. A chaque fois qu'un évenement spécial intervient, l'API appelera ce callback. */
	VERBOSE_INFO		VerboseInfo;				/*!< \~English Structure used to store callback's informations. \~French Structure utilisée pour sauvegarder les informations du callback.*/
}DEVICE_INFO, *PDEVICE_INFO;




#endif


#endif