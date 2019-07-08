#include "pch.h"
#include "CSoftware.h"

//64位安装路径
#define  X86_SUBKEY  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
//32位安装路径
#define  X64_SUBKEY _T("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall")



CSoftware::CSoftware()
{
}


CSoftware::~CSoftware()
{
}

bool CSoftware::GetSoftwareList(VSoftAll * vSoftInfos)
{
	//1.主键位置
	HKEY RootKey = HKEY_LOCAL_MACHINE;
	//接收将要打开的键句柄
	HKEY hkResult = 0;


	//2.1默认64位系统子键位置
	LPCTSTR  lpSubKey = X64_SUBKEY;
	//2.2检测系统位数
	if (!Is64BitSystem())
		lpSubKey = X86_SUBKEY;

	//3.打开注册表键
	LONG lReturn = RegOpenKeyEx(
		RootKey,
		lpSubKey,
		0,
		KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,
		&hkResult
	);

	//索引下标
	DWORD dwIndex = 0;

	//4.循环遍历Uninstall目录下的子键
	while (TRUE)
	{
		CSoftwareInfo info;
		DWORD dwkeyLen = 255;
		//注册表名称
		WCHAR szNewKeyName[MAX_PATH] = {};
		//枚举注册表名称
		LONG lReturn = RegEnumKeyEx(
			hkResult,									//	子键句柄
			dwIndex,									//子建的索引
			szNewKeyName,						//指向保存子建名称
			&dwkeyLen,								//子建名称长度
			0, NULL, NULL, NULL);
		if (lReturn != ERROR_SUCCESS)
		{
			break;
		}


		//5.组合子建成完整名称
		WCHAR strMidReg[MAX_PATH] = {};
		swprintf_s(strMidReg, L"%s%s%s", lpSubKey, L"\\", szNewKeyName);

		//6.打开新子键 获得句柄
		HKEY hkValueKey = 0;
		RegOpenKeyEx(RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);
		if (hkValueKey == NULL)
		{
			dwIndex++;
			continue;
		}

		//7.获取键值
		DWORD dwNameLen = 0;
		DWORD dwType = 0;
		//8.获取软件名字
		TCHAR szBuff[260] = {};
		lReturn = RegQueryValueEx(hkValueKey, L"DisplayName", 0, 0, 0, &dwNameLen);
		if (lReturn == ERROR_SUCCESS) {
			RegQueryValueEx(hkValueKey, L"DisplayName", 0, &dwType, (PBYTE)szBuff, &dwNameLen);
			info.m_Name = szBuff;
		}
		else {
			info.m_Name = szNewKeyName;
		}
		//卸载路径
		dwType = 0;
		RegQueryValueEx(hkValueKey, L"Uninstallstring", 0, 0, 0, &dwNameLen);
		if (dwNameLen != 0) {
		RegQueryValueEx(hkValueKey, L"Uninstallstring", 0, &dwType, (LPBYTE)szBuff, &dwNameLen);
		info.m_SoftUniPath = szBuff;
	}

		//光标路径
		dwType = 0;
		RegQueryValueEx(hkValueKey, L"DisplayIcon", 0, 0, 0, &dwNameLen);
		if (dwNameLen != 0) {
			RegQueryValueEx(hkValueKey, L"DisplayIcon", 0, &dwType, (LPBYTE)szBuff, &dwNameLen);
			info.m_SoftIconPath = szBuff;
		}

		dwType = 0;
		RegQueryValueEx(hkValueKey, L"DisplayVersion", 0,0,0, &dwNameLen);
		if (dwNameLen != 0) {
			RegQueryValueEx(hkValueKey, L"DisplayVersion", 0, &dwType, (LPBYTE)szBuff, &dwNameLen);//版本号
			info.m_SoftVer = szBuff;
		}

		dwType = 0;
		RegQueryValueEx(hkValueKey, L"InstallDate", 0, 0, 0, &dwNameLen);
		if (dwNameLen != 0) {
			RegQueryValueEx(hkValueKey, L"InstallDate", 0, &dwType, (LPBYTE)szBuff, &dwNameLen);//安装日期
			info.m_SoftDate = szBuff;
		}

		dwType = 0;
		RegQueryValueEx(hkValueKey, L"URLInfoAbout", 0,0,0, &dwNameLen);
		if (dwNameLen != 0) {
			RegQueryValueEx(hkValueKey, L"URLInfoAbout", 0, &dwType, (LPBYTE)szBuff, &dwNameLen);//网址
			info.m_SoftUrl = szBuff;
		}

		dwType = 0;
		RegQueryValueEx(hkValueKey, L"InstallLocation", 0,0,0, &dwNameLen);
		if (dwNameLen != 0) {
			RegQueryValueEx(hkValueKey, L"InstallLocation", 0, &dwType, (LPBYTE)szBuff, &dwNameLen);//安装路径
			info.m_SoftInsPath = szBuff;
		}
		//遍历下一个键
		dwIndex++;
		RegCloseKey(hkValueKey);
		vSoftInfos->push_back(info);

	}

	RegCloseKey(hkResult);

	return false;
}

bool CSoftware::UnInstallSoftware(CString cSoftUnPath)
{
	ShellExecute(NULL, L"open", cSoftUnPath, 0, 0, SW_SHOW);
	return false;
}

bool CSoftware::ShowSoftwareDir(CString cPath)
{
	ShellExecute(NULL, L"explore", cPath, 0, 0, SW_SHOW);
	return false;
}

BOOL CSoftware::Is64BitSystem()
{

	SYSTEM_INFO si = { 0 };
	typedef void (WINAPI *LPFN_PGNSI)(LPSYSTEM_INFO);
	LPFN_PGNSI pGNSI = (LPFN_PGNSI)GetProcAddress(GetModuleHandleA(("kernel32.dll")), "GetNativeSystemInfo");
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
			si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		{
			return TRUE;
		}
		return FALSE;

	return 0;
}
