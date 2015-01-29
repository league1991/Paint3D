#pragma once
#include "ui_object.h"

class ObjectTransformWidget:public QScrollArea,Ui::TransformEditor
{
	Q_OBJECT
public:
	ObjectTransformWidget(void);
	~ObjectTransformWidget(void);

public slots:
	void updateWidgets();
	void updateSceneObject();
	void updateObjectName();
};
