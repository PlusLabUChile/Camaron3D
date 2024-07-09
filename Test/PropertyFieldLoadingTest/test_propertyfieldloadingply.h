#ifndef TEST_PROPERTYFIELDLOADINGPLY_H
#define TEST_PROPERTYFIELDLOADINGPLY_H
#include <QTest>

class PropertyFieldLoadingPly;
class UnitCube;
class PropertyFieldLoadingPlyTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void loadDefs();
		void loadDefsBinary();
		void loadPropertiesASCII();
		void loadPropertiesBinary();
		void cleanupTestCase();
	private:
		PropertyFieldLoadingPly* Loader;
		std::string path = "./Test/PropertyFieldLoadingTest/data/PLY/";
	};

#endif