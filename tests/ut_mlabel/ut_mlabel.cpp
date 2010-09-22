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

#include "ut_mlabel.h"

#include <mwidget.h>
#include <mwidgetcontroller.h>
#include <mlabelview.h>
#include <mtheme.h>
#include <mwindow.h>
#include <mapplication.h>
#include <mlabelhighlighter.h>

#include <QSignalSpy>
#include <QTestEventList>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGesture>
#include <QTextDocument>

#define SAVE_IMAGE(fileName, image) \
    do{ \
        QFile f(fileName); f.open(QIODevice::WriteOnly); image.save(&f, "PNG"); f.close(); \
    } while(0)

// Subclass of MLabel in order to simulate clicks
TestMLabel::TestMLabel(): MLabel()
{
}

TestMLabel::~TestMLabel()
{
}

void TestMLabel::simulateClick(const QPointF &pos)
{
    QGraphicsSceneMouseEvent *pressevent = new QGraphicsSceneMouseEvent();
    QGraphicsSceneMouseEvent *releaseevent = new QGraphicsSceneMouseEvent();
    pressevent->setPos(pos);
    releaseevent->setPos(pos);
    mousePressEvent(pressevent);
    mouseReleaseEvent(releaseevent);
    delete pressevent;
    delete releaseevent;
}

QImage TestMLabel::getLabelImage()
{
    QPixmap pixmap(size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    MWidgetController::paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

MApplication *app;

void Ut_MLabel::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mlabel" };
    app = new MApplication(argc, app_name);

    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_mlabel.css");
}

void Ut_MLabel::init()
{
    label = new TestMLabel;
    label->setGeometry(QRectF(0, 0, 400, 20));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
}

void Ut_MLabel::cleanup()
{
    delete label;
}

void Ut_MLabel::cleanupTestCase()
{
    delete app;
}

void Ut_MLabel::testTestConstruction()
{
    MLabel *label = new MLabel();
    delete label;

    label = new MLabel("this is plain text!");
    delete label;

    label = new MLabel("this is <b>rich</b> text!");
    delete label;

    label = new MLabel();
    label->setText("this is <b>rich</b> text!");
    MLabelModel *model = new MLabelModel();
    model->setText("this is plain text!");
    label->setModel(model);
    delete label;
}


void Ut_MLabel::testTextAlignment_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is plain text!";
    QTest::newRow("rich") << "<b>this is rich text!</b>";
}
void Ut_MLabel::testTextAlignment()
{
    QVector<Qt::Alignment> alignments;
    alignments.push_back(Qt::AlignLeft | Qt::AlignTop);
    alignments.push_back(Qt::AlignRight | Qt::AlignTop);
    alignments.push_back(Qt::AlignHCenter | Qt::AlignTop);
    alignments.push_back(Qt::AlignLeft | Qt::AlignVCenter);
    alignments.push_back(Qt::AlignRight | Qt::AlignVCenter);
    alignments.push_back(Qt::AlignHCenter | Qt::AlignVCenter);
    alignments.push_back(Qt::AlignLeft | Qt::AlignBottom);
    alignments.push_back(Qt::AlignRight | Qt::AlignBottom);
    alignments.push_back(Qt::AlignHCenter | Qt::AlignBottom);

    label->setMinimumSize(500, 500);
    label->setMaximumSize(500, 500);
    label->setPreferredSize(500, 500);
    label->setGeometry(QRectF(0, 0, 500, 500));

    QFETCH(QString, text);
    label->setText(text);
    QVERIFY(text == label->text());

    QVector<QImage> images;

    for (int i = 0; i < alignments.count(); i++) {
        label->setAlignment(alignments[i]);
        QVERIFY(label->alignment() == alignments[i]);

        QImage image = captureImage(label);
        images.push_back(image);
        //image.save(QString("0x") + QString::number(alignments[i], 16), "PNG");
    }

    for (int i = 0; i < alignments.count(); i++) {
        for (int j = i + 1; j < alignments.count(); j++) {
            if (images[i] == images[j]) {
                qDebug() << "Images are similar with alignments:" <<
                         QString("0x") + QString::number(alignments[i], 16) <<
                         "and" <<
                         QString("0x") + QString::number(alignments[j], 16);
            }

            QVERIFY(images[i] != images[j]);
        }
    }
}

void Ut_MLabel::testLayoutDirection_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is plain text!";
    QTest::newRow("rich") << "<b>this is rich text!</b>";
}

void Ut_MLabel::testLayoutDirection()
{
    QSKIP("Wait for issue in bug 175685 to be resolved", SkipSingle);

    QFETCH(QString, text);
    label->setText(text);
    QVERIFY(text == label->text());

    label->setLayoutDirection(Qt::LeftToRight);
    QVERIFY(label->layoutDirection() == Qt::LeftToRight);
    QImage ltr = captureImage(label);

    label->setLayoutDirection(Qt::RightToLeft);
    QVERIFY(label->layoutDirection() == Qt::RightToLeft);
    QImage rtl = captureImage(label);

    //ltr.save("ltr", "PNG");
    //rtl.save("rtl", "PNG");

    QVERIFY(ltr != rtl);
}

void Ut_MLabel::testTextWordWrap_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("equal");

    QTest::newRow("plain") << "David Michael Hasselhoff (born July 17, 1952) is an American actor and singer.He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006." << true;
    QTest::newRow("rich") << "<b>David Michael Hasselhoff (born July 17, 1952) is an American actor and singer. He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006.</b>" << false;
}

void Ut_MLabel::testTextWordWrap()
{
    QFETCH(QString, text);

    label->setText(text);
    QVERIFY(text == label->text());

    label->setWordWrap(true);
    QVERIFY(label->wordWrap() == true);
    QImage wrapped = captureImage(label);

    label->setWordWrap(false);
    QVERIFY(label->wordWrap() == false);
    QImage unwrapped = captureImage(label);

    //wrapped.save("wrapped", "PNG");
    //unwrapped.save("unwrapped", "PNG");

    //
    QVERIFY(wrapped != unwrapped);
}

void Ut_MLabel::testTextElide_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "David Michael Hasselhoff (born July 17, 1952) is an American actor and singer.He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006.";
    // For some reason this doesn't work in CC -> solve why
    //QTest::newRow("rich") << "<b>David Michael Hasselhoff (born July 17, 1952) is an American actor and singer. He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006.</b>";
}

void Ut_MLabel::testTextElide()
{
    QFETCH(QString, text);
    label->setText(text);
    QVERIFY(text == label->text());

    label->setWordWrap(false);
    label->setTextElide(true);
    QVERIFY(label->textElide() == true);
    QImage elided = captureImage(label);

    label->setTextElide(false);
    QVERIFY(label->textElide() == false);
    QImage unelided = captureImage(label);

    label->setLayoutDirection(Qt::RightToLeft);
    QImage elidedRtl = captureImage(label);

    //elided.save("elided", "PNG");
    //unelided.save("unelided", "PNG");
    //elidedRtl.save("elidedrtl", "PNG");

    QVERIFY(elided != unelided);
    QVERIFY(elided != elidedRtl);
}

void Ut_MLabel::testRichTextElide_data()
{
    QTest::addColumn<QString>("text");
    QTest::newRow("rich") << "<b>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa</b>";
}

void Ut_MLabel::testRichTextElide()
{
    QFETCH(QString, text);
    label->setLayoutDirection(Qt::LeftToRight);
    label->setWordWrap(false);
    label->setText(text);
    QVERIFY(text == label->text());

    label->setTextElide(true);
    QVERIFY(label->textElide() == true);
    QImage elided = captureImage(label);

    label->setTextElide(false);
    QVERIFY(label->textElide() == false);
    QImage unelided = captureImage(label);

    QImage leftUnelided = unelided.copy(0, 0, unelided.width() / 2, unelided.height());
    QImage rightUnelided = unelided.copy(unelided.width() / 2, 0, unelided.width() / 2, unelided.height());

    QImage leftElided = elided.copy(0, 0, elided.width() / 2, elided.height());
    QImage rightElided = elided.copy(elided.width() / 2, 0, elided.width() / 2, elided.height());

    QCOMPARE(elided.isNull(), false);
    QCOMPARE(unelided.isNull(), false);

    QVERIFY(elided != unelided);

    //'...' is added to the rightmost part of the label in LTR mode
    QVERIFY(leftUnelided == leftElided);
    QVERIFY(rightUnelided != rightElided);

}

void Ut_MLabel::testHighlighting()
{
    QSKIP("Wait for issue in bug #164207 to be resolved", SkipSingle);
    label->setText("Label <b>rich</b>");

    QImage nonhighlighted = captureImage(label);

    MCommonLabelHighlighter *h = new MCommonLabelHighlighter(QRegExp("Label"));
    label->addHighlighter(h);
    QImage highlighted = captureImage(label);

    QVERIFY(nonhighlighted != highlighted);

    QSignalSpy spyClick(h, SIGNAL(clicked(QString)));
    label->simulateClick(QPoint(15, 5));
    QCOMPARE(spyClick.count(), 1);

    QSignalSpy spyLongPress(h, SIGNAL(longPressed(QString)));

    QList<QGesture *> list;
    QTapAndHoldGesture gesture;
    list.append( &gesture );
    QGestureEvent gestureEvent(list);
    label->tapAndHoldGestureEvent (&gestureEvent, &gesture );

    QCOMPARE(spyLongPress.count(), 1);

    label->removeHighlighter(NULL);
    label->removeHighlighter(h);
    delete h;
}

void Ut_MLabel::testHighlightingModel()
{
    QSKIP("Wait for issue in bug #164207 to be resolved", SkipSingle);
    label->setText("Label <b>rich</b>");

    QImage nonhighlighted = captureImage(label);

    MCommonLabelHighlighter *h = new MCommonLabelHighlighter(QRegExp("Label"));
    label->model()->addHighlighter(h);
    QImage highlighted = captureImage(label);

    QVERIFY(nonhighlighted != highlighted);

    QSignalSpy spyClick(h, SIGNAL(clicked(QString)));
    label->simulateClick(QPoint(15, 5));
    QCOMPARE(spyClick.count(), 1);

    QSignalSpy spyLongPress(h, SIGNAL(longPressed(QString)));

    QList<QGesture *> list;
    QTapAndHoldGesture gesture;
    list.append( &gesture );
    QGestureEvent gestureEvent(list);
    label->tapAndHoldGestureEvent (&gestureEvent, &gesture );

    QCOMPARE(spyLongPress.count(), 1);

    label->model()->removeHighlighter(NULL);
    label->model()->removeHighlighter(h);
    delete h;
}



void Ut_MLabel::testAnchor()
{
    label->setText("<a href=\"http://www.nokia.com\">www.nokia.com</a>");
    anchorHitTest(QPointF(15, 5), 1);
    anchorHitTest(QPointF(200, 0), 0);
}

void Ut_MLabel::anchorHitTest(const QPointF &point, int expectedHits)
{
    QSignalSpy spy(label, SIGNAL(linkActivated(QString)));
    label->simulateClick(point);
    QCOMPARE(spy.count(), expectedHits);
}

QImage Ut_MLabel::captureImage(MLabel *label)
{
    QPixmap pixmap(label->size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    label->paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

void Ut_MLabel::testClear_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_MLabel::testClear()
{
    QFETCH(QString, text);

    QImage emptyLabel = captureImage(label);
    label->setText(text);
    QImage xxxxLabel = captureImage(label);
    QVERIFY(emptyLabel != xxxxLabel);

    label->setText("");
    QImage clearedLabel = captureImage(label);

    QVERIFY(label->text().isEmpty());
    QVERIFY(xxxxLabel != clearedLabel);
}

void Ut_MLabel::testPlainTextColor_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_MLabel::testPlainTextColor()
{
    QFETCH(QString, text);

    TestMLabel *label = new TestMLabel();
    label->setText(text);
    label->setGeometry(QRectF(0, 0, 400, 20));
    QImage imageLabel = captureImage(label);

    TestMLabel *anotherLabel = new TestMLabel();
    anotherLabel->setText(text);
    anotherLabel->setGeometry(QRectF(0, 0, 400, 20));
    QImage anotherLabelImage = captureImage(anotherLabel);
    QVERIFY2(imageLabel == anotherLabelImage, "Images should be equal, until we set name for second label.");
    anotherLabel->setObjectName("greyLabel");
    anotherLabel->setText(text);
    anotherLabelImage = captureImage(anotherLabel);
    QVERIFY2(imageLabel != anotherLabelImage, "Images shoudn't be equal, because in css colors are different");
    delete label;
    delete anotherLabel;
}
void Ut_MLabel::testSizeHint_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QTextOption::WrapMode>("wrapMode");
    QTest::addColumn<qreal>("widthConstraint");

    QStringList texts;
    texts.append("Short plain text");
    texts.append("Long plain text that gets wrapped several times within the label boundaries.");
    texts.append("UnwrappablePlainTextWithoutSpaces");
    texts.append("Partly wrappable text containing VeryLongUnwrappableWordsThatExceedTheWidth of the label.");
    texts.append("<b>Some short rich text</b>");
    texts.append("Long rich text <b>that</b> gets wrapped <i>several times</i> within the label boundaries.");

    typedef QPair<QByteArray, QTextOption::WrapMode> WrapInfo;
    QList<WrapInfo> wrapModes;
    wrapModes.append(WrapInfo("NoWrap", QTextOption::NoWrap));
    wrapModes.append(WrapInfo("WordWrap", QTextOption::WordWrap));
    wrapModes.append(WrapInfo("ManualWrap", QTextOption::ManualWrap));
    wrapModes.append(WrapInfo("WrapAnywhere", QTextOption::WrapAnywhere));
    wrapModes.append(WrapInfo("WrapAtWordBoundaryOrAnywhere", QTextOption::WrapAtWordBoundaryOrAnywhere));

    static QList<QByteArray> descriptions;
    for (qreal widthConstraint = -1.0; widthConstraint < 90.0; widthConstraint += 30.0) {
        foreach (const WrapInfo &wrapInfo, wrapModes) {
            foreach (const QString &text, texts) {
                const QString descr = text.left(20) + ".../" + wrapInfo.first + "/" + QString::number(widthConstraint);
                descriptions.append(descr.toAscii());
                QTest::newRow(descriptions.last().constData()) << text << wrapInfo.second << widthConstraint;
            }
        }
    }
}

void Ut_MLabel::testSizeHint()
{
    QFETCH(QString, text);
    QFETCH(QTextOption::WrapMode, wrapMode);
    QFETCH(qreal, widthConstraint);

    TestMLabel *label = new TestMLabel();
    label->setText(text);
    label->setGeometry(QRectF(0, 0, 400, 20));

    label->setWordWrap(true);
    label->setWrapMode(wrapMode);

    QSizeF constraint(widthConstraint, -1);
    QSizeF minSizeHint = label->sizeHint(Qt::MinimumSize, constraint);
    QSizeF prefSizeHint = label->sizeHint(Qt::PreferredSize, constraint);
    QSizeF maxSizeHint = label->sizeHint(Qt::MaximumSize, constraint);

    QVERIFY(minSizeHint.width() <= prefSizeHint.width() || minSizeHint.height() <= prefSizeHint.height());
    QVERIFY(prefSizeHint.width() <= maxSizeHint.width() || prefSizeHint.height() <= maxSizeHint.height());

    if (widthConstraint > 0.0) {
        QVERIFY(minSizeHint.width() <= widthConstraint);
        QVERIFY(prefSizeHint.width() <= widthConstraint);
    }

    // To verify whether the preferred size hint allows to show the whole text,
    // a local QTextDocument is created as reference.
    QTextDocument textDocument(text);
    if (Qt::mightBeRichText(text)) {
        const QString spanTag("<span>%1</span>");
        textDocument.setHtml(spanTag.arg(text));
    }
    textDocument.setDocumentMargin(0);
    textDocument.setDefaultFont(label->font());
    QTextOption textOption;
    textOption.setWrapMode(wrapMode);
    textDocument.setDefaultTextOption(textOption);
    textDocument.setTextWidth(prefSizeHint.width());
    if (textDocument.size().width() <= prefSizeHint.width()) {
        QVERIFY(textDocument.size().height() == prefSizeHint.height());
    } else {
        // If the width of the text document is greater than the width
        // set by QTextDocument::setTextWidth(), then no wrapping has
        // been done and the width has been exceeded.
        // However the preferred size hint always respects the maximum
        // available width (see MLabelViewSimple::resizeEvent()).
        QVERIFY(textDocument.size().height() <= prefSizeHint.height());
    }
}

void Ut_MLabel::testUnknownSizeHint()
{
    QSizeF hint = label->sizeHint((Qt::SizeHint)12345);
}

void Ut_MLabel::testFont_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_MLabel::testFont()
{
    QFETCH(QString, text);

    MLabel *label = new TestMLabel();
    label->setText(text);

    // this will create the view and load CSS font
    label->preferredSize();

    QFont newFont("Times", 30, QFont::Bold);
    label->setFont(newFont);

    QVERIFY(label->font() == newFont);

    // make style reload
    label->setActive(true);
    label->setActive(false);

    QVERIFY(label->font() == newFont);
}

void Ut_MLabel::testColor_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_MLabel::testColor()
{
    QFETCH(QString, text);

    MLabel *label = new TestMLabel();
    label->setText(text);

    // this will create the view and load CSS font
    label->preferredSize();

    QColor color("#FF00FF");
    label->setColor(color);

    QVERIFY(label->color() == color);

    // make style reload
    label->setActive(true);
    label->setActive(false);

    QVERIFY(label->color() == color);
}

void Ut_MLabel::linefeedBeforeFirstTag()
{
    label->setWordWrap(true);

    label->setText("\n<b></b>\na\nb\nc\nd\ne\nf\ng\n");
    QImage image1  = captureImage(label);
    
    label->setText("\na<br>b<br>c<br>d<br>e<br>");
    QImage image2  = captureImage(label);
    
    //Linefeed characters are ignored and but <br> tags are interpreted as line breaks

    //Ignoring linefeeds causes content to be horizontal
    QVERIFY2(contentWidth(image1) > contentHeight(image1), "'\n' needs to be ignored in rich label");
    //Interpreting <br> tags causes content to be vertical
    QVERIFY2(contentHeight(image2) > contentWidth(image2), "<br> needs to work in rich label");

}

void Ut_MLabel::highlighterEmptyRegexp()
{   
    //This test verifies that label doesn't
    //go to an infinite loop when a highlighter 
    //matching an empty string is used.
    
    QRegExp regexp("^");
    MCommonLabelHighlighter highlighter(regexp);
    label->setText("test");
    label->addHighlighter(&highlighter);
    captureImage(label);
}


void Ut_MLabel::removeAllHighlighters()
{
    label->setText("Label <b>rich</b>");

    QImage nonhighlighted = captureImage(label);

    MCommonLabelHighlighter highlighter(QRegExp("Label"));
    label->addHighlighter(&highlighter);
    QImage highlighted = captureImage(label);

    QVERIFY(nonhighlighted != highlighted);

    label->removeAllHighlighters();
    QImage highlightingRemoved = captureImage(label);
    QVERIFY(nonhighlighted == highlightingRemoved);
    
    
}

QRect Ut_MLabel::contentRect(const QImage& image)
{
    int minX, minY, maxX, maxY;

    bool found = false;
    for(minX = 0; !found && minX < image.width(); ++minX) {
        for(int i = 0; !found && i < image.height(); ++i) {
            if (image.pixel(minX, i) != 0) {
                found = true;
            }
        }
    }

    found = false;
    for(minY = 0; !found && minY < image.height(); ++minY) {
        for(int i = 0; !found && i < image.width(); ++i) {
            if (image.pixel(i, minY) != 0) {
                found = true;
            }
        }
    }

    found = false;
    for(maxX = image.width() - 1; !found && maxX >= 0; --maxX) {
        for(int i = 0; !found && i < image.height(); ++i) {
            if (image.pixel(maxX, i) != 0) {
                found = true;
            }
        }
    }

    found = false;
    for(maxY = image.height() - 1; !found && maxY >= 0; --maxY) {
        for(int i = 0; !found && i < image.width(); ++i) {
            if (image.pixel(i, maxY) != 0) {
                found = true;
            }
        }
    }

    return QRect(QPoint(minX, minY), QPoint(maxX, maxY));

}

int Ut_MLabel::contentHeight(const QImage& image)
{
    QRect rect = contentRect(image);
    return rect.bottom() - rect.top();
}

int Ut_MLabel::contentWidth(const QImage& image)
{
    QRect rect = contentRect(image);
    return rect.right() - rect.left();
}

void Ut_MLabel::wrapModes_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "aaa bbb ccc dddddddddddddddd eeeeeeeeeeeeeee fffffffffffffffff gggggggggggggggg hhhhhhhhhhhhhhhh iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj kkkkkkkkkkk";
    
    QTest::newRow("rich") << "<b>aaa bbb ccc dddddddddddddddd eeeeeeeeeeeeeee fffffffffffffffff gggggggggggggggg hhhhhhhhhhhhhhhh iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj kkkkkkkkkkk</b>";
}
    
void Ut_MLabel::wrapModes()
{
    QFETCH(QString, text);
    label->setText(text);
    label->resize(100, 800);

    QImage defaultMode = captureImage(label);

    label->setWordWrap(true);

    label->setWrapMode(QTextOption::ManualWrap);
    QCOMPARE(label->wrapMode(), QTextOption::ManualWrap);
    QImage manualWrap = captureImage(label);

    label->setWrapMode(QTextOption::WordWrap);
    QCOMPARE(label->wrapMode(), QTextOption::WordWrap);
    QImage wordWrap = captureImage(label);
    
    label->setWrapMode(QTextOption::WrapAnywhere);
    QCOMPARE(label->wrapMode(), QTextOption::WrapAnywhere);
    QImage wrapAnywhere = captureImage(label);

    label->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    QCOMPARE(label->wrapMode(), QTextOption::WrapAtWordBoundaryOrAnywhere);
    QImage wrapAtWordBoundaryOrAnywhere = captureImage(label);

    label->setWrapMode(QTextOption::NoWrap);
    QCOMPARE(label->wrapMode(), QTextOption::NoWrap);
    QImage noWrap = captureImage(label);
    
    //No wrapping by default
    QVERIFY(contentWidth(defaultMode) > contentHeight(defaultMode));

    QVERIFY(contentWidth(manualWrap) > contentHeight(manualWrap));
    QVERIFY(contentHeight(wordWrap) > contentWidth(wordWrap));
    QVERIFY(contentHeight(wrapAnywhere) > contentWidth(wrapAnywhere));
    QVERIFY(contentHeight(wrapAtWordBoundaryOrAnywhere) > contentWidth(wrapAtWordBoundaryOrAnywhere));
    QVERIFY(contentWidth(noWrap) > contentHeight(noWrap));

    //Words that don't fit one line are not displayed 
    //completely in wordWrap mode. In wrapAtWordBoundaryOrAnywhere mode these long 
    //words are displayed by using more than one line.
    QVERIFY(contentHeight(wrapAtWordBoundaryOrAnywhere) > contentHeight(wordWrap));

}

void Ut_MLabel::multiLengthSeparator()
{
    label->resize(100, 100);

    QChar separatorChar(0x9c, 0);
    QString separator(separatorChar);

    QString shortWord = "short";
    QString longWord = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    QString shortAndShort = shortWord + separator + shortWord;
    QString longAndShort = longWord + separator + shortWord;

    label->setText(shortAndShort);
    QImage shortAndShortCapture = captureImage(label);
    label->setText(shortWord);
    QImage shortCapture = captureImage(label);

    QCOMPARE(shortCapture.isNull(), false);
    QCOMPARE(shortAndShortCapture.isNull(), false);
    QCOMPARE(shortCapture, shortAndShortCapture);

    label->setText(longAndShort);
    QImage longAndShortCapture = captureImage(label);

    QCOMPARE(shortCapture, longAndShortCapture);

}



QTEST_APPLESS_MAIN(Ut_MLabel);
