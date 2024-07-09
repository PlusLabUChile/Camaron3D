#ifndef TEST_PROPERTYFIELDLOADINGELENODE_H
#define TEST_PROPERTYFIELDLOADINGELENODE_H
#include <QTest>

class PropertyFieldLoadingEleNode;
class PropertyFieldLoadingEleNodeTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void loadDefs();
		void loadFloatProperty();
		void cleanupTestCase();
	private:
		PropertyFieldLoadingEleNode* Loader;
		std::string path = "./Test/PropertyFieldLoadingTest/data/EleNode/";
	};

#endif