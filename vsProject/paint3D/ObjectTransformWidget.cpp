#include "StdAfx.h"
#include "ObjectTransformWidget.h"
#include "Paint3DFrame.h"

ObjectTransformWidget::ObjectTransformWidget(void)
{
	setupUi(this);

	connect(transXSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(transYSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(transZSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(rotXSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(rotYSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(rotZSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(scaleXSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(scaleYSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(scaleZSpinBox, SIGNAL(editingFinished()), this, SLOT(updateSceneObject()));
	connect(nameEdit, SIGNAL(editingFinished()), this, SLOT(updateObjectName()));
}

ObjectTransformWidget::~ObjectTransformWidget(void)
{
}

void ObjectTransformWidget::updateWidgets()
{
	QSharedPointer<RenderableObject> pO = Paint3DFrame::getInstance()->viewWidget->getSelectedObject();
	if (pO)
	{
		ObjectTransform& trans =pO->getTransform();
		const QVector3D& pos = trans.getTranslate();
		transXSpinBox->setValue(pos.x());
		transYSpinBox->setValue(pos.y());
		transZSpinBox->setValue(pos.z());
		const QQuaternion& q= trans.getRotate();
		float w = q.scalar();
		float x = q.x();
		float y = q.y();
		float z = q.z();
		float a = atan2(2 *(w*x+y*z),1-2*(x*x+y*y));
		float b = asin(2*(w*y-z*x));
		float c = atan2(2*(w*z+x*y),1-2*(y*y+z*z));
		rotXSpinBox->setValue(a/M_PI*180);
		rotYSpinBox->setValue(b/M_PI*180);
		rotZSpinBox->setValue(c/M_PI*180);
		const QVector3D& sca = trans.getScale();
		scaleXSpinBox->setValue(sca.x());
		scaleYSpinBox->setValue(sca.y());
		scaleZSpinBox->setValue(sca.z());
		nameEdit->setText(pO->getName());
	}
	this->setEnabled(!pO.isNull());
}

void ObjectTransformWidget::updateSceneObject()
{
	QSharedPointer<RenderableObject> pO = Paint3DFrame::getInstance()->viewWidget->getSelectedObject();
	if (pO)
	{
		ObjectTransform  trans;
		trans.setTranslate(QVector3D(transXSpinBox->value(), transYSpinBox->value(), transZSpinBox->value()));
		float a = rotXSpinBox->value()/180.0*M_PI;
		float b = rotYSpinBox->value()/180.0*M_PI;
		float c = rotZSpinBox->value()/180.0*M_PI;
		float cosA2 = cos(a/2);
		float sinA2 = sin(a/2);
		float cosB2 = cos(b/2);
		float sinB2 = sin(b/2);
		float cosC2 = cos(c/2);
		float sinC2 = sin(c/2);
		QQuaternion newQuat;
		newQuat.setScalar(cosA2*cosB2*cosC2+sinA2*sinB2*sinC2);
		newQuat.setX     (sinA2*cosB2*cosC2-cosA2*sinB2*sinC2);
		newQuat.setY     (cosA2*sinB2*cosC2+sinA2*cosB2*sinC2);
		newQuat.setZ     (cosA2*cosB2*sinC2-sinA2*sinB2*cosC2);
		trans.setRotate(newQuat);
		trans.setScale(QVector3D(scaleXSpinBox->value(), scaleYSpinBox->value(), scaleZSpinBox->value()));
		QUndoCommand* cmd = new ManipulateCommand(pO->getObjectID(),pO->getTransform(),trans, ManipulateCommand::MANCMD_ALL);
		Paint3DFrame::getInstance()->scene->getUndoStack().push(cmd);
	}
}

void ObjectTransformWidget::updateObjectName()
{
	QString oldName = Paint3DFrame::getInstance()->viewWidget->getSelectedObject()->getName();
	Paint3DFrame::getInstance()->scene->rename(oldName, nameEdit->text());
	nameEdit->setText(Paint3DFrame::getInstance()->viewWidget->getSelectedObject()->getName());
}
