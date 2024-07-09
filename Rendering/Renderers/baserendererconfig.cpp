#include "baserendererconfig.h"

BaseRendererConfig::BaseRendererConfig(QWidget *parent) : QWidget(parent)
{

}

void BaseRendererConfig::forceUpdate() {
	emit onConfigForceApply();
}