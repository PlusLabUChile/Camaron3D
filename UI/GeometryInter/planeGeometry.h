#ifndef PLANEGEOMETRY_H
#define PLANEGEOMETRY_H

#include <QWidget>
#include <glm/glm.hpp>

namespace Ui
{
    class PlaneGeometryWidget ;
} 

class PlaneGeometryWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit PlaneGeometryWidget(QWidget *parent = 0);
        ~PlaneGeometryWidget();
        void readConfiguration();
        void resetValues();
        void changeLabelsName(int);
        glm::vec3 getNormal();
        glm::vec3 getPosition();
        void setTextValues(glm::vec3, glm::vec3);

    public slots:
        void callDeleteWidget();

    signals:
        void deleteWidget(int);

    private:
        Ui::PlaneGeometryWidget *ui;
        int id;
        glm::vec3 normal, pos;
};

#endif // PLANEGEOMETRY_H