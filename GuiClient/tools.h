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