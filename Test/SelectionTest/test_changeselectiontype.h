#ifndef TEST_CHANGESELECTIONTYPE_H
#define TEST_CHANGESELECTIONTYPE_H

#include <QTest>
#include <QWidget>

class RModel;
class OpenGLContext;
class ChangeSelectionType;
class ChangeSelectionTypeConfig;
class ChangeSelectionTypeTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void setupCompatibleConversion();
		void setupIncompatibleConversion();
		void setupSameConversion();
		void polygonToPolyhedronExclusive();
		void polygonToPolyhedronInclusive();
		void polyhedronToPolygonExclusive();
		void polyhedronToPolygonInclusive();
		///@note Conversions to and from vertices  are not implemented yet
		/// if possible attempt to find a way to simplify this code

		void cleanupTestCase();

	private:
		OpenGLContext* context;
		RModel* rmodel;
		ChangeSelectionType* tested;
		ChangeSelectionTypeConfig* config;
	};

#endif