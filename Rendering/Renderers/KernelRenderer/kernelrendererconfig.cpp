#include "kernelrendererconfig.h"
#include "Utils/qtutils.h"
#include "ui_kernelrendererconfig.h"

KernelRendererConfig::KernelRendererConfig(QWidget *parent) :
    BaseRendererConfig(parent),
    ui(new Ui::KernelRendererConfig)
    {ui->setupUi(this);}

KernelRendererConfig::~KernelRendererConfig()
{
    delete ui;
}

void KernelRendererConfig::readConfig()
{
    float defaultValue = 1.0f;

    this->wireFrameColors.x = QtUtils::readFloatFromQText(ui->lineEdit_WF_r->text(), defaultValue);
    this->wireFrameColors.y = QtUtils::readFloatFromQText(ui->lineEdit_WF_g->text(), defaultValue);
    this->wireFrameColors.z = QtUtils::readFloatFromQText(ui->lineEdit_WF_b->text(), defaultValue);
    this->wireFrameColors.w = QtUtils::readFloatFromQText(ui->lineEdit_WF_a->text(), defaultValue);
    this->wireFrameLineWidthM = QtUtils::readIntFromQText(ui->lineEdit_WF_w->text(),1);

    this->baseModelColors.x = QtUtils::readFloatFromQText(ui->lineEdit_BM_r->text(), defaultValue);
    this->baseModelColors.y = QtUtils::readFloatFromQText(ui->lineEdit_BM_g->text(), defaultValue);
    this->baseModelColors.z = QtUtils::readFloatFromQText(ui->lineEdit_BM_b->text(), defaultValue);
    this->baseModelColors.w = QtUtils::readFloatFromQText(ui->lineEdit_BM_a->text(), defaultValue);
}