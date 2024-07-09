#ifndef SELECTIONSTRATEGY_H
#define SELECTIONSTRATEGY_H
#include <string>
#include <QWidget>
#define SELECTION_STRATEGY_BASE_WEIGHT 5.0f
namespace vis{
class Polyhedron;
class Polygon;
class Vertex;
class Element;
class Edge;
}
class Model;
class Selection;
class RModel;
class SelectionStrategy
{
	public:
		SelectionStrategy(std::string newname);
		virtual ~SelectionStrategy();
		virtual bool selectElement( vis::Polyhedron *, Selection* );
		virtual bool selectElement( vis::Polygon *, Selection* );
		virtual bool selectElement( vis::Vertex *, Selection* );
		virtual bool selectElement( vis::Edge *, Selection* );
		virtual void selectElement( Selection* );
		virtual bool isFullFilled( vis::Polyhedron * v );
		virtual bool isFullFilled( vis::Polygon * m );
		virtual bool isFullFilled( vis::Vertex * m );
		virtual bool isFullFilled( vis::Edge * m );
		virtual bool isFullFilled( Selection* );

		virtual bool selectElementsFrom(Model*, RModel*, Selection*, bool setupPreApplied = false);

		const char* getName() const;
		virtual bool hasConfigWidget();
		virtual void QApplicationInitiated();
		virtual QWidget* getConfigWidget();
		virtual bool applyButtonEnabled();
		virtual bool setup();
		virtual bool setupPreApplying(Selection*, RModel *);
		bool isAppliedOnSelection();
		float getWeight();
	protected:
		bool applyOnSelection;
		float weight;
	private:
		const std::string name;
};


#endif // SELECTIONSTRATEGY_H
