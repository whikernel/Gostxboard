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
* \file      controlfunctions.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Header of controlfunctions.c
*
* \~French
* \file		controlfunctions.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Header de controlfunctions.c
*
*/

#ifndef _CONTROL_FUNCTION_H
#define _CONTROL_FUNCTION_H

#include "dispatch.h"
#include "defines.h"

#ifdef _DEPRECIATED
KSERVICE_ROUTINE InterruptKeyboard;
#endif //depreciated

VOID  GostxBoard_EvtIoInternalDeviceControl(
    _In_ WDFQUEUE      Queue,
    _In_ WDFREQUEST    Request,
    _In_ size_t        OutputBufferLength,
    _In_ size_t        InputBufferLength,
    _In_ ULONG         IoControlCode
    );

VOID  GostxBoard_EvtIoDeviceControlFromRawPdo(
    _In_ WDFQUEUE      Queue,
    _In_ WDFREQUEST    Request,
    _In_ size_t        OutputBufferLength,
    _In_ size_t        InputBufferLength,
    _In_ ULONG         IoControlCode
    );

VOID  GostxBoard_EvtIoDeviceControlForRawPdo(
    _In_ WDFQUEUE      Queue,
    _In_ WDFREQUEST    Request,
    _In_ size_t        OutputBufferLength,
    _In_ size_t        InputBufferLength,
    _In_ ULONG         IoControlCode
    );

#ifdef _DEPRECIATED
VOID GostxBoard_ServiceCallback(
    _In_ PDEVICE_OBJECT  DeviceObject,
    _In_ PKEYBOARD_INPUT_DATA InputDataStart,
    _In_ PKEYBOARD_INPUT_DATA InputDataEnd,
    _In_ _Out_ PULONG InputDataConsumed
    );
#endif


NTSTATUS GostxBoard_InitializationRoutine(
	_In_ PVOID                           InitializationContext,
	_In_ PVOID                           SynchFuncContext,
	_In_ PI8042_SYNCH_READ_PORT          ReadPort,
	_In_ PI8042_SYNCH_WRITE_PORT         WritePort,
	_Out_ PBOOLEAN                       TurnTranslationOn
	);


BOOLEAN GostxBoard_IsrHook(
	PVOID                  IsrContext,
	PKEYBOARD_INPUT_DATA   CurrentInput,
	POUTPUT_PACKET         CurrentOutput,
	UCHAR                  StatusByte,
	PUCHAR                 DataByte,
	PBOOLEAN               ContinueProcessing,
	PKEYBOARD_SCAN_STATE   ScanState
	);


NTSTATUS	HandleIoctlInitKey( 
			_In_	PIO_STACK_LOCATION	pioStackLoc, 
			_In_	PIRP		Irp, 
			_In_	PEPROCESS	pEprocess,
			_In_	HANDLE		hProcessId,
			_Out_	UINT*		uiDataWritten);


NTSTATUS	HandleBusyCipher( 
			_In_	PIO_STACK_LOCATION	pioStackLoc, 
			_In_	PIRP		Irp, 
			_Out_	UINT*		uiDataWritten);


NTSTATUS	HandleIoctlCloseKey( 
			_In_	PIO_STACK_LOCATION	pioStackLoc, 
			_In_	PIRP	Irp, 
			_Out_	UINT*	uiDataWritten);


//***********************************************
// But : Fonction de gestion d'un IOCTL Test Connection.
//		Permet à une application de tester sa connexion avec 
//		le driver. 
//
//-[_In_] - PIO_STACK_LOCATION
//-[_In_] - PIRP
//-[_Out_] - NTSTATUS
//****
NTSTATUS	HandleIoctlTestConnection(
	_In_	PIO_STACK_LOCATION	pioStackLoc,
	_In_	PIRP	Irp,
	_Out_	UINT*	uiDataWritten);

//***********************************************
// But : Permet d'arreter le chiffrement et de 
//		supprimer l'application courante du 
//		du contexte. 
//
//****
VOID	ResetCipher();





#endif