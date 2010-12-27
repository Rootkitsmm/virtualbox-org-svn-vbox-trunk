/* $Id$ */
/** @file
 * IOM - Internal header file.
 */

/*
 * Copyright (C) 2006-2007 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ___IOMInternal_h
#define ___IOMInternal_h

#include <VBox/cdefs.h>
#include <VBox/types.h>
#include <VBox/vmm/iom.h>
#include <VBox/vmm/stam.h>
#include <VBox/vmm/pgm.h>
#include <VBox/vmm/pdmcritsect.h>
#include <VBox/param.h>
#include <iprt/assert.h>
#include <iprt/avl.h>



/** @defgroup grp_iom_int   Internals
 * @ingroup grp_iom
 * @internal
 * @{
 */

/**
 * MMIO range descriptor.
 */
typedef struct IOMMMIORANGE
{
    /** Avl node core with GCPhys as Key and GCPhys + cbSize - 1 as KeyLast. */
    AVLROGCPHYSNODECORE         Core;
    /** Start physical address. */
    RTGCPHYS                    GCPhys;
    /** Size of the range. */
    uint32_t                    cb;
    uint32_t                    u32Alignment; /**< Alignment padding. */

    /** Pointer to user argument - R3. */
    RTR3PTR                     pvUserR3;
    /** Pointer to device instance - R3. */
    PPDMDEVINSR3                pDevInsR3;
    /** Pointer to write callback function - R3. */
    R3PTRTYPE(PFNIOMMMIOWRITE)  pfnWriteCallbackR3;
    /** Pointer to read callback function - R3. */
    R3PTRTYPE(PFNIOMMMIOREAD)   pfnReadCallbackR3;
    /** Pointer to fill (memset) callback function - R3. */
    R3PTRTYPE(PFNIOMMMIOFILL)   pfnFillCallbackR3;

    /** Pointer to user argument - R0. */
    RTR0PTR                     pvUserR0;
    /** Pointer to device instance - R0. */
    PPDMDEVINSR0                pDevInsR0;
    /** Pointer to write callback function - R0. */
    R0PTRTYPE(PFNIOMMMIOWRITE)  pfnWriteCallbackR0;
    /** Pointer to read callback function - R0. */
    R0PTRTYPE(PFNIOMMMIOREAD)   pfnReadCallbackR0;
    /** Pointer to fill (memset) callback function - R0. */
    R0PTRTYPE(PFNIOMMMIOFILL)   pfnFillCallbackR0;

    /** Pointer to user argument - RC. */
    RTRCPTR                     pvUserRC;
    /** Pointer to device instance - RC. */
    PPDMDEVINSRC                pDevInsRC;
    /** Pointer to write callback function - RC. */
    RCPTRTYPE(PFNIOMMMIOWRITE)  pfnWriteCallbackRC;
    /** Pointer to read callback function - RC. */
    RCPTRTYPE(PFNIOMMMIOREAD)   pfnReadCallbackRC;
    /** Pointer to fill (memset) callback function - RC. */
    RCPTRTYPE(PFNIOMMMIOFILL)   pfnFillCallbackRC;
    /** Alignment padding. */
    RTRCPTR                     RCPtrAlignment;

    /** Description / Name. For easing debugging. */
    R3PTRTYPE(const char *)     pszDesc;
} IOMMMIORANGE;
/** Pointer to a MMIO range descriptor, R3 version. */
typedef struct IOMMMIORANGE *PIOMMMIORANGE;


/**
 * MMIO address statistics. (one address)
 *
 * This is a simple way of making on demand statistics, however it's a
 * bit free with the hypervisor heap memory.
 */
typedef struct IOMMMIOSTATS
{
    /** Avl node core with the address as Key. */
    AVLOGCPHYSNODECORE          Core;

    /** Number of accesses (subtract ReadRZToR3 and WriteRZToR3 to get the right
     *  number). */
    STAMCOUNTER                 Accesses;

    /** Profiling read handler overhead in R3. */
    STAMPROFILE                 ProfReadR3;
    /** Profiling write handler overhead in R3. */
    STAMPROFILE                 ProfWriteR3;
    /** Counting and profiling reads in R0/RC. */
    STAMPROFILE                 ProfReadRZ;
    /** Counting and profiling writes in R0/RC. */
    STAMPROFILE                 ProfWriteRZ;

    /** Number of reads to this address from R0/RC which was serviced in R3. */
    STAMCOUNTER                 ReadRZToR3;
    /** Number of writes to this address from R0/RC which was serviced in R3. */
    STAMCOUNTER                 WriteRZToR3;
} IOMMMIOSTATS;
AssertCompileMemberAlignment(IOMMMIOSTATS, Accesses, 8);
/** Pointer to I/O port statistics. */
typedef IOMMMIOSTATS *PIOMMMIOSTATS;


/**
 * I/O port range descriptor, R3 version.
 */
typedef struct IOMIOPORTRANGER3
{
    /** Avl node core with Port as Key and Port + cPorts - 1 as KeyLast. */
    AVLROIOPORTNODECORE         Core;
#if HC_ARCH_BITS == 64 && !defined(RT_OS_WINDOWS)
    uint32_t                    u32Alignment; /**< The sizeof(Core) differs. */
#endif
    /** Start I/O port address. */
    RTIOPORT                    Port;
    /** Size of the range. */
    uint16_t                    cPorts;
    /** Pointer to user argument. */
    RTR3PTR                     pvUser;
    /** Pointer to the associated device instance. */
    R3PTRTYPE(PPDMDEVINS)       pDevIns;
    /** Pointer to OUT callback function. */
    R3PTRTYPE(PFNIOMIOPORTOUT)  pfnOutCallback;
    /** Pointer to IN callback function. */
    R3PTRTYPE(PFNIOMIOPORTIN)   pfnInCallback;
    /** Pointer to string OUT callback function. */
    R3PTRTYPE(PFNIOMIOPORTOUTSTRING) pfnOutStrCallback;
    /** Pointer to string IN callback function. */
    R3PTRTYPE(PFNIOMIOPORTINSTRING) pfnInStrCallback;
    /** Description / Name. For easing debugging. */
    R3PTRTYPE(const char *)     pszDesc;
} IOMIOPORTRANGER3;
/** Pointer to I/O port range descriptor, R3 version. */
typedef IOMIOPORTRANGER3 *PIOMIOPORTRANGER3;

/**
 * I/O port range descriptor, R0 version.
 */
typedef struct IOMIOPORTRANGER0
{
    /** Avl node core with Port as Key and Port + cPorts - 1 as KeyLast. */
    AVLROIOPORTNODECORE         Core;
#if HC_ARCH_BITS == 64 && !defined(RT_OS_WINDOWS)
    uint32_t                    u32Alignment; /**< The sizeof(Core) differs. */
#endif
    /** Start I/O port address. */
    RTIOPORT                    Port;
    /** Size of the range. */
    uint16_t                    cPorts;
    /** Pointer to user argument. */
    RTR0PTR                     pvUser;
    /** Pointer to the associated device instance. */
    R0PTRTYPE(PPDMDEVINS)       pDevIns;
    /** Pointer to OUT callback function. */
    R0PTRTYPE(PFNIOMIOPORTOUT)  pfnOutCallback;
    /** Pointer to IN callback function. */
    R0PTRTYPE(PFNIOMIOPORTIN)   pfnInCallback;
    /** Pointer to string OUT callback function. */
    R0PTRTYPE(PFNIOMIOPORTOUTSTRING) pfnOutStrCallback;
    /** Pointer to string IN callback function. */
    R0PTRTYPE(PFNIOMIOPORTINSTRING) pfnInStrCallback;
    /** Description / Name. For easing debugging. */
    R3PTRTYPE(const char *)     pszDesc;
} IOMIOPORTRANGER0;
/** Pointer to I/O port range descriptor, R0 version. */
typedef IOMIOPORTRANGER0 *PIOMIOPORTRANGER0;

/**
 * I/O port range descriptor, RC version.
 */
typedef struct IOMIOPORTRANGERC
{
    /** Avl node core with Port as Key and Port + cPorts - 1 as KeyLast. */
    AVLROIOPORTNODECORE         Core;
    /** Start I/O port address. */
    RTIOPORT                    Port;
    /** Size of the range. */
    uint16_t                    cPorts;
    /** Pointer to user argument. */
    RTRCPTR                     pvUser;
    /** Pointer to the associated device instance. */
    RCPTRTYPE(PPDMDEVINS)       pDevIns;
    /** Pointer to OUT callback function. */
    RCPTRTYPE(PFNIOMIOPORTOUT)  pfnOutCallback;
    /** Pointer to IN callback function. */
    RCPTRTYPE(PFNIOMIOPORTIN)   pfnInCallback;
    /** Pointer to string OUT callback function. */
    RCPTRTYPE(PFNIOMIOPORTOUTSTRING) pfnOutStrCallback;
    /** Pointer to string IN callback function. */
    RCPTRTYPE(PFNIOMIOPORTINSTRING) pfnInStrCallback;
#if HC_ARCH_BITS == 64
    RTRCPTR                     RCPtrAlignment; /**< pszDesc is 8 byte aligned. */
#endif
    /** Description / Name. For easing debugging. */
    R3PTRTYPE(const char *)     pszDesc;
} IOMIOPORTRANGERC;
/** Pointer to I/O port range descriptor, RC version. */
typedef IOMIOPORTRANGERC *PIOMIOPORTRANGERC;


/**
 * I/O port statistics. (one I/O port)
 *
 * This is a simple way of making on demand statistics, however it's a
 * bit free with the hypervisor heap memory.
 */
typedef struct IOMIOPORTSTATS
{
    /** Avl node core with the port as Key. */
    AVLOIOPORTNODECORE          Core;
#if HC_ARCH_BITS != 64 || !defined(RT_OS_WINDOWS)
    uint32_t                    u32Alignment; /**< The sizeof(Core) differs. */
#endif
    /** Number of INs to this port from R3. */
    STAMCOUNTER                 InR3;
    /** Profiling IN handler overhead in R3. */
    STAMPROFILE                 ProfInR3;
    /** Number of OUTs to this port from R3. */
    STAMCOUNTER                 OutR3;
    /** Profiling OUT handler overhead in R3. */
    STAMPROFILE                 ProfOutR3;

    /** Number of INs to this port from R0/RC. */
    STAMCOUNTER                 InRZ;
    /** Profiling IN handler overhead in R0/RC. */
    STAMPROFILE                 ProfInRZ;
    /** Number of INs to this port from R0/RC which was serviced in R3. */
    STAMCOUNTER                 InRZToR3;

    /** Number of OUTs to this port from R0/RC. */
    STAMCOUNTER                 OutRZ;
    /** Profiling OUT handler overhead in R0/RC. */
    STAMPROFILE                 ProfOutRZ;
    /** Number of OUTs to this port from R0/RC which was serviced in R3. */
    STAMCOUNTER                 OutRZToR3;
} IOMIOPORTSTATS;
AssertCompileMemberAlignment(IOMIOPORTSTATS, InR3, 8);
/** Pointer to I/O port statistics. */
typedef IOMIOPORTSTATS *PIOMIOPORTSTATS;


/**
 * The IOM trees.
 * These are offset based the nodes and root must be in the same
 * memory block in HC. The locations of IOM structure and the hypervisor heap
 * are quite different in R3, R0 and RC.
 */
typedef struct IOMTREES
{
    /** Tree containing I/O port range descriptors registered for HC (IOMIOPORTRANGEHC). */
    AVLROIOPORTTREE         IOPortTreeR3;
    /** Tree containing I/O port range descriptors registered for R0 (IOMIOPORTRANGER0). */
    AVLROIOPORTTREE         IOPortTreeR0;
    /** Tree containing I/O port range descriptors registered for RC (IOMIOPORTRANGERC). */
    AVLROIOPORTTREE         IOPortTreeRC;

    /** Tree containing the MMIO range descriptors (IOMMMIORANGE). */
    AVLROGCPHYSTREE         MMIOTree;

    /** Tree containing I/O port statistics (IOMIOPORTSTATS). */
    AVLOIOPORTTREE          IOPortStatTree;
    /** Tree containing MMIO statistics (IOMMMIOSTATS). */
    AVLOGCPHYSTREE          MMIOStatTree;
} IOMTREES;
/** Pointer to the IOM trees. */
typedef IOMTREES *PIOMTREES;


/**
 * Converts an IOM pointer into a VM pointer.
 * @returns Pointer to the VM structure the PGM is part of.
 * @param   pIOM   Pointer to IOM instance data.
 */
#define IOM2VM(pIOM)  ( (PVM)((char*)pIOM - pIOM->offVM) )

/**
 * IOM Data (part of VM)
 */
typedef struct IOM
{
    /** Offset to the VM structure. */
    RTINT                           offVM;

    /** Pointer to the trees - RC ptr. */
    RCPTRTYPE(PIOMTREES)            pTreesRC;
    /** Pointer to the trees - R3 ptr. */
    R3PTRTYPE(PIOMTREES)            pTreesR3;
    /** Pointer to the trees - R0 ptr. */
    R0PTRTYPE(PIOMTREES)            pTreesR0;

    /** The ring-0 address of IOMMMIOHandler. */
    R0PTRTYPE(PFNPGMR0PHYSHANDLER)  pfnMMIOHandlerR0;
    /** The RC address of IOMMMIOHandler. */
    RCPTRTYPE(PFNPGMRCPHYSHANDLER)  pfnMMIOHandlerRC;
#if HC_ARCH_BITS == 64
    RTRCPTR                         padding;
#endif

    /** Lock serializing EMT access to IOM. */
    PDMCRITSECT                     EmtLock;

    /** @name Caching of I/O Port and MMIO ranges and statistics.
     * (Saves quite some time in rep outs/ins instruction emulation.)
     * @{ */
    R3PTRTYPE(PIOMIOPORTRANGER3)    pRangeLastReadR3;
    R3PTRTYPE(PIOMIOPORTRANGER3)    pRangeLastWriteR3;
    R3PTRTYPE(PIOMIOPORTSTATS)      pStatsLastReadR3;
    R3PTRTYPE(PIOMIOPORTSTATS)      pStatsLastWriteR3;
    R3PTRTYPE(PIOMMMIORANGE)        pMMIORangeLastR3;
    R3PTRTYPE(PIOMMMIOSTATS)        pMMIOStatsLastR3;

    R0PTRTYPE(PIOMIOPORTRANGER0)    pRangeLastReadR0;
    R0PTRTYPE(PIOMIOPORTRANGER0)    pRangeLastWriteR0;
    R0PTRTYPE(PIOMIOPORTSTATS)      pStatsLastReadR0;
    R0PTRTYPE(PIOMIOPORTSTATS)      pStatsLastWriteR0;
    R0PTRTYPE(PIOMMMIORANGE)        pMMIORangeLastR0;
    R0PTRTYPE(PIOMMMIOSTATS)        pMMIOStatsLastR0;

    RCPTRTYPE(PIOMIOPORTRANGERC)    pRangeLastReadRC;
    RCPTRTYPE(PIOMIOPORTRANGERC)    pRangeLastWriteRC;
    RCPTRTYPE(PIOMIOPORTSTATS)      pStatsLastReadRC;
    RCPTRTYPE(PIOMIOPORTSTATS)      pStatsLastWriteRC;
    RCPTRTYPE(PIOMMMIORANGE)        pMMIORangeLastRC;
    RCPTRTYPE(PIOMMMIOSTATS)        pMMIOStatsLastRC;
    /** @} */

    /** @name I/O Port statistics.
     * @{ */
    STAMCOUNTER                     StatInstIn;
    STAMCOUNTER                     StatInstOut;
    STAMCOUNTER                     StatInstIns;
    STAMCOUNTER                     StatInstOuts;
    /** @} */

    /** @name MMIO statistics.
     * @{ */
    STAMPROFILE                     StatRZMMIOHandler;
    STAMCOUNTER                     StatRZMMIOFailures;

    STAMPROFILE                     StatRZInstMov;
    STAMPROFILE                     StatRZInstCmp;
    STAMPROFILE                     StatRZInstAnd;
    STAMPROFILE                     StatRZInstOr;
    STAMPROFILE                     StatRZInstXor;
    STAMPROFILE                     StatRZInstBt;
    STAMPROFILE                     StatRZInstTest;
    STAMPROFILE                     StatRZInstXchg;
    STAMPROFILE                     StatRZInstStos;
    STAMPROFILE                     StatRZInstLods;
#ifdef IOM_WITH_MOVS_SUPPORT
    STAMPROFILEADV                  StatRZInstMovs;
    STAMPROFILE                     StatRZInstMovsToMMIO;
    STAMPROFILE                     StatRZInstMovsFromMMIO;
    STAMPROFILE                     StatRZInstMovsMMIO;
#endif
    STAMCOUNTER                     StatRZInstOther;

    STAMCOUNTER                     StatRZMMIO1Byte;
    STAMCOUNTER                     StatRZMMIO2Bytes;
    STAMCOUNTER                     StatRZMMIO4Bytes;
    STAMCOUNTER                     StatRZMMIO8Bytes;

    STAMCOUNTER                     StatR3MMIOHandler;

    RTUINT                          cMovsMaxBytes;
    RTUINT                          cStosMaxBytes;
    /** @} */
} IOM;
/** Pointer to IOM instance data. */
typedef IOM *PIOM;


/**
 * IOM per virtual CPU instance data.
 */
typedef struct IOMCPU
{
    /** For saving stack space, the disassembler state is allocated here instead of
     * on the stack.
     * @note The DISCPUSTATE structure is not R3/R0/RZ clean!  */
    union
    {
        /** The disassembler scratch space. */
        DISCPUSTATE                 DisState;
        /** Padding. */
        uint8_t                     abDisStatePadding[DISCPUSTATE_PADDING_SIZE];
    };
    uint8_t                         Dummy[16];
} IOMCPU;
/** Pointer to IOM per virtual CPU instance data. */
typedef IOMCPU *PIOMCPU;


RT_C_DECLS_BEGIN

#ifdef IN_RING3
PIOMIOPORTSTATS iomR3IOPortStatsCreate(PVM pVM, RTIOPORT Port, const char *pszDesc);
PIOMMMIOSTATS   iomR3MMIOStatsCreate(PVM pVM, RTGCPHYS GCPhys, const char *pszDesc);
#endif /* IN_RING3 */

VMMDECL(int)        IOMMMIOHandler(PVM pVM, RTGCUINT uErrorCode, PCPUMCTXCORE pRegFrame, RTGCPTR pvFault, RTGCPHYS GCPhysFault, void *pvUser);
#ifdef IN_RING3
DECLCALLBACK(int)   IOMR3MMIOHandler(PVM pVM, RTGCPHYS GCPhys, void *pvPhys, void *pvBuf, size_t cbBuf, PGMACCESSTYPE enmAccessType, void *pvUser);
#endif


/**
 * Gets the I/O port range for the specified I/O port in the current context.
 *
 * @returns Pointer to I/O port range.
 * @returns NULL if no port registered.
 *
 * @param   pIOM    IOM instance data.
 * @param   Port    Port to lookup.
 */
DECLINLINE(CTX_SUFF(PIOMIOPORTRANGE)) iomIOPortGetRange(PIOM pIOM, RTIOPORT Port)
{
#ifdef IN_RING3
    if (PDMCritSectIsInitialized(&pIOM->EmtLock))
#endif
        Assert(IOMIsLockOwner(IOM2VM(pIOM)));
    CTX_SUFF(PIOMIOPORTRANGE) pRange = (CTX_SUFF(PIOMIOPORTRANGE))RTAvlroIOPortRangeGet(&pIOM->CTX_SUFF(pTrees)->CTX_SUFF(IOPortTree), Port);
    return pRange;
}


/**
 * Gets the I/O port range for the specified I/O port in the HC.
 *
 * @returns Pointer to I/O port range.
 * @returns NULL if no port registered.
 *
 * @param   pIOM    IOM instance data.
 * @param   Port    Port to lookup.
 */
DECLINLINE(PIOMIOPORTRANGER3) iomIOPortGetRangeR3(PIOM pIOM, RTIOPORT Port)
{
#ifdef IN_RING3
    if (PDMCritSectIsInitialized(&pIOM->EmtLock))
#endif
        Assert(IOMIsLockOwner(IOM2VM(pIOM)));
    PIOMIOPORTRANGER3 pRange = (PIOMIOPORTRANGER3)RTAvlroIOPortRangeGet(&pIOM->CTX_SUFF(pTrees)->IOPortTreeR3, Port);
    return pRange;
}


/**
 * Gets the MMIO range for the specified physical address in the current context.
 *
 * @returns Pointer to MMIO range.
 * @returns NULL if address not in a MMIO range.
 *
 * @param   pIOM    IOM instance data.
 * @param   GCPhys  Physical address to lookup.
 */
DECLINLINE(PIOMMMIORANGE) iomMMIOGetRange(PIOM pIOM, RTGCPHYS GCPhys)
{
#ifdef IN_RING3
    if (PDMCritSectIsInitialized(&pIOM->EmtLock))
#endif
        Assert(IOMIsLockOwner(IOM2VM(pIOM)));
    PIOMMMIORANGE pRange = pIOM->CTX_SUFF(pMMIORangeLast);
    if (    !pRange
        ||  GCPhys - pRange->GCPhys >= pRange->cb)
        pIOM->CTX_SUFF(pMMIORangeLast) = pRange = (PIOMMMIORANGE)RTAvlroGCPhysRangeGet(&pIOM->CTX_SUFF(pTrees)->MMIOTree, GCPhys);
    return pRange;
}

#ifdef VBOX_STRICT
/**
 * Gets the MMIO range for the specified physical address in the current context.
 *
 * @returns Pointer to MMIO range.
 * @returns NULL if address not in a MMIO range.
 *
 * @param   pIOM    IOM instance data.
 * @param   GCPhys  Physical address to lookup.
 */
DECLINLINE(PIOMMMIORANGE) iomMMIOGetRangeUnsafe(PIOM pIOM, RTGCPHYS GCPhys)
{
    PIOMMMIORANGE pRange = pIOM->CTX_SUFF(pMMIORangeLast);
    if (    !pRange
        ||  GCPhys - pRange->GCPhys >= pRange->cb)
        pIOM->CTX_SUFF(pMMIORangeLast) = pRange = (PIOMMMIORANGE)RTAvlroGCPhysRangeGet(&pIOM->CTX_SUFF(pTrees)->MMIOTree, GCPhys);
    return pRange;
}
#endif


#ifdef VBOX_WITH_STATISTICS
/**
 * Gets the MMIO statistics record.
 *
 * In ring-3 this will lazily create missing records, while in GC/R0 the caller has to
 * return the appropriate status to defer the operation to ring-3.
 *
 * @returns Pointer to MMIO stats.
 * @returns NULL if not found (R0/GC), or out of memory (R3).
 *
 * @param   pIOM        IOM instance data.
 * @param   GCPhys      Physical address to lookup.
 * @param   pRange      The MMIO range.
 */
DECLINLINE(PIOMMMIOSTATS) iomMMIOGetStats(PIOM pIOM, RTGCPHYS GCPhys, PIOMMMIORANGE pRange)
{
    Assert(IOMIsLockOwner(IOM2VM(pIOM)));
    /* For large ranges, we'll put everything on the first byte. */
    if (pRange->cb > PAGE_SIZE)
        GCPhys = pRange->GCPhys;

    PIOMMMIOSTATS pStats = pIOM->CTX_SUFF(pMMIOStatsLast);
    if (    !pStats
        ||  pStats->Core.Key != GCPhys)
    {
        pStats = (PIOMMMIOSTATS)RTAvloGCPhysGet(&pIOM->CTX_SUFF(pTrees)->MMIOStatTree, GCPhys);
# ifdef IN_RING3
        if (!pStats)
            pStats = iomR3MMIOStatsCreate(IOM2VM(pIOM), GCPhys, pRange->pszDesc);
# endif
    }
    return pStats;
}
#endif

/* IOM locking helpers. */
int     iomLock(PVM pVM);
int     iomTryLock(PVM pVM);
void    iomUnlock(PVM pVM);

/* Disassembly helpers used in IOMAll.cpp & IOMAllMMIO.cpp */
bool    iomGetRegImmData(PDISCPUSTATE pCpu, PCOP_PARAMETER pParam, PCPUMCTXCORE pRegFrame, uint64_t *pu64Data, unsigned *pcbSize);
bool    iomSaveDataToReg(PDISCPUSTATE pCpu, PCOP_PARAMETER pParam, PCPUMCTXCORE pRegFrame, uint64_t u32Data);

RT_C_DECLS_END


#ifdef IN_RING3

#endif

/** @} */

#endif /* ___IOMInternal_h */
