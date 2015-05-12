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
* \file      tools.cpp
* \author	 Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief    Defines some tools functions for the interface
*
* \details  Defines some tools functions for the interface, string ones.
*
* \~French
* \file		tools.cpp
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit des fonctions utilitaires pour l'interface
*
* \details	Définit des fonctions utilitaires pour l'interface
*/



#include "stdafx.h"
#include <string.h>
#include "tools.h"

/**
*\~English
*\brief		Get last char of a string
*\details	The function returns the last char of a string
*
*\~French
*\brief		Récupère le dernier char d'une chaine
*\details	La fonction retourne le dernier char d'une chaine de caractère. 
*/
char GetLastChar(char * cMessage, int dwSize)
{
	int  dwIndex = 0;

	//
	// For the length of the chain, or, while the end symbols is encountered
	//
	while ((dwIndex <= dwSize) || (cMessage[dwIndex] != '\0'))
	{
		dwIndex++;
	}
	//
	// Return the last char 
	//
	return cMessage[dwIndex - 1];
}



/**
*\~English
*\brief		Add a char to a string
*\details	The functions adds a char to a string. \n Developer has to care about the length 
*			of the string to avoid memory corruption
*
*\~French
*\brief		Ajoute un char à une chaine de charactères
*\details	La fonction ajoute un char à une chaine de charactères.\n Le développeur doit faire attention à la longueur 
*			de la chaine pour éviter toute corruption mémoire. 
*/
int AppendChar(char * String, char Char)
{
	int		dwLength = 0;

	dwLength = strlen(String);
	String[dwLength] = Char;
	String[dwLength + 1] = '\0';

	return 0;
}