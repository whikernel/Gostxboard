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

#ifndef _MAINDIALOG_H
#define _MAIN_DIAOG_

#include "stdafx.h"
#include <intrin.h>


#define CALL_STD	0x0
#define CALL_WARN 	0x1
#define CALL_ERR	0x2
#define CALL_CRIT	0x3
#define CALL_STOP	0x4
#define CALL_START	0x5

#define ERR_COLOR	0x1
#define WARN_COLOR	0x2
#define STD_COLOR	0x0


BOOL CALLBACK MainDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void UpdateProgressBar(int dwValue, char * Message, int color );
void Start();
void UpdateState(char * Message);
void UpdateStateW(LPCSTR Message);
char CipherChar(char cChar);
void UpdateDecipheredText(USHORT cChar);




#endif 