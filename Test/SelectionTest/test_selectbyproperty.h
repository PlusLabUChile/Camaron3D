#ifndef TEST_SELECTBYPROPERTY_H
#define TEST_SELECTBYPROPERTY_H

#include <QTest>
#include <QWidget>

class RModel;
class EvaluationStrategy;
class SelectionStrategyByProperty;
class SelectionStrategyByPropertyConfig;
class SelectByPropertyTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void selectElement();
		void setupNewSelection();
		void setupUnionSelection();
		void cleanupTestCase();

	private:
		RModel* rmodel;
		SelectionStrategyByProperty* tested;
		SelectionStrategyByPropertyConfig* config;
	};

#endif