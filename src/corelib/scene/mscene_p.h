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

#ifndef MSCENE_P_H
#define MSCENE_P_H

#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QList>

class MSceneManager;
class MOnDisplayChangeEvent;
class QGraphicsSceneMouseEvent;

class MScenePrivate
{
    Q_DECLARE_PUBLIC(MScene)

    struct Fps
    {
        Fps() : frameCount(-1), fps(0.0f) {}
        int frameCount;
        QTime time;
        float fps;
    };
    struct FpsLog
    {
        QTextStream stream;
        QFile output;
    };

public:
    MScenePrivate();
    virtual ~MScenePrivate();

    void init();
    void setSceneManager(MSceneManager *sceneManager);

    void onDisplayChangeEvent(MOnDisplayChangeEvent *event);
    QList<MWidget *> filterMWidgets(QList<QGraphicsItem *> itemsList);
    void sendEventToMWidgets(QList<MWidget *> itemsList, QEvent *event);

    QBrush fpsBackgroundBrush;
    QPen   boundingRectLinePen;
    QBrush boundingRectFillBrush;

    void showFpsCounter(QPainter *painter, float fps);
    void logFpsCounter(const QTime *timestamp, float fps);

    void touchPointCopyPosToLastPos(QTouchEvent::TouchPoint &point);
    void touchPointCopyMousePosToPointPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    void touchPointCopyMousePosToPointStartPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    void touchPointMirrorMousePosToPointPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    void touchPointMirrorMousePosToPointStartPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);

    bool eventEmulateTwoFingerGestures(QEvent *event);
    bool eventEmulatePinch(QEvent *event);
    void fillMarginRectWithPattern(QPainter *painter, const QRectF& rect, int thickness);

    void handleGestureEvent(QEvent* event);

    /*
     MeegoTouch needs to have widgets that stop
     propagation of gestures. We implement that using
     MSceneWindows, which are supposed to swallow all
     gesture events. The scene windows on the other hand
     need to inform the scene that they swallowed gesture
     event and they use the notifyGestureCaughtByPanel
     method.
     */
    void notifyGestureCaughtByPanel(Qt::GestureType gestureType);
    void notifyGestureAcceptedByChild(Qt::GestureType gestureType);

public Q_SLOTS:
    void _q_initialPressDeliveryTimeout();

protected:
    MScene *q_ptr;
    MSceneManager *manager;

    QGraphicsWidget* eventEater;
    bool cancelSent;
    QTimer *initialPressTimer;
    QGraphicsSceneMouseEvent mousePressEvent;

    //Two finger gestures emulation variables
    QTouchEvent::TouchPoint emuPoint1, emuPoint2;
    bool pinchEmulationEnabled;

    Fps fps;
    FpsLog fpsLog;

    QList<Qt::GestureType>   childrenAcceptedGestures;
    QList<Qt::GestureType>   panelAcceptedGestures;
};

#endif // MSCENE_P_H
