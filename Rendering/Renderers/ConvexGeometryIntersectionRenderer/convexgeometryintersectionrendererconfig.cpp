#include "convexgeometryintersectionrendererconfig.h"
#include "ui_convexgeometryintersectionrendererconfig.h"
#include "Rendering/Renderers/ModelMainRenderer/mainrendererconfig.h"
#include "Utils/fileutils.h"
#include "Utils/qtutils.h"
#include <QFileDialog>
#include "UI/clickableqlabel.h"
#include <iostream>

ConvexGeometryIntersectionRendererConfig::ConvexGeometryIntersectionRendererConfig(QWidget *parent) :
	BaseRendererConfig(parent),
	//mainRendererConfig(this),
	ui(new Ui::ConvexGeometryIntersectionRendererConfig)
{
	ui->setupUi(this);
	mainRendererConfig = new MainRendererConfig(this);
	ui->widget_mm_wid->layout()->addWidget(mainRendererConfig);
	//connect(ui->pushButton_normals, SIGNAL(clicked()), &this->planeWidget,SLOT(show()));
	readConfig();
	changedFollowModel = true;
	mainRendererConfig->depthPeelingNotAvailable();
	textChangedChangeBool();
	actualIndexTab = 0;
	connect(ui->tabWidget_convexGeom, SIGNAL(tabBarClicked(int)), this, SLOT(changeConvexGeometry(int)));
	connect(ui->pushButton_addplane, SIGNAL(clicked()), this, SLOT(addPlane()));
	
	planesContainer = new QVBoxLayout(this);
	QWidget* container = new QWidget();
	container->setLayout(planesContainer);
	ui->scrollArea->setWidget(container);
	ui->scrollArea->setWidgetResizable(true);
}

ConvexGeometryIntersectionRendererConfig::~ConvexGeometryIntersectionRendererConfig()
{
	delete ui;
}
void ConvexGeometryIntersectionRendererConfig::readConfig(){
	mainRendererConfig->readConfig();
	bool oldFollowModel = followModel;
	followModel = ui->checkBox_move_with_model->isChecked();
	changedFollowModel = followModel != oldFollowModel;
	normalLength = QtUtils::readFloatFromQText(ui->lineEdit_normal_length->text(),
											   1.0f);
	drawNormal = ui->checkBox_draw_normals->isChecked();
	invertIntersection = ui->checkBox_invert->isChecked();

	convexGeometryColor.x = QtUtils::readFloatFromQText(ui->lineEdit_cg_r->text(),0.0f);
	convexGeometryColor.y = QtUtils::readFloatFromQText(ui->lineEdit_cg_g->text(),1.0f);
	convexGeometryColor.z = QtUtils::readFloatFromQText(ui->lineEdit_cg_b->text(),0.0f);
	convexGeometryColor.w = QtUtils::readFloatFromQText(ui->lineEdit_cg_a->text(),0.5f);

	convexGeometryNormalColor.x = QtUtils::readFloatFromQText(ui->lineEdit_cgn_r->text(),1.0f);
	convexGeometryNormalColor.y = QtUtils::readFloatFromQText(ui->lineEdit_cgn_g->text(),1.0f);
	convexGeometryNormalColor.z = QtUtils::readFloatFromQText(ui->lineEdit_cgn_b->text(),0.0f);
	convexGeometryNormalColor.w = QtUtils::readFloatFromQText(ui->lineEdit_cgn_a->text(),1.0f);

	borderCutColor.x = QtUtils::readFloatFromQText(ui->lineEdit_cbc_r->text(),1.0f);
	borderCutColor.y = QtUtils::readFloatFromQText(ui->lineEdit_cbc_g->text(),0.0f);
	borderCutColor.z = QtUtils::readFloatFromQText(ui->lineEdit_cbc_b->text(),0.0f);
	borderCutColor.w = QtUtils::readFloatFromQText(ui->lineEdit_cbc_a->text(),1.0f);

	outsideColor.x = QtUtils::readFloatFromQText(ui->lineEdit_outc_r->text(),1.0f);
	outsideColor.y = QtUtils::readFloatFromQText(ui->lineEdit_outc_g->text(),0.0f);
	outsideColor.z = QtUtils::readFloatFromQText(ui->lineEdit_outc_b->text(),0.0f);
	outsideColor.w = QtUtils::readFloatFromQText(ui->lineEdit_outc_a->text(),1.0f);

	outsideSelectedColor.x = QtUtils::readFloatFromQText(ui->lineEdit_outs_r->text(),1.0f);
	outsideSelectedColor.y = QtUtils::readFloatFromQText(ui->lineEdit_outs_g->text(),0.0f);
	outsideSelectedColor.z = QtUtils::readFloatFromQText(ui->lineEdit_outs_b->text(),0.0f);
	outsideSelectedColor.w = QtUtils::readFloatFromQText(ui->lineEdit_outs_a->text(),1.0f);

	drawOutsideElements = ui->checkBox_drawOutsideElements->isChecked();
	resetPosition = ui->radioButton__reset->isChecked();

	changeConvexGeometry(actualIndexTab);
}

void ConvexGeometryIntersectionRendererConfig::textChangedChangeBool(){
	geometryChanged = true;
}

void ConvexGeometryIntersectionRendererConfig::changeConvexGeometry(int index){
	actualIndexTab = index;
	textChangedChangeBool();
	switch (actualIndexTab)
	{
	case 0:
		centerSphere.x = QtUtils::readFloatFromQText(ui->lineEdit_sphere_cx->text(),0.0f);
		centerSphere.y = QtUtils::readFloatFromQText(ui->lineEdit_sphere_cy->text(),0.0f);
		centerSphere.z = QtUtils::readFloatFromQText(ui->lineEdit_sphere_cz->text(),0.0f);
		radiusSphere = QtUtils::readFloatFromQText(ui->lineEdit_sphere_radius->text(),25.0f);
		nverticesSphere = QtUtils::readIntFromQText(ui->lineEdit_sphere_nvertices->text(),1000);
		break;
	
	case 1:
		if(true){
			valuePlanes.clear();
			for(unsigned int i = 0; i < planes.size(); i++){
				planes[i]->readConfiguration();
				valuePlanes.push_back(new Plane(planes[i]->getPosition(),planes[i]->getNormal()));
			}
		}
		else{
			for(unsigned int i = 0; i < planes.size(); i++){
				planes[i]->readConfiguration();
				valuePlanes[i]->setOrigin(planes[i]->getPosition());
				valuePlanes[i]->setNormal(planes[i]->getNormal());
				valuePlanes[i]->getPointsPlane();
			}
		}
		break;
	default:
		break;
	}
}

void ConvexGeometryIntersectionRendererConfig::updateSphereInputs(){
	ui->lineEdit_sphere_cx->setText(QString::number(centerSphere.x));
	ui->lineEdit_sphere_cy->setText(QString::number(centerSphere.y));
	ui->lineEdit_sphere_cz->setText(QString::number(centerSphere.z));
	ui->lineEdit_sphere_radius->setText(QString::number(radiusSphere));
	textChangedChangeBool();
}

void ConvexGeometryIntersectionRendererConfig::updatePlaneInputs(){
	for(unsigned int id = 0; id < planes.size(); id++){
		planes[id]->setTextValues(valuePlanes[id]->getOrigin(), valuePlanes[id]->getNormal());
	}
	textChangedChangeBool();
}

void ConvexGeometryIntersectionRendererConfig::deleteWidgetByIndex(int id){
	auto dplane = planes[id];
	auto dvplane = valuePlanes[id];
	planes.erase(planes.begin() + id);
	valuePlanes.erase(valuePlanes.begin() + id);

	delete dplane;
	delete dvplane;

	for(unsigned int i = id; i < planes.size(); i++){
		planes[i]->changeLabelsName(i);
	}
}

void ConvexGeometryIntersectionRendererConfig::addPlane(){
	PlaneGeometryWidget* plane = new PlaneGeometryWidget(this);
	connect(plane, SIGNAL(deleteWidget(int)), this, SLOT(deleteWidgetByIndex(int)));
	plane->resetValues();
	plane->changeLabelsName(planes.size());
	planes.push_back(plane);
	plane->setMinimumSize(280,230);
	valuePlanes.push_back(new Plane(glm::vec3(0.0f),glm::vec3(0.0f)));
	ui->scrollArea->widget()->layout()->addWidget(plane);
	textChangedChangeBool();
}
