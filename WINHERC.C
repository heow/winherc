// Clipboard copying program to Herc Graphics Card for Windows 3.0
// 22 Jan 92 by Heow

#include <windows.h>
#include <dos.h>
#include "dpmi.h"

//-------------------------------------------------------------------- WinMain

long FAR PASCAL WndProc (HWND, WORD, WORD, LONG);

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
	{
	static char szAppName [] = "WinHerc";
	HWND	   	hwnd;
	MSG			msg;
	WNDCLASS	wndclass;

	if (!hPrevInstance)
		{
		wndclass.style			= CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc    = WndProc;
		wndclass.cbClsExtra		= 0;
		wndclass.cbWndExtra		= 0;
		wndclass.hInstance		= hInstance;
		wndclass.hIcon			= NULL;
		wndclass.hCursor        = LoadCursor (NULL, IDC_ARROW);
		wndclass.hbrBackground  = GetStockObject (WHITE_BRUSH);
		wndclass.lpszMenuName   = NULL;
		wndclass.lpszClassName  = szAppName;

		RegisterClass (&wndclass);
		}

	hwnd = CreateWindow (szAppName,
		"WinHerc",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow (hwnd, nCmdShow);
	UpdateWindow (hwnd);

	while (GetMessage (&msg, NULL, 0, 0))
		{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
		}
	return msg.wParam;
	}

//-------------------------------------------------------------------- WinProc

long FAR PASCAL WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
	{
	static HWND			hwndNextViewer;
	HANDLE				hGMem;
	HDC 				hdc;
	LPSTR				lpGMem;
	PAINTSTRUCT     	ps;
	RECT				rect;

	int 				iMonoAddr;
	unsigned			sel, ds;

	extern char			_B000H[2000];
	char far			*ptr;
	char far			*textptr;
	int                 i, j, colcnt;
	char				nextchar, lastchar;

	switch (message)
		{
		case WM_CREATE:
			hwndNextViewer = SetClipboardViewer (hwnd);
			return 0;

		case WM_CHANGECBCHAIN:
			if (wParam == hwndNextViewer)
				hwndNextViewer = LOWORD (lParam);
			else if (hwndNextViewer)
				SendMessage (hwndNextViewer, message, wParam, lParam);
			return 0;

		case WM_DRAWCLIPBOARD:
			if (hwndNextViewer)
				SendMessage (hwndNextViewer, message, wParam, lParam);
			InvalidateRect (hwnd, NULL, TRUE);
			return 0;

		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps);
			GetClientRect (hwnd, &rect);
			OpenClipboard (hwnd);

			if (hGMem = GetClipboardData (CF_TEXT))
				{
				lpGMem = GlobalLock (hGMem);
				DrawText (hdc, lpGMem, -1, &rect, DT_EXPANDTABS);

				ptr = (LPSTR) MAKELONG (0, _B000H); // make far ptr to video

				for (i=0; i <= 4000; i+=2) // clear screen (even addr)
					ptr[i] = ' ';

				// put text in now
				i=0;
				colcnt = 0;
				textptr = lpGMem;
				lastchar = 0x00;
				nextchar = 0x00;
				while (*textptr != NULL && i <= 4000)
					{
					if (colcnt == 81)
						colcnt = 0;
					lastchar = nextchar;
					nextchar = *(textptr++);
					if (nextchar == 0x0D && lastchar != 0x0A)
						{
						i+=(160-(colcnt*2));
						colcnt=0;
						}
					if (nextchar == 0x0A && lastchar != 0x0D)
						{
						i+=(160-(colcnt*2));
						colcnt=0;
						}
					if (nextchar == 0x09)
						{
						j=0;
						while (j < 8)
							{
							i+=2;
							colcnt+=1;
							j++;
							if (colcnt % 8 == 0)
								j=8;
							}
						}
					if (nextchar != 0x0A && nextchar != 0x0D && nextchar != 0x09)
						{
						ptr[i] = nextchar;
						i+=2;
						colcnt+=1;
						}
					}

				GlobalUnlock (hGMem);
				}

			CloseClipboard ();
			EndPaint (hwnd, &ps);
			return 0;

		case WM_DESTROY:
			ChangeClipboardChain (hwnd, hwndNextViewer);
			PostQuitMessage (0);
			return 0;
		}
	return DefWindowProc (hwnd, message, wParam, lParam);
	}
