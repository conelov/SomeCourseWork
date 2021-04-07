//
// Created by dym on 26.12.2020.
//

#ifndef COURSEWORKD_CELLIMAGE_HPP
#define COURSEWORKD_CELLIMAGE_HPP

#include <QLabel>

class MainForm;

class Cell : public QLabel {
Q_OBJECT
public:
    enum Type : uint8_t {null, X, O};
    QPoint const position;

    Cell(QPoint pos, MainForm *const parent);

    Type getType() const {return type_;};

signals:

    void clicked(const Cell &);

protected:
    void mousePressEvent(QMouseEvent *ev) override;

    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap pixmap_;
    const MainForm *parent_;
    Type type_ = null;
};


#endif //COURSEWORKD_CELLIMAGE_HPP
