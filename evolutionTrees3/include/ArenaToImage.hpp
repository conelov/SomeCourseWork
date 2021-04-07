#ifndef EVOLUTIONTREES_ARENATOIMAGE_HPP
#define EVOLUTIONTREES_ARENATOIMAGE_HPP

#include <QtCore/QSize>
#include <QtGui/QPen>
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include "Area.hpp"

namespace gui {
class Area : public simulationTree::Area {
public:
    typedef QVector<QRgb> Colors;

    [[nodiscard]] static Colors getDefaultColors() {
        return Colors{
//                qRgb(25, 25, 25)          /*fill*/,
//                qRgb(170, 170, 170)       /*seed*/,
//                qRgb(220, 220, 220)       /*active*/,
//                qRgb(190, 190, 190)       /*passive*/,
//                qRgb(47, 79, 79),
//                qRgb(97, 64, 81),
//                qRgb(109, 101, 82),
//                qRgb(0, 49, 83),
//                qRgb(30, 89, 69),
//                qRgb(52, 59, 41)
//                qRgb(25, 25, 25)          /*fill*/,
//                qRgb(170, 170, 170)       /*seed*/,
//                qRgb(220, 220, 220)       /*active*/,
//                qRgb(188, 143, 143)       /*passive*/,
//                qRgb(178, 34, 34),
//                qRgb(219, 112, 147),
//                qRgb(109, 101, 82),
//                qRgb(147, 112, 219),
//                qRgb(218, 165, 32),
//                qRgb(0, 191, 255),
//                qRgb(30, 144, 255),
//                qRgb(154, 205, 50),
//                qRgb(60, 179, 113)
//                qRgb(25, 25, 25)          /*fill*/,
//                qRgb(170, 170, 170)       /*seed*/,
//                qRgb(220, 220, 220)       /*active*/,
//                qRgb(188, 143, 143)       /*passive*/,
//                qRgb(250, 128, 114),
//                qRgb(238, 232, 170),
//                qRgb(238, 232, 170),
//                qRgb(216, 191, 216),
//                qRgb(224, 255, 255),
//                qRgb(176, 224, 230),
//                qRgb(222, 184, 135),
//                qRgb(107, 142, 35),
//                qRgb(60, 179, 113)
                qRgb(35, 35, 35)          /*fill*/,
                qRgb(170, 170, 170)       /*seed*/,
                qRgb(200, 200, 200)       /*active*/,
                qRgb(120, 120, 120)       /*passive*/,
                qRgb(250, 128, 114),
                qRgb(238, 232, 170),
                qRgb(238, 232, 170),
                qRgb(216, 191, 216),
                qRgb(224, 255, 255),
                qRgb(176, 224, 230),
                qRgb(222, 184, 135),
                qRgb(107, 142, 35),
                qRgb(60, 179, 113),
                qRgb(255, 204, 204),
                qRgb(176, 134, 156),
                qRgb(176, 134, 164),
                qRgb(165, 134, 176),
                qRgb(120, 129, 173),
                qRgb(100, 140, 140),
                qRgb(100, 140, 131),
                qRgb(106, 153, 116),
                qRgb(47, 97, 40),
                qRgb(122, 153, 98),
                qRgb(140, 148, 89),
                qRgb(163, 161, 88),
                qRgb(179, 155, 96),
                qRgb(196, 154, 106)
        };
    }

    static constexpr size_t fillIndex = 0,
            firstFreeColorIndex =
            std::max({static_cast<const size_t>(simulationTree::Area::TypeCell::seed),
                      static_cast<const size_t>(simulationTree::Area::TypeCell::treeActive),
                      static_cast<const size_t>(simulationTree::Area::TypeCell::treePassive)}) + 1;
private:
    QImage image;
    std::map<simulationTree::TreeBase const *const, Colors::const_iterator const> bindColors{};
    Area::LifeResultContainer data;
public:
    QPen massPen, otherPen;
    Area::Average average;

    Area() = delete;

    template<typename ...Args>
    Area(Colors const inColors, QSize inB, Args &&...args)
            : Area(inColors, {inB.width(), inB.height()}, std::forward<Args>(args)...) {}

    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "EndlessLoop"

    template<typename ...Args>
    Area(Colors const inColors, simulationTree::Point const inB, Args &&...args)
            : simulationTree::Area(inB, std::forward<Args>(args)...),
              image(QSize(inB.x, inB.y), QImage::Format_Indexed8),
              massPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin),
              otherPen(Qt::white, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin) {
        image.setColorTable(inColors);
    }

    #pragma clang diagnostic pop

    [[nodiscard]] bool noFrame() const noexcept;

    void nextFrame();

    [[nodiscard]] QPixmap renderObserve(size_t) const;

    void renderMain(float, QLabel &label) const;
};
}
#endif //EVOLUTIONTREES_ARENATOIMAGE_HPP
