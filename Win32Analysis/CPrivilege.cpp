#include "pch.h"
#include "CPrivilege.h"
#include <Shlobj.h>
#pragma comment(lib,"Shell32.lib")


CPrivilege::CPrivilege()
{
}


CPrivilege::~CPrivilege()
{
}

BOOL CPrivilege::EnumPrivileges(VPrivilegeAll *PrivilegeInfos, HANDLE hanlde)
{
	//获取进程令牌句柄
	HANDLE hToken;
	CPrivilegeInfo info;
	OpenProcessToken(hanlde, TOKEN_QUERY, &hToken);
	if (!hToken)
	{
		return FALSE;
	}
	//查询令牌中的权限
	DWORD dwSize;
	//第一次获取查询数据的大小
	GetTokenInformation(hToken,
		TokenPrivileges, NULL, NULL, &dwSize);
	//第二次根据上次获取的大小申请足够大的内存后，
	//将内存地址传进去获取想要的数据
	char* pBuf = new char[dwSize] {};
	GetTokenInformation(hToken,
		TokenPrivileges, pBuf, dwSize, &dwSize);
	//将内存中的内容用要查询数据结构体解析
	//这里事权限的结构体
	TOKEN_PRIVILEGES* pTp = (TOKEN_PRIVILEGES*)pBuf;

	//权限个数
	DWORD dwCount = pTp->PrivilegeCount;
	//指向权限数组首地址
	LUID_AND_ATTRIBUTES* pLaa = pTp->Privileges;
	//输出权限
	for (int i = 0; i < dwCount; i++, pLaa++)
	{
		TCHAR szName[100] = {};
		DWORD dwLen = sizeof(szName);
		//查询此权限对应的字符串
		LookupPrivilegeName(0, &pLaa->Luid, szName, &dwLen);
		//状态,0:关闭，1：默认值，2:开启，3：默认开启 
		//printf("[%s] -- 状态[%d]\n", szName, pLaa->Attributes);
		info.m_Name = szName;
		info.m_Status.Format(L"%d", pLaa->Attributes);
		PrivilegeInfos->push_back(info);
	}
	delete pBuf;
	return TRUE;
}

BOOL CPrivilege::QueryPrivileges(HANDLE hanlde)
{
	// 1. 获得本进程的令牌
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return false;
	// 2. 获取提升类型
	TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;
	BOOL                 bIsAdmin = false;
	DWORD                dwSize = 0;


	if (GetTokenInformation(hToken, TokenElevationType, &ElevationType,
		sizeof(TOKEN_ELEVATION_TYPE), &dwSize)) {
		// 2.1 创建管理员组的对应SID
		BYTE adminSID[SECURITY_MAX_SID_SIZE];
		dwSize = sizeof(adminSID);
		CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);


		// 2.2 判断当前进程运行用户角色是否为管理员
		if (ElevationType == TokenElevationTypeLimited) {

			// a. 获取连接令牌的句柄
			HANDLE hUnfilteredToken = NULL;
			GetTokenInformation(hToken, TokenLinkedToken, (PVOID)&hUnfilteredToken,
				sizeof(HANDLE), &dwSize);

			// b. 检查这个原始的令牌是否包含管理员的SID
			if (!CheckTokenMembership(hUnfilteredToken, &adminSID, &bIsAdmin))
				return false;
			CloseHandle(hUnfilteredToken);
		}
		else {
			bIsAdmin = IsUserAnAdmin();
		}
		CloseHandle(hToken);
	}
	// 3. 判断具体的权限状况
	BOOL bFullToken = false;
	switch (ElevationType) {
	case TokenElevationTypeDefault: /* 默认的用户或UAC被禁用 */
		if (IsUserAnAdmin())  bFullToken = true; // 默认用户有管理员权限
		else                  bFullToken = false;// 默认用户不是管理员组
		break;
	case TokenElevationTypeFull:    /* 已经成功提高进程权限 */
		if (IsUserAnAdmin())  bFullToken = true; //当前以管理员权限运行
		else                  bFullToken = false;//当前未以管理员权限运行
		break;
	case TokenElevationTypeLimited: /* 进程在以有限的权限运行 */
		if (bIsAdmin)  bFullToken = false;//用户有管理员权限，但进程权限有限
		else           bFullToken = false;//用户不是管理员组，且进程权限有限
	}
	return bFullToken;
}

BOOL CPrivilege::EnablePrivileges(BOOL Enable,const TCHAR * szPrivilege, HANDLE hanlde)
{
	BOOL fOk = FALSE;    
	HANDLE hToken;
	// 以修改权限的方式，打开进程的令牌
	if (OpenProcessToken(hanlde, TOKEN_ADJUST_PRIVILEGES,
		&hToken)) {
		// 令牌权限结构体
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		//获得LUID
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		//启动或者关闭
		tp.Privileges[0].Attributes = Enable ? SE_PRIVILEGE_ENABLED : 0;
		//修改权限
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		fOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return(fOk);
}

BOOL CPrivilege::StartAdminRun(TCHAR *szPath)
{
	//  以管理员权限重新打开进程
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = L"runas";      // 请求提升权限
	sei.lpFile = szPath;		// 可执行文件路径
	sei.lpParameters = NULL;          // 不需要参数
	sei.nShow = SW_SHOWNORMAL; // 正常显示窗口
	return ShellExecuteEx(&sei);
}

BOOL CPrivilege::AddAdminIcon(HWND hWnd, DWORD dwId)
{
	Button_SetElevationRequiredState(::GetDlgItem(hWnd, dwId), 1);
	return 0;
}

BOOL CPrivilege::AddAdminDrag()
{
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);
	return 1;
}
