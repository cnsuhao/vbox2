/** @file
 * UICocoaApplication - C++ interface to NSApplication for handling -sendEvent.
 */

/*
 * Copyright (C) 2009-2014 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ___darwin_VBoxCocoaApplication_h
#define ___darwin_VBoxCocoaApplication_h

/* Qt includes: */
#include <QMap>

/* GUI includes: */
#include "VBoxCocoaHelper.h"

ADD_COCOA_NATIVE_REF(UICocoaApplicationPrivate);
ADD_COCOA_NATIVE_REF(NSAutoreleasePool);
ADD_COCOA_NATIVE_REF(NSString);
ADD_COCOA_NATIVE_REF(NSWindow);

/* Forward declarations: */
class QObject;
class QWidget;

/** Event handler callback.
 * @returns true if handled, false if not.
 * @param   pvCocoaEvent    The Cocoa event.
 * @param   pvCarbonEvent   The Carbon event.
 * @param   pvUser          The user argument.
 */
typedef bool (*PFNVBOXCACALLBACK)(const void *pvCocoaEvent, const void *pvCarbonEvent, void *pvUser);

/** Native notification callback type for QObject. */
typedef void (*PfnNativeNotificationCallbackForQObject)(QObject *pObject, const QMap<QString, QString> &userInfo);
/** Native notification callback type for QWidget. */
typedef void (*PfnNativeNotificationCallbackForQWidget)(const QString &strNativeNotificationName, QWidget *pWidget);

/* C++ singleton for our private NSApplication object. */
class UICocoaApplication
{
public:

    /** Returns singleton access instance. */
    static UICocoaApplication* instance();

    /** Destructor. */
    ~UICocoaApplication();

    /** Returns whether application is currently active. */
    bool isActive() const;

    /** Hides the application. */
    void hide();

    /** Register native @a pfnCallback of the @a pvUser taking event @a fMask into account. */
    void registerForNativeEvents(uint32_t fMask, PFNVBOXCACALLBACK pfnCallback, void *pvUser);
    /** Unregister native @a pfnCallback of the @a pvUser taking event @a fMask into account. */
    void unregisterForNativeEvents(uint32_t fMask, PFNVBOXCACALLBACK pfnCallback, void *pvUser);

    /** Register passed @a pObject to native notification @a strNativeNotificationName, using @a pCallback as handler. */
    void registerToNotificationOfWorkspace(const QString &strNativeNotificationName, QObject *pObject, PfnNativeNotificationCallbackForQObject pCallback);
    /** Unregister passed @a pWidget from native notification @a strNativeNotificationName. */
    void unregisterFromNotificationOfWorkspace(const QString &strNativeNotificationName, QObject *pObject);

    /** Register passed @a pWidget to native notification @a strNativeNotificationName, using @a pCallback as handler. */
    void registerToNotificationOfWindow(const QString &strNativeNotificationName, QWidget *pWidget, PfnNativeNotificationCallbackForQWidget pCallback);
    /** Unregister passed @a pWidget from native notification @a strNativeNotificationName. */
    void unregisterFromNotificationOfWindow(const QString &strNativeNotificationName, QWidget *pWidget);

    /** Redirects native notification @a pstrNativeNotificationName for application to registered listener. */
    void nativeNotificationProxyForObject(NativeNSStringRef pstrNativeNotificationName, const QMap<QString, QString> &userInfo);
    /** Redirects native notification @a pstrNativeNotificationName for window @a pWindow to registered listener. */
    void nativeNotificationProxyForWidget(NativeNSStringRef pstrNativeNotificationName, NativeNSWindowRef pWindow);

private:

    /** Constructor. */
    UICocoaApplication();

    /** Holds the singleton access instance. */
    static UICocoaApplication *m_pInstance;

    /** Holds the private NSApplication instance. */
    NativeUICocoaApplicationPrivateRef m_pNative;
    /** Holds the private NSAutoreleasePool instance. */
    NativeNSAutoreleasePoolRef m_pPool;

    /** Map of notification callbacks registered for corresponding QObject(s). */
    QMap<QObject*, QMap<QString, PfnNativeNotificationCallbackForQObject> > m_objectCallbacks;
    /** Map of notification callbacks registered for corresponding QWidget(s). */
    QMap<QWidget*, QMap<QString, PfnNativeNotificationCallbackForQWidget> > m_widgetCallbacks;
};

#endif /* ___darwin_VBoxCocoaApplication_h */

