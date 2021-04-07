//
// Created by dym on 25.12.2020.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainForm.h" resolved

#include "MainForm.hpp"
#include "ui_MainForm.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStyleFactory>
#include <utility>

namespace {
class ErrMessageBox {
protected:
  QString const fileName;
  int callMB(QWidget* parent, char const* title, char const* prev) const {
    return QMessageBox::warning(
        parent, title, prev + fileName + ". Повторить попытку?", QMessageBox::Abort, QMessageBox::No, QMessageBox::Yes);
  }

public:
  ErrMessageBox(const QString fileName) : fileName(fileName) {}

  [[nodiscard]] virtual int exec(QWidget* parent) const = 0;
};

class FileCollision : public ErrMessageBox {
public:
  FileCollision(const QString fileName) : ErrMessageBox(fileName) {}

  int exec(QWidget* parent) const override {
    return callMB(parent, "Коллизия имён файлов", "Невозможно сохранить файл ");
  }
};

class FileOpen : public ErrMessageBox {
public:
  FileOpen(const QString fileName) : ErrMessageBox(fileName) {}

  int exec(QWidget* parent) const override {
    return callMB(parent, "Невозможно открыть файл", "Неизвестный формат файла ");
  }
};
} // namespace

MainForm::~MainForm() { delete ui; }

MainForm::MainForm(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainForm), radioFormat(this) {
  ui->setupUi(this);
  ui->pushButtonImagePrev->setEnabled(false);
  ui->pushButtonImageNext->setEnabled(false);
  ui->pushButtonConvertAll->hide();
  for (auto const& themeName : QStyleFactory::keys()) {
    auto action = new QAction(themeName, this);
    QObject::connect(action, &QAction::triggered, this, &MainForm::setTheme);
    ui->menu->addAction(action);
  }
  radioFormat.addButton(ui->radioButtonJPG);
  radioFormat.addButton(ui->radioButtonBMP);
  radioFormat.addButton(ui->radioButtonGIF);
  radioFormat.addButton(ui->radioButtonJPEG);
  radioFormat.addButton(ui->radioButtonPBM);
  radioFormat.addButton(ui->radioButtonPGM);
  radioFormat.addButton(ui->radioButtonPNG);
  radioFormat.addButton(ui->radioButtonPPM);
  QObject::connect(ui->pushButtonSelect, &QPushButton::clicked, this, [this] {
    ui->progressBar->setValue(0);
    QFileDialog fileDialog(this);
    fileDialog.setDirectory("./");
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (fileDialog.exec())
      setImageList(fileDialog.selectedFiles());
  });
  QObject::connect(ui->image, &ImageScaled::dropingImages, this, &MainForm::setImageList);
  QObject::connect(ui->pushButtonImageNext, &QPushButton::clicked, this, [this] {
    ui->pushButtonImagePrev->setEnabled(true);
    ui->image->setPixmap(*++nImage);
    if (std::next(nImage) == imageList.cend())
      ui->pushButtonImageNext->setEnabled(false);
  });
  QObject::connect(ui->pushButtonImagePrev, &QPushButton::clicked, this, [this] {
    ui->pushButtonImageNext->setEnabled(true);
    ui->image->setPixmap(*--nImage);
    if (nImage == imageList.begin())
      ui->pushButtonImagePrev->setEnabled(false);
  });
  QObject::connect(ui->pushButtonConvertAny, &QPushButton::clicked, this, [this] {
    ui->progressBar->setMaximum( 1);
    ui->progressBar->setValue(1);
    try {
      convertAny();
    } catch (FileCollision const& ex) {
      ex.exec(this);
    } catch (FileOpen const& ex) {
      ex.exec(this);
    }
  });
  QObject::connect(ui->pushButtonConvertAll, &QPushButton::clicked, this, &MainForm::convertAll);
  QObject::connect(ui->image, &ImageScaled::doubleClick, this, [this] { ui->pushButtonSelect->clicked(); });
}

void MainForm::setImageList(QStringList inImage) {
  ui->pushButtonImagePrev->setEnabled(false);
  if (inImage.size() > 1) {
    ui->pushButtonImageNext->setEnabled(true);
    ui->pushButtonConvertAll->show();
    ui->pushButtonConvertAny->setText("Конвертировать выбранное");
    ui->statusbar->showMessage(QString("Выбрано %1 изображения для конвертации").arg(inImage.size()));
  } else {
    ui->pushButtonImageNext->setEnabled(false);
    ui->pushButtonConvertAll->hide();
    ui->pushButtonConvertAny->setText("Конвертировать");
    ui->statusbar->showMessage(QString("Выбрано " + inImage.front() + " изображение"));
  }
  ui->image->setPixmap(*(nImage = (imageList = std::move(inImage)).cbegin()));
}

void MainForm::convertAny(QString const path) {
  auto const formatStr = radioFormat.checkedButton()->text().toStdString();
  auto const formatting = [ui = ui, format = formatStr.c_str(), suffix = ui->lineEditNewFileName->text(),
                              multi = !path.isEmpty()](QString const oldFileFull) {
    QFileInfo const fileInfo(oldFileFull);
    QString const newFileFull = fileInfo.path() + "/" +
                                (suffix.isEmpty() ? fileInfo.baseName()
                                    : multi       ? suffix + fileInfo.baseName()
                                                  : suffix) +
                                "." + QString(format).toLower();
    if (QFileInfo::exists(newFileFull))
      throw FileCollision(newFileFull);
    if (QImage image; image.load(oldFileFull))
      image.save(newFileFull, format);
    else
      throw FileOpen(newFileFull);
    const QFileInfo fileInfoOld(oldFileFull);
    ui->statusbar->showMessage(
        QString(fileInfoOld.fileName() + " сохранено в " + fileInfoOld.path() + " в формате " + format));
  };

  if (auto fileName = path.isEmpty() ? *nImage : path; path.isEmpty())
    try {
      formatting(fileName);
    } catch (ErrMessageBox& ex) {
      if (ex.exec(this) == QMessageBox::Yes)
        convertAny(path);
    }
  else
    formatting(fileName);
}

void MainForm::convertAll() {
  ui->progressBar->setMaximum(imageList.size());
  ui->progressBar->setValue(0);
  for (size_t i{}; i < imageList.size(); ++i) {
    try {
      convertAny(imageList[i]);
    } catch (ErrMessageBox& ex) {
      if (int answer = ex.exec(this); answer == QMessageBox::Abort)
        break;
      else if (answer == QMessageBox::Yes)
        try {
          convertAny(imageList[i]);
        } catch (ErrMessageBox& ex) {
          if (ex.exec(this) == QMessageBox ::Yes)
            convertAny(imageList[i]);
        }
    }
    ui->progressBar->setValue(i + 1u);
  }
  ui->statusbar->showMessage(QString("Конвертированно %1 изображений").arg(imageList.size()));
}
void MainForm::setTheme(bool checked) {
  auto action = qobject_cast<QAction*>(sender());
  assert(action);
  QApplication::setStyle(QStyleFactory::create(action->text()));
}
