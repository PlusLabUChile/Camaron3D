// Add C includes here

#if defined __cplusplus
    // Add C++ includes here
    // C++ libraries
    #include <iostream>
    #include <vector>
    #include <list>
    #include <chrono>
    #include <string>
    #include <memory>
    #include <map>
    #include <limits>
    #include <sstream>
    #include <unordered_map>
    // #include <windows.h>
    #include <algorithm> 
    #include <fstream>
   
    // External libraries
    #include <glm/glm.hpp>
    #include <GL/glew.h>

    // Qt libraries
    #include <Qt>
    #include <QApplication> // Qt includes
    #include <QMainWindow>
    #include <QWidget>
    #include <QThread>
    #include <QTimer>
    #include <QGLWidget>
    #include <QAction>
    #include <QObject>
    #include <QSemaphore>

    // UI Qt
    #include <QPushButton>
    #include <QLabel>
    #include <QRect>
    #include <QIcon>
    #include <QString>
    #include <QColorDialog>
    #include <QListWidgetItem>

    // Event Qt
    #include <QMouseEvent>
    #include <QKeyEvent>
    #include <QWheelEvent>

    // Camaron
    #include "Rendering/Renderers/baserendererconfig.h"
    #include "Model/Model.h"
    #include "Exceptions/unknownextensionexception.h"
    // #include "exceptionmessage.h"
    #include "UI/aboutcamaron.h"
    #include "Utils/streamscanner.h"
    #include "Utils/qtworkerbase.h"
    #include "Utils/qtutils.h"
    #include "Utils/keyboardstate.h"
    #include "Utils/crosstimer.h"
    #include "Utils/fileutils.h"
    #include "Utils/matrixtransformationutils.h"
    #include "Utils/renderinglock.h"
    #include "Utils/endianess.h"
    #include "Common/Constants.h"
#endif