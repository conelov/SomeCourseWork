//
// Created by dym on 05.12.2020.
//

#include <QtGui/QPainter>
#include "include/ArenaToImage.hpp"

using namespace gui;

#pragma clang diagnostic push

#pragma ide diagnostic ignored "EndlessLoop"

void Area::nextFrame() {
    if (noLife())
        throw std::runtime_error("an empty array of elements passed");

    auto const invertY = [imageSize = image.size()](simulationTree::Point const pos) {
        if (pos.x >= imageSize.width() || pos.y >= imageSize.height())
            throw std::overflow_error(
                    "AreaToIm::nextFrame: the size of the image is less than the position in the input array of cells");
        return QPoint(pos.x, imageSize.height() - pos.y - 1);
    };
    data.clear();
    average.reset();
    life(&data, &average);
    image.fill(fillIndex);
    ////Удалить из пребинденных цветов деревья, не имеющиеся во входном массиве
    std::erase_if(bindColors, [&data = this->data](decltype(bindColors)::value_type const i) {
        return data.cend() == std::ranges::find_if(data, [ptr = i.first](simulationTree::Area::CellLifeResult const j) {
            return ptr == j.idPtr && j.type == simulationTree::Area::TypeCell::treeMass;
        });
    });

    auto const colorsCEnd = image.colorTable().cend();
    auto const colorsCBeginFree = std::next(image.colorTable().cbegin(), firstFreeColorIndex);
    auto iDataCurr = data.cbegin();
    if (bindColors.empty()) {
        for (; iDataCurr != data.cend(); ++iDataCurr)
            if (iDataCurr->type != simulationTree::Area::TypeCell::treeMass)
                image.setPixel(invertY(iDataCurr->position), static_cast<size_t>(iDataCurr->type));
            else {
                auto const[iBind, isBind] = bindColors.emplace(iDataCurr->idPtr, colorsCBeginFree);
                assert(isBind);
                image.setPixel(invertY(iDataCurr->position), firstFreeColorIndex);
                break;
            }
    }
    auto prevEmplace = bindColors.cbegin();
    for (; iDataCurr != data.cend(); ++iDataCurr)
        if (iDataCurr->type != simulationTree::Area::TypeCell::treeMass)
            image.setPixel(invertY(iDataCurr->position), static_cast<size_t>(iDataCurr->type));
        else {
            prevEmplace = bindColors.emplace_hint(prevEmplace, iDataCurr->idPtr,
                                                  std::next(prevEmplace->second) == colorsCEnd
                                                  ? colorsCBeginFree
                                                  : std::next(prevEmplace->second));
            image.setPixel(invertY(iDataCurr->position),
                           std::distance(colorsCBeginFree, prevEmplace->second) + firstFreeColorIndex);
        }
}

#pragma clang diagnostic pop

QPixmap Area::renderObserve(size_t const height) const {
    return QPixmap::fromImage(image).scaled(image.width() * (height / image.height()), height,
                                            Qt::KeepAspectRatioByExpanding);
}

void Area::renderMain(float const inScale, QLabel &label) const {
    auto const scale = inScale * 0.925;
    QSize const scaledSize(image.width() * scale, image.height() * scale);
    label.setFixedSize(scaledSize);

    QPainter painter(&label);
    painter.drawImage(QRect(0, 0, scaledSize.width(), scaledSize.height()), image);
    painter.setFont(QFont("System-ui"));

    for (simulationTree::Area::CellLifeResult const cell : data) {
        QRect const rect(cell.position.x * scale, scaledSize.height() - cell.position.y * scale - scale, scale, scale);
        painter.setPen(cell.type == simulationTree::Area::TypeCell::treeMass ? massPen : otherPen);
        painter.drawRect(rect);
    }
}

bool Area::noFrame() const noexcept {
    return data.empty() || noLife();
}

