#include "kernelrenderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Model/Model.h"
#include "Utils/shaderutils.h"

#define POSITION_ATTRIBUTE 0
#define FLAG_ATTRIBUTE 1

KernelRenderer::KernelRenderer():Renderer("Polyhedron Kernel Renderer")
{
    config = (KernelRendererConfig*)0;
    rendererWeight = RENDERER_WEIGHT_BASE+0.2f;
}

KernelRenderer::~KernelRenderer()
{
    if(config) delete config;
}

void KernelRenderer::glewIsReadyRenderer()
{
    config = new KernelRendererConfig();
    ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
    vertexShaderData.addFile("Rendering/Renderers/KernelRenderer/kr.vert"); 

	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("Rendering/Renderers/KernelRenderer/kr.geom");

    ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
    fragmentShaderData.addFile("Rendering/Renderers/KernelRenderer/kr.frag");

    std::vector<ShaderLoadingData> shaderList;
    shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
    shaderList.push_back(fragmentShaderData);

    std::vector<VertexAttributeBindingData> attributeList;
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
    VertexAttributeBindingData flagAttr = {FLAG_ATTRIBUTE, "VertexFlag"};
	attributeList.push_back(positionAttr);
    attributeList.push_back(flagAttr);

    theProgram = ShaderUtils::CreateProgram(shaderList, attributeList);
    if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM) this->ok = false;
    this->applyConfigChanges();
}


void KernelRenderer::draw(RModel* rmodel, CameraCamaron* camera)
{
    if(!rmodel->isKernelComputed()) return;

    glUseProgram(theProgram);
 	ShaderUtils::setUniform(theProgram, "MVP",camera->getMVP());
	ShaderUtils::setUniform(theProgram, "ViewportMatrix",camera->getViewPortMatrix());
	ShaderUtils::setUniform(theProgram, "Line.Width",config->wireFrameLineWidthM);
	ShaderUtils::setUniform(theProgram, "Line.Color",config->wireFrameColors);
	ShaderUtils::setUniform(theProgram, "Line.isDrawn",1);
	ShaderUtils::setUniform(theProgram, "ModelBaseColor",config->baseModelColors);

    glEnableVertexAttribArray(POSITION_ATTRIBUTE);
	glEnableVertexAttribArray(FLAG_ATTRIBUTE);

	if(rmodel->kernel_positionDataBufferObject == RModel::NULL_BUFFER){
        drawOriginalModel(rmodel);
        return;
    }
	else{
		glBindBuffer(GL_ARRAY_BUFFER, rmodel->kernel_positionDataBufferObject);
		glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_DOUBLE, GL_FALSE, 0,
							(GLubyte *)NULL );
		glBindBuffer(GL_ARRAY_BUFFER, rmodel->kernel_vertexFlagsDataBufferObject);
		glVertexAttribIPointer( FLAG_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
								(GLubyte *)NULL );
		glDrawArrays(GL_TRIANGLES, 0, rmodel->kernel_vertexFlagsAttribute.size() );
	}
	glDisableVertexAttribArray(POSITION_ATTRIBUTE);
	glDisableVertexAttribArray(FLAG_ATTRIBUTE);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);
}

void KernelRenderer::drawOriginalModel(RModel* rmodel){
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( FLAG_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
    glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );
}

BaseRendererConfig* KernelRenderer::getRendererConfigWidget(){
    return this->config;
}

bool KernelRenderer::hasRendererConfigWidget()
{
    return true;
}

void KernelRenderer::applyConfigChanges(RModel *)
{
    this->config->readConfig();
}

bool KernelRenderer::rmodelChanged(RModel *rmodel)
{
	if(!rmodel || !rmodel->getOriginalModel())
		return false;
	return true;
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(KernelRenderer);
