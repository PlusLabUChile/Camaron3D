#ifndef PROPERTYFIELDDEFVISITOR
#define PROPERTYFIELDDEFVISITOR
#include <memory>

class ScalarFieldDef;

class PropertyFieldDefVisitor
{
	public:
		virtual void visit(std::shared_ptr<ScalarFieldDef>) {}
};

#endif // PROPERTYFIELDDEFVISITOR

