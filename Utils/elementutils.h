#ifndef ELEMENTUTILS_H
#define ELEMENTUTILS_H
#include <string>
namespace vis{
class Element;
}
class EvaluationStrategyRegistry;
class Selection;
class ElementUtils
{
	public:
		static std::string getPropertiesDisplay(vis::Element*, Selection*);
	private:
		ElementUtils();
		static EvaluationStrategyRegistry* instance;
};

#endif // ELEMENTUTILS_H
