#pragma once
#include "stdafx.h"
#include <atlstr.h>
//#include "Primitive.h"
//#include "DisplaySystem.h"

struct ObjMaterial
{
	QString name;				//name of material
	QVector3D diffuse;			//漫反射颜色
};

class ObjReader
{
public:
	static enum coordType{VERTCOORDS,TEXCOORDS,NORCOORDS};
	static enum indexType{VERTINDICES,TEXINDICES,NORINDICES};

	ObjReader();
	~ObjReader();

	QVector<QSharedPointer<Mesh>>	meshes;
	QVector<ObjMaterial>			materials;

	//读取obj以及连带的mtl文件
	//obj和mtl文件必须在同一目录
	bool read(const QString& fileName);
	QSharedPointer<Mesh> getMesh(int ithMesh){return meshes[ithMesh];}
	int  getNumMeshes(){return meshes.size();}

	//return vertex buffer
	bool getWireArray(int primitive,GLWireArray*vB);

	void showObj();
	void showMtl();
	void clear();

private:
	//只读取obj文件
	bool readObjFile(const QString& fileName);
	//只读取mtl文件
	bool readMtlFile(const QString& fileName);
	//把vector类型的坐标数组转化成float类型数组
	//pArray为数组指针，nFloats为数组元素个数
	//type有3种取值：OBJ_VERTICES,OBJ_TEXCOORDS,OBJ_NORMALS
	//表示返回对应类型的数组
	//nthPrimitive指定返回哪一个物体的数组
	void getCoordArray(float*&pArray,unsigned int&nFloats,
		unsigned nthPrimitive,coordType type);
	//把vector类型的索引数组转化成unsigned int类型数组
	//参数与getCoordArray类似，nInt是索引数，为面数*3
	//注意索引值的3倍才是对应顶点，法向量数组的下标
	//注意索引值的2倍才是对应贴图坐标数组的下标
	void getIndexArray(unsigned int*&pArray,unsigned int &nInts,
		unsigned nthPrimitive,indexType type);

};
