/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef UT_STATUSBARVIEW_H
#define UT_STATUSBARVIEW_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/MStatusBarView class declaration
#include <mstatusbarview.h>

Q_DECLARE_METATYPE(MStatusBarView *);

class Ut_MStatusBarView : public QObject
{
    Q_OBJECT
public:
    Ut_MStatusBarView();

private slots:
    void initTestCase();
    void cleanupTestCase();
#ifdef Q_WS_X11
//    void testXDamageWhileTogglingVisibility();
//    void testXDamageWhileTogglingProviderAvailability();
#endif

private:
    MStatusBarView *m_subject;
    MStatusBar *m_statusbar;
};

#endif
