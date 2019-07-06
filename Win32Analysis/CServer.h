#pragma once
#include <atlstr.h>
#include <vector>
using std::vector;
#include"tchar.h"

class CServerInfo {
public:
	CString m_Name;
	CString m_StartName;
	CString m_Status;
	CString m_Type;
	CString m_StartType;
	CString m_DisplayName;
	CString m_Path;
};

/*重定义返回类型*/
typedef vector<CServerInfo>  VServerAll;

class CServer
{
public:
	CServer();
	~CServer();

	/*获取当前服务*/
	bool GetServerList(VServerAll *vServerInfos);
	/*获取启动类型*/
	const TCHAR* GetServerStartType(DWORD StartGen);
	/*获取当前服务状态*/
	const TCHAR* GetServerStatus(DWORD ServerGen);
	/*获取当前服务*/
	const TCHAR* GetServerType(DWORD GenreNow);


	/*创建服务*/
	bool CreateTheServer(
		TCHAR *szName,
		TCHAR *szDispalyName,
		TCHAR *szPath,
		DWORD StartType,
		DWORD ServiceType,
		SC_HANDLE hSCManager = 0);

	/*关闭服务*/
	bool DeleteTheServer(TCHAR *szName, SC_HANDLE hSCManager = 0);

	/*启动服务*/
	bool StartTheServer(TCHAR *szName, SC_HANDLE hSCManager = 0);

	/*停止服务*/
	bool StopTheServer(TCHAR *szName,SC_HANDLE hSCManager = 0);


};

