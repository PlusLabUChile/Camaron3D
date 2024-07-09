#ifndef BENCH_MODELLOADINGELENODE_H
#define BENCH_MODELLOADINGELENODE_H

#include <QTest>
#include "ModelLoading/modelloadingelenode.h"

class PolyhedronMesh;
class ModelLoadingEleNodeBench: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();

		void sphereModelLoad_data();
		void sphereModelLoad();
		void cleanupTestCase();

	private:
		ModelLoadingEleNode* Loader;
	};

#endif