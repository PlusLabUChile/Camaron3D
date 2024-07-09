#ifndef KERNELWIDGET_H
#define KERNELWIDGET_H
#include <QWidget>
#include <string.h>
#include "Utils/fileutils.h"

namespace Ui{
    class KernelWidget;
}

class KernelWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit KernelWidget(QWidget *parent = 0);
        ~KernelWidget();
        void setValuesKernelModel(int, int, bool);
        void setModelName(std::string);
        void resetValues();

    private:
        void setStatus(bool);
        Ui::KernelWidget *ui;
        std::string modelName;
        int verticesKernel, kernelPolygons;
        bool statusKernel;
};

#endif // KERNELWIDGET_H
