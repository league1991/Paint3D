#pragma once
#include "globject.h"
#include "GLTranslateManipulator.h"
#include "GLRotateManipulator.h"
#include "GLScaleManipulator.h"
class GLScene
{
public:
    enum ToolType{TOOL_SELECT = 0, TOOL_TRANSLATE, TOOL_ROTATE, TOOL_SCALE, TOOL_FACE_SELECT};

    GLScene(void);
    ~GLScene(void);

    // 网格操作
    void loadMesh(const char* path);
    //GLMesh* getMesh(const CString name);
    //bool removeMesh(const CString name);

    // 计算栅格操作
    void addGrid();
	void addMatlabGrid();

    // 一般操作
    RenderableObject* getCurSelectedObject(int filter = RenderableObject::GL_GRID | RenderableObject::GL_MESH);
    void getObjectNames(std::vector<QString>& names, int filter = RenderableObject::GL_GRID | RenderableObject::GL_MESH);
    RenderableObject* selectObject(const QString name, int filter = RenderableObject::GL_GRID | RenderableObject::GL_MESH);
    RenderableObject* selectObject(int x, int y);
    RenderableObject* getObject(QString& name, int filter = RenderableObject::GL_GRID | RenderableObject::GL_MESH);
    bool removeObject(const QString name, int filter = RenderableObject::GL_GRID | RenderableObject::GL_MESH);
    bool removeCurSelected(int filter = RenderableObject::GL_GRID | RenderableObject::GL_MESH);
    void draw();
    bool rename(const QString& oldName, const QString& newName);
    
    // 摄像机操作
    void centerCamera();

    // 选择操纵工具
    void setTool(GLScene::ToolType type);
    // 鼠标按下时，调用 captureObject，表示尝试捕捉物体
    // 鼠标拖动时，调用 manipulateObject， 表示对物体进行操纵
    // 鼠标松开时，调用 releaseObject，表示释放对物体的操纵
    bool captureObject   (int x, int y);
    bool manipulateObject(int x, int y);
    bool releaseObject   (int x, int y);
    bool isManipulating();

    // 鼠标移动时对鼠标的一些响应
    bool onMouseHover(int x, int y);

    GLCamera& getCamera(){return m_camera;}

    bool m_showBackgroundGrid;
private:
    std::vector<RenderableObject*> m_pObjects;

    Camera  m_camera;
    RenderableObject* m_curSelectObj;

    TranslateManipulator m_translateTool;
    RotateManipulator    m_rotateTool;
    ScaleManipulator     m_scaleTool;
    Manipulator*         m_curTool;
    ToolType               m_curToolType;

    int m_mouseBeginPos[2];    // 临时变量，记录鼠标按下时屏幕坐标
    int m_mouseCurrPos[2];
    bool m_isRectSelecting;

    RenderableObject* selectObject(const CVector3d& ori, const CVector3d& dir);
    GLMesh* searchMesh(const QString name);
    void    removeAllMesh();
    void    makeNameUnique(RenderableObject* newObj);
    // 检查与光线相交的最近的物体，但不选中物体
    RenderableObject* intersectObject( const CVector3d& ori, const CVector3d& dir );
};

