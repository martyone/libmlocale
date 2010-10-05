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

#include "pt_mlocale.h"

void Pt_MLocale::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QProcess process;
#ifdef HAVE_ICU
    process.start("sh -c \"dpkg -s libicu44 | grep Version | perl -pe 's/^Version:[[:space:]]*([^[[:space:]]+)$/$1/g'\"");
    if (!process.waitForFinished()) {
        qDebug() << "cannot run process to check libicu44 package version , exiting ...";
        exit(1);
    }
    icuPackageVersion = process.readAllStandardOutput();
    icuPackageVersion.replace("\n", "");
    qDebug() << "libicu44 package version is:" << icuPackageVersion;
#endif
}

void Pt_MLocale::cleanupTestCase()
{
    delete qap;
}

void Pt_MLocale::init()
{
}

void Pt_MLocale::cleanup()
{
}

void Pt_MLocale::benchmarkCountryEndonym()
{
    QString language("de_CH");   // only this should matter
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // should not matter
    QString lcNumeric("en_US");  // should not matter
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);

    QString result("Schweiz");
    QCOMPARE(locale.countryEndonym(), result);

    QBENCHMARK {
        locale.countryEndonym();
    }
}

QTEST_APPLESS_MAIN(Pt_MLocale);
