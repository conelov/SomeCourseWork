#include <QtGui/QPainter>
#include <ch_FPSControl.h>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./ui_advancedSettingForm.h"

using namespace ch;

MainWindow::~MainWindow() {
    delete ui;
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          timerAutoNext(this),
          area(std::in_place, gui::Area::getDefaultColors(), simulationTree::Point{500, 30}, 100,
               simulationTree::def::ConstantInit{},
               simulationTree::def::MaxMin<simulationTree::def::Value>{5, 1}) {
    ui->setupUi(this);
    QObject::connect(ui->buttonManualStep, &QPushButton::clicked, this, &MainWindow::buttonNextStep);
    QObject::connect(ui->buttonAutoStep, &QPushButton::clicked, this, &MainWindow::buttonAutoNext);
    QObject::connect(ui->buttonAdvancedSettings, &QPushButton::clicked, this, &MainWindow::buttonSetting);
    QObject::connect(&this->timerAutoNext, &QTimer::timeout, this, &MainWindow::timerTimeout);
    QObject::connect(ui->spinBoxFPS, QOverload<int>::of(&QSpinBox::valueChanged),
                     [&timer = timerAutoNext](int const i) {timer.setInterval(1000 / i);});
    QObject::connect(ui->spinBoxSolidLevel, QOverload<int>::of(&QSpinBox::valueChanged),
                     [&area = this->area](int const i) {area->solidLevel = i;});
    QObject::connect(ui->spinBoxSolidPower, QOverload<int>::of(&QSpinBox::valueChanged),
                     [&area = this->area](int const i) {area->solidPower = i;});

    ui->spinBoxSolidLevel->setValue(area->solidLevel);

    ui->spinBoxSolidPower->setValue(area->solidPower);
    ui->mainWindow->area = ui->observeWindow->area = &area.value();
    ui->mainWindow->scrollWidget = ui->scrollArea;
}

void MainWindow::buttonNextStep() {
    if (area->noLife()) {
        deadLife();
    } else {
        undeadLife();
        area->nextFrame();
        ui->labelAverageAge->setText(QString::number(area->average.age));
        ui->labelAverageEnergy->setText(QString::number(area->average.energy));
        ui->labelAverageLen->setText(QString::number(area->average.lenGene));
        ui->labelIteration->setText(QString::number(area->iterations()));
        ui->observeWindow->repaint();
        ui->mainWindow->repaint();
    }
}

void MainWindow::buttonAutoNext() {
    if (this->timerAutoNext.isActive()) {
        this->timerAutoNext.stop();
        ui->buttonAutoStep->setText(QString("auto next"));
        ui->buttonManualStep->setEnabled(true);
    } else {
        ui->buttonAutoStep->setText(QString("stop"));
        ui->buttonManualStep->setEnabled(false);
        this->timerAutoNext.start(1000 / ui->spinBoxFPS->value());
    }
}

void MainWindow::deadLife() {
    ui->observeWindow->hide();
}

void MainWindow::undeadLife() {
    ui->observeWindow->show();
}

void MainWindow::timerTimeout() {
    if (area->noLife()) {
        buttonAutoNext();
        deadLife();
    } else
        buttonNextStep();
}

void MainWindow::buttonSetting() {
    if (!settingForm) {
        settingForm.emplace();
        using namespace simulationTree;
        def::Constant const constant(def::ConstantInit{});
        settingForm->ui->spinBoxMutationChance->setValue(constant.chromoMutatChance);
        settingForm->ui->spinBoxTrueMutationChance->setValue(constant.chromoMutatTrueChance);
        settingForm->ui->spinBoxLenMutationChance->setValue(constant.chromoNewLenChance);
        settingForm->ui->spinBoxAge->setValue(constant.ageInit);
        settingForm->ui->spinBoxBuffHeight->setValue(area->buffEnergyLimitFromHeight);
        settingForm->ui->spinBoxBuffHeightMax->setValue(area->buffEnergyLimitMaxFromHeight);
        settingForm->ui->spinBoxEnergyLimit->setValue(area->energyLimit);
        QObject::connect(settingForm->ui->buttonMutationChanceApply, &QPushButton::clicked, this,
                         &MainWindow::changeConstant);
        QObject::connect(settingForm->ui->buttonMutationChanceApplyExit, &QPushButton::clicked, [this] {
            this->changeConstant();
            settingForm->hide();
        });
    }
    settingForm->show();
}

void MainWindow::changeConstant() {
    using namespace simulationTree;
    area->setConstant({.chromoMutatChance = static_cast<def::Value>(settingForm->ui->spinBoxMutationChance->value()),
                              .chromoMutatTrueChance = static_cast<def::Value>(settingForm->ui->spinBoxTrueMutationChance->value()),
                              .chromoNewLenChance =  static_cast<def::Value>(settingForm->ui->spinBoxLenMutationChance->value()),
                              .ageInit = static_cast<def::Value>(settingForm->ui->spinBoxAge->value())});
}
