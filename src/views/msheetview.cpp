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

#include <mscenemanager.h>
#include "mviewcreator.h"
#include <msheet.h>
#include "msheetview.h"
#include "msheetview_p.h"
#include "mabstractwidgetanimation.h"
#include <MDebug>

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

//////////////
/// MSheetSlot

MSheetSlot::MSheetSlot(QGraphicsItem *parent) : MStylableWidget(parent)
{
    connect(this, SIGNAL(geometryChanged()), SLOT(resizeChildWidget()));
}

MSheetSlot::~MSheetSlot()
{
    // Widget belongs to controller. Remove it from scene graph.
    QGraphicsWidget *widget = widgetPointer.data();
    if (widget) {
        widget->setParentItem(0);
        scene()->removeItem(widget);
        widgetPointer.clear();
    }
}

void MSheetSlot::setWidget(QGraphicsWidget *widget)
{
    QGraphicsWidget *currentWidget = widgetPointer.data();

    if (widget == currentWidget)
        return;

    if (currentWidget) {
        currentWidget->setParentItem(0);
        if (scene())
            scene()->removeItem(currentWidget);
    }

    if (widget) {
        widget->setParentItem(this);
        widget->setPos(0.0f, 0.0f);
        widgetPointer = widget;
        resizeChildWidget();
    } else {
        widgetPointer.clear();
    }
}

void MSheetSlot::resizeChildWidget()
{
    QGraphicsWidget *widget = widgetPointer.data();
    if (widget) {
        widget->resize(size());
    }
}

//////////////
/// MSheetViewPrivate

MSheetViewPrivate::MSheetViewPrivate()
    : q_ptr(0),
      rootLayout(0),
      headerSlot(0),
      centralSlot(0),
      headerAnimation(0)
{
}

MSheetViewPrivate::~MSheetViewPrivate()
{
    //rootLayout->removeItem(headerSlot);
    delete headerSlot;
    headerSlot = 0;

    //rootLayout->removeItem(centralSlot);
    delete centralSlot;
    centralSlot = 0;

    if (qobject_cast<MSheet *>(controller)) {
        // controller is still valid.
        // causes rootLayout to be deleted
        controller->setLayout(0);
    }
    rootLayout = 0;
}

void MSheetViewPrivate::init()
{
    rootLayout = new QGraphicsAnchorLayout(controller);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    headerSlot = new MSheetSlot(controller);
    headerSlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    rootLayout->addCornerAnchors(headerSlot, Qt::TopRightCorner, rootLayout, Qt::TopRightCorner);

    centralSlot = new MSheetSlot(controller);
    centralSlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rootLayout->addCornerAnchors(centralSlot, Qt::TopLeftCorner, headerSlot, Qt::BottomLeftCorner);
    rootLayout->addCornerAnchors(centralSlot, Qt::BottomRightCorner, rootLayout, Qt::BottomRightCorner);
}

void MSheetViewPrivate::updateStyle()
{
    Q_Q(MSheetView);

    headerSlot->setStyleName(q->style()->headerSlotStyleName());
    centralSlot->setStyleName(q->style()->centralSlotStyleName());

    delete headerAnimation;
    if (!q->style()->headerAnimation().isEmpty()) {
        headerAnimation = qobject_cast<MAbstractWidgetAnimation*>(MTheme::animation(q->style()->headerAnimation()));
        headerAnimation->setTargetWidget(headerSlot);
    }
}

void MSheetViewPrivate::updateHeaderVisibility()
{
    Q_Q(MSheetView);
    if (q->model()->headerVisible())
        headerSlot->setPos(0,0);

    if (headerAnimation) {
        if (q->model()->headerVisible())
            headerAnimation->setTransitionDirection(MAbstractWidgetAnimation::In);
        else
            headerAnimation->setTransitionDirection(MAbstractWidgetAnimation::Out);
        headerAnimation->start();
    }
}

//////////////
/// MSheetView

MSheetView::MSheetView(MSheet *controller) :
    MSceneWindowView(*(new MSheetViewPrivate), controller)
{
    Q_D(MSheetView);
    d->q_ptr = this;
    d->init();
}

MSheetView::~MSheetView()
{
}

void MSheetView::setupModel()
{
    MSceneWindowView::setupModel();

    QList<const char*> modifications;
    modifications << MSheetModel::HeaderWidget;
    modifications << MSheetModel::CentralWidget;
    modifications << MSheetModel::HeaderVisible;

    updateData(modifications);
}

void MSheetView::updateData(const QList<const char *> &modifications)
{
    Q_D(MSheetView);
    MSceneWindowView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == MSheetModel::HeaderVisible)
            d->updateHeaderVisibility();
        else if (member == MSheetModel::CentralWidget)
            d->centralSlot->setWidget(model()->centralWidget());
        else if (member == MSheetModel::HeaderWidget)
            d->headerSlot->setWidget(model()->headerWidget());
    }
}

void MSheetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't let it propagate to widgets below
    event->accept();
}

void MSheetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't let it propagate to widgets below
    event->accept();
}

void MSheetView::applyStyle()
{
    Q_D(MSheetView);

    MSceneWindowView::applyStyle();
    d->updateStyle();
}

M_REGISTER_VIEW_NEW(MSheetView, MSheet)
