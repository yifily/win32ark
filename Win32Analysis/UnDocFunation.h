#pragma once
#include <windows.h>
#include<TlHelp32.h>
#include <winternl.h>
#pragma comment(lib,"ntdll.lib")

//线程基本信息结构体
typedef struct _THREAD_BASIC_INFORMATION
{
	NTSTATUS  ExitStatus;
	PTEB  TebBaseAddress;
	CLIENT_ID  ClientId;
	ULONG_PTR  AffinityMask;
	KPRIORITY  Priority;
	LONG  BasePriority;
}THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

//线程信息枚举值
typedef enum _MYTHREADINFOCLASS
{
	UnThreadBasicInformation,				//线程基本信息，线程环境块
	UnThreadTimes,
	UnThreadPriority,
	UnThreadBasePriority,
	UnThreadAffinityMask,
	UnThreadImpersonationToken,
	UnThreadDescriptorTableEntry,
	UnThreadEnableAlignmentFaultFixup,
	UnThreadEventPair_Reusable,
	UnThreadQuerySetWin32StartAddress = 9, //线程入口地址
	UnThreadZeroTlsCell,
	UnThreadPerformanceCount,
	UnThreadAmILastThread,
	UnThreadIdealProcessor,
	UnThreadPriorityBoost,
	UnThreadSetTlsArrayAddress,
	UnThreadIsIoPending1,
	UnThreadHideFromDebugger,
	UnThreadBreakOnTermination,
	UnThreadSwitchLegacyState,
	UnThreadIsTerminated,
	UnMaxThreadInfoClass
}MYTHREADINFOCLASS;

//进程信息枚举值
typedef enum _MYPROCESSINFOCLASS {
	UnProcessBasicInformation,				//进程基本信息  PROCESS_BASIC_INFORMATION
	UnProcessQuotaLimits,
	UnProcessIoCounters,
	UnProcessVmCounters,
	UnProcessTimes,
	UnProcessBasePriority,
	UnProcessRaisePriority,
	UnProcessDebugPort,
	UnProcessExceptionPort,
	UnProcessAccessToken,
	UnProcessLdtInformation,
	UnProcessLdtSize,
	UnProcessDefaultHardErrorMode,
	UnProcessIoPortHandlers,
	UnProcessPooledUsageAndLimits,
	UnProcessWorkingSetWatch,
	UnProcessUserModeIOPL,
	UnProcessEnableAlignmentFaultFixup,
	UnProcessPriorityClass,
	UnProcessWx86Information,
	UnProcessHandleCount,					  //进程句柄个数
	UnProcessAffinityMask,
	UnProcessPriorityBoost,
	UnProcessDeviceMap,
	UnProcessSessionInformation,
	UnProcessForegroundInformation,
	UnProcessWow64Information,
	UnProcessImageFileName,
	UnProcessLUIDDeviceMapsEnabled,
	UnProcessBreakOnTermination,
	UnProcessDebugObjectHandle,			//调试对象句柄
	UnProcessDebugFlags,					//调试标志
	UnProcessHandleTracing,
	UnProcessIoPriority,
	UnProcessExecuteFlags,
	UnProcessResourceManagement,
	UnProcessCookie,
	UnProcessImageInformation,
	UnMaxProcessInfoClass

}MYPROCESSINFOCLASS, *PMYPROCESSINFOCLASS;

//系统信息枚举值
typedef enum _MYSYSTEMINFOCLASS {
	UnSystemBasicInformation,
	UnSystemProcessorInformation,				//SYSTEM_PROCESSOR_INFORMATION
	UnSystemPerformanceInformation,
	UnSystemTimeOfDayInformation,
	UnSystemPathInformation,
	UnSystemProcessInformation,
	UnSystemCallCountInformation,
	UnSystemDeviceInformation,
	UnSystemProcessorPerformanceInformation,
	UnSystemFlagsInformation,
	UnSystemCallTimeInformation,
	UnSystemModuleInformation,				//系统模块信息，所有进程信息
	UnSystemLocksInformation,
	UnSystemStackTraceInformation,
	UnSystemPagedPoolInformation,
	UnSystemNonPagedPoolInformation,
	UnSystemHandleInformation,
	UnSystemObjectInformation,
	UnSystemPageFileInformation,
	UnSystemVdmInstemulInformation,
	UnSystemVdmBopInformation,
	UnSystemFileCacheInformation,
	UnSystemPoolTagInformation,
	UnSystemInterruptInformation,
	UnSystemDpcBehaviorInformation,
	UnSystemFullMemoryInformation,
	UnSystemLoadGdiDriverInformation,
	UnSystemUnloadGdiDriverInformation,
	UnSystemTimeAdjustmentInformation,
	UnSystemSummaryMemoryInformation,
	UnSystemMirrorMemoryInformation,
	UnSystemPerformanceTraceInformation,
	UnSystemObsolete0,
	UnSystemExceptionInformation,
	UnSystemCrashDumpStateInformation,
	UnSystemKernelDebuggerInformation,
	UnSystemContextSwitchInformation,
	UnSystemRegistryQuotaInformation,
	UnSystemExtendServiceTableInformation,
	UnSystemPrioritySeperation,
	UnSystemVerifierAddDriverInformation,
	UnSystemVerifierRemoveDriverInformation,
	UnSystemProcessorIdleInformation,
	UnSystemLegacyDriverInformation,
	UnSystemCurrentTimeZoneInformation,
	UnSystemLookasideInformation,
	UnSystemTimeSlipNotification,
	UnSystemSessionCreate,
	UnSystemSessionDetach,
	UnSystemSessionInformation,
	UnSystemRangeStartInformation,
	UnSystemVerifierInformation,
	UnSystemVerifierThunkExtend,
	UnSystemSessionProcessInformation,
	UnSystemLoadGdiDriverInSystemSpace,
	UnSystemNumaProcessorMap,
	UnSystemPrefetcherInformation,
	UnSystemExtendedProcessInformation,
	UnSystemRecommendedSharedDataAlignment,
	UnSystemComPlusPackage,
	UnSystemNumaAvailableMemory,
	UnSystemProcessorPowerInformation,
	UnSystemEmulationBasicInformation,
	UnSystemEmulationProcessorInformation,
	UnSystemExtendedHandleInformation,
	UnSystemLostDelayedWriteInformation,
	UnSystemBigPoolInformation,
	UnSystemSessionPoolTagInformation,
	UnSystemSessionMappedViewInformation,
	UnSystemHotpatchInformation,
	UnSystemObjectSecurityMode,
	UnSystemWatchdogTimerHandler,
	UnSystemWatchdogTimerInformation,
	UnSystemLogicalProcessorInformation,
	UnSystemWow64SharedInformation,
	UnSystemRegisterFirmwareTableInformationHandler,
	UnSystemFirmwareTableInformation,
	UnSystemModuleInformationEx,
	UnSystemVerifierTriageInformation,
	UnSystemSuperfetchInformation,
	UnSystemMemoryListInformation,
	UnSystemFileCacheInformationEx,
	UnMaxSystemInfoClass
}MYSYSTEMINFOCLASS, *PMYSYSTEMINFOCLASS;

//进程模块信息
typedef struct _SYSTME_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[256];
} SYSTME_MODULE_INFORMATION, *PSYSTME_MODULE_INFORMATION;

//查询系统模块返回的结构
typedef struct _SYSTEM_PROCESS_MODULES
{
	ULONG NumberOfModules;
	SYSTME_MODULE_INFORMATION Modules[1];
} SYSTEM_PROCESS_MODULES, *PSYSTEM_PROCESS_MODULES;
