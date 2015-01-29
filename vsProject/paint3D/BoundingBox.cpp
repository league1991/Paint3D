#include "BoundingBox.h"
#include "stdafx.h"

float BoundingBox::delta = 1e-4f;

BoundingBox::BoundingBox(void)
{
}
BoundingBox::~BoundingBox(void)
{
}
BoundingBox::BoundingBox(QVector3D ipMin,QVector3D ipMax)
{
	pMin=ipMin;pMax=ipMax;
}
void BoundingBox::merge(const QVector3D&point,bool isBigger)
{
	float e = 0.1f;
	if(isBigger)
		e = delta;
	if(pMin.x()>point.x()-e)pMin.setX(point.x()-e);
	if(pMin.y()>point.y()-e)pMin.setY(point.y()-e);
	if(pMin.z()>point.z()-e)pMin.setZ(point.z()-e);
	if(pMax.x()<point.x()+e)pMax.setX(point.x()+e);
	if(pMax.y()<point.y()+e)pMax.setY(point.y()+e);
	if(pMax.z()<point.z()+e)pMax.setZ(point.z()+e);
}
void BoundingBox::merge(const BoundingBox&iBox)
{
	if(pMin.x()>iBox.pMin.x())pMin.setX(iBox.pMin.x());
	if(pMin.y()>iBox.pMin.y())pMin.setY(iBox.pMin.y());
	if(pMin.z()>iBox.pMin.z())pMin.setZ(iBox.pMin.z());
	if(pMax.x()<iBox.pMax.x())pMax.setX(iBox.pMax.x());
	if(pMax.y()<iBox.pMax.y())pMax.setY(iBox.pMax.y());
	if(pMax.z()<iBox.pMax.z())pMax.setZ(iBox.pMax.z());
}

void BoundingBox::draw()const
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glDisable(GL_CULL_FACE);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(pMin.x(),pMin.y(),pMin.z());
	glVertex3f(pMax.x(),pMin.y(),pMin.z());
	glVertex3f(pMin.x(),pMax.y(),pMin.z());
	glVertex3f(pMax.x(),pMax.y(),pMin.z());
	glVertex3f(pMin.x(),pMax.y(),pMax.z());
	glVertex3f(pMax.x(),pMax.y(),pMax.z());
	glVertex3f(pMin.x(),pMin.y(),pMax.z());
	glVertex3f(pMax.x(),pMin.y(),pMax.z());
	glVertex3f(pMin.x(),pMin.y(),pMin.z());
	glVertex3f(pMax.x(),pMin.y(),pMin.z());
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}
inline void BoundingBox::swap(float&t1,float&t2)
{
	float temp=t1;t1=t2;t2=temp;
}

int BoundingBox::maxAxis()
{
	float x=pMax.x()-pMin.x();
	float y=pMax.y()-pMin.y();
	float z=pMax.z()-pMin.z();
	return (x>y)?((x>z)?0:2):((y>z)?1:2);
}
void BoundingBox::displayCoords()const
{
	qDebug()<<"pMin: \t";
	qDebug() << pMin;
	qDebug()<<"pMax: ";
	qDebug() << pMax;
	qDebug()<<endl;
}
BoundingBox BoundingBox::operator =(const BoundingBox&box)
{
	pMax=box.pMax;
	pMin=box.pMin;
	return *this;
}
float BoundingBox::halfArea()
{
	QVector3D boxExtent=pMax-pMin;
	return boxExtent.x()*boxExtent.y()+
			boxExtent.y()*boxExtent.z()+
			boxExtent.z()*boxExtent.x();
}

float BoundingBox::area()
{
	return halfArea()*2.0f;
}

bool BoundingBox::operator==( const BoundingBox& box ) const
{
	if(pMin == box.pMin && pMax == box.pMax)
		return true;
	return false;
}

void BoundingBox::getTightBound( QVector3D& tightMin, QVector3D& tightMax )
{
	tightMin = pMin + QVector3D(delta, delta, delta);
	tightMax = pMax - QVector3D(delta, delta, delta);
}

bool BoundingBox::intersect( const BoundingBox& iBox, BoundingBox& resultBox )
{
	resultBox.pMin.setX(pMin.x() > iBox.pMin.x() ? pMin.x() : iBox.pMin.x());
	resultBox.pMax.setX(pMax.x() < iBox.pMax.x() ? pMax.x() : iBox.pMax.x());
	if (resultBox.pMin.x() > resultBox.pMax.x())
		return false;
	resultBox.pMin.setY(pMin.y() > iBox.pMin.y() ? pMin.y() : iBox.pMin.y());
	resultBox.pMax.setY(pMax.y() < iBox.pMax.y() ? pMax.y() : iBox.pMax.y());
	if (resultBox.pMin.y() > resultBox.pMax.y())
		return false;
	resultBox.pMin.setZ(pMin.z() > iBox.pMin.z() ? pMin.z() : iBox.pMin.z());
	resultBox.pMax.setZ(pMax.z() < iBox.pMax.z() ? pMax.z() : iBox.pMax.z());
	if (resultBox.pMin.z() > resultBox.pMax.z())
		return false;
	return true;
}

bool BoundingBox::isInBoxInclusive( const QVector3D& point )
{
	return point.x() >= pMin.x() && point.y() >= pMin.y() && point.z() >= pMin.z() &&
		   point.x() <= pMax.x() && point.y() <= pMax.y() && point.z() <= pMax.z() ;
}

bool BoundingBox::isInBoxExclusive( const QVector3D& point )
{
	return point.x() > pMin.x() && point.y() > pMin.y() && point.z() > pMin.z() &&
		   point.x() < pMax.x() && point.y() < pMax.y() && point.z() < pMax.z() ;
}

bool BoundingBox::onFace( QVector3D& point, int& face )
{
	if (point.x() == pMin.x())
	{
		face = 1; return true;
	} 
	else if (point.x() == pMax.x())
	{
		face = 0; return true;
	}
	else if (point.y() == pMin.y())
	{
		face = 3; return true;
	} 
	else if (point.y() == pMax.y())
	{
		face = 2; return true;
	}
	else if (point.z() == pMin.z())
	{
		face = 5; return true;
	} 
	else if (point.z() == pMax.z())
	{
		face = 4; return true;
	}
	return false;
}