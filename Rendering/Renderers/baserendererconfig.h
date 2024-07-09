#ifndef BASERENDERERCONFIG_H
#define BASERENDERERCONFIG_H

#include <QWidget>
#include <memory>

class ScalarFieldDef;

class BaseRendererConfig : public QWidget
{
	Q_OBJECT
	public:
		explicit BaseRendererConfig(QWidget *parent = 0);
	protected:
		void forceUpdate();

	signals:
		void onConfigForceApply();

};

#endif // BASERENDERERCONFIG_H
