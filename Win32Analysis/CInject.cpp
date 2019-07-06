#include "pch.h"
#include <Windows.h>
#include "CInject.h"
#include ".\keystone\keystone.h"
#pragma comment (lib,"keystone/x86/keystone_x86.lib")


CInject::CInject():m_CurMode(),m_CurType(),m_InitInject(false)
{

}


CInject::~CInject()
{

}

/*初始化注入*/
bool CInject::InitInjectObj(int nPid, INJECT_TYPE Type, INJECT_MODE Mode)
{
	m_InitInject = true;
	m_pid = nPid;
	m_CurType = Type;
	m_CurMode = Mode;
	return true;
}

/*开始注入*/
bool CInject::StartInject(char * pBuff, unsigned int size)
{

	/*注入模式*/
	switch (m_CurMode)
	{
		/*远程线程注入*/
	case INJECT_MODE::Mode_Remote:
		{
			/*注入类型*/
			switch (m_CurType)
			{
				/*指令注入*/
			case INJECT_TYPE::Inject_Asm:
				AsmCodeInject(pBuff);
	
				break;
				/*dll注入*/
			case INJECT_TYPE::Inject_Dll:
				DllInject(pBuff);
				break;
				/*机器码注入*/
			case INJECT_TYPE::Inject_Opcode:
				OpcodeInject(pBuff, size);
				break;
			default:
				break;
		}
	
		}


		break;
	}


	return false;
}

/*汇编指令注入*/
bool CInject::AsmCodeInject(char * szAsmCode)
{
	/*转换汇编指令到Opcode 的长度*/
	int nCodeLen = GetAsmLength(szAsmCode);

	/*打开进程*/
	HANDLE hProcess = OpenProcess(
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		FALSE,
		m_pid);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/*在目标进程申请空间*/
	SIZE_T Count = 0;
	LPVOID pMem = VirtualAllocEx(hProcess, NULL, nCodeLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	/*反汇编指令： 将申请的空间地址作为反汇编起始位置*/
	ks_engine *pengine = NULL;
	if (KS_ERR_OK != ks_open(KS_ARCH_X86, KS_MODE_32, &pengine))
	{
		printf("反汇编引擎初始化失败\n");
		return 0;
	}
	unsigned char* opcode = NULL; // 汇编得到的opcode的缓冲区首地址
	unsigned int nOpcodeSize = 0; // 汇编出来的opcode的字节数

	int nRet = 0; // 保存函数的返回值，用于判断函数是否执行成功
	size_t stat_count = 0; // 保存成功汇编的指令的条数
	nRet = ks_asm(pengine, /* 汇编引擎句柄，通过ks_open函数得到*/
		szAsmCode, /*要转换的汇编指令*/
		(uint64_t)pMem, /*汇编指令所在的地址*/
		&opcode,/*输出的opcode*/
		&nOpcodeSize,/*输出的opcode的字节数*/
		&stat_count /*输出成功汇编的指令的条数*/
	);

	// 返回值等于-1时反汇编错误
	if (nRet == -1)
	{
		printf("错误信息：%s\n", ks_strerror(ks_errno(pengine)));
		return 0;
	}

	/*写入指令*/
	WriteProcessMemory(hProcess, pMem, opcode, nOpcodeSize, &Count);

	/*创建远程线程*/
	HANDLE hTread = CreateRemoteThread(
		hProcess,
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)pMem,
		NULL,
		NULL,
		NULL
	);

	/*等待线程结束*/
	WaitForSingleObject(hTread, -1);
	/*关闭句柄*/
	CloseHandle(hTread);
	CloseHandle(hProcess);
	// 释放空间
	ks_free(opcode);
	// 关闭句柄
	ks_close(pengine);
	return true;
}

/*Opcode注入*/
bool CInject::OpcodeInject(char * pOpcode, unsigned int nSize)
{
	/*打开进程*/
	HANDLE hProcess = OpenProcess(
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		FALSE,
		m_pid);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/*在目标进程申请空间*/
	SIZE_T Count = 0;
	LPVOID pMem = VirtualAllocEx(hProcess, NULL, nSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	/*写入opocde*/
	WriteProcessMemory(hProcess, pMem, pOpcode, nSize, &Count);

	/*创建远程线程*/
	HANDLE hTread = CreateRemoteThread(
		hProcess,
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)pMem,
		NULL,
		NULL,
		NULL
	);
	if (hTread == INVALID_HANDLE_VALUE || hTread == NULL)
	{
		return FALSE;
	}

	/*等待线程结束*/
	WaitForSingleObject(hTread, -1);
	/*关闭句柄*/
	CloseHandle(hTread);
	CloseHandle(hProcess);
	return true;
}

bool CInject::DllInject(char *szDllPath)
{
	/*打开进程*/
	HANDLE hProcess = OpenProcess(
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		FALSE,
		m_pid);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	//在指定的进程中申请空间
	LPVOID pMem = VirtualAllocEx(hProcess, NULL, 2* MAX_PATH, MEM_COMMIT, PAGE_READWRITE);


	//向这块内存写入dll路径
	SIZE_T Count;
	WriteProcessMemory(hProcess, pMem, szDllPath, MAX_PATH, &Count);

		//利用进程中的线程回调 ，加载自己的dll
		HANDLE hTread = CreateRemoteThread(hProcess,
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)LoadLibraryA,
			pMem,
			NULL,
			NULL
		);
		if (hTread == INVALID_HANDLE_VALUE || hTread == NULL)
		{
			return FALSE;
		}
		/*等待线程结束*/
		WaitForSingleObject(hTread, -1);
		CloseHandle(hProcess);
		CloseHandle(hTread);
		return TRUE;


	return false;
}

/*获取指令长度*/
unsigned int CInject::GetAsmLength(char * szAsmCode)
{
	ks_engine *pengine = NULL;
	if (KS_ERR_OK != ks_open(KS_ARCH_X86, KS_MODE_32, &pengine))
	{
		printf("反汇编引擎初始化失败\n");
		return 0;
	}

	unsigned char* opcode = NULL; // 汇编得到的opcode的缓冲区首地址
	unsigned int nOpcodeSize = 0; // 汇编出来的opcode的字节数

	int nRet = 0; // 保存函数的返回值，用于判断函数是否执行成功
	size_t stat_count = 0; // 保存成功汇编的指令的条数

	nRet = ks_asm(pengine, /* 汇编引擎句柄，通过ks_open函数得到*/
		szAsmCode, /*要转换的汇编指令*/
		0x401000, /*汇编指令所在的地址*/
		&opcode,/*输出的opcode*/
		&nOpcodeSize,/*输出的opcode的字节数*/
		&stat_count /*输出成功汇编的指令的条数*/
	);

	// 返回值等于-1时反汇编错误
	if (nRet == -1)
	{
		// 输出错误信息
		printf("错误信息：%s\n", ks_strerror(ks_errno(pengine)));
		return 0;
	}

	// 释放空间
	ks_free(opcode);

	// 关闭句柄
	ks_close(pengine);
	return nOpcodeSize;
}
