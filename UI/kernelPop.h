#ifndef KERNELPOP_H
#define KERNELPOP_H
#include <QWidget>
#include <string.h>
#include "Utils/fileutils.h"

namespace Ui{
    class KernelPop;
}

class KernelPop : public QWidget
{
    Q_OBJECT

    public:
        explicit KernelPop(QWidget *parent = 0);
        ~KernelPop();

    private:
        Ui::KernelPop *ui;
};

#endif // KERNELPOP_H
