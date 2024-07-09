#ifndef TEST_SELECTBYID_H
#define TEST_SELECTBYID_H

#include <QTest>
#include <QWidget>

class RModel;
class Selection;
class SelectById;
class SelectByIdConfig;
class SelectByIdTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void selectElements();
		void setupNewSelection();
		void setupOtherSelection();
		void cleanupTestCase();

	private:
		RModel* rmodel;
		SelectById* tested;
		SelectByIdConfig* config;
	};

#endif