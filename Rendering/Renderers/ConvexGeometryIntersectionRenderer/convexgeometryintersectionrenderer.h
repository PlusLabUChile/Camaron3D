#ifndef CONVEXGEOMETRYINTERSECTIONRENDERER_H
#define CONVEXGEOMETRYINTERSECTIONRENDERER_H
#include <GL/glew.h>
#include "SelectionStrategies/SelectionStrategy.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionconfigselection.h"
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/sphericconvexgeometry.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometrydefinedbyplanes.h"

class ConvexGeometryIntersectionRenderer:public Renderer, public SelectionStrategy
{
	public:
		ConvexGeometryIntersectionRenderer();
		virtual ~ConvexGeometryIntersectionRenderer();
		//Renderer Overide functions
		virtual void glewIsReadyRenderer();
		void draw(RModel*, CameraCamaron*);
		BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);
		void setCamera(CameraCamaron* camera);
		void updateSphereInputs();

		//Selection Strategy Override Functions
		virtual bool hasConfigWidget();
		virtual BaseRendererConfig* getConfigWidget();
		virtual bool selectElement( vis::Polyhedron *, Selection* );
		virtual bool selectElement( vis::Polygon *, Selection* );
		bool setup();
		virtual bool setupPreApplying(Selection*, RModel *);
		virtual bool isFullFilled( vis::Polygon *);
		virtual bool isFullFilled( vis::Polyhedron *);
		virtual bool isFullFilled( vis::Vertex *);
		
	private:
		void createMainShader();
		void drawWithMainShader(RModel*, CameraCamaron*);
		//Main Shader attributes
		GLuint theProgram;
		glm::mat4 lastMVMatrix;
		void drawWithConvexGeometryShader(CameraCamaron*);
		void setConvexGeometry(RModel *);
		bool createInsideAttributeBufferObject(RModel *);
		void selectNewConvexGeometry(int);

		ConvexGeometryIntersectionRendererConfig* configRenderer;
		ConvexGeometryIntersectionConfigSelection* configSelection;
		GenericConvexGeometry *convexGeometry;
		RModel *rmodel;
		float plane_length;
		CameraCamaron* camera;
		SphericConvexGeometry* SphereGeometry;
		ConvexGeometryDefinedByPlanes* PlanesGeometry;

		//Selection private methods

		bool selectElementAlgorithm( Model*, vis::Element * e, int elementType);
		bool selectElement( vis::Element * e,int elementType,Selection* s );
		bool selectElementFollowModel( vis::Element * e );
		bool selectCPU(Model*, vis::Polygon*);
		bool selectCPU(Model*, vis::Polyhedron*);
		bool selectCPU(vis::Vertex*);
		inline bool gpuSelectionAlgorithmAvailable();
};
bool ConvexGeometryIntersectionRenderer::gpuSelectionAlgorithmAvailable(){
	return false;
}

#endif // CONVEXGEOMETRYINTERSECTIONRENDERER_H
