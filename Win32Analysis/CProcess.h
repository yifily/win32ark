#pragma once
#include <Windows.h>
#include <vector>
#include <atlstr.h>
#include "UnDocFunation.h"
#include<TlHelp32.h>
using std::vector;


/*进程信息结构体*/
class PSInfo {
public:
	CString m_Path;
	CString m_Name;
	CString m_Pid;
	CString m_Ppid;
	CString m_Cmdline;
	CString m_Peb;
	
};

/*重定义返回类型*/
typedef vector<PSInfo>  VPsAll;

class CProcess
{
	
public:
	/*获取进程信息*/
	bool GetProcessInfo(VPsAll *psinfo);
	/*获取进程命令行*/
	CString GetProcessCmdline(HANDLE hProcess);
	/*获取进程PEB*/
	CString GetProcessPeb(HANDLE hProcess);


};

