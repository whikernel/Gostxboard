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
* \file      defines.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Define some defines needed by the driver 
*
* \~French
* \file		defines.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit quelques defines nécessaires au fonctionnement du driver
*
*/

#ifndef _DEFINES_H
#define _DEFINES_H



#include <wdf.h>
#include <kbdmou.h>
#include <Ntdd8042.h>
#include <wmilib.h>
#include "../Commons/crypto.h"

//
// Defines size of an adress depending of 
// the targeted platform
//
#ifdef _M_X64
#define UADDR	UINT64
#else
#define UADDR	UINT32
#endif

/**
* \~English
* \brief	Extract the makecode from a scancode
* 
*\~French
*\brief Extrait le makecode d'un scancode
*
*/
#define GET_MAKE_CODE(_sc_)  (_sc_ & 0x7F)   

//
// Source : minwindef.h
//
typedef unsigned char		UCHAR;
typedef unsigned short		USHORT;
typedef unsigned long		ULONG;
typedef unsigned long       DWORD;

#pragma pack(1)
/**
* \~English
* \brief Structure of the IDT
*
*\~French
*\brief Structure de l'IDT
*
*/

typedef struct _IDTR {
	UINT16 bytes;			/*!<\~English Bytes representing the ID service. \~French Octets représentant l'ID du service*/
	UINT32 addr;			/*!<\~English Corresponding address of the service. \~French Adresse du service correspondant */
} IDTR;
#pragma pack()


#pragma pack(1)
//
// Description d'un descripteur isr
//
/**
* \~English 
* \brief Partially documented structure  leaked on internet. \n Used for IDT hook; 
*
* \~French 
* \brief Structure partiellement documentée qui a fuité sur internet.\n Utilisée pour le hook de l'IDT.
*/

typedef struct _DESC {
	UINT16	offset00;
	UINT16	segsel	;
	CHAR	unused;//	: 5;
	CHAR	zeros;//	: 3;
	CHAR	type;//	: 5;
	CHAR	DPL	;//	: 2;
	CHAR	P	;//1;
	UINT16	offset16;
} DESC, *PDESC;
#pragma pack()


//
// Definition de l'extension
//
typedef struct _DEVICE_EXTENSION
{

    PDEVICE_OBJECT		TopOfStack;			//OExtension du périphérique
	DWORD				Identifier;			//Identifiant du périphérique
	UNICODE_STRING		nNameSpace;			//Nom du périphérique
	WDFQUEUE			rawPdoQueue;		//Queue des requetes du pdo
	WDFQUEUE			rawDispatchPdoQueue;//Queue des requêtes non default du pdo
	CONNECT_DATA		UpperConnectData;	//Données spécifiques au clavier lors d'une connection au driver port
	PVOID				UpperContext;
	PI8042_QUEUE_PACKET					QueueKeyboardPacket;
	PI8042_ISR_WRITE_PORT				IsrWritePort;
	PVOID								CallContext;
	PI8042_KEYBOARD_INITIALIZATION_ROUTINE	UpperInitializationRoutine;
	PI8042_KEYBOARD_ISR						UpperIsrHook;
	PKINTERRUPT								pInterrupt;
	WDFCMRESLIST							CmResList;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;
//
// Déclaration du contexte du périphérique
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_EXTENSION, FilterGetData);


//
// Extension du périphérique pdo
//
typedef struct _RPDO_DEVICE_DATA
{

    ULONG				InstanceNo;		// Instances du périphérique
    WDFQUEUE			ParentQueue;	// Queue d'attentes du parent

} RPDO_DEVICE_DATA, *PRPDO_DEVICE_DATA;
//
// Déclaration du contexte du périphérique
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(RPDO_DEVICE_DATA, PdoGetData);



//
// Device identifier
//
#define		IRP_DEVICE_IDENTIFIER	0x0B38AC69

// Longueur maximale d'un ID
#define MAX_ID_LEN 128

//
// Verification IRP (dispatch.c)
//
static		DWORD	dwCurrentState  =	0;


//
// Structure permettant de sauvergarder le GUID
// et la clé publique
//
typedef struct _APP_CREDENTIALS {
	GUID				guidApp;			//GUID pour identifier (deprec)
	unsigned long long 	cPublicKey;		    //Clé de chiffrement publique
	unsigned long long    MasterKey;		//Master Key
	PEPROCESS			pEprocess;			//PE du processus appelant
	HANDLE				hProcessId;			//Id du processus appelant
	char				cHashPubKey[33];	//Hash de la clé
	WDFDEVICE			wdfCurrentDevice;	//Current DeviceObject
	UNICODE_STRING		DriverName;			// Current driver name 
	UNICODE_STRING		i8042Name;			// i8042prt driver name
	UNICODE_STRING		apciName;			// acpi driver name  	
	PVOID				Unused;				//Inutilisé
} APP_CREDENTIALS, * PAPP_CREDENTIALS;


//
// Structure de passage à un worker pour 
// l'écriture dans un fichier log
//
typedef struct _WORK_ITEM_WRITE {
	PWORK_QUEUE_ITEM	WorkQueueItem;
	PIO_WORKITEM		pioWorkItem;
	PDEVICE_OBJECT		pDeviceObject;
	PVOID				pBuffer;
	size_t				cBuffer;
} WORK_ITEM_WRITE, * PWORK_ITEM_WRITE;




//
// Structure de sauvergarde du nombre d'essais d'une
// routine de completion 
//
typedef struct _IRP_COMPLETION_TRACE {
	PDEVICE_EXTENSION	pDeviceExtension;	//Extension du périphérique
	ULONG				ulAttempt;			//Nb essais
} IRP_COMPLETION_TRACE, *PIRP_COMPLETION_TRACE;


//
// Define du nombre maximum de requete que peut demander une 
// IRP pour la routine Completion. Ce nombre d'essais dépassé, 
// l'IRP sera passée
//
#define IRP_COMPLETION_MAX_REQUEST		1

//
// Max de la stack IRP
//
#define		MAX_IRP_STACK	100

//
// Structure d'une stack interne
// permettant de retenir les états 
// des IRP
//
typedef struct _IRP_STACK {
	DWORD			Stack[MAX_IRP_STACK];		//Stack (5 elements)
	DWORD			TopOfStack;		// First
} IRP_STACK, * PIRP_STACK;

//
// On déclare la stack en statique
//
static IRP_STACK	irpStack;


// Define the common portion of the keyboard/mouse device extension. 
// 
typedef struct _COMMON_DATAE {
	PDEVICE_OBJECT      Self;
	PKINTERRUPT InterruptObject;
	KSPIN_LOCK          InterruptSpinLock;
	PDEVICE_OBJECT      TopOfStack;
	PDEVICE_OBJECT      PDO;
	IO_REMOVE_LOCK RemoveLock;
	PIRP OutstandingPowerIrp;
	DEVICE_POWER_STATE PowerState;
	SYSTEM_POWER_STATE SystemState;
	POWER_ACTION ShutdownType;
	ULONG InputCount;
	LONG EnableCount;
	KTIMER DataConsumptionTimer;
	KDPC RetriesExceededDpc;
	KDPC ErrorLogDpc;
	KDPC TimeOutDpc;
	KDPC ResetDpc;
	ULONG SequenceNumber;
	CONNECT_DATA ConnectData;
	WMILIB_CONTEXT WmiLibInfo;
	OUTPUT_PACKET CurrentOutput;
	CM_PARTIAL_RESOURCE_DESCRIPTOR InterruptDescriptor;
	PNP_DEVICE_STATE PnpDeviceState;
	SHORT ResendCount;
	BOOLEAN OkayToLogOverflow;
	BOOLEAN Initialized;
	BOOLEAN IsIsrActivated;
	BOOLEAN IsKeyboard;
	BOOLEAN Started;
} COMMON_DATA, *PCOMMON_DATAE;

// 
// Define the keyboard portion of the port device extension. 
// 
typedef struct _PORT_KEYBOARD_EXTENSION {
	struct _COMMON_DATAE Common;
	UCHAR PowerCaps;
	UCHAR PowerEvent;
	UCHAR CurrentScanCode, LastScanCode;
	PIRP SysButtonEventIrp;
	KDPC SysButtonEventDpc;
	KSPIN_LOCK SysButtonSpinLock;
	UNICODE_STRING SysButtonInterfaceName;
	KEYBOARD_ATTRIBUTES KeyboardAttributes;
	KEYBOARD_ID_EX KeyboardIdentifierEx;
	KEYBOARD_TYPEMATIC_PARAMETERS KeyRepeatCurrent;
	KEYBOARD_INDICATOR_PARAMETERS KeyboardIndicators;
	KDPC KeyboardIsrDpc;
	KDPC KeyboardIsrDpcRetry; 
	LONG DpcInterlockKeyboard;
	PKEYBOARD_INPUT_DATA InputData;
	PKEYBOARD_INPUT_DATA DataIn;
	PKEYBOARD_INPUT_DATA DataOut;
	PKEYBOARD_INPUT_DATA DataEnd;
	KEYBOARD_INPUT_DATA CurrentInput;
	KEYBOARD_SCAN_STATE CurrentScanState;
	PI8042_KEYBOARD_INITIALIZATION_ROUTINE InitializationHookCallback;
	PI8042_KEYBOARD_ISR IsrHookCallback;
	PVOID HookContext;
	LONG CrashFlags;   
	LONG CurrentCrashFlags;
	UCHAR CrashScanCode;
	UCHAR CrashScanCode2;
	UCHAR FailedReset;
} PORT_KEYBOARD_EXTENSION, *PPORT_KEYBOARD_EXTENSION;
					 
#define OFFSET_HOOK_I8042	sizeof(	COMMON_DATA )					+ \
							4 * sizeof( UCHAR )						+ \
							sizeof(PIRP)							+ \
							sizeof(KDPC)							+ \
							sizeof(KSPIN_LOCK)						+ \
							sizeof(UNICODE_STRING)					+ \
							sizeof(KEYBOARD_ATTRIBUTES)				+ \
							sizeof(KEYBOARD_ID_EX)					+ \
							sizeof(KEYBOARD_TYPEMATIC_PARAMETERS)	+ \
							sizeof(KEYBOARD_INDICATOR_PARAMETERS)	+ \
							2 * sizeof(KDPC)						+ \
							sizeof(LONG) 
  

#endif
