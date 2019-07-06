#pragma once
#include<atlstr.h>
#include <vector>
using std::vector;
#include"UnDocFunation.h"

/*模块信息结构体*/
class ModuleInfo {
public:
	CString m_Name;
	CString m_Path;
	CString m_BaseImage;
	CString m_Size;
	CString m_Entry;

};

/*重定义返回类型*/
typedef vector<ModuleInfo>  VModAll;



class CModule
{
public:
	CModule();
	~CModule();

	/*获取模块信息*/
	bool GetModuleInfo(VModAll *ModuleInfos,DWORD dwPid);

};

