#include "pch.h"
#include "CModule.h"
#include <Psapi.h>
#include <shlwapi.h>
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"shlwapi.lib")


CModule::CModule()
{
}


CModule::~CModule()
{
}

bool CModule::GetModuleInfo(VModAll * ModuleInfos, DWORD dwPid)
{
	HMODULE Modlist[1000];
	DWORD nSize = 0;
	wchar_t fileName[260] = { 0 };
	MODULEINFO mdinfo = {};

	/*打开进程*/
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

	if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
	{
		/*获取当前进程模块列表*/
		EnumProcessModules(hProcess, 0,0, &nSize);
		EnumProcessModules(hProcess, Modlist, nSize, &nSize);
		/*遍历模块信息*/
		for (int i = 0; i < nSize / sizeof(HMODULE); i++)
		{
			ModuleInfo mdInfo;
			/*获取模块基本信息*/
			GetModuleFileNameEx(hProcess, Modlist[i], fileName, sizeof(fileName));


			//dll名字
			mdInfo.m_Name = PathFindFileName(fileName);
			//路径名
			mdInfo.m_Path = fileName;
			//镜像基址大小入口地址
			GetModuleInformation(hProcess, Modlist[i], &mdinfo, sizeof(mdinfo));
			mdInfo.m_BaseImage.Format(L"0X%08X",mdinfo.lpBaseOfDll);
			mdInfo.m_Size.Format(L"0X%08X", mdinfo.SizeOfImage);
			mdInfo.m_Entry.Format(L"0X%08X", mdinfo.EntryPoint);

			//保存到列表中
			ModuleInfos->push_back(mdInfo);
		}

	}
	CloseHandle(hProcess);


	return false;
}
