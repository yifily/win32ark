#pragma once
class CInject
{
public:
	/*注入类型*/
	enum INJECT_TYPE{
		Inject_Dll,
		Inject_Asm,
		Inject_Opcode
	};
	/*注入方式*/
	enum INJECT_MODE {
		Mode_Remote,
		Mode_Message,
		Mode_Apc
	};

	CInject();
	~CInject();
	/*初始化注入*/
	bool InitInjectObj(int nPid, INJECT_TYPE Type, INJECT_MODE Mode);

	/*开始注入*/
	bool StartInject(char *pBuff, unsigned int size);

	/*代码注入*/
	bool AsmCodeInject(char * szAsmCode);

	/*Opcode注入*/
	bool OpcodeInject(char * pOpcode, unsigned int size);

	/*dll注入*/
	bool DllInject(char * szDllPath);

private:

	unsigned int GetAsmLength(char * pOpcode);





	INJECT_TYPE m_CurType;	/*当前注入类型*/
	INJECT_MODE m_CurMode;  /*当前注入模式*/
	bool m_InitInject;		/*初始化注入对象*/
	int m_pid;				/*目标pid*/


};

