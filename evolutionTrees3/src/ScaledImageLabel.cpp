//
// Created by dym on 08.12.2020.
//
#include <QtGui/QPainter>
#include "ScaledImageLabel.hpp"

ScaledImageLabel::ScaledImageLabel(QWidget *parent)
        : QLabel(parent) {}

void ScaledImageLabel::paintEvent(QPaintEvent *const event) {
    if (area && !area->noFrame()) {
        QPixmap const pixmap = area->renderObserve(height());
        QPainter(this).drawPixmap(QPoint(
                (size().width() - pixmap.width()) / 2, (size().height() - pixmap.height()) / 2),
                                  pixmap);
        setMinimumSize(pixmap.size());
    }
    QLabel::paintEvent(event);
}