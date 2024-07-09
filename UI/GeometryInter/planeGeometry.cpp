#include "planeGeometry.h"
#include "ui_planeGeometry.h"
#include "Utils/qtutils.h"
#include <iostream>


PlaneGeometryWidget::PlaneGeometryWidget(QWidget * parent):
	QWidget(parent,Qt::Tool|
			Qt::Window |
			Qt::CustomizeWindowHint|
			Qt::WindowMinimizeButtonHint|
			Qt::WindowCloseButtonHint),
	ui(new Ui::PlaneGeometryWidget)
{
    ui->setupUi(this);
    // connect signals
    readConfiguration();
    resetValues();
    connect(ui->pushButton_delete, SIGNAL(clicked()), this, SLOT(callDeleteWidget()));
}

PlaneGeometryWidget::~PlaneGeometryWidget(){
    delete ui;
}

void PlaneGeometryWidget::readConfiguration(){
    normal.x = QtUtils::readFloatFromQText(ui->lineEdit_nx->text(),1.0f);
    normal.y = QtUtils::readFloatFromQText(ui->lineEdit_ny->text(),1.0f);
    normal.z = QtUtils::readFloatFromQText(ui->lineEdit_nz->text(),1.0f);
    
    if(normal == glm::vec3(0.0f)){
        normal = glm::vec3(1.0f);
        ui->lineEdit_nx->setText(QString("1.0"));
        ui->lineEdit_ny->setText(QString("1.0"));
        ui->lineEdit_nz->setText(QString("1.0"));
    }
    
    pos.x = QtUtils::readFloatFromQText(ui->lineEdit_px->text(),0.0f);
    pos.y = QtUtils::readFloatFromQText(ui->lineEdit_py->text(),0.0f);
    pos.z = QtUtils::readFloatFromQText(ui->lineEdit_pz->text(),0.0f);
}

void PlaneGeometryWidget::changeLabelsName(int id){
    this->id = id;
    ui->label_title->setText(QString("Plane N°") + QString::number(id));
}

void PlaneGeometryWidget::resetValues(){
    normal = glm::vec3(0.0f);
    pos = glm::vec3(0.0f);
}

glm::vec3 PlaneGeometryWidget::getNormal(){
    return this->normal;
}

glm::vec3 PlaneGeometryWidget::getPosition(){
    return this->pos;
}

void PlaneGeometryWidget::setTextValues(glm::vec3 new_pos, glm::vec3 new_normal){
    ui->lineEdit_px->setText(QString::number(new_pos.x));
    ui->lineEdit_py->setText(QString::number(new_pos.y));
    ui->lineEdit_pz->setText(QString::number(new_pos.z));

    ui->lineEdit_nx->setText(QString::number(new_normal.x));
    ui->lineEdit_ny->setText(QString::number(new_normal.y));
    ui->lineEdit_nz->setText(QString::number(new_normal.z));
}

void PlaneGeometryWidget::callDeleteWidget(){
    emit deleteWidget(this->id);
}
