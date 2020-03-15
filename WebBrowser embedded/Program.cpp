#include <Windows.h>
#include <tchar.h>

#include "WebBrowser.h"

HINSTANCE hInst;
HWND hWndMain;
TCHAR* szWndTitleMain = _T("Embedded WebBrowser in Pure C++");
TCHAR* szWndClassMain = _T("WndClass_EmbeddedWB");

ATOM RegMainClass();
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hWndAddressBar;

#define btnBack 1
#define btnNext 2
#define btnRefresh 3
#define btnGo 4

WebBrowser *webBrowser1;

INT WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   INT nCmdShow)
{

	OleInitialize(NULL);


	MSG msg;

	hInst = hInstance;

	if (!RegMainClass())
	{
		MessageBox(NULL, _T("Cannot register main window class"),
			         _T("Error No. 1"),
					 MB_ICONERROR);
		return 1;
	}

	hWndMain = CreateWindowEx(0, szWndClassMain,
						      szWndTitleMain,
							  WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, CW_USEDEFAULT,
						      CW_USEDEFAULT, CW_USEDEFAULT,
							  NULL, NULL, hInst, NULL);

	ShowWindow(hWndMain, nCmdShow);

	RECT rcClient;
	GetClientRect(hWndMain, &rcClient);

	webBrowser1 = new WebBrowser(hWndMain);
	RECT rc;
	rc.left = 0;
	rc.top = 45;
	rc.right = rcClient.right;
	rc.bottom = rcClient.bottom;
	webBrowser1->SetRect(rc);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

ATOM RegMainClass()
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szWndClassMain;
	wc.lpszMenuName = NULL;
	wc.style = 0;

	return RegisterClass(&wc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		CreateWindowEx(0, _T("BUTTON"),
					   _T("<<< Back"),
					   WS_CHILD | WS_VISIBLE,
					   5, 5,
					   80, 30,
					   hWnd, (HMENU) btnBack, hInst, NULL);

		CreateWindowEx(0, _T("BUTTON"),
					   _T("Next >>>"),
					   WS_CHILD | WS_VISIBLE,
					   90, 5,
					   80, 30,
					   hWnd, (HMENU) btnNext, hInst, NULL);

		CreateWindowEx(0, _T("BUTTON"),
					   _T("Refresh"),
					   WS_CHILD | WS_VISIBLE,
					   175, 5,
					   80, 30,
					   hWnd, (HMENU) btnRefresh, hInst, NULL);

		hWndAddressBar =
		CreateWindowEx(0, _T("EDIT"),
					   _T("http://codeproject.com/"),
					   WS_CHILD | WS_VISIBLE | WS_BORDER,
					   260, 10,
					   200, 20,
					   hWnd, NULL, hInst, NULL);

		CreateWindowEx(0, _T("BUTTON"),
					   _T("Go"),
					   WS_CHILD | WS_VISIBLE,
					   465, 5,
					   80, 30,
					   hWnd, (HMENU) btnGo, hInst, NULL);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case btnBack:
			webBrowser1->GoBack();
			break;
		case btnNext:
			webBrowser1->GoForward();
			break;
		case btnRefresh:
			webBrowser1->Refresh();
			break;
		case btnGo:
			TCHAR *szUrl = new TCHAR[1024];
			GetWindowText(hWndAddressBar, szUrl, 1024);
			webBrowser1->Navigate(szUrl);
			break;
		}
		break;
	case WM_SIZE:
		if (webBrowser1 != 0)
		{
			RECT rcClient;
			GetClientRect(hWndMain, &rcClient);

			RECT rc;
			rc.left = 0;
			rc.top = 45;
			rc.right = rcClient.right;
			rc.bottom = rcClient.bottom;
			if (webBrowser1 != 0)
				webBrowser1->SetRect(rc);
		}
		break;
	case WM_DESTROY:
		ExitProcess(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}