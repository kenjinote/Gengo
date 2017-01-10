#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

void TextOutV(HDC hdc, LPRECT lpRect, LPCWSTR lpszText)
{
	if (!hdc || !lpRect || !lpszText) return;
	const UINT align = SetTextAlign(hdc, TA_CENTER);
	const int nSize = lstrlenW(lpszText);
	if (!nSize) return;
	int nWidth = 0;
	int nHeight = 0;
	LPCWSTR p = lpszText;
	while (*p)
	{
		SIZE size;
		GetTextExtentPoint32W(hdc, p, 1, &size);
		nWidth += size.cx;
		nHeight += size.cy;
		p = CharNextW(p);
	}
	const int offset = 10;
	const int x = lpRect->left + (lpRect->right - lpRect->left - nWidth / nSize) / 2;
	int y = lpRect->top + (lpRect->bottom - lpRect->top - nHeight - offset * (nSize - 1)) / 2;
	p = lpszText;
	while (*p)
	{
		TextOutW(hdc, x, y, p, 1);
		SIZE size;
		GetTextExtentPoint32W(hdc, p, 1, &size);
		y += size.cy + offset;
		p = CharNextW(p);
	}
	SetTextAlign(hdc, align);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WCHAR szGengo[256];
	switch (msg)
	{
	case WM_CREATE:
		{
			SYSTEMTIME systime;
			GetLocalTime(&systime);
			GetDateFormat(1041, DATE_USE_ALT_CALENDAR, &systime, TEXT("gg"), szGengo, 256);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			const HDC hdc = BeginPaint(hWnd, &ps);
			RECT rect;
			GetClientRect(hWnd, &rect);
			TextOutV(hdc, &rect, szGengo);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("現在の元号"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
