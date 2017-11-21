#include "StdAfx.h"
#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "GLViewWidget.h"
#include "Paint3DFrame.h"


GLViewWidget::GLViewWidget(QWidget* parent):QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	backgroundClr.setRgb(150,150,150);
	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);
	isAltDown = false;
	isCtrlDown = false;
	isShiftDown= false;
	curTool = NULL;
	curToolType = TOOL_SELECT;
	isPickMode = false;
	pickCallBackFun = NULL;
}

GLViewWidget::~GLViewWidget(void)
{
}

void GLViewWidget::initializeGL()
{
	GLContext::instance()->getQGLFunctions()->initializeGLFunctions(this->context());
	qglClearColor(backgroundClr);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	Paint3DFrame::getInstance()->scene->init();
	// QMessageBox::information(NULL, QObject::tr("Info"), QObject::tr("All initialized."));
}

void GLViewWidget::paintGL()
{
	if (curToolType == TOOL_PAINT)
		glClearColor(94.0f/ 255.0f, 111.0f/ 255.0f, 138.0f/ 255.0f, 1.0f);
	else if (curToolType == TOOL_FACE_SELECT)
		glClearColor(102.0f/ 255.0f, 91.0f/ 255.0f, 164.0f/ 255.0f, 1.0f);
	else
		glClearColor(0.5,0.5,0.5,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 画出场景
	Paint3DFrame::getInstance()->scene->draw();

	// 画出操纵器
	if (curTool)
	{
		Camera& camera = Paint3DFrame::getInstance()->scene->getCamera();
		curTool->draw(camera);
	}

	// 画出选框
	if (curToolType == TOOL_FACE_SELECT && isLMBDown)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glLineWidth(2.0f);
		glEnable(GL_LINE_STIPPLE); 
		glLineStipple (1, 0x0F0F);  

		float w = (float)this->width();
		float h = (float)this->height();
		float curPoint[2]   = {curPos.x() / w * 2 - 1, 1 - curPos.y() / h * 2 };
		float pressPoint[2] = {pressPos.x() / w * 2 - 1, 1 - pressPos.y() / h * 2 };
		glColor3f(65.0f/ 255.0f, 65.0f/ 255.0f, 65.0f/ 255.0f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(curPoint[0], curPoint[1],0.5);
		glVertex3f(pressPoint[0], curPoint[1],0.5);
		glVertex3f(pressPoint[0], pressPoint[1],0.5);
		glVertex3f(curPoint[0], pressPoint[1],0.5);
		glEnd();

		glDisable(GL_LINE_STIPPLE);
		glEnable(GL_DEPTH_TEST);
	}
	glFlush();
}

void GLViewWidget::resizeGL( int width, int height )
{
	Paint3DFrame::getInstance()->scene->resizeCamera(width, height);
	updateGL();
}

void GLViewWidget::mousePressEvent( QMouseEvent *event )
{
	int x = event->x();
	int y = event->y();
	// 优先处理拾取模式
	if (isPickMode && pickCallBackFun)
	{
		pickCallBackFun(Paint3DFrame::getInstance()->scene->intersectObject(x,y));
		isPickMode = false;
		pickCallBackFun = NULL;
	}

	isLMBDown = event->buttons() & Qt::LeftButton;
	isMMBDown = event->buttons() & Qt::MidButton;
	isRMBDown = event->buttons() & Qt::RightButton;
	lastPos = event->pos();
	pressPos = event->pos();

	QSharedPointer<Scene> scene = Paint3DFrame::getInstance()->scene;
	if (!isAltDown)
	{
		if (isLMBDown)
		{
			curPos = event->pos();

			QVector3D ori, dir;
			scene->getCamera().getRay(x, y, ori, dir);
			if (curToolType == GLViewWidget::TOOL_SELECT)    // 如果当前为选择工具，直接选择物体
			{
				curSelectObj = scene->selectObject(x, y);
			}
			else if (curToolType == GLViewWidget::TOOL_PAINT)
			{
				scene->getPainter().beginPaint(QVector2D(x, y));
			}
			else if (curTool)            
			{
				// 如果为移动/旋转/缩放工具，
				// 首先检查是否点击了操纵轴，
				// 如果是，开始操纵，如果不是，说明是想选择物体
				QWeakPointer<RenderableObject> oldObject = curSelectObj;
				QWeakPointer<RenderableObject> newObject = scene->intersectObject(x,y);
				bool isSameObj = oldObject == newObject;
				char axis = curTool->intersect(ori, dir);
				bool isHitAxis = axis != -1;
				if (!oldObject && !newObject.isNull())
				{ // 原来没有选中物体，现在选中一个新物体
					curSelectObj = newObject;
					curSelectObj.data()->select();
					curTool->captureObject(QSharedPointer<RenderableObject>(curSelectObj).toWeakRef());
				}
				else if (newObject.isNull())
				{    // 没有选中新物体
					if (isHitAxis)
					{    // 但是选中了操纵器的一个轴，于是操纵当前物体
						//m_curSelectObj = m_curTool->getCurObject();
						if (curSelectObj)
						{
							curSelectObj.data()->select();
							curTool->selectAxis(axis);
							curTool->beginManipulate(ori, dir, axis);
						}
					}
					else
					{  // 什么都没选中
						curTool->releaseObject();
						if (!curSelectObj.isNull())
						{
							curSelectObj.data()->deselect();
							curSelectObj.clear();
						}
					}
				}
				else if ( isHitAxis && oldObject)
				{    // 选中原有物体的一个轴向
					//m_curSelectObj = m_curTool->getCurObject();
					curTool->selectAxis(axis);
					curTool->beginManipulate(ori, dir, axis);
				}
				else if (!isSameObj && !isHitAxis) 
				{    // 选中一个新物体
					if (!curSelectObj.isNull())
						curSelectObj.data()->deselect();
					curSelectObj = newObject;
					curSelectObj.data()->select();
					curTool->captureObject(curSelectObj);
				}
			}
		}
	}
	Paint3DFrame::getInstance()->layerEditor->updateList();
	Paint3DFrame::getInstance()->actionPaint->setEnabled(!curSelectObj.isNull());
	Paint3DFrame::getInstance()->actionSelectFace->setEnabled(!curSelectObj.isNull());
	Paint3DFrame::getInstance()->transformEditor->updateWidgets();
	updateGL();
}

void GLViewWidget::mouseMoveEvent( QMouseEvent *event )
{
	int x = event->x();
	int y = event->y();
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();
	QSharedPointer<Scene>scene = Paint3DFrame::getInstance()->scene;
	Camera& camera = scene->getCamera();

	curPos = event->pos();
	if (isAltDown)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			scene->rotateCamera(dx, dy);
		} 
		else if (event->buttons() & Qt::RightButton) 
		{
			scene->zoomCamera(2*(dx + dy));
		}
		else if (event->buttons() & Qt::MiddleButton) 
		{
			scene->moveCamera(dx, dy);
		}
	}
	else
	{
		if (isLMBDown)
		{			
			QVector3D ori,dir;
			scene->getCamera().getRay(x, y, ori, dir);
			if (curSelectObj && curTool)
			{
				if (curTool->isManipulating())
				{
					curTool->goOnManipulate(ori, dir);
				}
			}
			else if (curToolType == GLViewWidget::TOOL_PAINT)
			{
				scene->getPainter().goOnPaint(QVector2D(x,y));
			}
		}
		else
		{
			//Paint3DFrame::getInstance()->scene->onMouseHover(event->x(), event->y());
			QVector3D ori, dir;
			camera.getRay(x, y, ori, dir);
			if (curTool)
			{
				if (!curTool->isManipulating())
				{
					char axis = curTool->intersect(ori, dir);
					if (axis >= 0 && axis <= 2)
					{
						curTool->selectAxis(axis);
					}
				}
			}
			else if (curToolType == GLViewWidget::TOOL_PAINT)
			{
				scene->getPainter().onMouseHover(QVector2D(x,y));
			}
		}
			
	}
	lastPos = event->pos();
	updateGL();
}

void GLViewWidget::mouseReleaseEvent( QMouseEvent *event )
{
	int x = event->x();
	int y = event->y();
	QSharedPointer<Scene>scene = Paint3DFrame::getInstance()->scene;
	Camera& camera = scene->getCamera();
	if (!isAltDown && isLMBDown)
	{
		if (curSelectObj && curTool)
		{	// 操纵器命令
			if (curTool->isManipulating())
			{
				curTool->endManipulate();
				ManipulateCommand::ManipulateCommandType type;
				switch(curToolType)
				{
				case TOOL_TRANSLATE:
					type = ManipulateCommand::MANCMD_TRANSLATE; break;
				case TOOL_ROTATE:
					type = ManipulateCommand::MANCMD_ROTATE; break;
				case TOOL_SCALE:
					type = ManipulateCommand::MANCMD_SCALE; break;
				}
				QUndoCommand* cmd = new ManipulateCommand(
					curSelectObj.data()->getObjectID(), 
					curTool->getOldTransform(), 
					curTool->getNewTransform(),
					type);
				scene->getUndoStack().push(cmd);
				goto END_RELEASE;
			}
		}
		else if (curToolType == GLViewWidget::TOOL_PAINT)
		{	// 画画命令
			scene->getPainter().endPaint(QVector2D(x,y));
		}
		else if (curToolType == GLViewWidget::TOOL_FACE_SELECT)
		{	// 面选择命令
			GeometryExposer& exposer = scene->getGeometryExposer();
			float w = this->width();
			float h = this->height();
			QVector2D minRatio(min(x,pressPos.x())/w, min(y, pressPos.y())/h);
			QVector2D maxRatio(max(x,pressPos.x())/w, max(y, pressPos.y())/h);
			QSet<int>faceIDSet;
			Mesh* objPtr = (Mesh*)curSelectObj.data();
			exposer.getRegionFaceID(minRatio, maxRatio, objPtr->getObjectID(), faceIDSet);
			if (isCtrlDown)
				objPtr->addSelectedFaceID(faceIDSet);
			else if (isShiftDown)
				objPtr->removeSelectedFaceID(faceIDSet);
			else
				objPtr->setSelectedFaceID(faceIDSet);
		}
	}
END_RELEASE:
	isLMBDown = false;
	Paint3DFrame::getInstance()->transformEditor->updateWidgets();
	updateGL();
	return;
}
void GLViewWidget::keyPressEvent( QKeyEvent *event )
{
	switch(event->key())
	{
	case Qt::Key_Alt:
		isAltDown = true;
		break;
	case Qt::Key_Control:
		isCtrlDown = true;
		break;
	case Qt::Key_Shift:
		isShiftDown = true;
		break;
	case Qt::Key_Delete:
		QSharedPointer<RenderableObject> renObj = Paint3DFrame::getInstance()->viewWidget->getSelectedObject();;
		if (renObj)
		{
			QUndoCommand* cmd = new RemoveObjectCommand(renObj);
			Paint3DFrame::getInstance()->scene->getUndoStack().push(cmd);
			Paint3DFrame::getInstance()->actionSelect->trigger();
			Paint3DFrame::getInstance()->transformEditor->updateWidgets();
		}
	}
}

void GLViewWidget::keyReleaseEvent( QKeyEvent *event )
{
	switch(event->key())
	{
	case Qt::Key_Alt:
		isAltDown = false;break;
	case Qt::Key_Control:
		isCtrlDown = false; break;
	case Qt::Key_Shift:
		isShiftDown = false; break;
	}
}


void GLViewWidget::wheelEvent( QWheelEvent *event )
{
	Paint3DFrame::getInstance()->scene->zoomCamera(event->delta());
	if (curToolType == TOOL_PAINT)
	{
		Paint3DFrame::getInstance()->scene->getPainter().onMouseHover(QVector2D(lastPos.x(),lastPos.y()));
	}
	updateGL();
}

void GLViewWidget::setTool( ToolType type )
{
	if (curToolType == TOOL_PAINT)
	{
		Paint3DFrame::getInstance()->paintEditor->attachToCamera(false);
	}
	curToolType = type;
	QSharedPointer<Scene> scene = Paint3DFrame::getInstance()->scene;
	bool isPaintMode = type == TOOL_PAINT;
	Mesh::enableWireFrame(!isPaintMode);
	scene->getBrush().activate(isPaintMode);
	Paint3DFrame::getInstance()->actionImport->setEnabled(!isPaintMode);
	Paint3DFrame::getInstance()->actionPlaneLocator->setEnabled(!isPaintMode);
	switch (type)
	{
	case TOOL_TRANSLATE:
		curTool = &translateTool;
		break;
	case TOOL_ROTATE:
		curTool = &rotateTool;
		break;
	case TOOL_SCALE:
		curTool = &scaleTool;
		break;
	case TOOL_PAINT:
	case TOOL_SELECT:
	case TOOL_FACE_SELECT:
		curTool = NULL;
	}
	if (curTool)
	{
		QSharedPointer<RenderableObject>cso(curSelectObj);
		curTool->captureObject(cso.toWeakRef());
	}
	if (curToolType == GLViewWidget::TOOL_PAINT && curSelectObj)
	{
		if (curSelectObj.data()->getType() & RenderableObject::OBJ_MESH)
		{
			QWeakPointer<Mesh> wm = qWeakPointerCast<Mesh>(QSharedPointer<RenderableObject>(curSelectObj));
			scene->getPainter().setObject(wm);
			scene->getBrush().setObject(wm);
		}
	}
	bool hasObject = !curSelectObj.isNull();
	isPaintMode = curToolType == GLViewWidget::TOOL_PAINT;
	Paint3DFrame::getInstance()->paintEditor->setEnabled(hasObject && isPaintMode);
	Paint3DFrame::getInstance()->paintEditor->changeColorPicker(true);
	if (!isPaintMode)
		Paint3DFrame::getInstance()->scene->setPickerObjectUnfrozen();
	updateGL();
}

bool GLViewWidget::focusCurSelected()
{
	if (curSelectObj)
	{
		RenderableObject *obj = curSelectObj.data();
		Paint3DFrame::getInstance()->scene->getCamera().setCenter(obj->getCenter(), obj->getApproSize() );
		Paint3DFrame::getInstance()->scene->updateGeometryImage();

		return true;
	}
	return false;
}

QSharedPointer<RenderableObject> GLViewWidget::getSelectedObject()
{
	return curSelectObj;
}

void GLViewWidget::enterPickerMode( void (*callBackFun)(QSharedPointer<RenderableObject>) )
{
	pickCallBackFun = callBackFun;
	isPickMode = true;
	setCursor(Qt::PointingHandCursor);
}
