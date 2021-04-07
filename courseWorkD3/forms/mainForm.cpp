//
// Created by dym on 26.12.2020.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainForm.h" resolved

#include "mainForm.hpp"
#include "ui_mainForm.h"
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSizePolicy>
#include <cmath>
#include <functional>
#include <vector>

namespace {
template <typename Predicate>
class Accumulator {
  Predicate predicate_;
  QPoint prevPosition{};
  Cell::Type type = Cell::null;
  uint8_t coutn{};

public:
  Accumulator(Predicate&& predicate) : predicate_(std::forward<Predicate>(predicate)) {}

  uint8_t operator()(Cell const& cell) {
    if (!(cell.getType() == type && predicate_(prevPosition, cell.position))) {
      type = cell.getType();
      coutn = {};
      prevPosition = cell.position;
    }
    prevPosition = cell.position;
    return ++coutn;
  }
};

} // namespace

MainForm::~MainForm() {
  delete lineEdit_W;
  delete lineEdit_H;
  delete ui;
}

QLineEdit* MainForm::createLineEdit(const QString litera, const QString mask) const {
  auto lineEdit = new QLineEdit((QWidget*)this);
  lineEdit->setObjectName("lineEdit_" + litera);
  lineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  lineEdit->setFixedWidth(35);
  lineEdit->setMaxLength(2);
  lineEdit->setPlaceholderText(litera + ":");
  lineEdit->setInputMask(mask);
  return lineEdit;
}

MainForm::MainForm(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), lineEdit_W(createLineEdit("W")), lineEdit_H(createLineEdit("H")),
      lineEdit_nWin(createLineEdit("Win", "9")), pmX(QPixmap("./X.jpg")), pmO(QPixmap("./O.jpg")),
      labelPlayer(new QLabel(" Xодит игрок X", this)) {
  ui->setupUi(this);
  ui->toolBar->addWidget(lineEdit_W);
  ui->toolBar->addWidget(lineEdit_H);
  ui->toolBar->addWidget(lineEdit_nWin);
  ui->toolBar->addSeparator();
  ui->toolBar->addWidget(labelPlayer);
  QObject::connect(lineEdit_nWin, &QLineEdit::returnPressed, this, &MainForm::newGame);
  QObject::connect(lineEdit_W, &QLineEdit::returnPressed, this, &MainForm::newGame);
  QObject::connect(lineEdit_H, &QLineEdit::returnPressed, this, &MainForm::newGame);
}

void MainForm::newGame() {
  const auto sizeH = lineEdit_H->text().toUInt(), sizeW = lineEdit_W->text().toUInt();
  //  if (sizeH > 30 || sizeW > 30) {
  //    QMessageBox::warning(

  cells.clear();
  for (size_t h{}; h < sizeH; ++h)
    for (size_t w{}; w < sizeW; ++w) {
      cells.push_back(std::make_unique<Cell>(QPoint(w, h), this));
      cells.back()->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
      cells.back()->setFrameShape(QFrame::Box);
      cells.back()->setFrameShadow(QFrame::Plain);
      QObject::connect(cells.back().get(), &Cell::clicked, this, &MainForm::clickedCellAny);
      ui->gridLayout->addWidget(cells.back().get(), h, w);
    }
}

void MainForm::clickedCellAny(Cell const& cellClicked) {
  {
    auto playerLitera = labelPlayer->text();
    playerLitera.back() = (firstPlayer = !firstPlayer) ? 'X' : 'O';
    labelPlayer->setText(playerLitera);
  }

  const auto winLen = lineEdit_nWin->text().toUInt();
  struct CellRef {
    decltype(cells)::pointer cell;
    QPoint position;
    CellRef(decltype(cells)::reference inCell, QPoint inPonit) : cell(&inCell), position(inPonit) {}
    CellRef(decltype(cells)::reference inCell) : CellRef(inCell, inCell.get()->position) {}
  };
  std::vector<CellRef> activeCells;
  typedef decltype(activeCells)::iterator ActiveCellsIt;
  typedef decltype(QPoint{}.x()) (QPoint::*PointDimension)() const;
  auto const checkLines = [this, &activeCells, winLen](PointDimension const memFn) {
    std::sort(activeCells.begin(), activeCells.end(),
        [memFn](CellRef const& lhs, CellRef const& rhs) { return (lhs.position.*memFn)() < (rhs.position.*memFn)(); });
    ActiveCellsIt first = activeCells.begin(), last;
    do {
      last = std::find_if(first, activeCells.end(),
          [memFn, border = (first->position.*memFn)()](CellRef& i) { return (i.position.*memFn)() != border; });
      Accumulator accum(
          [](QPoint prev, QPoint curr) { return std::abs(prev.x() - curr.x()) + std::abs(prev.y() - curr.y()) == 1; });
      for (auto i = first; i != last; ++i)
        if (accum(**i->cell) >= winLen)
          return true;
      first = last;
    } while (first != activeCells.end());
    return false;
  };

  std::copy_if(cells.begin(), cells.end(), std::back_inserter(activeCells),
      [](decltype(cells)::reference i) { return i->getType() != Cell::null; });
  if (cells.size() == activeCells.size()) {
    QMessageBox::information(this, "Конец игры", "Ничья");
    cells.clear();
    return;
  }
  if (checkLines(&QPoint::x) || checkLines(&QPoint::y)) {
    QMessageBox::information(this, "Конец игры", QString("Победил игрок %1").arg(firstPlayer ? "O" : "X"));
    cells.clear();
    return;
  }

  //  std::ranges::for_each(
  //      activeCells, [transform = QTransform().rotate(45),
  //                       oPos = QPoint(lineEdit_W->text().toUInt() / 2, lineEdit_H->text().toUInt() / 2)](
  //                       CellRef& cell) { cell.position = transform.map(cell.position - oPos) + oPos; });
  //  if (checkLines(&QPoint::x) || checkLines(&QPoint::y))
  //    return;
}
