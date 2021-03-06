/** @file
 * VBox & DTrace - Types and Constants.
 */

/*
 * Copyright (C) 2012-2015 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 */


/*
 * Types used by the other D structure and type definitions.
 *
 * These are taken from a variation of VBox and IPRT headers.
 */
#pragma D depends_on library vbox-arch-types.d

typedef uint16_t                RTSEL;
typedef uint32_t                RTRCPTR;
typedef uintptr_t               RTNATIVETHREAD;
typedef struct RTTHREADINT     *RTTHREAD;
typedef struct RTTRACEBUFINT   *RTTRACEBUF;


typedef uint32_t                VMSTATE;
typedef uint32_t                VMCPUID;
typedef uint32_t                RTCPUID;
typedef struct UVMCPU          *PUVMCPU;
typedef uintptr_t               PVMR3;
typedef uint32_t                PVMRC;
typedef struct VM              *PVMR0;
typedef struct SUPDRVSESSION   *PSUPDRVSESSION;
typedef struct UVM             *PUVM;
typedef uint32_t                CPUMCPUVENDOR;
typedef struct CPUMCTXCORE     *PCPUMCTXCORE;

typedef struct VBOXGDTR
{
    uint16_t    cb;
    uint16_t    au16Addr[4];
} VBOXGDTR, VBOXIDTR;

typedef struct STAMPROFILEADV
{
    uint64_t            au64[5];
} STAMPROFILEADV;

