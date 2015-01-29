#include "StdAfx.h"
#include "GLScene.h"
#include <float.h>

GLScene::GLScene(void):
    m_curTool (NULL), 
    m_curToolType(GLScene::TOOL_SELECT),
    m_isRectSelecting(false),
    m_curSelectObj (NULL),
    m_showBackgroundGrid(false)
{
}


GLScene::~GLScene(void)
{
    removeAllMesh();
}

RenderableObject* GLScene::selectObject( const QString name , int filter)
{
    RenderableObject* selObj = NULL;
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        if ((*pM)->getName() == name && ((*pM)->getType() & filter))
        {
            selObj = *pM;
            selObj->select();
        }
        else
            (*pM)->deselect();
    }
    m_curSelectObj = selObj;
    if (m_curTool)
    {
        m_curTool->captureObject(m_curSelectObj);
    }
    return selObj;
}

GLMesh* GLScene::searchMesh( const QString name )
{
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        if ((*pM)->getName() == name && (*pM)->getType() == RenderableObject::GL_MESH)
            return (GLMesh*)*pM;
    }
    return NULL;
}
/*
GLMesh* GLScene::getMesh( const CString name )
{
    return searchMesh(name);
}

bool GLScene::removeMesh( const CString name )
{
    for (std::vector<GLObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        GLObject* obj = *pM;
        if (obj->getName() == name && obj->getType() == GLObject::GL_MESH)
        {
            delete obj;
            m_pObjects.erase(pM);
            return true;
        }
    }
    return false;
}*/

void GLScene::loadMesh( const char* path )
{
    GLMesh* newMesh = new GLMesh;
    newMesh->load(path);
    makeNameUnique(newMesh);
    m_pObjects.push_back(newMesh);
    m_curSelectObj = newMesh;
    if (m_curTool)
    {
        m_curTool->captureObject(newMesh);
    }
}

void GLScene::removeAllMesh()
{
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        delete *pM;
    }
    m_pObjects.clear();
    m_curSelectObj = NULL;
}

void GLScene::draw()
{

    m_camera.applyGLMatrices();
    
    if (m_showBackgroundGrid)
    {
        glLineWidth(1.0f);
        glColor3f(0.5,0.5,0.5);
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        for (int i = -10; i <= 10; ++i)
        {
            glVertex3f(-10, i, 0);
            glVertex3f(10, i, 0);
            glVertex3f(i, -10, 0);
            glVertex3f(i, 10, 0);
        }
    }
    glLineWidth(2.0f);
    glColor3f(0.8,0,0);
    glVertex3f(-10,0,0);
    glVertex3f(10,0,0);
    glColor3f(0,0.8,0);
    glVertex3f(0,-10,0);
    glVertex3f(0,10,0);
    glEnd();
    glEnable(GL_LIGHTING);

    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        (*pM)->draw();
    }
    if (m_curTool)
    {
        m_curTool->draw();
    }    
    if (m_isRectSelecting)
    {
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glScalef(2,2,1);
        glTranslatef(-0.5,-0.5,0);
        int res[2];
        m_camera.getScreenResolution(res[0], res[1]);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00ff);
        glLineWidth(2.0f);
        glColor3f(0,0,0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(m_mouseBeginPos[0] / (float)res[0], m_mouseBeginPos[1] / (float)res[1]);
        glVertex2f(m_mouseBeginPos[0] / (float)res[0], m_mouseCurrPos[1] / (float)res[1]);
        glVertex2f(m_mouseCurrPos [0] / (float)res[0], m_mouseCurrPos[1] / (float)res[1]);
        glVertex2f(m_mouseCurrPos [0] / (float)res[0], m_mouseBeginPos[1] / (float)res[1]);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        glEnable(GL_DEPTH_TEST);
    }
}

RenderableObject* GLScene::selectObject( const CVector3d& ori, const CVector3d& dir )
{
    float bestT = FLT_MAX;
    RenderableObject* bestObj = NULL;
    for (std::vector<RenderableObject*>::iterator pM= m_pObjects.begin();
         pM != m_pObjects.end(); ++pM)
    {
        float t;
        if ((*pM)->isIntersect(ori, dir, t))
        {
            if (t < bestT)
            {
                bestObj = *pM;
                bestT = t;
            }
        }
    }
    if (bestObj)
    {
        bestObj->select();
    }
    for (std::vector<RenderableObject*>::iterator pM= m_pObjects.begin();pM != m_pObjects.end(); ++pM)
    {
        if (*pM != bestObj)
            (*pM)->deselect();
    }
    m_curSelectObj = bestObj;
    return bestObj;
}
RenderableObject* GLScene::intersectObject( const CVector3d& ori, const CVector3d& dir )
{
    float bestT = FLT_MAX;
    RenderableObject* bestObj = NULL;
    for (std::vector<RenderableObject*>::iterator pM= m_pObjects.begin();
        pM != m_pObjects.end(); ++pM)
    {
        float t;
        if ((*pM)->isIntersect(ori, dir, t))
        {
            if (t < bestT)
            {
                bestObj = *pM;
                bestT = t;
            }
        }
    }
    return bestObj;
}
RenderableObject* GLScene::selectObject( int x, int y )
{
    CVector3d ori,dir;
    m_camera.getRay(x, y, ori, dir);
    return selectObject(ori, dir);
}

void GLScene::setTool( GLScene::ToolType type )
{
    m_curToolType = type;
    switch (type)
    {
    case TOOL_TRANSLATE:
        m_curTool = &m_translateTool;
        break;
    case TOOL_ROTATE:
        m_curTool = &m_rotateTool;
        break;
    case TOOL_SCALE:
        m_curTool = &m_scaleTool;
        break;
    case TOOL_SELECT:
    case TOOL_FACE_SELECT:
        m_curTool = NULL;
    }
    if (m_curTool)
    {
        m_curTool->captureObject(m_curSelectObj);
    }
}

bool GLScene::captureObject( int x, int y )
{
    m_mouseCurrPos[0] = m_mouseBeginPos[0] = x;
    m_mouseCurrPos[1] = m_mouseBeginPos[1] = y;

    CVector3d ori, dir;
    m_camera.getRay(x, y, ori, dir);
    if (m_curToolType == GLScene::TOOL_SELECT)    // 如果当前为选择工具，直接选择物体
    {
        selectObject(ori, dir);
    }
    else if (m_curToolType == GLScene::TOOL_FACE_SELECT)
    {
        m_isRectSelecting = true;
    }
    else if (m_curTool)            
    {
        // 如果为移动/旋转/缩放工具，
        // 首先检查是否点击了操纵轴，
        // 如果是，开始操纵，如果不是，说明是想选择物体
        RenderableObject* oldObject = m_curSelectObj;
        RenderableObject* newObject = intersectObject(ori, dir);
        bool isSameObj = oldObject == newObject;
        char axis = m_curTool->intersect(ori, dir);
        bool isHitAxis = axis != -1;
        
        if (!oldObject && newObject)
        { // 原来没有选中物体，现在选中一个新物体
            m_curSelectObj = newObject;
            m_curSelectObj->select();
            m_curTool->captureObject(m_curSelectObj);
        }
        else if (!newObject)
        {    // 没有选中新物体
            if (isHitAxis)
            {    // 但是选中了操纵器的一个轴，于是操纵当前物体
                //m_curSelectObj = m_curTool->getCurObject();
                if (m_curSelectObj)
                {
                    m_curSelectObj->select();
                    m_curTool->selectAxis(axis);
                    m_curTool->beginManipulate(ori, dir, axis);
                }
            }
            else
            {  // 什么都没选中
                m_curTool->releaseObject();
                if (m_curSelectObj)
                {
                    m_curSelectObj->deselect();
                    m_curSelectObj = NULL;
                }
            }
        }
        else if ( isHitAxis && oldObject)
        {    // 选中原有物体的一个轴向
            //m_curSelectObj = m_curTool->getCurObject();
            m_curTool->selectAxis(axis);
            m_curTool->beginManipulate(ori, dir, axis);
        }
        else if (!isSameObj && !isHitAxis) 
        {    // 选中一个新物体
            if (m_curSelectObj)
                m_curSelectObj->deselect();
            m_curSelectObj = newObject;
            m_curSelectObj->select();
            m_curTool->captureObject(m_curSelectObj);
        }
    }
    return true;
}

bool GLScene::manipulateObject( int x, int y )
{
    CVector3d ori, dir;
    m_mouseCurrPos[0] = x;
    m_mouseCurrPos[1] = y;
    m_camera.getRay(x, y, ori, dir);
    if (m_curSelectObj && m_curTool)
    {
        if (m_curTool->isManipulating())
        {
            m_curTool->goOnManipulate(ori, dir);
            return true;
        }
    }
    return false;
}

bool GLScene::releaseObject(int x, int y)
{
    if (m_curToolType == GLScene::TOOL_FACE_SELECT)
    {
        m_isRectSelecting = false;
        if (m_curSelectObj)
            if (m_curSelectObj->getType() == RenderableObject::GL_MESH)
            {
                CMatrix44 viewMat = m_camera.getViewMatrix();
                CMatrix44 projMat = m_camera.getProjMatrix();
                GLMesh* pmesh = (GLMesh*)m_curSelectObj;
                int min[2], max[2], viewRes[2];
                min[0] = min(m_mouseBeginPos[0], x);
                min[1] = min(m_mouseBeginPos[1], y);
                max[0] = max(m_mouseBeginPos[0], x);
                max[1] = max(m_mouseBeginPos[1], y);
                m_camera.getScreenResolution(viewRes[0], viewRes[1]);
                pmesh->selectFacetsByScreenRect(min, max, viewRes, viewMat, projMat);
            }
    }
    else if (m_curSelectObj && m_curTool)
    {
        if (m_curTool->isManipulating())
        {
            m_curTool->endManipulate();
            return true;
        }
    }
    return false;
}

bool GLScene::onMouseHover( int x, int y )
{
    CVector3d ori, dir;
    m_camera.getRay(x, y, ori, dir);
    if (m_curTool)
    {
        if (!m_curTool->isManipulating())
        {
            char axis = m_curTool->intersect(ori, dir);
            m_curTool->selectAxis(axis);
        }
        return true;
    }
    return false;
}

bool GLScene::isManipulating()
{
    if (m_curTool)
    {
        return m_curTool->isManipulating();
    }
    return false;
}

bool GLScene::rename( const QString& oldName, const QString& newName )
{
    std::vector<RenderableObject*>::iterator pSel = m_pObjects.end();
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        if ((*pM)->getName() == oldName)
            pSel = pM;
        if ((*pM)->getName() == newName)
            return false;
    }
    if (pSel != m_pObjects.end())
    {
        (*pSel)->setName(newName);
        return true;
    }
    return false;
}

bool GLScene::removeObject( const QString name, int filter /*= GLObject::GL_GRID | GLObject::GL_MESH*/ )
{
    if (m_curTool)
        if (m_curTool->getCurObject())
            return false;

    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        RenderableObject* obj = *pM;
        if (obj->getName() == name && (obj->getType() & filter))
        {
            delete obj;
            m_pObjects.erase(pM);
            return true;
        }
    }
    return false;
}

void GLScene::addGrid()
{
    GLGrid* grid = new GLGrid;
    makeNameUnique(grid);
    m_pObjects.push_back(grid);
}

void GLScene::getMeshes( std::vector<GLMesh*>& meshes )
{
    meshes.clear();
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        if ((*pM)->getType() == RenderableObject::GL_MESH)
        {
            meshes.push_back((GLMesh*)*pM);
        }
    }
}

void GLScene::getObjectNames( std::vector<QString>& names, int filter /*= GLObject::GL_GRID | GLObject::GL_MESH*/ )
{
    names.clear();
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        if ((*pM)->getType() & filter)
            names.push_back((*pM)->getName());
    }
}


RenderableObject* GLScene::getCurSelectedObject(int filter)
{
    if (!m_curSelectObj)
        return NULL;
    if (m_curSelectObj->getType() & filter)
        return m_curSelectObj;
    return NULL;
}

void GLScene::centerCamera()
{
    if (m_curSelectObj)
    {
        m_camera.setCenter(m_curSelectObj->getCenter());
    }
    else
    {
        m_camera.setCenter(CVector3d(0,0,0));
    }
}

bool GLScene::removeCurSelected( int filter /*= GLObject::GL_GRID | GLObject::GL_MESH*/ )
{
    if (m_curTool)
        if (m_curTool->getCurObject())
            return false;
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        RenderableObject* obj = *pM;
        if (obj->isSelected() && (obj->getType() & filter))
        {
            delete obj;
            m_pObjects.erase(pM);
            return true;
        }
    }
    return false;
}

void GLScene::makeNameUnique( RenderableObject* newObj )
{
    bool isUnique = false;
BEGIN_SEARCH:
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        if ((*pM)->getName() == newObj->getName())
        {
            newObj->setName((*pM)->getName() + "_1");
            goto BEGIN_SEARCH;
        }
    }
}

RenderableObject* GLScene::getObject( QString& name , int filter)
{
    for (std::vector<RenderableObject*>::iterator pM = m_pObjects.begin(); pM != m_pObjects.end(); ++pM)
    {
        if ((*pM)->getName() == name && ((*pM)->getType() & filter))
        {
            return *pM;
        }
    }
    return NULL;
}

void GLScene::addMatlabGrid()
{
	GLGrid* grid = new GLMatlabGrid;
	makeNameUnique(grid);
	m_pObjects.push_back(grid);
}
