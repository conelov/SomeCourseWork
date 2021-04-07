//
// Created by dym on 26.12.2020.
//

#ifndef COURSEWORKD_MAINFORM_HPP
#define COURSEWORKD_MAINFORM_HPP

#include "cellImage.hpp"
#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class QLineEdit;
class QLabel;
class MainForm final : public QMainWindow {
  Q_OBJECT
public:
  QPixmap const pmX, pmO;

  explicit MainForm(QWidget* parent = nullptr);

  ~MainForm() override;

  bool isFirstPlayer() const { return firstPlayer; };

private:
  Ui::MainWindow* ui;
  std::vector<std::unique_ptr<Cell>> cells;
  QLineEdit *lineEdit_W, *lineEdit_H, *lineEdit_nWin;
  QLabel* labelPlayer;
  bool firstPlayer = true;

  [[nodiscard]] QLineEdit* createLineEdit(const QString litera, const QString mask = "99") const;

private slots:

  void newGame();

  void clickedCellAny(Cell const&);
};

#endif // COURSEWORKD_MAINFORM_HPP
