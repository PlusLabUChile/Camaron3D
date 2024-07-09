#include "kernelPop.h"
#include "ui_kernelPop.h"

KernelPop::KernelPop(QWidget *parent) :
	QWidget(parent,Qt::Tool|
			Qt::Window |
			Qt::CustomizeWindowHint|
			Qt::WindowMinimizeButtonHint|
			Qt::WindowCloseButtonHint),
	ui(new Ui::KernelPop)
{
	ui->setupUi(this);
	connect(ui->pushButton_openKernel, SIGNAL(clicked()), parent, SLOT(openKernelWindow()));

}

KernelPop::~KernelPop(){
    delete ui;
}