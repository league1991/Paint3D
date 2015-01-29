#pragma once
#include "Manipulator.h"
#define ROT_M_BALL_RADIUS        3
#define ROT_M_BALL_SEGMENTS      32
#define ROT_M_DETECT_WIDTH       0.5f
#define ROT_M_BALL_WIDTH	     0.1
class RotateManipulator:public Manipulator
{
public:
    RotateManipulator(void);
    ~RotateManipulator(void);

    void draw(Camera& camera);
    char intersect(const QVector3D& rayOri, const QVector3D& rayDir);
    void beginManipulate(const QVector3D& rayOri, const QVector3D& rayDir, char axis);
    void goOnManipulate(const QVector3D& rayOri, const QVector3D& rayDir);

private:

	void setSize(float size)
	{
		ballRadius = ROT_M_BALL_RADIUS * size;
		detectWidth = ROT_M_DETECT_WIDTH * size;
		ballWidth = ROT_M_BALL_WIDTH * size;
	}
	float ballRadius;
	float detectWidth;
	float ballWidth;

    float beginProjPos;
    QVector3D rotLineOrigin;
    QVector3D rotLineTangent;
	QMatrix4x4 beginMatrix;
};

