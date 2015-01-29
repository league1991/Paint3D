#pragma once
#include "ObjReader.h"
#include <QtOpenGL/QGLShader>
#define SCREEN_BUFFER_RATIO 2

// 分配各个类采用的纹理寄存器起始序号
#define SCENE_TEXTURE_REGISTER_OFFSET 0 
#define MESH_TEXTURE_REGISTER_OFFSET  5  

uint qHash(const QWeakPointer<RenderableObject>& key);
class Scene
{
public:

	Scene(void);
	~Scene(void);

	void clear();
	bool init();
	void addPlanePicker();
	bool importObj(const QString& fileName);
	bool importEnvMap(const QString& fileName);
	void draw();

	void rotateCamera(float dx, float dy);
	void moveCamera(float dx, float dy);
	void zoomCamera(float dz);

	bool removeObject(QSharedPointer<RenderableObject>& obj);
	void insertObject(const QSharedPointer<RenderableObject>& obj);
	bool rename( const QString& oldName, const QString& newName );
	void resizeCamera(int width, int height);

	QWeakPointer<RenderableObject> selectObject( int x, int y );

	inline Camera& getCamera(){return camera;}
	inline GeometryExposer& getScreenExposer(){return exposer;}

	QSharedPointer<RenderableObject> getObject(int objID);
	QSharedPointer<RenderableObject> getObject(const QString& objName);
	Mesh*      getMesh( int objID);
	Brush&     getBrush(){return *(brush.data());}
	Painter&   getPainter(){return *(painter.data());}
	StrokeLib& getStrokeLib(){return strokeLib;}
	QUndoStack&getUndoStack(){return undoStack;}
	GeometryExposer&getGeometryExposer(){return exposer;}
	void       getObjectNames(RenderableObject::ObjectType type, QStringList& names);

	void setObjectFrozen(QSet<QWeakPointer<RenderableObject>> obj);
	void setObjectUnfrozen(QSet<QWeakPointer<RenderableObject>>obj);
	void setOtherObjectFrozen(QSet<QWeakPointer<RenderableObject>> obj);
	void setAllObjectUnfrozen();
	void setPickerObjectFrozen();
	void setPickerObjectUnfrozen();
	const QSet<QWeakPointer<RenderableObject>>& getFrozenObjectSet(){return frozenObjectSet;}
	// 更新记录场景信息的图像
	void updateGeometryImage(){isGeometryImgInvalid = true;}
	// 检查与光线相交的最近的物体，但不选中物体
	QWeakPointer<RenderableObject> intersectObject( int x, int y );

	void save(const QString& fileName);
	void open(const QString& fileName);
private:
	void drawGrid();
	void makeNameUnique(RenderableObject* newObj);
	void refreshExposerObjectList();

	QVector<QSharedPointer<RenderableObject>> objectArray;
	QSet<QWeakPointer<RenderableObject>> frozenObjectSet;
	//QVector<QSharedPointer<ObjMaterial>> materials;
	GeometryExposer exposer;
	Camera camera;

	QSharedPointer<Brush>   brush;
	QSharedPointer<Painter> painter;
	StrokeLib               strokeLib;

	CubeMap                 envMap;

	QUndoStack              undoStack;


	bool isGeometryImgInvalid;		// 是否需要重新生成 geometry image
	int mouseBeginPos[2];    // 临时变量，记录鼠标按下时屏幕坐标
	int mouseCurPos[2];
	bool isRectSelecting;

	bool isEnvMapUpdated;

	QWeakPointer<RenderableObject> selectObject( int objID );
};
