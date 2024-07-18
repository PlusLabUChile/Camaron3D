#ifndef VISUALIZADOR_H
#define VISUALIZADOR_H

#define TAB_SELECT_USING_GEOMETRY_INDEX 0
#define TAB_SELECT_USING_EVALUATION_STRATEGY_INDEX 1
#include <QMainWindow>
#include <QWidget>
#include <QRect>
#include <list>
#include "rendererconfigpopup.h"
#include "SelectionStrategies/Selection.h"
#include "ModelGeneralStatics/modelgeneralstaticscollector.h"
#include "UI/staticsframe.h"
#include "UI/selectiontableview.h"
#include "UI/mainconfiguration.h"
#include "UI/modelgeneralstaticswidget.h"
#include "UI/aboutcamaron.h"
#include "UI/KeyshortcutConfig/keyshortcutconfiguration.h"
#include "UI/modelloadingprogressdialog.h"
#include "UI/kernelloadingprogressdialog.h"
#include "UI/propertyfieldloaddialog.h"
#include "Utils/crosstimer.h"
#include "Rendering/Renderers/KernelRenderer/KernelAlgorithm/polyhedronKernel.h"
#include "UI/kernelWidget.h"
#include "UI/kernelPop.h"
#include "UI/Camera/Camera.h"
#include <chrono>
#include <string.h>

class CustomGLViewer;
namespace Ui {
	class Visualizador;
}
class EvaluationStrategyRegistry;
class ModelExportStrategyRegistry;
class SelectionStrategyRegistry;
class RendererRegistry;
class SelectionStrategy;
class ModelLoadingFactory;
class PropertyFieldLoadingFactory;
class RModel;
class RenderersList;
class Model;
class MouseSelection;
struct SelectionStrategyData;
struct IndexList{
	std::vector<int> indexElementsList;
};
class Visualizador : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit Visualizador(QWidget *parent = 0);
		~Visualizador();
		bool initGlewAndRendererFactory();
	public slots:
		void closeModel();
		void openFile();
		void importFilePropertyField();
		void loadPropertyFieldsQThread(std::string, std::vector<std::shared_ptr<PropertyFieldDef>>);
		void onLoadedPropertyFields();
		void exportSelection();
		void selectUsingSelectionStrategy();
		void evaluate();
		void exportLoadedModel();
		void exportKernelModel();
		void changeRenderer(int);
		void changeSelectionStrategy(int);
		void changeEvaluationStrategy(int);
		void showEvaluationStrategyDescription();
		void configurationUpdated();
		void receiveSelectionBox(int,int,int,int);
		void showModelGeneralStatics();
		void addSecondaryRenderer();
		void secondaryRendererRemoved();
		//void mouseDoubleClickEvent(QMouseEvent *e);
		void openRecentFile();
		void glVersionPopup();
		void clearRecentFiles();
		void getLoadedModelFromLoadingStrategy();
		void showAxesChanged(bool value);
		void callCalculateKernel();
		void finishCalculateKernel();
		void openKernelWindow();

		void closeQDockWidget();


	private:
		static const unsigned int MAX_RECENT_FILES = 5;
		void exportModel(Model*);
		void fixDockWidgetPositions();
		void cleanSelectionStrategyConfigQWidget();
		void setBaseSelectionStrategyData(SelectionStrategyData*);
		void fillEvaluationStrategiesComboBox();
		void fillSelectionStrategiesComboBox();
		void fillRendererComboBox();
		void connectModelLoadingStrategies(ModelLoadingFactory*);
		void connectPropertyFieldLoadingStrategies(PropertyFieldLoadingFactory*);
		void connectPolyhedronKernelCompute();
		void loadRecentFiles();
		void saveRecentFiles();
		void updateRecentFilesQActions();
		void addRecentFiles(QString filepath);
		void enableAndDisableWidgets();
		void checkInformationRenderer(int);

		std::string getModelAcceptedExtensions(ModelLoadingFactory* factory);
		std::string getPropertyFieldAcceptedExtensions(PropertyFieldLoadingFactory* factory);
		std::string getModelAcceptedExportExtensions();
		void setupEvaluationStrategiesStatics();
		void openModelFromFilePathQThread(QString filename);
		void openPropertyFieldDialogFromFilePath(QString filename, bool userTriggered);
		Ui::Visualizador *ui;

		// Camera
		CameraCamaron* camera;

		//Model
		bool modelGeneralStaticsCollected;
		Selection selection;
		Model* model;
		ModelGeneralStaticsCollector modelGeneralStaticsCollector;
		//Model Loading
		std::string fileFormats, fileFormatsPropertyField;
		std::string fileExportFormats;
		//Model Rendering
		CustomGLViewer* customGLViewer;
		RenderersList* renderersList;
		RendererConfigPopUp rendererConfigPopup;
		RModel* rmodel;

		Model* kernel_model;
		RModel* kernel_rmodel;
		PolyhedronKernel* kernelAlgorithm;

		//SelectionStrategy
		SelectionStrategy* selectionStrategy;
		MouseSelection* mouseSelection;
		//Registries
		RendererRegistry* rendererRegistry;
		EvaluationStrategyRegistry* evaluationStrategyRegistry;
		ModelLoadingFactory* modelLoadingFactory;
		PropertyFieldLoadingFactory* propertyFieldLoadingFactory;
		SelectionStrategyRegistry* selectionStrategyRegistry;
		ModelExportStrategyRegistry* modelExportStrategyRegistry;

		//Statistics
		StaticsFrame staticsFrame;

		//Selection Table
		SelectionTableView selectionTableView;
		MainConfiguration mainConfiguration;
		KeyShortcutConfiguration shortcutconfig;
		ModelGeneralStaticsWidget modelGeneralStaticsWidget;

		//State
		QByteArray savedState;
		//QDockingWidgetOrder
		QList<QDockWidget*> dockList;
		//Camaron About
		AboutCamaron aboutCamaron;
		//Evaluation Strategy Config
		QWidget* evalStrategyConfigWidget;
		std::vector<QAction*> recentFilesQActions;
		std::list<std::string> recentFilesnames;

		ModelLoadingProgressDialog progressDialog;
		PropertyFieldLoadDialog propertyFieldDialog;
		KernelLoadingProgressDialog kernelDialog;
		CrossTimer timer;

		KernelWidget kernelWidget;
		KernelPop kernelPop;
};

#endif // VISUALIZADOR_H
