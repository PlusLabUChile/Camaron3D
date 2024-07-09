#ifndef CONVEXGEOMETRYINTERSECTIONRENDERERCONFIG_H
#define CONVEXGEOMETRYINTERSECTIONRENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"
#include "UI/GeometryInter/planeGeometry.h"
#include <QVBoxLayout>
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/plane.h"


namespace Ui {
class ConvexGeometryIntersectionRendererConfig;
}
class MainRendererConfig;
class ConvexGeometryIntersectionRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit ConvexGeometryIntersectionRendererConfig(QWidget *parent = 0);
		~ConvexGeometryIntersectionRendererConfig();
		void readConfig();
		MainRendererConfig* mainRendererConfig;

		std::string geometryText;
		bool geometryChanged;
		bool drawNormal;
		bool followModel = true; ///@todo: Determine correct initial value
		bool changedFollowModel;
		float normalLength;
		glm::vec4 convexGeometryColor;
		glm::vec4 convexGeometryNormalColor;
		glm::vec4 borderCutColor;
		glm::vec4 outsideColor;
		glm::vec4 outsideSelectedColor;
		bool invertIntersection;
		bool resetPosition;
		bool drawOutsideElements;
		int actualIndexTab;

		//Sphere values
		glm::vec3 centerSphere;
		float radiusSphere;
		int nverticesSphere;

		//Planes values;
		std::vector<Plane*> valuePlanes;

		void updateSphereInputs();
		void updatePlaneInputs();

	public slots:
		void textChangedChangeBool();
		void changeConvexGeometry(int);
		void addPlane();
		void deleteWidgetByIndex(int);
		
	private:
		Ui::ConvexGeometryIntersectionRendererConfig *ui;
		std::vector<PlaneGeometryWidget*> planes;
		QVBoxLayout* planesContainer;
		

};

#endif // CONVEXGEOMETRYINTERSECTIONRENDERERCONFIG_H
