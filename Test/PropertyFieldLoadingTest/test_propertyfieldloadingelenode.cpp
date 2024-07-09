#include "test_propertyfieldloadingelenode.h"
#include "PropertyFieldLoading/propertyfieldloadingelenode.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Exceptions/modelloadingexception.h"
#include "Model/Element/Vertex.h"
#include "Model/Model.h"
#include <memory>
#include <cstdio>

void PropertyFieldLoadingEleNodeTest::initTestCase(){
	Loader = new PropertyFieldLoadingEleNode();
	}

void PropertyFieldLoadingEleNodeTest::loadDefs(){
	///@note both Tetgen and Triangle models use the same file to store property
	/// fields, no need to test them separately
	std::vector<std::shared_ptr<PropertyFieldDef>> properties = Loader->loadDefs(path + "polygon_mesh.node");

	///@note Properties in this file don't have names and will always contain floats,
	///only check if we are capable of detecting them appropriately.
	QVERIFY(properties.size() == 4);
	for(std::shared_ptr<PropertyFieldDef> prop : properties){
		QVERIFY(prop->getElementsCount() == 8);
		}
	}

void PropertyFieldLoadingEleNodeTest::loadFloatProperty(){
	UnitCube cubeClone(false);
	Model* test_model = cubeClone.polygonmesh;
	std::vector<std::shared_ptr<PropertyFieldDef>> properties = Loader->loadDefs(path + "polygon_mesh.ply");

	///@todo Improve file content test min and max values
	float expected_values[4] = {-0.5f, 0.0f, 0.5f, 1.0f};
	Loader->load(path + "polygon_mesh.ply", (Model*)test_model, properties);

	///@todo Create MeshExtemsion method to compare PropertyField values inside
	/// any mesh type
	for (unsigned int i  = 0; i < properties.size(); i++){
		std::vector<float>& data = properties[i]->getPropertyData();
		for(unsigned j = 0; j < data.size(); j++){
			QCOMPARE(data[j], expected_values[i]);
			}
		}
	}

void PropertyFieldLoadingEleNodeTest::cleanupTestCase(){
	delete Loader;
	}
