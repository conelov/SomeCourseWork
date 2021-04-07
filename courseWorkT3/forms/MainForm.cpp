
#include "MainForm.hpp"
#include "ui_MainForm.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStyleFactory>
#include <QXmlStreamReader>
namespace {
class ErrFileMsgI {
protected:
  QString const fileName;
  int callMB(QWidget* parent, char const* title, char const* prev) const {
    return QMessageBox::warning(parent, title, prev + fileName + ". Повторить попытку?", QMessageBox::Cancel,
        QMessageBox::Yes, QMessageBox::No);
  }

public:
  ErrFileMsgI(const QString fileName) : fileName(fileName) {}

  virtual int exec(QWidget* parent) const = 0;
};

class FileExist : public ErrFileMsgI {
public:
  FileExist(const QString fileName) : ErrFileMsgI(fileName) {}

  [[nodiscard]] int exec(QWidget* parent) const override {
    return callMB(parent, "Файл существует", "Невозможно сохранить файл");
  }
};

class FileOpen : public ErrFileMsgI {
public:
  FileOpen(const QString fileName) : ErrFileMsgI(fileName) {}

  [[nodiscard]] int exec(QWidget* parent) const override {
    return callMB(parent, "Невозможно открыть файл", "Неизвестный формат файла ");
  }
};
auto const removeHtml = [](QString const htmlString) {
  QXmlStreamReader xml(htmlString);
  QString textString;
  while (!xml.atEnd()) {
    if (xml.readNext() == QXmlStreamReader::Characters) {
      textString += xml.text();
    }
  }
  return textString;
};
} // namespace

MainForm::~MainForm() { delete ui; }
void MainForm::selectStyle(bool checked) {
  auto action = qobject_cast<QAction*>(sender());
  assert(action);
  QApplication::setStyle(QStyleFactory::create(action->text()));
}
MainForm::MainForm(QWidget* parent, const Qt::WindowFlags& flags)
    : QMainWindow(parent, flags), ui(new Ui::MainForm),
      openFilesFD(new QFileDialog(this, "Выбрать файлы", "./", "Текст (*.rtf)")),
      dirSaveFD(new QFileDialog(this, "Выбрать директорию сохранения", "./")) {
  ui->setupUi(this);
  openFilesFD->setAcceptMode(QFileDialog::AcceptOpen);
  openFilesFD->setFileMode(QFileDialog::ExistingFiles);
  dirSaveFD->setAcceptMode(QFileDialog::AcceptOpen);
  dirSaveFD->setFileMode(QFileDialog::Directory);
  dirSaveFD->setDirectory("./");
  for (auto const& style : QStyleFactory::keys()) {
    auto action = new QAction(style, this);
    QObject::connect(action, &QAction::triggered, this, &MainForm::selectStyle);
    ui->menuThemes->addAction(action);
  }
  QObject::connect(ui->checkBoxSimpleText, &QCheckBox::toggled, this, [this] {
    ui->checkBoxIsRegex->setEnabled(!ui->checkBoxIsRegex->isEnabled());
    if (!selectedFiles.empty())
      changeText(itViewFile);
  });
}
void MainForm::selectPath(bool checked) {
  if (!dirSaveFD->exec())
    return;
  ui->labelCurrentDirectorySave->setText(dirSaveFD->selectedFiles().front());
  if (ui->labelCurrentFile->text().isEmpty())
    return;
  ui->pushButtonConvertAll->setEnabled(true);
  ui->pushButtonConvertOnce->setEnabled(true);
}
void MainForm::checkSelectedPath() {
  if (ui->labelCurrentDirectorySave->text().isEmpty())
    selectPath({});
}
void MainForm::fileOpen(bool checked) {
  ui->pushButtonPrev->setEnabled(false);
  if (!openFilesFD->exec()) {
    ui->pushButtonNext->setEnabled(false);
    return;
  }
  selectedFiles = openFilesFD->selectedFiles();
  if (selectedFiles.size() > 1)
    ui->pushButtonNext->setEnabled(true);
  changeText(selectedFiles.cbegin());
  ui->progressBar->setValue(0);
  if (ui->labelCurrentDirectorySave->text().isEmpty())
    return;
  ui->pushButtonConvertAll->setEnabled(true);
  ui->pushButtonConvertOnce->setEnabled(true);
}
void MainForm::buttonPreview(bool checked) {
  auto send = qobject_cast<QPushButton*>(sender());
  if (&*send == &*ui->pushButtonPrev) {
    if (std::prev(itViewFile) == selectedFiles.cbegin())
      ui->pushButtonPrev->setEnabled(false);
    ui->pushButtonNext->setEnabled(true);
    changeText(std::prev(itViewFile));
  } else {
    if (std::next(itViewFile, 2) == selectedFiles.cend())
      ui->pushButtonNext->setEnabled(false);
    ui->pushButtonPrev->setEnabled(true);
    changeText(std::next(itViewFile));
  }
}
void MainForm::changeText(decltype(itViewFile) itNew) {
  ui->statusbar->showMessage("Загрузка ...");
  ui->statusbar->repaint();
  QFile file(*(itViewFile = itNew));
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    ui->statusbar->showMessage("Невозможно открыть файл");
    ui->labelCurrentFile->clear();
    return;
  }
  ui->labelCurrentFile->setText(QFileInfo(file).filePath());

  auto const array = file.readAll();
  ui->sourceText->setPlainText(array);
  ui->convertedText->setPlainText(ui->checkBoxSimpleText->checkState() == Qt::Checked
                                      ? ui->checkBoxIsRegex->checkState() == Qt::Checked
                                            ? QTextDocument{}.toHtml(array).remove(QRegExp(regRemoveHtml))
                                            : removeHtml(QTextDocument{}.toHtml(array))
                                      : QTextDocument{}.toHtml(array));
  ui->statusbar->clearMessage();
}
void MainForm::convertOnce(bool) {
  do {
    try {
      convertOnce(QString{});
    } catch (FileExist& ex) {
      if (ex.exec(this) == QMessageBox::Yes)
        continue;
    }
    break;
  } while (true);
}
void MainForm::convertOnce(QString const fileName) {
  bool const once = fileName.isEmpty();
  if (once) {
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(0);
  }
  QFileInfo const fileInfo(once ? *itViewFile : fileName);
  const bool isSimpleText = ui->checkBoxSimpleText->checkState() == Qt::Checked;
  QFile file(ui->labelCurrentDirectorySave->text() + "/" + fileInfo.baseName() + (isSimpleText ? +".txt" : ".html"));
  if (file.exists())
    throw FileExist(fileInfo.filePath());
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    assert(false);
  QTextStream(&file) << (isSimpleText ? ui->convertedText->toPlainText() : ui->convertedText->document()->toRawText());
  if (once) {
    ui->progressBar->setValue(1);
    ui->statusbar->showMessage(QString("Конвертация файла %1 завершена").arg(QFileInfo(*itViewFile).fileName()));
  }
}
void MainForm::convertAll(bool) {
  ui->progressBar->setMaximum(selectedFiles.size());
  ui->progressBar->setValue(0);
  for (size_t i{}; i < selectedFiles.size(); ++i)
    do {
      try {
        ui->progressBar->setValue(i);
        convertOnce(selectedFiles[i]);
      } catch (FileExist& ex) {
        if (auto const r = ex.exec(this); r == QMessageBox::Yes)
          continue;
        else if (r == QMessageBox::Cancel)
          goto A;
      }
      break;
    } while (true);
  ui->statusbar->showMessage(QString("Конвертация %1 файлов завершена").arg(selectedFiles.size()));
A:
  ui->progressBar->setValue(selectedFiles.size());
}
