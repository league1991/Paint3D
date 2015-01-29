#pragma once
#define SAMPLE_PIXEL_INTERVAL 1.f		// 每隔多少个像素采样一次 
#define BRUSH_CAMERA_DISTANCE 0.05f		// 画笔摄像机离表面的距离
// 笔刷放置器负责将鼠标坐标转换成笔刷的实际位置
class Painter
{
public:
	Painter(Scene* s);;
	~Painter(){}

	// 设置画画的对象
	void setObject(QWeakPointer<Mesh> obj){curObj = obj;}
	// 设置笔刷
	void setBrush(QWeakPointer<Brush> b){curBrush = b;}

	// 画画，当鼠标左键按下时，调用 beginPaint
	// 鼠标拖动时，调用 goOnPaint
	// 鼠标左键松开时，调用 endPaint
	// 为了使笔刷连续，会对位置进行插值
	virtual void beginPaint(const QVector2D& beginPos) = 0;
	virtual void goOnPaint(const QVector2D& newPos) = 0;
	virtual void endPaint(const QVector2D& endPos) = 0;

	virtual void onMouseHover( const QVector2D& newPos ) = 0;

protected:
	bool getWorldPos(const QVector2D& ratio, QVector3D& pos);

	bool isPainting;
	QVector2D lastScreenPos;	// 上一次移动笔刷的屏幕位置
	float paintLength;			// 画下曲线的总长
	float deltaLength;			// 上一次画下后画笔又经过的弧长

	QWeakPointer<Mesh> curObj;
	QWeakPointer<Brush> curBrush;
	Scene* scene;
};

// 会把画笔放置在物体表面
class SurfacePainter:public Painter
{
public:
	SurfacePainter(Scene* s);
	~SurfacePainter(){}

	void beginPaint( const QVector2D& beginPos );
	void goOnPaint(const QVector2D& newPos);
	void endPaint(const QVector2D& endPos);

	void onMouseHover( const QVector2D& newPos );
protected:
	//QVector3D lastWorldPos;
};