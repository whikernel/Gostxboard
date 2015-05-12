// GuiClient.cpp : définis le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "MainDialog.h"
#include "GuiClient.h"

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;								// instance actuelle
TCHAR szTitle[MAX_LOADSTRING];					// Le texte de la barre de titre
TCHAR szWindowClass[MAX_LOADSTRING];			// le nom de la classe de fenêtre principale


//***********************************************
// But : Fonction principale gérant l'interface
//		Cette fonction est la premère appelée lors 
//		du lancement de l'éxécutable
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





