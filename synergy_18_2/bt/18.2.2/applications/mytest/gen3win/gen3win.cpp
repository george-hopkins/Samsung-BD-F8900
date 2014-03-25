// gen3win.cpp : Defines the entry point for the application.
#include "stdafx.h"
#include "gen3win.h"
#include <windows.h>
#include <commctrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
static HANDLE btThread;
// Forward declarations of functions included in this code module:
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK MainProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam);
extern int  CsrMain(void) ;
extern void KeyMessageService(char m);
HWND hwndMain;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
    DialogBox(hInstance,(LPCTSTR)IDD_MAIN,NULL,(DLGPROC)MainProc);
  
	return (int) 0;
}



LRESULT CALLBACK MainProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{

	DWORD    threadId;

	
	switch (msg)
	{


		case WM_INITDIALOG:
			btThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) CsrMain, NULL, 0, &threadId);
		   if(btThread == INVALID_HANDLE_VALUE)
			{
				printf("\nBT thread create failure\n");
			}

			CeSetThreadPriority(btThread, 120);
			return TRUE;
			break;


		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:



					return TRUE;
				case IDCANCEL:


					EndDialog(hDlg,FALSE);
					return TRUE;
				case IDC_BUTTON_0:
					KeyMessageService('0');
					return TRUE;
				case IDC_BUTTON_1:
					KeyMessageService('1');

					return TRUE;
				case IDC_BUTTON_2:
					KeyMessageService('2');

					return TRUE;
				case IDC_BUTTON_3:
					KeyMessageService('3');

					return TRUE;
				case IDC_BUTTON_4:
					KeyMessageService('4');
					return TRUE;
				case IDC_BUTTON_5:
					KeyMessageService('5');
					return TRUE;
				case IDC_BUTTON_6:
					KeyMessageService('6');
					return TRUE;
				case IDC_BUTTON_7:
					KeyMessageService('7');
					return TRUE;
				case IDC_BUTTON_8:
					KeyMessageService('8');
					return TRUE;
				case IDC_BUTTON_9:
					KeyMessageService('9');
					return TRUE;	
				case IDMENU:
					KeyMessageService('M');
					return TRUE;

			}
			return FALSE;
	}
	return FALSE;
}


