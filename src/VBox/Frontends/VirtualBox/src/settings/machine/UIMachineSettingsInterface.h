/* $Id$ */
/** @file
 * VBox Qt GUI - UIMachineSettingsInterface class declaration.
 */

/*
 * Copyright (C) 2008-2015 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ___UIMachineSettingsInterface_h___
#define ___UIMachineSettingsInterface_h___

/* GUI includes: */
#include "UISettingsPage.h"
#include "UIMachineSettingsInterface.gen.h"

/* Forward declarations: */
class UIActionPool;

/* Machine settings / User Interface page / Data: */
struct UIDataSettingsMachineInterface
{
    /* Constructor: */
    UIDataSettingsMachineInterface()
#ifndef Q_WS_MAC
        : m_fShowMiniToolBar(false)
        , m_fMiniToolBarAtTop(false)
#endif /* !Q_WS_MAC */
    {}

    /* Functions: */
    bool equal(const UIDataSettingsMachineInterface &other) const
    {
#ifndef Q_WS_MAC
        return (m_fShowMiniToolBar == other.m_fShowMiniToolBar) &&
               (m_fMiniToolBarAtTop == other.m_fMiniToolBarAtTop);
#else /* Q_WS_MAC */
        Q_UNUSED(other);
        return true;
#endif /* Q_WS_MAC */
    }

    /* Operators: */
    bool operator==(const UIDataSettingsMachineInterface &other) const { return equal(other); }
    bool operator!=(const UIDataSettingsMachineInterface &other) const { return !equal(other); }

    /* Variables: */
#ifndef Q_WS_MAC
    bool m_fShowMiniToolBar;
    bool m_fMiniToolBarAtTop;
#endif /* !Q_WS_MAC */
};
typedef UISettingsCache<UIDataSettingsMachineInterface> UICacheSettingsMachineInterface;

/* Machine settings / User Interface page: */
class UIMachineSettingsInterface : public UISettingsPageMachine,
                                   public Ui::UIMachineSettingsInterface
{
    Q_OBJECT;

public:

    /** Constructor, early takes @a strMachineID into account for size-hint calculation. */
    UIMachineSettingsInterface(const QString strMachineID);
    /** Destructor. */
    ~UIMachineSettingsInterface();

protected:

    /* API: Cache stuff: */
    bool changed() const { return m_cache.wasChanged(); }

    /* API: Load data to cache from corresponding external object(s),
     * this task COULD be performed in other than GUI thread: */
    void loadToCacheFrom(QVariant &data);
    /* API: Load data to corresponding widgets from cache,
     * this task SHOULD be performed in GUI thread only: */
    void getFromCache();

    /* API: Save data from corresponding widgets to cache,
     * this task SHOULD be performed in GUI thread only: */
    void putToCache();
    /* API: Save data from cache to corresponding external object(s),
     * this task COULD be performed in other than GUI thread: */
    void saveFromCacheTo(QVariant &data);

    /* Helper: Navigation stuff: */
    void setOrderAfter(QWidget *pWidget);

    /* Helper: Translation stuff: */
    void retranslateUi();

    /* Helper: Polishing stuff: */
    void polishPage();

private:

    /** Prepare routine. */
    void prepare();

    /** Cleanup routine. */
    void cleanup();

    /* Cache: */
    UICacheSettingsMachineInterface m_cache;

    /** Holds the machine ID copy. */
    const QString m_strMachineID;
    /** Holds the action-pool instance. */
    UIActionPool *m_pActionPool;
};

#endif // ___UIMachineSettingsInterface_h___
