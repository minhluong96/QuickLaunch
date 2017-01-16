#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100

//Windows Aero theme
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Global Variables:
HINSTANCE hInst, hInstHook, hInstIcon;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND main, searchBox;
NOTIFYICONDATA nid = {};
HHOOK hHook;
int k = 0, state = 0;
std::vector<CApp*> app;
std::vector<CApp*> currentSearch;

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);
void LoadNIcon(HWND hwnd);
void LoadFile(WCHAR* path);
void LoadSearchBox();
void Excute();
void ClearItem();
void SortData();
void LoadData();
void SaveData();