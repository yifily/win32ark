#pragma once
#include<atlstr.h>
#include <vector>
using std::vector;


class WndInfo{
public:
	CString m_Name;
	CString m_ClassName;
	bool m_IsShowWnd;
	vector<WndInfo> m_ChildWnds;
};
typedef vector<WndInfo> VWndAll;




class CWindow
{
public:
	CWindow();
	~CWindow();

	/*获取窗口信息*/
	bool GetWindowInfo(VWndAll& WndInfos);

	/*枚举子窗口回调函数*/
	static BOOL CALLBACK EnumChildProc( HWND   hwnd,  LPARAM lParam);

	/*枚举顶级窗口回调函数*/
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);



};

