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

#ifndef ML10N_MBREAKITERATOR_H
#define ML10N_MBREAKITERATOR_H

#include "mlocaleexport.h"
#include "mlocale.h"

#include <QString>

namespace ML10N {

class MBreakIteratorPrivate;

class MLOCALE_EXPORT MBreakIterator
{
public:
    enum Type {LineIterator, WordIterator, CharacterIterator, SentenceIterator, TitleIterator};

    MBreakIterator(const MLocale &locale, const QString &text, Type type = WordIterator);
    explicit MBreakIterator(const QString &text, Type type = WordIterator);
    virtual ~MBreakIterator();

    // java-style iterator interface:
    bool hasNext() const;
    bool hasPrevious() const;
    int next();
    int next(int index); // searching from explicit index
    int peekNext();
    int peekPrevious();
    int previous();
    int previous(int index);
    int previousInclusive();
    int previousInclusive(int index);
    void toBack();
    void toFront();
    int index() const;
    void setIndex(int index);
    bool isBoundary();
    bool isBoundary(int index);

private:
    Q_DISABLE_COPY(MBreakIterator)
    Q_DECLARE_PRIVATE(MBreakIterator)
    MBreakIteratorPrivate *const d_ptr;
};

}

#endif
