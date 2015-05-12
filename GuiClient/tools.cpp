#include "stdafx.h"

#include <string.h>

#include "tools.h"


//***********************************************
// But : Permet de récupérer le dernier charactère
//	d'une chaine. La chaine doit contenir le caractère 
//	de terminaison '\0'. Le cas échéant le dwSize ième - 1
//	charactère est renvoyé.
//
//- CHAR
//****
char GetLastChar(char * cMessage, int dwSize)
{
	int  dwIndex = 0;

	//
	// Pour la longueur de la chaine, ou jusqu'à rencontrer 
	// le caractère de terminaison
	//
	while ((dwIndex <= dwSize) || (cMessage[dwIndex] != '\0'))
	{
		dwIndex++;
	}
	//
	// On retourne le dernier caractère
	//
	return cMessage[dwIndex - 1];
}


//***********************************************
// But : Ajout un caractère à une chaine. Aucune 
//	vérification n'est faite sur la taille de la chaine.
//	L'utilisateur doit s'assurer que la chaine peut contenir
//	ce caractère de plus.  
//
//- INT 
//****
int AppendChar(char * String, char Char)
{
	int		dwLength = 0;

	dwLength = strlen(String);
	String[dwLength] = Char;
	String[dwLength + 1] = '\0';

	return 0;
}