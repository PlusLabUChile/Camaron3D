#ifndef KERNELLOADINGPROGRESSDIALOG_H
#define KERNELLOADINGPROGRESSDIALOG_H
#include <QWidget>

namespace Ui {
class KernelLoadingProgressDialog;
}

class KernelLoadingProgressDialog : public QWidget
{
		Q_OBJECT

	public:
		explicit KernelLoadingProgressDialog(QWidget *parent = 0);
		~KernelLoadingProgressDialog();
		void setupForNewModel(int nvertices,
							  int npolygons = 0);
		static const int RMODEL_READY = 5;
		static const int BASE_MODE_READY = 1;
		static const int COMPLETED_VERTEX_POLYGON_R = 3;
		static const int NORMALS_CALCULATED = 4;
        static const int COMPLETE_KERNEL_COMPUTE = 2;

	public slots:
		void setupProgressBarForNewModel(int,int);
		void setLoadedInformation(int);
		void setLoadedComputed(int);
		void setLoadedKernel(int, int);
		void stageComplete(int);
		void closeKernelDialog();
        
	private:
		Ui::KernelLoadingProgressDialog *ui;
		void resetValues(int,int);

        void setGettingInformation();
        void setComputeKernel();
        void setKernelModel();

		void updateProgressBar();
		int loadedInformation, loadedKernel, facesComputed;
		int totalVertices,totalPolygons,totalelements;
		int progressVarValue;
		int progressStages;
		int progressVarMax;
};

#endif // MODELLOADINGPROGRESSDIALOG_H
