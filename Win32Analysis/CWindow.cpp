#include "pch.h"
#include "CWindow.h"


CWindow::CWindow()
{
}


CWindow::~CWindow()
{
}

bool CWindow::GetWindowInfo(VWndAll & WndInfos)
{
	/*Ã¶¾Ù´°¿Ú*/
	EnumWindows(CWindow::EnumWindowsProc, (LPARAM)&WndInfos);
	Sleep(10);
	return false;
}

BOOL CWindow::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	VWndAll * WndInfos = (VWndAll*)lParam;


	WndInfo Winfo;
	TCHAR szTitle[MAX_PATH] = { 0 };
	TCHAR szClass[MAX_PATH] = { 0 };
	int nMaxCount = MAX_PATH;
	

	GetWindowText(hwnd, szTitle, nMaxCount);
	GetClassName(hwnd, szClass, nMaxCount);
	Winfo.m_Name = szTitle;
	Winfo.m_ClassName = szClass;
	
	
	WndInfos->push_back(Winfo);
	//EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&WndInfos[WndInfos->size()-1]);

	return 1;
}


BOOL CWindow::EnumChildProc(HWND hwnd, LPARAM lParam)
{
	VWndAll * WndInfos = (VWndAll*)lParam;


	WndInfo Winfo;
	TCHAR szTitle[MAX_PATH] = { 0 };
	TCHAR szClass[MAX_PATH] = { 0 };
	int nMaxCount = MAX_PATH;


	GetWindowText(hwnd, szTitle, nMaxCount);
	GetClassName(hwnd, szClass, nMaxCount);
	Winfo.m_Name = szTitle;
	Winfo.m_ClassName = szClass;

	WndInfos->push_back(Winfo);
	EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&WndInfos[WndInfos->size() - 1]);
	return 1;
}

