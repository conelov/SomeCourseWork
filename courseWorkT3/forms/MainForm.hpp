
#ifndef COURSEWORKT_MAINFORM_HPP
#define COURSEWORKT_MAINFORM_HPP

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainForm;
}
class QFileDialog;
QT_END_NAMESPACE

class MainForm final : public QMainWindow {
  Q_OBJECT

  Ui::MainForm* ui;
  std::unique_ptr<QFileDialog> openFilesFD, dirSaveFD;
  QStringList selectedFiles;
  QStringList::const_iterator itViewFile;
  static constexpr auto regRemoveHtml = "<[^>]*>";
  typedef decltype(itViewFile) (*ShifterIterFileList)(decltype(itViewFile), int);

public:
  MainForm(QWidget* parent = nullptr, const Qt::WindowFlags& flags = Qt::WindowFlags());
  virtual ~MainForm();
private slots:
  void selectStyle(bool checked);
  void selectPath(bool checked);
  void fileOpen(bool checked);
  void buttonPreview(bool checked);
  void convertOnce(bool);
  void convertAll(bool);

private:
  void changeText(decltype(itViewFile));
  void checkSelectedPath();
  void convertOnce(QString );
};

#endif // COURSEWORKT_MAINFORM_HPP