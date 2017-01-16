#include "stdafx.h"
#include "App.h"


CApp::CApp()
{
}

CApp::CApp(WCHAR* Name, WCHAR* Path, int Seq)
{
	wcscpy_s(displayName, MAX_PATH, Name);
	wcscpy_s(path, MAX_PATH, Path);
	seq = Seq;
}



WCHAR* CApp::GetName()
{
	return displayName;
}

WCHAR * CApp::GetPath()
{
	return path;
}

int CApp::GetSeq()
{
	return seq;
}

void CApp::SetSeq(int Seq)
{
	seq = Seq;
}

void CApp::IncSeq()
{
	seq++;
}



void CApp::Execute()
{
	//set info to execute
	SHELLEXECUTEINFO info = { 0 };
	info.cbSize = sizeof(SHELLEXECUTEINFO);
	info.fMask = 0;
	info.hwnd = GetDesktopWindow();
	info.lpVerb = NULL;
	info.lpFile = path;
	info.lpParameters = _T("");
	info.nShow = SW_SHOW;
	info.lpDirectory = NULL;
	info.hInstApp = NULL;

	ShellExecuteEx(&info);
}

CApp::~CApp()
{
}
