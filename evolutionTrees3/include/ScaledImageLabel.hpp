//
// Created by dym on 08.12.2020.
//

#ifndef EVOLUTIONTREES_SCALEDIMAGELABEL_HPP
#define EVOLUTIONTREES_SCALEDIMAGELABEL_HPP

#include <QtWidgets/QLabel>
#include "ArenaToImage.hpp"

class ScaledImageLabel : public QLabel {
Q_OBJECT
public:
    gui::Area *area{};

    ScaledImageLabel(QWidget *parent = Q_NULLPTR);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif //EVOLUTIONTREES_SCALEDIMAGELABEL_HPP