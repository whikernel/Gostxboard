// GuiClient.cpp�: d�finis le point d'entr�e pour l'application.
//

#include "stdafx.h"
#include "MainDialog.h"
#include "GuiClient.h"

#define MAX_LOADSTRING 100

// Variables globales�:
HINSTANCE hInst;								// instance actuelle
TCHAR szTitle[MAX_LOADSTRING];					// Le texte de la barre de titre
TCHAR szWindowClass[MAX_LOADSTRING];			// le nom de la classe de fen�tre principale


//***********************************************
// But : Fonction principale g�rant l'interface
//		Cette fonction est la prem�re appel�e lors 
//		du lancement de l'�x�cutable
//
//****
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





