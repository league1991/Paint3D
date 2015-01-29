#pragma once
#include <QtOpenGL/QGLWidget>

class Scene;

class GLViewWidget:public QGLWidget
{
	Q_OBJECT
public:
	GLViewWidget(QWidget* parent = 0);
	~GLViewWidget(void);

	enum ToolType{TOOL_SELECT = 0, TOOL_FACE_SELECT,TOOL_TRANSLATE, TOOL_ROTATE, TOOL_SCALE, TOOL_PAINT};
	void setTool(ToolType type);
	Manipulator*& getCurTool(){return curTool;}
	ToolType getCurToolType(){return curToolType;}
	bool focusCurSelected();
	QSharedPointer<RenderableObject> getSelectedObject();
	void clearSelectedObject(){curSelectObj = QWeakPointer<RenderableObject>();}

	void enterPickerMode(void (*callBackFun)(QSharedPointer<RenderableObject>));

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);


private:
	QWeakPointer<RenderableObject>curSelectObj;
	QColor backgroundClr;
	TranslateManipulator translateTool;
	RotateManipulator    rotateTool;
	ScaleManipulator     scaleTool;
	ToolType curToolType;
	Manipulator* curTool;

	bool isPickMode;		// 拾取模式，接受在场景中拾取物体的请求
	void (*pickCallBackFun)(QSharedPointer<RenderableObject>);

	bool isAltDown;
	bool isCtrlDown;
	bool isShiftDown;
	bool isLMBDown;
	bool isMMBDown;
	bool isRMBDown;
		
	QPoint pressPos;
	QPoint lastPos;
	QPoint curPos;
};
