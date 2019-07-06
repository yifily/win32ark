#pragma once
#include <Windows.h>
#include <atlstr.h>
#include <vector>
using std::vector;

class CPe
{
public:

	// 导出表
	struct ExportInfo {
		DWORD Order;
		DWORD Rva;
		DWORD Foa;
		char FunName[100];
	}*PEXPORTINFO;


	//IAT表
	struct IATInfo_Item {
		char FunName[250];
		DWORD Order;
		DWORD RVA;
	};

	struct IATInfo {
		char dllName[250];
		vector<IATInfo_Item> vec_item;
	}*PIATINFO;


	// 资源name ,资源的RVA,FOA,Size
	struct ResourceInfo_Item {
		char  ResName[100];
		DWORD Rva;
		DWORD Foa;
		DWORD Size;
	};

	struct ResourceInfo {
		char ResTypeName[100];
		vector<ResourceInfo_Item> ItemInfo;
	};


	//重定位表数据
	struct RelocationInfo {
		DWORD BaseRVA;
		DWORD offset;
		DWORD offsetFoa;
		DWORD RelocationData; //重定位的数据
	};





	CPe(TCHAR *pSzName);

	//解析pe
	BOOL AnalysisPe();

	//获取DOS头
	PIMAGE_DOS_HEADER GetDosHeader();

	//获取NT头
	PIMAGE_NT_HEADERS GetNtHeader();

	//获取区段头表
	PIMAGE_SECTION_HEADER GetSectionHeader();

	//rva =>  foa
	DWORD RvaToOfset(DWORD RVA);


	//获取导出表export
	PIMAGE_EXPORT_DIRECTORY GetExportDirectory();

	//获取导入表import
	PIMAGE_IMPORT_DESCRIPTOR GetImportDescriptor();

	//获取导出表信息  Order  - Rva  - Foa  - FunName
	BOOL GetExprotInfo(vector<ExportInfo> & vec_info);

	//解析IAT
	BOOL GetIATInfo(vector< IATInfo> &vec_info);

	//获取资源表
	PIMAGE_RESOURCE_DIRECTORY GetResourceDirectory();

	//解析资源表
	BOOL GetResourceInfo(vector<ResourceInfo>& vec_ResInfo);


	//获取重定位表
	PIMAGE_BASE_RELOCATION GetRelocation();

	//解析重定位表
	BOOL GetRelocationInfo(vector<RelocationInfo> & vec_RelInfo);






	~CPe();
private:
	//路径
	CString m_szExePath;
	//文件内容
	BYTE * m_pbuff;

};


