#include "test_propertyfieldloadingply.h"
#include "PropertyFieldLoading/propertyfieldloadingply.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Exceptions/modelloadingexception.h"
#include "Model/Element/Vertex.h"
#include "Model/Model.h"
#include <memory>
#include <cstdio>
#include "Exceptions/modelloadingexception.h"

void PropertyFieldLoadingPlyTest::initTestCase(){
	Loader = new PropertyFieldLoadingPly();
	}

/***
* @brief Attempt to load vertex properties of each supported type
*
* Test should return the types that failed to be identified by the regular expressions
* of the tested class
*/
void PropertyFieldLoadingPlyTest::loadDefs(){
	std::vector<std::string> expected_definitions = {"character", "unsigned_character",
													 "short_integer", "unsigned_short_integer",
													 "integer", "unsigned_integer",
													 "single_precision_float",
													 "double_precision_float"};
	std::vector<std::shared_ptr<PropertyFieldDef>> properties = Loader->loadDefs(path + "ascii.ply");
	unsigned int errors = 0;
	std::string warning_message = "";
	for (unsigned int i = 0; i < expected_definitions.size(); i++){
		std::shared_ptr<PropertyFieldDef> current_property = properties[i];
		std::string prop_name = current_property->getName();
		if (prop_name.compare(expected_definitions[i])){
			errors += 1;
			warning_message = "[WARN] Unable to load definition for " + expected_definitions[i] + " type";
			QWARN(warning_message.c_str());
			}
		}
	if (errors){
		QFAIL("PropertyField class failed to load expected definitions");
		}
	}

void PropertyFieldLoadingPlyTest::loadDefsBinary(){
	/***
	* @note In the current implementation, binary data is currently unsupported,
	* still the class should throw an exception in this case.
	* This is not happening either.
	* @todo Implement this functionality properly
	*/
	std::vector<std::shared_ptr<PropertyFieldDef>> properties;
	QVERIFY_EXCEPTION_THROWN(properties = Loader->loadDefs(path + "binary_little.ply"), ExceptionMessage);

	QVERIFY_EXCEPTION_THROWN(properties = Loader->loadDefs(path + "binary_big.ply"), ExceptionMessage);

	Q_UNUSED(properties);
	}

void PropertyFieldLoadingPlyTest::loadPropertiesASCII(){
	/***
	* For this case, numbers are read as strings, which are later converted to
	* float values. Due to this there are no constraints associated with the
	* intended data type size.
	*/
	UnitCube cubeClone(false);
	Model* test_model = cubeClone.polygonmesh;
	std::vector<std::shared_ptr<PropertyFieldDef>> properties = Loader->loadDefs(path + "ascii.ply");

	Loader->load(path + "ascii.ply", test_model, properties);

	///@todo Improve data values for edge case testing
	float expected_values[8] = {5.0f, 25.0f, 250.0f, 500.0f, 1000.0f, 2500.0f, 0.5f, 25.5f};
	///@todo Create MeshExtemsion method to compare PropertyField values inside
	///any mesh type
	
	for (unsigned int i  = 0; i < properties.size(); i++){
		std::vector<float>& data = properties[i]->getPropertyData();
		for(unsigned j = 0; j < data.size(); j++){
			QCOMPARE(data[j], expected_values[i]);
			}
		}
	}

void PropertyFieldLoadingPlyTest::loadPropertiesBinary(){
	/***
	* In the case of binary files, data type size is relevant, attempting to read
    * everything as a float will generate garbage data when reading any other
    * property field. Note that data should still be converted to floats after
	* reading each field.
	*/

	QSKIP("Unimplemented feature");
	}

void PropertyFieldLoadingPlyTest::cleanupTestCase(){
	delete Loader;
	}
