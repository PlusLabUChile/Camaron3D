#include "qtutils.h"
#include "Utils/qtworkerbase.h"
#include <QWidget>
#include <QDockWidget>
QtUtils::QtUtils(){}
float QtUtils::readFloatFromQText(QString text, float defaultValue){
	bool ok = true;
	float readFloat = text.toFloat(&ok);
	if(!ok)
		return defaultValue;
	return readFloat;

}
int QtUtils::readIntFromQText(QString text, int defaultValue){
	bool ok = true;
	int readInt = text.toInt(&ok);
	if(!ok)
		return defaultValue;
	return readInt;

}
int QtUtils::readUIntFromQText(QString text, uint defaultValue){
	bool ok = true;
	uint readInt = text.toUInt(&ok);
	if(!ok)
		return defaultValue;
	return readInt;

}
void QtUtils::fixDockingWidgetArea(){

}

void QtUtils::fixDockingWidgetSize(QDockWidget* toFix){
	toFix->setMinimumSize(QSize(1,1));
}
QThread* QtUtils::runWorkerInThread(QtWorkerBase *worker, bool autodelete){
	QThread* thread = new QThread;
	worker->moveToThread(thread);
	//connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
	QObject::connect(thread, SIGNAL(started()), worker, SLOT(process()));
	QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
	if(autodelete){
		QObject::connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
		QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));//*/

		/*QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
	//delete thread only when thread has really finished
	QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	QObject::connect(thread, SIGNAL(terminated()), thread, SLOT(deleteLater()));//*/
	}
	thread->start();
	return thread;
}

void QtUtils::runWorkersInThread(std::vector<QtWorkerBase*>& workers,
												 bool wait){
	std::vector<QThread*> threads;
	for( QtWorkerBase* worker : workers ){
		threads.push_back(runWorkerInThread(worker,!wait));
	}
	bool throwBadAlloc = false;
	if(wait){
		for(std::vector<QThread*>::size_type i = 0;i<threads.size();i++){
			threads[i]->wait();
			throwBadAlloc = throwBadAlloc || workers[i]->hasBadAlloc();
			delete workers[i];
			delete threads[i];
		}
	}
	if(throwBadAlloc){
		throw std::bad_alloc();
	}
}
