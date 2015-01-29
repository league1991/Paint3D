#pragma once
#include "Manipulator.h"
#define SCALE_M_AXIS_LENGTH        4.0f
#define SCALE_M_BOX_SIZE        15.0f
#define SCALE_M_DETECT_RADIUS    0.5f
class ScaleManipulator:public Manipulator
{
public:
    ScaleManipulator(void);
    ~ScaleManipulator(void);

    void draw(Camera& camera);
    char intersect(const QVector3D& rayOri, const QVector3D& rayDir);
    void beginManipulate(const QVector3D& rayOri, const QVector3D& rayDir, char axis);
    void goOnManipulate(const QVector3D& rayOri, const QVector3D& rayDir);
private:
	void setSize(float size)
	{
		axisLength = SCALE_M_AXIS_LENGTH * size;
		boxSize = SCALE_M_BOX_SIZE * size;
		detectRadius = SCALE_M_DETECT_RADIUS * size;
	}
	float axisLength;
	float boxSize;
	float detectRadius;

    float beginProjPos;            // 开始缩放时投影位置
    float curScale;                // 当前缩放比例
};

