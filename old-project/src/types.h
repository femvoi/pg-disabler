#pragma once

#include <ntddk.h>
#include <ntifs.h>
#include <stdint.h>
#include <ntimage.h>

typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation, // q: SYSTEM_BASIC_INFORMATION
    SystemProcessorInformation, // q: SYSTEM_PROCESSOR_INFORMATION
    SystemPerformanceInformation, // q: SYSTEM_PERFORMANCE_INFORMATION
    SystemTimeOfDayInformation, // q: SYSTEM_TIMEOFDAY_INFORMATION
    SystemPathInformation, // not implemented
    SystemProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
    SystemCallCountInformation, // q: SYSTEM_CALL_COUNT_INFORMATION
    SystemDeviceInformation, // q: SYSTEM_DEVICE_INFORMATION
    SystemProcessorPerformanceInformation, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION (EX in: USHORT ProcessorGroup)
    SystemFlagsInformation, // q: SYSTEM_FLAGS_INFORMATION
    SystemCallTimeInformation, // not implemented // SYSTEM_CALL_TIME_INFORMATION // 10
    SystemModuleInformation, // q: RTL_PROCESS_MODULES
    SystemLocksInformation, // q: RTL_PROCESS_LOCKS
    SystemStackTraceInformation, // q: RTL_PROCESS_BACKTRACES
    SystemPagedPoolInformation, // not implemented
    SystemNonPagedPoolInformation, // not implemented
    SystemHandleInformation, // q: SYSTEM_HANDLE_INFORMATION
    SystemObjectInformation, // q: SYSTEM_OBJECTTYPE_INFORMATION mixed with SYSTEM_OBJECT_INFORMATION
    SystemPageFileInformation, // q: SYSTEM_PAGEFILE_INFORMATION
    SystemVdmInstemulInformation, // q: SYSTEM_VDM_INSTEMUL_INFO
    SystemVdmBopInformation, // not implemented // 20
    SystemFileCacheInformation, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemCache)
    SystemPoolTagInformation, // q: SYSTEM_POOLTAG_INFORMATION
    SystemInterruptInformation, // q: SYSTEM_INTERRUPT_INFORMATION (EX in: USHORT ProcessorGroup)
    SystemDpcBehaviorInformation, // q: SYSTEM_DPC_BEHAVIOR_INFORMATION; s: SYSTEM_DPC_BEHAVIOR_INFORMATION (requires SeLoadDriverPrivilege)
    SystemFullMemoryInformation, // not implemented // SYSTEM_MEMORY_USAGE_INFORMATION
    SystemLoadGdiDriverInformation, // s (kernel-mode only)
    SystemUnloadGdiDriverInformation, // s (kernel-mode only)
    SystemTimeAdjustmentInformation, // q: SYSTEM_QUERY_TIME_ADJUST_INFORMATION; s: SYSTEM_SET_TIME_ADJUST_INFORMATION (requires SeSystemtimePrivilege)
    SystemSummaryMemoryInformation, // not implemented // SYSTEM_MEMORY_USAGE_INFORMATION
    SystemMirrorMemoryInformation, // s (requires license value "Kernel-MemoryMirroringSupported") (requires SeShutdownPrivilege) // 30
    SystemPerformanceTraceInformation, // q; s: (type depends on EVENT_TRACE_INFORMATION_CLASS)
    SystemObsolete0, // not implemented
    SystemExceptionInformation, // q: SYSTEM_EXCEPTION_INFORMATION
    SystemCrashDumpStateInformation, // s: SYSTEM_CRASH_DUMP_STATE_INFORMATION (requires SeDebugPrivilege)
    SystemKernelDebuggerInformation, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION
    SystemContextSwitchInformation, // q: SYSTEM_CONTEXT_SWITCH_INFORMATION
    SystemRegistryQuotaInformation, // q: SYSTEM_REGISTRY_QUOTA_INFORMATION; s (requires SeIncreaseQuotaPrivilege)
    SystemExtendServiceTableInformation, // s (requires SeLoadDriverPrivilege) // loads win32k only
    SystemPrioritySeparation, // s (requires SeTcbPrivilege)
    SystemVerifierAddDriverInformation, // s: UNICODE_STRING (requires SeDebugPrivilege) // 40
    SystemVerifierRemoveDriverInformation, // s: UNICODE_STRING (requires SeDebugPrivilege)
    SystemProcessorIdleInformation, // q: SYSTEM_PROCESSOR_IDLE_INFORMATION (EX in: USHORT ProcessorGroup)
    SystemLegacyDriverInformation, // q: SYSTEM_LEGACY_DRIVER_INFORMATION
    SystemCurrentTimeZoneInformation, // q; s: RTL_TIME_ZONE_INFORMATION
    SystemLookasideInformation, // q: SYSTEM_LOOKASIDE_INFORMATION
    SystemTimeSlipNotification, // s: HANDLE (NtCreateEvent) (requires SeSystemtimePrivilege)
    SystemSessionCreate, // not implemented
    SystemSessionDetach, // not implemented
    SystemSessionInformation, // not implemented (SYSTEM_SESSION_INFORMATION)
    SystemRangeStartInformation, // q: SYSTEM_RANGE_START_INFORMATION // 50
    SystemVerifierInformation, // q: SYSTEM_VERIFIER_INFORMATION; s (requires SeDebugPrivilege)
    SystemVerifierThunkExtend, // s (kernel-mode only)
    SystemSessionProcessInformation, // q: SYSTEM_SESSION_PROCESS_INFORMATION
    SystemLoadGdiDriverInSystemSpace, // s: SYSTEM_GDI_DRIVER_INFORMATION (kernel-mode only) (same as SystemLoadGdiDriverInformation)
    SystemNumaProcessorMap, // q: SYSTEM_NUMA_INFORMATION
    SystemPrefetcherInformation, // q; s: PREFETCHER_INFORMATION // PfSnQueryPrefetcherInformation
    SystemExtendedProcessInformation, // q: SYSTEM_EXTENDED_PROCESS_INFORMATION
    SystemRecommendedSharedDataAlignment, // q: ULONG // KeGetRecommendedSharedDataAlignment
    SystemComPlusPackage, // q; s: ULONG
    SystemNumaAvailableMemory, // q: SYSTEM_NUMA_INFORMATION // 60
    SystemProcessorPowerInformation, // q: SYSTEM_PROCESSOR_POWER_INFORMATION (EX in: USHORT ProcessorGroup)
    SystemEmulationBasicInformation, // q: SYSTEM_BASIC_INFORMATION
    SystemEmulationProcessorInformation, // q: SYSTEM_PROCESSOR_INFORMATION
    SystemExtendedHandleInformation, // q: SYSTEM_HANDLE_INFORMATION_EX
    SystemLostDelayedWriteInformation, // q: ULONG
    SystemBigPoolInformation, // q: SYSTEM_BIGPOOL_INFORMATION
    SystemSessionPoolTagInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION
    SystemSessionMappedViewInformation, // q: SYSTEM_SESSION_MAPPED_VIEW_INFORMATION
    SystemHotpatchInformation, // q; s: SYSTEM_HOTPATCH_CODE_INFORMATION
    SystemObjectSecurityMode, // q: ULONG // 70
    SystemWatchdogTimerHandler, // s: SYSTEM_WATCHDOG_HANDLER_INFORMATION // (kernel-mode only)
    SystemWatchdogTimerInformation, // q: SYSTEM_WATCHDOG_TIMER_INFORMATION // NtQuerySystemInformationEx // (kernel-mode only)
    SystemLogicalProcessorInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx
    SystemWow64SharedInformationObsolete, // not implemented
    SystemRegisterFirmwareTableInformationHandler, // s: SYSTEM_FIRMWARE_TABLE_HANDLER // (kernel-mode only)
    SystemFirmwareTableInformation, // SYSTEM_FIRMWARE_TABLE_INFORMATION
    SystemModuleInformationEx, // q: RTL_PROCESS_MODULE_INFORMATION_EX // since VISTA
    SystemVerifierTriageInformation, // not implemented
    SystemSuperfetchInformation, // q; s: SUPERFETCH_INFORMATION // PfQuerySuperfetchInformation
    SystemMemoryListInformation, // q: SYSTEM_MEMORY_LIST_INFORMATION; s: SYSTEM_MEMORY_LIST_COMMAND (requires SeProfileSingleProcessPrivilege) // 80
    SystemFileCacheInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (same as SystemFileCacheInformation)
    SystemThreadPriorityClientIdInformation, // s: SYSTEM_THREAD_CID_PRIORITY_INFORMATION (requires SeIncreaseBasePriorityPrivilege) // NtQuerySystemInformationEx
    SystemProcessorIdleCycleTimeInformation, // q: SYSTEM_PROCESSOR_IDLE_CYCLE_TIME_INFORMATION[] (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx
    SystemVerifierCancellationInformation, // SYSTEM_VERIFIER_CANCELLATION_INFORMATION // name:wow64:whNT32QuerySystemVerifierCancellationInformation
    SystemProcessorPowerInformationEx, // not implemented
    SystemRefTraceInformation, // q; s: SYSTEM_REF_TRACE_INFORMATION // ObQueryRefTraceInformation
    SystemSpecialPoolInformation, // q; s: SYSTEM_SPECIAL_POOL_INFORMATION (requires SeDebugPrivilege) // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
    SystemProcessIdInformation, // q: SYSTEM_PROCESS_ID_INFORMATION
    SystemErrorPortInformation, // s (requires SeTcbPrivilege)
    SystemBootEnvironmentInformation, // q: SYSTEM_BOOT_ENVIRONMENT_INFORMATION // 90
    SystemHypervisorInformation, // q: SYSTEM_HYPERVISOR_QUERY_INFORMATION
    SystemVerifierInformationEx, // q; s: SYSTEM_VERIFIER_INFORMATION_EX
    SystemTimeZoneInformation, // q; s: RTL_TIME_ZONE_INFORMATION (requires SeTimeZonePrivilege)
    SystemImageFileExecutionOptionsInformation, // s: SYSTEM_IMAGE_FILE_EXECUTION_OPTIONS_INFORMATION (requires SeTcbPrivilege)
    SystemCoverageInformation, // q: COVERAGE_MODULES s: COVERAGE_MODULE_REQUEST // ExpCovQueryInformation (requires SeDebugPrivilege)
    SystemPrefetchPatchInformation, // SYSTEM_PREFETCH_PATCH_INFORMATION
    SystemVerifierFaultsInformation, // s: SYSTEM_VERIFIER_FAULTS_INFORMATION (requires SeDebugPrivilege)
    SystemSystemPartitionInformation, // q: SYSTEM_SYSTEM_PARTITION_INFORMATION
    SystemSystemDiskInformation, // q: SYSTEM_SYSTEM_DISK_INFORMATION
    SystemProcessorPerformanceDistribution, // q: SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx // 100
    SystemNumaProximityNodeInformation, // q; s: SYSTEM_NUMA_PROXIMITY_MAP
    SystemDynamicTimeZoneInformation, // q; s: RTL_DYNAMIC_TIME_ZONE_INFORMATION (requires SeTimeZonePrivilege)
    SystemCodeIntegrityInformation, // q: SYSTEM_CODEINTEGRITY_INFORMATION // SeCodeIntegrityQueryInformation
    SystemProcessorMicrocodeUpdateInformation, // s: SYSTEM_PROCESSOR_MICROCODE_UPDATE_INFORMATION
    SystemProcessorBrandString, // q: CHAR[] // HaliQuerySystemInformation -> HalpGetProcessorBrandString, info class 23
    SystemVirtualAddressInformation, // q: SYSTEM_VA_LIST_INFORMATION[]; s: SYSTEM_VA_LIST_INFORMATION[] (requires SeIncreaseQuotaPrivilege) // MmQuerySystemVaInformation
    SystemLogicalProcessorAndGroupInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX (EX in: LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType) // since WIN7 // NtQuerySystemInformationEx // KeQueryLogicalProcessorRelationship
    SystemProcessorCycleTimeInformation, // q: SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION[] (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx
    SystemStoreInformation, // q; s: SYSTEM_STORE_INFORMATION (requires SeProfileSingleProcessPrivilege) // SmQueryStoreInformation
    SystemRegistryAppendString, // s: SYSTEM_REGISTRY_APPEND_STRING_PARAMETERS // 110
    SystemAitSamplingValue, // s: ULONG (requires SeProfileSingleProcessPrivilege)
    SystemVhdBootInformation, // q: SYSTEM_VHD_BOOT_INFORMATION
    SystemCpuQuotaInformation, // q; s: PS_CPU_QUOTA_QUERY_INFORMATION
    SystemNativeBasicInformation, // q: SYSTEM_BASIC_INFORMATION
    SystemErrorPortTimeouts, // SYSTEM_ERROR_PORT_TIMEOUTS
    SystemLowPriorityIoInformation, // q: SYSTEM_LOW_PRIORITY_IO_INFORMATION
    SystemTpmBootEntropyInformation, // q: BOOT_ENTROPY_NT_RESULT // ExQueryBootEntropyInformation
    SystemVerifierCountersInformation, // q: SYSTEM_VERIFIER_COUNTERS_INFORMATION
    SystemPagedPoolInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypePagedPool)
    SystemSystemPtesInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemPtes) // 120
    SystemNodeDistanceInformation, // q: USHORT[4*NumaNodes] // (EX in: USHORT NodeNumber) // NtQuerySystemInformationEx
    SystemAcpiAuditInformation, // q: SYSTEM_ACPI_AUDIT_INFORMATION // HaliQuerySystemInformation -> HalpAuditQueryResults, info class 26
    SystemBasicPerformanceInformation, // q: SYSTEM_BASIC_PERFORMANCE_INFORMATION // name:wow64:whNtQuerySystemInformation_SystemBasicPerformanceInformation
    SystemQueryPerformanceCounterInformation, // q: SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION // since WIN7 SP1
    SystemSessionBigPoolInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION // since WIN8
    SystemBootGraphicsInformation, // q; s: SYSTEM_BOOT_GRAPHICS_INFORMATION (kernel-mode only)
    SystemScrubPhysicalMemoryInformation, // q; s: MEMORY_SCRUB_INFORMATION
    SystemBadPageInformation, // SYSTEM_BAD_PAGE_INFORMATION
    SystemProcessorProfileControlArea, // q; s: SYSTEM_PROCESSOR_PROFILE_CONTROL_AREA
    SystemCombinePhysicalMemoryInformation, // s: MEMORY_COMBINE_INFORMATION, MEMORY_COMBINE_INFORMATION_EX, MEMORY_COMBINE_INFORMATION_EX2 // 130
    SystemEntropyInterruptTimingInformation, // q; s: SYSTEM_ENTROPY_TIMING_INFORMATION
    SystemConsoleInformation, // q; s: SYSTEM_CONSOLE_INFORMATION
    SystemPlatformBinaryInformation, // q: SYSTEM_PLATFORM_BINARY_INFORMATION (requires SeTcbPrivilege)
    SystemPolicyInformation, // q: SYSTEM_POLICY_INFORMATION (Warbird/Encrypt/Decrypt/Execute)
    SystemHypervisorProcessorCountInformation, // q: SYSTEM_HYPERVISOR_PROCESSOR_COUNT_INFORMATION
    SystemDeviceDataInformation, // q: SYSTEM_DEVICE_DATA_INFORMATION
    SystemDeviceDataEnumerationInformation, // q: SYSTEM_DEVICE_DATA_INFORMATION
    SystemMemoryTopologyInformation, // q: SYSTEM_MEMORY_TOPOLOGY_INFORMATION
    SystemMemoryChannelInformation, // q: SYSTEM_MEMORY_CHANNEL_INFORMATION
    SystemBootLogoInformation, // q: SYSTEM_BOOT_LOGO_INFORMATION // 140
    SystemProcessorPerformanceInformationEx, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION_EX // (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx // since WINBLUE
    SystemCriticalProcessErrorLogInformation, // CRITICAL_PROCESS_EXCEPTION_DATA
    SystemSecureBootPolicyInformation, // q: SYSTEM_SECUREBOOT_POLICY_INFORMATION
    SystemPageFileInformationEx, // q: SYSTEM_PAGEFILE_INFORMATION_EX
    SystemSecureBootInformation, // q: SYSTEM_SECUREBOOT_INFORMATION
    SystemEntropyInterruptTimingRawInformation, // q; s: SYSTEM_ENTROPY_TIMING_INFORMATION
    SystemPortableWorkspaceEfiLauncherInformation, // q: SYSTEM_PORTABLE_WORKSPACE_EFI_LAUNCHER_INFORMATION
    SystemFullProcessInformation, // q: SYSTEM_EXTENDED_PROCESS_INFORMATION with SYSTEM_PROCESS_INFORMATION_EXTENSION (requires admin)
    SystemKernelDebuggerInformationEx, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX
    SystemBootMetadataInformation, // 150 // (requires SeTcbPrivilege)
    SystemSoftRebootInformation, // q: ULONG
    SystemElamCertificateInformation, // s: SYSTEM_ELAM_CERTIFICATE_INFORMATION
    SystemOfflineDumpConfigInformation, // q: OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2
    SystemProcessorFeaturesInformation, // q: SYSTEM_PROCESSOR_FEATURES_INFORMATION
    SystemRegistryReconciliationInformation, // s: NULL (requires admin) (flushes registry hives)
    SystemEdidInformation, // q: SYSTEM_EDID_INFORMATION
    SystemManufacturingInformation, // q: SYSTEM_MANUFACTURING_INFORMATION // since THRESHOLD
    SystemEnergyEstimationConfigInformation, // q: SYSTEM_ENERGY_ESTIMATION_CONFIG_INFORMATION
    SystemHypervisorDetailInformation, // q: SYSTEM_HYPERVISOR_DETAIL_INFORMATION
    SystemProcessorCycleStatsInformation, // q: SYSTEM_PROCESSOR_CYCLE_STATS_INFORMATION (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx // 160
    SystemVmGenerationCountInformation,
    SystemTrustedPlatformModuleInformation, // q: SYSTEM_TPM_INFORMATION
    SystemKernelDebuggerFlags, // SYSTEM_KERNEL_DEBUGGER_FLAGS
    SystemCodeIntegrityPolicyInformation, // q; s: SYSTEM_CODEINTEGRITYPOLICY_INFORMATION
    SystemIsolatedUserModeInformation, // q: SYSTEM_ISOLATED_USER_MODE_INFORMATION
    SystemHardwareSecurityTestInterfaceResultsInformation,
    SystemSingleModuleInformation, // q: SYSTEM_SINGLE_MODULE_INFORMATION
    SystemAllowedCpuSetsInformation, // s: SYSTEM_WORKLOAD_ALLOWED_CPU_SET_INFORMATION
    SystemVsmProtectionInformation, // q: SYSTEM_VSM_PROTECTION_INFORMATION (previously SystemDmaProtectionInformation)
    SystemInterruptCpuSetsInformation, // q: SYSTEM_INTERRUPT_CPU_SET_INFORMATION // 170
    SystemSecureBootPolicyFullInformation, // q: SYSTEM_SECUREBOOT_POLICY_FULL_INFORMATION
    SystemCodeIntegrityPolicyFullInformation,
    SystemAffinitizedInterruptProcessorInformation, // q: KAFFINITY_EX // (requires SeIncreaseBasePriorityPrivilege)
    SystemRootSiloInformation, // q: SYSTEM_ROOT_SILO_INFORMATION
    SystemCpuSetInformation, // q: SYSTEM_CPU_SET_INFORMATION // since THRESHOLD2
    SystemCpuSetTagInformation, // q: SYSTEM_CPU_SET_TAG_INFORMATION
    SystemWin32WerStartCallout,
    SystemSecureKernelProfileInformation, // q: SYSTEM_SECURE_KERNEL_HYPERGUARD_PROFILE_INFORMATION
    SystemCodeIntegrityPlatformManifestInformation, // q: SYSTEM_SECUREBOOT_PLATFORM_MANIFEST_INFORMATION // NtQuerySystemInformationEx // since REDSTONE
    SystemInterruptSteeringInformation, // q: in: SYSTEM_INTERRUPT_STEERING_INFORMATION_INPUT, out: SYSTEM_INTERRUPT_STEERING_INFORMATION_OUTPUT // NtQuerySystemInformationEx // 180
    SystemSupportedProcessorArchitectures, // p: in opt: HANDLE, out: SYSTEM_SUPPORTED_PROCESSOR_ARCHITECTURES_INFORMATION[] // NtQuerySystemInformationEx
    SystemMemoryUsageInformation, // q: SYSTEM_MEMORY_USAGE_INFORMATION
    SystemCodeIntegrityCertificateInformation, // q: SYSTEM_CODEINTEGRITY_CERTIFICATE_INFORMATION
    SystemPhysicalMemoryInformation, // q: SYSTEM_PHYSICAL_MEMORY_INFORMATION // since REDSTONE2
    SystemControlFlowTransition, // (Warbird/Encrypt/Decrypt/Execute)
    SystemKernelDebuggingAllowed, // s: ULONG
    SystemActivityModerationExeState, // s: SYSTEM_ACTIVITY_MODERATION_EXE_STATE
    SystemActivityModerationUserSettings, // q: SYSTEM_ACTIVITY_MODERATION_USER_SETTINGS
    SystemCodeIntegrityPoliciesFullInformation, // NtQuerySystemInformationEx
    SystemCodeIntegrityUnlockInformation, // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION // 190
    SystemIntegrityQuotaInformation,
    SystemFlushInformation, // q: SYSTEM_FLUSH_INFORMATION
    SystemProcessorIdleMaskInformation, // q: ULONG_PTR[ActiveGroupCount] // since REDSTONE3
    SystemSecureDumpEncryptionInformation, // NtQuerySystemInformationEx
    SystemWriteConstraintInformation, // SYSTEM_WRITE_CONSTRAINT_INFORMATION
    SystemKernelVaShadowInformation, // SYSTEM_KERNEL_VA_SHADOW_INFORMATION
    SystemHypervisorSharedPageInformation, // SYSTEM_HYPERVISOR_SHARED_PAGE_INFORMATION // since REDSTONE4
    SystemFirmwareBootPerformanceInformation,
    SystemCodeIntegrityVerificationInformation, // SYSTEM_CODEINTEGRITYVERIFICATION_INFORMATION
    SystemFirmwarePartitionInformation, // SYSTEM_FIRMWARE_PARTITION_INFORMATION // 200
    SystemSpeculationControlInformation, // SYSTEM_SPECULATION_CONTROL_INFORMATION // (CVE-2017-5715) REDSTONE3 and above.
    SystemDmaGuardPolicyInformation, // SYSTEM_DMA_GUARD_POLICY_INFORMATION
    SystemEnclaveLaunchControlInformation, // SYSTEM_ENCLAVE_LAUNCH_CONTROL_INFORMATION
    SystemWorkloadAllowedCpuSetsInformation, // SYSTEM_WORKLOAD_ALLOWED_CPU_SET_INFORMATION // since REDSTONE5
    SystemCodeIntegrityUnlockModeInformation, // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION
    SystemLeapSecondInformation, // SYSTEM_LEAP_SECOND_INFORMATION
    SystemFlags2Information, // q: SYSTEM_FLAGS_INFORMATION
    SystemSecurityModelInformation, // SYSTEM_SECURITY_MODEL_INFORMATION // since 19H1
    SystemCodeIntegritySyntheticCacheInformation, // NtQuerySystemInformationEx
    SystemFeatureConfigurationInformation, // q: in: SYSTEM_FEATURE_CONFIGURATION_QUERY, out: SYSTEM_FEATURE_CONFIGURATION_INFORMATION; s: SYSTEM_FEATURE_CONFIGURATION_UPDATE // NtQuerySystemInformationEx // since 20H1 // 210
    SystemFeatureConfigurationSectionInformation, // q: in: SYSTEM_FEATURE_CONFIGURATION_SECTIONS_REQUEST, out: SYSTEM_FEATURE_CONFIGURATION_SECTIONS_INFORMATION // NtQuerySystemInformationEx
    SystemFeatureUsageSubscriptionInformation, // q: SYSTEM_FEATURE_USAGE_SUBSCRIPTION_DETAILS; s: SYSTEM_FEATURE_USAGE_SUBSCRIPTION_UPDATE
    SystemSecureSpeculationControlInformation, // SECURE_SPECULATION_CONTROL_INFORMATION
    SystemSpacesBootInformation, // since 20H2
    SystemFwRamdiskInformation, // SYSTEM_FIRMWARE_RAMDISK_INFORMATION
    SystemWheaIpmiHardwareInformation,
    SystemDifSetRuleClassInformation, // s: SYSTEM_DIF_VOLATILE_INFORMATION (requires SeDebugPrivilege)
    SystemDifClearRuleClassInformation, // s: NULL (requires SeDebugPrivilege)
    SystemDifApplyPluginVerificationOnDriver, // SYSTEM_DIF_PLUGIN_DRIVER_INFORMATION (requires SeDebugPrivilege)
    SystemDifRemovePluginVerificationOnDriver, // SYSTEM_DIF_PLUGIN_DRIVER_INFORMATION (requires SeDebugPrivilege) // 220
    SystemShadowStackInformation, // SYSTEM_SHADOW_STACK_INFORMATION
    SystemBuildVersionInformation, // q: in: ULONG (LayerNumber), out: SYSTEM_BUILD_VERSION_INFORMATION // NtQuerySystemInformationEx // 222
    SystemPoolLimitInformation, // SYSTEM_POOL_LIMIT_INFORMATION (requires SeIncreaseQuotaPrivilege) // NtQuerySystemInformationEx
    SystemCodeIntegrityAddDynamicStore, // CodeIntegrity-AllowConfigurablePolicy-CustomKernelSigners
    SystemCodeIntegrityClearDynamicStores, // CodeIntegrity-AllowConfigurablePolicy-CustomKernelSigners
    SystemDifPoolTrackingInformation,
    SystemPoolZeroingInformation, // q: SYSTEM_POOL_ZEROING_INFORMATION
    SystemDpcWatchdogInformation, // q; s: SYSTEM_DPC_WATCHDOG_CONFIGURATION_INFORMATION
    SystemDpcWatchdogInformation2, // q; s: SYSTEM_DPC_WATCHDOG_CONFIGURATION_INFORMATION_V2
    SystemSupportedProcessorArchitectures2, // q: in opt: HANDLE, out: SYSTEM_SUPPORTED_PROCESSOR_ARCHITECTURES_INFORMATION[] // NtQuerySystemInformationEx // 230
    SystemSingleProcessorRelationshipInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX // (EX in: PROCESSOR_NUMBER Processor) // NtQuerySystemInformationEx
    SystemXfgCheckFailureInformation, // q: SYSTEM_XFG_FAILURE_INFORMATION
    SystemIommuStateInformation, // SYSTEM_IOMMU_STATE_INFORMATION // since 22H1
    SystemHypervisorMinrootInformation, // SYSTEM_HYPERVISOR_MINROOT_INFORMATION
    SystemHypervisorBootPagesInformation, // SYSTEM_HYPERVISOR_BOOT_PAGES_INFORMATION
    SystemPointerAuthInformation, // SYSTEM_POINTER_AUTH_INFORMATION
    SystemSecureKernelDebuggerInformation, // NtQuerySystemInformationEx
    SystemOriginalImageFeatureInformation, // q: in: SYSTEM_ORIGINAL_IMAGE_FEATURE_INFORMATION_INPUT, out: SYSTEM_ORIGINAL_IMAGE_FEATURE_INFORMATION_OUTPUT // NtQuerySystemInformationEx
    SystemMemoryNumaInformation, // SYSTEM_MEMORY_NUMA_INFORMATION_INPUT, SYSTEM_MEMORY_NUMA_INFORMATION_OUTPUT // NtQuerySystemInformationEx
    SystemMemoryNumaPerformanceInformation, // SYSTEM_MEMORY_NUMA_PERFORMANCE_INFORMATION_INPUTSYSTEM_MEMORY_NUMA_PERFORMANCE_INFORMATION_INPUT, SYSTEM_MEMORY_NUMA_PERFORMANCE_INFORMATION_OUTPUT // since 24H2 // 240
    SystemCodeIntegritySignedPoliciesFullInformation,
    SystemSecureCoreInformation, // SystemSecureSecretsInformation
    SystemTrustedAppsRuntimeInformation, // SYSTEM_TRUSTEDAPPS_RUNTIME_INFORMATION
    SystemBadPageInformationEx, // SYSTEM_BAD_PAGE_INFORMATION
    SystemResourceDeadlockTimeout, // ULONG
    SystemBreakOnContextUnwindFailureInformation, // ULONG (requires SeDebugPrivilege)
    SystemOslRamdiskInformation, // SYSTEM_OSL_RAMDISK_INFORMATION
    SystemCodeIntegrityPolicyManagementInformation, // since 25H2
    SystemMemoryNumaCacheInformation,
    SystemProcessorFeaturesBitMapInformation,
    MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS;

typedef enum _KTHREAD_STATE
{
    Initialized,
    Ready,
    Running,
    Standby,
    Terminated,
    Waiting,
    Transition,
    DeferredReady,
    GateWaitObsolete,
    WaitingForProcessInSwap,
    MaximumThreadState
} KTHREAD_STATE, * PKTHREAD_STATE;

typedef struct _SYSTEM_THREAD_INFORMATION
{
    LARGE_INTEGER KernelTime;       // Number of 100-nanosecond intervals spent executing kernel code.
    LARGE_INTEGER UserTime;         // Number of 100-nanosecond intervals spent executing user code.
    LARGE_INTEGER CreateTime;       // The date and time when the thread was created.
    ULONG WaitTime;                 // The current time spent in ready queue or waiting (depending on the thread state).
    PVOID StartAddress;             // The initial start address of the thread.
    CLIENT_ID ClientId;             // The identifier of the thread and the process owning the thread.
    KPRIORITY Priority;             // The dynamic priority of the thread.
    KPRIORITY BasePriority;         // The starting priority of the thread.
    ULONG ContextSwitches;          // The total number of context switches performed.
    KTHREAD_STATE ThreadState;      // The current state of the thread.
    KWAIT_REASON WaitReason;        // The current reason the thread is waiting.
} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;


typedef struct _SYSTEM_PROCESS_INFORMATION
{
    ULONG NextEntryOffset;                  // The address of the previous item plus the value in the NextEntryOffset member. For the last item in the array, NextEntryOffset is 0.
    ULONG NumberOfThreads;                  // The NumberOfThreads member contains the number of threads in the process.
    ULONGLONG WorkingSetPrivateSize;        // The total private memory that a process currently has allocated and is physically resident in memory. // since VISTA
    ULONG HardFaultCount;                   // The total number of hard faults for data from disk rather than from in-memory pages. // since WIN7
    ULONG NumberOfThreadsHighWatermark;     // The peak number of threads that were running at any given point in time, indicative of potential performance bottlenecks related to thread management.
    ULONGLONG CycleTime;                    // The sum of the cycle time of all threads in the process.
    LARGE_INTEGER CreateTime;               // Number of 100-nanosecond intervals since the creation time of the process. Not updated during system timezone changes.
    LARGE_INTEGER UserTime;                 // Number of 100-nanosecond intervals the process has executed in user mode.
    LARGE_INTEGER KernelTime;               // Number of 100-nanosecond intervals the process has executed in kernel mode.
    UNICODE_STRING ImageName;               // The file name of the executable image.
    KPRIORITY BasePriority;                 // The starting priority of the process.
    HANDLE UniqueProcessId;                 // The identifier of the process.
    HANDLE InheritedFromUniqueProcessId;    // The identifier of the process that created this process. Not updated and incorrectly refers to processes with recycled identifiers. 
    ULONG HandleCount;                      // The current number of open handles used by the process.
    ULONG SessionId;                        // The identifier of the Remote Desktop Services session under which the specified process is running. 
    ULONG_PTR UniqueProcessKey;             // since VISTA (requires SystemExtendedProcessInformation)
    SIZE_T PeakVirtualSize;                 // The peak size, in bytes, of the virtual memory used by the process.
    SIZE_T VirtualSize;                     // The current size, in bytes, of virtual memory used by the process.
    ULONG PageFaultCount;                   // The total number of page faults for data that is not currently in memory. The value wraps around to zero on average 24 hours.
    SIZE_T PeakWorkingSetSize;              // The peak size, in kilobytes, of the working set of the process.
    SIZE_T WorkingSetSize;                  // The number of pages visible to the process in physical memory. These pages are resident and available for use without triggering a page fault.
    SIZE_T QuotaPeakPagedPoolUsage;         // The peak quota charged to the process for pool usage, in bytes.
    SIZE_T QuotaPagedPoolUsage;             // The quota charged to the process for paged pool usage, in bytes.
    SIZE_T QuotaPeakNonPagedPoolUsage;      // The peak quota charged to the process for nonpaged pool usage, in bytes.
    SIZE_T QuotaNonPagedPoolUsage;          // The current quota charged to the process for nonpaged pool usage.
    SIZE_T PagefileUsage;                   // The total number of bytes of page file storage in use by the process.
    SIZE_T PeakPagefileUsage;               // The maximum number of bytes of page-file storage used by the process.
    SIZE_T PrivatePageCount;                // The number of memory pages allocated for the use by the process.
    LARGE_INTEGER ReadOperationCount;       // The total number of read operations performed.
    LARGE_INTEGER WriteOperationCount;      // The total number of write operations performed.
    LARGE_INTEGER OtherOperationCount;      // The total number of I/O operations performed other than read and write operations.
    LARGE_INTEGER ReadTransferCount;        // The total number of bytes read during a read operation.
    LARGE_INTEGER WriteTransferCount;       // The total number of bytes written during a write operation.
    LARGE_INTEGER OtherTransferCount;       // The total number of bytes transferred during operations other than read and write operations.
    SYSTEM_THREAD_INFORMATION Threads[1];   // This type is not defined in the structure but was added for convenience.
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

typedef struct _KLDR_DATA_TABLE_ENTRY {
    struct _LIST_ENTRY InLoadOrderLinks;
    VOID* ExceptionTable;
    ULONG ExceptionTableSize;
    VOID* GpValue;
    struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;
    VOID* DllBase;
    VOID* EntryPoint;
    ULONG SizeOfImage;
    struct _UNICODE_STRING FullDllName;
    struct _UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    union {
        USHORT SignatureLevel : 4;
        USHORT SignatureType : 3;
        USHORT Unused : 9;
        USHORT EntireField;
    } u1;
    VOID* SectionPointer;
    ULONG CheckSum;
    ULONG CoverageSectionSize;
    VOID* CoverageSection;
    VOID* LoadedImports;
    VOID* Spare;
    ULONG SizeOfImageNotRounded;
    ULONG TimeDateStamp;
} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;

typedef union _KPRCBFLAG {
    volatile LONG PrcbFlags;
    struct {
        ULONG BamQosLevel : 8;
        ULONG PendingQosUpdate : 2;
        ULONG CacheIsolationEnabled : 1;
        ULONG TracepointActive : 1;
        ULONG PrcbFlagsReserved : 20;
    };
} KPRCBFLAG, * PKPRCBFLAG;

typedef struct _KDESCRIPTOR {
    USHORT Pad[3];
    USHORT Limit;
    PVOID Base;
} KDESCRIPTOR, * PKDESCRIPTOR;

typedef struct _KSPECIAL_REGISTERS {
    ULONGLONG Cr0;
    ULONGLONG Cr2;
    ULONGLONG Cr3;
    ULONGLONG Cr4;
    ULONGLONG KernelDr0;
    ULONGLONG KernelDr1;
    ULONGLONG KernelDr2;
    ULONGLONG KernelDr3;
    ULONGLONG KernelDr6;
    ULONGLONG KernelDr7;
    KDESCRIPTOR Gdtr;
    KDESCRIPTOR Idtr;
    USHORT Tr;
    USHORT Ldtr;
    ULONG MxCsr;
    ULONGLONG DebugControl;
    ULONGLONG LastBranchToRip;
    ULONGLONG LastBranchFromRip;
    ULONGLONG LastExceptionToRip;
    ULONGLONG LastExceptionFromRip;
    ULONGLONG Cr8;
    ULONGLONG MsrGsBase;
    ULONGLONG MsrGsSwap;
    ULONGLONG MsrStar;
    ULONGLONG MsrLStar;
    ULONGLONG MsrCStar;
    ULONGLONG MsrSyscallMask;
    ULONGLONG Xcr0;
    ULONGLONG MsrFsBase;
    ULONGLONG SpecialPadding0;
} KSPECIAL_REGISTERS, * PKSPECIAL_REGISTERS;

typedef struct _KPROCESSOR_STATE {
    KSPECIAL_REGISTERS SpecialRegisters;
    struct _CONTEXT ContextFrame;
} KPROCESSOR_STATE, * PKPROCESSOR_STATE;

typedef struct _PP_LOOKASIDE_LIST {
    struct _GENERAL_LOOKASIDE* P;
    struct _GENERAL_LOOKASIDE* L;
} PP_LOOKASIDE_LIST, * PPP_LOOKASIDE_LIST;

typedef struct _KAFFINITY_EX {
    USHORT Count;
    USHORT Size;
    ULONG Reserved;
    ULONGLONG Bitmap[20];
} KAFFINITY_EX, * PKAFFINITY_EX;

typedef struct _KSTATIC_AFFINITY_BLOCK {
    union {
        KAFFINITY_EX KeFlushTbAffinity;
        KAFFINITY_EX KeFlushWbAffinity;
        KAFFINITY_EX KeSyncContextAffinity;
    };
    KAFFINITY_EX KeFlushTbDeepIdleAffinity;
    KAFFINITY_EX KeIpiSendAffinity;
    KAFFINITY_EX KeIpiSendIpiSet;
} KSTATIC_AFFINITY_BLOCK, * PKSTATIC_AFFINITY_BLOCK;

typedef struct _KTIMER_TABLE_STATE {
    ULONGLONG LastTimerExpiration[2];
    ULONG LastTimerHand[2];
} KTIMER_TABLE_STATE, * PKTIMER_TABLE_STATE;

typedef struct _KTIMER_TABLE_ENTRY {
    ULONGLONG Lock;
    struct _LIST_ENTRY Entry;
    union _ULARGE_INTEGER Time;
} KTIMER_TABLE_ENTRY, * PKTIMER_TABLE_ENTRY;

typedef struct _KTIMER_TABLE {
    struct _KTIMER* TimerExpiry[64];
    KTIMER_TABLE_ENTRY TimerEntries[2][256];
    KTIMER_TABLE_STATE TableState;
} KTIMER_TABLE, * PKTIMER_TABLE;

typedef struct _KDPC_LIST {
    struct _SINGLE_LIST_ENTRY ListHead;
    struct _SINGLE_LIST_ENTRY* LastEntry;
} KDPC_LIST, * PKDPC_LIST;

typedef struct _KDPC_DATA {
    KDPC_LIST DpcList;
    ULONGLONG DpcLock;
    volatile LONG DpcQueueDepth;
    ULONG DpcCount;
    struct _KDPC* volatile ActiveDpc;
} KDPC_DATA, * PKDPC_DATA;

typedef struct _RTL_RB_TREE {
    struct _RTL_BALANCED_NODE* Root;
    union {
        UCHAR Encoded : 1;
        struct _RTL_BALANCED_NODE* Min;
    };
} RTL_RB_TREE, * PRTL_RB_TREE;

typedef struct _PROC_IDLE_POLICY {
    UCHAR PromotePercent;
    UCHAR DemotePercent;
    UCHAR PromotePercentBase;
    UCHAR DemotePercentBase;
    UCHAR AllowScaling;
    UCHAR ForceLightIdle;
} PROC_IDLE_POLICY, * PPROC_IDLE_POLICY;

typedef union _PPM_IDLE_SYNCHRONIZATION_STATE {
    LONG AsLong;
    struct {
        LONG RefCount : 24;
        ULONG State : 8;
    };
} PPM_IDLE_SYNCHRONIZATION_STATE, * PPPM_IDLE_SYNCHRONIZATION_STATE;

typedef struct _PROC_FEEDBACK {
    ULONGLONG Lock;
    ULONGLONG CyclesLast;
    ULONGLONG CyclesActive;
    struct _PROC_FEEDBACK_COUNTER* Counters[2];
    ULONGLONG LastUpdateTime;
    ULONGLONG UnscaledTime;
    volatile LONGLONG UnaccountedTime;
    ULONGLONG ScaledTime[2];
    ULONGLONG UnaccountedKernelTime;
    ULONGLONG PerformanceScaledKernelTime;
    ULONG UserTimeLast;
    ULONG KernelTimeLast;
    ULONGLONG IdleGenerationNumberLast;
    ULONGLONG HvActiveTimeLast;
    ULONGLONG StallCyclesLast;
    ULONGLONG StallTime;
    UCHAR KernelTimesIndex;
    UCHAR CounterDiscardsIdleTime;
} PROC_FEEDBACK, * PPROC_FEEDBACK;

typedef struct _PPM_FFH_THROTTLE_STATE_INFO {
    UCHAR EnableLogging;
    ULONG MismatchCount;
    UCHAR Initialized;
    ULONGLONG LastValue;
    union _LARGE_INTEGER LastLogTickCount;
} PPM_FFH_THROTTLE_STATE_INFO, * PPPM_FFH_THROTTLE_STATE_INFO;

typedef struct _PROC_IDLE_SNAP {
    ULONGLONG Time;
    ULONGLONG Idle;
} PROC_IDLE_SNAP, * PPROC_IDLE_SNAP;

typedef struct _PROC_PERF_CHECK_CONTEXT {
    struct _PROC_PERF_DOMAIN* Domain;
    struct _PROC_PERF_CONSTRAINT* Constraint;
    struct _PROC_PERF_CHECK* PerfCheck;
    struct _PROC_PERF_LOAD* Load;
    struct _PROC_PERF_HISTORY* PerfHistory;
    ULONG Utility;
    ULONG AffinitizedUtility;
    ULONG MediaUtility;
    USHORT LatestAffinitizedPercent;
    USHORT AveragePerformancePercent;
    ULONG RelativePerformance;
    UCHAR NtProcessor;
} PROC_PERF_CHECK_CONTEXT, * PPROC_PERF_CHECK_CONTEXT;

typedef struct _PROCESSOR_POWER_STATE {
    struct _PPM_IDLE_STATES* IdleStates;
    struct _PROC_IDLE_ACCOUNTING* IdleAccounting;
    ULONGLONG IdleTimeLast;
    ULONGLONG IdleTimeTotal;
    volatile ULONGLONG IdleTimeEntry;
    ULONGLONG IdleTimeExpiration;
    UCHAR NonInterruptibleTransition;
    UCHAR PepWokenTransition;
    UCHAR HvTargetState;
    UCHAR SoftParked;
    ULONG TargetIdleState;
    PROC_IDLE_POLICY IdlePolicy;
    PPM_IDLE_SYNCHRONIZATION_STATE Synchronization;
    PROC_FEEDBACK PerfFeedback;
    enum _PROC_HYPERVISOR_STATE Hypervisor;
    ULONG LastSysTime;
    ULONGLONG WmiDispatchPtr;
    LONG WmiInterfaceEnabled;
    PPM_FFH_THROTTLE_STATE_INFO FFHThrottleStateInfo;
    struct _KDPC PerfActionDpc;
    volatile LONG PerfActionMask;
    PROC_IDLE_SNAP HvIdleCheck;
    PROC_PERF_CHECK_CONTEXT CheckContext;
    struct _PPM_CONCURRENCY_ACCOUNTING* Concurrency;
    struct _PPM_CONCURRENCY_ACCOUNTING* ClassConcurrency;
    UCHAR ArchitecturalEfficiencyClass;
    UCHAR PerformanceSchedulingClass;
    UCHAR EfficiencySchedulingClass;
    UCHAR Unused;
    UCHAR Parked;
    UCHAR LongPriorQosPeriod;
    union {
        ULONGLONG SnapTimeLast;
        ULONGLONG EnergyConsumed;
    };
    ULONGLONG ActiveTime;
    ULONGLONG TotalTime;
    struct _POP_FX_DEVICE* FxDevice;
    ULONGLONG LastQosTranstionTsc;
    ULONGLONG QosTransitionHysteresis;
    enum _KHETERO_CPU_QOS RequestedQosClass;
    enum _KHETERO_CPU_QOS ResolvedQosClass;
    USHORT QosEquivalencyMask;
    USHORT HwFeedbackTableIndex;
    UCHAR HwFeedbackParkHint;
    UCHAR HwFeedbackPerformanceClass;
    UCHAR HwFeedbackEfficiencyClass;
    UCHAR HeteroCoreType;
} PROCESSOR_POWER_STATE, * PPROCESSOR_POWER_STATE;

typedef struct _SYNCH_COUNTERS {
    ULONG SpinLockAcquireCount;
    ULONG SpinLockContentionCount;
    ULONG SpinLockSpinCount;
    ULONG IpiSendRequestBroadcastCount;
    ULONG IpiSendRequestRoutineCount;
    ULONG IpiSendSoftwareInterruptCount;
    ULONG ExInitializeResourceCount;
    ULONG ExReInitializeResourceCount;
    ULONG ExDeleteResourceCount;
    ULONG ExecutiveResourceAcquiresCount;
    ULONG ExecutiveResourceContentionsCount;
    ULONG ExecutiveResourceReleaseExclusiveCount;
    ULONG ExecutiveResourceReleaseSharedCount;
    ULONG ExecutiveResourceConvertsCount;
    ULONG ExAcqResExclusiveAttempts;
    ULONG ExAcqResExclusiveAcquiresExclusive;
    ULONG ExAcqResExclusiveAcquiresExclusiveRecursive;
    ULONG ExAcqResExclusiveWaits;
    ULONG ExAcqResExclusiveNotAcquires;
    ULONG ExAcqResSharedAttempts;
    ULONG ExAcqResSharedAcquiresExclusive;
    ULONG ExAcqResSharedAcquiresShared;
    ULONG ExAcqResSharedAcquiresSharedRecursive;
    ULONG ExAcqResSharedWaits;
    ULONG ExAcqResSharedNotAcquires;
    ULONG ExAcqResSharedStarveExclusiveAttempts;
    ULONG ExAcqResSharedStarveExclusiveAcquiresExclusive;
    ULONG ExAcqResSharedStarveExclusiveAcquiresShared;
    ULONG ExAcqResSharedStarveExclusiveAcquiresSharedRecursive;
    ULONG ExAcqResSharedStarveExclusiveWaits;
    ULONG ExAcqResSharedStarveExclusiveNotAcquires;
    ULONG ExAcqResSharedWaitForExclusiveAttempts;
    ULONG ExAcqResSharedWaitForExclusiveAcquiresExclusive;
    ULONG ExAcqResSharedWaitForExclusiveAcquiresShared;
    ULONG ExAcqResSharedWaitForExclusiveAcquiresSharedRecursive;
    ULONG ExAcqResSharedWaitForExclusiveWaits;
    ULONG ExAcqResSharedWaitForExclusiveNotAcquires;
    ULONG ExSetResOwnerPointerExclusive;
    ULONG ExSetResOwnerPointerSharedNew;
    ULONG ExSetResOwnerPointerSharedOld;
    ULONG ExTryToAcqExclusiveAttempts;
    ULONG ExTryToAcqExclusiveAcquires;
    ULONG ExBoostExclusiveOwner;
    ULONG ExBoostSharedOwners;
    ULONG ExEtwSynchTrackingNotificationsCount;
    ULONG ExEtwSynchTrackingNotificationsAccountedCount;
} SYNCH_COUNTERS, * PSYNCH_COUNTERS;

typedef struct _FILESYSTEM_DISK_COUNTERS {
    ULONGLONG FsBytesRead;
    ULONGLONG FsBytesWritten;
} FILESYSTEM_DISK_COUNTERS, * PFILESYSTEM_DISK_COUNTERS;

typedef struct _KENTROPY_TIMING_STATE {
    ULONG EntropyCount;
    ULONG Buffer[64];
    struct _KDPC Dpc;
    ULONG LastDeliveredBuffer;
} KENTROPY_TIMING_STATE, * PKENTROPY_TIMING_STATE;

typedef struct _IOP_IRP_STACK_PROFILER {
    ULONG Profile[20];
    ULONG TotalIrps;
} IOP_IRP_STACK_PROFILER, * PIOP_IRP_STACK_PROFILER;

typedef struct _KSECURE_FAULT_INFORMATION {
    ULONGLONG FaultCode;
    ULONGLONG FaultVa;
} KSECURE_FAULT_INFORMATION, * PKSECURE_FAULT_INFORMATION;

typedef struct _KSHARED_READY_QUEUE {
    ULONGLONG Lock;
    ULONG ReadySummary;
    struct _LIST_ENTRY ReadyListHead[32];
    CHAR RunningSummary[64];
    UCHAR Span;
    UCHAR LowProcIndex;
    UCHAR QueueIndex;
    UCHAR ProcCount;
    UCHAR ScanOwner;
    UCHAR Spare[3];
    ULONGLONG Affinity;
    ULONG ReadyThreadCount;
    ULONGLONG ReadyQueueExpectedRunTime;
} KSHARED_READY_QUEUE, * PKSHARED_READY_QUEUE;

typedef struct _KTIMER_EXPIRATION_TRACE {
    ULONGLONG InterruptTime;
    union _LARGE_INTEGER PerformanceCounter;
} KTIMER_EXPIRATION_TRACE, * PKTIMER_EXPIRATION_TRACE;

typedef struct _MACHINE_FRAME {
    ULONGLONG Rip;
    USHORT SegCs;
    USHORT Fill1[3];
    ULONG EFlags;
    ULONG Fill2;
    ULONGLONG Rsp;
    USHORT SegSs;
    USHORT Fill3[3];
} MACHINE_FRAME, * PMACHINE_FRAME;

typedef struct _MACHINE_CHECK_CONTEXT {
    MACHINE_FRAME MachineFrame;
    ULONGLONG Rax;
    ULONGLONG Rcx;
    ULONGLONG Rdx;
    ULONGLONG GsBase;
    ULONGLONG Cr3;
} MACHINE_CHECK_CONTEXT, * PMACHINE_CHECK_CONTEXT;

typedef struct _KREQUEST_PACKET {
    PVOID CurrentPacket[3];
    VOID(*WorkerRoutine)(PVOID, PVOID, PVOID, PVOID);
} KREQUEST_PACKET, * PKREQUEST_PACKET;

typedef struct _REQUEST_MAILBOX {
    struct _REQUEST_MAILBOX* Next;
    ULONGLONG RequestSummary;
    KREQUEST_PACKET RequestPacket;
    volatile LONG* NodeTargetCountAddr;
    volatile LONG NodeTargetCount;
} REQUEST_MAILBOX, * PREQUEST_MAILBOX;

typedef struct _KPRCB {
    ULONG MxCsr;
    UCHAR LegacyNumber;
    UCHAR ReservedMustBeZero;
    UCHAR InterruptRequest;
    UCHAR IdleHalt;
    struct _KTHREAD* CurrentThread;
    struct _KTHREAD* NextThread;
    struct _KTHREAD* IdleThread;
    UCHAR NestingLevel;
    UCHAR ClockOwner;
    union {
        UCHAR PendingTickFlags;
        struct {
            UCHAR PendingTick : 1;
            UCHAR PendingBackupTick : 1;
        };
    };
    UCHAR IdleState;
    ULONG Number;
    ULONGLONG RspBase;
    ULONGLONG PrcbLock;
    CHAR* PriorityState;
    CHAR CpuType;
    CHAR CpuID;
    union {
        USHORT CpuStep;
        struct {
            UCHAR CpuStepping;
            UCHAR CpuModel;
        };
    };
    ULONG MHz;
    ULONGLONG HalReserved[8];
    USHORT MinorVersion;
    USHORT MajorVersion;
    UCHAR BuildType;
    UCHAR CpuVendor;
    UCHAR LegacyCoresPerPhysicalProcessor;
    UCHAR LegacyLogicalProcessorsPerCore;
    ULONGLONG TscFrequency;
    ULONG CoresPerPhysicalProcessor;
    ULONG LogicalProcessorsPerCore;
    ULONGLONG PrcbPad04[4];
    struct _KNODE* ParentNode;
    ULONGLONG GroupSetMember;
    UCHAR Group;
    UCHAR GroupIndex;
    UCHAR PrcbPad05[2];
    ULONG InitialApicId;
    ULONG ScbOffset;
    ULONG ApicMask;
    PVOID AcpiReserved;
    ULONG CFlushSize;
    KPRCBFLAG PrcbFlags;
    union {
        struct {
            ULONGLONG TrappedSecurityDomain;
            union {
                UCHAR BpbState;
                struct {
                    UCHAR BpbCpuIdle : 1;
                    UCHAR BpbFlushRsbOnTrap : 1;
                    UCHAR BpbIbpbOnReturn : 1;
                    UCHAR BpbIbpbOnTrap : 1;
                    UCHAR BpbIbpbOnRetpolineExit : 1;
                    UCHAR BpbFlushRsbOnReturn : 1;
                    UCHAR BpbFlushRsbOnRetpolineExit : 1;
                    UCHAR BpbStateReserved : 1;
                };
            };
            union {
                UCHAR BpbFeatures;
                struct {
                    UCHAR BpbClearOnIdle : 1;
                    UCHAR BpbEnabled : 1;
                    UCHAR BpbSmep : 1;
                    UCHAR BpbFeaturesReserved : 5;
                };
            };
            UCHAR BpbCurrentSpecCtrl;
            UCHAR BpbKernelSpecCtrl;
            UCHAR BpbNmiSpecCtrl;
            UCHAR BpbUserSpecCtrl;
            volatile SHORT PairRegister;
        };
        ULONGLONG PrcbPad11[2];
    };
    KPROCESSOR_STATE ProcessorState;
    struct _XSAVE_AREA_HEADER* ExtendedSupervisorState;
    ULONG ProcessorSignature;
    ULONG ProcessorFlags;
    union {
        struct {
            UCHAR BpbRetpolineExitSpecCtrl;
            UCHAR BpbTrappedRetpolineExitSpecCtrl;
            union {
                UCHAR BpbTrappedBpbState;
                struct {
                    UCHAR BpbTrappedCpuIdle : 1;
                    UCHAR BpbTrappedFlushRsbOnTrap : 1;
                    UCHAR BpbTrappedIbpbOnReturn : 1;
                    UCHAR BpbTrappedIbpbOnTrap : 1;
                    UCHAR BpbTrappedIbpbOnRetpolineExit : 1;
                    UCHAR BpbTrappedFlushRsbOnReturn : 1;
                    UCHAR BpbTrappedFlushRsbOnRetpolineExit : 1;
                    UCHAR BpbTrappedBpbStateReserved : 1;
                };
            };
            union {
                UCHAR BpbRetpolineState;
                struct {
                    UCHAR BpbRunningNonRetpolineCode : 1;
                    UCHAR BpbIndirectCallsSafe : 1;
                    UCHAR BpbRetpolineEnabled : 1;
                    UCHAR BpbRetpolineStateReserved : 5;
                };
            };
            ULONG PrcbPad12b;
        };
        ULONGLONG PrcbPad12a;
    };
    ULONGLONG PrcbPad12[3];
    struct _KSPIN_LOCK_QUEUE LockQueue[17];
    PP_LOOKASIDE_LIST PPLookasideList[16];
    struct _GENERAL_LOOKASIDE_POOL PPNxPagedLookasideList[32];
    struct _GENERAL_LOOKASIDE_POOL PPNPagedLookasideList[32];
    struct _GENERAL_LOOKASIDE_POOL PPPagedLookasideList[32];
    ULONGLONG MsrIa32TsxCtrl;
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;
    volatile LONG MmPageFaultCount;
    volatile LONG MmCopyOnWriteCount;
    volatile LONG MmTransitionCount;
    volatile LONG MmDemandZeroCount;
    volatile LONG MmPageReadCount;
    volatile LONG MmPageReadIoCount;
    volatile LONG MmDirtyPagesWriteCount;
    volatile LONG MmDirtyWriteIoCount;
    volatile LONG MmMappedPagesWriteCount;
    volatile LONG MmMappedWriteIoCount;
    ULONG KeSystemCalls;
    ULONG KeContextSwitches;
    ULONG PrcbPad40;
    ULONG CcFastReadNoWait;
    ULONG CcFastReadWait;
    ULONG CcFastReadNotPossible;
    ULONG CcCopyReadNoWait;
    ULONG CcCopyReadWait;
    ULONG CcCopyReadNoWaitMiss;
    volatile LONG IoReadOperationCount;
    volatile LONG IoWriteOperationCount;
    volatile LONG IoOtherOperationCount;
    union _LARGE_INTEGER IoReadTransferCount;
    union _LARGE_INTEGER IoWriteTransferCount;
    union _LARGE_INTEGER IoOtherTransferCount;
    volatile LONG PacketBarrier;
    volatile LONG TargetCount;
    volatile ULONG IpiFrozen;
    ULONG PrcbPad30;
    PVOID IsrDpcStats;
    ULONG DeviceInterrupts;
    LONG LookasideIrpFloat;
    ULONG InterruptLastCount;
    ULONG InterruptRate;
    ULONGLONG PrcbPad31;
    struct _KPRCB* PairPrcb;
    KSTATIC_AFFINITY_BLOCK StaticAffinity;
    ULONGLONG PrcbPad35[5];
    union _SLIST_HEADER InterruptObjectPool;
    struct _RTL_HASH_TABLE* DpcRuntimeHistoryHashTable;
    struct _KDPC* DpcRuntimeHistoryHashTableCleanupDpc;
    VOID(*CurrentDpcRoutine)(struct _KDPC*, PVOID, PVOID, PVOID);
    ULONGLONG CurrentDpcRuntimeHistoryCached;
    ULONGLONG CurrentDpcStartTime;
    ULONGLONG PrcbPad41[1];
    KDPC_DATA DpcData[2];
    PVOID DpcStack;
    LONG MaximumDpcQueueDepth;
    ULONG DpcRequestRate;
    ULONG MinimumDpcRate;
    ULONG DpcLastCount;
    UCHAR ThreadDpcEnable;
    volatile UCHAR QuantumEnd;
    volatile UCHAR DpcRoutineActive;
    volatile UCHAR IdleSchedule;
    union {
        volatile LONG DpcRequestSummary;
        SHORT DpcRequestSlot[2];
        struct {
            SHORT NormalDpcState;
            SHORT ThreadDpcState;
        };
        struct {
            ULONG DpcNormalProcessingActive : 1;
            ULONG DpcNormalProcessingRequested : 1;
            ULONG DpcNormalThreadSignal : 1;
            ULONG DpcNormalTimerExpiration : 1;
            ULONG DpcNormalDpcPresent : 1;
            ULONG DpcNormalLocalInterrupt : 1;
            ULONG DpcNormalSpare : 10;
            ULONG DpcThreadActive : 1;
            ULONG DpcThreadRequested : 1;
            ULONG DpcThreadSpare : 14;
        };
    };
    ULONG PrcbPad93;
    ULONG LastTick;
    ULONG ClockInterrupts;
    ULONG ReadyScanTick;
    PVOID InterruptObject[256];
    KTIMER_TABLE TimerTable;
    ULONG PrcbPad92[10];
    struct _KGATE DpcGate;
    PVOID PrcbPad52;
    struct _KDPC CallDpc;
    LONG ClockKeepAlive;
    UCHAR PrcbPad60[2];
    union {
        struct {
            UCHAR NmiActive;
            UCHAR MceActive;
        };
        USHORT CombinedNmiMceActive;
    };
    LONG DpcWatchdogPeriod;
    LONG DpcWatchdogCount;
    volatile LONG KeSpinLockOrdering;
    ULONG DpcWatchdogProfileCumulativeDpcThreshold;
    PVOID CachedPtes;
    struct _LIST_ENTRY WaitListHead;
    ULONGLONG WaitLock;
    ULONG ReadySummary;
    LONG AffinitizedSelectionMask;
    ULONG QueueIndex;
    ULONG PrcbPad75[2];
    ULONG DpcWatchdogSequenceNumber;
    struct _KDPC TimerExpirationDpc;
    RTL_RB_TREE ScbQueue;
    struct _LIST_ENTRY DispatcherReadyListHead[32];
    ULONG InterruptCount;
    ULONG KernelTime;
    ULONG UserTime;
    ULONG DpcTime;
    ULONG InterruptTime;
    ULONG AdjustDpcThreshold;
    UCHAR DebuggerSavedIRQL;
    UCHAR GroupSchedulingOverQuota;
    volatile UCHAR DeepSleep;
    UCHAR PrcbPad80;
    ULONG DpcTimeCount;
    ULONG DpcTimeLimit;
    ULONG PeriodicCount;
    ULONG PeriodicBias;
    ULONG AvailableTime;
    ULONG KeExceptionDispatchCount;
    ULONG ReadyThreadCount;
    ULONGLONG ReadyQueueExpectedRunTime;
    ULONGLONG StartCycles;
    ULONGLONG TaggedCyclesStart;
    ULONGLONG TaggedCycles[3];
    ULONGLONG AffinitizedCycles;
    ULONGLONG ImportantCycles;
    ULONGLONG UnimportantCycles;
    ULONG DpcWatchdogProfileSingleDpcThreshold;
    volatile LONG MmSpinLockOrdering;
    PVOID volatile CachedStack;
    ULONG PageColor;
    ULONG NodeColor;
    ULONG NodeShiftedColor;
    ULONG SecondaryColorMask;
    UCHAR PrcbPad81[6];
    UCHAR ExceptionStackActive;
    UCHAR TbFlushListActive;
    PVOID ExceptionStack;
    ULONGLONG PrcbPad82[1];
    ULONGLONG CycleTime;
    ULONGLONG Cycles[4][2];
    ULONG CcFastMdlReadNoWait;
    ULONG CcFastMdlReadWait;
    ULONG CcFastMdlReadNotPossible;
    ULONG CcMapDataNoWait;
    ULONG CcMapDataWait;
    ULONG CcPinMappedDataCount;
    ULONG CcPinReadNoWait;
    ULONG CcPinReadWait;
    ULONG CcMdlReadNoWait;
    ULONG CcMdlReadWait;
    ULONG CcLazyWriteHotSpots;
    ULONG CcLazyWriteIos;
    ULONG CcLazyWritePages;
    ULONG CcDataFlushes;
    ULONG CcDataPages;
    ULONG CcLostDelayedWrites;
    ULONG CcFastReadResourceMiss;
    ULONG CcCopyReadWaitMiss;
    ULONG CcFastMdlReadResourceMiss;
    ULONG CcMapDataNoWaitMiss;
    ULONG CcMapDataWaitMiss;
    ULONG CcPinReadNoWaitMiss;
    ULONG CcPinReadWaitMiss;
    ULONG CcMdlReadNoWaitMiss;
    ULONG CcMdlReadWaitMiss;
    ULONG CcReadAheadIos;
    volatile LONG MmCacheTransitionCount;
    volatile LONG MmCacheReadCount;
    volatile LONG MmCacheIoCount;
    ULONG PrcbPad91;
    PVOID MmInternal;
    PROCESSOR_POWER_STATE PowerState;
    PVOID HyperPte;
    struct _LIST_ENTRY ScbList;
    struct _KDPC ForceIdleDpc;
    struct _KDPC DpcWatchdogDpc;
    struct _KTIMER DpcWatchdogTimer;
    struct _CACHE_DESCRIPTOR Cache[5];
    ULONG CacheCount;
    volatile ULONG CachedCommit;
    volatile ULONG CachedResidentAvailable;
    PVOID WheaInfo;
    PVOID EtwSupport;
    PVOID ExSaPageArray;
    ULONG KeAlignmentFixupCount;
    ULONG PrcbPad95;
    union _SLIST_HEADER HypercallPageList;
    ULONGLONG* StatisticsPage;
    ULONGLONG GenerationTarget;
    ULONGLONG PrcbPad85[4];
    PVOID HypercallCachedPages;
    PVOID VirtualApicAssist;
    KAFFINITY_EX PackageProcessorSet;
    ULONG PackageId;
    ULONG PrcbPad86;
    ULONGLONG SharedReadyQueueMask;
    KSHARED_READY_QUEUE* SharedReadyQueue;
    ULONG SharedQueueScanOwner;
    ULONG ScanSiblingIndex;
    ULONGLONG CoreProcessorSet;
    ULONGLONG ScanSiblingMask;
    ULONGLONG LLCMask;
    ULONGLONG CacheProcessorMask[5];
    struct _PROCESSOR_PROFILE_CONTROL_AREA* ProcessorProfileControlArea;
    PVOID ProfileEventIndexAddress;
    PVOID* DpcWatchdogProfile;
    PVOID* DpcWatchdogProfileCurrentEmptyCapture;
    PVOID SchedulerAssist;
    SYNCH_COUNTERS SynchCounters;
    ULONGLONG PrcbPad94;
    FILESYSTEM_DISK_COUNTERS FsCounters;
    UCHAR VendorString[13];
    UCHAR PrcbPad100[3];
    ULONGLONG FeatureBits;
    union _LARGE_INTEGER UpdateSignature;
    ULONGLONG PteBitCache;
    ULONG PteBitOffset;
    ULONG PrcbPad105;
    struct _CONTEXT* Context;
    ULONG ContextFlagsInit;
    ULONG PrcbPad115;
    struct _XSAVE_AREA* ExtendedState;
    PVOID IsrStack;
    KENTROPY_TIMING_STATE EntropyTimingState;
    ULONGLONG PrcbPad110;
    struct {
        ULONG UpdateCycle;
        union {
            SHORT PairLocal;
            struct {
                UCHAR PairLocalLow;
                UCHAR PairLocalForceStibp : 1;
                UCHAR Reserved : 4;
                UCHAR Frozen : 1;
                UCHAR ForceUntrusted : 1;
                UCHAR SynchIpi : 1;
            };
        };
        union {
            SHORT PairRemote;
            struct {
                UCHAR PairRemoteLow;
                UCHAR Reserved2;
            };
        };
        UCHAR Trace[24];
        ULONGLONG LocalDomain;
        ULONGLONG RemoteDomain;
        struct _KTHREAD* Thread;
    } StibpPairingTrace;
    struct _SINGLE_LIST_ENTRY AbSelfIoBoostsList;
    struct _SINGLE_LIST_ENTRY AbPropagateBoostsList;
    struct _KDPC AbDpc;
    IOP_IRP_STACK_PROFILER IoIrpStackProfilerCurrent;
    IOP_IRP_STACK_PROFILER IoIrpStackProfilerPrevious;
    KSECURE_FAULT_INFORMATION SecureFault;
    ULONGLONG PrcbPad120;
    KSHARED_READY_QUEUE LocalSharedReadyQueue;
    ULONGLONG PrcbPad125[2];
    ULONG TimerExpirationTraceCount;
    ULONG PrcbPad127;
    KTIMER_EXPIRATION_TRACE TimerExpirationTrace[16];
    ULONGLONG PrcbPad128[7];
    REQUEST_MAILBOX* Mailbox;
    ULONGLONG PrcbPad130[7];
    MACHINE_CHECK_CONTEXT McheckContext[2];
    ULONGLONG PrcbPad134[4];
    struct _KLOCK_QUEUE_HANDLE SelfmapLockHandle[4];
    ULONGLONG PrcbPad134a[4];
    UCHAR PrcbPad138[128];
    UCHAR PrcbPad138a[64];
    ULONGLONG KernelDirectoryTableBase;
    ULONGLONG RspBaseShadow;
    ULONGLONG UserRspShadow;
    ULONG ShadowFlags;
    ULONG PrcbPad138b;
    ULONGLONG PrcbPad138c;
    USHORT PrcbPad138d;
    USHORT VerwSelector;
    ULONG DbgMceNestingLevel;
    ULONG DbgMceFlags;
    ULONG PrcbPad139b;
    ULONGLONG PrcbPad140[505];
    ULONGLONG PrcbPad140a[8];
    ULONGLONG PrcbPad141[504];
    UCHAR PrcbPad141a[64];
    REQUEST_MAILBOX RequestMailbox[1];
} KPRCB, * PKPRCB;

