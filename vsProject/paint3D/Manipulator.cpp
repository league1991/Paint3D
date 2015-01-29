#include "StdAfx.h"
#include "Manipulator.h"

const char Manipulator::otherAxis[3][2] = {{1,2},{0,2},{0,1}};
const float Manipulator::cosTable[13] = {1, 0.87 ,0.5, 0, -0.5, -0.87, -1, -0.87, -0.5, 0, 0.5, 0.87,1};
const float Manipulator::sinTable[13] = {0, 0.5, 0.87,1, 0.87 ,0.5, 0, -0.5, -0.87, -1, -0.87, -0.5, 0};
const float Manipulator::axisColor[3][3]  = {{1,0,0},{0,1,0},{0,0,1}};

Manipulator::Manipulator(void)
{
    isWorking = 0;
}


Manipulator::~Manipulator( void )
{

}
/*
void GLManipulator::setCenter( const QVector3D& center )
{
    m_center = center;
}*/

bool Manipulator::intersectOriginCylinder( const QVector3D& rayOri, const QVector3D& rayDir, 
    const char axis, const float radius, const float start, const float end, float& t )
{
    char otherAxis0 = otherAxis[axis][0];
    char otherAxis1 = otherAxis[axis][1];
	float vOri[] = {rayOri.x(), rayOri.y(), rayOri.z()};
	float vDir[] = {rayDir.x(), rayDir.y(), rayDir.z()};

    float A = vDir[otherAxis0] * vDir[otherAxis0] + vDir[otherAxis1] * vDir[otherAxis1];
    float B = 2.0 *(vOri[otherAxis0] * vDir[otherAxis0] + vOri[otherAxis1] * vDir[otherAxis1]);
    float C = vOri[otherAxis0] * vOri[otherAxis0] + vOri[otherAxis1] * vOri[otherAxis1] - radius * radius;
    float delta = B * B - 4 * A * C;
    if (delta < 0)        // not root
        return false;
    float rayT[2];
    rayT[0] = (-B + sqrt(delta)) / (2 * A);
    rayT[1] = (-B - sqrt(delta)) / (2 * A);
    float z0    = vOri[axis] + vDir[axis] * rayT[0];
    float z1    = vOri[axis] + vDir[axis] * rayT[1];

    bool isIsect[2];
    isIsect[0] = z0 > start && z0 < end && rayT[0] > 0;
    isIsect[1] = z1 > start && z1 < end && rayT[1] > 0;
    if (!isIsect[0] && !isIsect[1])
        return false;

    if (isIsect[0] && isIsect[1])
        t = min(rayT[0], rayT[1]);
    else if (isIsect[0])
        t = rayT[0];
    else
        t = rayT[1];
    return true;
}


bool Manipulator::intersectOriginDisk( const QVector3D& rayOri, const QVector3D& rayDir, 
    const char axis, const float innerRadius, const float outerRadius, float& t )
{
	float vOri[] = {rayOri.x(), rayOri.y(), rayOri.z()};
	float vDir[] = {rayDir.x(), rayDir.y(), rayDir.z()};

    float rayT = -vOri[axis] / vDir[axis];
    if (rayT > 0)
    {
        char otherAxis0 = otherAxis[axis][0];
        char otherAxis1 = otherAxis[axis][1];

        float v0 = vOri[otherAxis0] + rayT * vDir[otherAxis0];
        float v1 = vOri[otherAxis1] + rayT * vDir[otherAxis1];

        float r = sqrt(v0 * v0 + v1 * v1);
        if (r >=innerRadius && r <= outerRadius)
        {
            t = rayT;
            return true;
        }
    }
    return false;
}

float Manipulator::projRayToOriginAxis( const QVector3D& rayOri, const QVector3D& rayDir, const char axis )
{
	float vOri[] = {rayOri.x(), rayOri.y(), rayOri.z()};
	float vDir[] = {rayDir.x(), rayDir.y(), rayDir.z()};

    float d12 = vDir[axis];
	float c1  = QVector3D::dotProduct(rayOri, rayDir);
    float c2  = vOri[axis];
    float denominator = 1 - d12 * d12;
    float absDeno = abs(denominator);
    if (absDeno < 0.01f)
    {
        denominator *= 0.01f / absDeno;
    }
    return (c2 - d12 * c1) / denominator;
}

float Manipulator::projRayToLine( const QVector3D& rayOri, const QVector3D& rayDir, const QVector3D& lineOri, const QVector3D& lineDir )
{
    QVector3D ori = rayOri - lineOri;
	float c1 = QVector3D::dotProduct(ori, rayDir);
    float c2 = QVector3D::dotProduct(ori, lineDir);
    float d11 = 1;
    float d12 = QVector3D::dotProduct(rayDir,lineDir);
    float d22 = 1;
    float denominator = 1 - d12 * d12;
    float absDeno = abs(denominator);
    if (absDeno < 0.01f)
    {
        denominator *= 0.01f / absDeno;
    }
    return (c2*d11 - d12 * c1) / denominator;
}



void Manipulator::captureObject( QWeakPointer<RenderableObject> object )
{
    curObject = object;
    if(curObject)
    {		
		RenderableObject* obj = curObject.data();
        obj->select();
    }
}

void Manipulator::endManipulate()
{
	RenderableObject* obj = curObject.data();
	if (obj)
	{
		newTransform = obj->getTransform();
	}
    isWorking = false;
}

void Manipulator::releaseObject()
{
	if(!curObject.isNull())
	{
		curObject.data()->deselect();
	}
	curObject.clear();
}

void Manipulator::transformRayToLocal( QVector3D& ori, QVector3D& dir)
{
	const QMatrix4x4& rotMat = getTransform()->getRotMatrix();
	ori = ori - curObject.data()->getCenter();
	ori = ori * rotMat;
	dir = dir * rotMat;
}
