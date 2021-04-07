//
// Created by dym on 09.12.2020.
//

#ifndef EVOLUTIONTREES_SCALEDMAINIMAGELABEL_HPP
#define EVOLUTIONTREES_SCALEDMAINIMAGELABEL_HPP

#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include "ArenaToImage.hpp"

class ScaledMainImageLabel : public QLabel{
Q_OBJECT
public:
    gui::Area *area{};
    QScrollArea * scrollWidget{};
    float winScale = 20;

    ScaledMainImageLabel(QWidget *parent = Q_NULLPTR);
protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif //EVOLUTIONTREES_SCALEDMAINIMAGELABEL_HPP
