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
* \file      GuiClient.cpp
* \author	 Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Entry file of the graphical interface
*
* \details  The file contains only the main function of the interface. 
*
* \~French
* \file		GuiClient.cpp
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Fichier d'entrée de l'interface graphique
*
* \details	Le fichier contient seulement la fonction main de l'interface. 
*/

#include "stdafx.h"
#include "MainDialog.h"
#include "GuiClient.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;								// instance actuelle
TCHAR szTitle[MAX_LOADSTRING];					// Le texte de la barre de titre
TCHAR szWindowClass[MAX_LOADSTRING];			// le nom de la classe de fenêtre principale


/**
*\~English
*\brief		Define the main entry the window
*\details	The function is called when the executable is launched. It only creates a DialogBox 
*			which represents the window. 
*
*\~French
*\brief		Définit l'entrée main de la fenêtre
*\details	La fonction est appelée lorsque l'exéctuable est lancé. Elle créée simplement une 
*			dialogbox qui représente la fenêtre. 
*/
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	hInst = hInstance;

	/* Create main dialog box */
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), NULL, MainDialogProc);

	return 0;
}





