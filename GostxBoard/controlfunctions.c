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
* \file     controlfunctions.c
* \author   P.A
* \version  1.0
* \date     May 21, 2015
* \brief    Defines control functions
*
* \details  Defines control functions such as IOCTL handler. \n
*			All communication and control requests are processed here.
*			
*
* \~French
* \file		controlfunctions.c
* \author	P.A
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit les fonctions de contrôles
*
* \details	Définit les fonctions de contrôles, comme les fonctions de 
*			traitement des IOCTL. \n 
*			Toutes les communications inter-driver/application sont traitées
*			ici.
*/


#include "idthook.h"
#include "controlfunctions.h"
#include "../Commons/crypto.h"
#include "../Commons/defineKbFr.h"

#include <ntdd8042.h>

#include "defines.h"

USHORT		uMemory = 0;

extern PAPP_CREDENTIALS	pCurrentApp;

/**
* \~English
* \brief    Device control for raw pdo handler	
* \details  Dispatch routine for raw pdo IOCTL. \n 
*			These IOCTL are send by a client application which 
*			wants to communicate with the driver. They are 
*			forwarded to the parent's fdo in order to be processed.
*
*\param		WDFQUEUE      Queue		A handle to a queue object associated with the ioctl
*\param		WDFREQUEST    Request	A handle to a request object associated with the ioctl 
*\param		size_t        OutputBufferLength  Size of the ouput buffer of the ioctl
*\param		size_t        InputBufferLength	   Size of the inpout buffer of the ioctl
*\param	    ULONG         IoControlCode		Code of the ioctl 
*
*
* \~French
* \brief    Routine de contrôle du raw pdo
* \details  Routine de contrôle du raw pdo. Elle reçoit les IOCTLs
*			destinées au raw pdo. Ces IOCTL sont envoyés par une application 
*			souhaitant communiquer avec le driver. Elles sont transmises au 
*			fdo du driver pour être traités
*
*\param		WDFQUEUE      Queue		Un handle sur l'objet de file associé à l'ioctl
*\param		WDFREQUEST    Request	Un handle sur l'objet de la requête associée à l'ioctl
*\param		size_t        OutputBufferLength  Taille du buffer sortant de l'ioctl
*\param		size_t        InputBufferLength	  Taille du buffer entrant de l'ioctl
*\param	    ULONG         IoControlCode		Code de l'ioctl
*
*/
VOID  GostxBoard_EvtIoDeviceControlForRawPdo(
    _In_ WDFQUEUE      Queue,
    _In_ WDFREQUEST    Request,
    _In_ size_t        OutputBufferLength,
    _In_ size_t        InputBufferLength,
    _In_ ULONG         IoControlCode
    )
{
    NTSTATUS						ntStatus			= STATUS_SUCCESS;
	WDFDEVICE						wdfParent			= NULL;		
    PRPDO_DEVICE_DATA				pdoData				= NULL;
	WDF_REQUEST_FORWARD_OPTIONS		forwardOptions		= {0};

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
	UNREFERENCED_PARAMETER(Queue);

#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_EvtIoDeviceControleForRawPdo::Entry\n");
#endif

	//
	// Get the data from queue
	//
	wdfParent = WdfIoQueueGetDevice(Queue);
	pdoData = PdoGetData(wdfParent);

    switch (IoControlCode) {

		case IOCTL_INIT_KEY :
		case IOCTL_CLOSE_KEY :
		case IOCTL_TEST_CONNECTION :
			//
			// Forward the request to the parent's fdo, 
			// so that they can be processed
			//
				WDF_REQUEST_FORWARD_OPTIONS_INIT(&forwardOptions);
				ntStatus = WdfRequestForwardToParentDeviceIoQueue(Request, 
							pdoData->ParentQueue, &forwardOptions);
				if( !NT_SUCCESS(ntStatus) )
				{
					 WdfRequestComplete(Request, ntStatus);
				}
			break;
    default:
        WdfRequestComplete(Request, ntStatus);
        break;
    }

#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_EvtIoDeviceControleForRawPdo::Exit\n");
#endif

    return;
}



/**
* \~English
* \brief    Internal Device control routine
* \details  Dispatch routine for internal IOCTL. \n
*			These IOCTL are send by other drivers or OS and are destinated 
*			to the driver. This allows the driver to register some callbacks
*			needed for the protection, as IsrHook. \n
*			There are a lot of depreciated code, there left here for educationnal
*			purpose only.
*
*\param		WDFQUEUE      Queue		A handle to a queue object associated with the ioctl
*\param		WDFREQUEST    Request	A handle to a request object associated with the ioctl
*\param		size_t        OutputBufferLength  Size of the ouput buffer of the ioctl
*\param		size_t        InputBufferLength	   Size of the inpout buffer of the ioctl
*\param	    ULONG         IoControlCode		Code of the ioctl
*
*
* \~French
* \brief    Routine de contrôle interne
* \details  Routine de contrôle interne. \nElle reçoit les IOCTLs	 
*			envoyées par les autres drivers ou l'OS. Cela permet au driver 
*			d'enregistrer des callbacks nécessaires au fonctionnement du
*			module de protection, comme IsrHook. Il y a de nombreuses parties
*			de code dépréciées. Elles sont laissées dans un but éducatif. 
*			
*
*\param		WDFQUEUE      Queue		Un handle sur l'objet de file associé à l'ioctl
*\param		WDFREQUEST    Request	Un handle sur l'objet de la requête associée à l'ioctl
*\param		size_t        OutputBufferLength  Taille du buffer sortant de l'ioctl
*\param		size_t        InputBufferLength	  Taille du buffer entrant de l'ioctl
*\param	    ULONG         IoControlCode		Code de l'ioctl
*
*/
VOID  GostxBoard_EvtIoInternalDeviceControl(
    _In_ WDFQUEUE      Queue,
    _In_ WDFREQUEST    Request,
    _In_ size_t        OutputBufferLength,
    _In_ size_t        InputBufferLength,
    _In_ ULONG         IoControlCode
    )
{

	NTSTATUS							ntStatus						= STATUS_SUCCESS;
	WDFDEVICE							wdfDevice						= NULL;
	PDEVICE_EXTENSION					pDeviceExtension				= NULL;
	size_t								szLength						= { 0 };
	PINTERNAL_I8042_HOOK_KEYBOARD		hookKeyboard					= NULL;
	WDF_REQUEST_SEND_OPTIONS			wdfOptions						= { 0 };

#ifdef _DEPRECIATED
	PCONNECT_DATA						pConnectData					= { 0 };
	PINTERNAL_I8042_START_INFORMATION	hookI8042						= NULL;
	PCM_PARTIAL_RESOURCE_DESCRIPTOR		desc							= NULL;
	KIRQL								kIrql							= { 0 };
	KINTERRUPT_MODE						kiMode							= { 0 };
	BOOLEAN								bIrqShare						= TRUE;
	ULONG								uIndex							= 0,
										uVector							= 0,
										uNbDesc							= 0;
	KAFFINITY							kAffinity						= { 0 };
#endif // Depreciated

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Entry \n");
	#endif
	UNREFERENCED_PARAMETER(OutputBufferLength);
	UNREFERENCED_PARAMETER(InputBufferLength);

	//
	// Check if IRQL allows pagination. 
	// It is not necessary to be higher
	//
	PAGED_CODE();

	//
	// Get the data from queue 
	//
	wdfDevice = WdfIoQueueGetDevice(Queue);
	pDeviceExtension = FilterGetData(wdfDevice);

	switch( IoControlCode )
	{
		case IOCTL_INTERNAL_KEYBOARD_CONNECT:
#ifdef _DEPRECIATED
// [DEPRECIATED] //
		//
		// A keyboard class device driver has been connected 
		// to the driver port. We allow only one connection
		//
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Handling KEYBOARD_CONNECT ioctl \n");
		#endif
			if (pDeviceExtension->UpperConnectData.ClassService != NULL) {
				ntStatus = STATUS_SHARING_VIOLATION;
				break;
			}

			//
			// Get the input buffer of request, and save it in the device extension
			//
			ntStatus = WdfRequestRetrieveInputBuffer(Request,
				sizeof(CONNECT_DATA),
				&pConnectData,
				&szLength);
			if (!NT_SUCCESS(ntStatus)){
				#ifdef _DEBUG
					DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Error retieving buffer \n");
				#endif
				break;
			}

			NT_ASSERT(szLength == InputBufferLength);

			pDeviceExtension->UpperConnectData = *pConnectData;

			//
			// Register the callback. Each time a request will be send by lower driver, 
			// we will be notified. Theses packets contain keystrokes. So they can be 
			// ciphered
			//
			pConnectData->ClassDeviceObject = WdfDeviceWdmGetDeviceObject(wdfDevice);

			#pragma warning(disable:4152)  // Non autorised conversion ptr/fct
				pConnectData->ClassService = GostxBoard_ServiceCallback;
			#pragma warning(default:4152)
#endif // Depreciated
			break;
		case IOCTL_INTERNAL_I8042_HOOK_KEYBOARD :
				#ifdef _DEBUG
					DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Handling I8042_HOOK ioctl \n");
				#endif

				//
				// Get the imput buffer of the request and saved it in the device extension
				//
				ntStatus = WdfRequestRetrieveInputBuffer(Request,
					sizeof(INTERNAL_I8042_HOOK_KEYBOARD),
					&hookKeyboard,
					&szLength);
				if (!NT_SUCCESS(ntStatus))
				{
					#ifdef _DEBUG
						DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Error retieving buffer \n");
					#endif
					break;
				}
				NT_ASSERT(szLength == InputBufferLength);


				//
				// Register our initialization routine and replace the contexte with ours. 
				// Register our interrupt service routine to the i8042prt port driver.
				// This will be used to retrieve scancodes before any processing.
				//

				pDeviceExtension->UpperContext = hookKeyboard->Context;

				hookKeyboard->Context = (PVOID)pDeviceExtension;

				if (hookKeyboard->IsrRoutine) {
					pDeviceExtension->UpperIsrHook = hookKeyboard->IsrRoutine;
				}
				hookKeyboard->IsrRoutine = (PI8042_KEYBOARD_ISR)GostxBoard_IsrHook;
		

				//
				// Store every previous declaration in othe device extension
				//
				pDeviceExtension->IsrWritePort = hookKeyboard->IsrWritePort;
				pDeviceExtension->QueueKeyboardPacket = hookKeyboard->QueueKeyboardPacket;
				pDeviceExtension->CallContext = hookKeyboard->CallContext;

				ntStatus = STATUS_SUCCESS;
			break;
		case IOCTL_INTERNAL_I8042_KEYBOARD_START_INFORMATION :
#ifdef _DEPRECIATED
// [DEPRECIATED] // 
			#ifdef _DEBUG
				DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Handling INTERNAL_I8042_START ioctl \n");
			#endif
			//
			// Get the imput buffer of the request and saved it in the device extension
			//
			ntStatus = WdfRequestRetrieveInputBuffer(Request,
				sizeof(INTERNAL_I8042_START_INFORMATION),
				&hookI8042,
				&szLength);
			if (!NT_SUCCESS(ntStatus))
			{
				#ifdef _DEBUG
					DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Error retieving buffer \n");
				#endif
				break;
			}
			NT_ASSERT(szLength == InputBufferLength);
			
			//
			// The hookI8042 structure contains the interrupt code generated by a keystroke.
			// We save it on the device extension.
			//
			pDeviceExtension->pInterrupt = hookI8042->InterruptObject;

			//
			// Register a callback on the received routine in order to be notified when 
			// an interrupt is generated. We get the device descriptor to initialize the ISR
			//
			uNbDesc = WdfCmResourceListGetCount(pDeviceExtension->CmResList);
			for (uIndex = 0; uIndex < uNbDesc; uIndex++)
			{
				//
				// For all the lists of resources, get the descriptors and look for interrupts 
				// associated in order to inherit parameters
				//
				desc = WdfCmResourceListGetDescriptor(
					pDeviceExtension->CmResList,
					uIndex);
				switch (desc->Type)
				{
					case CmResourceTypeInterrupt :
						kIrql		= (KIRQL)desc->u.Interrupt.Level;
						uVector		= desc->u.Interrupt.Vector;
						kAffinity	= desc->u.Interrupt.Affinity;
						kiMode		= (desc->Flags == CM_RESOURCE_INTERRUPT_LATCHED) ? Latched : LevelSensitive ;
						bIrqShare	= (desc->ShareDisposition == CmResourceShareShared);
						break;
				}
			}
			//
			// Associate our ISR to the interrupt
			//
			ntStatus = IoConnectInterrupt(&hookI8042->InterruptObject,
				(PKSERVICE_ROUTINE)InterruptKeyboard,
				(PVOID)pDeviceExtension, NULL,
				uVector, kIrql, kIrql, kiMode, bIrqShare, kAffinity, FALSE);
			if (! NT_SUCCESS(ntStatus))
			{
				#ifdef _DEBUG
					DbgPrint("\n[X] GostxBoard_InternalIOCTLHandler::Unable to set ISR on keybd interrupt.Exit Code %#08x\n\n", ntStatus);
				#endif
			}
			/*#ifdef _DEBUG
					DbgPrint("\n[+] GostxBoard_InternalIOCTLHandler::Trying to corrupt IDT\n", ntStatus);
				#endif
			#pragma warning( disable : 4305)
				// Hook the Isr
				HookIsr(0x80, (UINT32)hookroutine);
			#pragma warning( default: 4305)		*/
#endif // Depreciated
			break;
		case IOCTL_INTERNAL_KEYBOARD_DISCONNECT: 
			//
			// The keyboard has been disconnected from port driver.
			// Delete connections parameters associated to the device extension.
			//
			#ifdef _DEBUG
				DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Handling KEYBOARD_DISCONNECT ioctl \n");
			#endif
				pDeviceExtension->UpperConnectData.ClassDeviceObject = NULL;
				pDeviceExtension->UpperConnectData.ClassService = NULL;
				
				ntStatus = STATUS_SUCCESS;
			break;

		//
		// By default, do nothing with IOCTLs
		//
		case IOCTL_KEYBOARD_QUERY_ATTRIBUTES:
		case IOCTL_KEYBOARD_QUERY_INDICATOR_TRANSLATION:
		case IOCTL_KEYBOARD_QUERY_INDICATORS:
		case IOCTL_KEYBOARD_SET_INDICATORS:
		case IOCTL_KEYBOARD_QUERY_TYPEMATIC:
		case IOCTL_KEYBOARD_SET_TYPEMATIC:
		default :
			#ifdef _DEBUG
				DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Handling default ioctl code %#08X \n", IoControlCode);
			#endif
			break;
    }

	//
	// Check if everything's fine and complete request if not
	//
	if ( !NT_SUCCESS(ntStatus) ) 
	{
		#ifdef _DEBUG
			DbgPrint("\n[x] GostxBoard_GostxBoard_InternalIOCTLHandler::An error occurred while trying to handle IOCTL. \n");
		#endif
        WdfRequestComplete(Request, ntStatus);
        return;
    }

	//
	// We are not interested in post processing the IRP so 
	// fire and forget.
	//
	WDF_REQUEST_SEND_OPTIONS_INIT(&wdfOptions,
		WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET);

	if( WdfRequestSend(Request, WdfDeviceGetIoTarget(wdfDevice), &wdfOptions)
		== FALSE )
	{
		ntStatus = WdfRequestGetStatus(Request);
		#ifdef _DEBUG
			DbgPrint("[x] GostxBoard_GostxBoard_InternalIOCTLHandler::WdfRequestSend failed: 0x%x\n", ntStatus);
		#endif
		WdfRequestComplete(Request, ntStatus);
	}
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_InternalIOCTLHandler::Exit \n");
	#endif

	return;
}


/**
* \~English
* \brief    Device control routine from raw pdo
* \details  Dispatch routine for raw pdo ioctl. \n
*			These ioctl are send by the raw pdo, when it receives 
*			a ioctl from a client application. \n This function processes  
*			the application ioctl such as connection with the driver and 
*			start of a cipher session.
*			
*\param		WDFQUEUE      Queue		A handle to a queue object associated with the ioctl
*\param		WDFREQUEST    Request	A handle to a request object associated with the ioctl
*\param		size_t        OutputBufferLength  Size of the ouput buffer of the ioctl
*\param		size_t        InputBufferLength	   Size of the inpout buffer of the ioctl
*\param	    ULONG         IoControlCode		Code of the ioctl
*
*
* \~French
* \brief    Routine de contrôle du raw pdo
* \details  Routine de contrôle du raw pdo. \n
*			La fonction reçoit les ioctls transmises par le raw pdo lorsque
*			celui-ci reçoit des ioctls provenant d'une application cliente. \n
*			Elle traite les IOCTL comme la connexion avec le driver ou le démarrage 
*			d'une session de chiffrement.
*
*\param		WDFQUEUE      Queue		Un handle sur l'objet de file associé à l'ioctl
*\param		WDFREQUEST    Request	Un handle sur l'objet de la requête associée à l'ioctl
*\param		size_t        OutputBufferLength  Taille du buffer sortant de l'ioctl
*\param		size_t        InputBufferLength	  Taille du buffer entrant de l'ioctl
*\param	    ULONG         IoControlCode		Code de l'ioctl
*
*/
VOID  GostxBoard_EvtIoDeviceControlFromRawPdo(
    _In_ WDFQUEUE      Queue,
    _In_ WDFREQUEST    Request,
    _In_ size_t        OutputBufferLength,
    _In_ size_t        InputBufferLength,
    _In_ ULONG         IoControlCode
    )
{
	NTSTATUS			ntStatus			= STATUS_SUCCESS;
	PEPROCESS			pEprocess			= NULL;
	HANDLE				hProcessID			= NULL;
	PIRP				pIrp				= NULL;
	PIO_STACK_LOCATION	pioStackLoc			= NULL;
	UINT				uiDataWritten		= 0;

	UNREFERENCED_PARAMETER(Queue);
	UNREFERENCED_PARAMETER( InputBufferLength );
	UNREFERENCED_PARAMETER( OutputBufferLength );
	UNREFERENCED_PARAMETER( InputBufferLength );

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_IOCTLHandler::Entry \n");
	#endif

	//
	// Immediatly save ID and PE of the calling processus, in order to later
	// identify the application.
	//
	pEprocess	= IoGetCurrentProcess();
	hProcessID	= PsGetCurrentProcessId();

	//
	// Get the application and then the IRP stack
	//
	pIrp = WdfRequestWdmGetIrp( Request );
	pioStackLoc = IoGetCurrentIrpStackLocation( pIrp );

	switch(IoControlCode)
	{
		case IOCTL_INIT_KEY :
			//
			// Got an IOCTL from an application which wants to 
			// begin a cipher session
			// If an application already uses the cipher, then
			// we have to refuse the connection
			//
			if(  pCurrentApp->pEprocess != NULL )
			{
				ntStatus = HandleBusyCipher( 
					pioStackLoc,
					pIrp,
					&uiDataWritten);
			} else {
				ntStatus = HandleIoctlInitKey( 
					pioStackLoc, 
					pIrp,
					pEprocess,
					hProcessID,
					&uiDataWritten ); 
			}
			break;
		case IOCTL_CLOSE_KEY :
			//
			// Got an IOCTL from an application which wants to 
			// end a cipher session
			//
			ntStatus = HandleIoctlCloseKey(
				pioStackLoc,
				pIrp,
				&uiDataWritten );
			break;
		case IOCTL_TEST_CONNECTION :
			//
			// Got an IOCTL from an application which wants to 
			// test presence et smooth functionning of the driver
			//
			ntStatus = HandleIoctlTestConnection(
				pioStackLoc,
				pIrp,
				&uiDataWritten);
			break;
		default:
			#ifdef _DEBUG
				DbgPrint("[+] GostxBoard_IOCTLHandler::Unknown IOCTL. \n");
				DbgPrint("[+] GostxBoard_IOCTLHandler::IOCTL Code 0x%x [%u] \n", 
					pioStackLoc->Parameters.DeviceIoControl.IoControlCode, 
					pioStackLoc->Parameters.DeviceIoControl.IoControlCode );
				DbgPrint("[-] GostxBoard_IOCTLHandler::Ignoring previous IOCTL \n");
			#endif

			break;
	}

	//
	// Complete the request and clean things
	//
	WdfRequestCompleteWithInformation( Request, ntStatus, uiDataWritten );
	pEprocess = NULL;
	hProcessID = NULL;

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_IOCTLHandler:: %d bytes written \n", pIrp->IoStatus.Information );
	#endif

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_IOCTLHandler::Exit \n");
	#endif

	return;
};



#ifdef _DEPRECIATED
/**
* \~English
* \brief    [Depreciated] Service Callback I8042prt
* \details  Service Callback I8042prt \n
*			Called by the i8042prt port driver after it has processed the scancode entry.\n
*			This function is depreciated since there are better ways ton handle requests.
*
* \param	PDEVICE_OBJECT  DeviceObject	A Pointer to a device object
* \param	PKEYBOARD_INPUT_DATA InputDataStart A pointer to an input data keyboard structure
* \param	PKEYBOARD_INPUT_DATA InputDataEnd A pointer to an output data keyboard structure
* \param	PULONG InputDataConsumed	Size of data consumed
*
*
* \~French
* \brief    [Déprécié] Callback de service I8042prt
* \details  Callback de service I8042prt \n
*			La fonction est appelée par le driver de port i8042prt après qu'il a traité 
*			le scancode. \n La fonction est déprécié puisqu'il existe un moyen plus efficace de 
*			d'intercepter les requêtes.
*
* \param	PDEVICE_OBJECT  DeviceObject		Un pointeur vers l'objet périphérique
* \param	PKEYBOARD_INPUT_DATA InputDataStart Un pointeur vers une structure d'entrée du clavier
* \param	PKEYBOARD_INPUT_DATA InputDataEnd	Un pointeur vers une structure de sortie du clavier
* \param	PULONG InputDataConsumed			Taille des données traités
*
*/
VOID GostxBoard_ServiceCallback(
    _In_ PDEVICE_OBJECT  DeviceObject,
    _In_ PKEYBOARD_INPUT_DATA InputDataStart,
    _In_ PKEYBOARD_INPUT_DATA InputDataEnd,
    _In_ _Out_ PULONG InputDataConsumed
    )
{
	PDEVICE_EXTENSION   pDeviceExtension		= NULL;
	WDFDEVICE			wdfDevice				= NULL;

#ifdef _DEBUG
	DbgPrint("\n\n[+] GostxBoard_ServiceCallback:: Entry \n");
#endif

	//
	// Chack that IRQL is not to high
	PAGED_CODE();

	//
	// Get the handle of the wdf object 
	//
	wdfDevice = WdfWdmDeviceGetWdfDeviceHandle(DeviceObject);
	if (wdfDevice == NULL)
	{
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_ServiceCallback:: Error : Unable to retrieve DeviceObject Handle \n");
		#endif
	}

	//
	// Get the data  
	//
	pDeviceExtension = FilterGetData(wdfDevice);

	//
	// If it is a keystroke, then cipher it
	//
	//CipherScanCode(InputDataStart);
	
	
	(*(PSERVICE_CALLBACK_ROUTINE)(ULONG_PTR)pDeviceExtension->UpperConnectData.ClassService)(
		pDeviceExtension->UpperConnectData.ClassDeviceObject,
		InputDataStart,
		InputDataEnd,
		InputDataConsumed);

#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_ServiceCallback:: Exit \n");
#endif
	return;
};

#endif	// Depreciated


/**
* \~English
* \brief    IOCTL Init Session Handler
* \details  IOCTL Init Session Handler routine. \n
*			This functions is called when a cliente application wants to 
*			connect itself with the driver. It initiates a temporary cipher key, 
*			used to cipher scancodes, and then return the ioctl.
*
*\param		PIO_STACK_LOCATION	pioStackLoc		A pointer to the stack location
*\param 	PIRP		Irp		A pointer to the IRP
*\param		PEPROCESS	pEprocess	A pointer to the EProcess of the calling application
*\param		HANDLE		hProcess	A handle to the process of the calling application 
*\param		UINT*		uiDataWritten  Data written
*\return	NTSTATUS	Status of the operation
*
* \~French
* \brief    Traitement de l'IOCTL Init Session
* \details  Traitement de l'IOCTL Init Session \n
*			La fonction est appelée lorsqu'une application cliente souhaite se connecter
*			au driver. Elle initialise une clé de chiffrement temporaire, utilisée pour
*			chiffrer le scancode, puis retourne l'IOCTL complétée.
*
*\param		PIO_STACK_LOCATION	pioStackLoc		Un pointeur vers la pile io
*\param 	PIRP		Irp		Un pointer vers l'IRP
*\param		PEPROCESS	pEprocess	Un pointeur	vers le EProcess de l'application appelante
*\param		HANDLE		hProcess	Un pointeur vers le processus de l'application appelante
*\param		UINT*		uiDataWritten  Données écrites
*\return	NTSTATUS	Status de l'opération
*
*/
NTSTATUS	HandleIoctlInitKey( 
			_In_	PIO_STACK_LOCATION	pioStackLoc, 
			_In_	PIRP		Irp, 
			_In_	PEPROCESS	pEprocess,
			_In_	HANDLE		hProcess,
			_Out_	UINT*		uiDataWritten)
{
	NTSTATUS			ntStatus		= STATUS_SUCCESS;
	PPUBLIC_KEY_INIT	pkiPublic		,
						pkiReturn		= NULL;
#ifdef _DEPRECIATED
	char *				cToHash			= NULL;
	unsigned char 		ucHashKey[32]	= {0};
#endif // depreciated

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlInitKey::Entry\n");
	#endif

	//
	// Check that there are enought space in both input and output buffer 
	//
	if ( pioStackLoc->Parameters.DeviceIoControl.InputBufferLength != 
		sizeof( PUBLIC_KEY_INIT ) ||
		pioStackLoc->Parameters.DeviceIoControl.OutputBufferLength !=
		sizeof( PUBLIC_KEY_INIT ) )
	{
		Irp->IoStatus.Information	= 0;
		return STATUS_INVALID_BUFFER_SIZE;
	}

	//
	// Check that buffer is valid ( mem exists and alignment )
	//
	ASSERT(Irp->AssociatedIrp.SystemBuffer != NULL);

	try {
		ProbeForWrite(Irp->UserBuffer,
			sizeof(PUBLIC_KEY_INIT), 1 );
	} except(STATUS_ACCESS_VIOLATION) {

		//
		// Buffer is not valid
		//
		Irp->IoStatus.Information = 0;
		*uiDataWritten = 0;
		return STATUS_INVALID_DEVICE_REQUEST;
	}

	//
	// Get the buffer
	//
	pkiPublic = (PPUBLIC_KEY_INIT)Irp->AssociatedIrp.SystemBuffer;
	pkiReturn = (PPUBLIC_KEY_INIT)Irp->AssociatedIrp.SystemBuffer;


	#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_HandleIoctlInitKey::Key Received \n");
			DbgPrint("[+]-- KEY : %x \n", pkiPublic->cPublicKey );
		#endif

		//
		// Store the process ID and handle in our backup structure
		//
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_HandleIoctlInitKey::Saving App_Cred \n");
		#endif

		RtlSecureZeroMemory( pCurrentApp , 
			sizeof( pCurrentApp ) );
		RtlCopyMemory( &(pCurrentApp->guidApp), 
			&(pkiPublic->guidApp), 
			sizeof( GUID ));
		pCurrentApp->pEprocess = pEprocess;
		pCurrentApp->hProcessId = hProcess;

		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_HandleIoctlInitKey::PEprocess : %x \n", pCurrentApp->pEprocess);
		#endif
		//
		// Generate the temporary key
		//
		pCurrentApp->cPublicKey = GenerateKey();

		//
		// Allocate memory for hash calculus [unused]
		//
#ifdef _DEPRECIATED
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_HandleIoctlInitKey::Allocating memory for hash\n");
		#endif
		cToHash = (char *) ExAllocatePool( NonPagedPool, 
			sizeof( pkiPublic->cPublicKey ) + sizeof( GUID ) );
		
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_DeviceControleDispatch::Coupling Data \n");
		#endif

		//
		// Copy data to generate unique hash
		//
		RtlCopyMemory( cToHash, 
			pCurrentApp, 
			sizeof( pkiPublic->cPublicKey ) + sizeof( GUID ) ); 
			

		//
		// Generate hash
		//
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_DeviceControleDispatch::Generating Hash\n");
		#endif
		HashKey( cToHash, sizeof( cToHash ), ucHashKey);
			
		//
		// Free memory used for hash generation
		//
		ExFreePool( cToHash );

		//
		// Store hash
		//
		RtlCopyMemory( &(pCurrentApp->cHashPubKey), &ucHashKey, sizeof( ucHashKey ) );
#endif // depreciated
		
		//
		// File return structure
		//
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_DeviceControleDispatch::Filling pkiReturn \n");
		#endif
		pkiReturn->bCipherStarted = TRUE;

#ifdef _DEPRECIATED
		RtlCopyMemory( &(pkiReturn->cHashPubKey) , &(pCurrentApp->cHashPubKey) , sizeof( pCurrentApp->cHashPubKey ) - 1);
#endif // depreciated

		RtlCopyMemory( &(pkiReturn->guidApp), &(pCurrentApp->guidApp), sizeof( GUID ) );
		pkiReturn->cPublicKey = pCurrentApp->cPublicKey;
		pkiReturn->dwError = SESSION_STARTED;

		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_NewKEY : \n");
			DbgPrint("[+]	- PublicKey : %#016I64X \n", pkiReturn->cPublicKey);
			DbgPrint("[+]	- Hash : %s \n", pkiReturn->cHashPubKey);
			DbgPrint("[+]	- Error : %d \n", pkiReturn->dwError);
			DbgPrint("[+]	- Initialized : %d \n", pkiReturn->bCipherStarted);
		#endif

		//
		// Size of data written
		//
		*uiDataWritten = sizeof( PUBLIC_KEY_INIT );

		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_HandleIoctlInitKey::App Added \n");

			DbgPrint(" ___________________________________________\n");
			DbgPrint("|   App with PID %x has ask for a cipher  |\n", pCurrentApp->hProcessId);
			DbgPrint("| session and is now linked with the driver |\n");
			DbgPrint("|___________________________________________|\n\n\n");

			DbgPrint("[+] GostxBoard_HandleIoctlInitKey::Exit\n");
		#endif
		DbgPrint("\n\n----- New Cipher Session Started ! -----\n\n");

		//
		// Reset the cipher
		//
		StreamCipher( 0, 0);
	return ntStatus;
}




/**
* \~English
* \brief    IOCTL Close Session Handler
* \details  IOCTL Close Session Handler routine. \n
*			This functions is called when a cliente application wants to
*			close a session with the driver. It deletes all associated keys, 
*			stops the cipher session and return back the ioctl.
*
*\param		PIO_STACK_LOCATION	pioStackLoc		A pointer to the stack location
*\param 	PIRP		Irp		A pointer to the IRP
*\param		UINT*		uiDataWritten  Data written
*\return	NTSTATUS	Status of the operation
*
* \~French
* \brief    Traitement de l'IOCTL Close Session
* \details  Traitement de l'IOCTL Close Session \n
*			La fonction est appelée lorsqu'une application cliente souhaite se déconnecter
*			du driver. Elle  supprimes toutes les clés associées, arrête le chiffrement et 
*			retourne l'ioctl.
*
*\param		PIO_STACK_LOCATION	pioStackLoc		Un pointeur vers la pile io
*\param 	PIRP		Irp		Un pointer vers l'IRP
*\param		UINT*		uiDataWritten  Données écrites
*\return	NTSTATUS	Status de l'opération
*
*/
NTSTATUS	HandleIoctlCloseKey( 
			_In_	PIO_STACK_LOCATION	pioStackLoc, 
			_In_	PIRP	Irp, 
			_Out_	UINT*	uiDataWritten)
{
	NTSTATUS				ntStatus			= STATUS_SUCCESS;
	PPUBLIC_CLOSE_SESSION	pcsCloseSession		,
							pcsCloseReturn		= NULL;

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlCloseKey::Entry\n");
	#endif

		//
		// Check that there are enought space in both input and output buffer 
		//
		if( pioStackLoc->Parameters.DeviceIoControl.OutputBufferLength != 
			sizeof( PUBLIC_CLOSE_SESSION ) ||
			pioStackLoc->Parameters.DeviceIoControl.InputBufferLength != 
			sizeof( PUBLIC_CLOSE_SESSION ) )
		{
			#ifdef _DEBUG
				DbgPrint("[-] HandleIoctlCloseKey::Not Enought Space for Message\n");
			#endif
			return STATUS_NO_MEMORY;
		}

	ASSERT(Irp->AssociatedIrp.SystemBuffer != NULL);
	try {
		ProbeForWrite(Irp->UserBuffer,
			sizeof(PUBLIC_CLOSE_SESSION), 1);
	} except(STATUS_ACCESS_VIOLATION) {

		//
		// Buffer is not valid
		//
		Irp->IoStatus.Information = 0;
		*uiDataWritten = 0;
		return STATUS_INVALID_DEVICE_REQUEST;
	}

	//
	// Get the ioctl from buffer
	//
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlCloseKey::Getting IOCTL Buffer\n");
	#endif
	pcsCloseSession = (PPUBLIC_CLOSE_SESSION)Irp->AssociatedIrp.SystemBuffer;
	pcsCloseReturn	= (PPUBLIC_CLOSE_SESSION)Irp->AssociatedIrp.SystemBuffer;

	//
	// Check that buffer is not empty
	//
	if( pcsCloseSession == NULL )
	{
		#ifdef _DEBUG
			DbgPrint("[x] GostxBoard_HandleIoctlCloseKey::Error : Null Buffer\n");
		#endif
		return STATUS_INVALID_PARAMETER;
	}
	
	ResetCipher();

	pcsCloseReturn->bAppisDeleted = TRUE;
	pcsCloseReturn->dwError = SESSION_CLOSED;
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlCloseKey::CloseSessionAppDeleted : %d\n", 
			pcsCloseReturn->bAppisDeleted);
			DbgPrint(" ______________________________________\n");
			DbgPrint("| App has closed the cipehered session |\n", pCurrentApp->hProcessId);
			DbgPrint("|\tand is now free as the cipher\t|\n");
			DbgPrint("|______________________________________|\n\n\n");
	#endif
	DbgPrint("\n\n----- Cipher Session Closed ! -----\n\n");

	//
	// Size of data written
	//
		*uiDataWritten = sizeof( PUBLIC_CLOSE_SESSION );

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlCloseKey::Exit\n");
	#endif
	return ntStatus;
}




/**
* \~English
* \brief    IOCTL Test Connection Handler
* \details  IOCTL Test Connection Handler routine.\n
*			This functions is called when a cliente application wants to
*			test if the driver is available and is working well. The function test 
*			the cryptographic algorithms of the driver and then cipher the vector test given 
*			via the IOCTL. The application wil check itself if the cipher vector is well ciphered. 
*
*\param		PIO_STACK_LOCATION	pioStackLoc		A pointer to the stack location
*\param 	PIRP		Irp		A pointer to the IRP
*\param		UINT*		uiDataWritten  Data written
*\return	NTSTATUS	Status of the operation
*
* \~French
* \brief    Traitement de l'IOCTL Test Connection
* \details  Traitement de l'IOCTL Test Connection \n
*			La fonction est appelée lorsqu'une application cliente souhaite vérifier 
*			que le driver est disponible et fonctionne. La fonction teste les 
*			algorithmes de chiffrements puis vérifie chiffre le vecteur de test passé via 
*			IOCTL. L'application appelante vérifiera que le vecteur est chiffré proprement. 
*
*\param		PIO_STACK_LOCATION	pioStackLoc		Un pointeur vers la pile io
*\param 	PIRP		Irp		Un pointer vers l'IRP
*\param		UINT*		uiDataWritten  Données écrites
*\return	NTSTATUS	Status de l'opération
*
*/
NTSTATUS	HandleIoctlTestConnection(
	_In_	PIO_STACK_LOCATION	pioStackLoc,
	_In_	PIRP	Irp,
	_Out_	UINT*	uiDataWritten)
{
	NTSTATUS				ntStatus			= STATUS_SUCCESS;
	PPUBLIC_TEST_CONNECTION	ptcTestConnection	= NULL,
							ptcTestReturn		= NULL;
	DWORD					dwReturn			= RETURN_SUCCESS,
							dwIndex				= 0;

#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_HandleIoctlTest::Entry\n");
#endif

	//
	// Check that there are enought space in both input and output buffer 
	//
	if (pioStackLoc->Parameters.DeviceIoControl.OutputBufferLength !=
		sizeof(PUBLIC_TEST_CONNECTION) ||
		pioStackLoc->Parameters.DeviceIoControl.InputBufferLength !=
		sizeof(PUBLIC_TEST_CONNECTION))
	{
		#ifdef _DEBUG
			DbgPrint("[-] HandleIoctlTest::Not Enought Space for Message\n");
		#endif
		return STATUS_NO_MEMORY;
	}
 
	ASSERT(Irp->AssociatedIrp.SystemBuffer != NULL);
	try {
		ProbeForWrite(Irp->UserBuffer,
			sizeof(PUBLIC_CLOSE_SESSION), 1);
	} except(STATUS_ACCESS_VIOLATION) {

		//
		// Buffer is not valid
		//
		Irp->IoStatus.Information = 0;
		*uiDataWritten = 0;
		return STATUS_INVALID_DEVICE_REQUEST;
	}


	//
	// Get IOCTL from buffer
	//
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlTest::Getting IOCTL Buffer\n");
	#endif
	ptcTestConnection = (PPUBLIC_TEST_CONNECTION)Irp->AssociatedIrp.SystemBuffer;
	ptcTestReturn = (PPUBLIC_TEST_CONNECTION)Irp->AssociatedIrp.SystemBuffer;


	//
	// Check if valid
	//
	if (ptcTestConnection == NULL)
	{
		#ifdef _DEBUG
			DbgPrint("[x] GostxBoard_HandleIoctlTest::Error : Null Buffer\n");
		#endif
		return STATUS_INVALID_PARAMETER;
	}

	//
	// Run a self test on vital cryptographics functions
	//
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlTest::Encryption self test asked\n");
	#endif

	dwReturn = RunSelfTest();

	if (CODE_SUCCESS(dwReturn))
	{
		dwReturn = DRIVER_AVAILABLE;
	}

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlTest::Running dynamic vectors test... ");
	#endif

	//
	// Run the application test vector
	//
	// Initiate the temporary 64bits app key
	//
	pCurrentApp->cPublicKey = ptcTestConnection->TestKey;
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleIoctlTest::Test key %#016I64X\nVector :", pCurrentApp->cPublicKey );
	#endif
	//
	// Encrypt each scancode in the given vector
	//
	for (dwIndex = 0; dwIndex < TEST_VECTOR_SIZE; dwIndex++)
	{
		
		#ifdef _DEBUG
			DbgPrint("[%#x,", ptcTestReturn->TestVector[dwIndex].usPlain);
		#endif 
		bSilent = TRUE;
		CipherKey((PUCHAR)&(ptcTestReturn->TestVector[dwIndex].usPlain), NULL);
		bSilent = FALSE;
		#ifdef _DEBUG
			DbgPrint("%#x]::", ptcTestReturn->TestVector[dwIndex].usPlain);
		#endif
		
	}

	//
	// Reset the encryption system 
	//
	ResetCipher();
#ifdef _DEBUG
	DbgPrint("Ok.\n");
#endif

	//
	// Complete the ioctl 
	//
	ptcTestReturn->DriverAvailable = TRUE;
	ptcTestReturn->dwError = dwReturn;

	//
	// Size of data written
	//
	*uiDataWritten = sizeof(PUBLIC_TEST_CONNECTION);

#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_HandleIoctlTest::Exit\n");
#endif
	return ntStatus;
}




/**
* \~English
* \brief    IOCTL Busy Cipher Handler
* \details  IOCTL Busy Cipher Handler routine. \n
*			This functions is called when a cliente application wants to
*			begin a cipher session and the cipher is already taken. It only 
*			fill the IOCTL with the busy flag CIPHER_BUSY and refused the connection.
*
*\param		PIO_STACK_LOCATION	pioStackLoc		A pointer to the stack location
*\param 	PIRP		Irp		A pointer to the IRP
*\param		UINT*		uiDataWritten  Data written
*\return	NTSTATUS	Status of the operation
*
* \~French
* \brief    Traitement de l'IOCTL Busy Cipher
* \details  Traitement de l'IOCTL Busy CIpher \n
*			La fonction est appelée lorsqu'une application cliente souhaite démarrer
*			une session de chiffrement et que le drievr est déjà pris par une autre 
*			application. La fonction retourne simplement l'IOCTL avec le flag CIPHER_BUSY.
*
*\param		PIO_STACK_LOCATION	pioStackLoc		Un pointeur vers la pile io
*\param 	PIRP		Irp		Un pointer vers l'IRP
*\param		UINT*		uiDataWritten  Données écrites
*\return	NTSTATUS	Status de l'opération
*
*/
NTSTATUS	HandleBusyCipher( 
			_In_	PIO_STACK_LOCATION	pioStackLoc, 
			_In_	PIRP		Irp, 
			_Out_	UINT*		uiDataWritten)
{
	NTSTATUS				ntStatus		= STATUS_SUCCESS;
	PPUBLIC_KEY_INIT		pkiPublic		= NULL;

	UNREFERENCED_PARAMETER( pioStackLoc );

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleBusyCipher::Entry\n");
	#endif

	//
	// Check that there are enought space in both input and output buffer 
	//
	if (pioStackLoc->Parameters.DeviceIoControl.InputBufferLength !=
		sizeof(PUBLIC_KEY_INIT) ||
		pioStackLoc->Parameters.DeviceIoControl.OutputBufferLength !=
		sizeof(PUBLIC_KEY_INIT))
	{
		Irp->IoStatus.Information = 0;
		return STATUS_INVALID_BUFFER_SIZE;
	}

	//
	// Check that buffer is valid ( mem exists )
	//
	ASSERT(Irp->AssociatedIrp.SystemBuffer != NULL);

	//
	// Get the buffer from IRP
	//
	pkiPublic = (PPUBLIC_KEY_INIT)Irp->AssociatedIrp.SystemBuffer;

	//
	// Fill the return structure
	//
	pkiPublic->bCipherStarted = FALSE;
	pkiPublic->dwError = CIPHER_BUSY;

	//
	// Size of data written
	//
	*uiDataWritten = sizeof( PUBLIC_KEY_INIT );


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HandleBusyCipher:Exit\n");
	#endif
	return ntStatus;
}




/**
* \~English
* \brief    Reset the stream cipher
* \details  Reset the stream cipher and erase application credentials \n
*
*\param		void
*\return	void
*
* \~French
* \brief    Réinitialise la fonction de chiffrement
* \details  Réinitialise la fonction de chiffrement et écrase les identifiants de l'application.
*
*\param		void
*\return	void
*
*/
VOID	ResetCipher()
{
	
	//
	// Reset the stream cipher
	//
	StreamCipher( 0, 0 );

	//
	// Update the backup structure except the master key
	//
	RtlSecureZeroMemory(&(pCurrentApp->cHashPubKey), sizeof(pCurrentApp->cHashPubKey));
	RtlSecureZeroMemory(&(pCurrentApp->cPublicKey), sizeof(pCurrentApp->cPublicKey));
	RtlSecureZeroMemory(&(pCurrentApp->guidApp), sizeof(pCurrentApp->guidApp));
	RtlSecureZeroMemory(&(pCurrentApp->hProcessId), sizeof(pCurrentApp->hProcessId));
	RtlSecureZeroMemory(&(pCurrentApp->pEprocess), sizeof(pCurrentApp->pEprocess));
	RtlSecureZeroMemory(&(pCurrentApp->Unused), sizeof(pCurrentApp->Unused));
	pCurrentApp->pEprocess = NULL;

	//
	// Reset flag state
	//
	dwCurrentState = 0;
}



/**
* \~English
* \brief    Hook of i8042prt ISR
* \details  Hook of the isr of the i8042prt port driver\n
*			This functions is called at the begining of the process of 
*			a keyboard interrupt. At this level of hook, there are no one below 
*			us, ensuring that we are one of the first to receive requets.\\
*
*\param		PVOID     IsrContext The contexte of the ISR
*\param		PKEYBOARD_INPUT_DATA   pCurrentInput	A pointer to an keyboard input data structure provided by i8042prt
*\param		POUTPUT_PACKET         CurrentOutput	A pointer to an outout packet data structure provided by i8042prt
*\param		UCHAR                  StatusByte		The status associated with the interrupt
*\param		PUCHAR                 DataByte			A pointer to an uchar provided by i8042prt. It contains the scancode.
*\param		PBOOLEAN               ContinueProcessing	A pointer to a boolean. If false, i8042prt will not process the request
*\param		PKEYBOARD_SCAN_STATE   ScanState		A pointer to a structure representing the state
*\return	BOOLEAN					Status of this operation
*
* \~French
* \brief    Hook de l'ISR du driver de port i8042prt
* \details  Hook de l'ISR du driver de port i8042prt \n
*			Cette routine est appelée au début du processus 
*			de gestion d'une interruption clavier. A ce niveau 
*			là, il n'y a pas plus personnes en dessous de nous, 
*			on est donc assuré qu'on est les premiers à recevoir 
*			les IRPs. 
*
*\param		PVOID     IsrContext The contexte of the ISR
*\param		PKEYBOARD_INPUT_DATA   pCurrentInput	Un pointeur vers une structure clavier d'entrée, fournis par i8042prt
*\param		POUTPUT_PACKET         CurrentOutput	Un pointeur vers une structure clavier de sortie, fournis par i8042prt
*\param		UCHAR                  StatusByte		Status associé à l'interruption
*\param		PUCHAR                 DataByte			Un pointeur vers un uchar, fournis par i8042prt. Il contient le scancode.
*\param		PBOOLEAN               ContinueProcessing	Un pointeur vers un booléen. Si FALSE, i8042prt ne traitera pas la requête.
*\param		PKEYBOARD_SCAN_STATE   ScanState		Un pointeur vers une structure représentant l'etat
*
*/
BOOLEAN GostxBoard_IsrHook(
	PVOID                  IsrContext,
	PKEYBOARD_INPUT_DATA   pCurrentInput,
	POUTPUT_PACKET         CurrentOutput,
	UCHAR                  StatusByte,
	PUCHAR                 DataByte,
	PBOOLEAN               ContinueProcessing,
	PKEYBOARD_SCAN_STATE   ScanState
	)
{
	BOOLEAN						bReturn					= TRUE;
	PDEVICE_EXTENSION			pDeviceExtension		= NULL;
	void *						ptr						= NULL;
	PPORT_KEYBOARD_EXTENSION	pPortKeyboardExension	= NULL;
	size_t						offset					= 0;

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_IsrHook[i8042]::Entry\n");
	#endif
	//
	// Get the context of the ISR
	//
	pDeviceExtension = (PDEVICE_EXTENSION)IsrContext;

	//
	// Try to cipher the received scan code. The decision of ciphering is made 
	// internally in the function CipherScanCode
	//
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_IsrHook[i8042]::Received : 0x%x\n", *DataByte);
	#endif

	CipherScanCode(pCurrentInput, DataByte);	  	
	pCurrentInput->MakeCode = GET_MAKE_CODE(*DataByte);

	if (pDeviceExtension->UpperIsrHook) {
		bReturn = (*pDeviceExtension->UpperIsrHook) (
			pDeviceExtension->UpperContext,
			pCurrentInput,
			CurrentOutput,
			StatusByte,
			DataByte,
			ContinueProcessing,
			ScanState
			);

		if (!bReturn || !(*ContinueProcessing)) {
			return bReturn;
		}
	}

	//
	// We try to modificate the backup internally made by i8042prt.
	// So, as we approximatively know the structure of the pool associated
	// with some given parameters, we try to reconstructe the structures and then, 
	// modificate the elements we are looking for
	//
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_IsrHook[i8042]::Hook internal i8042prt\n");
	#endif
	offset = offsetof(struct _PORT_KEYBOARD_EXTENSION, CurrentInput);
	ptr = (void *)(pCurrentInput - offset );
	ptr = (char *)ptr + 0x1600;
	pPortKeyboardExension = (PPORT_KEYBOARD_EXTENSION)ptr;
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_IsrHook[i8042]::Hook : CurrentScanCode 0x%x -> 0x%x\n", &(pPortKeyboardExension->CurrentScanCode), pPortKeyboardExension->CurrentScanCode);
		DbgPrint("[+] GostxBoard_IsrHook[i8042]::Hook : LastScanCode 0x%x -> 0x%x \n", &(pPortKeyboardExension->LastScanCode), pPortKeyboardExension->LastScanCode);
	#endif
	RtlCopyMemory(&(pPortKeyboardExension->LastScanCode), DataByte, sizeof(UCHAR));
	RtlCopyMemory(&(pPortKeyboardExension->CurrentScanCode), DataByte, sizeof(UCHAR));

	//
	// We tell i8042prt to process the request
	//
	*ContinueProcessing = TRUE;

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_IsrHook[i8042]::Exit : 0x%x : 0x%x : 0x%x\n\n", pCurrentInput->MakeCode, *DataByte, uMemory);
	#endif
	return bReturn;
}


#ifdef _DEPRECIATED
/**
* \~English
* \brief    [Depreciated] Handle keyboard interrupt
* \details   [Depreciated] Handle keyboard interrupt\n
*
*\param		struct _KINTERRUPT *Interrupt	A pointer to an interrupt structure
*\param		PVOID  ServiceContext			A pointer to the service context associated with the interrupt
*\return	BOOLEAN					Status of this operation
*
* \~French
* \brief    [Depreciated] Traite les interruptions clavier
* \details  [Depreciated] Traite les interruptions clavier\n
*
*\param		struct _KINTERRUPT *Interrupt	Un pointeur vers une structure d'interruption
*\param		PVOID  ServiceContext			Un pointeur vers le contexte du service associé à l'intérruption
*\return	BOOLEAN					Status de l'opération
*/
BOOLEAN InterruptKeyboard(
	_In_  struct _KINTERRUPT *Interrupt,
	_In_  PVOID              ServiceContext
	)
{
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_Interrupt[ACPI]::Received !\n");	
	#endif

		UNREFERENCED_PARAMETER(Interrupt);
		UNREFERENCED_PARAMETER(ServiceContext);

	return TRUE;
}
#endif // depreciated
