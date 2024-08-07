#ifndef SCALARPROPERTYRENDERERCONFIG_H
#define SCALARPROPERTYRENDERERCONFIG_H

#include <GL/glew.h>
#include <QWidget>
#include "Rendering/Renderers/baserendererconfig.h"

template <typename T> class RModelPropertyFieldDef;
namespace Ui {
class ScalarPropertyRendererConfig;
}
class Model;
class RModel;
class PropertyFieldDef;
class ScalarPropertyRendererConfig : public BaseRendererConfig//, public ModelVisitor
{
		Q_OBJECT
		
	public:
		explicit ScalarPropertyRendererConfig(QWidget *parent = 0);
		virtual ~ScalarPropertyRendererConfig();
		void readConfig();
		bool setBoundsFromModel();

		//std::shared_ptr<RModelPropertyFieldDef<ScalarFieldDef>> selectedScalarRModelDef;
		std::shared_ptr<ScalarFieldDef> selectedScalarDef;
		int coloring_type;
		int inverse_intensity;
		void setScalarFields(std::vector<std::shared_ptr<ScalarFieldDef>> &scalarFields);
		//virtual void visit(PolygonMesh* model);
		//virtual void visit(PolyhedronMesh* model);
		std::map<PropertyFieldDef*, std::vector<float>> selectedBounds;
		int elementDrawnOption;

		static const int DRAW_ALL = 0;
		static const int DRAW_ONLY_SELECTED = 3;
		static const int DRAW_ONLY_UNSELECTED = 4;

	private:
		Ui::ScalarPropertyRendererConfig *ui;
//		void loadScalarDefs();
//		void onNewModelLoaded();
		std::vector<std::shared_ptr<ScalarFieldDef>> scalarFields;
		//std::map<int,std::shared_ptr<ScalarFieldDef>> scalarDefIdsMap;
		RModel* rmodel;
		Model* model;
		Model* lastModel;
	private slots:
		void loadBoundsFromModel();
		void changedProperty(int index);

};

#endif // SCALARPROPERTYRENDERERCONFIG_H
