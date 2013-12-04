/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef PT_CALENDAR_H
#define PT_CALENDAR_H


#include <QtTest/QtTest>
#include <QObject>
#include <MLocale>
#include <MCalendar>

class Pt_MCalendar : public QObject
{
    Q_OBJECT

private:
    QString icuPackageVersion;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void benchmarkFormatDateTimePosixFormatString_U_QDateTime();
    void benchmarkFormatDateTimePosixFormatString_U_MCalendar();
    void benchmarkFormatDateTimePosixFormatString_V_MCalendar();
    void benchmarkFormatDateTimePosixFormatString_r_MCalendar();
    void benchmarkFormatDateTimePosixFormatString_R_MCalendar();
    void benchmarkFormatDateTimePosixFormatString_t_MCalendar();
    void benchmarkIcuFormatString();
    void benchmarkFormatDateTime();
    void benchmarkFormatDateTimeICU();
};

#endif
