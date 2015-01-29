#include "StdAfx.h"
#include "RenderableObject.h"
#include "Paint3DFrame.h"

unsigned RenderableObject::currNewObjectID = 1;

QHash<QString,int> RenderableObject::nameMap;

RenderableObject::RenderableObject(void):
    isObjSelected(false), 
    isObjVisible(true),
	localBBox(QVector3D(0,0,0),QVector3D(0,0,0)),
	alphaForAppearance(0.5f)
{
	objectID = currNewObjectID++;
	setName(QString("Object") + QString::number(objectID));
}


RenderableObject::~RenderableObject(void)
{
	nameMap.remove(this->name);
}

float RenderableObject::getApproSize()
{
	QVector3D wMin = transform.getTransformMatrix() * localBBox.pMin;
	QVector3D wMax = transform.getTransformMatrix() * localBBox.pMax;
	return (wMax - wMin).length();
}

void RenderableObject::setTransform( const ObjectTransform& trans )
{
	transform =trans;
}

void RenderableObject::setName( const QString& newName )
{
	QString finalName = newName;
	QHash<QString,int>::iterator i = nameMap.find(newName);
	if (i != nameMap.end() && name != newName)
	{	// 新名字与其他物体的名字冲突		
		while(nameMap.contains(finalName))
		{
			finalName = finalName + QString("_1");
		}
	}
	nameMap.remove(this->name);
	this->name = finalName;
	nameMap.insert(finalName, objectID);
}






void ObjectTransform::updateTransformMatrix()
{
	m_rotMatrix.setToIdentity();
	m_rotMatrix.rotate(m_rotation);
	m_transformMatrix.setToIdentity();
	m_transformMatrix.translate(m_translation);
	m_transformMatrix *= m_rotMatrix;
	m_norTransformMatrix = m_transformMatrix;
	m_transRotMatrix = m_transformMatrix;
	m_transformMatrix.scale(m_scale);
	m_norTransformMatrix.scale(1 / m_scale.x(), 1 / m_scale.y(), 1 / m_scale.z());

	m_invTransformMatrix.setToIdentity();
	m_invTransformMatrix.scale(1 / m_scale.x(), 1 / m_scale.y(), 1 / m_scale.z());
	QMatrix4x4 transposeRot = m_rotMatrix;
	m_invTransformMatrix *= transposeRot.transposed();
	m_invTransformMatrix.translate(-m_translation);
	// 还要更新场景的GeometryImage
	paint3DApp->scene->updateGeometryImage();
}


QDataStream& operator<<(QDataStream& out, const QSharedPointer<RenderableObject>&pObj)
{
	out << pObj->type;
	out << pObj->name;
	out << pObj->transform;
	if (pObj->type == RenderableObject::OBJ_MESH)
	{
		Mesh* mesh = (Mesh*)pObj.data();
		out << *mesh;
	}
	else if (pObj->type == RenderableObject::OBJ_PICKER_OBJECT)
	{
		PlanePicker* picker = (PlanePicker*)pObj.data();
		out << *picker;
	}
	return out;
}

QDataStream& operator>>(QDataStream&  in, QSharedPointer<RenderableObject>&pObj)
{
	quint32 type;
	QString name;
	ObjectTransform trans;
	in >> type;
	in >> name;
	in >> trans;
	if (type == RenderableObject::OBJ_MESH)
	{
		QSharedPointer<Mesh> pM(new Mesh);
		pObj = pM;
		in >> *pM;
	}
	else if (type == RenderableObject::OBJ_PICKER_OBJECT)
	{
		QSharedPointer<PlanePicker> pM(new PlanePicker);
		pObj = pM;
		in >> *pM;
	}
	pObj->setName(name);
	pObj->transform = trans;;
	return in;
}

QDataStream& operator<<(QDataStream& out, const ObjectTransform& trans)
{
	out << trans.m_translation
		<< trans.m_rotation
		<< trans.m_scale;
	return out;
}

QDataStream& operator>>(QDataStream& in ,       ObjectTransform& trans)
{
	in  >> trans.m_translation
		>> trans.m_rotation
		>> trans.m_scale;
	trans.updateTransformMatrix();
	return in;
}