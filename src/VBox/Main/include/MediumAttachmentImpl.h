/* $Id: $ */
/** @file
 *
 * VirtualBox COM class implementation
 */

/*
 * Copyright (C) 2006-2013 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ____H_MEDIUMATTACHMENTIMPL
#define ____H_MEDIUMATTACHMENTIMPL

#include "MediumAttachmentWrap.h"

class ATL_NO_VTABLE MediumAttachment :
    public MediumAttachmentWrap
{
public:

    DECLARE_EMPTY_CTOR_DTOR(MediumAttachment)

    HRESULT FinalConstruct();
    void FinalRelease();

    // public initializer/uninitializer for internal purposes only
    HRESULT init(Machine *aParent,
                 Medium *aMedium,
                 const Bstr &aControllerName,
                 LONG aPort,
                 LONG aDevice,
                 DeviceType_T aType,
                 bool fImplicit,
                 bool fPassthrough,
                 bool fTempEject,
                 bool fNonRotational,
                 bool fDiscard,
                 bool fHotPluggable,
                 const Utf8Str &strBandwidthGroup);
    HRESULT initCopy(Machine *aParent, MediumAttachment *aThat);
    void uninit();

    // public internal methods
    void i_rollback();
    void i_commit();

    // unsafe public methods for internal purposes only (ensure there is
    // a caller and a read lock before calling them!)
    bool i_isImplicit() const;
    void i_setImplicit(bool aImplicit);

    const ComObjPtr<Medium>& i_getMedium() const;
    const Bstr i_getControllerName() const;
    LONG i_getPort() const;
    LONG i_getDevice() const;
    DeviceType_T i_getType() const;
    bool i_getPassthrough() const;
    bool i_getTempEject() const;
    bool i_getNonRotational() const;
    bool i_getDiscard() const;
    Utf8Str& i_getBandwidthGroup() const;
    bool i_getHotPluggable() const;

    bool i_matches(CBSTR aControllerName, LONG aPort, LONG aDevice);

    /** Must be called from under this object's write lock. */
    void i_updateMedium(const ComObjPtr<Medium> &aMedium);

    /** Must be called from under this object's write lock. */
    void i_updatePassthrough(bool aPassthrough);

    /** Must be called from under this object's write lock. */
    void i_updateTempEject(bool aTempEject);

    /** Must be called from under this object's write lock. */
    void i_updateNonRotational(bool aNonRotational);

    /** Must be called from under this object's write lock. */
    void i_updateDiscard(bool aDiscard);

    /** Must be called from under this object's write lock. */
    void i_updateEjected();

    /** Must be called from under this object's write lock. */
    void i_updateBandwidthGroup(const Utf8Str &aBandwidthGroup);

    void i_updateParentMachine(Machine * const pMachine);

    /** Must be called from under this object's write lock. */
    void i_updateHotPluggable(bool aHotPluggable);

    /** Get a unique and somewhat descriptive name for logging. */
    const char* i_getLogName(void) const { return mLogName.c_str(); }

private:

    // Wrapped IMediumAttachment properties
    HRESULT getMedium(ComPtr<IMedium> &aHardDisk);
    HRESULT getController(com::Utf8Str &aController);
    HRESULT getPort(LONG *aPort);
    HRESULT getDevice(LONG *aDevice);
    HRESULT getType(DeviceType_T *aType);
    HRESULT getPassthrough(BOOL *aPassthrough);
    HRESULT getTemporaryEject(BOOL *aTemporaryEject);
    HRESULT getIsEjected(BOOL *aEjected);
    HRESULT getDiscard(BOOL *aDiscard);
    HRESULT getNonRotational(BOOL *aNonRotational);
    HRESULT getBandwidthGroup(ComPtr<IBandwidthGroup> &aBandwidthGroup);
    HRESULT getHotPluggable(BOOL *aHotPluggable);

    struct Data;
    Data *m;

    Utf8Str mLogName;                   /**< For logging purposes */
};

#endif // ____H_MEDIUMATTACHMENTIMPL
/* vi: set tabstop=4 shiftwidth=4 expandtab: */
