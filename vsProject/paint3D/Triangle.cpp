#include "stdafx.h"
#include "MathValues.h"
#include "Vector3.h"
#include "Triangle.h"
#include <gl/glut.h>
Triangle::Triangle(void)
{
// 	point1=Vector3(0.0f,0.0f,0.0f);
// 	point2=Vector3(1.0f,0.0f,0.0f);
// 	point3=Vector3(0.0f,1.0f,0.0f);
// 	texCoord1=Vector2(0.0f,0.0f);
// 	texCoord2=Vector2(1.0f,0.0f);
// 	texCoord3=Vector2(0.0f,1.0f);
// 	normal1=normal2=normal3=Vector3(0.0f,0.0f,1.0f);
// 	mtlId=0;
}
Triangle::Triangle(
		 const Vector3&ipoint1,
		 const Vector3&ipoint2,
		 const Vector3&ipoint3,
		 const Vector2&itexCoord1,
		 const Vector2&itexCoord2,
		 const Vector2&itexCoord3,
		 const Vector3&inormal1,
		 const Vector3&inormal2,
		 const Vector3&inormal3,
		 unsigned int imtlId
		 ):point1(ipoint1),point2(ipoint2),point3(ipoint3),
		 texCoord1(itexCoord1),texCoord2(itexCoord2),texCoord3(itexCoord3),
		 normal1(inormal1),normal2(inormal2),normal3(inormal3),
		 mtlId(imtlId)
{}

Triangle::~Triangle(void)
{
}
float Triangle::area()
{
	Vector3 p21=point2-point1;
	Vector3 p31=point3-point1;
	return abs(0.5f*p21.cross(p31).length());
}
//求交函数,注意光线的方向向量必须事先单位化
#if INTERSECTION_METHOD==ISECTMETHOD_A
float Triangle::intersectTest(const Ray&r)
{
	Vector3 E1=point2-point1;
	Vector3 E2=point3-point1;
	Vector3 T=r.point-point1;
	Vector3 S1=r.direction.cross(E2);
	Vector3 S2=T.cross(E1);
	float S1E1=S1.dot(E1);

	if(abs(S1E1)<1e-9f)		//认为分母为0 ，故求交失败
		return M_INF_BIG;

	float t=S2.dot(E2)/S1E1;
	if(t<r.tMin||t>r.tMax)	
		//t超出光线范围，求交失败,注意当t=tMax的时候有可能求交成功
		return M_INF_BIG;

	float b1=S1.dot(T)/S1E1;
	if(b1<0.0f)		//b1超出范围，求交失败
		return M_INF_BIG;

	float b2=S2.dot(r.direction)/S1E1;
	if(b2<0.0f)		//b2超出范围，求交失败
		return M_INF_BIG;
	if(b1+b2>1.0f)
		return M_INF_BIG;
	//至此求交成功
	return t;
}

#elif INTERSECTION_METHOD==ISECTMETHOD_B
//第二种求交办法，由于多次运用叉乘，故速度比第一种稍慢
float Triangle::intersectTest(Ray&r)
{
	Vector3 p2p1 = point2 - point1;
	Vector3 p3p2 = point3 - point2;
	Vector3 p1p3 = point1 - point3;
	Vector3 n = p2p1.cross(p1p3);
	float t = (point1 - r.point).dot(n) / (r.direction.dot(n));
	if(t<=r.tMin||t>r.tMax)	
		return M_INF_BIG;
	Vector3 x = r.point + t*r.direction;
	float t1 = (x - point1).cross(p2p1).dot(n);
	float t2 = (x - point2).cross(p3p2).dot(n);
	//比较计算结果是否异号,异号说明出现了点在三角形外的情况
	if( (*((int*)&t1) ^ *((int*)&t2)) & 0x80000000)
		return M_INF_BIG;
	t1 = (x - point3).cross(p1p3).dot(n);
	if( (*((int*)&t1) ^ *((int*)&t2)) & 0x80000000)
		return M_INF_BIG;
	return t;
}
#endif
void Triangle::intersect(Ray&r,DifferentialGeometry&DG)
{
	Vector3 E1=point2-point1;
	Vector3 E2=point3-point1;
	Vector3 T=r.point-point1;
	Vector3 S1=r.direction.cross(E2);
	Vector3 S2=T.cross(E1);
	float S1E1=S1.dot(E1);
	float t=S2.dot(E2)/S1E1;
	float b1=S1.dot(T)/S1E1;
	float b2=S2.dot(r.direction)/S1E1;
	r.tMax=t;
	DG.position=(1.0f-b1-b2)*point1+b1*point2+b2*point3;
	DG.normal=(1.0f-b1-b2)*normal1+b1*normal2+b2*normal3;
	DG.texCoord=(1.0f-b1-b2)*texCoord1+b1*texCoord2+b2*texCoord3;
	DG.normal.normalize();
	DG.mtlId=this->mtlId;

	//算出副切线
	DG.bitangent=DG.normal.cross(DG.rayDir);
	if(DG.bitangent.lengthSquared()<1e-4f)
	{
		//认为是零向量，此时normal和rayDir或者共线，或者至少一个为0
		if(abs(DG.normal.x)>abs(DG.normal.y))
		{
			DG.bitangent=Vector3(-DG.normal.z,0.0f,DG.normal.x);
		}
		else
		{
			DG.bitangent=Vector3(0.0f,DG.normal.z,-DG.normal.y);
		}
	}
	//算出主切线
	DG.bitangent.normalize();
	DG.tangent=DG.bitangent.cross(DG.normal);
	DG.rayDir=-1*r.direction;

	//算出dpdu dpdv dndu dndv
	float a=texCoord3.x-texCoord1.x;
	float b=texCoord3.y-texCoord1.y;
	float c=texCoord2.x-texCoord1.x;
	float d=texCoord2.y-texCoord1.y;
	float det=a*d-b*c;
	if(abs(det)>1e-5f)
	{
		float invDet=1.0f/det;
		DG.dpdu=(d*E2-b*E1)*invDet;
		DG.dpdv=(a*E1-c*E2)*invDet;
		Vector3 N1=normal2-normal1;
		Vector3 N2=normal3-normal1;
		DG.dndu=(d*N2-b*N1)*invDet;
		DG.dndv=(a*N1-c*N2)*invDet;
	}
	else
	{
		DG.dpdu=DG.dpdv=DG.dndu=DG.dndv=Vector3(0.0f,0.0f,0.0f);
	}
}
void Triangle::getPoint(float b1,float b2,
						Vector3&position,
						Vector3&normal,
						Vector2&texCoord)
{
	position=(1.0f-b1-b2)*point1+b1*point2+b2*point3;
	normal=(1.0f-b1-b2)*normal1+b1*normal2+b2*normal3;
	texCoord=(1.0f-b1-b2)*texCoord1+b1*texCoord2+b2*texCoord3;
}
void Triangle::draw(bool showNormal,bool fillMode)
{
	if(fillMode)
		glPolygonMode(GL_FRONT,GL_FILL);
	glBegin(GL_TRIANGLES);
//	glColor3f(texCoord1.x,texCoord1.y,0.2f);
	glVertex3f(point1.x,point1.y,point1.z);
//	glColor3f(texCoord2.x,texCoord2.y,0.2f);
	glVertex3f(point2.x,point2.y,point2.z);
//	glColor3f(texCoord3.x,texCoord3.y,0.2f);
	glVertex3f(point3.x,point3.y,point3.z);
	glEnd();
	if(showNormal)
	{
		Vector3 norEnd1,norEnd2,norEnd3;
		norEnd1=point1+normal1*0.2f;
		norEnd2=point2+normal2*0.2f;
		norEnd3=point3+normal3*0.2f;
		glBegin(GL_LINES);
		glVertex3f(point1.x,point1.y,point1.z);
		glVertex3f(norEnd1.x,norEnd1.y,norEnd1.z);
		glVertex3f(point2.x,point2.y,point2.z);
		glVertex3f(norEnd2.x,norEnd2.y,norEnd2.z);
		glVertex3f(point3.x,point3.y,point3.z);
		glVertex3f(norEnd3.x,norEnd3.y,norEnd3.z);
		glEnd();
	}
}
void Triangle::showCoords()
{
	cout<<"#####Triangle#####"<<endl;
	cout<<"vertices"<<endl;
	point1.showCoords();
	point2.showCoords();
	point3.showCoords();
	cout<<"normals"<<endl;
	normal1.showCoords();
	normal2.showCoords();
	normal3.showCoords();
	cout<<"texcoords"<<endl;
	texCoord1.showCoords();
	texCoord2.showCoords();
	texCoord3.showCoords();
	cout<<"mtlID"<<mtlId<<endl;
}
Vector3 Triangle::getCentroid()
{
	return (point1+point2+point3)/3.0f;
}

void Triangle::showVertexCoords()
{
	cout<<"vertices"<<endl;
	point1.showCoords();
	point2.showCoords();
	point3.showCoords();
}