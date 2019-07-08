#pragma once
#include <Windows.h>
#include <atlstr.h>
#include <vector>
using std::vector;
#include"tchar.h"

class CSoftwareInfo {
public:
	CString m_Name;
	CString m_SoftUniPath;
	CString m_SoftInsPath;
	CString m_SoftDate;
	CString m_SoftVer;
	CString m_SoftIconPath;
	CString m_SoftUrl;

};

/*重定义返回类型*/
typedef vector<CSoftwareInfo>  VSoftAll;


class CSoftware
{
public:
	CSoftware();
	~CSoftware();
	/*获取软件列表*/
	bool GetSoftwareList(VSoftAll *vSoftInfos);
	/*卸载软件*/
	bool UnInstallSoftware(CString cSoftUnPath);
	/*指定目录显示资源管理器*/
	bool ShowSoftwareDir(CString cPath);

	/*获取的当前操作系统版本*/
	BOOL Is64BitSystem();

};

