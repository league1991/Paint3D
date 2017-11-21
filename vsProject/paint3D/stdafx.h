#include <math.h>
#include <float.h>
#define GLUT_DISABLE_ATEXIT_HACK //这句话极其重要
#include <GL/glew.h>
//#include <gl/GLU.H>
//#include "glextensions.h"
//#include <GL/glut.h>

#include <iostream>
using namespace std;

#include <QDebug>
#include <QtGui>
#include <qvector.h>
#include <qmath.h>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QSharedPointer>
#include <QWeakPointer>
//#include <QtOpenGL>
#include <QtOpenGL/QGLFunctions>
#include <QtOpenGL/QGLShaderProgram>
#include <QtOpenGL/QGLBuffer>
#include <QtOpenGL/QGLFramebufferObject>
//#include <QtPlugin>


#include "GLContext.h"
#include "BoundingBox.h"
#include "RenderableObject.h"
#include "CanvasSeamFiller.h"
#include "Canvas.h"
#include "Mesh.h"
#include "ObjReader.h"
#include "CubeMap.h"
#include "Camera.h"
#include "Manipulator.h"
#include "TranslateManipulator.h"
#include "RotateManipulator.h"
#include "ScaleManipulator.h"
#include "GeometryExposer.h"
#include "Stroke.h"
#include "PaintPicker.h"
#include "Brush.h"
#include "Painter.h"
#include "HistoryCommand.h"
#include "Scene.h"
