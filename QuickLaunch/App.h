#pragma once
class CApp
{
private:
	WCHAR displayName[MAX_PATH];
	WCHAR path[MAX_PATH];
	int seq;
public:
	CApp();
	CApp(WCHAR* Name, WCHAR* Path, int Seq);
	WCHAR* GetName();
	WCHAR* GetPath();
	int GetSeq();
	void SetSeq(int Seq);
	void IncSeq();
	void Execute();
	~CApp();
};

