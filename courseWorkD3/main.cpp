#include "mainForm.hpp"
#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MainForm form;
  form.show();

  return QApplication::exec();
}