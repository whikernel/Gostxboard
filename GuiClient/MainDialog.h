#ifndef _MAINDIALOG_H
#define _MAIN_DIAOG_

#include "stdafx.h"
#include <intrin.h>


#define CALL_STD	0x0
#define CALL_WARN 	0x1
#define CALL_ERR	0x2
#define CALL_CRIT	0x3
#define CALL_STOP	0x4
#define CALL_START	0x5

#define ERR_COLOR	0x1
#define WARN_COLOR	0x2
#define STD_COLOR	0x0


BOOL CALLBACK MainDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void UpdateProgressBar(int dwValue, char * Message, int color );
void Start();
void UpdateState(char * Message);
void UpdateStateW(LPCSTR Message);
char CipherChar(char cChar);
void UpdateDecipheredText(USHORT cChar);




#endif 