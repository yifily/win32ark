// Win32Analysis.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "CProcess.h"
#include "CThread.h"
#include "CModule.h"
#include "CPe.h"
#include "CWindow.h"
#include "CInject.h"
#include "CServer.h"
#include "CPrivilege.h"

int main()
{
	setlocale(LC_ALL, "chs");

	//1.获取进程信息
	//CProcess ps;
	//VPsAll psinfo;
	//ps.GetProcessInfo(&psinfo);
	//for (auto i : psinfo)
	//{
	//	wprintf_s(L"%s\n", i.m_Name.GetBuffer());
	//	wprintf_s(L"\t%s\n", i.m_Pid.GetBuffer());
	//	wprintf_s(L"\t%s\n", i.m_Cmdline.GetBuffer());
	//}

	//2.获线程信息
	//CThread ps;
	//VThAll thinfo;
	//ps.GetThreadInfo(&thinfo,38396);
	//for (auto i : thinfo)
	//{
	//	wprintf_s(L"%s\n", i.m_Tid.GetBuffer());
	//	wprintf_s(L"\t%s\n", i.m_Start.GetBuffer());
	//	wprintf_s(L"\t%s\n", i.m_Teb.GetBuffer());
	//}

	//3.获取模块信息
	//CModule  md;
	//VModAll mdinfos;
	//md.GetModuleInfo(&mdinfos,38396);
	//for (auto i : mdinfos)
	//{
	//	wprintf_s(L"%s\n", i.m_Name.GetBuffer());
	//	wprintf_s(L"\t%s\n", i.m_BaseImage.GetBuffer());
	//	wprintf_s(L"\t%s\n", i.m_Entry.GetBuffer());
	//}

	//4.解析PE信息
	//CPe pe((TCHAR*)L"..\\Debug\\Win32Analysis.exe");
	//vector<CPe::IATInfo> vec_info;
	//pe.GetIATInfo(vec_info);

	//for (auto i : vec_info)
	//{
	//	printf("%s\n", i.dllName);
	//	for (auto x : i.vec_item)
	//	{
	//		printf("	%s\n", x.FunName);
	//	}
	//}

	//5.窗口遍历
	//CWindow wnd;
	//VWndAll info;
	//wnd.GetWindowInfo(info);
	//for (auto x : info)
	//{
	//	wprintf(L"%s\n", x.m_Name);
	//}

	//6.注入工具
	//指令注入
	//int a = (int)MessageBox;
	//char buff[200];
	//sprintf_s(buff, "push 0;push 0;push 0;push 0;call 0x%08X;", a);
	//CInject inject;
	//inject.InitInjectObj(18372, CInject::Inject_Asm, CInject::Mode_Remote);
	//inject.StartInject(buff,0);

	//dll注入
	//CInject inject;
	//inject.InitInjectObj(42572, CInject::Inject_Dll, CInject::Mode_Remote);
	//char path[] = "C:\\Users\\42140\\source\\repos\\Win32Analysis\\Win32Analysis\\keystone.dll";
	//inject.StartInject(path, sizeof(path));

	//7.服务信息

	//CServer Server;

	//Server.StartTheServer((TCHAR*)L"XblGameSave", 0);
	//Server.StopTheServer((TCHAR*)L"XblGameSave", 0);

	//VServerAll vServerList;
	//Server.GetServerList(&vServerList);
	//for (auto i : vServerList)
	//{

	//	wprintf_s(L"%s\n", i.m_Name);
	//	wprintf_s(L"\t%s\n", i.m_DisplayName);
	//	wprintf_s(L"\t%s\n", i.m_Status);
	//}

	//8权限
	//VPrivilegeAll info;
	//CPrivilege Privliege;
	//HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE /*20488*/);


	//提升权限
	//Privliege.EnablePrivileges(TRUE,SE_DEBUG_NAME);

	//遍历权限
	//Privliege.EnumPrivileges(&info);
	//for (auto i : info)
	//{
	//	wprintf_s(L"%s [%s]\n", i.m_Name, i.m_Status);
	//}
	


}
