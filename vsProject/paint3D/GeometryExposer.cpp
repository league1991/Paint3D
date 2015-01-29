#include "stdafx.h"
#include "GeometryExposer.h"
	
const GLenum GeometryExposer::renderTargets[] = 
{ GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };

GeometryExposer::GeometryExposer()
{
	fboShader = NULL;
	uvTexData = NULL;
	norTexData = NULL;
	width = 256;
	height = 256;
	projType = GE_PROJ_PERSPECTIVE;
	depthType = GE_DEPTH_GEOMETRY;
	isProjDepthTypeUpdated = true;
	depthBufferObj = frameBufferObj = -1;
	textureObj[0] = textureObj[1] = -1;
}

GeometryExposer::~GeometryExposer()
{
	if (fboShader)
		delete fboShader;
	destroyBuffers();
}


bool GeometryExposer::isFBOSupported()
{
	return QGLFramebufferObject::hasOpenGLFramebufferObjects();
}

void GeometryExposer::init( int w, int h )
{
	width = w;
	height = h;
	generateBuffers();

	fboShader = new QGLShaderProgram;
	fboShader->addShaderFromSourceFile(QGLShader::Vertex, "fboShaderVS.glsl");
	fboShader->addShaderFromSourceFile(QGLShader::Fragment, "fboShaderFS.glsl");
	fboShader->link();
	fboShader->bind();
}

bool GeometryExposer::generateBuffers()
{
	if (depthBufferObj == -1)
	{
		glGenRenderbuffersEXT(1, &depthBufferObj);
	}
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferObj);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

	if (frameBufferObj == -1)
	{
		glGenFramebuffersEXT(1, &frameBufferObj);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObj);

	if (uvTexData)
	{
		char* uvBuf = (char*)uvTexData;
		delete []uvBuf;
	}
	unsigned char* buf = new unsigned char[width * height * sizeof(UVTexPixelData) * 2];
	for (int i = 0; i < width * height * sizeof(UVTexPixelData) * 2; ++i)
	{
		buf[i] = 255;
	}
	uvTexData = (UVTexPixelData*)buf;
	norTexData = (NormalTexPixelData*)(buf + width * height * sizeof(UVTexPixelData));

	if (textureObj[0] == -1)
	{
		glGenTextures(2, textureObj);
	}
	glBindTexture(GL_TEXTURE_2D, textureObj[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, uvTexData);
	glBindTexture(GL_TEXTURE_2D, textureObj[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, norTexData);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureObj[0], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, textureObj[1], 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBufferObj);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		qDebug() << "error when binding texture to fbo" << endl;
		QMessageBox::critical(NULL, 
			QObject::tr("Error"), 
			QObject::tr("Fail to initialize framebuffer object."));
	}
	return status == GL_FRAMEBUFFER_COMPLETE_EXT;
}

void GeometryExposer::exposeGeometry()
{
	// 切换framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObj);

	GLint view[4]; 
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0, width, height);
	glClearColor(CLEAR_COLOR_FLOAT,CLEAR_COLOR_FLOAT,CLEAR_COLOR_FLOAT,CLEAR_COLOR_FLOAT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawBuffers(2, renderTargets);
	int i = 0;

	QSharedPointer<QGLShaderProgram>meshShader = Mesh::getGeometryShader();
	if (meshShader)
	{
		meshShader->bind();/*
		if (isProjDepthTypeUpdated)
		{*/
			float isPerspDepth = projType == GE_PROJ_PERSPECTIVE ? 1.0f : 0.0f;
			float isGeometryDepth = depthType == GE_DEPTH_GEOMETRY ? 1.0f : 0.0f;
			meshShader->setUniformValue("isPerspDepth", isPerspDepth);
			meshShader->setUniformValue("isGeometryDepth", isGeometryDepth);
			isProjDepthTypeUpdated = false;/*
		}*/

		for (QVector<QWeakPointer<RenderableObject>>::iterator pO = sceneObjs.begin();
			pO != sceneObjs.end(); ++pO)
		{
			if (*pO)
			{
				(*pO).data()->drawGeometry();++i;
			}
		}
		meshShader->release();
	}

	glPopAttrib();

	// 切换回原来的framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// 把帧缓存数据读回内存
	glBindTexture(GL_TEXTURE_2D,textureObj[0]);
	glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_SHORT,(GLvoid*)uvTexData);
	glBindTexture(GL_TEXTURE_2D,textureObj[1]);
	glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_SHORT,(GLvoid*)norTexData);

	GLenum errorMsg = glGetError();
	if (errorMsg != GL_NO_ERROR)
		qDebug() << "error occurs when exposing" << endl;
}

void GeometryExposer::setRenderObject( const QWeakPointer<RenderableObject>& object )
{
	sceneObjs.clear();
	sceneObjs.append(object);
}

void GeometryExposer::setRenderObject( const QVector<QWeakPointer<RenderableObject>>& objects )
{
	sceneObjs = objects;
}

void GeometryExposer::destroyBuffers()
{
	if (uvTexData)
	{
		glDeleteTextures(2, textureObj);
		glDeleteFramebuffersEXT(1, &frameBufferObj);
		glDeleteRenderbuffersEXT(1, &depthBufferObj);
		textureObj[0] = textureObj[1]= -1;
		frameBufferObj = depthBufferObj = -1;
		delete[] uvTexData;
		uvTexData = NULL;
		norTexData = NULL;
	}
}

void GeometryExposer::setResolution( int w, int h )
{
	destroyBuffers();
	width = max(w,1);
	height =max(h,1);
	generateBuffers();
}


bool GeometryExposer::getNormalAndDepth( const QVector2D& ratio, QVector3D& normal, float& depth )const
{
	int xID = max(0, min(ratio.x() * width, width - 1));
	int yID = max(0, min((1 - ratio.y()) * height, height - 1));
	if (isPixelEmpty(xID, yID))
		return false;
	NormalTexPixelData& norPixel = norTexData[xID + yID * width];
	normal = QVector3D((norPixel.nx - 32767.5) / 32767.5,
		               (norPixel.ny - 32767.5) / 32767.5,
					   (norPixel.nz - 32767.5) / 32767.5);
	float norDepth = norPixel.depth / 65535.0;
	depth = norDepth / max(1 - norDepth, 0.001);	// 保证depth为有效值并小于1000
	return true;
}

bool GeometryExposer::getUVAndID(const QVector2D& ratio, QVector3D& uv,unsigned char& objectID, unsigned int& faceID )const
{
	int xID = max(0, min(ratio.x() * width, width - 1));
	int yID = max(0, min((1 - ratio.y()) * height, height - 1));
	if (isPixelEmpty(xID, yID))
		return false;
	UVTexPixelData &uvPixel = uvTexData[xID + yID * width];
	uv = QVector2D(uvPixel.u / 65535.0, uvPixel.v / 65535.0);
	objectID = uvPixel.objectID;
	faceID = uvPixel.faceID & FACE_ID_MASK;
	return true;
}

bool GeometryExposer::isEmpty( const QVector2D& ratio )const
{
	int xID = max(0, min(ratio.x() * width, width - 1));
	int yID = max(0, min((1 - ratio.y()) * height, height - 1));
	return isPixelEmpty(xID, yID);
}

bool GeometryExposer::getAll(const QVector2D& ratio, QVector3D& normal, float& depth,QVector2D& uv,unsigned char& objectID, unsigned int& faceID)const
{
	int xID = max(0, min(ratio.x() * width, width - 1));
	int yID = max(0, min((1 - ratio.y()) * height, height - 1));
	UVTexPixelData &uvPixel = uvTexData[xID + yID * width];
	if (uvPixel.objectID == NO_OBJECT_BIT)
		return false;
	objectID = uvPixel.objectID;
	uv = QVector2D(uvPixel.u / 65535.0, uvPixel.v / 65535.0);
	faceID = uvPixel.faceID & FACE_ID_MASK;

	NormalTexPixelData& norPixel = norTexData[xID + yID * width];
	normal = QVector3D ((norPixel.nx - 32767.5) / 32767.5,
						(norPixel.ny - 32767.5) / 32767.5,
						(norPixel.nz - 32767.5) / 32767.5);
	float norDepth = norPixel.depth / 65535.0;
	depth = norDepth / max(1 - norDepth, 0.001);
	return true;
}

bool GeometryExposer::getObjectID( const QVector2D& ratio, unsigned char& objectID )const
{
	int xID = max(0, min(ratio.x() * width, width - 1));
	int yID = max(0, min((1 - ratio.y()) * height, height - 1));

	UVTexPixelData &uvPixel = uvTexData[xID + yID * width];
	objectID = uvPixel.objectID;
	unsigned int fID = uvPixel.faceID & FACE_ID_MASK;
	return objectID != 0xff;
}

void GeometryExposer::getRegionFaceID( const QVector2D& minRatio, const QVector2D& maxRatio, int objectID, QSet<int>& faceIDSet )const
{
	int minX = max(0, min(      minRatio.x() * width, width - 1));
	int maxX = max(0, min(      maxRatio.x() * width, width - 1));
	int minY = max(0, min((1 - maxRatio.y()) * height, height - 1));
	int maxY = max(0, min((1 - minRatio.y()) * height, height - 1));
	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			UVTexPixelData &uvPixel = uvTexData[x + y * width];
			if (uvPixel.objectID == NO_OBJECT_BIT ||
				uvPixel.objectID != objectID)
				continue;
			unsigned faceID = uvPixel.faceID & FACE_ID_MASK;
			faceIDSet.insert(faceID);
		}
	}
}
