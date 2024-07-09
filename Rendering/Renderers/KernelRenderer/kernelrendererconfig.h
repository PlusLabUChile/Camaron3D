#ifndef KERNELRENDERERCONFIG_H
#define KERNELRENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"

namespace Ui {
class KernelRendererConfig;
}

class KernelRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit KernelRendererConfig(QWidget *parent = 0);
		virtual ~KernelRendererConfig();
		void readConfig();
		glm::vec4 wireFrameColors;
		glm::vec4 baseModelColors;
		int wireFrameLineWidthM;
		
	private:
		Ui::KernelRendererConfig *ui;
};

#endif // KERNELRENDERERCONFIG_H
