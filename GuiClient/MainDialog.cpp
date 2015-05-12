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
* \file      MainDialog.cpp
* \author	 Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Defines almost all functions of the graphical interface
*
* \details  Defines almost all functions used to display and handle the graphical interface. \n 
*			It is done in order to show how the API can be used by any application. \n 
*			It is surely not a perfect app and does not represent the work which has been 
*			done for the security module. 
*
* \~French
* \file		MainDialog.cpp
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit la plupart des fonctions de l'interface graphique
*
* \details	Définit les plupart des fonctions utilisés pour générer et gérer l'interface graphique. \n 
*			Elle a été créée pour montrer comment l'API peut-être utilisée par n'importe quelle application. \n 
*			Ce n'est certainement pas une application parfaite et elle ne représente pas le travail accomplie pour le module 
*			de sécurité. 
*/


#include "stdafx.h"
#include <CommCtrl.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>

#include "MainDialog.h"
#include "Resource.h"
#include "tools.h"


HWND						hProgressBar = NULL;
HWND						hMainWindow = NULL;
DWORD						dwIndexChar = 0;
BOOL						bCipherStarted = FALSE;
char *						cBtnDisconnect	= "Disconnect";
char *						cBtnConnect		= "Connect";

DWORD						LastReturn = 0x0;

void CallbackVerbose(
	_In_ char *		Message,
	_In_ DWORD 		dwLastReturnCode,
	_In_ BYTE		bLevel
	);


typedef void(*PGOSTXBOARD_VERBOSE_CALLBACK) (
	_In_ char *		Message,
	_In_ DWORD 		dwLastReturnCode,
	_In_ BYTE		bLevel
	);

typedef DWORD(__stdcall *SESSION_CALLBACK)(PGOSTXBOARD_VERBOSE_CALLBACK VerboseCallback);
typedef DWORD(__stdcall *SESSION)(void);
typedef void(__stdcall *DEV)(void);
typedef char*(__stdcall *PRINT)(DWORD);
typedef DWORD(__stdcall *CRYPTO)(PUCHAR);
typedef DWORD(__stdcall *MONITOR)(LPDWORD lpId, LPDWORD lpReturn);
#define CODE_SUCCESS(_code_)		(_code_ < 0xff)


HINSTANCE			hinstLib;
SESSION_CALLBACK	InitiateSession;
SESSION				BeginSession,
					CloseSession;
CRYPTO				DecipherKey;
PRINT				PrintCode;
MONITOR				MonitorProcessus;
DEV					GenerateVectors;
DWORD				dwReturn,
					dwProcessId;


LPTSTR lpBuffer = (LPTSTR)L"Cliquer sur connecter pour commencer";

/**
*\~English
*\brief		Define the main callback of the window
*\details	Define the main callback of the window currently displayed. Every elements 
*			linked to the window are processed in this function
*
*\~French
*\brief		Definit le callback principal de la fenêtre
*\details	Définit le callback principal de la fenêtre. Tous les élements qui y sont 
*			liés sont gérés dans cette fonction 
*/
INT_PTR CALLBACK MainDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD		lw			= LOWORD(wParam);
	WORD		hw			= HIWORD(wParam);
	LRESULT		lpLength	= 0;
//	DWORD	dwProcessId		= 0;
	char	*  cBuffer		= NULL;
	char	Ciphered		;

	char * Message = NULL;

	UNREFERENCED_PARAMETER(lParam);

	switch (uMsg)
	{
			case WM_INITDIALOG:
			{
				hMainWindow = hwndDlg;	

				UpdateState("Loading library..");
				//
				// Load dll
				//
				hinstLib = LoadLibrary(TEXT("GostxBoardx86.dll"));
				if (hinstLib == NULL)
				{
					// Dll not found
					UpdateState("Dll not found"); 
					MessageBoxA(NULL, "Fatal error.\nGostxBoard.dll was not found.", "GostxBoard Error", MB_ICONERROR);
					
					//EndDialog(hwndDlg, IDCANCEL);
					return (INT_PTR)FALSE;
				}
				InitiateSession		= (SESSION_CALLBACK)GetProcAddress(hinstLib, "GostxBoardInitiateSession");
				BeginSession		= (SESSION)GetProcAddress(hinstLib, "GostxBoardBeginCipherSession");
				CloseSession		= (SESSION)GetProcAddress(hinstLib, "GostxBoardCloseCipherSession");
				DecipherKey			= (CRYPTO)GetProcAddress(hinstLib, "GostxBoardDecipherChar");
				MonitorProcessus	= (MONITOR)GetProcAddress(hinstLib, "GostxBoardStartMonitorProcessusState");
				PrintCode			= (PRINT)GetProcAddress(hinstLib, "GostxBoardGetCodeMessage");
				GenerateVectors		= (DEV)GetProcAddress(hinstLib, "GostxBoardRegenerateStaticTestVectors");

				if ((!InitiateSession) && (!BeginSession) && (!CloseSession)
					&& (!DecipherKey) && (!MonitorProcessus)
					&& (!PrintCode) && (!GenerateVectors))
				{
					//
					// Error. Unable to load at least on function
					//
					UpdateState("Unable to load dll's functions");
					MessageBoxA(NULL, "Fatal error.\nUnable to load dll's functions", "GostxBoard Error", MB_ICONERROR);
					EndDialog(hwndDlg, IDCANCEL);
					return (INT_PTR)FALSE;
				}
				UpdateState("Done. Initialization completed.");

				//
				// Initiate messages
				//
				UpdateState("Click connect to begin a cipher session");
				SetDlgItemTextA(hwndDlg, IDC_INFO_STATE, "App not protected\nNo connection");
				SetDlgItemTextA(hwndDlg, IDC_INFO_STRCT, "No data");

				//
				// Initiate entry mirror field. It is in read only
				//
				SendMessage( GetDlgItem(hwndDlg, IDT_NORMAL_TEXT), EM_SETREADONLY ,(WPARAM)TRUE, (LPARAM)NULL );
				//
				// Initiate the progress bar on a 100 basis
				//
				hProgressBar = GetDlgItem(hwndDlg, IDC_PROGRESS_BAR);
				SendMessage(hProgressBar, PBM_SETRANGE32, 0, 100);
				SendMessage(hProgressBar, PBM_SETSTEP, 1, 0);
				//
				//Redraw the progress bar to update 
				//
				InvalidateRect(hProgressBar, NULL, TRUE);


				return (INT_PTR)TRUE;
			}
			case WM_COMMAND:
			case WM_NOTIFY:
			{
				switch (hw) 
				{
				case EN_UPDATE :
						//
						// A key is received. This signal is emitted before the char to be displayed. 
						// So we can process the entry immediatly. Get the entry buffer, the last char and then 
						// cipher it, if necessary. 
						//
						lpLength = SendMessage(GetDlgItem(hMainWindow, IDT_CIPHER_TEXT), WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);

						// Allocate 
						cBuffer = (char *)malloc((lpLength + 2) * sizeof(char));

						// Get text from input field
						GetDlgItemText(hMainWindow, IDT_CIPHER_TEXT, (LPSTR)cBuffer, (lpLength+ 1) );
						Ciphered = GetLastChar(cBuffer, strlen(cBuffer) - 1);
						if (bCipherStarted)
						{
							CipherChar(Ciphered);
						}
						else
							UpdateDecipheredText(Ciphered);

						free(cBuffer);

						return (INT_PTR)TRUE;
					break;
				case EN_KILLFOCUS :
					//
					// The focus on the input field is lost. 
					// We have to stop the cipher session in order to keep 
					// the generation key synchronized with the driver
					//
					if (bCipherStarted)
						CloseSession();
					return (INT_PTR)TRUE;
					break;
				case EN_SETFOCUS : 
					//
					// The focus of the imput field is set. 
					// If a session is started, we have to restart 
					// a cipher session
					//
					if (bCipherStarted)
						BeginSession();
					return (INT_PTR)TRUE;
				}
				switch (lw)
				{
					case IDM_START :
					case IDC_CONNECT_BUTTON:
					{
						//
						// User wants to start a cipher session. Check if a cipher session is already launched and 
						// start the session.
						//
						if (!bCipherStarted)
						{
							SendMessage(GetDlgItem(hMainWindow, IDM_START), WM_SETTEXT, (WPARAM)NULL, (LPARAM)cBtnDisconnect);
							SendMessage(GetDlgItem(hMainWindow, IDC_CONNECT_BUTTON), WM_SETTEXT, (WPARAM)NULL, (LPARAM)cBtnDisconnect);
							Start();
							if (!CODE_SUCCESS(LastReturn))
							{
									Message = PrintCode(LastReturn);
									UpdateProgressBar(100, Message, 1);
							}
							else
							{
								bCipherStarted = TRUE;
								UpdateProgressBar(-1, NULL, WARN_COLOR);
							}

						}
						else
						{
							SendMessage(GetDlgItem(hMainWindow, IDM_START), WM_SETTEXT, (WPARAM)NULL, (LPARAM)cBtnConnect);
							SendMessage(GetDlgItem(hMainWindow, IDC_CONNECT_BUTTON), WM_SETTEXT, (WPARAM)NULL, (LPARAM)cBtnConnect);
							LastReturn = CloseSession();
							if (!CODE_SUCCESS(LastReturn))
							{
								Message = PrintCode(LastReturn);
								UpdateProgressBar(100, Message, 1);
							}
							else
							{
								bCipherStarted = FALSE;
							}
						}
						return (INT_PTR)TRUE;
					}
						break;
					case IDC_RESET_BUTTON :
						//
						// Reset texts fields 
						//
							SetWindowText(GetDlgItem(hMainWindow, IDT_NORMAL_TEXT), (LPCSTR)"");
							SetWindowText(GetDlgItem(hMainWindow, IDT_CIPHER_TEXT), (LPCSTR)"");
						return (INT_PTR)TRUE;
						break;
					case IDM_EXIT: 
					case IDOK:
					case IDCANCEL:
					{
						// 
						// User wants to exit the application. 
						// We just kill the process. Every thing will be cleaned.
						//
						EndDialog(hwndDlg, IDCANCEL);
						return 1;
					}
					break;
					break;
					case IDM_ABOUT :
					case IDC_HELP_BUTTON :
						//
						// User wants help. We display a dialog box to help him. 
						// Yes it is in french.. 
						//
						DialogBox( NULL, MAKEINTRESOURCE(IDD_ABOUT_DIALOG), NULL, AboutDialogProc);
					break;

				}
				return 0;
			}
			default :
				break;

	}
	return (INT_PTR)FALSE;
}



/**
*\~English
*\brief		Update the progress bar
*\details	The function updates the progress bar of the window as the message in the listbox. \n
*			
*\param		int dwValue Value of the progress bar on a 100-basis
*\param		char * Message Message to display on the listbox.
*\param		int color Color of the progress bar, defined in the header of the file. 
*\return	void
*
*\~French
*\brief		Met à jour la barre de progression
*\details	La fonction met à jour la barre de progression ainsi que le message l'accompagnant dans la listbox.   \n
*
*\param		int dwValue Valeur de la barre de progression sur une base de 100.
*\param		char * Message Message à afficher dans la list box
*\param		int color Couleur de la barre de progression , définit dans le header de ce fichier. 
*\return	void
*/
void UpdateProgressBar(int dwValue, char * Message, int color )
{
	DWORD		dwCount			= 0;
	COLORREF	redBar			= RGB( 205,38, 38 );
	COLORREF	greenBar		= RGB(0, 205, 0);
	COLORREF	orangeBar		= RGB(255, 139, 61);

	// Update the progress bar 
	if (dwValue > 0)
		SendMessageA(hProgressBar, PBM_SETPOS, (int)(100.0 * dwValue / 100), 0);

	// Update the message
	if (Message)
		SendMessageA(GetDlgItem(hMainWindow, IDC_STATE), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)Message);

	// Focus on the message of list box
	dwCount = SendMessage(GetDlgItem(hMainWindow, IDC_STATE), LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
	SendMessage(GetDlgItem(hMainWindow, IDC_STATE), LB_SETCARETINDEX, (WPARAM)(dwCount - 1), (LPARAM)FALSE);

	// Update the color of the progress bar
	if( color == ERR_COLOR )
	{
		// Red
		SendMessageA(hProgressBar, PBM_SETBARCOLOR, (WPARAM)0, (LPARAM)redBar);
	}
	else if (color == WARN_COLOR)
	{
		// Orange 
		SendMessageA(hProgressBar, PBM_SETBARCOLOR, (WPARAM)0, (LPARAM)orangeBar);
	}
	else
	{
		// Green 
		SendMessageA(hProgressBar, PBM_SETBARCOLOR, (WPARAM)0, (LPARAM)greenBar);
	}

	// Refresh the interface
	InvalidateRect(hProgressBar, NULL, TRUE);
	ShowWindow(hProgressBar, SW_HIDE);
	ShowWindow(hProgressBar, SW_SHOW);
	ShowWindow(GetDlgItem(hMainWindow, IDC_STATE), SW_HIDE);
	ShowWindow(GetDlgItem(hMainWindow, IDC_STATE), SW_SHOW);

}




/**
*\~English
*\brief		Update the list box message
*\details	The function updates the message in the listbox. \n
*
*\param		char * Message Message to display on the listbox.
*\return	void 
*
*\~French
*\brief		Met à jour la listbox
*\details	La fonction met à jour le message dans la listbox.   \n
*
*\param		char * Message Message à afficher dans la list box
*\return	void
*/
void UpdateState(char * Message)
{
	DWORD	dwCount = 0;
	// Add thge message in the list box
	SendMessageA(GetDlgItem(hMainWindow, IDC_STATE), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)Message);
	// Focus on last message
	dwCount = SendMessage(GetDlgItem(hMainWindow, IDC_STATE), LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
	SendMessage(GetDlgItem(hMainWindow, IDC_STATE), LB_SETCARETINDEX, (WPARAM)(dwCount - 1), (LPARAM)FALSE);
	// Update the window
	ShowWindow(GetDlgItem(hMainWindow, IDC_STATE), SW_HIDE);
	ShowWindow(GetDlgItem(hMainWindow, IDC_STATE), SW_SHOW);

	return;
}



/**
*\~English
*\brief		Initiate and start a cipher session
*\details	The function calls the API functions to initiate and start a new cipher session. \n 
*			Return value is stored in global value LastReturn. 
*
*\~French
*\brief		Initialise et démarre une sesison de chiffrement
*\details	La fonction appelle les fonction de l'API pour initialiser et démarrer une session de chiffrement. \n
*			La valeur de retour est stocké dans la variable globale LastReturn.
*/
void Start()
{
	DWORD dwLocalReturn = 0;

	//
	// Initiate session 
	// 
	dwLocalReturn = InitiateSession((PGOSTXBOARD_VERBOSE_CALLBACK)CallbackVerbose);
	if (!CODE_SUCCESS(dwLocalReturn))
	{

		UpdateProgressBar(100, PrintCode(dwLocalReturn), 1);

		goto end;
	}

	//
	// Get the current process ID, so the API 
	// will be able to monitor us. 
	//
	dwProcessId = GetCurrentProcessId();
	UpdateState("Begining monitor.");
	dwLocalReturn = MonitorProcessus(&dwProcessId, &dwReturn);
	if (!CODE_SUCCESS(dwLocalReturn) || !CODE_SUCCESS(dwReturn))
	{
		UpdateProgressBar(100, PrintCode(dwLocalReturn), 1);
		goto end;
	}
	UpdateState("Start completed.");

end:
	return;
}



/**
*\~English
*\brief		Update the decrypted field
*\details	The function appends a char to the decrypted field. \n
*
*\param		USHORT cChar Char to add to decrypted field. 
*\return	void 
*
*\~French
*\brief		Met à jour la fenêtre de déchiffrement.
*\details	La fonction ajoute un caractère au champ de déchiffrement.
*
*\param		USHORT cChar Char à ajouter au champ
*\return	void
*/
void UpdateDecipheredText(USHORT cChar)
{
	char *		cText		= NULL;
	LRESULT		lpLength	= 0;

	// Get the current length of the text
	lpLength = SendMessage(GetDlgItem(hMainWindow, IDT_NORMAL_TEXT), WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);

	// Allocate buffer to get text field
	cText = (char *)malloc((lpLength + 2)* sizeof(char));

	// Get the current text in a temporary buffer
	SendMessage(GetDlgItem(hMainWindow, IDT_NORMAL_TEXT), WM_GETTEXT, lpLength + 1 , (LPARAM)cText);

	// Add the char
	AppendChar(cText, (char)cChar);

	// Update the window
	SetWindowTextA(GetDlgItem(hMainWindow, IDT_NORMAL_TEXT), cText);
	
	//Free 
	free(cText);
	cText = NULL;
	
	return;
}


/**
*\~English
*\brief		Decrypt a keyboard entry
*\details	Decrypt an entry if necessary. The function calls the API to do so. \n 
*			The function updates the decrypted filed itself.
*
*\param		char cChar Char to decipher
*\return	char 42
*
*\~French
*\brief		Déchiffre une entrée clavier 
*\details	Déchiffre une entrée clavier si nécessaire. La fonction appelle l'API pour efféctuer cette action. \n 
*			La fonction met automatiquement à jour le champ de déchiffrement.
*	
*\param		char cChar Caractère à déchiffrer
*\return	char 42
*/
char CipherChar(char cChar)
{
	UCHAR	usInKey = 0;
	DWORD	dwReturn = 0;

	usInKey = cChar;

	//
	// Decipher the key
	//
	dwReturn = DecipherKey(&usInKey);

	if (!CODE_SUCCESS(dwReturn))
	{ 
		UpdateProgressBar(100, "Error while handling ciphering.", 1);
	}

	//
	// Display the char 
	//
	UpdateDecipheredText((USHORT)usInKey);
	
	return (char)(-1);
}



/**
*\~English
*\brief		Window callback of help dialog 
*\details	This function is the callback of each events happening on the help window
*
*
*\~French
*\brief		Callback de la fenêtre d'aide
*\details	Cette fonction permet de gérer tous les évenements liés à la fenêtre d'aide.  
*
*/
INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD	lw				= LOWORD(wParam);

	UNREFERENCED_PARAMETER(lParam);

	switch (uMsg)
	{
			case WM_INITDIALOG:
			{
				// Initiates the text.. 
				SetDlgItemTextA(hwndDlg, IDC_ABOUT_TEXT, "Build v3.2 - May 2015\nCette application permet de réaliser un pont chiffrant entre le clavier et l'application elle-même.\nToutes les données entrées sur le clavier sont chiffrées via un driver filtre inséré dans la pile des drivers claviers puis diffusées au reste du système.\nSeule l'application ici présente peut déchiffrer les données.\nEn effet, lors de l'initialisation via le bouton connecter, le driver et l'application échangent une clé de chiffrement. Cette clé permet d'initialiser un générateur pseudo-alétoire dont la sortie va permettre de chiffrer les données. Ainsi seule l'application disposant de la clé et du même générateur pseudo aléatoire peut déchiffrer. Pour éviter tout risque, la clé est renouvellée tous les 10 caractères. Une et une seule application peut demander le chiffrement au driver en même temps. En cas de conflits, les demandes sont rejetés et l'utilisateur est averti.\nAfin de garantir un maximum de sécurité, lorsque l'application n'est plus detectée en premier plan, la session s'arrête et les touches ne sont plus chiffrées. Si jamais l'application crash, le driver le détecte immédiatement et met stoppe la session de chiffrement.\n\n\n\nCrédits Paul Amicelli aka WhiteKernel - 2015");
				return (INT_PTR)TRUE;
			}
			case WM_COMMAND:
			case WM_NOTIFY:
			{
				switch (lw)
				{
					case IDM_EXIT: 
					case IDOK:
					{
						// Users wants to exit the window
						EndDialog(hwndDlg, IDCANCEL);
						return 1;
					}
					break;
				}
				return 0;
			}
			default :
				break;

	}
	return (INT_PTR)FALSE;
}




/**
*\~English
*\brief		Callback of the API
*\details	This function is a callback provided by the API in order to inform the application 
*			from special events. 
*
*\param		char * Message - Message of the API
*\param		dwLastReturnCode - Last return code of the internal functions of API
*\param		BYTE bLevel - Gravity of the call 
*\return	void
*
*
*\~French
*\brief		Callback de l'API
*\details	Cette fonction à l'application cliente d'être tenu informée de tout ce qu'il se passe
*			au niveau de l'api. 
*
*\param		char * Message - Message de l'API
*\param		dwLastReturnCode - Dernier code de retour interne à l'API
*\param		BYTE bLevel - Gravité de l'appel
*
*/
void CallbackVerbose(
	_In_ char *		Message,
	_In_ DWORD 		dwLastReturnCode,
	_In_ BYTE		bLevel
	)
{
	DBG_UNREFERENCED_PARAMETER(dwLastReturnCode);
	DBG_UNREFERENCED_PARAMETER(Message);
	DBG_UNREFERENCED_PARAMETER(bLevel);

	switch (bLevel)
	{
	case CALL_STD:
		UpdateState(Message);
		break;
	case CALL_WARN:
		UpdateProgressBar(-1, Message, WARN_COLOR);
		break;
	case CALL_ERR:
	case CALL_CRIT:
	case CALL_STOP:
		UpdateProgressBar(100, Message, ERR_COLOR);
		break;
	case CALL_START:
		UpdateProgressBar(100, Message, STD_COLOR);
		break;
	}

	return;
}