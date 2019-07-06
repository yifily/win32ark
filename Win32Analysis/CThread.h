#pragma once
#include<atlstr.h>
#include <vector>
using std::vector;
#include"UnDocFunation.h"



/*进程信息结构体*/
class ThInfo {
public:
	CString m_Tid;
	CString m_Start;
	CString m_Teb;
	CString m_Status;

};

/*重定义返回类型*/
typedef vector<ThInfo>  VThAll;

class CThread
{
public:
	CThread();
	~CThread();

	/*获取线程信息*/
	bool GetThreadInfo(VThAll * threadinfos, int pid);

	/*获取线程入口点*/
	DWORD GetThreadStartAddress(HANDLE hThread);

	/*获取线程Teb*/
	DWORD GetThreadTed(HANDLE hThread);

	/*挂起线程*/
	bool SuspendTheThread(DWORD dwTid);

	/*恢复线程*/
	bool ResumeTheThread(DWORD dwTid);

	/*获取当前状态 true 正在运行，false 挂起状态*/
	bool GetThreadStatus(DWORD dwTid);

};

