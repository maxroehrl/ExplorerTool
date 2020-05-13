#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Global variables
HINSTANCE hInst;
LPCWSTR szWindowClass = L"Explorer Tool";
HWND colorButton;

// Functions
void InitInstance(HWND);
LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
void PickColor(HWND hWnd);
COLORREF RGB2BGR(const COLORREF rgb, const bool isOpaque = false);
bool SetRegistryDWord(LPCWSTR key, LPCWSTR name, DWORD dword);