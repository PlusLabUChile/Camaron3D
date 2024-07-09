#ifndef TRIANGLERADIUSRATIO_H
#define TRIANGLERADIUSRATIO_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"
class TriangleRadiusRatio: public PolygonEvaluation
{
	public:
		TriangleRadiusRatio();
		virtual ~TriangleRadiusRatio();
		void value(Model* model, vis::Polygon* t);
		float getNullValue();
		bool hasDescriptionQWidget();
		QWidget* getEvaluationStrategyDescriptionQWidget();
		void QApplicationInitiatedEv();
	private:
		QWidget* descriptionWidget;
};

#endif // TRIANGLERADIUSRATIO_H
