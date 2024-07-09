#include <QApplication>
#include "visualizador.h"
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include "Utils/openglutils.h"
#include "Factories/EvaluationStrategyRegistry.h"
#include "Factories/modelexportstrategyregistry.h"
#include "Factories/ModelLoadingFactory.h"
#include "Factories/selectionstrategyregistry.h"
#include "Factories/RendererRegistry.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Visualizador w;
	w.show();
	if(!w.initGlewAndRendererFactory()){
		w.hide();
		RendererRegistry::deleteSingletonInstance();
		EvaluationStrategyRegistry::deleteSingletonInstance();
		ModelExportStrategyRegistry::deleteSingletonInstance();
		ModelLoadingFactory::deleteSingletonInstances();
		SelectionStrategyRegistry::deleteSingletonInstance();
		return -1;//fail
	}
	int end = a.exec();
	RendererRegistry::deleteSingletonInstance();
	EvaluationStrategyRegistry::deleteSingletonInstance();
	ModelExportStrategyRegistry::deleteSingletonInstance();
	ModelLoadingFactory::deleteSingletonInstances();
	SelectionStrategyRegistry::deleteSingletonInstance();
	OpenGLUtils::deleteSingletonInstance();
	return end;
}

