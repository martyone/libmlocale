/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_MSHEETVIEW_H
#define UT_MSHEETVIEW_H

#include <QObject>
#include <QtTest/QtTest>

class MSheetView;
class MApplication;
class MApplicationPage;
class MSheet;
class MApplicationWindow;

class Ut_MSheetView : public QObject
{
    Q_OBJECT
public:
    Ut_MSheetView();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testHeaderHiding();

private:
    MApplication* app;
    MApplicationWindow* window;
    MSheetView *subject;
    MSheet* sheet;
};

#endif

