#include "scalarpropertyrenderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Model/Model.h"
#include "Utils/shaderutils.h"
#include "Common/Constants.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "PropertyFieldLoading/scalarfieldlistaddervisitor.h"
#include <iostream>

#define POSITION_ATTRIBUTE 0
#define VERTEX_SCALARPROP 1
#define VERTEX_FLAGS_ATTRIBUTE 2

ScalarPropertyRenderer::ScalarPropertyRenderer():Renderer("Scalar Property Coloring"){

	config = (ScalarPropertyRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+2.0f;
}
ScalarPropertyRenderer::~ScalarPropertyRenderer(){
	if(config)
		delete config;

}
void ScalarPropertyRenderer::glewIsReadyRenderer(){
	//load shaders and initial values
	config = new ScalarPropertyRendererConfig();
	//config->readConfig();

	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/ScalarPropertyRenderer/spr.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("Rendering/Renderers/ScalarPropertyRenderer/spr.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/ScalarPropertyRenderer/spr.frag");


	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);

	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData scalarPropAttr = {VERTEX_SCALARPROP, "VertexScalar"};
	VertexAttributeBindingData flagsPropAttr = {VERTEX_FLAGS_ATTRIBUTE, "VertexFlags"};

	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(scalarPropAttr);
	attributeList.push_back(flagsPropAttr);

	program = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(program == ShaderUtils::FAIL_CREATING_PROGRAM){
		this->ok = false;
	}

}

void ScalarPropertyRenderer::draw(RModel* rmodel,CameraCamaron* camera){
	if(!rmodel || !config->selectedScalarDef)
		return;

	glUseProgram(program);
	ShaderUtils::setUniform(program,"MVP",camera->getMVP());
	ShaderUtils::setUniform(program,"coloring_type",config->coloring_type);
	ShaderUtils::setUniform(program,"inverse_intensity",config->inverse_intensity);
	ShaderUtils::setUniform(program,"min_bound",config->selectedBounds[(PropertyFieldDef*)config->selectedScalarDef.get()][0]);
	ShaderUtils::setUniform(program,"max_bound",config->selectedBounds[(PropertyFieldDef*)config->selectedScalarDef.get()][1]);
	ShaderUtils::setUniform(program, "ElementDrawOption",config->elementDrawnOption);

	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
	glEnableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex flags
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );

	auto currentRModelPropertyFieldDef = rmodel->loadPropertyField(config->selectedScalarDef);

	// Map index 1 to the vertex scalar buffer
	glBindBuffer(GL_ARRAY_BUFFER, currentRModelPropertyFieldDef->getBuffer());
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, currentRModelPropertyFieldDef->getStride(),
						   currentRModelPropertyFieldDef->getOffset());

	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );

	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );
	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_SCALARPROP); // Vertex position
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex flags
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);
}

BaseRendererConfig* ScalarPropertyRenderer::getRendererConfigWidget(){
	return this->config;
}

bool ScalarPropertyRenderer::hasRendererConfigWidget(){
	return true;
}
void ScalarPropertyRenderer::applyConfigChanges(RModel *){
	this->config->readConfig();
}

bool ScalarPropertyRenderer::rmodelChanged(RModel* rmodel){
	if(rmodel->bounds.size()<6
		|| !rmodel->getOriginalModel()
		|| rmodel->getOriginalModel()->getPolygonsCount() == 0)
		return false;

	lastModel = rmodel->getOriginalModel();
	std::vector<std::shared_ptr<ScalarFieldDef>> scalarFields;
	ScalarFieldListAdderVisitor visitor(scalarFields);
	for(std::shared_ptr<PropertyFieldDef> prop : lastModel->getPropertyFieldDefs()) {
		prop->accept(visitor);
	}
	if(scalarFields.size() == 0)
		return false;
	config->setScalarFields(scalarFields);

	return true;
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(ScalarPropertyRenderer);
