#pragma once
#include <Windows.h>
#include <atlstr.h>
#include <vector>
using std::vector;
#include"tchar.h"

class CPrivilegeInfo {
public:
	CString m_Name;
	CString m_Status;
};

/*重定义返回类型*/
typedef vector<CPrivilegeInfo>  VPrivilegeAll;



class CPrivilege
{
public:
	CPrivilege();
	~CPrivilege();

	/*遍历权限*/
	BOOL EnumPrivileges(VPrivilegeAll *PrivilegeInfos,HANDLE hanlde = INVALID_HANDLE_VALUE);

	/*权限查询 是否拥有管理员所有权限*/
	BOOL QueryPrivileges(HANDLE hanlde = INVALID_HANDLE_VALUE);

	/*提升权限*/
	BOOL EnablePrivileges(BOOL Enable,const TCHAR *szPrivilege, HANDLE hanlde = INVALID_HANDLE_VALUE);

	/*管理员启动*/
	BOOL StartAdminRun(TCHAR *szPath);

	/*添加管理员图标*/
	BOOL AddAdminIcon(HWND hWnd, DWORD dwId);

	/*管理员运行拖拽*/
	BOOL AddAdminDrag();



};

