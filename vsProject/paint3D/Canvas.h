#pragma once
#define UPDATE_REGION_INVALID_COORD 65535		// 标记一个不合法的矩形更新区域（也就是不用更新）
#define TEX_DIM 1024
#define CANVAS_MAX_SIZE	4096					// 一个图层的最大尺寸为 CANVAS_MAX_SIZE * CANVAS_MAX_SIZE
// 一个颜色纹理像素从高位到低位按 ARGB 排列
#define R_I2F(x)   ((x >>16) & 0xff) / 255.0f	
#define G_I2F(x)   ((x >> 8) & 0xff) / 255.0f
#define B_I2F(x)        ((x) & 0xff) / 255.0f
#define A_I2F(x)   ((x >>24) & 0xff) / 255.0f
// 一个surface纹理像素从高位到低位按 IOR 折射 高光 反射  排列
#define RL_I2F(x)        ((x) & 0xff) / 255.0f
#define GL_I2F(x)   ((x >> 8) & 0xff) / 255.0f
#define RR_I2F(x)   ((x >>16) & 0xff) / 255.0f
#define ID_I2F(x)   ((x >>24) & 0xff) / 255.0f

#define INIT_COLOR_BITS 0x00ffffff
#define INIT_BACKBROUND_COLOR_BITS 0xffffffff
#define INIT_SURF_BITS  0x00ff0000
#define INIT_THICKNESS  0.0f


struct CanvasLayer
{
public:
	enum LayerChannel
	{
		CHNL_REFLECTION = 0,
		CHNL_GLOSSNESS  = 1,
		CHNL_REFRACTION = 2,
		CHNL_IOR        = 3,
		CHNL_COLOR      = 4,
		CHNL_THICKNESS  = 5
	};
	CanvasLayer(int width, int height);
	CanvasLayer(const CanvasLayer& layer);
	CanvasLayer();
	~CanvasLayer();

	inline float& getThickness(int x, int y)
	{return thickness[x + y * width];}

	inline unsigned& getColorPixel(int x, int y)
	{return ((unsigned*)colorImage.scanLine(y))[x];}
	
	inline unsigned& getSurfPixel(int x, int y)
	{return ((unsigned*)surfaceImage.scanLine(y))[x];}

	inline unsigned* getColorPointer()
	{return (unsigned*)colorImage.scanLine(0);}

	inline unsigned* getSurfPointer()
	{return (unsigned*)surfaceImage.scanLine(0);}

	inline float* getThicknessPointer()
	{ return thickness;	}


	void setLayerFromImage(const QImage& image, const LayerChannel channel);
	void saveLayer(const QString& fileName, const LayerChannel channel);
	// 图层混合,指定一个区域，与输入的三个数组中存放的像素值做混合,结果写回输入数组
	// isContinuous = 1时，数组中只包括要更新的像素，也就是说被更新的内存是整块连续的
	// isContinuous = 0时，数组中包括所有图像像素，也就是说被更新的内存不是整块连续的
	void blendRegion(int xmin, int ymin, 
		int regionWidth, int regionHeight, 
		unsigned* bufColor, unsigned* bufSurf, float* bufThick, 
		int isContinuous = 1);

	void blendRegion(int xmin, int ymin, 
		int regionWidth, int regionHeight, float* bufThick, 
		int isContinuous = 1);
	void blendRegion(int xmin, int ymin, int regionWidth, int regionHeight, PixelPos* updatePixels, unsigned updateCount, unsigned* bufColor, unsigned* bufSurf, float* bufThick, int isContinuous);
	void blendRegion(int xmin, int ymin, int regionWidth, int regionHeight, PixelPos*updatePixels, unsigned updateCount, float* bufThick, int isContinuous);
	void setRegion(int xmin, int ymin, 
		int regionWidth, int regionHeight, 
		unsigned* bufColor, unsigned* bufSurf, float* bufThick, 
		int isContinuous = 1);

	void setRegion(int xmin, int ymin, 
		int regionWidth, int regionHeight, 
		float* bufThick, 
		int isContinuous = 1);
	void setRegion(int xmin, int ymin, int regionWidth, int regionHeight, PixelPos*updatePixels, unsigned updateCount, unsigned* bufColor, unsigned* bufSurf, float * bufThick, int isContinuous);
	void setRegion(int xmin, int ymin, int regionWidth, int regionHeight, PixelPos*updatePixels, unsigned updateCount, float* bufThick, int isContinuous);
	void setVisible(bool isVisible){this->isVisible = isVisible;}
	void setName(const QString& name)
	{
		this->name = name;
	}
	const QString& getName(){return name;}
	CanvasLayer& operator=(const CanvasLayer& layer);

	friend QDataStream& operator<<(QDataStream& out, const QSharedPointer<CanvasLayer>& layer);
	friend QDataStream& operator>>(QDataStream& in, QSharedPointer<CanvasLayer>& layer);
private:
	void resetPixels();


	QString name;					// 图层名字
	bool isVisible;

	int width, height;				// 分辨率尺寸
	QImage colorImage, surfaceImage;// 存储像素格式
	float* thickness;				// 本层厚度
};

class Canvas
{
public:
	Canvas();
	~Canvas(void);


	void init( 
		QVector<QVector3D>& vertices, 
		QVector<QVector3D>& normals, 
		QVector<QVector2D>& texcoords, 
		QVector<ObjTriangle>& faces);

	void release();

	void getResolution(int& width, int& height)
	{	width = this->width; height = this->height;}

	// 像素操作
	// 读写当前选中的层的像素值,调用前请确定当前选中了层
	inline QVector4D getCurLayerColorPixel(int x, int y)
	{	unsigned &c = curLayerColorPointer[y * width + x];	
	return QVector4D(R_I2F(c), G_I2F(c), B_I2F(c), A_I2F(c));}

	inline QVector4D getCurLayerSurfacePixel(int x, int y)
	{	unsigned &c = curLayerSurfacePointer[y * width + x];	
	return QVector4D(RL_I2F(c), GL_I2F(c), RR_I2F(c), ID_I2F(c));}

	inline float getCurLayerThicknessPixel(int x, int y)
	{return curLayerThicknessPointer[y * width + x];}

	inline void setCurLayerColorPixel(int x, int y, const QVector4D& pixel)
	{
		unsigned char c[4] = {pixel.z()*255, pixel.y()*255, pixel.x()*255, pixel.w()*255};
		curLayerColorPointer[y * width + x] = *((unsigned int*)c);
	    mergeUpdateRegion(x,y); isDirty = true;
	}
	inline void setCurLayerSurfacePixel(int x, int y, const QVector4D& pixel)
	{
		unsigned char c[4] = {pixel.x()*255, pixel.y()*255, pixel.z()*255, pixel.w()*255};
		curLayerSurfacePointer[y * width + x] = *((unsigned int*)c);
		mergeUpdateRegion(x,y);	isDirty = true;
	}

	inline void setCurLayerColorPixel(int x, int y, unsigned rgba)
	{
		curLayerColorPointer[y * width + x] = rgba;
		mergeUpdateRegion(x,y); isDirty = true;
	}
	inline void setCurLayerSurfacePixel(int x, int y, unsigned surf)
	{
		curLayerSurfacePointer[y * width + x] = surf;
		mergeUpdateRegion(x,y);	isDirty = true;
	}

	inline void setCurLayerThicknessPixel(int x, int y, float thickness)
	{
		curLayerThicknessPointer[y * width + x] = thickness;
		mergeUpdateRegion(x,y);	isDirty = true;
	}

	inline float getTotalThicknessPixel(int x, int y)
	{return totalThick[y * width + x];}
	inline QVector4D getTotalColorPixel(int x, int y)
	{	unsigned &c = totalColor[y * width + x];	
	return QVector4D(R_I2F(c), G_I2F(c), B_I2F(c), A_I2F(c));}

	inline QVector4D getTotalSurfacePixel(int x, int y)
	{	unsigned &c = totalSurf[y * width + x];	
	return QVector4D(RL_I2F(c), GL_I2F(c), RR_I2F(c), ID_I2F(c));}

	inline unsigned* getTriangleMaskImage(){return (unsigned*)triangleMaskImage.scanLine(0);}


	
	// 图层操作
	QSharedPointer<CanvasLayer> generateLayer();	// 生成一个层
	void insertLayer(QSharedPointer<CanvasLayer> pLayer, int ithLayer);
	bool removeLayer(QSharedPointer<CanvasLayer>& pLayer, int ithLayer);
	bool replaceLayer( QSharedPointer<CanvasLayer>& newLayer, QSharedPointer<CanvasLayer>& oldLayer,int ithLayer );
	bool setLayer( QSharedPointer<CanvasLayer>& newLayer,int ithLayer );
	bool moveUpLayer(int ithLayer);
	bool moveDownLayer(int ithLayer);
	void setCurLayerFromImage(const QImage& image, CanvasLayer::LayerChannel channel);
	void getLayerNames(QStringList& names);
	QSharedPointer<CanvasLayer> getCurLayer(){return layers[curSelectedLayer];}
	int  getNumLayers(){return layers.size();}
	int  getCurLayerID(){return curSelectedLayer;}
	void copyCurLayerColorArray(unsigned* colorArray);
	void copyCurLayerSurfaceArray(unsigned* surfArray);
	void copyCurLayerThicknessArray(float* thicknessArray);
	bool selectLayer(int ithLayer);
	void saveCanvas(const QString& fileName, const CanvasLayer::LayerChannel channel);

	// openGL操作
	void updateGLTextures();		// 把修改后的图像发送到openGL
	GLint getGLColorTexObj(){return glColorTexObj;}
	GLint getGLSurfTexObj(){return glSurfTexObj;}
	GLint getGLThicknessTexObj(){return glThicknessTexObj;}
	GLint getGLBaseThicknessTexObj(){return glBaseThicknessTexObj;}

	// 接缝填充相关操作
	CanvasSeamFiller& getSeamFiller(){return seamFiller;}


	friend QDataStream& operator<<(QDataStream& out, const Canvas& canvas);
	friend QDataStream& operator>>(QDataStream& in, Canvas& canvas);

	static const int maxCanvasSize;	// 最大的图像尺寸

private:
	void updateAll();
	void setUpdateRegionEmpty();
	void setUpdateRegionFull();
	void clearArray(unsigned* color, unsigned* surf, float* thick, int nPixels);
	void mergeUpdateRegion(int x, int y);;
	void clearRegion(int xmin, int ymin, int regionWidth, int regionHeight, PixelPos* updatePixels, unsigned updateCount, unsigned* bufColor, unsigned* bufSurf, float * bufThick, int isContinuous);

	int width, height;

	QVector<QSharedPointer<CanvasLayer>> layers;

	// 记录当前选中的层以及对应的指针,指针不需要预先分配空间
	int curSelectedLayer;			
	unsigned int* curLayerColorPointer;
	unsigned int* curLayerSurfacePointer;
	float *       curLayerThicknessPointer;

	// 记录图层的整体效果的数据，指针需要预先分配空间
	float * curLayerBaseThickness;	// 当前图层以下的图层的总厚度

	unsigned int* totalColor;
	unsigned int* totalSurf;
	float*        totalThick;

	// 记录要更新的像素位置
	PixelPos* updatePixelSet;
	int* updatePixelMask;
	unsigned updatePixelCount;
	int updateRegion[2][2];			// 标记需要更新区域 {{xmin,ymin},{xmax,ymax}}
	bool isDirty;					// 表示是否存在没有更新到opengl纹理的像素

	// 减少接缝影响的对象
	QImage triangleMaskImage;		// 记录三角形覆盖像素的图片
	CanvasSeamFiller seamFiller;	// 负责清除裂缝

	// opengl 纹理对象
	GLuint glColorTexObj;			
	GLuint glSurfTexObj;			
	GLuint glThicknessTexObj;
	GLuint glBaseThicknessTexObj;
};

