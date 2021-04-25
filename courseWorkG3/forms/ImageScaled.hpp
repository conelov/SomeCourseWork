//
// Created by dym on 25.12.2020.
//

#ifndef COURCEWORKG_IMAGESCALED_HPP
#define COURCEWORKG_IMAGESCALED_HPP
//#include "ui_ImageScaled.h"

#include <QLabel>

class ImageScaled final : public QLabel {
Q_OBJECT
public:
    ImageScaled(QWidget *parent = nullptr);

    void setPixmap(QPixmap pixmap);
signals:
    void dropingImages(QStringList);
    void doubleClick();
protected:
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event)override;
    void dragEnterEvent(QDragEnterEvent *event) override;

private:
    void paintEvent(QPaintEvent *event) override;

    QPixmap pixmap_;
};


#endif //COURCEWORKG_IMAGESCALED_HPP
