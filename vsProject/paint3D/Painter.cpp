#include "stdafx.h"
#include "Painter.h"

Painter::Painter( Scene* s )
{
	scene = s;
	isPainting = false;
}


bool Painter::getWorldPos( const QVector2D& ratio, QVector3D& pos )
{
	const GeometryExposer& exposer = scene->getScreenExposer();
	const Camera& camera = scene->getCamera();
	QVector3D normal;
	float depth;
	bool res = exposer.getNormalAndDepth(ratio, normal, depth);
	if (res)
		pos = camera.getWorldPos(ratio.x(), ratio.y(), depth);
	return res;
}

SurfacePainter::SurfacePainter( Scene* s ):Painter(s)
{

}

void SurfacePainter::beginPaint( const QVector2D& beginPos )
{
	isPainting = !curObj.isNull();
	paintLength = 0.0f;
	deltaLength = curBrush.data()->getInterval();
	lastScreenPos = beginPos;
	if (curBrush)
	{
		curBrush.data()->beginPaint();
	}
}

void SurfacePainter::goOnPaint( const QVector2D& newScreenPos )
{
	if (curObj.isNull() || curBrush.isNull() || !isPainting)
		return;

	GeometryExposer& shutter = scene->getScreenExposer();
	Camera& camera = scene->getCamera();
	Brush* brush = curBrush.data();
	float brushInterval = brush->getInterval();

	QVector2D deltaScreenPos = newScreenPos - lastScreenPos;
	QVector2D deltaScreenRatio = camera.getScreenRatio(deltaScreenPos);
	int nSubds = min(max(deltaScreenPos.length() / SAMPLE_PIXEL_INTERVAL,1),50);
	QVector2D dRatio = deltaScreenRatio / float(nSubds);

	QVector3D viewPoint = camera.getOrigin();
	QVector2D lastScreenRatio = camera.getScreenRatio(lastScreenPos);
	QVector2D curScreenRatio = lastScreenRatio + dRatio;
	QVector3D curWorldPos, oldWorldPos;

	// 检查上一次的屏幕位置是否存在物体
	QVector3D lastNormal; float lastDepth;
	bool isLastRatioHitObject = shutter.getNormalAndDepth(lastScreenRatio, lastNormal, lastDepth);
	if (isLastRatioHitObject)
		oldWorldPos = camera.getWorldPos(lastScreenRatio.x(), lastScreenRatio.y(), lastDepth);

	int placeObjID = brush->getPlacerObjectID();
	for (int i = 0; i < nSubds; i++, curScreenRatio += dRatio)
	{
		QVector2D texCoord;
		float depth;
		unsigned char objID;
		unsigned int faceID;
		QVector3D normal;
		bool hasObject = shutter.getAll(curScreenRatio, normal, depth, texCoord, objID, faceID);
		if(hasObject && objID == placeObjID)
		{
			// 当发现当前位置存在可供放置画笔的物体时，检查上一个位置是否击中物体，
			// 若是，则可以放置GE渲染； 若否，则只记录当前位置
			brush->activate(true);
			if (isLastRatioHitObject)
			{
				// 统计笔尖经过的弧长
				curWorldPos = camera.getWorldPos(curScreenRatio.x(), curScreenRatio.y(), depth);
				QVector3D x = curWorldPos - oldWorldPos;
				float l = x.length();
				paintLength += l;
				deltaLength += l;

				// 放置画笔
				QVector3D z = camera.getWorldNormalFromView(normal);
				if (QVector3D::dotProduct(curWorldPos - viewPoint,z) < 0)
					z *= -1;
				QVector3D y = QVector3D::crossProduct(x, z);
				y.normalize();
				x = QVector3D::crossProduct(z, y);
				QVector3D cameraOri = curWorldPos + z * -BRUSH_CAMERA_DISTANCE;
				brush->setPosWorld(cameraOri, x, y, z);
				// 当长度大于画笔间隔时，开始画画
				if (deltaLength > brushInterval)
				{
					deltaLength -= brushInterval;
					brush->addPaintPos(cameraOri, x, y, z);
				}
				oldWorldPos = curWorldPos;
			}
			else
			{
				oldWorldPos = camera.getWorldPos(curScreenRatio.x(), curScreenRatio.y(), depth);
				isLastRatioHitObject = true;
			}
		}
		else
		{
			isLastRatioHitObject = false;
			brush->activate(false);
		}		
	}
	brush->worldPaint();
	curObj.data()->updateGLTextures();
	lastScreenPos = newScreenPos;
}

void SurfacePainter::endPaint( const QVector2D& endPos )
{
	if (curBrush)
	{
		curBrush.data()->endPaint();
	}
	paintLength = 0.0f;
	deltaLength = 0.0f;
	isPainting =false;
}

void SurfacePainter::onMouseHover( const QVector2D& newPos )
{
	if (curObj.isNull() || curBrush.isNull() || isPainting)
		return;	

	GeometryExposer& shutter = scene->getScreenExposer();
	Camera& camera = scene->getCamera();
	Brush* brush = curBrush.data();

	QVector3D viewPoint = camera.getOrigin();
	QVector2D newRatio = camera.getScreenRatio(newPos);
	QVector2D texCoord;
	float depth;
	unsigned char objID;
	unsigned int faceID;
	QVector3D normal;
	bool hasObject = shutter.getAll(newRatio, normal, depth, texCoord, objID, faceID);
	if(hasObject && brush->getPlacerObjectID() == objID)
	{
		brush->activate(true);
		QVector3D newWorldPos = camera.getWorldPos(newRatio.x(), newRatio.y(), depth);
		QVector3D x(1,0,0);// = newWorldPos - lastWorldPos;
		QVector3D z = camera.getWorldNormalFromView(normal);
		if (QVector3D::dotProduct(newWorldPos - viewPoint,z) < 0)
			z *= -1;
		QVector3D y = QVector3D::crossProduct(x, z);
		y.normalize();
		x = QVector3D::crossProduct(z, y);
		QVector3D cameraOri = newWorldPos + z * -BRUSH_CAMERA_DISTANCE;
		brush->setPosWorld(cameraOri, x, y, z);
		//lastWorldPos = newWorldPos;
	}
	else
	{
		brush->activate(false);
	}
	lastScreenPos = newPos;
}
