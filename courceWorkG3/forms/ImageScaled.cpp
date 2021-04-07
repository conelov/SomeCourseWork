//
// Created by dym on 25.12.2020.
//

#include <QPainter>
#include<QDebug>
#include <QDropEvent>
#include <QMimeData>
#include "ImageScaled.hpp"

ImageScaled::ImageScaled(QWidget *parent)
        : QLabel(parent) {
}

void ImageScaled::paintEvent(QPaintEvent *const event) {
    if (!pixmap_.isNull()) {
        QPixmap const scaledPixmap = pixmap_.scaled(size(), Qt::KeepAspectRatio);
        QPainter(this).drawPixmap(
                QPoint((size().width() - scaledPixmap.width()) / 2, (size().height() - scaledPixmap.height()) / 2),
                scaledPixmap);
    }
    QLabel::paintEvent(event);
}
void ImageScaled::mousePressEvent(QMouseEvent *event){
    emit doubleClick();
    QLabel::mousePressEvent(event);
}
void ImageScaled::setPixmap(QPixmap const pixmap) {
    pixmap_ = pixmap;
    repaint();
}

void ImageScaled::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
    QWidget::dragEnterEvent(event);
}

void ImageScaled::dropEvent(QDropEvent *event) {
    QStringList images;
    for (auto const &i : event->mimeData()->urls())
        images += i.toLocalFile();
    emit dropingImages(std::move(images));

    QWidget::dropEvent(event);
}


