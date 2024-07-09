#ifndef KERNELRENDERER_H
#define KERNELRENDERER_H
#include <GL/glew.h>
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/KernelRenderer/kernelrendererconfig.h"
#include "UI/kernelPop.h"
#include <iostream>
class KernelRendererConfig;
class RModel;
class Model;

class KernelRenderer: public Renderer
{
	public:
		KernelRenderer();
		virtual ~KernelRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*, CameraCamaron*);
		void drawOriginalModel(RModel*);
		BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);
   
	private:
		GLuint theProgram;
		KernelRendererConfig* config;
};
#endif // KERNELRENDERER_H
