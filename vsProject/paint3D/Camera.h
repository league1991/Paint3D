#pragma once
#include <math.h>

#define CAMERA_INIT_ALPHA_DEGREE                     45.0f
#define CAMERA_INIT_BETA_DEGREE                      -45.0f

#define CAMERA_FOV_Y_DEGREE 60
#define CAMERA_FOV_Y_RAD    CAMERA_FOV_Y_DEGREE * M_PI / 180.0f
class Camera
{
public:
    Camera(void);
    ~Camera(void);

    enum ProjType{GLCAMERA_ORTHO = 0, GLCAMERA_PERSP = 1};
    void resetCamera();
    void setScreenResolution(int x, int y);
    void getScreenResolution(int&x, int&y){x = screenRes[0]; y = screenRes[1];}
    void setProjMode(ProjType type);

	// 以下3个函数只是设置摄像机参数，没有把这些参数应用到openGL
    void rotateCamera(double dx, double dy);
    void moveCamera(double dx, double dy);
    void zoomCamera(double dz);

    void applyGLMatrices();

    void getRay( int x, int y, QVector3D& ori, QVector3D& dir );

    QMatrix4x4 getViewMatrix(){return viewMatrix;}
    QMatrix4x4 getProjMatrix(){return projMatrix;}

	// 根据屏幕一点的位置以及深度值(zRatio)计算出世界空间的点
	QVector3D  getWorldPos(float xRatio, float yRatio, float zRatio)const;
	// 把关于观察坐标系的法线变换到世界坐标
	QVector3D  getWorldNormalFromView(const QVector3D& viewNormal);
	// 单位化屏幕坐标
	QVector2D  getScreenRatio(const QVector2D& screenCoord)
	{ return QVector2D(screenCoord.x() / screenRes[0], screenCoord.y() / screenRes[1]);	}

    void setCenter(QVector3D& center, float length = 10);

	const QVector3D& getOrigin(){return origin;}

	void addAttachedObjects(const QWeakPointer<RenderableObject> obj);
	void clearAttachedObjects();
	void setAttachObjectOffset(float x, float y, float z);
	void setAttachObjectRotation(float angle);
	void setAttachObjectScale(float x, float y);

	void getCameraTransform(QVector3D& trans, QQuaternion& rot);
	void getScreenSize(float&w, float&h);
private:

	void updateAll();
    void computeViewParam();
    void computeProjParam();
	void computeAttachedTransfrom();
	// 以下为投影参数，一旦改变这些参数需要重新设置openGL投影矩阵
    ProjType projType;
    int     screenRes[2];
    double  aspectRatio;        // width / height

    // 以下为观察参数，一旦改变需要重新设置观察矩阵
    QVector3D target;
    double  deltaAngle[2];
	double  deltaLength;
    double  length;

    // 由上面3个成员算出,因此任意一个成员改变，都要重新算一次
    QVector3D direction[3];
    QVector3D origin;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;

	QVector<QWeakPointer<RenderableObject>> attachedObjects;
	float attachOffset[3];
	float attachRotation;
	float attachScale[2];
};

