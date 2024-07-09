#include "kernelWidget.h"
#include "ui_kernelWidget.h"
#include "Common/Constants.h"
#include <iostream>

KernelWidget::KernelWidget(QWidget *parent) :
	QWidget(parent,Qt::Tool|
			Qt::Window |
			Qt::CustomizeWindowHint|
			Qt::WindowMinimizeButtonHint|
			Qt::WindowCloseButtonHint),
	ui(new Ui::KernelWidget)
{
	ui->setupUi(this);
    connect(ui->calculateKernel, SIGNAL(clicked()), parent, SLOT(callCalculateKernel()));
    connect(ui->exportKernel, SIGNAL(clicked()), parent, SLOT(exportKernelModel()));
}

KernelWidget::~KernelWidget(){
    delete ui;
}

void KernelWidget::resetValues(){
    setModelName("");
    setValuesKernelModel(0, 0, false);
}

void KernelWidget::setModelName(std::string name){
    modelName = name;
    ui->label_name_model->setText(QString::fromStdString(FileUtils::getFileNameWithoutPath(modelName)));
}

void KernelWidget::setValuesKernelModel(int vcount, int pcount, bool status){
    verticesKernel = vcount;
    kernelPolygons = pcount;
    statusKernel = status;
    ui->label_count_vertices->setText(QString::number(verticesKernel));
    ui->label_count_polygons->setText(QString::number(kernelPolygons));
    ui->calculateKernel->setEnabled(!status);
    ui->exportKernel->setEnabled(status);
    setStatus(status);
}

void KernelWidget::setStatus(bool status){
    if(status){
        if (kernelPolygons != 0){
            ui->label_status->setStyleSheet("QLabel { background-color : green; color : white; }");
            ui->label_status->setText(QString("Calculated"));
        }
        else{
            ui->label_status->setStyleSheet("QLabel { background-color : yellow; color : black; }");
            ui->label_status->setText(QString("Not existed"));
        }
    }
    else{
        ui->label_status->setStyleSheet("QLabel { background-color : red; color : white; }");
        ui->label_status->setText(QString("Not calculated"));    
    }
}

