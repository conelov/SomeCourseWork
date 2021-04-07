//
// Created by dym on 09.12.2020.
//

#include <QtGui/QPainter>
#include "ScaledMainImageLabel.hpp"

ScaledMainImageLabel::ScaledMainImageLabel(QWidget *parent)
        : QLabel(parent) {}

void ScaledMainImageLabel::paintEvent(QPaintEvent *event) {
    assert(scrollWidget);
    if (area && !area->noFrame()) {
        area->renderMain(std::min(winScale, static_cast<float>(scrollWidget->height()) / area->borders().y), *this);
    } else {
        QPainter painter(this);
        painter.setFont(QFont("Arial"));
        painter.drawText(QPoint(size().width() / 2, size().height() / 2), "No life");
        setMinimumHeight(0);
    }

    QLabel::paintEvent(event);
}