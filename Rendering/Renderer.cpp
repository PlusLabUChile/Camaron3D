#include "Rendering/Renderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Model/Model.h"
#include "Model/Element/Element.h"
Renderer::Renderer(std::string newname):name(newname){
	this->ok = true;
	rendererWeight = RENDERER_WEIGHT_BASE;
}

Renderer::~Renderer(){}
void Renderer::draw(RModel*, CameraCamaron*){}
void Renderer::rendererSelected(RModel*){}
void Renderer::setCamera(CameraCamaron*){}
bool Renderer::rmodelChanged(RModel* rmodel){
	return rmodel->getOriginalModel() &&
			rmodel->getOriginalModel()->getPolygonsCount() != 0;
}
BaseRendererConfig*  Renderer::getRendererConfigWidget(){
	return (BaseRendererConfig*)0;
}

bool  Renderer::hasRendererConfigWidget(){
	return false;
}
const char* Renderer::getRendererName( ) const{
	return this->name.c_str();
}

bool Renderer::isOk(){
	return this->ok;
}
void Renderer::applyConfigChanges(RModel *){}

glm::vec3 Renderer::isDisplayedViewPortCheck(vis::Element* e, CameraCamaron* camera){
	//if check viewport con coords viewport
	glm::vec3 wpos;
	if(camera->isOrthoProjectionInUse())
		wpos = glm::vec3(camera->getMVPVP()*glm::vec4(e->getGeometricCenter(),1.0f));
	else{
		glm::vec4 mvpCoord = camera->getMVP()*glm::vec4(e->getGeometricCenter(),1.0f);
		mvpCoord /= mvpCoord.w;
		wpos = glm::vec3(camera->getViewPortMatrix()*mvpCoord);
	}
	glm::vec2 vportSize = camera->getViewPortSize();
	wpos.y = vportSize.y-wpos.y;
	if(wpos.x<0 ||
			wpos.y<0 ||
			wpos.x>vportSize.x ||
			wpos.y>vportSize.y)
		return glm::vec3(-1.0f,-1.0f,-1.0f);
	return wpos;
}

bool Renderer::isDisplayed(vis::Element*, RModel*){
	return true;
}
void Renderer::glewIsReadyRenderer(){}

float Renderer::getRendererWeight(){
	return rendererWeight;
}

void Renderer::setRendererWeight(float f){
	rendererWeight = f;
}
bool Renderer::isValidAsSecondaryRenderer(){
	return true;
}
