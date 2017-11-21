#include "StdAfx.h"
#include "ScaleManipulator.h"


ScaleManipulator::ScaleManipulator(void)
{
	setSize(1.0f);
}


ScaleManipulator::~ScaleManipulator(void)
{
}


void ScaleManipulator::draw(Camera& camera)
{
    if (curObject.isNull())
        return;

	QVector3D center = curObject.data()->getCenter();
	QVector3D delta = camera.getOrigin() - center;
	if (!isWorking)
	{
		setSize(delta.length() * M_SIZE_FACTOR);
	}

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glMultMatrixf(getTransform()->getTransRotMatrix().constData());

    glPointSize(15.0);
    for (char axis = 0; axis < 3; ++axis)
    {
        if (axis == curSelectedAxis)
            glColor3f(1,1,0);
        else
            glColor3fv(axisColor[axis]);

		float endPointV[] = {0,0,0};
		endPointV[axis] = axisLength;
		glLineWidth(2);
        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3fv(endPointV);
        glEnd();

        glBegin(GL_POINTS);
        glVertex3fv(endPointV);
        glEnd();
    }
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

char ScaleManipulator::intersect( const QVector3D& rayOri, const QVector3D& rayDir)
{
	if (!curObject.isNull())
	{
		QVector3D ori = rayOri, dir = rayDir;
		transformRayToLocal(ori, dir);
        float t;
        for (int axis = 0; axis < 3; ++axis)
        {
            if(intersectOriginCylinder(ori, dir, axis, detectRadius, 0, axisLength,t))
            {
                return axis; 
            }
        }
    }
    return -1;
}

void ScaleManipulator::beginManipulate( const QVector3D& rayOri, const QVector3D& rayDir , char axis)
{
	M_CHECK_OBJECT(curObject)

	QVector3D ori = rayOri, dir = rayDir;
	transformRayToLocal(ori, dir);
    isWorking = true;
    curSelectedAxis = axis;
    beginProjPos = projRayToOriginAxis(ori, dir, curSelectedAxis);
    curScale = 1.0f;
}

void ScaleManipulator::goOnManipulate( const QVector3D& rayOri, const QVector3D& rayDir )
{
	M_CHECK_OBJECT(curObject)

	QVector3D ori = rayOri, dir = rayDir;
	transformRayToLocal(ori, dir);
    float pos = projRayToOriginAxis(ori, dir, curSelectedAxis);
    curScale = 1.0f + 0.1*(pos - beginProjPos);
	beginProjPos = pos;
    curScale *= curScale;
	float scaleV[] = {1,1,1};
	scaleV[curSelectedAxis] = curScale;
	getTransform()->scale(QVector3D(scaleV[0],scaleV[1],scaleV[2]));
}

