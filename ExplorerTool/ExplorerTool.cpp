#include "stdafx.h"
#include "ExplorerTool.h"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = szWindowClass;
	RegisterClass(&wc);

	hInst = hInstance;

	HWND hWnd = CreateWindow(szWindowClass, L"Explorer Tool",
		WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 100,
		nullptr, nullptr, hInstance, nullptr);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
    return static_cast<int>(msg.wParam);
}

LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		InitInstance(hWnd);
		break;
	case WM_COMMAND:
		if (reinterpret_cast<HWND>(lParam) == colorButton) {
			PickColor(hWnd);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void InitInstance(HWND hWnd) {
	colorButton = CreateWindow(WC_BUTTON, L"Set accent color inactive",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 5, 265, 50,
		hWnd, nullptr, hInst, nullptr);
	SendMessage(colorButton, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), TRUE);
}

void PickColor(HWND hWnd) {
	COLORREF customColors[16] = {
		0xFFFFFF, 0x000000, 0xFF0000, 0x00FF00, 0x0000FF, 0x796c62
	};

	CHOOSECOLOR cc = {};
	cc.lStructSize = sizeof cc;
	cc.hwndOwner = hWnd;
	cc.lpCustColors = static_cast<LPDWORD>(customColors);
	cc.rgbResult = 0x796c62;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE) {
		if (SetRegistryDWord(L"Software\\Microsoft\\Windows\\DWM", L"AccentColorInactive", RGB2BGR(cc.rgbResult)) != ERROR_SUCCESS) {
			MessageBox(hWnd, L"Failed to set 'AccentColorInactive'!", L"Error", MB_OK);
		}
	}
}

COLORREF RGB2BGR(const COLORREF rgb, const bool isOpaque) {
	BYTE a = isOpaque ? 0xFF : 0;
	BYTE r = GetRValue(rgb);
	BYTE g = GetGValue(rgb);
	BYTE b = GetBValue(rgb);
	return a << 24 | b << 16 | g << 8 | r;
}

bool SetRegistryDWord(LPCWSTR key, LPCWSTR name, DWORD dword) {
	HKEY hkey;
	LSTATUS res = RegOpenKey(HKEY_CURRENT_USER, key, &hkey);

	if (res == ERROR_SUCCESS) {
		res = RegSetValueEx(hkey, name, 0, REG_DWORD, reinterpret_cast<BYTE*>(&dword), sizeof DWORD);
		RegCloseKey(hkey);
	}
	return res;
}