#pragma once
#include "RenderableObject.h"
#include <math.h>

#define M_SIZE_FACTOR  0.08
#define M_CHECK_OBJECT(obj) if(!obj){return;}

class Manipulator
{
public:
	enum ManipulateAxis{M_AXIS_X = 0x1, M_AXIS_Y = 0x1 << 1, M_AXIS_Z = 0x1 << 2};
    Manipulator(void);
    virtual ~Manipulator(void);

    // 判断光线与操纵器是否相交，返回相交的轴，如果没有交点，返回-1
    virtual char intersect(const QVector3D& rayOri, const QVector3D& rayDir) = 0;

    // 选择高亮显示的轴
    void selectAxis(char axis){curSelectedAxis = axis;}
    
    // 把操纵器附着到一个物体上，之后会对这个物体操纵
	virtual void captureObject( QWeakPointer<RenderableObject> object );
	virtual void releaseObject();

    // 以下3个函数用于实现操纵器的变换操作
    // 当左键被按下，鼠标指针跟操纵器的一个轴向相交时，调用beginManipulate
    // 当鼠标拖动时，调用 goOnManipulate
    // 当松开鼠标时，调用 endManipulate
    virtual void beginManipulate(const QVector3D& rayOri, const QVector3D& rayDir, char axis) = 0;
    virtual void goOnManipulate(const QVector3D& rayOri, const QVector3D& rayDir) = 0;
    virtual void endManipulate();

    virtual void draw(Camera& camera) = 0;
    bool isManipulating(){return isWorking;}
    //QMatrix4x4 getTransformMatrix(){return m_transformMatrix;}
    QWeakPointer<RenderableObject> getCurObject(){return curObject;}

	inline ObjectTransform& getOldTransform(){return oldTransform;}
	inline ObjectTransform& getNewTransform(){return newTransform;}
protected:
	virtual void setSize(float size) = 0;

	inline ObjectTransform* getTransform(){return &(curObject.data()->getTransform());}
    // 设置被操作物体的坐标变换矩阵
    void setObjectTransformMatrix();

    // 与位于原点的圆柱体求交,用于检查鼠标是否位于某一轴向上面
    bool intersectOriginCylinder( 
		const QVector3D& rayOri, const QVector3D& rayDir, 
		const char axis, const float radius, 
		const float start, const float end, float& t );

    // 计算光线与某一个坐标轴公垂线在该轴上的垂足
    float projRayToOriginAxis( 
		const QVector3D& rayOri, const QVector3D& rayDir, 
		const char axis);

    // 计算光线与某一个直线的公垂线在该直线上的垂足
    float projRayToLine(
		const QVector3D& rayOri, const QVector3D& rayDir, 
		const QVector3D& lineOri, const QVector3D& lineDir);

    // 与位于原点的圆盘求交,用于检查鼠标是否位于某一轴向上面
	bool intersectOriginDisk( 
		const QVector3D& rayOri, const QVector3D& rayDir, 
		const char axis, const float innerRadius, 
		const float outerRadius, float& t );

	void transformRayToLocal(QVector3D& ori, QVector3D& dir);


	char  curSelectedAxis;		// 记录当前鼠标指针所在的坐标轴
	bool  isWorking;            // 记录是否正在被操纵
	QWeakPointer<RenderableObject> curObject;   // 被操纵的物体
	ObjectTransform oldTransform, newTransform; // 操纵前后物体的变换参数

	static const char otherAxis[3][2];
	static const float cosTable[13];
	static const float sinTable[13];
	static const float axisColor[3][3];
};

