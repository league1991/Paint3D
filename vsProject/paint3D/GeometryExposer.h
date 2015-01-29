#pragma once 
#include <QtOpenGL/QGLShaderProgram>
#define CLEAR_COLOR_FLOAT		1
#define USHORT_NORMALIZE_FACTOR	65535.0f
#define CLEAR_COLOR_SHORT		CLEAR_COLOR_FLOAT * 65535
#define NO_OBJECT_BIT			0xff	// 表示没有任何物体的一位
#define FACE_ID_MASK			0xffffff

// 这个类负责获得场景几何体表面的信息
class GeometryExposer
{
public:
	// 投影的方式，这个方式会影响depth的值
	// 如果是透视投影，深度值为观察空间坐标到摄像机原点的连线
	// 如果是正交投影，深度值为观察空间坐标z值
	enum ProjectionType{ GE_PROJ_ORTHOGRAPHIC = 0,GE_PROJ_PERSPECTIVE = 1};
	// 决定是采用几何体的z值还是表面纹理厚度作为深度值
	enum DepthType{GE_DEPTH_GEOMETRY, GE_DEPTH_THICKNESS};
	struct UVTexPixelData
	{
		unsigned short u, v;					// 纹理坐标
		union
		{
			unsigned int faceID;				// 三角形序号，注意最高8位被pID占用
			struct{
				unsigned char dummy[3];			// 占位字节，没有用
				unsigned char objectID;			// 几何体序号，注意占用了三角形序号高8位
			};
		};
	};
	struct NormalTexPixelData
	{
		unsigned short nx;
		unsigned short ny;
		unsigned short nz;
		unsigned short depth;
	};

	GeometryExposer();
	~GeometryExposer();
	static bool isFBOSupported();

	GLuint getUVTexObj(){return textureObj[0];}
	GLuint getNormalTexObj(){return textureObj[1];}

	// 获得屏幕某个的像素值,x y 比率为[0,1]注意屏幕坐标系原点在左上角
	// u v纹理,相应各位意义及长度为    (低)[u(16),       v(16), objectID(8), faceID(24)]（高）
	// normal纹理各位意义及长度为      (低)[norX(16), norY(16),    norZ(16),  depth(16)]（高）

	// 获得法线坐标及深度，注意法线可能背向屏幕
	bool getNormalAndDepth(const QVector2D& ratio, QVector3D& normal, float& depth)const;
	// 获得纹理坐标，几何体序号和三角面序号
	bool getUVAndID(const QVector2D& ratio, QVector3D& uv,unsigned char& objectID, unsigned int& faceID )const;
	// 获得所有信息
	bool getAll(const QVector2D& ratio, QVector3D& normal, float& depth,QVector2D& uv,unsigned char& objectID, unsigned int& faceID)const;
	// 获得物体序号，如果没有物体，序号为255
	bool getObjectID(const QVector2D& ratio, unsigned char& objectID)const;
	// 判断对应位置是否有物体
	bool isEmpty(const QVector2D& ratio)const;
	void getRegionFaceID( const QVector2D& minRatio, const QVector2D& maxRatio, int objectID, QSet<int>& faceIDSet )const;
	const UVTexPixelData* getUVAndIDArray()const{return uvTexData;}
	const NormalTexPixelData* getNormalAndDepthArray()const{return norTexData;}

	// 设置需要获取表面信息的物体
	void setRenderObject(const QWeakPointer<RenderableObject>& object);
	void setRenderObject(const QVector<QWeakPointer<RenderableObject>>& objects);

	// 设置投影方式
	void setProjectionType(ProjectionType type)
	{projType = type;isProjDepthTypeUpdated = false;}
	void setDepthType(DepthType type)
	{depthType = type;isProjDepthTypeUpdated = true;}


	void init(int width, int height);			// 初始化，分配各种缓存
	void setResolution(int width, int height);	// 重新设置分辨率
	void getResolution(int&width, int&height)
	{width = this->width; height = this->height;}
	void exposeGeometry();						// 渲染一次，之后就可以读取表面几何信息

private:
	bool generateBuffers();
	void destroyBuffers();
	inline bool isPixelEmpty(int xID, int yID)const
	{return uvTexData[xID + yID * width].objectID  == NO_OBJECT_BIT;}

	ProjectionType projType;
	DepthType      depthType;
	bool isProjDepthTypeUpdated;

	// 存储场景几何信息的纹理,每个纹理像素占4个短整数，共64位
	// textureData[0] ... uv 纹理  [u(16), v(16), objectID(8), faceID(24)]
	// textureData[1] ... 法线纹理 [norX(16), norY(16), norZ(16), depth(16)]
	UVTexPixelData* uvTexData;
	NormalTexPixelData* norTexData;

	static const GLenum renderTargets[];
	QGLShaderProgram* fboShader;
	QVector<QWeakPointer<RenderableObject>> sceneObjs;
	
	GLenum status;
	GLuint width, height;
	GLuint textureObj[2];
	GLuint frameBufferObj, depthBufferObj;
};