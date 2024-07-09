#ifndef RENDERER_H
#define RENDERER_H
#include <string>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"
#include "UI/Camera/Camera.h"

#define RENDERER_WEIGHT_BASE 50.0f
namespace vis{
class Element;
}
class RModel;
class Renderer{
	public:
		Renderer(std::string newname);
		virtual ~Renderer();
		virtual void glewIsReadyRenderer();
		virtual void draw(RModel*, CameraCamaron*);
		virtual void rendererSelected(RModel* rmodel);
		virtual bool rmodelChanged(RModel* rmodel);
		virtual void setCamera(CameraCamaron*);
		virtual BaseRendererConfig* getRendererConfigWidget();
		virtual bool hasRendererConfigWidget();
		virtual void applyConfigChanges(RModel*);
		const char* getRendererName() const;
		bool isOk();
		float getRendererWeight();
		void setRendererWeight(float);
		//is Displayed
		virtual bool isValidAsSecondaryRenderer();
		virtual bool isDisplayed(vis::Element*, RModel*);
		virtual glm::vec3 isDisplayedViewPortCheck(vis::Element*, CameraCamaron*);
	protected:
		bool ok;
		float rendererWeight;
	private:
		const std::string name;
};

#endif // RENDERER_H
