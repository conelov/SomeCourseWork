#include "forms/MainForm.hpp"
#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainForm mainForm;
  mainForm.show();
  return QApplication::exec();
  return 0;
}
