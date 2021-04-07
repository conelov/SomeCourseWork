//
// Created by dym on 26.12.2020.
//

#include <QPainter>
#include <QVariant>
#include "mainForm.hpp"
#include "cellImage.hpp"

Cell::Cell(QPoint pos, MainForm *const parent)
        : QLabel(parent), position(pos), parent_(parent) {}

void Cell::mousePressEvent(QMouseEvent *ev) {
    if (type_ == null) {
        if (parent_->isFirstPlayer()) {
            pixmap_ = parent_->pmX;
            type_ = X;
        }
        else {
            pixmap_ = parent_->pmO;
            type_ = O;
        }
        repaint();
        emit clicked(*this);
    }

    QLabel::mousePressEvent(ev);
}

void Cell::paintEvent(QPaintEvent *event) {
    if (!pixmap_.isNull()) {
        QPixmap const scaledPixmap = pixmap_.scaled(size(), Qt::KeepAspectRatio);
        QPainter(this).drawPixmap(
                QPoint((size().width() - scaledPixmap.width()) / 2, (size().height() - scaledPixmap.height()) / 2),
                scaledPixmap);
    }
    QLabel::paintEvent(event);
}
