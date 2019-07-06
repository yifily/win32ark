#include "pch.h"
#include "CProcess.h"



/*获取进程信息*/
bool CProcess::GetProcessInfo(VPsAll* psinfo)
{
	//1创建进程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	//2临时保存信息
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//3遍历所有进程
	Process32First(hSnap, &pe);

	do
	{
		PSInfo ps;
		//保存进程信息
		ps.m_Name.Format(L"%s", pe.szExeFile);
		ps.m_Pid.Format(L"%8d",pe.th32ProcessID);
		ps.m_Ppid.Format(L"%8d", pe.th32ParentProcessID);


		//获取进程路径
		HANDLE hprocess = OpenProcess(
			PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ,
			FALSE, pe.th32ProcessID);
		//打开进程失败
		if (hprocess == INVALID_HANDLE_VALUE || hprocess == NULL)
		{
			//未知
			ps.m_Cmdline = "";
			ps.m_Path = "";
		}
		else {
			//获取命令行参数
			ps.m_Cmdline = GetProcessCmdline(hprocess);
			//获取进程路径
			wchar_t exepath[260] = {};
			DWORD dwSize = 260;
			QueryFullProcessImageNameW(hprocess, 0, exepath, &dwSize);
			ps.m_Path = exepath;

			//获取进程PEB
			ps.m_Peb = GetProcessPeb(hprocess);

		}
		psinfo->push_back(ps);
	} while (Process32Next(hSnap, &pe));

	CloseHandle(hSnap);



	return true;
}

/*获取进程命令行*/
CString CProcess::GetProcessCmdline(HANDLE hProcess)
{
	// 获取PEB
	PROCESS_BASIC_INFORMATION info;
	ULONG uSize;
	NtQueryInformationProcess(
		hProcess,
		ProcessBasicInformation,
		&info, sizeof(info),
		&uSize
	);

	LPVOID puser_addr;
	_RTL_USER_PROCESS_PARAMETERS user_info;
	DWORD dwSize;

	//读取进程用户信息地址
	LPVOID pAddr = (LPVOID)((int)info.PebBaseAddress + 0x10);
	if (info.PebBaseAddress == 0)
	{
		return "";
	}
	if (!ReadProcessMemory(hProcess, pAddr, &puser_addr, sizeof(puser_addr), &dwSize))
	{
		return "";
	}

	// 读取进程用户信息内容
	if (!ReadProcessMemory(hProcess, puser_addr, &user_info, sizeof(user_info), &dwSize))
	{
		return "";
	}


	//读取字符串
	wchar_t wPath[260] = {};

	pAddr = (LPVOID)((int)user_info.CommandLine.Buffer);
	if (!ReadProcessMemory(hProcess, pAddr, &wPath, 260, &dwSize))
	{
		return "";
	}

	//获取命令行
	return wPath;
}

CString CProcess::GetProcessPeb(HANDLE hProcess)
{
	// 获取PEB
	PROCESS_BASIC_INFORMATION info;
	ULONG uSize;
	NtQueryInformationProcess(
		hProcess,
		ProcessBasicInformation,
		&info, sizeof(info),
		&uSize
	);
	CString csPeb;
	csPeb.Format(L"%08X", info.PebBaseAddress);
	return csPeb;
}
