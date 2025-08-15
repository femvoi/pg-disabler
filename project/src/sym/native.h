#ifndef native_h
#define native_h	

extern "C" {
    extern LIST_ENTRY* PsLoadedModuleList;

    u64 RtlFindExportedRoutineByName(
        u64 image_base,
        const char* routine_name
    );

    u64 RtlImageDirectoryEntryToData(
        u64 image_base,
        bool mapped_as_image,
        u16 directory_entry,
        u32* size
    );

    u64 PsGetProcessSectionBaseAddress(
        PEPROCESS process
    );

    u64 PsGetProcessPeb(
        PEPROCESS process
    );
}

typedef struct _KLDR_DATA_TABLE_ENTRY {
    LIST_ENTRY in_load_order_links;
    u64 exception_table;
    u32 exception_table_size;
    u64 gp_value;
    u64 non_paged_debug_info;
    u64 image_base;
    u64 image_entry;
    u32 image_size;
    UNICODE_STRING image_full_name;
    UNICODE_STRING image_base_name;
    u32 flags;
    u16 load_count;
    union {
        u16 signature_level : 4;
        u16 signature_type : 3;
        u16 unused : 9;
        u16 entire_field;
    } signature_field;
    u64 section_pointer;
    u32 check_sum;
    u32 converage_section_size;
    u64 coverage_section;
    u64 loaded_imports;
    u64 spare;
    u32 size_of_image_non_rounded;
    u32 time_date_stamped;
}KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;

union _KGDTENTRY64 {
    struct {
        u16 LimitLow;
        u16 BaseLow;
    };

    struct {
        u8 BaseMiddle;
        u8 Flags1;
        u8 Flags2;
        u8 BaseHigh;
    } bytes;

    struct {
        struct {
            u32 BaseMiddle : 8; 
            u32 Type : 5;       
            u32 Dpl : 2;        
            u32 Present : 1;    
            u32 LimitHigh : 4;  
            u32 System : 1;     
            u32 LongMode : 1;   
            u32 DefaultBig : 1; 
            u32 Granularity : 1;
            u32 BaseHigh : 8;   
        } bits;
        u32 BaseUpper;
    };

    struct {
        u32 MustBeZero;
        s64 DataLow;
    };

    s64 DataHigh;
};

union _KIDTENTRY64 {
    struct {
        u16 OffsetLow;
        u16 Selector;
    };

    u16 IstIndex : 3;
    u16 Reserved0 : 5;
    u16 Type : 5;
    u16 Dpl : 2;

    struct {
        u16 Present : 1;
        u16 OffsetMiddle;
    };

    struct {
        u32 OffsetHigh;
        u32 Reserved1;
    };

    u64 Alignment;
};

typedef struct _KTSS64 {
    u32 Reserved0;
    u64 Rsp0;
    u64 Rsp1;
    u64 Rsp2;
    u64 Ist[8];
    u64 Reserved1;
    u16 Reserved2;
    u16 IoMapBase;
}KTSS64, *PKTSS64;

typedef struct _MACHINE_FRAME {
    u64 Rip;
    u16 SegCs;
    u16 Fill1[3];
    u32 EFlags;
    u32 Fill2;
    u64 Rsp;
    u16 SegSs;
    u16 Fill3[3];
}MACHINE_FRAME, *PMACHINE_FRAME;

typedef struct _KAFFINITY_EX {
    u16 Count;
    u16 Size;
    u32 Reserved;
    u64 Bitmap[20];
} KAFFINITY_EX, * PKAFFINITY_EX;

typedef union _KWAIT_STATUS_REGISTER {
    u8 Flags;
    struct {
        u8 State : 3;
        u8 Affinity : 1;
        u8 Priority : 1;
        u8 Apc : 1;
        u8 UserApc : 1;
        u8 Alert : 1;
    };
} KWAIT_STATUS_REGISTER, * PKWAIT_STATUS_REGISTER;

typedef struct _KTHREAD {
    struct _DISPATCHER_HEADER Header;
    void* SListFaultAddress;
    u64 QuantumTarget;
    void* InitialStack;
    volatile void* StackLimit;
    void* StackBase;
    u64 ThreadLock;
    volatile u64 CycleTime;
    u32 CurrentRunTime;
    u32 ExpectedRunTime;
    void* KernelStack;
    struct _XSAVE_FORMAT* StateSaveArea;
    volatile struct _KSCHEDULING_GROUP* SchedulingGroup;
    KWAIT_STATUS_REGISTER WaitRegister;
    volatile u8 Running;
    u8 Alerted[2];

    union {
        struct {
            u32 SpareMiscFlag0 : 1;
            u32 ReadyTransition : 1;
            u32 ProcessReadyQueue : 1;
            u32 WaitNext : 1;
            u32 SystemAffinityActive : 1;
            u32 Alertable : 1;
            u32 UserStackWalkActive : 1;
            u32 ApcInterruptRequest : 1;
            u32 QuantumEndMigrate : 1;
            u32 UmsDirectedSwitchEnable : 1;
            u32 TimerActive : 1;
            u32 SystemThread : 1;
            u32 ProcessDetachActive : 1;
            u32 CalloutActive : 1;
            u32 ScbReadyQueue : 1;
            u32 ApcQueueable : 1;
            u32 ReservedStackInUse : 1;
            u32 UmsPerformingSyscall : 1;
            u32 ApcPendingReload : 1;
            u32 TimerSuspended : 1;
            u32 SuspendedWaitMode : 1;
            u32 Reserved : 11;
        };
        s32 flag;
    }misc_flags;

    union {
        struct {
            u32 AutoAlignment : 1;
            u32 DisableBoost : 1;
            u32 UserAffinitySet : 1;
            u32 AlertedByThreadId : 1;
            u32 QuantumDonation : 1;
            u32 EnableStackSwap : 1;
            u32 GuiThread : 1;
            u32 DisableQuantum : 1;
            u32 ChargeOnlySchedulingGroup : 1;
            u32 DeferPreemption : 1;
            u32 QueueDeferPreemption : 1;
            u32 ForceDeferSchedule : 1;
            u32 SharedReadyQueueAffinity : 1;
            u32 FreezeCount : 1;
            u32 TerminationApcRequest : 1;
            u32 AutoBoostEntriesExhausted : 1;
            u32 KernelStackResident : 1;
            u32 EtwStackTraceApcInserted : 8;
            u32 ReservedFlags : 7;
        };
        volatile s32 flag;
    }thread_flags;

    u32 Spare0;
    u32 SystemCallNumber;
    u32 Spare1;
    void* FirstArgument;
    struct _KTRAP_FRAME* TrapFrame;
} KTHREAD, * PKTHREAD;

typedef union _KPRCBFLAG {
    volatile s32 PrcbFlags;
    struct {
        u32 BamQosLevel : 8;
        u32 PendingQosUpdate : 2;
        u32 CacheIsolationEnabled : 1;
        u32 TracepointActive : 1;
        u32 PrcbFlagsReserved : 20;
    };
} KPRCBFLAG, * PKPRCBFLAG;

typedef struct _KDESCRIPTOR {
    u16 Pad[3];
    u16 Limit;
    void* Base;
} KDESCRIPTOR, * PKDESCRIPTOR;

typedef struct _KSPECIAL_REGISTERS {
    u64 Cr0;
    u64 Cr2;
    u64 Cr3;
    u64 Cr4;
    u64 KernelDr0;
    u64 KernelDr1;
    u64 KernelDr2;
    u64 KernelDr3;
    u64 KernelDr6;
    u64 KernelDr7;
    KDESCRIPTOR Gdtr;
    KDESCRIPTOR Idtr;
    u16 Tr;
    u16 Ldtr;
    u32 MxCsr;
    u64 DebugControl;
    u64 LastBranchToRip;
    u64 LastBranchFromRip;
    u64 LastExceptionToRip;
    u64 LastExceptionFromRip;
    u64 Cr8;
    u64 MsrGsBase;
    u64 MsrGsSwap;
    u64 MsrStar;
    u64 MsrLStar;
    u64 MsrCStar;
    u64 MsrSyscallMask;
    u64 Xcr0;
    u64 MsrFsBase;
    u64 SpecialPadding0;
} KSPECIAL_REGISTERS, * PKSPECIAL_REGISTERS;

typedef struct _KPROCESSOR_STATE {
    KSPECIAL_REGISTERS SpecialRegisters;
    struct _CONTEXT ContextFrame;
} KPROCESSOR_STATE, * PKPROCESSOR_STATE;

typedef struct _PP_LOOKASIDE_LIST {
    struct _GENERAL_LOOKASIDE* P;
    struct _GENERAL_LOOKASIDE* L;
} PP_LOOKASIDE_LIST, * PPP_LOOKASIDE_LIST;

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
    u64 LastTimerExpiration[2];
    u32 LastTimerHand[2];
} KTIMER_TABLE_STATE, * PKTIMER_TABLE_STATE;

typedef struct _KTIMER_TABLE_ENTRY {
    u64 Lock;
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
    u64 DpcLock;
    volatile s32 DpcQueueDepth;
    u32 DpcCount;
    struct _KDPC* volatile ActiveDpc;
} KDPC_DATA, * PKDPC_DATA;

typedef struct _RTL_RB_TREE {
    struct _RTL_BALANCED_NODE* Root;
    union {
        u8 Encoded : 1;
        struct _RTL_BALANCED_NODE* Min;
    };
} RTL_RB_TREE, * PRTL_RB_TREE;

typedef struct _PROC_IDLE_POLICY {
    u8 PromotePercent;
    u8 DemotePercent;
    u8 PromotePercentBase;
    u8 DemotePercentBase;
    u8 AllowScaling;
    u8 ForceLightIdle;
} PROC_IDLE_POLICY, * PPROC_IDLE_POLICY;

typedef union _PPM_IDLE_SYNCHRONIZATION_STATE {
    s32 AsLong;
    struct {
        s32 RefCount : 24;
        u32 State : 8;
    };
} PPM_IDLE_SYNCHRONIZATION_STATE, * PPPM_IDLE_SYNCHRONIZATION_STATE;

typedef struct _PROC_FEEDBACK {
    u64 Lock;
    u64 CyclesLast;
    u64 CyclesActive;
    struct _PROC_FEEDBACK_COUNTER* Counters[2];
    u64 LastUpdateTime;
    u64 UnscaledTime;
    volatile s64 UnaccountedTime;
    u64 ScaledTime[2];
    u64 UnaccountedKernelTime;
    u64 PerformanceScaledKernelTime;
    u32 UserTimeLast;
    u32 KernelTimeLast;
    u64 IdleGenerationNumberLast;
    u64 HvActiveTimeLast;
    u64 StallCyclesLast;
    u64 StallTime;
    u8 KernelTimesIndex;
    u8 CounterDiscardsIdleTime;
} PROC_FEEDBACK, * PPROC_FEEDBACK;

typedef struct _PPM_FFH_THROTTLE_STATE_INFO {
    u8 EnableLogging;
    u32 MismatchCount;
    u8 Initialized;
    u64 LastValue;
    union _LARGE_INTEGER LastLogTickCount;
} PPM_FFH_THROTTLE_STATE_INFO, * PPPM_FFH_THROTTLE_STATE_INFO;

typedef struct _PROC_IDLE_SNAP {
    u64 Time;
    u64 Idle;
} PROC_IDLE_SNAP, * PPROC_IDLE_SNAP;

typedef struct _PROC_PERF_CHECK_CONTEXT {
    struct _PROC_PERF_DOMAIN* Domain;
    struct _PROC_PERF_CONSTRAINT* Constraint;
    struct _PROC_PERF_CHECK* PerfCheck;
    struct _PROC_PERF_LOAD* Load;
    struct _PROC_PERF_HISTORY* PerfHistory;
    u32 Utility;
    u32 AffinitizedUtility;
    u32 MediaUtility;
    u16 LatestAffinitizedPercent;
    u16 AveragePerformancePercent;
    u32 RelativePerformance;
    u8 NtProcessor;
} PROC_PERF_CHECK_CONTEXT, * PPROC_PERF_CHECK_CONTEXT;

typedef struct _PROCESSOR_POWER_STATE {
    struct _PPM_IDLE_STATES* IdleStates;
    struct _PROC_IDLE_ACCOUNTING* IdleAccounting;
    u64 IdleTimeLast;
    u64 IdleTimeTotal;
    volatile u64 IdleTimeEntry;
    u64 IdleTimeExpiration;
    u8 NonInterruptibleTransition;
    u8 PepWokenTransition;
    u8 HvTargetState;
    u8 SoftParked;
    u32 TargetIdleState;
    PROC_IDLE_POLICY IdlePolicy;
    PPM_IDLE_SYNCHRONIZATION_STATE Synchronization;
    PROC_FEEDBACK PerfFeedback;
    enum _PROC_HYPERVISOR_STATE Hypervisor;
    u32 LastSysTime;
    u64 WmiDispatchPtr;
    s32 WmiInterfaceEnabled;
    PPM_FFH_THROTTLE_STATE_INFO FFHThrottleStateInfo;
    struct _KDPC PerfActionDpc;
    volatile s32 PerfActionMask;
    PROC_IDLE_SNAP HvIdleCheck;
    PROC_PERF_CHECK_CONTEXT CheckContext;
    struct _PPM_CONCURRENCY_ACCOUNTING* Concurrency;
    struct _PPM_CONCURRENCY_ACCOUNTING* ClassConcurrency;
    u8 ArchitecturalEfficiencyClass;
    u8 PerformanceSchedulingClass;
    u8 EfficiencySchedulingClass;
    u8 Unused;
    u8 Parked;
    u8 LongPriorQosPeriod;
    union {
        u64 SnapTimeLast;
        u64 EnergyConsumed;
    };
    u64 ActiveTime;
    u64 TotalTime;
    struct _POP_FX_DEVICE* FxDevice;
    u64 LastQosTranstionTsc;
    u64 QosTransitionHysteresis;
    enum _KHETERO_CPU_QOS RequestedQosClass;
    enum _KHETERO_CPU_QOS ResolvedQosClass;
    u16 QosEquivalencyMask;
    u16 HwFeedbackTableIndex;
    u8 HwFeedbackParkHint;
    u8 HwFeedbackPerformanceClass;
    u8 HwFeedbackEfficiencyClass;
    u8 HeteroCoreType;
} PROCESSOR_POWER_STATE, * PPROCESSOR_POWER_STATE;

typedef struct _SYNCH_COUNTERS {
    u32 SpinLockAcquireCount;
    u32 SpinLockContentionCount;
    u32 SpinLockSpinCount;
    u32 IpiSendRequestBroadcastCount;
    u32 IpiSendRequestRoutineCount;
    u32 IpiSendSoftwareInterruptCount;
    u32 ExInitializeResourceCount;
    u32 ExReInitializeResourceCount;
    u32 ExDeleteResourceCount;
    u32 ExecutiveResourceAcquiresCount;
    u32 ExecutiveResourceContentionsCount;
    u32 ExecutiveResourceReleaseExclusiveCount;
    u32 ExecutiveResourceReleaseSharedCount;
    u32 ExecutiveResourceConvertsCount;
    u32 ExAcqResExclusiveAttempts;
    u32 ExAcqResExclusiveAcquiresExclusive;
    u32 ExAcqResExclusiveAcquiresExclusiveRecursive;
    u32 ExAcqResExclusiveWaits;
    u32 ExAcqResExclusiveNotAcquires;
    u32 ExAcqResSharedAttempts;
    u32 ExAcqResSharedAcquiresExclusive;
    u32 ExAcqResSharedAcquiresShared;
    u32 ExAcqResSharedAcquiresSharedRecursive;
    u32 ExAcqResSharedWaits;
    u32 ExAcqResSharedNotAcquires;
    u32 ExAcqResSharedStarveExclusiveAttempts;
    u32 ExAcqResSharedStarveExclusiveAcquiresExclusive;
    u32 ExAcqResSharedStarveExclusiveAcquiresShared;
    u32 ExAcqResSharedStarveExclusiveAcquiresSharedRecursive;
    u32 ExAcqResSharedStarveExclusiveWaits;
    u32 ExAcqResSharedStarveExclusiveNotAcquires;
    u32 ExAcqResSharedWaitForExclusiveAttempts;
    u32 ExAcqResSharedWaitForExclusiveAcquiresExclusive;
    u32 ExAcqResSharedWaitForExclusiveAcquiresShared;
    u32 ExAcqResSharedWaitForExclusiveAcquiresSharedRecursive;
    u32 ExAcqResSharedWaitForExclusiveWaits;
    u32 ExAcqResSharedWaitForExclusiveNotAcquires;
    u32 ExSetResOwnerPointerExclusive;
    u32 ExSetResOwnerPointerSharedNew;
    u32 ExSetResOwnerPointerSharedOld;
    u32 ExTryToAcqExclusiveAttempts;
    u32 ExTryToAcqExclusiveAcquires;
    u32 ExBoostExclusiveOwner;
    u32 ExBoostSharedOwners;
    u32 ExEtwSynchTrackingNotificationsCount;
    u32 ExEtwSynchTrackingNotificationsAccountedCount;
} SYNCH_COUNTERS, * PSYNCH_COUNTERS;

typedef struct _FILESYSTEM_DISK_COUNTERS {
    u64 FsBytesRead;
    u64 FsBytesWritten;
} FILESYSTEM_DISK_COUNTERS, * PFILESYSTEM_DISK_COUNTERS;

typedef struct _KENTROPY_TIMING_STATE {
    u32 EntropyCount;
    u32 Buffer[64];
    struct _KDPC Dpc;
    u32 LastDeliveredBuffer;
} KENTROPY_TIMING_STATE, * PKENTROPY_TIMING_STATE;

typedef struct _IOP_IRP_STACK_PROFILER {
    u32 Profile[20];
    u32 TotalIrps;
} IOP_IRP_STACK_PROFILER, * PIOP_IRP_STACK_PROFILER;

typedef struct _KSECURE_FAULT_INFORMATION {
    u64 FaultCode;
    u64 FaultVa;
} KSECURE_FAULT_INFORMATION, * PKSECURE_FAULT_INFORMATION;

typedef struct _KSHARED_READY_QUEUE {
    u64 Lock;
    u32 ReadySummary;
    struct _LIST_ENTRY ReadyListHead[32];
    s8 RunningSummary[64];
    u8 Span;
    u8 LowProcIndex;
    u8 QueueIndex;
    u8 ProcCount;
    u8 ScanOwner;
    u8 Spare[3];
    u64 Affinity;
    u32 ReadyThreadCount;
    u64 ReadyQueueExpectedRunTime;
} KSHARED_READY_QUEUE, * PKSHARED_READY_QUEUE;

typedef struct _KTIMER_EXPIRATION_TRACE {
    u64 InterruptTime;
    union _LARGE_INTEGER PerformanceCounter;
} KTIMER_EXPIRATION_TRACE, * PKTIMER_EXPIRATION_TRACE;

typedef struct _MACHINE_CHECK_CONTEXT {
    MACHINE_FRAME MachineFrame;
    u64 Rax;
    u64 Rcx;
    u64 Rdx;
    u64 GsBase;
    u64 Cr3;
} MACHINE_CHECK_CONTEXT, * PMACHINE_CHECK_CONTEXT;

typedef struct _KREQUEST_PACKET {
    void* CurrentPacket[3];
    VOID(*WorkerRoutine)(void*, void*, void*, void*);
} KREQUEST_PACKET, * PKREQUEST_PACKET;

typedef struct _REQUEST_MAILBOX {
    struct _REQUEST_MAILBOX* Next;
    u64 RequestSummary;
    KREQUEST_PACKET RequestPacket;
    volatile s32* NodeTargetCountAddr;
    volatile s32 NodeTargetCount;
} REQUEST_MAILBOX, * PREQUEST_MAILBOX;

typedef struct _KPRCB {
    u32 MxCsr;
    u8 LegacyNumber;
    u8 ReservedMustBeZero;
    u8 InterruptRequest;
    u8 IdleHalt;
    struct _KTHREAD* CurrentThread;
    struct _KTHREAD* NextThread;
    struct _KTHREAD* IdleThread;
    u8 NestingLevel;
    u8 ClockOwner;
    union {
        u8 PendingTickFlags;
        struct {
            u8 PendingTick : 1;
            u8 PendingBackupTick : 1;
        };
    };
    u8 IdleState;
    u32 Number;
    u64 RspBase;
    u64 PrcbLock;
    s8* PriorityState;
    s8 CpuType;
    s8 CpuID;
    union {
        u16 CpuStep;
        struct {
            u8 CpuStepping;
            u8 CpuModel;
        };
    };
    u32 MHz;
    u64 HalReserved[8]; // 0x48
    u16 MinorVersion;
    u16 MajorVersion;
    u8 BuildType;
    u8 CpuVendor;
    u8 LegacyCoresPerPhysicalProcessor;
    u8 LegacyLogicalProcessorsPerCore;
    u64 TscFrequency;
    u32 CoresPerPhysicalProcessor;
    u32 LogicalProcessorsPerCore;
    u64 PrcbPad04[4];
    struct _KNODE* ParentNode;
    u64 GroupSetMember;
    u8 Group;
    u8 GroupIndex;
    u8 PrcbPad05[2];
    u32 InitialApicId;
    u32 ScbOffset;
    u32 ApicMask;
    void* AcpiReserved; // 0xe0
    u32 CFlushSize;
    KPRCBFLAG PrcbFlags;
    union {
        struct {
            u64 TrappedSecurityDomain;
            union {
                u8 BpbState;
                struct {
                    u8 BpbCpuIdle : 1;
                    u8 BpbFlushRsbOnTrap : 1;
                    u8 BpbIbpbOnReturn : 1;
                    u8 BpbIbpbOnTrap : 1;
                    u8 BpbIbpbOnRetpolineExit : 1;
                    u8 BpbFlushRsbOnReturn : 1;
                    u8 BpbFlushRsbOnRetpolineExit : 1;
                    u8 BpbStateReserved : 1;
                };
            };
            union {
                u8 BpbFeatures;
                struct {
                    u8 BpbClearOnIdle : 1;
                    u8 BpbEnabled : 1;
                    u8 BpbSmep : 1;
                    u8 BpbFeaturesReserved : 5;
                };
            };
            u8 BpbCurrentSpecCtrl;
            u8 BpbKernelSpecCtrl;
            u8 BpbNmiSpecCtrl;
            u8 BpbUserSpecCtrl;
            volatile s16 PairRegister;
        };
        u64 PrcbPad11[2];
    };
    KPROCESSOR_STATE ProcessorState;
    struct _XSAVE_AREA_HEADER* ExtendedSupervisorState;
    u32 ProcessorSignature;
    u32 ProcessorFlags;
    union {
        struct {
            u8 BpbRetpolineExitSpecCtrl;
            u8 BpbTrappedRetpolineExitSpecCtrl;
            union {
                u8 BpbTrappedBpbState;
                struct {
                    u8 BpbTrappedCpuIdle : 1;
                    u8 BpbTrappedFlushRsbOnTrap : 1;
                    u8 BpbTrappedIbpbOnReturn : 1;
                    u8 BpbTrappedIbpbOnTrap : 1;
                    u8 BpbTrappedIbpbOnRetpolineExit : 1;
                    u8 BpbTrappedFlushRsbOnReturn : 1;
                    u8 BpbTrappedFlushRsbOnRetpolineExit : 1;
                    u8 BpbTrappedBpbStateReserved : 1;
                };
            };
            union {
                u8 BpbRetpolineState;
                struct {
                    u8 BpbRunningNonRetpolineCode : 1;
                    u8 BpbIndirectCallsSafe : 1;
                    u8 BpbRetpolineEnabled : 1;
                    u8 BpbRetpolineStateReserved : 5;
                };
            };
            u32 PrcbPad12b;
        };
        u64 PrcbPad12a;
    };
    u64 PrcbPad12[3];
    struct _KSPIN_LOCK_QUEUE LockQueue[17];
    PP_LOOKASIDE_LIST PPLookasideList[16];
    struct _GENERAL_LOOKASIDE_POOL PPNxPagedLookasideList[32];
    struct _GENERAL_LOOKASIDE_POOL PPNPagedLookasideList[32];
    struct _GENERAL_LOOKASIDE_POOL PPPagedLookasideList[32];
    u64 MsrIa32TsxCtrl;
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;
    volatile s32 MmPageFaultCount;
    volatile s32 MmCopyOnWriteCount;
    volatile s32 MmTransitionCount;
    volatile s32 MmDemandZeroCount;
    volatile s32 MmPageReadCount;
    volatile s32 MmPageReadIoCount;
    volatile s32 MmDirtyPagesWriteCount;
    volatile s32 MmDirtyWriteIoCount;
    volatile s32 MmMappedPagesWriteCount;
    volatile s32 MmMappedWriteIoCount;
    u32 KeSystemCalls;
    u32 KeContextSwitches;
    u32 PrcbPad40;
    u32 CcFastReadNoWait;
    u32 CcFastReadWait;
    u32 CcFastReadNotPossible;
    u32 CcCopyReadNoWait;
    u32 CcCopyReadWait;
    u32 CcCopyReadNoWaitMiss;
    volatile s32 IoReadOperationCount;
    volatile s32 IoWriteOperationCount;
    volatile s32 IoOtherOperationCount;
    union _LARGE_INTEGER IoReadTransferCount;
    union _LARGE_INTEGER IoWriteTransferCount;
    union _LARGE_INTEGER IoOtherTransferCount;
    volatile s32 PacketBarrier;
    volatile s32 TargetCount;
    volatile u32 IpiFrozen;
    u32 PrcbPad30;
    void* IsrDpcStats;
    u32 DeviceInterrupts;
    s32 LookasideIrpFloat;
    u32 InterruptLastCount;
    u32 InterruptRate;
    u64 PrcbPad31;
    struct _KPRCB* PairPrcb;
    KSTATIC_AFFINITY_BLOCK StaticAffinity;
    u64 PrcbPad35[5];
    union _SLIST_HEADER InterruptObjectPool;
    struct _RTL_HASH_TABLE* DpcRuntimeHistoryHashTable;
    struct _KDPC* DpcRuntimeHistoryHashTableCleanupDpc; // 0x3098
    VOID(*CurrentDpcRoutine)(struct _KDPC*, void*, void*, void*); // 0x30a0
    u64 CurrentDpcRuntimeHistoryCached;
    u64 CurrentDpcStartTime;
    u64 PrcbPad41[1];
    KDPC_DATA DpcData[2]; // 0x30c0
    void* DpcStack;
    s32 MaximumDpcQueueDepth;
    u32 DpcRequestRate;
    u32 MinimumDpcRate;
    u32 DpcLastCount;
    u8 ThreadDpcEnable;
    volatile u8 QuantumEnd;
    volatile u8 DpcRoutineActive;
    volatile u8 IdleSchedule;
    union {
        volatile s32 DpcRequestSummary;
        s16 DpcRequestSlot[2];
        struct {
            s16 NormalDpcState;
            s16 ThreadDpcState;
        };
        struct {
            u32 DpcNormalProcessingActive : 1;
            u32 DpcNormalProcessingRequested : 1;
            u32 DpcNormalThreadSignal : 1;
            u32 DpcNormalTimerExpiration : 1;
            u32 DpcNormalDpcPresent : 1;
            u32 DpcNormalLocalInterrupt : 1;
            u32 DpcNormalSpare : 10;
            u32 DpcThreadActive : 1;
            u32 DpcThreadRequested : 1;
            u32 DpcThreadSpare : 14;
        };
    };
    u32 PrcbPad93;
    u32 LastTick;
    u32 ClockInterrupts;
    u32 ReadyScanTick;
    void* InterruptObject[256];
    KTIMER_TABLE TimerTable; // 0x3940
    u32 PrcbPad92[10];
    struct _KGATE DpcGate; // 0x7b80
    void* PrcbPad52;
    struct _KDPC CallDpc; // 0x7ba0
} KPRCB, * PKPRCB;

#endif // !native_h
