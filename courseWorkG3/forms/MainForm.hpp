//
// Created by dym on 25.12.2020.
//

#ifndef COURCEWORKG_MAINFORM_HPP
#define COURCEWORKG_MAINFORM_HPP

#include <QButtonGroup>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainForm;
}
QT_END_NAMESPACE

class MainForm final : public QMainWindow {
  Q_OBJECT
public:
  MainForm(QWidget* oldFileFull = nullptr);

  ~MainForm() override;

private:
  Ui::MainForm* ui;
  QStringList imageList;
  decltype(imageList)::const_iterator nImage{};
  QButtonGroup radioFormat;

  void setImageList(QStringList);
  void convertAny(QString path = {});
  void convertAll();

private slots:
void setTheme(bool);
};

#endif // COURCEWORKG_MAINFORM_HPP
