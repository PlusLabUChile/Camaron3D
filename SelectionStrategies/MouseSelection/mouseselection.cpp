#include "mouseselection.h"
#include "Utils/shaderutils.h"
#include "Utils/framebufferobjecthandle.h"
#include "SelectionStrategies/Selection.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Rendering/RModel/rmodel.h"
#include "Common/Constants.h"
#include "Utils/PolygonUtils.h"
#include "Utils/polyhedronutils.h"
#include "Utils/VertexUtils.h"
#define POSITION_ATTRIBUTE 0
#define VERTEX_FLAGS_ATTRIBUTE 1
#define IDS_ATTRIBUTE 2
#define NORMAL_ATTRIBUTE 3
MouseSelection::MouseSelection():
	SelectionStrategy("Mouse Selection")
{
	fboHandle = (FrameBufferObjectHandle*)0;
	createComputeShader();
	RENDER_TEXTURE_SHADER_SELECTION_AVAILABLE = createFBOShader();
	//RENDER_TEXTURE_SHADER_SELECTION_AVAILABLE = false;
	weight = SELECTION_STRATEGY_BASE_WEIGHT-1.0f;
	memBuffer = (unsigned int*)0;
	memBufferSize = 0;
	renderBufferSideSizes.push_back(100);
	renderBufferSideSizes.push_back(1000);
	renderBufferSideSizes.push_back(2000);
	renderBufferSideSizes.push_back(4000);
	renderBufferSideSizes.push_back(8000);
	resizeBuffer(renderBufferSideSizes[0]);
}

MouseSelection::~MouseSelection(){
	clearBuffer();
}

bool MouseSelection::createComputeShader(){
	/*theProgram = 0;
	ShaderLoadingData computeShader(GL_COMPUTE_SHADER);
	computeShader.addFile("SelectionStrategies/MouseSelection/cs.vert");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(computeShader);

	std::vector<VertexAttributeBindingData> attributeList;
	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		return false;
	COMPUTE_SHADER_AVAILABLE_SELECTION_AVAILABLE = true;
	return true;*/
	return false;
}
bool MouseSelection::selectElement( vis::Polygon *e, Selection* s){
	return selectElement(e,vis::CONSTANTS::POLYGON,s);
}
bool MouseSelection::selectElement( vis::Vertex *e, Selection* s){
	return selectElement(e,vis::CONSTANTS::VERTEX,s);

}
bool MouseSelection::selectElement( vis::Polyhedron *e, Selection* s){
	return selectElement(e,vis::CONSTANTS::POLYHEDRON,s);
}
bool MouseSelection::selectElement( vis::Element * e, int elementType, Selection* s){
	if(config.NEW_SELECTION == config.selectionType){
		if(selectElementAlgorithm(s->getRModel()->getOriginalModel(),e, elementType)){
			s->addSelectedElement(e);
			return true;
		}
		if(e->isSelected())
			s->removeSelectedElement(s->getRModel()->getOriginalModel(),e);
		return false;
	}
	if(config.JOIN_SELECTION == config.selectionType){
		if(e->isSelected())
			return true;
		if(selectElementAlgorithm(s->getRModel()->getOriginalModel(),e, elementType)){
			s->addSelectedElement(e);
			return true;
		}
		return false;
	}
	if(config.INTERSECT_SELECTION == config.selectionType){
		if(!e->isSelected())
			return false;
		if(selectElementAlgorithm(s->getRModel()->getOriginalModel(),e, elementType))
			return true;
		if(elementType==vis::CONSTANTS::POLYHEDRON)
			s->removeSelectedElement(s->getRModel()->getOriginalModel(),(vis::Polyhedron*)e);
		else
			s->removeSelectedElement(s->getRModel()->getOriginalModel(),e);
		return false;
	}
	if(config.INVERT_SELECTION == config.selectionType){
		if(selectElementAlgorithm(s->getRModel()->getOriginalModel(),e, elementType)){
			if(e->isSelected()){
				if(elementType==vis::CONSTANTS::POLYHEDRON)
					s->removeSelectedElement(s->getRModel()->getOriginalModel(),(vis::Polyhedron*)e);
				else
					s->removeSelectedElement(s->getRModel()->getOriginalModel(),e);
				return false;
			}
			s->addSelectedElement(e);
			return true;
		}
		return e->isSelected();
	}
	return false;
}
bool MouseSelection::selectElementAlgorithm( Model* model, vis::Element * e, int elementType){
	if(elementType == vis::CONSTANTS::POLYGON){
		if(RENDER_TEXTURE_SHADER_SELECTION_AVAILABLE && fboHandle)
			return selectFBO((vis::Polygon*)e);
		return selectCPU(model, (vis::Polygon*)e);
	}
	if(elementType == vis::CONSTANTS::POLYHEDRON){
		if(RENDER_TEXTURE_SHADER_SELECTION_AVAILABLE && fboHandle)
			return selectFBO(model, (vis::Polyhedron*)e);
		return selectCPU(model, (vis::Polyhedron*)e);
	}
	if(elementType == vis::CONSTANTS::VERTEX){
		if(RENDER_TEXTURE_SHADER_SELECTION_AVAILABLE && fboHandle)
			return selectFBO((vis::Vertex*)e);
		return selectCPU(model, (vis::Vertex*)e);
	}
	return false;
}

bool MouseSelection::isFullFilled( vis::Vertex * ){
	return config.elementsType == vis::CONSTANTS::VERTEX;
}

bool MouseSelection::isFullFilled( vis::Polygon * ){
	return config.elementsType == vis::CONSTANTS::POLYGON;
}

bool MouseSelection::isFullFilled( vis::Polyhedron * ){
	return config.elementsType == vis::CONSTANTS::POLYHEDRON;
}

bool MouseSelection::isFullFilled( Selection * s){
	config.elementsType = s->getSelectionType();
	return true;
}

bool MouseSelection::hasFaceFacingCamera(Model* model, vis::Polyhedron *p){
	std::vector<unsigned int>& polygonsIDS = model->getElementsRelations()->getPolygonsPolyhedronsById(p->getId());
	// std::vector<vis::Polygon*>& polygons = p->getPolyhedronPolygons();
	for( unsigned int polygonID : polygonsIDS ){
		vis::Polygon* polygon = &model->getPolygons()[polygonID];
		if(PolygonUtils::isPolygonAtSurface(model, polygon))
			if(glm::dot(normalMatrix*polygon ->getNormal(),
						cameraPosition) < 0.0f)
				return true;
	}
	return false;
}
vis::Polygon* MouseSelection::getFaceFacingCamera(Model* model, vis::Polyhedron *p){
	std::vector<unsigned int>& polygonsIDS = model->getElementsRelations()->getPolygonsPolyhedronsById(p->getId());
	// std::vector<vis::Polygon*>& polygons = p->getPolyhedronPolygons();
	for( unsigned int polygonID : polygonsIDS ){
		vis::Polygon* polygon = &model->getPolygons()[polygonID];
		if(PolygonUtils::isPolygonAtSurface(model, polygon))
			if(glm::dot(normalMatrix*polygon ->getNormal(),
						cameraPosition) < 0.0f)
				return polygon ;
	}
	return (vis::Polygon*)0;
}
//CPU SELECTION
bool MouseSelection::selectCPU( Model* model, vis::Polyhedron *p){
	if(config.onlySurface){
		if(!PolyhedronUtils::isPolyhedronAtSurface(model, p))
			return false;
		vis::Polygon* facingCamera = getFaceFacingCamera(model, p);
		if(config.onlyFront){
			if(!facingCamera)
				return false;
			return selectCPU(model, facingCamera);
		}
		if(config.onlyBack && facingCamera)
			return false;
	}
	else if(config.onlyInterior && PolyhedronUtils::isPolyhedronAtSurface(model, p) )
		return false;

	std::vector<unsigned int>& polygonsIDS = model->getElementsRelations()->getPolygonsPolyhedronsById(p->getId());
	if(rectSelection){
		for(unsigned int pID : polygonsIDS ){
			vis::Polygon* polygon = &model->getPolygons()[pID];
			if(!selectCPU(model, polygon))
				return false;//there is one unselected vertex
		}
		return true;
	}
	for(unsigned int pID : polygonsIDS ){
		vis::Polygon* polygon = &model->getPolygons()[pID];
		if(pointInsidePolygon(model, polygon))
			return true;//there is one unselected vertex
	}
	return false;
}
bool MouseSelection::selectCPU( Model* model, vis::Polygon * p){
	if(config.onlySurface && config.elementsType == vis::CONSTANTS::POLYGON){
		if(!PolygonUtils::isPolygonAtSurface(model, p))
			return false;
		if(config.onlyFront &&
				glm::dot(normalMatrix*p->getNormal(),
						 cameraPosition) > 0.0f)
			return false;
		if(config.onlyBack &&
				glm::dot(normalMatrix*p->getNormal(),
						 cameraPosition) < 0.0f)
			return false;//*/
	}
	else if(config.onlyInterior && config.elementsType == vis::CONSTANTS::POLYGON && PolygonUtils::isPolygonAtSurface(model, p) )
		return false;

	if(rectSelection){
		std::vector<unsigned int>& vertIDS = model->getElementsRelations()->getVertexPolygonsById(p->getId());
		// std::vector<vis::Vertex*>& vertices = p->getVertices();
		for( unsigned int vertID : vertIDS ){
			vis::Vertex* vertex = &model->getVertices()[vertID];
			if(!selectCPU(model, vertex))
				return false;//there is one unselected vertex
		}
		return true;
	}
	return pointInsidePolygon(model, p);
}
bool MouseSelection::selectCPU( Model* model, vis::Vertex * v){
	//se podria chequear con la normal si la cara ta apuntando pa atras
	if(config.onlySurface && config.elementsType == vis::CONSTANTS::VERTEX && !VertexUtils::isVertexAtSurface(model, v))
		return false;
	glm::vec2 viewPortCoords = getViewPortCoords(v->getCoords());
	if(rectSelection){
		if(viewPortCoords.x>=start.x &&
				viewPortCoords.y>=start.y &&
				viewPortCoords.x<=end.x &&
				viewPortCoords.y<=end.y)
			return true;
		return false;
	}
	if(glm::distance(viewPortCoords,start)<config.pixelTolerance)
		return true;
	return false;
}
//FBO SELECTION
bool MouseSelection::selectFBO( Model* model, vis::Polyhedron *p){
	if(config.onlySurface){
		if(!PolyhedronUtils::isPolyhedronAtSurface(model, p))
			return false;
		vis::Polygon* facingCamera = getFaceFacingCamera(model, p);
		if(config.onlyFront){
			if(!facingCamera)
				return false;
			return selectFBO(facingCamera);
		}
		if(config.onlyBack && facingCamera)
			return false;
	}
	else if(config.onlyInterior && PolyhedronUtils::isPolyhedronAtSurface(model, p) )
		return false;

	std::vector<unsigned int>& polygonsIDS = model->getElementsRelations()->getPolygonsPolyhedronsById(p->getId());
	if(rectSelection){
		for(unsigned int pID : polygonsIDS ){
			vis::Polygon* polygon = &model->getPolygons()[pID];
			if(!selectFBO(polygon))
				return false;//there is one unselected vertex
		}
		return true;
	}
	for(unsigned int pID : polygonsIDS ){
		vis::Polygon* polygon = &model->getPolygons()[pID];
		if(selectFBO(polygon))
			return true;//there is one unselected vertex
	}
	return false;
}
bool MouseSelection::selectFBO( vis::Polygon * p){
	std::vector<int>& rmodelPos = p->getRmodelPositions();
	typedef std::vector<int>::size_type st;
	if(rectSelection){
		for(st i = 0;i<rmodelPos.size();i+=3){
			if(!isElementSelected(rmodelPos[i]))
				return false;
		}
		return true;
	}
	else{
		for(st i = 0;i<rmodelPos.size();i+=3){
			if(isElementSelected(rmodelPos[i]))
				return true;
		}
		return false;
	}
}
bool MouseSelection::selectFBO( vis::Vertex * v){
	std::vector<int>& rmodelPos = v->getRmodelPositions();
	for( int position : rmodelPos )
		if(isElementSelected(position))
			return true;
	return false;
}

void MouseSelection::setRectCorners(glm::vec2 s, glm::vec2 e){
	start = glm::vec2(std::min(s.x,e.x),std::min(s.y,e.y));
	end = glm::vec2(std::max(s.x,e.x),std::max(s.y,e.y));
	rectSelection = true;
}

void MouseSelection::setPointSelection(glm::vec2 s){
	start = s;
	rectSelection = false;
}
void MouseSelection::setMVPVP(glm::mat4 m){
	mvpvp = m;
}
void MouseSelection::setVP(glm::mat4 m){
	vp = m;
}
void MouseSelection::setMVP(glm::mat4 m){
	mvp = m;
}

void MouseSelection::setCamera(CameraCamaron* cam){
	camera = cam;
}

void MouseSelection::setViewPortSize(glm::vec2 size){
	viewPortSize = size;
}
//weak - not robust, only for convex polygon with no holes
bool MouseSelection::pointInsidePolygon(Model* model, vis::Polygon *p){
	std::vector<unsigned int>& verticesIDS = model->getElementsRelations()->getVertexPolygonsById(p->getId());
	if(verticesIDS.size() <= 2u)
		return false;
	typedef std::vector<vis::Vertex*>::size_type it_type;
	bool left = true;
	bool right = true;
	glm::vec2 from = getViewPortCoords(model->getVertices()[verticesIDS[0]].getCoords());
	for(it_type i = 1; i <= verticesIDS.size();i++){
		glm::vec2 to = getViewPortCoords(model->getVertices()[verticesIDS[i%verticesIDS.size()]].getCoords());
		//check
		float calc = (start.y - from.y)*(to.x - from.x) - (start.x - from.x)*(to.y - from.y);
		left = left && calc > 0;
		right = right && calc < 0;
		from = to;
	}
	return left || right;
}
bool MouseSelection::applyButtonEnabled(){
	return false;
}
bool MouseSelection::hasConfigWidget(){
	return true;
}

QWidget* MouseSelection::getConfigWidget(){
	return &config;
}
bool MouseSelection::setup(){
	config.readConfig();
	applyOnSelection = config.selectionType==MouseSelectionConfig::INTERSECT_SELECTION;
	return true;
}
void MouseSelection::setNormalMatrix(glm::mat3 n){
	normalMatrix = n;
}
#include "Utils/crosstimer.h"
#include <iostream>
bool MouseSelection::setupPreApplying(Selection* sel, RModel* rmodel){
	// cameraPosition = rmodel->getCameraPosition();
	if(config.selectionType == MouseSelectionConfig::NEW_SELECTION &&
			(!rectSelection ||
			config.elementsType != sel->getSelectionType())){
		sel->clearSelectedElements();
		sel->setSelectionType(config.elementsType);
	}else{
		if(sel->getSelectedElements().empty())
			sel->setSelectionType(config.elementsType);
		if(sel->getSelectionType()!= config.elementsType)
			return false;
	}
	if(RENDER_TEXTURE_SHADER_SELECTION_AVAILABLE){
		CrossTimer cr;
		runRenderSelectionToTextureShader(sel,rmodel);
		std::cout << "Mouse Selection Shader: " << cr.getTranscurredSeconds()<<std::endl;
	}
	return true;
}
#include <fstream>
#include "Utils/openglutils.h"
void MouseSelection::getSelectedElements(){
	std::cout << "BUTESS::: "<<std::endl;
	//unsigned short* bbint = (unsigned short*)bytes;
	for(int i = 0;i<memBufferSize*memBufferSize;i++){
		//unsigned short red = bbint[i]>>6;
		if(memBuffer[i]!=100&&memBuffer[i]!=255)
			std::cout << (int)(memBuffer[i])<<" ";
	}
}
void writeSelectedElementsToFile(unsigned int* buffer, int size){
	std::fstream f("datos.raw",std::fstream::out | std::fstream::binary |
				   std::fstream::trunc);
	f.write((char*)buffer,size*sizeof(unsigned int));
	f.close();
}
bool MouseSelection::runRenderSelectionToTextureShader(Selection*, RModel* rmodel){
	resizeBuffer(rmodel->nVertices);
	if(!fboHandle)
		return false;
	GLfloat pointSize;
	glGetFloatv(GL_POINT_SIZE,&pointSize);
	fboHandle->bind();
	glPointSize(1.0f);
	glDisable( GL_DEPTH_TEST );
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glDisable(GL_POINT_SMOOTH);
	//glDisable (GL_BLEND);
	glDisable (GL_DITHER);
	glDisable (GL_FOG);
	glDisable (GL_LIGHTING);
	glDisable (GL_TEXTURE_1D);
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_TEXTURE_3D);
	glShadeModel (GL_FLAT);
	glDisable(GL_MULTISAMPLE);//*/
	//unsigned int clear_value[4] = { UNSELECTED_ELEMENT, 0, 0, 0 };
	unsigned int clear_value[4] = { 0, 0, 0, 0 };
	glClearBufferuiv(GL_COLOR, 0, clear_value);
	//bind our automata shader

	glBlendFunc(GL_ONE,GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_OR);
	glUseProgram(theProgramRenderTexture);
	glBindFragDataLocation(theProgramRenderTexture,GL_COLOR_ATTACHMENT0,"outputColor");
	ShaderUtils::setUniform(theProgramRenderTexture,"MVPVP",camera->getMVPVP());
	ShaderUtils::setUniform(theProgramRenderTexture,"MVP",camera->getMVP());
	ShaderUtils::setUniform(theProgramRenderTexture,"VP",camera->getViewPortMatrix());
	ShaderUtils::setUniform(theProgramRenderTexture,"orthoProjection",camera->isOrthoProjectionInUse());
	ShaderUtils::setUniform(theProgramRenderTexture,"NormalMatrix",normalMatrix);
	ShaderUtils::setUniform(theProgramRenderTexture,"CellWidth", 2.0f/memBufferSize); //pass in the width of the cells
	ShaderUtils::setUniform(theProgramRenderTexture,"BufferWidthHeight", memBufferSize); //pass in the width of render buffer

	//CONFIG
	ShaderUtils::setUniform(theProgramRenderTexture,"config.onlySurface",config.onlySurface);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.onlyInterior",config.onlyInterior);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.onlyFront",config.onlyFront);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.onlyBack",config.onlyBack);
	//ShaderUtils::setUniform(theProgramRenderTexture,"config.closestToCamera",config.closestToCamera);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.elementsType",config.elementsType);
	//ShaderUtils::setUniform(theProgramRenderTexture,"config.selectionType",config.selectionType);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.rectSelection",rectSelection);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.pixelTolerance",config.pixelTolerance);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.start",start);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.end",end);
	ShaderUtils::setUniform(theProgramRenderTexture,"viewPortOriginalSize",viewPortSize);
	ShaderUtils::setUniform(theProgramRenderTexture,"config.CameraPosition",cameraPosition);



	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	glEnableVertexAttribArray(IDS_ATTRIBUTE); // Vertex selection
	glEnableVertexAttribArray(NORMAL_ATTRIBUTE); // Vertex selection

	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexNormalDataBufferObject);
	glVertexAttribPointer( NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->rmodelVertexPositionBufferObject);
	glVertexAttribIPointer( IDS_ATTRIBUTE, 1, GL_INT, 0,
							(GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );
	//glReadBuffer(GL_COLOR_ATTACHMENT0);

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	glDisableVertexAttribArray(IDS_ATTRIBUTE); // Vertex selection
	glDisableVertexAttribArray(NORMAL_ATTRIBUTE); // Vertex selection
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	/*glReadPixels(0,0,memBufferSize,memBufferSize,
				 GL_RED_INTEGER,GL_UNSIGNED_BYTE ,memBuffer);//*/
	glReadPixels(0,0,memBufferSize,memBufferSize,
				 GL_RED_INTEGER,GL_UNSIGNED_INT ,memBuffer);//*/
	fboHandle->unbind();
	glPointSize(pointSize);
	//std::cout << "Datos size: "<<memBufferSize<<"x"<<memBufferSize<<std::endl;
	//writeSelectedElementsToFile(memBuffer,memBufferSize*memBufferSize);
	return true;
}
bool MouseSelection::createFBO(){

	ColorRenderBufferData data = {GL_R32UI,GL_COLOR_ATTACHMENT0};
	std::vector<ColorRenderBufferData> dataVector;
	dataVector.push_back(data);
	//Render buffer
	fboHandle = new FrameBufferObjectHandle(memBufferSize,memBufferSize,dataVector);
	if(!fboHandle->isOk()){
		memBufferSize = 0;
		delete fboHandle;
		return false;
	}
	return true;
}
bool MouseSelection::createFBOShader(){
	theProgramRenderTexture = 0;
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("SelectionStrategies/MouseSelection/ms.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("SelectionStrategies/MouseSelection/ms.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("SelectionStrategies/MouseSelection/ms.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData selectAttr = {VERTEX_FLAGS_ATTRIBUTE, "VertexFlags"};
	VertexAttributeBindingData idsAttr = {IDS_ATTRIBUTE, "VertexIds"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData normalAttr = {NORMAL_ATTRIBUTE, "VertexNormal"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(idsAttr);
	attributeList.push_back(selectAttr);
	attributeList.push_back(normalAttr);

	theProgramRenderTexture = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgramRenderTexture == ShaderUtils::FAIL_CREATING_PROGRAM){
		theProgramRenderTexture = 0;
		return false;
	}
	RENDER_TEXTURE_SHADER_SELECTION_AVAILABLE = true;
	return true;
}

void MouseSelection::resizeBuffer(int n){
	if(n<0)n = 0;
	else{
		std::cout << "N: "<<n<<std::endl;
		for( int renderBufferSideSize : renderBufferSideSizes ){
			if((renderBufferSideSize*renderBufferSideSize*32)>n){
				n = renderBufferSideSize;
				break;
			}
		}
	}
	if(n!=memBufferSize){
		clearBuffer();
		memBufferSize = n;
		//memBuffer = new unsigned char[memBufferSize*memBufferSize];
		memBuffer = new unsigned int[memBufferSize*memBufferSize];
		createFBO();
	}
}
void MouseSelection::clearBuffer(){
	if(memBufferSize){
		memBufferSize = 0;
		delete[] memBuffer;
		if(fboHandle){
			delete fboHandle;
			fboHandle = 0;
		}
	}
}
void MouseSelection::setUseOrthoProjection(bool b){
	useOrthoProjection = b;
}
