#pragma once
#define NUM_GL_VALUE_BUFFERS 5
#define NUM_GL_INDEX_BUFFERS 1
#define NUM_GL_BUFFERS NUM_GL_VALUE_BUFFERS + NUM_GL_INDEX_BUFFERS

#define PROGRAM_VERTEX_ATTRIBUTE	0
#define PROGRAM_NORMAL_ATTRIBUTE	1
#define PROGRAM_TANGENT_ATTRIBUTE	2
#define PROGRAM_BITANGENT_ATTRIBUTE 3
#define PROGRAM_TEXCOORD_ATTRIBUTE	4

struct GLWireArray
{
	float*vertices;
	unsigned*indices;
	int nFaces;
};

struct TriangleData
{
	QVector3D vertex[3];
	QVector3D normal[3];
	QVector2D texCoord[3];
};

class Mesh:public RenderableObject
{
	friend class ObjReader;
public:
	Mesh();
	~Mesh();

	// 准备openGL vertex buffer object，用于以后绘制
	void init();
	void drawGeometry();
	void drawAppearance();
	void releaseGLBuffer();

	// initCanvas 初始化画布，要求在initGLBuffer之后调用
	void initCanvas(int texWidth = TEX_DIM, int texHeight = TEX_DIM);
	Canvas& getCanvas(){return canvas;}
	void updateGLTextures();

	bool getTriangleData(const int faceID, TriangleData& triData);
	static QSharedPointer<QGLShaderProgram> getAppearanceShader()
	{return appearProgram;}
	static QSharedPointer<QGLShaderProgram> getGeometryShader()
	{return geometryProgram;}

	const QSet<int>& getSelectFaceIDSet(){return selectedFaceIDSet;}
	void setSelectedFaceID( const QSet<int>& faceIDSet );
	void addSelectedFaceID( const QSet<int>& faceIDSet );
	void removeSelectedFaceID( const QSet<int>& faceIDSet );
	void clearSelectedFaceID();
	static void enableWireFrame(bool isShow)
	{
		isWireFrameEnabled = isShow;
	}

	friend QDataStream& operator<<(QDataStream& out, const Mesh&mesh);
	friend QDataStream& operator>>(QDataStream& in, Mesh&mesh);

protected:
	void buildLocalBBox();
	void buildGLArrays();				// 构建openGL用的数组
	void optimizeArrays();				// 对ObjReader读取到的数据进行优化，
										// 确保同一个点只出现一次
	void releaseGLArrays();				// 释放openGL数组
	void buildSelectVtxIDArray();
	void drawWireFrame();
	void drawSelectedFaces();
	static bool isWireFrameEnabled;

	static QSharedPointer<QGLShaderProgram> geometryProgram;
	static QSharedPointer<QGLShaderProgram> appearProgram;


	// openGL绘图相关的数据,一个三角形占3个元素
	QVector<QVector3D> glVertexBuffer;	
	QVector<QVector3D> glNormalBuffer;  
	QVector<QVector3D> glTangentBuffer;
	QVector<QVector3D> glBitangentBuffer;
	QVector<QVector4D> glTexcoordBuffer; // (x,y,z,w) -> (u,v,ObjectID,FaceID)

	// opgnGL VBO对象，依次为 vertex normal tangent bitangent texcoord
	QGLBuffer glBuffer[NUM_GL_BUFFERS];

	// 从obj文件读取到的网格数据
	QVector<QVector3D>   vertices;		//vertex coordinate
	QVector<QVector3D>   normals;		//normal coordinate
	QVector<QVector2D>   texcoords;		//texture coordinate
	QVector<ObjTriangle> faces;			//faces

	// 选中的面集合
	QSet<int> selectedFaceIDSet;
	QVector<int> selectedVertexIDArray;

	Canvas canvas;

};


