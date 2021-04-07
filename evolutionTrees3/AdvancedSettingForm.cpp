//
// Created by dym on 09.12.2020.
//
#include "AdvancedSettingForm.h"
#include "./ui_advancedSettingForm.h"
#include <libTree/include/Definitions.hpp>

AdvancedSettingForm::AdvancedSettingForm(QWidget *parent)
        : QWidget(parent),
          ui(new Ui::advancedSettingForm) {
    ui->setupUi(this);
    using namespace simulationTree;
    ui->spinBoxMutationChance->setMinimum(def::compileConstant.value.min);
    ui->spinBoxMutationChance->setMaximum(def::compileConstant.value.max);
    ui->spinBoxTrueMutationChance->setMinimum(def::compileConstant.value.min);
    ui->spinBoxTrueMutationChance->setMaximum(def::compileConstant.value.max);
    ui->spinBoxLenMutationChance->setMinimum(def::compileConstant.chromoLen.min);
    ui->spinBoxLenMutationChance->setMaximum(def::compileConstant.chromoLen.max);
    ui->spinBoxAge->setMinimum(std::numeric_limits<def::Value>::min());
    ui->spinBoxAge->setMaximum(std::numeric_limits<def::Value>::max());
}
