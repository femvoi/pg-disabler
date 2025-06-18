#pragma once

#include <ntddk.h>
#include <ntifs.h>
#include <stdint.h>
#include <ntimage.h>

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

