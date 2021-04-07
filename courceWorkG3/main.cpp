#include <QApplication>
#include <QPushButton>
#include "forms/MainForm.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainForm mainForm;
    mainForm.show();

    return QApplication::exec();
}
