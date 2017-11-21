#include "StdAfx.h"
#include "RotateManipulator.h"
#include <float.h>
RotateManipulator::RotateManipulator(void)
{
	setSize(1.0f);
}


RotateManipulator::~RotateManipulator(void)
{
}

void RotateManipulator::draw(Camera& camera)
{
    if (curObject.isNull())
        return;

	QVector3D center = curObject.data()->getCenter();
	QVector3D delta = camera.getOrigin() - center;
	if (!isWorking)
	{
		setSize(delta.length() * M_SIZE_FACTOR);
	}

	glEnable (GL_CULL_FACE);
	glCullFace(GL_BACK);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(getTransform()->getTransRotMatrix().constData());
    glLineWidth(3.0f);

	float angle = 0;
	float deltaAngle = 3.1416 * 2 / ROT_M_BALL_SEGMENTS;

	float color[3][3];
	memcpy(color, axisColor, sizeof(float) * 9);
	color[curSelectedAxis][0] = 1;
	color[curSelectedAxis][1] = 1;
	color[curSelectedAxis][2] = 0;

    glBegin(GL_QUADS);
    for (char segment = 0; segment < ROT_M_BALL_SEGMENTS; segment++)
    {
        float cosAgl0 = cos(angle) * ballRadius;
        float sinAgl0 = sin(angle) * ballRadius;
		angle += deltaAngle;
		float cosAgl1 = cos(angle) * ballRadius;
		float sinAgl1 = sin(angle) * ballRadius;

		glColor3fv(color[0]);
		glVertex3f(-ballWidth, cosAgl0, sinAgl0);
		glVertex3f(-ballWidth, cosAgl1, sinAgl1);
		glVertex3f(ballWidth, cosAgl1, sinAgl1);
		glVertex3f(ballWidth, cosAgl0, sinAgl0);

		glColor3fv(color[1]);
		glVertex3f(cosAgl0, -ballWidth, sinAgl0);
		glVertex3f(cosAgl0, ballWidth, sinAgl0);
		glVertex3f(cosAgl1, ballWidth, sinAgl1);
		glVertex3f(cosAgl1, -ballWidth, sinAgl1);

		glColor3fv(color[2]);
		glVertex3f(cosAgl0, sinAgl0, -ballWidth);
		glVertex3f(cosAgl1, sinAgl1, -ballWidth);
		glVertex3f(cosAgl1, sinAgl1, ballWidth);
		glVertex3f(cosAgl0, sinAgl0, ballWidth);
    }
    glEnd();

    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

char RotateManipulator::intersect( const QVector3D& rayOri, const QVector3D& rayDir )
{
    float t;
	int bestAxis = -1;
	if (!curObject.isNull())
	{
		QVector3D ori = rayOri, dir = rayDir;
		transformRayToLocal(ori, dir);

        float bestT = FLT_MAX;
        for (int axis = 0; axis < 3; ++axis)
        {
            if(intersectOriginCylinder(ori, dir, axis, ballRadius, -detectWidth, detectWidth, t))
            {
                if (t < bestT)
                {
                    bestT = t;
                    bestAxis = axis;
                }
            }
            if (intersectOriginDisk(ori, dir, axis, ballRadius - detectWidth,ballRadius,  t))
            {
                if (t < bestT)
                {
                    bestT = t;
                    bestAxis = axis;
                }
            }
        }
    }
    return bestAxis;
}

void RotateManipulator::beginManipulate( const QVector3D& rayOri, const QVector3D& rayDir, char axis )
{
	M_CHECK_OBJECT(curObject)

	QVector3D ori = rayOri, dir = rayDir;
	transformRayToLocal(ori, dir);
	beginMatrix = getTransform()->getRotMatrix();
	QVector3D center = curObject.data()->getCenter();

    float t0 = FLT_MAX, t1 = FLT_MAX, t;
    intersectOriginCylinder(ori, dir, axis, ballRadius, -detectWidth, detectWidth, t0);
    intersectOriginDisk(ori, dir, axis, ballRadius - detectWidth,ballRadius,  t1);
    t = min(t0, t1);
    QVector3D pt = ori + t * dir;
	float ptV[] = {pt.x(), pt.y(), pt.z()};
	ptV[axis] = 0;
	pt = QVector3D(ptV[0], ptV[1], ptV[2]);
	float axisV[] = {0,0,0};
	axisV[axis] = 1;
    QVector3D axisVector(axisV[0], axisV[1], axisV[2]);
	rotLineTangent = QVector3D::crossProduct(axisVector,pt);
    rotLineTangent.normalize();
    rotLineOrigin = pt;
    beginProjPos = projRayToLine(ori, dir, rotLineOrigin, rotLineTangent);
    curSelectedAxis = axis;
    isWorking = true;
}

void RotateManipulator::goOnManipulate( const QVector3D& rayOri, const QVector3D& rayDir )
{
	M_CHECK_OBJECT(curObject)

	QVector3D ori = rayOri, dir = rayDir;
	ori = ori - curObject.data()->getCenter();
	ori = ori * beginMatrix;
	dir = dir * beginMatrix;

    float pos = projRayToLine(ori, dir, rotLineOrigin, rotLineTangent);
    float theta =  (pos - beginProjPos) * 8;
	beginProjPos = pos;

	float rotAxisV[] = {0,0,0};
	rotAxisV[curSelectedAxis] = 1;
	QVector3D rotAxis = getTransform()->getRotMatrix() * QVector3D(rotAxisV[0],rotAxisV[1],rotAxisV[2]);
	QQuaternion q;
	q = QQuaternion::fromAxisAndAngle(rotAxis, theta);
	getTransform()->rotate(q);
}
