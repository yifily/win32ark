#include "pch.h"

#include "CServer.h"


CServer::CServer()
{
}


CServer::~CServer()
{
}

/*获取服务列表*/
bool CServer::GetServerList(VServerAll * vServerInfos)
{
	//1.打开服务管理器，本地服务，默认数据库，权限所有
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用 需要内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;

	EnumServicesStatusEx(
		hSCM,										//服务句柄
		SC_ENUM_PROCESS_INFO,		//返回的属性 默认的都是这个值
		SERVICE_WIN32,						//服务类型   应用程序的服务
		SERVICE_STATE_ALL,				//服务状态，所有
		NULL,										//缓冲区
		0,												//缓冲区大小
		&dwSize,									//需要的大小
		&dwServiceNum,						//缓冲区的服务个数
		NULL,										//默认0
		NULL										//默认0
	);
	//3 申请需要的内存大小
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];

	//4.第二次枚举
	BOOL bstatus = FALSE;
	bstatus = EnumServicesStatusEx(
		hSCM,										//服务句柄
		SC_ENUM_PROCESS_INFO,		//返回的属性 默认的都是这个值
		SERVICE_WIN32,						//服务类型   应用程序的服务
		SERVICE_STATE_ALL,				//服务状态，所有
		(PBYTE)pEnumSerice,				//缓冲区
		dwSize,									//缓冲区大小
		&dwSize,
		&dwServiceNum,
		NULL, NULL
	);

	//5遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{

		CServerInfo ServiceInfo;
		////获取基础信息
		////服务名
		//pEnumSerice[i].lpServiceName;
		////服务状态   状态：停止 运行  暂停
		//pEnumSerice[i].ServiceStatusProcess.dwCurrentState;
		////服务类型  类型：文件驱动服务 驱动服务 独立进程服务
		//pEnumSerice[i].ServiceStatusProcess.dwServiceType;


		//6.服务详细信息
			//打开服务
		SC_HANDLE hService = OpenService(hSCM,
			pEnumSerice[i].lpServiceName,		//服务名
			SERVICE_QUERY_CONFIG				//打开权限
		);
		//第一次1获取缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig =
			(LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//第二次调用
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//运行状态类型
		ServiceInfo.m_Status.Format(L"%s", GetServerStatus(pEnumSerice[i].ServiceStatusProcess.dwCurrentState));
		//服务类型类型
		ServiceInfo.m_StartType.Format(L"%s", GetServerStartType(pServiceConfig->dwStartType));

		ServiceInfo.m_Path.Format(L"%s", pServiceConfig->lpBinaryPathName);							//服务全路经

		ServiceInfo.m_DisplayName.Format(L"%s", pServiceConfig->lpDisplayName);										//服务显示名

		ServiceInfo.m_StartName.Format(L"%s", pServiceConfig->lpServiceStartName);							//服务启动名

		ServiceInfo.m_Name.Format(L"%s", pEnumSerice[i].lpServiceName);											//创建的服务名

		vServerInfos->push_back(ServiceInfo);
		CloseServiceHandle(hService);
	}

	//关闭服务进程
	CloseServiceHandle(hSCM);

	return true;
}

const TCHAR * CServer::GetServerStartType(DWORD StartGen)
{
	switch (StartGen)
	{
	case SERVICE_AUTO_START:
		return _T("服务控制管理器在系统启动时自动启动服务");
	case SERVICE_BOOT_START:
		return _T("由系统装载程序启动的设备驱动程序,此值仅用于驱动程序服务");
	case SERVICE_DEMAND_START:
		return _T("服务开始由服务控制管理器,当一个进程调用startService函数");
	case SERVICE_DISABLED:
		return _T("不能启动的服务.尝试启动服务,导致错误的代码error_service_disabled");
	default:
		break;
	}
	return _T("由ioinitsystem功能启动设备驱动程序。此值仅用于驱动程序服务");
}

const TCHAR * CServer::GetServerStatus(DWORD ServerGen)
{
	switch (ServerGen)
	{
	case SERVICE_CONTINUE_PENDING:
		return _T("继续挂起");
	case SERVICE_PAUSE_PENDING:
		return _T("暂停挂起");
	case SERVICE_PAUSED:
		return _T("暂停");
	case SERVICE_RUNNING:
		return _T("正在运行");
	case SERVICE_START_PENDING:
		return _T("正在启动");
	case SERVICE_STOP_PENDING:
		return _T("正在停止");
	default:
		break;
	}
	return _T("未运行");
}

const TCHAR * CServer::GetServerType(DWORD GenreNow)
{
	switch (GenreNow)
	{
	case SERVICE_FILE_SYSTEM_DRIVER:
		return _T("文件系统驱动程序");
	case SERVICE_KERNEL_DRIVER:
		return _T("设备驱动程序");
	case SERVICE_WIN32_OWN_PROCESS:
		return _T("该服务运行在它自己的过程中");
	default:
		break;
	}
	return _T("该服务与其他服务共享一个进程");
}


bool CServer::CreateTheServer(TCHAR * szName, TCHAR * szDispalyName, TCHAR * szPath, DWORD StartType, DWORD ServiceType, SC_HANDLE hSCManager)
{
	if (hSCManager == NULL)
	{
		 hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	}
	DWORD ErrorControl = 0;
	if (hSCManager != NULL) {
		SC_HANDLE hService = CreateService(hSCManager, szName, szDispalyName, SERVICE_ALL_ACCESS, ServiceType, StartType, ErrorControl, szPath, NULL, NULL, NULL, NULL, TEXT(""));
		if (hService != NULL) {
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			return TRUE;
		}
		CloseServiceHandle(hSCManager);
		return FALSE;
	}
	else return FALSE;
}

bool CServer::DeleteTheServer(TCHAR * szName, SC_HANDLE hSCManager)
{
	if (hSCManager == NULL)
	{
		hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	}
	/*打开服务管理器*/

	if (hSCManager != NULL) {
		/*打开服务*/
		SC_HANDLE hService = OpenService(hSCManager, szName, DELETE);
		if (hService != NULL) {
			/*删除服务*/
			if (DeleteService(hService))
			{
				CloseServiceHandle(hService);
				CloseServiceHandle(hSCManager);
				return TRUE;
			}
			/*删除失败*/
			/*停止服务，再删除*/
			StopTheServer(szName, hSCManager);
			Sleep(500);
			DeleteService(hService);
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			return FALSE;
		}
		CloseServiceHandle(hSCManager);
		return FALSE;
	}
	else return FALSE;
}

bool CServer::StartTheServer(TCHAR * szName, SC_HANDLE hSCManager)
{
	if (hSCManager == NULL)
	{
		 hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	}
	if (hSCManager != NULL) {
		SC_HANDLE hService = OpenService(hSCManager, szName, SERVICE_START);
		if (hService != NULL) {
			if (StartService(hService, NULL, NULL))
			{
				CloseServiceHandle(hService);
				CloseServiceHandle(hSCManager);
				return TRUE;
			}
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			return FALSE;
		}
		CloseServiceHandle(hSCManager);
		return FALSE;
	}
	else return FALSE;
}

bool CServer::StopTheServer(TCHAR * szName, SC_HANDLE hSCManager)
{
	if (hSCManager == NULL)
	{
		 hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	}

	if (hSCManager != NULL) {
		SC_HANDLE hService = OpenService(hSCManager, szName, SERVICE_STOP);
		if (hService != NULL)
		{
			SERVICE_STATUS status;
			if (ControlService(hService, SERVICE_CONTROL_STOP, &status))
			{
				CloseServiceHandle(hService);
				CloseServiceHandle(hSCManager);
				return TRUE;
			}
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			return FALSE;
		}
		CloseServiceHandle(hSCManager);
		return FALSE;
	}
	else return FALSE;
}
