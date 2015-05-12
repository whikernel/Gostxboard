#include "stdafx.h"

#include <string.h>

#include "tools.h"


//***********************************************
// But : Permet de r�cup�rer le dernier charact�re
//	d'une chaine. La chaine doit contenir le caract�re 
//	de terminaison '\0'. Le cas �ch�ant le dwSize i�me - 1
//	charact�re est renvoy�.
//
//- CHAR
//****
char GetLastChar(char * cMessage, int dwSize)
{
	int  dwIndex = 0;

	//
	// Pour la longueur de la chaine, ou jusqu'� rencontrer 
	// le caract�re de terminaison
	//
	while ((dwIndex <= dwSize) || (cMessage[dwIndex] != '\0'))
	{
		dwIndex++;
	}
	//
	// On retourne le dernier caract�re
	//
	return cMessage[dwIndex - 1];
}


//***********************************************
// But : Ajout un caract�re � une chaine. Aucune 
//	v�rification n'est faite sur la taille de la chaine.
//	L'utilisateur doit s'assurer que la chaine peut contenir
//	ce caract�re de plus.  
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