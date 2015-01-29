#include "stdafx.h"
#include "ObjReader.h"

ObjReader::ObjReader()
{
//	readingThread=NULL;
}
ObjReader::~ObjReader()
{
	this->clear();
}
bool ObjReader::readObjFile(const QString& fileName)
{	
	bool isPrimitiveBegin=true;				//the first vertex of each primitive
	Mesh*currPrimitive=NULL;		//pointer to current primitive
	int currentMtlIndex=-1;					//the index of current Material

	//由于maya和max导出的obj格式有一点
	//不同，maya格式在每个primitive的第一个v之前还会有一个g标记
	//而max没有，这里仅当读到vt标记之后isGeometryBegin才开启，
	//确保仅当读到表示物体名称的g标记才把物体名称记录下来
	int totalVertices=0,totalTexcoords=0,totalNormals=0;

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QTextStream in(&file);

	#define T2V3D(token)	QVector3D(token[1].toFloat(), token[2].toFloat(), token[3].toFloat())
	#define T2V2D(token)	QVector2D(token[1].toFloat(), token[2].toFloat())

	while(!in.atEnd())
	{
		QString line = in.readLine();
		QStringList tokens = line.split(" ", QString::SkipEmptyParts);

		if (tokens.count() == 0)
			continue;

		if(tokens[0] == "v")//vertex flag
		{
			//a new primitive must begin with "v"
			//when encounter the first "v" of a primitive 
			//isPrimitiveBegin is true
			if(isPrimitiveBegin)
			{
				if(currPrimitive)
				{
					meshes.append(QSharedPointer<Mesh>(currPrimitive));
					totalVertices	+=	currPrimitive->vertices.size();
					totalTexcoords	+=	currPrimitive->texcoords.size();
					totalNormals	+=	currPrimitive->normals.size();
				}
				currPrimitive    = new Mesh;
				isPrimitiveBegin = false;
			}	
			currPrimitive->vertices.append(T2V3D(tokens));//vertex coordinate
		}
		else
		{
			isPrimitiveBegin=true;
			if(tokens[0] == "vt")						//vertex texcoord flag
			{
				currPrimitive->texcoords.push_back(T2V2D(tokens));
			}
			else if(tokens[0] == "vn")					//vertex normal flag
			{
				currPrimitive->normals.push_back(T2V3D(tokens));
			}
			else if(tokens[0] == "g" && tokens.count() == 2)//primitive name flag
			{
				currPrimitive->setName(tokens[1]);
			}
			else if(tokens[0] == "usemtl")//material flag
			{
				QString mtlName = tokens[1];
				QVector<ObjMaterial>::iterator mtlIterator;
				ObjMaterial*newMaterial;
				currentMtlIndex=-1;
				//update current material
				for(mtlIterator=materials.begin();
					mtlIterator!=materials.end();
					mtlIterator++)
					//find if there is an existing material with the 
					//same name
				{
					currentMtlIndex++;
					if(mtlIterator->name == mtlName)
						break;//find
				}
				if(mtlIterator==materials.end())
					//can't find,so create a new material
				{
					ObjMaterial newMaterial;
					newMaterial.name=mtlName;
					materials.push_back(newMaterial);
					currentMtlIndex=materials.size()-1;
				}
				//qDebug()<<Materials[0].name<<endl;
			}
			else if(tokens[0] == "f")//face flag
			{
				ObjTriangle currTriangle;
				unsigned idArray[4][3];
				int ithVertex = 0;
				for(; ithVertex < tokens.count() - 1; ithVertex++)
				{
					QStringList idStr = tokens[ithVertex + 1].split("/",QString::SkipEmptyParts);

					// 如果缺少索引，报错
					if (idStr.size() != 3)
						return false;

					int id = idStr[0].toInt();					// v
					idArray[ithVertex][0] = id > 0 ? 
						id - 1 - totalVertices : currPrimitive->vertices.size() + id;
					id = idStr[1].toInt();						// vt
					idArray[ithVertex][1] = id > 0 ? 
						id - 1 - totalTexcoords : currPrimitive->texcoords.size() + id;
					id = idStr[2].toInt();						// vn
					idArray[ithVertex][2] = id > 0 ? 
						id - 1 - totalNormals : currPrimitive->normals.size() + id;
				}

				currTriangle.vertIndex[0] = idArray[0][0];
				currTriangle.vertIndex[1] = idArray[1][0];
				currTriangle.vertIndex[2] = idArray[2][0];
				currTriangle.texcoordIndex[0] = idArray[0][1];
				currTriangle.texcoordIndex[1] = idArray[1][1];
				currTriangle.texcoordIndex[2] = idArray[2][1];
				currTriangle.norIndex[0] = idArray[0][2];
				currTriangle.norIndex[1] = idArray[1][2];
				currTriangle.norIndex[2] = idArray[2][2];
				currTriangle.mtlIndex=currentMtlIndex;
				currPrimitive->faces.push_back(currTriangle);

				if (ithVertex == 4)
				{
					currTriangle.vertIndex[1] = idArray[2][0];
					currTriangle.vertIndex[2] = idArray[3][0];
					currTriangle.texcoordIndex[1] = idArray[2][1];
					currTriangle.texcoordIndex[2] = idArray[3][1];
					currTriangle.norIndex[1] = idArray[2][2];
					currTriangle.norIndex[2] = idArray[3][2];
					currPrimitive->faces.push_back(currTriangle);
				}
			}
		}
	}
	if (currPrimitive)
	{
		meshes.push_back(QSharedPointer<Mesh>(currPrimitive));
	}
	return true;
}
bool ObjReader::getWireArray(int nthPrimitive,GLWireArray*vB)
{
	if(nthPrimitive<0||nthPrimitive>=(int)this->meshes.size())
		return false;

	//vertices
	int nVertices=(int)this->meshes[nthPrimitive]->vertices.size();
	(*vB).vertices=new float[3*nVertices];
	for(int i=0;i<nVertices;i++)
	{
		(*vB).vertices[3*i]=this->meshes[nthPrimitive]->vertices[i].x();
		(*vB).vertices[3*i+1]=this->meshes[nthPrimitive]->vertices[i].y();
		(*vB).vertices[3*i+2]=this->meshes[nthPrimitive]->vertices[i].z();
	}

	//indices
	vB->nFaces=(int)this->meshes[nthPrimitive]->faces.size();
	int nVertexIndices=3*vB->nFaces;
	vB->indices=new unsigned int[nVertexIndices];
	for(int i=0;i<nVertexIndices/3;i++)
	{
		vB->indices[i*3]=this->meshes[nthPrimitive]->faces[i].vertIndex[0];
		vB->indices[i*3+1]=this->meshes[nthPrimitive]->faces[i].vertIndex[1];
		vB->indices[i*3+2]=this->meshes[nthPrimitive]->faces[i].vertIndex[2];
	}
	return true;
}
void ObjReader::showObj()
{
	QVector<QSharedPointer<Mesh>>::iterator itPrimitive;
	QVector<QVector3D>::iterator itFloat3;
	QVector<QVector2D>::iterator itFloat2;
	QVector<ObjTriangle>::iterator itTriangle;

	for(itPrimitive=meshes.begin();
		itPrimitive!=meshes.end();
		itPrimitive++)
	{
		qDebug()<<"\n##################"<<endl;
		qDebug()<<"Object Name:"+(*itPrimitive)->name<<endl;
		qDebug()<<"##################"<<endl;
		qDebug()<<"\nvertices:"<<endl;
		for(itFloat3=(*itPrimitive)->vertices.begin();
			itFloat3!=(*itPrimitive)->vertices.end();
			itFloat3++)
		{
			qDebug()<<itFloat3->x()<<'\t'
				<<itFloat3->y()<<'\t'
				<<itFloat3->z()<<'\t'<<endl;
		}
		qDebug()<<"\nnormals:"<<endl;
		for(itFloat3=(*itPrimitive)->normals.begin();
			itFloat3!=(*itPrimitive)->normals.end();
			itFloat3++)
		{
			qDebug()<<itFloat3->x()<<'\t'
				<<itFloat3->y()<<'\t'
				<<itFloat3->z()<<'\t'<<endl;
		}
		qDebug()<<"\ntexcoords:"<<endl;
		for(itFloat2=(*itPrimitive)->texcoords.begin();
			itFloat2!=(*itPrimitive)->texcoords.end();
			itFloat2++)
		{
			qDebug()<<itFloat2->x()<<'\t'
				<<itFloat2->y()<<'\t'<<endl;
		}
		qDebug()<<"\nfaces:"<<endl;
		for(itTriangle=(*itPrimitive)->faces.begin();
			itTriangle!=(*itPrimitive)->faces.end();
			itTriangle++)
		{
			for(int i=0;i<3;i++)
			{
				qDebug()<<itTriangle->vertIndex[i]<<'/'
					<<itTriangle->texcoordIndex[i]<<'/'
					<<itTriangle->norIndex[i]<<'\t';
			}
			if (itTriangle->mtlIndex != 0xffffffff)
			{
				qDebug()<<"materialID:"<<itTriangle->mtlIndex<<"  material:"
					<<materials[itTriangle->mtlIndex].name<<endl;
			}
		}

	}
}
void ObjReader::clear()
{
	meshes.clear();
	materials.clear();
}

void ObjReader::getCoordArray(float*&pArray,unsigned int&nFloats,unsigned nthPrimitive,coordType type)
{
	unsigned int num;
	switch(type)
	{
	case VERTCOORDS:
		num=meshes[nthPrimitive]->vertices.size();//点数
		nFloats=num*3;//坐标数
		pArray=new float[nFloats];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[3*i]=meshes[nthPrimitive]->vertices[i].x();
			pArray[3*i+1]=meshes[nthPrimitive]->vertices[i].y();
			pArray[3*i+2]=meshes[nthPrimitive]->vertices[i].z();
		}
		break;
	case TEXCOORDS:
		num=meshes[nthPrimitive]->texcoords.size();//贴图坐标数
		nFloats=num*2;//坐标数
		pArray=new float[nFloats];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[2*i]=meshes[nthPrimitive]->texcoords[i].x();
			pArray[2*i+1]=meshes[nthPrimitive]->texcoords[i].y();
		}
		break;
	case NORCOORDS:
		num=meshes[nthPrimitive]->normals.size();//法向量数
		nFloats=num*3;//坐标数
		pArray=new float[nFloats];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[3*i]=meshes[nthPrimitive]->normals[i].x();
			pArray[3*i+1]=meshes[nthPrimitive]->normals[i].y();
			pArray[3*i+2]=meshes[nthPrimitive]->normals[i].z();
		}
		break;
	default:
		nFloats=0;
		pArray=NULL;
	}
	return;
}
void ObjReader::getIndexArray(unsigned int*&pArray,unsigned int &nInts,
				   unsigned nthPrimitive,indexType type)
{
	unsigned int num;
	switch(type)
	{
	case VERTINDICES:
		num=meshes[nthPrimitive]->faces.size();//面数
		nInts=num*3;//索引数
		pArray=new unsigned int[nInts];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[i*3]=meshes[nthPrimitive]->faces[i].vertIndex[0];//第一个顶点的索引
			pArray[i*3+1]=meshes[nthPrimitive]->faces[i].vertIndex[1];//第二个顶点的索引
			pArray[i*3+2]=meshes[nthPrimitive]->faces[i].vertIndex[2];//第三个顶点的索引
		}
		break;
	case TEXINDICES:
		num=meshes[nthPrimitive]->faces.size();//面数
		nInts=num*3;//索引数
		pArray=new unsigned int[nInts];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[i*3]=meshes[nthPrimitive]->faces[i].texcoordIndex[0];//第一个顶点的索引
			pArray[i*3+1]=meshes[nthPrimitive]->faces[i].texcoordIndex[1];//第二个顶点的索引
			pArray[i*3+2]=meshes[nthPrimitive]->faces[i].texcoordIndex[2];//第三个顶点的索引
		}
		break;
	case NORINDICES:
		num=meshes[nthPrimitive]->faces.size();//面数
		nInts=num*3;//索引数
		pArray=new unsigned int[nInts];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[i*3]=meshes[nthPrimitive]->faces[i].norIndex[0];//第一个顶点的索引
			pArray[i*3+1]=meshes[nthPrimitive]->faces[i].norIndex[1];//第二个顶点的索引
			pArray[i*3+2]=meshes[nthPrimitive]->faces[i].norIndex[2];//第三个顶点的索引
		}
		break;
	}
	return;
}/*
void ObjReader::fillPrimitive(unsigned int nthPrimitive, Primitive&pri)
{
	float*vertices,*texcoords,*normals;
	unsigned int*vertIndices,*texcoordIndices,*normalIndices;
	unsigned int vSize,tSize,nSize;//对应数组的大小
	unsigned int nIndices;
	getCoordArray(vertices,vSize,nthPrimitive,ObjReader::VERTCOORDS);
	getCoordArray(texcoords,tSize,nthPrimitive,ObjReader::TEXCOORDS);
	getCoordArray(normals,nSize,nthPrimitive,ObjReader::NORCOORDS);
	getIndexArray(vertIndices,nIndices,nthPrimitive,ObjReader::VERTINDICES);
	getIndexArray(texcoordIndices,nIndices,nthPrimitive,ObjReader::TEXINDICES);
	getIndexArray(normalIndices,nIndices,nthPrimitive,ObjReader::NORINDICES);
	pri.vertices=vertices;
	pri.texcoords=texcoords;
	pri.normals=normals;
	pri.vertexIndices=vertIndices;
	pri.normalIndices=normalIndices;
	pri.texCoordIndices=texcoordIndices;
	pri.nVertices=vSize/3;
	pri.nTexcoords=tSize/2;
	pri.nNormals=nSize/3;
	pri.nFaces=nIndices/3;

	pri.mtlIndices=new unsigned int[pri.nFaces];
	for(unsigned int i=0;i<pri.nFaces;i++)
	{
		pri.mtlIndices[i]=Primitives[nthPrimitive].faces[i].mtlIndex;
	}
}*/

bool ObjReader::readMtlFile(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QTextStream in(&file);

	while(!in.atEnd())
	{
		QString flag;
		char subFlag[20];
		in >> flag;

		if(flag == "newmtl")
		{
			ObjMaterial newMtl;
			in >> newMtl.name;
			while(1)
			{
				in >> flag;
				//漫反射颜色对应的flag
				if(flag == "Kd")
				{
					float v[3];
					in  >>v[0] >> v[1] >> v[2];
					newMtl.diffuse = QVector3D(v[0], v[1], v[2]);
					break;
				}
			}
			materials.push_back(newMtl);
		}
	}
	return true;
} 
void ObjReader::showMtl()
{
	QVector<ObjMaterial>::iterator p;
	for(p=materials.begin();p!=materials.end();p++)
	{
		qDebug()<<"\n##################"<<endl;
		qDebug()<<"Material Name:"+p->name<<endl;
		qDebug()<<"##################"<<endl;
		qDebug()<<"diffuse color: "
			<<p->diffuse.x()<<'\t'
			<<p->diffuse.y()<<'\t'
			<<p->diffuse.z()<<endl;
	}
}

bool ObjReader::read(const QString& fileName)
{
	QString fileName0 = fileName.left(fileName.length() - 3);
	QString objName = fileName0 + QString("obj");
	QString mtlName = fileName0 + QString("mtl");

 	bool mtlRes = readMtlFile(mtlName);
 	bool objRes = readObjFile(objName);
	return /*mtlRes &&*/ objRes;
}