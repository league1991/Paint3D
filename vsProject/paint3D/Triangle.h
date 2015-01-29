#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "DifferentialGeometry.h"

//选择哪种办法求三角形的交点
//试验表明，方法A比较快
#define ISECTMETHOD_A 1
#define ISECTMETHOD_B 2
#define INTERSECTION_METHOD ISECTMETHOD_A

class Triangle
{
public:
	Vector3 point1,point2,point3;
	Vector2 texCoord1,texCoord2,texCoord3;
	Vector3 normal1,normal2,normal3;
	unsigned int mtlId;

	Triangle(
		const Vector3&ipoint1,
		const Vector3&ipoint2,
		const Vector3&ipoint3,
		const Vector2&itexCoord1,
		const Vector2&itexCoord2,
		const Vector2&itexCoord3,
		const Vector3&inormal1,
		const Vector3&inormal2,
		const Vector3&inormal3,
		unsigned int mtlId
		);

	Triangle(void);
	virtual ~Triangle(void);

	float area();

	//以下两个是与光线求交的函数
	//-----------------------
	//intersectTest计算相交的时候的光线t值,如果不相交，
	//返回光线的M_INF_BIG，表示在无穷远处相交
	//测试的时候只需检测返回值是否小于tMax即可
	//-----------------------
	//intersect假定已经三角形与光线必有交点，
	//求出交点的DifferentialGeometry
	//-----------------------
	//多个三角形求交的时候，先循环调用intersectTest函数
	//得到离光线最近而又相交的多边形，再调用intersect函数
	//求出交点的DifferentialGeometry,并更新光线的tMax值
	//DifferentialGeometry在创建的时候不作初始化
	//然后输入intersect函数初始化各个变量
	float intersectTest(const Ray&r);
	void intersect(Ray&r,DifferentialGeometry&DG);
	//由uv参数获得表面一点及其法向量
	//用于灯光的采样
	void getPoint(float u,float v,
		Vector3&position,Vector3&normal,Vector2&texCoord);

	//在openGL中画出三角形，调试时用
	void draw(bool showNormal=false,bool fillMode=false);
	//输出三角形数据，调试时用
	void showCoords();
	//输出三角形顶点数据，调试时用
	void showVertexCoords();
	//返回质心
	Vector3 getCentroid();
};


