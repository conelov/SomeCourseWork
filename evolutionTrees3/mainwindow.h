#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QTimer>
#include "AdvancedSettingForm.h"
#include "ArenaToImage.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    std::optional<AdvancedSettingForm> settingForm;
    QTimer timerAutoNext;
    std::optional<gui::Area> area;

    void deadLife();

    void undeadLife();

private slots:

    void buttonNextStep();

    void buttonAutoNext();

    void buttonSetting();

    void timerTimeout();

    void changeConstant();
};

#endif // MAINWINDOW_H
