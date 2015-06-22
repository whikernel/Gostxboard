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
* \file      functions.h
* \author    GostCrypt Foundation - P.A
* \version   1.0
* \date      May 21, 2015
* \brief     Header of functions.c
*
* \~French
* \file		functions.h
* \author	GostCrypt Foundation - P.A
* \version  1.0
* \date		21 Mai 2015
* \brief	Header de functions.c
*
*/

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "driver.h"
#include "../Commons/defineKbFr.h"

//VOID HashKey( char * cKey, int keyLength, unsigned char usResult[32] );
VOID ProcessNotifyRoutine ( HANDLE parentId, HANDLE processId,BOOLEAN create );
DWORD CheckDriverSecurityContext(void);
NTSTATUS GostxBoard_CheckRegistry(BOOL * UpdateNeeded);
NTSTATUS GostxBoard_QueryValueKey(HANDLE RegKeyHandle, PWSTR SubKeyName, PWSTR ValueName, PVOID RegValuePtr);
NTSTATUS GostxBoard_ReadRegistryKey(PUNICODE_STRING keyPath, wchar_t *keyValueName, PKEY_VALUE_PARTIAL_INFORMATION *keyData);
NTSTATUS GostxBoard_WriteRegistryKey(PUNICODE_STRING keyPath, wchar_t *keyValueName, ULONG keyValueType, void *valueData, ULONG valueSize);

#endif