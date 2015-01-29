#include "StdAfx.h"
#include "Camera.h"

#include <math.h>

Camera::Camera(void)
{
    resetCamera();
}


Camera::~Camera(void)
{
}

void Camera::computeViewParam()
{
	QVector3D xRotAxis = QVector3D(0,0,direction[1].z());
	QQuaternion xRotQuat = QQuaternion::fromAxisAndAngle(xRotAxis, deltaAngle[0]);
	direction[0] = xRotQuat.rotatedVector(direction[0]);
	QQuaternion yRotQuat = QQuaternion::fromAxisAndAngle(direction[0], deltaAngle[1]);
	QQuaternion rotQuat  = yRotQuat * xRotQuat;

	direction[1] = rotQuat.rotatedVector(direction[1]);
	direction[2] = rotQuat.rotatedVector(direction[2]);

	origin =  target - direction[2] * length;

	viewMatrix.setToIdentity();
	viewMatrix.lookAt(origin, target, direction[1]);

}
void Camera::computeAttachedTransfrom()
{
	float tanAngle = tan(CAMERA_FOV_Y_RAD / 2.0f);
	double h = tanAngle * length;
	double w = h * aspectRatio;
	float xOffset = w * attachOffset[0];
	float yOffset = h * attachOffset[1];
	float zOffset = length * attachOffset[2] + 1;
	QQuaternion localQuat = QQuaternion::fromAxisAndAngle(0,0,1,attachRotation);
	QVector3D trans;QQuaternion rot;
	this->getCameraTransform(trans, rot);
	for (int i = 0; i < attachedObjects.size(); ++i)
	{
		if (attachedObjects[i])
		{
			RenderableObject* obj = attachedObjects[i].data();
			obj->getTransform().setRotate(localQuat);
			obj->getTransform().rotate(rot, target);
			obj->getTransform().setTranslate(origin + direction[0] * xOffset + direction[1] * yOffset + direction[2] * zOffset);
			obj->getTransform().setScale(QVector3D(attachScale[0] * w, attachScale[1] * h, 1));
		}
	}
}

void Camera::computeProjParam()
{
	projMatrix.setToIdentity();
    if(projType == Camera::GLCAMERA_PERSP)
    {
		projMatrix.perspective(CAMERA_FOV_Y_DEGREE, aspectRatio, 0.2,100);
    }
    else
	{
		float tanAngle = tan(CAMERA_FOV_Y_RAD / 2.0f);
        double h = tanAngle * length;
		double w = h * aspectRatio;
		projMatrix.ortho(-w, w, -h, h, 0, 1000);
    }
}


void Camera::setScreenResolution( int x, int y )
{
    screenRes[0] = x; screenRes[1] = y;
	glViewport(0,0,x,y);
    aspectRatio = screenRes[0] / (double)screenRes[1];
	updateAll();
}


void Camera::rotateCamera( double dx, double dy )
{
    deltaAngle[0] -= dx / 5.0;
	deltaAngle[1] -= dy / 5.0;
	updateAll();
	deltaAngle[0] = deltaAngle[1] = 0.0f;
}

void Camera::moveCamera( double dx, double dy )
{
    dx /= 100; dy/= 100;
	target -= direction[0] * dx - direction[1] * dy;
	updateAll();
}

void Camera::zoomCamera( double dz )
{
	float newLength =  max(0.1, length- dz / 90.0f);
    deltaLength =  length - newLength;
	length = newLength;
	updateAll();
	deltaLength = 0;
}

void Camera::getRay( int x, int y, QVector3D& ori, QVector3D& dir )
{
    double xRatio = x / (double)screenRes[0] * 2.0 - 1.0;
    double yRatio = 1.0 - y / (double)screenRes[1] * 2.0;

    if (projType == Camera::GLCAMERA_ORTHO)
    {
        double h = tan(CAMERA_FOV_Y_RAD / 2.0f) * length;
        double w = h * aspectRatio;
        xRatio *= w;
        yRatio *= h;
		ori.setX(origin.x() + xRatio * direction[0].x() + yRatio * direction[1].x());
		ori.setY(origin.y() + xRatio * direction[0].y() + yRatio * direction[1].y());
		ori.setZ(origin.z() + xRatio * direction[0].z() + yRatio * direction[1].z());
		dir = direction[2];
    }
    else
    {
        double h = tan(CAMERA_FOV_Y_RAD / 2.0f);
        double w = h * aspectRatio;
        xRatio *= w;
        yRatio *= h;
        QVector3D vd = direction[2] + direction[0] * xRatio + direction[1] * yRatio;
        vd.normalize();
		ori = origin;
		dir = vd;     
    }
}

void Camera::setProjMode( ProjType type )
{
    projType = type;
    computeProjParam();
}

void Camera::resetCamera()
{
    projType = GLCAMERA_PERSP;
    screenRes[0] = 640;
    screenRes[1] = 480;
    aspectRatio = screenRes[1] / (double)screenRes[0];
	origin = QVector3D(10,0,0);
    target = QVector3D(0,0,0);
	direction[0] = QVector3D(0,1,0);
	direction[1] = QVector3D(0,0,1);
	direction[2] = QVector3D(-1,0,0);
    deltaAngle[0] = CAMERA_INIT_ALPHA_DEGREE;
    deltaAngle[1] = CAMERA_INIT_BETA_DEGREE;
	deltaLength = 0;
    length = 10.0f;
	attachOffset[0] = attachOffset[1] = attachOffset[2] = 0;
	attachRotation = 0;
	attachScale[0] = attachScale[1] = 1;

	updateAll();
}

void Camera::applyGLMatrices()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(viewMatrix.constData());
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(projMatrix.constData());
}


void Camera::setCenter( QVector3D& center, float distance /*= 10*/ )
{
	target = center;
	length = distance;
	updateAll();
}

QVector3D Camera::getWorldPos( float xRatio, float yRatio, float zRatio )const
{
	float xR = xRatio * 2.0 - 1.0;
	float yR = 1.0 - yRatio * 2.0;
	QVector3D ori, dir;

	if (projType == Camera::GLCAMERA_ORTHO)
	{
		double h = tan(CAMERA_FOV_Y_RAD / 2.0f) * length;
		double w = h * aspectRatio;
		xR *= w;
		yR *= h;
		ori.setX(origin.x() + xR * direction[0].x() + yR * direction[1].x());
		ori.setY(origin.y() + xR * direction[0].y() + yR * direction[1].y());
		ori.setZ(origin.z() + xR * direction[0].z() + yR * direction[1].z());
		dir = direction[2];
	}
	else
	{
		double h = tan(CAMERA_FOV_Y_RAD / 2.0f);
		double w = h * aspectRatio;
		xR *= w;
		yR *= h;
		QVector3D vd = direction[2] + direction[0] * xR + direction[1] * yR;
		ori = origin;
		dir = vd;     
	}
	dir.normalize();
// 	float z = depthRatio  / (1 - depthRatio);
// 	return ori + z * dir;
	return ori + zRatio * dir;
}

QVector3D Camera::getWorldNormalFromView( const QVector3D& viewNormal )
{
	return QVector3D(
		viewNormal.x() * direction[0].x() + viewNormal.y() * direction[1].x() + viewNormal.z() * -direction[2].x(),
		viewNormal.x() * direction[0].y() + viewNormal.y() * direction[1].y() + viewNormal.z() * -direction[2].y(),
		viewNormal.x() * direction[0].z() + viewNormal.y() * direction[1].z() + viewNormal.z() * -direction[2].z());
}

void Camera::getCameraTransform( QVector3D& trans, QQuaternion& rot )
{
	trans = origin+direction[2];

	float xAngle = atan2(direction[0].y(), direction[0].x()) / M_PI * 180.0f;
	QQuaternion rotX = QQuaternion::fromAxisAndAngle(0,0,1,xAngle);
	float x = -direction[2].z();
	float y = direction[1].z();
	float yAngle = atan2(y,x) / M_PI * 180.0f;
	QVector3D newX = rotX.rotatedVector(QVector3D(1,0,0));
	QQuaternion rotY = QQuaternion::fromAxisAndAngle(newX, yAngle);
	rot = rotY * rotX;

}

void Camera::getScreenSize(float&w, float&h)
{
	if (projType == Camera::GLCAMERA_ORTHO)
	{
		h = 2 * tan(CAMERA_FOV_Y_RAD / 2.0f) * length;
	}
	else
		h = 2 * tan(CAMERA_FOV_Y_RAD / 2.0f);
	w = h * aspectRatio;
}

void Camera::addAttachedObjects( const QWeakPointer<RenderableObject> obj )
{
	attachedObjects.push_back(obj);
	updateAll();
}

void Camera::clearAttachedObjects()
{
	attachedObjects.clear();
}

void Camera::setAttachObjectOffset( float x, float y, float z )
{
	attachOffset[0] = x;
	attachOffset[1] = y;
	attachOffset[2] = z;
	updateAll();
}

void Camera::setAttachObjectRotation( float angle )
{
	attachRotation = angle;
	updateAll();
}

void Camera::setAttachObjectScale( float x, float y )
{
	attachScale[0] = x;
	attachScale[1] = y;
	updateAll();
}

void Camera::updateAll()
{
	computeViewParam();
	computeProjParam();
	computeAttachedTransfrom();
	deltaAngle[0] = deltaAngle[1] = 0;
	deltaLength = 0;
}



