#include "kernelloadingprogressdialog.h"
#include "ui_kernelloadingprogressdialog.h"
#include "Common/Constants.h"
#include <iostream>
KernelLoadingProgressDialog::KernelLoadingProgressDialog(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint),
	ui(new Ui::KernelLoadingProgressDialog)
{
	ui->setupUi(this);
}

KernelLoadingProgressDialog::~KernelLoadingProgressDialog()
{
	delete ui;
}
void KernelLoadingProgressDialog::setupForNewModel(int nvertices,
												  int npolygons){
	ui->progressBar->setEnabled(true);
	ui->pushButton->setVisible(false);
	resetValues(nvertices,npolygons);
	ui->progressBar->setValue(0);
	progressStages = 0;
	progressVarMax = (int)((nvertices+2*npolygons)*1.6f);
	ui->progressBar->setMaximum(progressVarMax);
	updateProgressBar();
	setGettingInformation();
	setComputeKernel();
	setKernelModel();
	}

void KernelLoadingProgressDialog::resetValues(int nv, int np){
    loadedInformation = 0;
	loadedKernel = 0;
    facesComputed = 0;
	totalPolygons = np==0 ? 1 : np;
    totalVertices = nv==0 ? 1 : nv;
	totalelements = 0;
}


void KernelLoadingProgressDialog::setGettingInformation(){
    ui->label_loaded_info->setText(QString::number(loadedInformation));
    ui->label_total_info->setText(QString::number(totalVertices + totalPolygons));
    ui->label_percent_info->setText(QString::number(((float)loadedInformation) / ((float)totalVertices + totalPolygons)*100.f)+QString("%"));
}

void KernelLoadingProgressDialog::setComputeKernel(){
    ui->label_loaded_compute->setText(QString::number(facesComputed));
    ui->label_total_compute->setText(QString::number(totalPolygons));
    ui->label_percent_compute->setText(QString::number(((float)facesComputed) / ((float)totalPolygons)*100.f)+QString("%"));
}

void KernelLoadingProgressDialog::setKernelModel(){
    ui->label_loaded_kernel->setText(QString::number(loadedKernel));
    ui->label_total_kernel->setText(QString::number(totalelements));
	ui->label_percent_kernel->setText(QString::number(((float)loadedKernel) / ((float)totalelements)*100.f)+QString("%"));
}

void KernelLoadingProgressDialog::setLoadedInformation(int n){
	if(loadedInformation!=n){
		loadedInformation = n;
		setGettingInformation();
		updateProgressBar();
	}
}

void KernelLoadingProgressDialog::setLoadedComputed(int n){
	if(facesComputed!=n){
		facesComputed = n;
		setComputeKernel();
		updateProgressBar();
	}
}

void KernelLoadingProgressDialog::setLoadedKernel(int n, int t){
	if(loadedKernel!=n){
		loadedKernel = n;
		totalelements = t;
		setKernelModel();
		updateProgressBar();
	}
}

void KernelLoadingProgressDialog::updateProgressBar(){
	progressVarValue = loadedInformation+facesComputed+
					   progressStages;
	ui->progressBar->setValue(progressVarValue);
}

void KernelLoadingProgressDialog::setupProgressBarForNewModel(int v,int p){
	setupForNewModel(v,p);
}
void KernelLoadingProgressDialog::stageComplete(int d){
	QString message("");
	switch(d){
		case RMODEL_READY:{
			message = QString("Rendering model complete...");break;
		}
		case COMPLETED_VERTEX_POLYGON_R:{
			message = QString("Completed vertex-polygon neighboor pointers...");break;
		}
		case NORMALS_CALCULATED:{
			message = QString("Normals calculated...");break;
		}
		case BASE_MODE_READY:{
			message = QString("Base model loaded...");break;
		}
	}
	progressStages+=0.1f*(totalVertices+totalPolygons+facesComputed);
	updateProgressBar();
}

void KernelLoadingProgressDialog::closeKernelDialog(){
	close();
}