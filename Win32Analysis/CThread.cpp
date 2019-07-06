#include "pch.h"
#include "CThread.h"


CThread::CThread()
{
}


CThread::~CThread()
{
}

bool CThread::GetThreadInfo(VThAll * threadinfos, int pid)
{
	//1创建进程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	//2临时保存信息
	THREADENTRY32 th = { sizeof(THREADENTRY32) };
	//3遍历所有进程
	Thread32First(hSnap, &th);

	do {
		//遍历指定进程
		if (th.th32OwnerProcessID == pid)
		{
			ThInfo thInfo;
			thInfo.m_Tid.Format(L"%8d",th.th32ThreadID);

			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, th.th32ThreadID);
			if (hThread != INVALID_HANDLE_VALUE && hThread != NULL)
			{
				//获取线程入口点
				thInfo.m_Start.Format(L"0X%08X", GetThreadStartAddress(hThread));
				/*获取TEB*/
				thInfo.m_Teb.Format(L"0X%08X", GetThreadTed(hThread));
				/*获取状态*/
				if (GetThreadStatus(th.th32ThreadID))
					thInfo.m_Status = L"运行中";
				else
					thInfo.m_Status = L"挂起";

			}	
			threadinfos->push_back(thInfo);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hSnap, &th));



	return false;
}

DWORD CThread::GetThreadStartAddress(HANDLE hThread)
{
	/*调用未文档函数*/
	DWORD lpAddr;
	ULONG ulSize;
	/*获取线程入地址*/
	NtQueryInformationThread(
		hThread,
		(THREADINFOCLASS)UnThreadQuerySetWin32StartAddress,
		&lpAddr,
		sizeof(lpAddr),
		&ulSize);

	return lpAddr;
}

DWORD CThread::GetThreadTed(HANDLE hThread)
{
	/*调用未文档函数*/
	THREAD_BASIC_INFORMATION info = { 0 };
	ULONG ulSize;
	/*获取线程入地址*/
	NtQueryInformationThread(
		hThread,
		(THREADINFOCLASS)UnThreadBasicInformation,
		&info,
		sizeof(info),
		&ulSize);

	return (DWORD)info.TebBaseAddress;
}

bool CThread::SuspendTheThread(DWORD dwTid)
{
	/*获取进程句柄*/
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTid);
	if (hThread != INVALID_HANDLE_VALUE && hThread != NULL)
	{
		SuspendThread(hThread);
		CloseHandle(hThread);
		return true;
	}
	return false;
}

bool CThread::ResumeTheThread(DWORD dwTid)
{
	/*获取进程句柄*/
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTid);
	if (hThread != INVALID_HANDLE_VALUE && hThread != NULL)
	{
		ResumeThread(hThread);
		CloseHandle(hThread);
		return true;
	}
	return false;
}

bool CThread::GetThreadStatus(DWORD dwTid)
{
	/*获取进程句柄*/
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTid);
	if (hThread != INVALID_HANDLE_VALUE && hThread != NULL)
	{
		/*利用挂起时返回上次引用次数，判断是否被挂起*/
		DWORD dwSuspendCount = SuspendThread(hThread);
		ResumeThread(hThread);
		CloseHandle(hThread);
		/*返回当前状态 0 表示正常运行，非零表示挂起*/
		return !dwSuspendCount;
	}
	return true;
}
