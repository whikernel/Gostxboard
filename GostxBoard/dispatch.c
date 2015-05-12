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
* \file      dispatch.c
* \author	 Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     [Depreciated] Defines dispatch functions
*
* \details   Defines dispatch functions, like 
*			 EvtIoRead which process IRP_MJ_READ. Theses functions are depreciated
*			 since the driver handle the request in better ways. This file is only 
*			 for educationnal purpose.
*
* \~French
* \file		dispatch.c
* \author	 Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	[Déprécié] Définit les fonctions de dispatch.
*
* \details	Définit les fonctions de dispatch, comme EvtIoRead
*			qui traite les IRPs IRP_MJ_READ. Les fonctions de ce fichiers sont 
*			dépréciés, le driver interceptant les requêtes de manière plus efficaces
*			dans les versions actuelles. Ce fichier est présent seulement dans un 
*			but educatif.
*/


#include "dispatch.h"
#include "../Commons/crypto.h"
#ifdef _DEPRECIATED




/**
* \~English
* \brief    [Depreciated] Read request callback	
* \details  The function handles and processes the read request	sent by upper 
*			drivers. \nSince there are not yet interesting data in the request 
*			, the function only register a read completion callback in order 
*			to be notified when the request is send back. \n
*			This function is depreciated since the driver handle the request far more
*			earlier.
*
* \param	WDFQUEUE	Queue		A handle to the queue object associated with the request
* \param	WDFREQUEST	Request		A handle to the request object
* \param    size_t		Length		The number of bytes to be read
* \return   void
*
* \~French
* \brief    [Déprécié] Callback des requêtes read
* \details  La fonction intercépte et traite les requêtes de type read envoyées
*			par les driver supérieurs.\n La fonction enregistre seulement une routine 
*			de completion puisqu'il y n'y a pas encore de données interessantes dans la
*			requête. \n
*			La fonction est dépréciée car dans les versions actuelles le driver intércepte les
*			requêtes de manière plus efficace.
*
* \param	WDFQUEUE	Queue		Un handle sur l'objet file associée à la requête
* \param	WDFREQUEST	Request		Un handle sur l'objet requête
* \param    size_t		Length		Le nombre d'octets devant être lues
* \return   void
*
*/
VOID GostxBoard_EvtIoRead(
		_In_  WDFQUEUE Queue,
		_In_  WDFREQUEST Request,
		_In_  size_t Length
)
 {
	NTSTATUS					ntStatus				= STATUS_SUCCESS;
	WDFDEVICE					wdfDevice				= NULL;
	WDFIOTARGET					wdfTarget				= NULL;


	UNREFERENCED_PARAMETER( Length );
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_EvtIoRead::Entry\n");
	#endif

	//
	// Get the handle of the device that the queue belongs to 
	//
	 wdfDevice = WdfIoQueueGetDevice(Queue);

	 //
	 // Formate the request for next driver
	 //
	 WdfRequestFormatRequestUsingCurrentType(Request);


	 //
	 // Register the callback routine to be notify when the request 
	 // will be send back, completed with the scancode
	 //
	#ifdef _DEBUG
		 DbgPrint("[+] GostxBoard_EvtIoRead::Registering completion routine\n");
	#endif
	WdfRequestSetCompletionRoutine( Request, 
		GostxBoard_CompletionReadRoutine, 
		NULL );

	//
	// Get the target device and then send the request to it
	//
	wdfTarget =  WdfDeviceGetIoTarget(wdfDevice);
	if (!WdfRequestSend(Request, wdfTarget, WDF_NO_SEND_OPTIONS) )
	{

		ntStatus = WdfRequestGetStatus(Request);		
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_EvtIoRead::Exit with status 0x%x \n");
		#endif
		WdfRequestCompleteWithInformation(Request, ntStatus, 0);
		return;
	}
	else
	{
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_EvtIoRead::Exit\n\n");
		#endif
		return;
	}
 }




/**
* \~English
* \brief    [Depreciate] Read completion callback
* \details  The function is called when a request travel back the driver stack.	\n
*			This means that the IRP contains the scancode, so we can process it. \n
*			This function is depreciated since the driver handle the request far more 
*			earlier. 
*
*\param		WDFREQUEST Request	A handle on the request to process
*\param		WDFIOTARGET Target	A handle to the target to send the request to
*\param		PWDF_REQUEST_COMPLETION_PARAMS Params	Parameters of the completion request 
*\param		WDFCONTEXT Context	Context of the completion
*\return	void
*
* \~French
* \brief    [Déprécié] Callback de complétion
* \details  La fonction est appelée quand une requête remonte le long de la pile des drivers. \n
*			Cela veut dire que l'IRP contient le scan-code, que l'on peut traiter. \n
*			La fonction est dépréciée car dans les versions actuelles le driver intércepte les 
*			requêtes de manière plus efficace. 
*
*\param		WDFREQUEST Request	Un handle sur la requête à traiter
*\param		WDFIOTARGET Target	Un handle sur la cible à laquelle envoyer la re^quête
*\param		PWDF_REQUEST_COMPLETION_PARAMS Params	Paramètres de la complétion
*\param		WDFCONTEXT Context	Contexte de la complétion
*
*/
VOID GostxBoard_CompletionReadRoutine (
	_In_  WDFREQUEST Request,
	_In_  WDFIOTARGET Target,
	_In_  PWDF_REQUEST_COMPLETION_PARAMS Params,
	_In_  WDFCONTEXT Context
	)
{
	NTSTATUS				ntStatus				= STATUS_SUCCESS;
	PIRP					Irp						= NULL;


	UNREFERENCED_PARAMETER( Params );
	UNREFERENCED_PARAMETER( Context );
	UNREFERENCED_PARAMETER( Target );

	#ifdef _DEBUG
	DbgPrint("[+] GostxBoard_CompletionReadRoutine::Entry \n");
	#endif

	//
	// Get the irp from the request
	//
	Irp = WdfRequestWdmGetIrp( Request );	


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_CompletionReadRoutine%d::Completing request");
	#endif
	
	//
	// Forward the request to the driver above
	//
	WdfRequestFormatRequestUsingCurrentType(Request);
	if (!WdfRequestSend(Request, Target, WDF_NO_SEND_OPTIONS))
	{
		#ifdef _DEBUG
			DbgPrint("... No device above. Completing directly \n");
		#endif
		ntStatus = WdfRequestGetStatus(Request);
		WdfRequestComplete(Request, Params->IoStatus.Status);
	}


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_CompletionReadRoutine%d::Exit\n");
	#endif
			

	return;
}

#endif // depreciated






