#pragma once
#define BRUSH_CAM_NEAR_PLANE -1.0f
#define BRUSH_CAM_FAR_PLANE  10.0f
#define BRUSH_DRAW_SEGMENTS  16
#define BRUSH_UPDATE_INIT_BUFSIZE	128 * 128	// 一次最多更新的像素（现在为画布最大大小的1/16）
#define BRUSH_PIXEL_BLEND_THRESHOLD	0.0001	// 权重大于这个值，像素才开始混合
#define BRUSH_BLEND_CURVE_SEGMENTS  50
#define BRUSH_MAX_GE_BUFFERS	5		// 存储的geometry exposer的最多结果数

#define TRIPROJ_PIXEL_RATIO		2.0f	// 影响范围占几个笔刷像素
#define TRIPROJ_WEIGHT_RATIO	0.1f	// 权重系数
class Scene;
// 这个类记录三角形表面的一些数据，用来进行纹理过滤
class TriangleProjData
{
public:
	// 计算一些内部参数，包括坐标范围，以及一个暂存变量
	inline void computeParam()		
	{
		// 计算三角形在UV空间的包围盒,坐标范围不应该超过这个包围盒
		QVector2D* t = triData.texCoord;
		float triUVBBox[2][2] = {{t[0].x(),t[0].y()},{t[0].x(),t[0].y()}};
		triUVBBox[0][0] = min(min(triUVBBox[0][0], t[1].x()), t[2].x());
		triUVBBox[0][1] = min(min(triUVBBox[0][1], t[1].y()), t[2].y());
		triUVBBox[1][0] = max(max(triUVBBox[1][0], t[1].x()), t[2].x());
		triUVBBox[1][1] = max(max(triUVBBox[1][1], t[1].y()), t[2].y());

		float pixelCRx = sqrt(pixelCoordInUVSpace[0][0] * pixelCoordInUVSpace[0][0] + 
								   pixelCoordInUVSpace[1][0] * pixelCoordInUVSpace[1][0]) * TRIPROJ_PIXEL_RATIO / 2;
		float pixelCRy = sqrt(pixelCoordInUVSpace[0][1] * pixelCoordInUVSpace[0][1] + 
								   pixelCoordInUVSpace[1][1] * pixelCoordInUVSpace[1][1]) * TRIPROJ_PIXEL_RATIO / 2;

		uvCoordRange[0] = min(pixelCRx, triUVBBox[1][0] - triUVBBox[0][0]);
		uvCoordRange[1] = min(pixelCRx, triUVBBox[1][1] - triUVBBox[0][1]);

		float deno = pixelCoordInUVSpace[0][0] * pixelCoordInUVSpace[1][1] - 
					 pixelCoordInUVSpace[0][1] * pixelCoordInUVSpace[1][0];
		invPixelCoordDet = 1.0f / (deno * deno * TRIPROJ_WEIGHT_RATIO * TRIPROJ_WEIGHT_RATIO);
	}

	// 获得坐标范围
	QVector2D getCoordRange(){return QVector2D(uvCoordRange[0], uvCoordRange[1]);}

	// 给定纹理空间一点相对于像素坐标系在纹理坐标空间的原点的坐标，计算其在像素坐标系中的权重
	inline float getWeightInEWA(float deltaCoordX, float deltaCoordY)
	{
		float px = deltaCoordX * pixelCoordInUVSpace[1][1] - deltaCoordY * pixelCoordInUVSpace[1][0];
		float py = deltaCoordX * pixelCoordInUVSpace[0][1] - deltaCoordY * pixelCoordInUVSpace[0][0];
		float exponent = (px * px + py * py) * invPixelCoordDet + 1.0f;
// 		int isBigger = exponent > 29.0;
// 		return expTable[int(exponent) * (1 - isBigger) + 29 * isBigger]; 
		return exponent / (exponent * exponent + 1);
	}

	int objID, faceID;
	TriangleData triData;					// 三角形的几何信息
	float pixelCoordInUVSpace[2][2];		// 映射到物体纹理空间的像素坐标系
	QMatrix4x4 modelMatrix;					// 物体的变换矩阵

	float uvCoordRange[2];					// 坐标范围uvC(oord)R(ange),即其在纹理空间影响的uv范围
											// 真正影响的范围为[-uvCR.x, uvCR.x],[-uvCR.y,uvCR.y]
	float invPixelCoordDet;					// 计算时的分母， 等于 1 / (det ^2 * TRIPROJ_WEIGHT_RATIO)

	static float expTable[30];				// 函数 exp(-x) 的查找表
};



struct PixelUpdateItem 
{
	QVector4D rgba;						// 颜色属性
	QVector4D surf;						// 表面属性
	float thickness;					// 厚度
	float weight;						// 权重
	unsigned x,y;						// 像素位置
	float norProj, depth;				// 法线投影和深度
};

struct PixelUpdateArray
{
public:
	PixelUpdateArray()
	{
		curUpdateCount = 0;
		curArraySize = BRUSH_UPDATE_INIT_BUFSIZE;
		int bitmapSize = Canvas::maxCanvasSize * Canvas::maxCanvasSize;
		updateBitmap = new int[bitmapSize];
		memset(updateBitmap, 0xff, bitmapSize * sizeof(int));
		updateArray = new PixelUpdateItem[curArraySize];
	}
	~PixelUpdateArray()
	{
		delete[] updateBitmap;
		delete[] updateArray;
	}
	inline void clearBitmapItem(int ithItem)
	{	updateBitmap[ithItem] = 0xffffffff;	}

	void reset(){curUpdateCount=0;}

	inline int  getBitmapItem(int ithItem)
	{return updateBitmap[ithItem];}

	inline PixelUpdateItem& getUpdateItem(int ithItem)
	{
		return updateArray[ithItem];
	}

	inline int getUpdatePixelCount(){return curUpdateCount;}

	void insertItem(int ithItem, const PixelUpdateItem& item)
	{
		if (curUpdateCount >= curArraySize)
		{
			// 重新分配空间
			curArraySize *= 2;
			PixelUpdateItem* newArray = new PixelUpdateItem[curArraySize];
			memcpy(newArray,  updateArray,  curUpdateCount * sizeof(PixelUpdateItem));
			delete[] updateArray;
			updateArray  = newArray;
			qDebug() << "cur array size: " 
				<< curArraySize * sizeof(PixelUpdateItem) / 1024.0f / 1024.0f  << "MB"<< endl;
		}
		if (updateBitmap[ithItem] == -1)
		{
			// 如果遇到新的像素，在像素更新表中插入一项
			updateBitmap[ithItem] = curUpdateCount;
			updateArray[curUpdateCount++] = item;
		}
		else
		{
			// 否则，加入到原来对应的样本中
			PixelUpdateItem& curItem = updateArray[updateBitmap[ithItem]];
			curItem.rgba += item.rgba;
			curItem.surf += item.surf;
			curItem.thickness += item.thickness;
			curItem.weight += item.weight;
			curItem.norProj += item.norProj;
			curItem.depth += item.depth;
		}
	}
private:
	int* updateBitmap;				// 计算过程中暂存更新位置的位图
	PixelUpdateItem* updateArray;	// 记录像素更新位置的位图
	int curArraySize;					// 当前分配缓冲区大小
	int curUpdateCount;				// 当前要更新的像素个数
};



class Brush
{
	friend class PaintThread;
public:
	enum BrushMode{MODE_BRUSH, MODE_ERASER};
	Brush(Scene* scene);
	~Brush();

	void setMode(BrushMode mode);
	void setSize(float size);
	void setResolutionRatio(float ratio);
	void setAngle(float angle){this->angle = angle;}
	void setRatio(float ratio){this->ratio = ratio;}
	void setSizeJitter(float sizeJitter){this->sizeJitter = sizeJitter;}
	void setAngleJitter(float angleJitter){this->angleJitter = angleJitter;}
	void setRatioJitter(float ratioJitter){this->ratioJitter = ratioJitter;}
	void setOffsetJitter(float offsetJitter){this->offsetJitter = offsetJitter;}
	void setDepthRange(float nearPlane, float farPlane)
	{ this->nearPlane = nearPlane; this->farPlane = farPlane;	}

	QSharedPointer<Stroke> getStroke(){return stroke;}
	void setStroke(QSharedPointer<Stroke> stroke){this->stroke = stroke;}


	void activate(bool isActive){this->isActive = isActive;}

	// 绘画前调用此函数，准备历史记录
	void beginPaint();
	// 添加要画的笔刷位置，添加完成后，调用worldPaint统一画出来
	void addPaintPos(const QVector3D& ori, const QVector3D& x, const QVector3D& y, const QVector3D& z);
	// worldPaint 
	// 在世界空间上画画，画笔可能附着在物体表面，或者某种约束表面
	// 这种情况下，需要先在放置的位置把物体渲染一次，得到表面几何信息，
	// 再把像素映射到纹理空间。
	void worldPaint();
	// 绘画后调用此函数，生成历史记录
	void endPaint();;
	// 设置画笔在世界空间的位置
	void setPosWorld(const QVector3D& ori, const QVector3D& x, const QVector3D& y, const QVector3D& z);

	// 在屏幕空间上画画，画笔被约束在屏幕空间，情形跟二维绘图类似
	// 这种情况下，不需要先渲染一次，而是通过读取场景表面信息，把像素映
	// 射到物体纹理空间。
	// 输入参数为单位化的屏幕坐标（注意屏幕坐标系原点在左上角）
	void setPosScreen(const QVector2D& screenRatio, const QVector2D &dir){}
	// 设置画笔在屏幕空间的位置
	void screenPaint(){}

	// 显示笔刷
	void draw();

	PurePaintPicker& getPurePaintPicker(){return *(purePaintPicker.data());}
	PaintPicker*     getCurPaintPicker(){return curPaintPicker.data();}
	// 获得当前用于放置画笔的对象ID
	// 若当前画笔采用纯色，则对象ID为要画的物体的ID，即画笔附着在物体表面画
	// 否则若采用各种几何形体的PaintPicker，对象ID为该PaintPicker的Mesh对象的ID
	int   getPlacerObjectID();		

	// 绘制参数
	float getInterval()const{return intervalRatio * size;}
	void  setIntervalRatio(float i){intervalRatio = i;}
	void  setObject(QWeakPointer<Mesh> obj);
	void  setColor(const QVector4D& colorParam, const QVector4D& surfParam, const float depth);
	void  setDefaultPaintPicker();
	void setCustomPaintPicker( QSharedPointer<PaintPicker> picker );
	// 设定是否影响某个通道
	void  setAffectColor(int isAffect);
	void  setAffectReflLevel(int isAffect);
	void  setAffectRefrLevel(int isAffect);
	void  setAffectGlossness(int isAffect);
	void  setAffectIOR(int isAffect);
	void  setAffectThickness(int isAffect);

	// 法线/深度调制
	void  setNormalBlendCurveCoef(float zeroPos, float onePos, float zeroWeight, float oneWeight);
	void  setDepthBlendCurveCoef(float zeroPos, float onePos, float zeroWeight, float oneWeight);

	static void computeBlendCurve(float* curve, int nSegs, float zeroPos, float onePos, float zeroWeight, float oneWeight);
	void setDepthAffectMode(GeometryExposer::DepthType mode);

public:
	bool  getAffectColor(){return isAffectColor == 1.0f;}
	bool  getAffectReflLevel(){return isAffectReflLevel == 1.0f;}
	bool  getAffectRefrLevel(){return isAffectRefrLevel == 1.0f;}
	bool  getAffectGlossness(){return isAffectGlossness == 1.0f;}
	bool  getAffectIOR(){return isAffectIOR == 1.0f;}
	bool  getAffectThickness(){return isAffectThickness == 1.0f;}

private:
	void setResolution(int x, int y);

	// 这个函数根据属性值，更新当前的属性
	void updateCurParams();
	void paintToObject(int ithBuffer);// 把像素画到模型上
	TriangleProjData getTriangleData( int triID, int objID);
	// 计算出一个像素映射到三角形纹理空间的坐标
	bool computeTriangleProj(TriangleProjData& data);

	inline float getRandomFloat_01()
	{randInt = randInt * 32773 + 239664539; return float(randInt) / 4294967295.0f;}
	inline float getRandomFloat_m11()
	{ randInt = randInt * 32773 + 239664539; return float(randInt) / 2147483647.5 - 1.0;}

	// ---------------------------------------------------------
	// 以下属性可以在界面上设置，在再次被设置之前，值不会变化
	// ---------------------------------------------------------
	QWeakPointer<Mesh> curObj;				// 当前绘制的对象
	QSharedPointer<PurePaintPicker> purePaintPicker;		// 存储笔刷颜色
	QWeakPointer<PaintPicker> curPaintPicker;			// 当前使用的拾色器
	bool isActive;							// 是否可见
	int  resolution[2];						// 分辨率
	float resRatio;							// 大小与分辨率的比率
	float intervalRatio;					// 绘制间隔
	float nearPlane;						// 近剪切平面
	float farPlane;							// 远剪切平面

	float size;								// 大小
	float angle;							// 角度
	float ratio;							// 高宽比（高/宽）

	// 一些抖动值，范围为0-1，
	// 计算实际参数时按照 （1 + jitter * rnd） * param计算
	float sizeJitter;						// 大小抖动
	float angleJitter;						// 角度抖动
	float offsetJitter;						// 偏移抖动
	float ratioJitter;						// 高宽比抖动

	float isAffectColor;					// 是否影响对应属性
	float isAffectReflLevel;
	float isAffectRefrLevel;
	float isAffectGlossness;
	float isAffectIOR;
	float isAffectThickness;

	float isBrushMode;						// 是否笔刷模式，否则为橡皮擦模式

	QSharedPointer<Stroke> stroke;			// 笔触

	// 绘画过程中用到的混合函数的查找表
	float normalBlendCurve[BRUSH_BLEND_CURVE_SEGMENTS];
	float depthBlendCurve[BRUSH_BLEND_CURVE_SEGMENTS];

	// ---------------------------------------------------------
	// 以下属性可能会在每次绘制前更新（当对属性指定随机值的时候）
	// ---------------------------------------------------------
	unsigned int randInt;					// 随机数
	float curAngle;							// 旋转角度
	QVector2D curSize;						// 大小（根据size和高宽比算出）
	float curOffset[2];						// 偏移量
	QVector3D paintLocalCoord[3];			// 用于画画的局部坐标
	QVector3D paintOrigin;					// 用于画画的坐标原点
	QMatrix4x4 viewMatrix, projMatrix;		// 观察矩阵（由paintLocalCoord，paintOrigin生成）

	// ---------------------------------------------------------
	// 以下属性用于绘制时的线程同步
	// ---------------------------------------------------------
	struct PaintPosition
	{
		QVector3D ori;
		QVector3D localCoord[3];
		QVector3D paintOri;
		QVector3D paintLocalCoord[3];
	};
	struct GeometryInfoBuffer
	{
		GeometryExposer::NormalTexPixelData* norArray;
		GeometryExposer::UVTexPixelData* uvArray;
	};
	// 将要进行绘制的位置
	QVector<PaintPosition> paintPos;	
	// 以下为用于线程间通信的变量
	QSemaphore* emptySlots, *fullSlots;
	QMutex paintMutex;
	// 存放GeometryExposer结果的数组
	GeometryInfoBuffer geoInfoBuf[BRUSH_MAX_GE_BUFFERS];


	// ---------------------------------------------------------
	// 以下属性用于历史记录
	// ---------------------------------------------------------
	// 绘制之前的图层属性
	QVector<unsigned> oldColorBuf, oldSurfBuf;
	QVector<float> oldThicknessBuf;
	// ---------------------------------------------------------
	// 以下属性在鼠标每次移动时会更新
	// ---------------------------------------------------------
	QVector3D dispLocalCoord[3];			// 用于显示笔刷图标的局部坐标系
	QVector3D dispOrigin;					// 用于显示笔刷图标的坐标原点

	// ---------------------------------------------------------
	// 这个属性由界面设置
	// ---------------------------------------------------------
	GeometryExposer  brushExposer;			// 世界空间画画时，获得场景几何体信息

	// ---------------------------------------------------------
	// 场景数据
	// ---------------------------------------------------------
	Scene* scene;							// 场景
	GeometryExposer* sceneExposer;			// 记录场景当前状况
	Camera* sceneCamera;					// 场景摄像机

	// ---------------------------------------------------------
	// 以下为绘画过程中用到的临时变量，由于频繁申请释放空间十分占时间，所以设置为静态变量
	// ---------------------------------------------------------
	static PixelUpdateArray updateArray;
};

class PaintThread : public QThread
{
	Q_OBJECT
public:
	PaintThread(Brush* brush, QObject *parent = 0);
	~PaintThread(){};

protected:
	void run();
private:
	Brush* brush;
};

