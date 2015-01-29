#pragma once


class ObjectTransform
{
public:
	ObjectTransform():m_scale(1,1,1){}
	// 操纵物体
	void translate(const QVector3D& dt){m_translation += dt; updateTransformMatrix();}
	void rotate(const QQuaternion& dr){m_rotation = dr * m_rotation; updateTransformMatrix();}
	void rotate(const QQuaternion& dr, const QVector3D& center)
	{
		m_rotation = dr * m_rotation;
		m_translation = dr.rotatedVector(m_translation - center) + center;
		updateTransformMatrix();
	}
	void scale(const QVector3D& ds){m_scale *= ds;updateTransformMatrix();}

	// 设置物体变换参数
	void setTranslate(const QVector3D& t){m_translation = t; updateTransformMatrix();}
	void setRotate(const QQuaternion& r){m_rotation = r; updateTransformMatrix();}
	void setScale(const QVector3D& s){m_scale = s;updateTransformMatrix();}

	const QMatrix4x4& getRotMatrix(){return m_rotMatrix;}
	const QMatrix4x4& getTransRotMatrix(){return m_transRotMatrix;}
	const QMatrix4x4& getTransformMatrix(){return m_transformMatrix;}
	const QMatrix4x4& getNorTransformMatrix(){return m_norTransformMatrix;}
	const QMatrix4x4& getInvTransformMatrix(){return m_invTransformMatrix;}

	// 获得物体变换参数
	const QVector3D& getTranslate(){return m_translation;}
	const QQuaternion& getRotate(){return m_rotation;}
	const QVector3D& getScale(){return m_scale;}

	friend QDataStream& operator>>(QDataStream& in ,       ObjectTransform& trans);
	friend QDataStream& operator<<(QDataStream& out, const ObjectTransform& trans);

protected:
	void updateTransformMatrix();


	QVector3D m_translation;
	QQuaternion m_rotation;
	QVector3D m_scale;

	QMatrix4x4 m_rotMatrix;						// Mr
	QMatrix4x4 m_transRotMatrix;				// Mt * Mr
	QMatrix4x4 m_transformMatrix;               // Mt * Mr * Ms 局部坐标到世界坐标的变换矩阵
	QMatrix4x4 m_invTransformMatrix;			// (Mt * Mr * Ms)-1 世界坐标到局部坐标的变换矩阵
	QMatrix4x4 m_norTransformMatrix;			// Mt * Mr * Ms^-1 法线的变换矩阵
};
// 抽象类，表示可以在场景中显示的物体
class RenderableObject
{
	enum DrawType{DRAW_GEOMETRY = 0x1, DRAW_BBOX = 0x1 << 1};
public:
	typedef QWeakPointer<RenderableObject> ObjectPointer;

	enum ObjectComponent{CMP_MESH = 0x1, CMP_PICKER = 0x2};
    enum ObjectType{OBJ_MESH = 0x1, OBJ_PICKER_OBJECT = CMP_MESH | CMP_PICKER};

    RenderableObject(void);
    virtual ~RenderableObject(void);

    RenderableObject::ObjectType getType(){return type;}

    void setName(const QString& name);
    const QString& getName(){return name;}

    // 选择物体
    void select(){isObjSelected = true;}
    void deselect(){isObjSelected = false;}
    bool isSelected(){return isObjSelected;}

    void hide(){isObjVisible = false;}
    void unhide(){isObjVisible = true;}
    void setVisible(bool isVisible) {isObjVisible = isVisible;}
    bool isVisible(){return isObjVisible;}

	// 获得物体近似大小
	float getApproSize();

	int getObjectID(){return objectID;}

	inline ObjectTransform& getTransform(){return transform;}
	void setTransform(const ObjectTransform& trans);

    QVector3D getCenter(){return transform.getTransformMatrix() * QVector3D(0,0,0);}

    virtual void drawGeometry() = 0;
	virtual void drawAppearance() = 0;

	float getOpacity(){return alphaForAppearance;}
	void  setOpacity(float alpha){alphaForAppearance = alpha;}


	// 光线与物体求交
	virtual bool isIntersect(const QVector3D& ori, const QVector3D& dir, float& t){return true;}

	friend QDataStream& operator<<(QDataStream& out, const QSharedPointer<RenderableObject>&pObj);
	friend QDataStream& operator>>(QDataStream&  in,       QSharedPointer<RenderableObject>&pObj);

protected:
    ObjectType type;
    QString name;								// 物体名称

	ObjectTransform transform;
	BoundingBox localBBox;

	float alphaForAppearance;					// 画出物体外观时的混合alpha


	// 为每个对象分配一个唯一的序号
	static unsigned currNewObjectID;
	unsigned objectID;

    bool isObjSelected;
    bool isObjVisible;

	static QHash<QString,int> nameMap;				// 物体名字集合，保证不重名

};