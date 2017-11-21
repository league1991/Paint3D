#include "stdafx.h"
#include "mesh.h"

QSharedPointer<QGLShaderProgram> Mesh::geometryProgram;
QSharedPointer<QGLShaderProgram> Mesh::appearProgram;

bool Mesh::isWireFrameEnabled = true;

Mesh::Mesh()
{
	type = OBJ_MESH;

	if (!geometryProgram)
	{
		geometryProgram = QSharedPointer<QGLShaderProgram>(new QGLShaderProgram);
		geometryProgram->addShaderFromSourceFile(QGLShader::Vertex,   "fboShaderVS.glsl");
		geometryProgram->addShaderFromSourceFile(QGLShader::Fragment, "fboShaderFS.glsl");
		geometryProgram->bindAttributeLocation("vertexV", PROGRAM_VERTEX_ATTRIBUTE);
		geometryProgram->bindAttributeLocation("normalV", PROGRAM_NORMAL_ATTRIBUTE);
		geometryProgram->bindAttributeLocation("texCoordV", PROGRAM_TEXCOORD_ATTRIBUTE);
		geometryProgram->link();
		geometryProgram->bind();
	}
	if (!appearProgram)
	{
		appearProgram = QSharedPointer<QGLShaderProgram>(new QGLShaderProgram);
		appearProgram->addShaderFromSourceFile(QGLShader::Vertex,   "sceneShaderVS.glsl");
		appearProgram->addShaderFromSourceFile(QGLShader::Fragment, "sceneShaderFS.glsl");
		appearProgram->bindAttributeLocation("vertexV", PROGRAM_VERTEX_ATTRIBUTE);
		appearProgram->bindAttributeLocation("normalV", PROGRAM_NORMAL_ATTRIBUTE);
		appearProgram->bindAttributeLocation("tangentV", PROGRAM_TANGENT_ATTRIBUTE);
		appearProgram->bindAttributeLocation("bitangentV", PROGRAM_BITANGENT_ATTRIBUTE);
		appearProgram->bindAttributeLocation("texCoordV", PROGRAM_TEXCOORD_ATTRIBUTE);
		appearProgram->link();
		appearProgram->bind();
	}
}


void Mesh::drawGeometry()
{
	if (!isObjVisible)
		return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(transform.getTransformMatrix().constData());

	geometryProgram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
	geometryProgram->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);
	geometryProgram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);

	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_NORMAL_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_TEXCOORD_ATTRIBUTE);

	// 表示当前表面厚度的纹理
	int texRegBase = GL_TEXTURE0 + MESH_TEXTURE_REGISTER_OFFSET;
	int texRegOffset = MESH_TEXTURE_REGISTER_OFFSET;
	GLContext::instance()->getQGLFunctions()->glActiveTexture(texRegBase + 3);								// 分配一个纹理寄存器
	glBindTexture(GL_TEXTURE_2D, canvas.getGLBaseThicknessTexObj());		// 把贴图对象绑定到寄存器
	geometryProgram->setUniformValue("baseThickTex", texRegOffset + 3);	// 把寄存器绑定到着色器变量

	// 顶点各属性数组
	glBuffer[0].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_VERTEX_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBuffer[1].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBuffer[4].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_TEXCOORD_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, glVertexBuffer.size());

	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_NORMAL_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_TEXCOORD_ATTRIBUTE);

	glPopMatrix();
}

void Mesh::buildGLArrays()
{
	int nElements = faces.size() * 3;
	glVertexBuffer.reserve(nElements);
	glNormalBuffer.reserve(nElements);
	glTangentBuffer.reserve(nElements);
	glBitangentBuffer.reserve(nElements);
	glTexcoordBuffer.reserve(nElements);

	for (int ithTri = 0; ithTri < faces.size(); ++ithTri)
	{
		ObjTriangle& f = faces[ithTri];

		QVector3D& v0 = vertices[f.vertIndex[0]];
		QVector3D& v1 = vertices[f.vertIndex[1]];
		QVector3D& v2 = vertices[f.vertIndex[2]];
		QVector2D& t0 = texcoords[f.texcoordIndex[0]];
		QVector2D& t1 = texcoords[f.texcoordIndex[1]];
		QVector2D& t2 = texcoords[f.texcoordIndex[2]];

		QVector2D dt1 = t1 - t0;
		QVector2D dt2 = t2 - t0;
		QVector3D dv1 = v1 - v0;
		QVector3D dv2 = v2 - v0;

		qreal det = dt1.x() * dt2.y() - dt2.x() * dt1.y();
		QVector3D tangent   = (dt2.y() * dv1 - dt1.y() * dv2) / det;
		QVector3D bitangent = (dt1.x() * dv2 - dt2.x() * dv1) / det;

		for (int ithPoint = 0; ithPoint < 3; ++ithPoint)
		{
			glVertexBuffer.push_back(vertices[f.vertIndex[ithPoint]]);
			glNormalBuffer.push_back( normals[f.norIndex[ithPoint]] );
			glTangentBuffer.push_back(tangent);
			glBitangentBuffer.push_back(bitangent);
			QVector2D& uv = texcoords[f.texcoordIndex[ithPoint]];
			glTexcoordBuffer.push_back(QVector4D(uv.x(), uv.y(), float(objectID), float(ithTri)));
		}
	}
}

void Mesh::releaseGLArrays()
{
	glVertexBuffer.clear();
	glNormalBuffer.clear();
	glTangentBuffer.clear();
	glBitangentBuffer.clear();
	glTexcoordBuffer.clear();
}

void Mesh::init()
{
	optimizeArrays();
	if (!glVertexBuffer.size())
		buildGLArrays();

	QVector<QVector3D>* buf[] = {
		&glVertexBuffer,
		&glNormalBuffer,
		&glTangentBuffer,
		&glBitangentBuffer
	};

	for (int i = 0; i < NUM_GL_VALUE_BUFFERS - 1; ++i)
	{
		glBuffer[i].create();
		glBuffer[i].bind();
		glBuffer[i].setUsagePattern(QGLBuffer::StaticDraw);
		glBuffer[i].allocate(buf[i]->constData(), sizeof(QVector3D) * buf[i]->size());
	}

	glBuffer[4].create();
	glBuffer[4].bind();
	glBuffer[4].setUsagePattern(QGLBuffer::StaticDraw);
	glBuffer[4].allocate(glTexcoordBuffer.constData(), sizeof(QVector4D) * glTexcoordBuffer.size());

	glBuffer[5] = QGLBuffer(QGLBuffer::IndexBuffer);
	glBuffer[5].setUsagePattern(QGLBuffer::DynamicDraw);
	glBuffer[5].create();
	glBuffer[5].bind();

	buildLocalBBox();
	canvas.init(vertices, normals, texcoords, faces);
}

void Mesh::releaseGLBuffer()
{
	if (vertices.size())
	{
		for (int i = 0; i < NUM_GL_VALUE_BUFFERS; ++i)
		{
			glBuffer[i].release();
		}
		releaseGLArrays();
	}
}

Mesh::~Mesh()
{
	qDebug() << "delete mesh " << objectID << endl;
	releaseGLBuffer();
	canvas.release();
}


void Mesh::buildLocalBBox()
{
	if (vertices.size())
	{
		localBBox.pMax = localBBox.pMin = vertices[0];
		for (int i = 1; i < vertices.size(); i++)
		{
			localBBox.merge(vertices[i]);
		}
	}
}

void Mesh::drawAppearance()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(transform.getTransformMatrix().constData());

	appearProgram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
	appearProgram->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);
	appearProgram->enableAttributeArray(PROGRAM_TANGENT_ATTRIBUTE);
	appearProgram->enableAttributeArray(PROGRAM_BITANGENT_ATTRIBUTE);
	appearProgram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);

	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_NORMAL_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_BITANGENT_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_TANGENT_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_TEXCOORD_ATTRIBUTE);

	int texRegBase = GL_TEXTURE0 + MESH_TEXTURE_REGISTER_OFFSET;
	int texRegOffset = MESH_TEXTURE_REGISTER_OFFSET;
	GLContext::instance()->getQGLFunctions()->glActiveTexture(texRegBase + 0);								// 分配一个纹理寄存器
	glBindTexture(GL_TEXTURE_2D, canvas.getGLColorTexObj());		// 把贴图对象绑定到寄存器
	appearProgram->setUniformValue("colorTex", texRegOffset + 0);	// 把寄存器绑定到着色器变量

	GLContext::instance()->getQGLFunctions()->glActiveTexture(texRegBase + 1);
	glBindTexture(GL_TEXTURE_2D, canvas.getGLSurfTexObj());
	appearProgram->setUniformValue("surfTex", texRegOffset + 1);

	GLContext::instance()->getQGLFunctions()->glActiveTexture(texRegBase + 2);
	glBindTexture(GL_TEXTURE_2D, canvas.getGLThicknessTexObj());
	appearProgram->setUniformValue("thickTex", texRegOffset + 2);

	appearProgram->setUniformValue("finalAlpha", alphaForAppearance);

	glBuffer[0].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_VERTEX_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBuffer[1].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBuffer[2].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_TANGENT_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBuffer[3].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_BITANGENT_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBuffer[4].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_TEXCOORD_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, glVertexBuffer.size());

	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_NORMAL_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_BITANGENT_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_TANGENT_ATTRIBUTE);
	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_TEXCOORD_ATTRIBUTE);

	drawSelectedFaces();
	if (isWireFrameEnabled && isObjSelected)
	{
		drawWireFrame();
	}
	glPopMatrix();

}
void Mesh::drawSelectedFaces()
{
	if (!selectedFaceIDSet.size())
		return;

	appearProgram->release();
	glLineWidth(3.0f);
	glColorMaterial(GL_FRONT,GL_AMBIENT);    //最开始颜色材质对应的是ambient的。所以要给为diffuse
	glColor3f(255.0 / 255.0f,159.0 / 255.0f,0.0 / 255.0f);
	glEnable(GL_COLOR_MATERIAL);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);
/*
	bool r = glBuffer[0].bind();
	GLContext::instance()->getQGLFunctions()->glVertexAttribPointer(PROGRAM_VERTEX_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/
	glBuffer[5].bind();
	glDrawElements(GL_TRIANGLES, selectedVertexIDArray.size(), GL_UNSIGNED_INT, 0);

	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);

	glDisable(GL_COLOR_MATERIAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	appearProgram->bind();
}
void Mesh::updateGLTextures()
{
	canvas.updateGLTextures();
}

bool Mesh::getTriangleData( const int faceID, TriangleData& triData )
{/*
	if (faceID < 0 || faceID >= faces.size())
		return false;*/
	ObjTriangle& f = faces[faceID];

	for (char i = 0; i < 3; ++i)
	{
		triData.vertex[i] = vertices[f.vertIndex[i]];
		triData.normal[i] = normals[f.norIndex[i]];
		triData.texCoord[i] = texcoords[f.texcoordIndex[i]];
	}
	return true;
}



void Mesh::drawWireFrame()
{
	appearProgram->release();
	glColor3f(0.8,0.8,0.8);
	glLineWidth(1.0f);

	GLContext::instance()->getQGLFunctions()->glEnableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);
	glColorMaterial(GL_FRONT,GL_AMBIENT);    //最开始颜色材质对应的是ambient的。所以要给为diffuse
	glColor3f(116.0 / 255.0f,190.0 / 255.0f,160.0 / 255.0f);
	glEnable(GL_COLOR_MATERIAL);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glDrawArrays(GL_TRIANGLES, 0, glVertexBuffer.size());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_COLOR_MATERIAL);

	GLContext::instance()->getQGLFunctions()->glDisableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);

	appearProgram->bind();
}

uint qHash ( const QVector3D& key )
{
	float x = key.x();
	float y = key.y();
	float z = key.z();
	float sum = x + y + z;
	return *((uint*)(&sum));
}

uint qHash ( const QVector2D& key )
{
	float x = key.x();
	float y = key.y();
	float sum = x + y;
	return *((uint*)(&sum));
}

void Mesh::optimizeArrays()
{
	// 把各种顶点数据加入到一个hashmap，合并相同顶点
	QMultiHash<QVector3D, int>vertexMap, normalMap;
	QMultiHash<QVector2D, int>texcoordMap;

	for (int ithVert = 0; ithVert < vertices.size(); ++ithVert)
		vertexMap.insertMulti(vertices[ithVert], ithVert);

	for (int ithNor = 0; ithNor < normals.size(); ++ithNor)
		normalMap.insertMulti(normals[ithNor], ithNor);

	for (int ithTex = 0; ithTex < texcoords.size(); ++ithTex)
		texcoordMap.insertMulti(texcoords[ithTex], ithTex);

	QVector<int> vertexIDMap(vertices.size()), 
		         normalIDMap(normals.size()), 
				 texcoordIDMap(texcoords.size());

	QVector<QVector3D> newVertexBuf, newNormalBuf;
	QVector<QVector2D> newTexcoordBuf;

	// 构建新的顶点缓冲
	QVector3D keyVertex(FLT_MAX, FLT_MAX, FLT_MAX);
	for (QMultiHash<QVector3D, int>::const_iterator iter = vertexMap.constBegin(); 
		 iter != vertexMap.constEnd(); ++iter) 
	{
		if (iter.key() != keyVertex)
		{	// 遇到新的顶点，将其加入新的数组
			keyVertex = iter.key();
			newVertexBuf.push_back(iter.key());
		}
		vertexIDMap[iter.value()] = newVertexBuf.size() - 1;
	}

	// 构建新的法线缓冲
	QVector3D keyNormal(FLT_MAX, FLT_MAX, FLT_MAX);
	for (QMultiHash<QVector3D, int>::const_iterator iter = normalMap.constBegin(); 
		iter != normalMap.constEnd(); ++iter) 
	{
		if (iter.key() != keyNormal)
		{	// 遇到新的顶点，将其加入新的数组
			keyNormal = iter.key();
			newNormalBuf.push_back(iter.key());
		}
		normalIDMap[iter.value()] = newNormalBuf.size() - 1;
	}


	// 构建新的贴图坐标缓冲
	QVector2D keyTexcoord(FLT_MAX, FLT_MAX);
	for (QMultiHash<QVector2D, int>::const_iterator iter = texcoordMap.constBegin(); 
		iter != texcoordMap.constEnd(); ++iter) 
	{
		if (iter.key() != keyTexcoord)
		{	// 遇到新的顶点，将其加入新的数组
			keyTexcoord = iter.key();
			newTexcoordBuf.push_back(iter.key());
		}
		texcoordIDMap[iter.value()] = newTexcoordBuf.size() - 1;
	}

	// 更新三角形索引
	for (int ithTri = 0; ithTri < faces.size(); ++ithTri)
	{
		ObjTriangle& tri = faces[ithTri];
		for (int ithPoint = 0; ithPoint < 3; ++ithPoint)
		{
			tri.vertIndex[ithPoint] = vertexIDMap[tri.vertIndex[ithPoint]];
			tri.norIndex[ithPoint] = normalIDMap[tri.norIndex[ithPoint]];
			tri.texcoordIndex[ithPoint] = texcoordIDMap[tri.texcoordIndex[ithPoint]];
		}
	}
	// 更新顶点数组
	vertices = newVertexBuf;
	normals = newNormalBuf;
	texcoords = newTexcoordBuf;
}

void Mesh::buildSelectVtxIDArray()
{
	selectedVertexIDArray.clear();
	foreach(const int& faceID, selectedFaceIDSet)
	{
		ObjTriangle& face = faces[faceID];
		selectedVertexIDArray.push_back(faceID*3);
		selectedVertexIDArray.push_back(faceID*3+1);
		selectedVertexIDArray.push_back(faceID*3+2);
	}
	glBuffer[5].bind();
	glBuffer[5].allocate(selectedVertexIDArray.constData(), sizeof(int) * selectedVertexIDArray.size());
}

void Mesh::clearSelectedFaceID()
{
	selectedFaceIDSet.clear();
	buildSelectVtxIDArray();
}

void Mesh::addSelectedFaceID( const QSet<int>& faceIDSet )
{
	selectedFaceIDSet += faceIDSet;
	buildSelectVtxIDArray();
}

void Mesh::removeSelectedFaceID( const QSet<int>& faceIDSet )
{
	selectedFaceIDSet -= faceIDSet;
	buildSelectVtxIDArray();
}

void Mesh::setSelectedFaceID( const QSet<int>& faceIDSet )
{
	selectedFaceIDSet = faceIDSet;
	buildSelectVtxIDArray();
}
QDataStream& operator<<(QDataStream& out, const ObjTriangle&tri)
{
	out << quint32(tri.vertIndex[0])
		<< quint32(tri.vertIndex[1])
		<< quint32(tri.vertIndex[2])
		<< quint32(tri.texcoordIndex[0])
		<< quint32(tri.texcoordIndex[1])
		<< quint32(tri.texcoordIndex[2])
		<< quint32(tri.norIndex[0])
		<< quint32(tri.norIndex[1])
		<< quint32(tri.norIndex[2])
		<< quint32(tri.mtlIndex);
	return out;
}

QDataStream& operator>>(QDataStream& in, ObjTriangle&tri)
{
	in  >> (tri.vertIndex[0])
		>> (tri.vertIndex[1])
		>> (tri.vertIndex[2])
		>> (tri.texcoordIndex[0])
		>> (tri.texcoordIndex[1])
		>> (tri.texcoordIndex[2])
		>> (tri.norIndex[0])
		>> (tri.norIndex[1])
		>> (tri.norIndex[2])
		>> (tri.mtlIndex);
	return in;
}

QDataStream& operator<<(QDataStream& out, const Mesh&mesh)
{
	out << mesh.vertices << mesh.normals << mesh.texcoords << mesh.faces
		<< mesh.canvas;
	return out;
}

QDataStream& operator>>(QDataStream& in, Mesh&mesh)
{
	in  >> mesh.vertices >> mesh.normals >> mesh.texcoords >> mesh.faces
		>> mesh.canvas;
	mesh.init();
	return in;
}