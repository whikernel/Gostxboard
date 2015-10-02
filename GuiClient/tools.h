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

#ifndef _TOOLS_H
#define _TOOLS_H

//***********************************************
// But : Permet de récupérer le dernier charactère
//	d'une chaine. La chaine doit contenir le caractère 
//	de terminaison '\0'. Le cas échéant le dwSize ième - 1
//	charactère est renvoyé.
//
//- char
//****
char GetLastChar(char * cMessage, int dwSize);


//***********************************************
// But : Ajout un caractère à une chaine. Aucune 
//	vérification n'est faite sur la taille de la chaine.
//	L'utilisateur doit s'assurer que la chaine peut contenir
//	ce caractère de plus.  
//
//- INT 
//****
int AppendChar(char * String, char Char);

#endif // tools.h