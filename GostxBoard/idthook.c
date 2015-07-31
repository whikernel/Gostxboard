//**********************************************************************//
// 					This file is part of GostxBoard						//
//																		//
//  GostxBoard is free software: you can redistribute it and/or modify	//
//       it under the terms of the Creative Commons [Attribution-		//	
//	NonCommercial-ShareAlike 4.0 International] License as published	// 
//				 by the Ditribution of Creative Commons 				//		
//                Such things are STRONGLY recommended.					//
//    																	//
// 	GostxBoard is distributed in the hope that it will be useful,		//
//  but WITHOUT ANY WARRANTY; without even the implied warranty of		//	
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		//
//  	Creative Commons [BY-NC-SA] License for more details.			//	
//  Do not blame the developper if something goes wrong. You can 		//	
//  either search on the Internet or email the developper to deal 		//
//                             with it.									//
//																		//
//  You should have received a copy of the Creative Commons [BY-NC-SA]	// 
//	             License along with GostxBoard. If not, see 			//
//	  <https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode>. 	//
//																		//
//                              P.A 									//
//                gostxboard_support@whitekernel.fr 					//
//**********************************************************************//


/**
* \~English
* \file      idthook.c
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     [Depreciated] Defines functions used to hook the IDT. 
*
* \details   Defines functions used to hook the IDT. \n These functions are depreciated since the hook
*			of the IDT is not a viable way of getting requests. This file is left in the project only 
*			for an educational purpose.
*			
*
* \~French
* \file		idthook.c
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	[Déprécié] Définit des fonctions utilisée pour le hook de l'IDT
*
* \details	Définit des fonctions utilisées pour le hook de l'IDT. \n Ces fonctions sont dépréciés puisque le 
*			hook de l'IDT n'est pas une solution viable pour récupérer les requêtes. Le fichier est laissé dans le
*			projet dans un but purement educatif. 
*/


#include "idthook.h"
#include "dispatch.h"
#include <windowsx.h>
#include <intrin.h>

#include "defines.h"

#pragma intrinsic (_disable, _enable)

void (*i8042Isr)() = NULL;

UINT32 uOldIsrAddr = 0;


/**
* \~English
* \brief    [Depreciated] Get descriptor address   
* \details	This function is specific to the hook of the IDT. It retrieves the adress
*			of the descriptor of a specified service in the IDT. \n 
*
* \param    UINT16 Service 	Identifier of the service to get the adress to
* \return	PDESC A pointer to a descriptor structure of the service #DESC.
*
* \~French
* \brief    [Déprécié] Récupère l'adresse du descripteur du service spécifié dans l'IDT
* \details  Cette fonction est spécifique au hook de l'IDT. Elle récupère l'adresse du 
*			descripteur lié à un service dans l'IDT.
*
* \param   UINT16 Service Identifiant du service à hooker
* \return  PDESC Un pointeur vers une structure descripteur  #DESC du service
*
*/
PDESC	 GetDescriptorAddr(
	_In_	UINT16		Service
	)
{
	IDTR	idtaddr			= { 0 };
	PDESC	pDescriptor		= { 0 };

	//
	// Try to get the IDTR address. 
	// Clean the interruption flag with cli instruction to avoid 
	//	access issues. 
	// Store the value of IDTR with sidt instruction 
	// Activate the interruption flag 
	//

	_disable();		// equal :  __asm { cli } -> Unable interruptions
	__sidt(&idtaddr);
	_enable();		// equal :  __asm { sti } -> Enable interruptions
	/*Equal  : _asm {
		cli;
		sidt idtaddr;
		sti;
	}*/

	//								   
	// Entries in interruption table are sized  8b, 
	// so, get the address of the service by com^puting :
	// IDTRAddr + 8* Service
	//
	pDescriptor = (PDESC)(idtaddr.addr + 8 * Service);
	
	return pDescriptor;
}


/**
* \~English
* \brief    [Depreciated] Get specified ISR address
* \details	This function is specific to the hook of the IDT. It retrieves the adress
*			of the specified ISR . 
*
* \param    UINT16 Service 	Identifier of the service to get the adress to
* \return	UINT32 Adress of the specified ISR.
*
* \~French
* \brief    [Déprécié] Récupère l'adresse de l'ISR spécifié
* \details  Cette fonction est spécifique au hook de l'IDT. Elle récupère l'adresse de 
*			de l'ISR spécifié en paramètre.
*
* \param   UINT16 Service Identifiant du service à hooker
* \return  UINT32	Adresse de l'ISR spécifiée
*
*/
UINT32	GetISRAddr( 
	_In_	UINT16		Service
	) 
{
	UINT32		uIsrAddr			= 0;
	PDESC		pDescriptorAddr		= NULL;

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_GetIsrAddr[IDT]::Entry\n");
	#endif

	pDescriptorAddr = GetDescriptorAddr(Service);

	//
	// Compute the ISR address from descriptor
	// A part is contained in the offset16 field, and another 
	// in the offset0 field.
	//
	uIsrAddr = pDescriptorAddr->offset16;
	uIsrAddr <<= uIsrAddr;
	uIsrAddr += pDescriptorAddr->offset00;

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_GetIsrAddr[IDT]::Isr is %#x\n", uIsrAddr);
	#endif

	uOldIsrAddr = uIsrAddr;
	
	return uIsrAddr;
}



//***********************************************
// But :	-- Code spécifique au hook de la table 
//			-- d'interruption
//		Fonction permettant de hooker et de réécrire 
//		une adresse spécifique de l'idt
//****
/**
* \~English
* \brief    [Depreciated] Hook the IDT
* \details	This function is specific to the hook of the IDT. It hooks the IDT 
*			and rewrite the ISR address of the specified service.
*
* \param    UINT16 Service 	Identifier of the service to get the adress to
* \param	UINT32 Adress of the specified ISR.
* \return	void
*
* \~French
* \brief    [Déprécié] Hook la table IDT
* \details  Cette fonction est spécifique au hook de l'IDT. Elle hook la table 
*			IDT et réécrit l'adresse ISR du service spécifié.
*
* \param   UINT16	Service Identifiant du service à hooker
* \param  UINT32	Adresse de l'ISR spécifiée
* \return	void
*
*/
void HookIsr(
	_In_	UINT16	Service,
	_In_	UINT32	HookAddr
	)
{
	UINT32		uIsrAddr			= 0;
	UINT16		uLow_Addr			= 0;
	UINT16		uHigh_Addr			= 0;
	PDESC		pDescriptorAddr		= NULL;

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HookIsr[IDT]::Entry \n");
	#endif

	//
	// Check if not already hooked
	//
	uIsrAddr = GetISRAddr(Service);
	if (uIsrAddr == HookAddr)
	{
		// Service is already hooked
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_HookIsr[IDT]::Service already hooked \n");
		#endif
	}
	else
	{
		#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HookIsr[IDT]::Hooking interrupt %#x with isr %#x to %#x \n", Service, uIsrAddr, HookAddr);
		#endif
		//
		// Get the ISR address and rewrite it 
		//
		pDescriptorAddr = GetDescriptorAddr(Service);
		uLow_Addr = (UINT16)HookAddr;
		HookAddr >>= 16;
		uHigh_Addr = (UINT16)HookAddr;
		_disable();		
		pDescriptorAddr->offset00 = uLow_Addr;
		pDescriptorAddr->offset16 = uHigh_Addr;
		_enable();	
		#ifdef _DEBUG
			DbgPrint("[+] GostxBoard_HookIsr[IDT]::IDT is successfully corrupted \n");
		#endif
	}

	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HookIsr[IDT]::Exit \n");
	#endif
	return;
}

#pragma warning(disable : 4740)
/*
__declspec(naked) void hookroutine()
{		
    #ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HookRoutine[IDT]::From IDT::Received \n");
	#endif
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_HookRoutine[IDT]::Giving Hand to i8042prt \n");
	#endif
	KeLowerIrql(PASSIVE_LEVEL);

   */

/*#ifdef _M_IX86
	i8042Isr =  (void (__stdcall*)()) uOldIsrAddr;
#else
	i8042Isr = (void (__cdecl*)())uOldIsrAddr;
#endif*/
	// Equivaut à :
/*	__asm {
		pushad;
		pushfd;

		push eax;
		call DbgPrintHook;

		popfd;
		popad;

		jmp uOldIsrAddr;
	} 

}
#pragma warning (default : 4740)	*/


void DbgPrintHook( UINT32 d ) {
  DbgPrint("[*] Inside Hook Routine - dispatch called %d", d);
}