#ifndef MODELGENERALSTATICSCOLLECTOR_H
#define MODELGENERALSTATICSCOLLECTOR_H
#include <map>
#include <string>
#include <vector>
#include <sstream>

/***
* @brief Struct representing a text subcategory
*
* This struct is rendered by the GUI with a title in bold letters (itemTitle),
* followed by a list of data entries, contained internally in the stats map.
*/

struct ModelStaticsItem{
	public:
		ModelStaticsItem(std::string s):itemTitle(s){}
		std::string itemTitle;
		std::map<std::string, std::string> stats;
		void addStat(std::string s,std::string f){
			stats.insert( std::make_pair( s, f ) );
		}
		void addStat(std::string s,float f){
			std::stringstream ss;
			ss << f;
			stats.insert( std::make_pair( s, ss.str() ) );
		}
		void addStat(std::string s,int f){
			std::stringstream ss;
			ss << f;
			stats.insert( std::make_pair( s, ss.str() ) );
		}
};
class Model;
namespace vis{
	class Vertex;
	class Polygon;
	class Polyhedron;
	}

/***
* @brief Compiles information shown by the "Model General Statistics" sub-menu
*
* The class recovers data from the Model object currently being displayed,
* using a ModelStaticsItem for each subcategory. This class resolves the model
* sub-type through the double dispatch interface in Model.h (fillModelStatsDD)
*/
class ModelGeneralStaticsCollector
{
	public:
		ModelGeneralStaticsCollector();
		void fillModelStatics(Model*);
		std::vector<ModelStaticsItem>& getModelStatics();
		ModelStaticsItem* findItem(std::string title);
		void clean();
	private:
		std::vector<ModelStaticsItem> modelStatics;

		void displayModelBoundaries(std::vector<float>& bounds);
		void fillVertexStatics(std::vector<vis::Vertex>& vertices);
		void fillPolygonsStatics(Model*);
		void fillPolyhedronsStatics(Model*);
};

#endif // MODELGENERALSTATICSCOLLECTOR_H
