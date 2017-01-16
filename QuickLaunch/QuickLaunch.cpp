// QuickLaunch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QuickLaunch.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUICKLAUNCH, szWindowClass, MAX_LOADSTRING);
	    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKLAUNCH));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKLAUNCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUICKLAUNCH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   main = CreateWindowEx(0, szWindowClass, szTitle ,WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,
      500, 200, 400, 300, NULL, NULL, hInstance, NULL);

   if (!main)
   {
      return FALSE;
   }

   //ShowWindow(main, nCmdShow);
   UpdateWindow(main);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
		//Get System Font
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);

		LoadNIcon(hWnd);

		ShowWindow(hWnd, SW_HIDE);
		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyHookProc, hInstHook, 0);

		searchBox = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | CBS_AUTOHSCROLL, 50, 30, 300, 30, hWnd, (HMENU)IDCB_SEARCH, hInst, NULL);
		SendMessage(searchBox, WM_SETFONT, WPARAM(hFont), TRUE);
		LoadFile(L"D:\\Program Files (x86)\\");
		LoadFile(L"D:\\Program Files\\");
		LoadFile(L"C:\\Program Files (x86)\\Google\\");
		LoadData();
		LoadSearchBox();
		}
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            
			case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			
			case IDCB_SEARCH: // combobox text changed event
			{
				if (HIWORD(wParam) == CBN_EDITUPDATE)
				{
					LoadSearchBox();
				}
			}break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    
	case WM_DESTROY:
		SaveData();
        PostQuitMessage(0);
        break;
	
	case APPWM_ICONNOTIFY:
		{
		switch (lParam)
		{
		case WM_LBUTTONUP:
			{
				if (0 == state)
				{
					state = 1;
					ShowWindow(main, SW_SHOW);
					SetFocus(searchBox);
				}
				else
				{
					state = 0;
					ShowWindow(main, SW_HIDE);
				}
			}
			break;
		case WM_RBUTTONUP:
			{
			HMENU NotifyMenu = CreatePopupMenu();
			AppendMenu(NotifyMenu, MF_STRING, IDM_EXIT, L"Exit");
			
			//Get Position of Notify Icon to popup the menu
			POINT pCursor;
			GetCursorPos(&pCursor);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(NotifyMenu, TPM_RIGHTBUTTON, pCursor.x, pCursor.y, 0, hWnd, NULL);
			PostMessage(hWnd, WM_NULL, 0, 0);
			}
			break;
		}
		}break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	}
	else
	{
		if ((GetAsyncKeyState(VK_SPACE) & 0x0001) && (GetAsyncKeyState(VK_LCONTROL) & (0x8000)) && 0 == k)
		{
			k = 1;
			if (0 == state)
			{
				state = 1;
				ShowWindow(main, SW_SHOW);
				SetFocus(searchBox);
				return TRUE;
			}
			else
			{
				state = 0;
				ShowWindow(main, SW_HIDE);
				return TRUE;
			}
		}
		else
		{
			if (GetAsyncKeyState(VK_RETURN) & 0x0001)
			{
				if (1 == state)
				{
					Excute();
					state = 0;
					ShowWindow(main, SW_HIDE);
				}
			}
			else
			{
				k = 0;
				return CallNextHookEx(hHook, nCode, wParam, lParam);
			}
		}
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void LoadNIcon(HWND hwnd)//load notify icon
{
	NOTIFYICONDATA nid = {};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = APPWM_ICONNOTIFY;

	wcscpy_s(nid.szTip, 20, L"Quick Launch");
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void LoadFile(WCHAR* path)
{
	LPWIN32_FIND_DATAW data = new WIN32_FIND_DATA();
	//find folder

	WCHAR tempPath[MAX_PATH];
	wcscpy_s(tempPath, MAX_PATH, path);
	wcscat_s(tempPath, MAX_PATH, L"\\*");
	HANDLE findFile = FindFirstFile(tempPath, data);
	do 
	{
		if (data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && wcscmp(data->cFileName,L".") != 0 && wcscmp(data->cFileName, L"..") != 0)
		{
			WCHAR folderPath[MAX_PATH];
			wcscpy_s(folderPath, MAX_PATH, path);
			wcscat_s(folderPath, MAX_PATH, data->cFileName);
			wcscat_s(folderPath, MAX_PATH, L"\\");
			LoadFile(folderPath);
		}
	} while (FindNextFile(findFile, data));

	// find *.exe
	data = new WIN32_FIND_DATA();
	WCHAR exePath[MAX_PATH];
	wcscpy_s(exePath, MAX_PATH, path);
	wcscat_s(exePath, MAX_PATH, L"*.exe");
	findFile = FindFirstFile(exePath, data);
	do
	{
		if (wcscmp(data->cFileName, L"") != 0)
		{
			CApp *temp;
			WCHAR fullPath[MAX_PATH], name[MAX_PATH];
			wcscpy_s(fullPath, MAX_PATH, path);
			wcscat_s(fullPath, MAX_PATH, L"\\");
			wcscat_s(fullPath, MAX_PATH, data->cFileName);

			wcscpy_s(name, MAX_PATH, data->cFileName);
			name[wcslen(name) - 4] = '\0';
			temp = new CApp(name, fullPath, 0);
			app.push_back(temp);
		}
	} while (FindNextFile(findFile, data));	
}

void ClearItem()// clear item of the searchbox
{
	int count = SendMessage(searchBox, CB_GETCOUNT, 0, 0);
	for (int i = count - 1; i >= 0; i--)
	{
		SendMessage(searchBox, CB_DELETESTRING, i, 0);
	}
}

void LoadSearchBox() //load the search everytime the box text changed
{
	currentSearch.clear();
	int size = GetWindowTextLength(searchBox);
	WCHAR *fileName = new WCHAR[size + 1];
	GetWindowText(searchBox, fileName, size + 1);
	std::wstring search(fileName);
	std::transform(search.begin(), search.end(), search.begin(), ::tolower);
	ClearItem();
	
	//load app into tempApp -- tempApp is a temp list for search
	for (int i = 0; i < app.size(); i++)
	{
		std::wstring temp(app[i]->GetName());
		std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
		if (-1 != temp.find(search))
		{
			CApp *tempApp = new CApp(app[i]->GetName(), app[i]->GetPath(), 0);
			currentSearch.push_back(tempApp);
		}
	}
	

	//load tempApp into searchBox
	for (int i = 0; i < currentSearch.size() && i < 10; i++)
	{
		SendMessage(searchBox, CB_ADDSTRING, 0, LPARAM(currentSearch[i]->GetName()));
	}
	if(state == 1)
	SendMessage(searchBox, CB_SHOWDROPDOWN, TRUE, 0);
}

void Excute() // excute the program
{
	int size = GetWindowTextLength(searchBox);
	WCHAR *fileName = new WCHAR[size + 1];
	GetWindowText(searchBox, fileName, size + 1);

	if(wcscmp(L"", fileName) != 0)
	{
		for (int i = 0; i < app.size(); i++)
		{
			if (wcscmp(app[i]->GetName(), fileName) == 0)
			{
				app[i]->IncSeq();
				app[i]->Execute();
				SortData();
			}
		}
	}
}

struct moreSeq
{
	inline bool operator() (CApp *app1, CApp *app2)
	{
		return (app1->GetSeq() > app2->GetSeq());
	}
};

void SortData()
{
	std::sort(app.begin(), app.end(), moreSeq());
}

int CountData()
{
	int count = 0;
	for (int i = 0; i < app.size(); i++)
	{
		if (app[i]->GetSeq() > 0)
			count++;
		else
			break;
	}
	return count;
}

void SaveData()
{
	int count = CountData();

	WCHAR *path = new WCHAR[MAX_PATH];
	WCHAR *buff = new WCHAR[MAX_PATH];
	swprintf(buff, MAX_PATH, L"%d", count);

	GetCurrentDirectory(MAX_PATH, path);
	wcscat_s(path, MAX_PATH, L"\\data.ini");
	WritePrivateProfileString(L"data", L"count", buff, path);

	for (int i = 0; i < count; i++)
	{
		WCHAR *seq = new WCHAR[MAX_PATH];
		swprintf(buff, MAX_PATH, L"%d", i);
		swprintf(seq, MAX_PATH, L"%d", app[i]->GetSeq());
		WritePrivateProfileString(buff, L"Name", app[i]->GetName(), path);
		WritePrivateProfileString(buff, L"Seq", seq, path);
	}
}

void LoadData()
{
	WCHAR *path = new WCHAR[MAX_PATH];
	WCHAR *buff = new WCHAR[MAX_PATH];
	int count;

	//get path
	GetCurrentDirectory(MAX_PATH, path);
	wcscat_s(path, MAX_PATH, L"\\data.ini");

	//get count
	GetPrivateProfileString(L"data", L"count", L"0", buff, MAX_PATH, path);
	count = _wtoi(buff);

	//update seq
	for (int i = 0; i < count; i++)
	{
		WCHAR *seq = new WCHAR[MAX_PATH];
		WCHAR *name = new WCHAR[MAX_PATH];
		swprintf(buff, MAX_PATH, L"%d", i);
		GetPrivateProfileString(buff, L"Name", L"Unknown", name, MAX_PATH, path);
		GetPrivateProfileString(buff, L"Seq", L"0", seq, MAX_PATH, path);
		for (int i = 0; i < app.size(); i++)
		{
			if (wcscmp(app[i]->GetName(), name) == 0)
			{
				app[i]->SetSeq(_wtoi(seq));
			}
		}
	}

	SortData();
}

