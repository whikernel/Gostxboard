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
//                                Paul Amicelli 									//
//                    driver_support@whitekernel.fr 					//
//**********************************************************************//

/**
* \~English
* \file      definesKbFr.h
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Defines scancodes for keyboards. 
*
* \details   Defines scancodes for keyboards. It contains the whitelist
*			 used to determine if scancode must be cipher or not. \n Most 
*			 of keyboards manufacturer follows following tables. Nevertheless,
*			 it is possible that some of theses scancode are not the same for 
*			 every keyboards. In that case, it is possible to add a whitelist for 
*			 a specific keyboard and calls it when detected. 
*
* \~French
* \file		defines_commons.h
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit les scancodes des les claviers
*
* \details Définit les scancodes pour les claviers. Il contient la liste blanche
*			utilisée pour déterminer si un scancode doit-être chiffré ou non.\n 
*			La plupart des constructeurs de claviers suivent les tables ci-dessous. 
*			Néanmoins, il est possible que certains de ces scancodes ne soient pas les 
*			même pour certains claviers. Dans ce cas, il est possible d'ajouter une liste
*			blanche pour un clavier en particulier, et l'appeler quand celui-ci est détecté. 
*/
#ifndef _DEFINE_KB_FR_H
#define	_DEFINE_KB_FR_H

#include "defines_common.h"


/**		   
*\~English  
*\brief		Storage structure of scancode and identifier.
*\details	This structure is used to structure the whitelist. \n
*			Each entry represente a key that needs to be ciphered. 
*
*\~French
*\brief		Structure de stockage d'un scancode et de son identificateur
*			humainement compréhensible.
*\details	Cette structure est utilisée pour construire la liste blanche. \n
*			Chaque entrée représente une clé qui doit-être chifrée.
*			
*/
typedef struct _KEY_MAP {
	USHORT	ScanCode;			/*!< \~English The scancode that needs to be ciphered. Value is hexadecimal. \~French Le scancode à chiffrer en hexadécimal*/
//	char	KeyID[15];			/*!< \~English The identifier of the above scancode.\n This value is for a specific language keyboard. In this file, this for the french one. \~French L'identificateur du scancode ci-dessus.\n Cette valeur est spécifique à une langue. Dans ce fichier, il s'agit du français. */
} KEY_MAP, *PKEY_MAP;








/**
*\~English
*\brief		Define the whitelist	
*\details	The whitelist which contains the scancodes that must be ciphered. \n 
*			Do not forget to change #AN_TABLE_SIZE value if you add or remove an element to the list. \n
*			Test vectors must be updated too. The function GostxabordRegenerateStaticTestVectors can be used to do so.  
*
*\~French
*\brief		Définit la liste blanche																		   
*\details	La liste blanche contient les scancodes qui doivent-être chiffrés. \n
*			Ne pas oublier de changer la valeur #AN_TABLE_SIZE si des élements sont ajoutés ou enlevés de la liste. \n
*			Les vecteurs de test statiques doivent aussi mis à jour. La fonction GostxvoardRegenerateStaticTestVectors peut être 
*			utilisée à ces fins.
*/
static KEY_MAP	KEYBOARD_MAP_ALPHAN_FR[] = {

/* ScanCode	 | French equivalent */
   	{ 0x02 }, //, "&" }, 
	{ 0x03 }, //, "é"},	
	{ 0x04 }, //, "Appostrophe" }, 		 
	{ 0x05 }, //, "'" }, 				 
	{ 0x06 }, //, "(" }, 				 
	{ 0x07 }, //, "-" }, 	
	{ 0x08 }, //,"è" },	
	{ 0x09 }, //, "_" }, 
	{ 0x0a }, //, "ç"},					 
	{ 0x0b }, //, "à"}	
	{ 0x0c }, //, ")" }, 				 
	{ 0x0d }, //, "=" }, 
	{ 0x10 }, //, "a" }, 				 
	{ 0x11 }, //, "z" }, 				 
	{ 0x12 }, //, "e" }, 				 
	{ 0x13 }, //, "r" }, 				 
	{ 0x14 }, //, "t" }, 				 
	{ 0x15 }, //, "y" }, 				 
	{ 0x16 }, //, "u" }, 				 
	{ 0x17 }, //, "i" }, 				 
	{ 0x18 }, //, "o" }, 	
	{ 0x19 }, //, "p" }			 
	{ 0x1a }, //, "^" }, 				 
	{ 0x1b }, //, "$" }, 	, 				 
	{ 0x1e }, //, "q" }, 				 
	{ 0x1f }, //, "s" }, 				 
	{ 0x20 }, //, "d" }, 				 
	{ 0x21 }, //, "f" }, 				 
	{ 0x22 }, //, "g" }, 				 
	{ 0x23 }, //, "h" }, 				 
	{ 0x24 }, //, "j" }, 				 
	{ 0x25 }, //, "k" }, 				 
	{ 0x26 }, //, "l" }, 				 
	{ 0x27 }, //, "m" }, 			 
	{ 0x2b }, //, "*" }, 	
	{ 0x2c }, //, "w" }, 					 
	{ 0x2d }, //, "x" }, 				 
	{ 0x2e }, //, "c" }, 				 
	{ 0x2f }, //, "v" }, 				 
	{ 0x30 }, //, "b" }, 				 
	{ 0x31 }, //, "n" }, 				 			 
	{ 0x32 }, //, "," }, 				 
	{ 0x33 }, //, ";" }, 				 
	{ 0x34 }, //, ":" }, 				 
	{ 0x35 }, //, "!" }, 				 
	{ 0x39 }, //, "Space" },
	{ 0x56 },  //,"<" },
	{ 0x28 }, 
	{ 0x29 }
};


/**
*\~English
*\brief		Defines number of elements present in the whitelist
*\details	This define must be changed if an element is added or removed from the whilelist. This is normally done programatically. 
*
*\~French
*\brief		Définit le nombre d'éléments présent dans la liste blanche
*\details	Ce define doit être changé si un élément est ajouté ou retiré de la liste blanche. Cela est normalement réalisé automatiquement.
*
*/
#define AN_TABLE_SIZE	RTL_NUMBER_OF(KEYBOARD_MAP_ALPHAN_FR)

/**
*\~English
*\brief		Defines number of elements present in the whitelist
*\details	This value must be changed if an element is added or removed from the whilelist.
*
*\~French
*\brief		Définit le nombre d'éléments présent dans la liste blanche
*\details	Cette veleur doit être changée si un élément est ajouté ou retiré de la liste blanche.
*
*/
static DWORD	KEYBOARD_MAP_ALPHAN_FR_SIZE = AN_TABLE_SIZE;
static KEY_MAP  KEYBOARD_SHUFFLE_ALPHAN_FR[AN_TABLE_SIZE];


/**
*\~English
* \brief	Define numerics keys 
* \details  Defines numerics keys on the numpad
*
*\~French
* \brief    Définit les clés numériques
* \details	Définit les clés numériques du padnumérique
*
*/
static KEY_MAP KEYBOARD_MAP_NUMPAD[] = {

	/* ScanCode	 | French equivalent */

		{ 0x4f }, //, "1" },			// 1
		{ 0x50 }, //, "2" },			// 2
		{ 0x51 }, //, "3" },			// 3
		{ 0x4b }, //, "4" },			// 4
		{ 0x4c }, //, "5" },			// 5
		{ 0x4d }, //, "6" },			// 6
		{ 0x47 }, //, "7" },			// 7
		{ 0x48 }, //, "8" },			// 8
		{ 0x49 } //, "9" }				// 9
};
#define NUMPAD_TABLE_SIZE		9



//======= Currently unused =======/

//**********************************************
// Scan-codes relatifs aux fonctions, stockés 
//sous la forme de KEY_MAP
//****
static KEY_MAP KEYBOARD_MAP_FUNCTION_FR[] = {

	/* ScanCode	 | French equivalent */

		{ 0x01 }, //, "ESC"	},			// Echap
		{ 0x0e }, //, "BACK"	},		// Retour Arrière
		{ 0x0f }, //, "LCTRL"	},		// Controle Gauche
		{ 0x1d }, //, "RCTRL"	},		// Controle Droit
		{ 0x38 }, //, "ALT"	},			// Alt
		{ 0x5b }, //, "LWIN"	},		// Windows Gauche
		{ 0x5c }, //, "RWIN"	},		// Windows Droit
		{ 0x5d }, //, "APP"	},			// Application
		{ 0x5f } //, "SLEEP"	},		// Veille
};


//**********************************************
// Scan-Codes relatifs aux touches FN d'un
//	clavier, stockés sous la forme de KEY_MAP
//****
static KEY_MAP KEYBOARD_MAP_FN_FR[] = {

	/* ScanCode	 | French equivalent */

		{ 0x3b }, //, "F1"	},
		{ 0x3c }, //, "F2"	},
		{ 0x3d }, //, "F3"	},
		{ 0x3e }, //, "F4"	},
		{ 0x3f }, //, "F5"	},
		{ 0x40 }, //, "F6"	},
		{ 0x41 }, //, "F7"	},
		{ 0x42 }, //, "F8"	},
		{ 0x43 }, //, "F9"	},
		{ 0x44 } //, "F10"	},
};


//**********************************************
// Scan-Codes relatifs aux touches usuellement 
//	utiliséss lors d'un traitement de texte, 
//  stockés sous la forme de KEY_MAP
//****
static KEY_MAP	KEYBORAD_MAP_TEXT_FR[] = {

	/* ScanCode	 | French equivalent */

		{ 0x0e }, //, "BACK"	},			// Retour Arrière
		{ 0x47 }, //, "ORIG"	},			// Debut
		{ 0x48 }, //, "UP"	},				// Fleche du haut 
		{ 0x4b }, //, "LEFT"	},			// Fleche gauche
		{ 0x4d }, //, "RIGHT"	},			// Fleche droite
		{ 0x4f }, //, "END"	},				// Fin
		{ 0x49 }, //, "PREC"	},			// Precedent(pages)
		{ 0x50 }, //, "DOWN"	},			// Fleche du bas
		{ 0x51 }, //, "NEXT"	},			// Suivant(pages)
		{ 0x52 }, //, "INSRT"	},			// Insertion
		{ 0x53 }, //, "DEL"	},				// Suppression
		{ 0x0f }, //, "ENTER" },			// Entrer
		{ 0x2a }, //, "LMAJ"	},			// Majuscule Gauche
		{ 0x36 }, //, "RMAJ"  },			// Majusucule Droite
		{ 0x38 }, //, "ALTGR"	},			// Alt Grammaire
		{ 0x3a } //, "VERMAJ"},				// Verrouiller Majuscule
};



//**********************************************
// Scan-Codes relatifs aux touches de gestion média
//	,stockés sous la forme de KEY_MAP
//****
static KEY_MAP KEYBOARD_MAP_MEDIA_FR[] = {

	/* ScanCode	 | French equivalent */

		{ 0x65 }, //, "BRWSEARCH"	},		// Recherche Navigateur
		{ 0x66 }, //, "BRWFAVORIT"},		// Favoris Navigateur
		{ 0x67 }, //, "BRWREFRESH"},		// Rafraichissement Page
		{ 0x68 }, //,	"BRWSTOP"},			// Stop chargement Page
		{ 0x69 }, //, "BRWFORWARD"},		// Page Suivante navigateur
		{ 0x6a }, //, "BRWBACK"},			// Page Précédente navigateur
		{ 0x6b }, //, "LAPP1"},				// Lancer Application 1
		{ 0x6c }, //, "LMAIL"},				// Lancer Application Mail
		{ 0x6d }, //, "LMEDIASLCT"},		// Lancer Selecteur de Média
		{ 0x37 }, //, "PRINTSCREEN"},		// Impression Ecran
		{ 0xff } //, "xENDx" }				// Identificateur de fin

};



//**********************************************
// Scan-Codes relatifs aux touches de gestion du
//	du son, stockés sous la forme de KEY_MAP
//****
static KEY_MAP KEYBOARD_MAP_SOUND_FR[] = {

	/* ScanCode	 | French equivalent */

		{ 0x30 }, //, "VUP"	},				// Volume Up
		{ 0x2e }, //, "VDOWN"	},			// Volume Down
		{ 0x20 }, //, "MUTE"	},			// Mute
		{ 0x22 }, //, "MPLAY"	},			// Play / Pause Media
		{ 0x24 }, //, "MSTOP" },			// Stop Media
		{ 0x19 }, //, "MNEXT" },			// Media Suivante
		{ 0x10 }, //, "MPREV"	},			// Media précédent
		{ 0xff } //, "xENDx" }				// Identificateur de fin
};


#endif