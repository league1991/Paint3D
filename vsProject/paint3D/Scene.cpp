#include "StdAfx.h"
#include "Scene.h"
#include "Paint3DFrame.h"
#include <QtWidgets/QMessageBox>

Scene::Scene(void)
{
	//curTool = NULL;
	//curToolType = TOOL_SELECT;
	isGeometryImgInvalid = true;
	isEnvMapUpdated = false;
}

Scene::~Scene(void)
{

}

void Scene::addPlanePicker()
{
	QSharedPointer<PlanePicker> picker(new PlanePicker);
	picker->init();
	objectArray.push_back(picker);
	refreshExposerObjectList();
}

bool Scene::importObj(const QString& fileName )
{
	ObjReader reader;
	if (!reader.read(fileName))
		return false;
	for (int i = 0; i < reader.getNumMeshes(); ++i)
	{
		QSharedPointer<Mesh> pM(reader.getMesh(i));
		pM->init();
		objectArray.push_back(pM);
	}
	refreshExposerObjectList();
	isGeometryImgInvalid = true;
	return true;
}

void Scene::drawGrid()
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor3f(0.3,0.3,0.3);
	for (int i = -10; i <= 10; ++i)
	{
		glVertex3f(-10,i,0);
		glVertex3f(10,i,0);
		glVertex3f(i,-10,0);
		glVertex3f(i,10,0);
	}
	glEnd();
}


bool Scene::init()
{
	unsigned err = glewInit();
	if (GLEW_OK != err)
	{
		const GLubyte *e = glewGetErrorString(err);
		QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("GLEW initialization error."));
		qDebug() << e;
	}

	if (glewIsSupported("GL_EXT_framebuffer_object"))
		qDebug() <<"Old EXT FBO available"<<endl;
	else
	{
		qDebug() <<"Old EXT FBO NOT available"<<endl;
		QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Old EXT FBO NOT available"));
	}

	if (glewIsSupported("GL_ARB_framebuffer_object"))
		qDebug() <<"Newer ARB FBO available"<<endl;
	else
	{
		qDebug() <<"Newer ARB FBO NOT available"<<endl;
		//QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("New ARB FBO NOT available"));
	}	

	bool isfbo = GeometryExposer::isFBOSupported();
	int w,h;
	camera.getScreenResolution(w,h);
	exposer.init(w / SCREEN_BUFFER_RATIO, h / SCREEN_BUFFER_RATIO);

	strokeLib.init();
	brush = QSharedPointer<Brush>(new Brush(this));
	brush->setStroke(strokeLib.getDefaultStroke());
	//QMessageBox::information(NULL, QObject::tr("Info"), QObject::tr("brush initialized."));
	painter = QSharedPointer<Painter>(new SurfacePainter(this));
	painter->setBrush(brush);
	//QMessageBox::information(NULL, QObject::tr("Info"), QObject::tr("painter initialized."));

	//importObj("expplane.obj");
	undoStack.setUndoLimit(50);

	
	envMap.load("background.png", 512);
	envMap.initGLBuffer();
	// QMessageBox::information(NULL, QObject::tr("Info"), QObject::tr("env map initialized."));
	//envMap.saveCubeMap("background.jpg");

	isGeometryImgInvalid = true;
	return true;
}

void Scene::draw()
{
	camera.applyGLMatrices();

	//画出网格线
	drawGrid();

	if (isGeometryImgInvalid)
	{
		exposer.exposeGeometry();
		isGeometryImgInvalid = false;
	}
	brush->draw();

	// 启用网格shader
	QSharedPointer<QGLShaderProgram>meshShader = Mesh::getAppearanceShader();
	if (meshShader)
	{
		meshShader->bind();

		// 绑定环境贴图,只绑定一次
		if (!isEnvMapUpdated)
		{
			// int texRegBase = GL_TEXTURE0_ARB + SCENE_TEXTURE_REGISTER_OFFSET;
			int texRegBase = GL_TEXTURE0 + SCENE_TEXTURE_REGISTER_OFFSET;
			int texRegOffset = SCENE_TEXTURE_REGISTER_OFFSET;
			//glActiveTextureARB(texRegBase + 0);	
			GLContext::instance()->getQGLFunctions()->glActiveTexture(texRegBase + 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, envMap.getGLTexObj());
			meshShader->setUniformValue("envTex", texRegOffset + 0);

			GLContext::instance()->getQGLFunctions()->glActiveTexture(texRegBase + 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, envMap.getBRDFTexObj());
			meshShader->setUniformValue("brdfLUT", texRegOffset + 1);
			isEnvMapUpdated = true;
		}

		// 设定转换矩阵
		QMatrix4x4 viewMatrix = camera.getViewMatrix();
		meshShader->setUniformValue("viewMatrixTranspose", viewMatrix.transposed());


		// 先画不透明的物体
		for (int i = 0; i < objectArray.size(); ++i)
		{
			if (objectArray[i] && objectArray[i]->getType() == RenderableObject::OBJ_MESH)
			{
				objectArray[i]->drawAppearance();
			}
		}

		// 再画透明的物体
 		glEnable(GL_BLEND);
 		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 		glDepthMask(GL_FALSE);
		for (int i = 0; i < objectArray.size(); ++i)
		{
			if (objectArray[i] && objectArray[i]->getType() == RenderableObject::OBJ_PICKER_OBJECT)
			{
				objectArray[i]->drawAppearance();
			}
		}
 		glDepthMask(GL_TRUE);
 		glDisable(GL_BLEND);

		// 释放shader
		meshShader->release();
	}


	if (Paint3DFrame::getInstance()->viewWidget->getCurToolType() == GLViewWidget::TOOL_PAINT)
		brush->draw();


	GLenum errorMsg = glGetError();
	if (errorMsg != GL_NO_ERROR)
		qDebug() << "error occurs when rendering" << endl;

}

bool Scene::rename( const QString& oldName, const QString& newName )
{
	QVector<QSharedPointer<RenderableObject>>::iterator pSel = objectArray.end();
	for (QVector<QSharedPointer<RenderableObject>>::iterator pM = objectArray.begin(); pM != objectArray.end(); ++pM)
	{
		if ((*pM)->getName() == oldName)
		{
			pSel = pM;	break;
		}
	}
	if (pSel != objectArray.end())
	{
		(*pSel)->setName(newName);
		return true;
	}
	return false;
}




QWeakPointer<RenderableObject> Scene::selectObject( int x, int y )
{
	int width, height;
	camera.getScreenResolution(width, height);
	float xRatio = x / float(width);
	float yRatio = y / float(height);
	unsigned char objID;
	exposer.getObjectID(QVector2D(xRatio, yRatio), objID);
	QWeakPointer<RenderableObject> curSelectObj;
	QSharedPointer<RenderableObject> bestObj;
	for (QVector<QSharedPointer<RenderableObject>>::iterator pM= objectArray.begin();
		pM != objectArray.end(); ++pM)
	{
		float t;
		if (objID == (*pM)->getObjectID())
		{
			(*pM)->select();
			curSelectObj = pM->toWeakRef();
			//brush.setObject(m_curSelectObj);
		}
		else
			(*pM)->deselect();
	}
	return curSelectObj;
}

QWeakPointer<RenderableObject> Scene::selectObject( int objID )
{
	QSharedPointer<RenderableObject> bestObj;
	for (QVector<QSharedPointer<RenderableObject>>::iterator pM= objectArray.begin();
		pM != objectArray.end(); ++pM)
	{
		float t;
		if (objID == (*pM)->getObjectID())
		{
			(*pM)->select();
			return pM->toWeakRef();
		}
		else
			(*pM)->deselect();
	}
	return QWeakPointer<RenderableObject>();
}

QWeakPointer<RenderableObject> Scene::intersectObject( int x, int y )
{
	int width, height;
	camera.getScreenResolution(width, height);
	float xRatio = x / float(width);
	float yRatio = y / float(height);
	unsigned char objID;
	exposer.getObjectID(QVector2D(xRatio, yRatio), objID);

	for (QVector<QSharedPointer<RenderableObject>>::iterator pM= objectArray.begin();
		pM != objectArray.end(); ++pM)
	{
		float t;
		if (objID == (*pM)->getObjectID())
		{
			return (*pM).toWeakRef();
		}
	}
	return QWeakPointer<RenderableObject>();
}

void Scene::resizeCamera( int width, int height )
{
	camera.setScreenResolution(width, height);
	exposer.setResolution(width / SCREEN_BUFFER_RATIO, height / SCREEN_BUFFER_RATIO);
	isGeometryImgInvalid = true;
}

void Scene::refreshExposerObjectList()
{
	QVector<QWeakPointer<RenderableObject>> v;
	for (int i = 0; i < objectArray.size(); i++)
	{
		QWeakPointer<RenderableObject> pO = objectArray[i].toWeakRef();
		if (!frozenObjectSet.contains(pO))
		{
			v.push_back(objectArray[i].toWeakRef());
		}
	}
	exposer.setRenderObject(v);
	isGeometryImgInvalid = true;
}

void Scene::rotateCamera( float dx, float dy )
{
	camera.rotateCamera(dx, dy);
	isGeometryImgInvalid = true;
}

void Scene::moveCamera( float dx, float dy )
{
	camera.moveCamera(dx, dy);
	isGeometryImgInvalid = true;
	
}

void Scene::zoomCamera( float dz )
{
	camera.zoomCamera(dz);
	isGeometryImgInvalid = true;
}

Mesh* Scene::getMesh( int objID )
{
	Mesh* mesh;
	for (QVector<QSharedPointer<RenderableObject>>::iterator pO = objectArray.begin();
		 pO != objectArray.end(); ++pO)
	{
		if (((*pO)->getType() & RenderableObject::OBJ_MESH) &&
			(*pO)->getObjectID() == objID)
		{
			mesh = (Mesh*)((*pO).data());
			return mesh;
		}
	}
	return NULL;
}

QSharedPointer<RenderableObject> Scene::getObject( int objID )
{
	for (QVector<QSharedPointer<RenderableObject>>::iterator pO = objectArray.begin();
		pO != objectArray.end(); ++pO)
	{
		if ((*pO)->getObjectID() == objID)
		{
			return ((*pO));
		}
	}
	return QSharedPointer<RenderableObject>();
}

QSharedPointer<RenderableObject> Scene::getObject( const QString& objName )
{
	for (QVector<QSharedPointer<RenderableObject>>::iterator pO = objectArray.begin();
		pO != objectArray.end(); ++pO)
	{
		if ((*pO)->getName() == objName)
		{
			return ((*pO));
		}
	}
	return QSharedPointer<RenderableObject>();
}

void Scene::getObjectNames( RenderableObject::ObjectType type, QStringList& names )
{
	for (QVector<QSharedPointer<RenderableObject>>::iterator pO = objectArray.begin();
		pO != objectArray.end(); ++pO)
	{
		if ((*pO)->getType() == RenderableObject::OBJ_PICKER_OBJECT)
		{
			names.push_back((*pO)->getName());
		}
	}
}



bool Scene::importEnvMap( const QString& fileName )
{
	envMap.releaseGLBuffer();
	envMap.load(fileName, 512);
	envMap.initGLBuffer();
	isEnvMapUpdated = false;
	return true;
}

bool Scene::removeObject(QSharedPointer<RenderableObject>& obj)
{
	for (QVector<QSharedPointer<RenderableObject>>::iterator pO = objectArray.begin();
		pO != objectArray.end(); ++pO)
	{
		if (*pO == obj)
		{
			objectArray.erase(pO);
			refreshExposerObjectList();
			isGeometryImgInvalid = true;
			return true;
		}
	}
	return false;
}

void Scene::insertObject( const QSharedPointer<RenderableObject>& obj )
{
	objectArray.push_back(obj);
	selectObject(obj->getObjectID());
	refreshExposerObjectList();
	isGeometryImgInvalid = true;
}

void Scene::setObjectFrozen( QSet<QWeakPointer<RenderableObject>> obj )
{
	frozenObjectSet +=obj;
	refreshExposerObjectList();
}

void Scene::setObjectUnfrozen( QSet<QWeakPointer<RenderableObject>> obj)
{
	frozenObjectSet -= obj;
	refreshExposerObjectList();
}

void Scene::setOtherObjectFrozen( QSet<QWeakPointer<RenderableObject>> obj )
{
	for (int i = 0; i < objectArray.size();++i)
	{
		if (!obj.contains(objectArray[i]))
		{
			frozenObjectSet.insert(objectArray[i].toWeakRef());
		}
	}
	refreshExposerObjectList();
}

void Scene::setAllObjectUnfrozen()
{
	frozenObjectSet.clear();
	refreshExposerObjectList();
}

void Scene::setPickerObjectFrozen()
{
	for (int i = 0; i < objectArray.size();++i)
	{
		if (objectArray[i]->getType() & RenderableObject::CMP_PICKER)
		{
			frozenObjectSet.insert(objectArray[i].toWeakRef());
		}
	}
	refreshExposerObjectList();
}

void Scene::setPickerObjectUnfrozen()
{
	for (int i = 0; i < objectArray.size();++i)
	{
		if (objectArray[i]->getType() & RenderableObject::CMP_PICKER)
		{
			frozenObjectSet.remove(objectArray[i].toWeakRef());
		}
	}
	refreshExposerObjectList();
}

void Scene::save( const QString& fileName )
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream out(&file);
		out.setVersion(QDataStream::Qt_5_3);
		out << objectArray;
	}
}

void Scene::open( const QString& fileName )
{
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		clear();
		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_5_3);
		in >> objectArray;
	}
	refreshExposerObjectList();
	isGeometryImgInvalid = true;
}

void Scene::clear()
{
	objectArray.clear();
	frozenObjectSet.clear();
	undoStack.clear();
	refreshExposerObjectList();
	brush->setObject(QWeakPointer<Mesh>());
	painter->setObject(QWeakPointer<Mesh>());
}


uint qHash( const QWeakPointer<RenderableObject>& key )
{
	return (uint)key.data();
}
