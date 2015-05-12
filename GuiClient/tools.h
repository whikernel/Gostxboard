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