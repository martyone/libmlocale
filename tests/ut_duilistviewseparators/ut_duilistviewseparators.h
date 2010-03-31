/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef UT_DUILISTVIEWSEPARATORS_H
#define UT_DUILISTVIEWSEPARATORS_H

#include <QtTest/QtTest>
#include <QAbstractListModel>
#include <DuiWidget>
#include "duiabstractcellcreator.h"

const int GroupHeaderHeight = 40;

class MyIndexedModel;
class DuiListViewPrivate;
class DuiPlainListViewPrivate;
class DuiPlainMultiColumnListViewPrivate;
class DuiGroupHeaderListViewPrivate;
class DuiMultiColumnListViewPrivate;

class Ut_DuiListViewSeparators : public QObject
{
    Q_OBJECT

private:
    void makePhoneBook();
    void makePhoneBookModel();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    DuiWidget *newHorizontalSeparator();
    DuiWidget *newVerticalSeparator();

    void testPlainSetNullHorizontalSeparator();
    void testPlainMulticolumnSetNullHorizontalSeparator();
    void testGroupedSetNullHorizontalSeparator();
    void testGroupedMulticolumnSetNullHorizontalSeparator();

    void testPlainMulticolumnSetNullVerticalSeparator();
    void testGroupedMulticolumnSetNullVerticalSeparator();

    void testPlainMulticolumnHorizontalSeparatorWidthWith2Columns();
    void testPlainMulticolumnHorizontalSeparatorWidthWith4Columns();
    void testPlainMulticolumnHorizontalSeparatorWidthWith8Columns();
    void testGroupedMulticolumnHorizontalSeparatorWidthWith2Columns();
    void testGroupedMulticolumnHorizontalSeparatorWidthWith4Columns();
    void testGroupedMulticolumnHorizontalSeparatorWidthWith8Columns();

    void testPlainHorizontalSeparatorWidth();
    void testGroupedHorizontalSeparatorWidth();

    void testPlainMulticolumnVerticalSeparatorWidth();
    void testGroupedMulticolumnVerticalSeparatorWidth();

    void testPlainMulticolumnVerticalSeparatorHeight();
    void testGroupedMulticolumnVerticalSeparatorHeight();

private:
    QObject *phoneBook;
    MyIndexedModel *phoneBookModel;

    DuiPlainListViewPrivate *listViewPrivate;
    DuiPlainMultiColumnListViewPrivate *plainMultiColumnListViewPrivate;
    DuiGroupHeaderListViewPrivate *groupedListViewPrivate;
    DuiMultiColumnListViewPrivate *multiColumnListViewPrivate;
};

#endif
