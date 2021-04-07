//
// Created by dym on 09.12.2020.
//

#ifndef EVOLUTIONTREES_ADVANCEDSETTINGFORM_HPP
#define EVOLUTIONTREES_ADVANCEDSETTINGFORM_HPP

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {class advancedSettingForm;}
QT_END_NAMESPACE
class MainWindow;

class AdvancedSettingForm : public QWidget {
    friend MainWindow;
Q_OBJECT
public:
    AdvancedSettingForm(QWidget *parent = nullptr);

private:
    Ui::advancedSettingForm *ui;
};


#endif //EVOLUTIONTREES_ADVANCEDSETTINGFORM_HPP
