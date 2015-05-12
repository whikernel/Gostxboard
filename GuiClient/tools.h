#ifndef _TOOLS_H
#define _TOOLS_H

//***********************************************
// But : Permet de r�cup�rer le dernier charact�re
//	d'une chaine. La chaine doit contenir le caract�re 
//	de terminaison '\0'. Le cas �ch�ant le dwSize i�me - 1
//	charact�re est renvoy�.
//
//- char
//****
char GetLastChar(char * cMessage, int dwSize);


//***********************************************
// But : Ajout un caract�re � une chaine. Aucune 
//	v�rification n'est faite sur la taille de la chaine.
//	L'utilisateur doit s'assurer que la chaine peut contenir
//	ce caract�re de plus.  
//
//- INT 
//****
int AppendChar(char * String, char Char);

#endif // tools.h