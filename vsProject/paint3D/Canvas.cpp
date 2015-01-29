#include "StdAfx.h"
#include "Canvas.h"

const int Canvas::maxCanvasSize = CANVAS_MAX_SIZE;

CanvasLayer::CanvasLayer()
{
	width = height = 0;
	thickness = NULL;
	isVisible = true;
}
CanvasLayer::CanvasLayer(int width, int height):
		colorImage(width, height, QImage::Format_ARGB32),
		surfaceImage(width, height, QImage::Format_ARGB32)
{
	this->width = width;
	this->height = height;
	isVisible = true;
	thickness = new float[width * height];
	resetPixels();
}

CanvasLayer::CanvasLayer( const CanvasLayer& layer )
{
	width = layer.width; height = layer.height;
	if (thickness)
	{
		delete[] thickness;
	}
	thickness = new float[width * height];
	memcpy(thickness, layer.thickness, sizeof(float) * width * height);
	colorImage = layer.colorImage;
	surfaceImage = layer.surfaceImage;
	name = layer.name;
}




void CanvasLayer::resetPixels()
{
	unsigned* colorBuf = (unsigned*)colorImage.scanLine(0);
	unsigned* surfaceBuf = (unsigned*)surfaceImage.scanLine(0);
	for (int i = 0; i < width * height; ++i)
	{
		thickness[i] = INIT_THICKNESS;
		colorBuf[i] = INIT_COLOR_BITS;
		surfaceBuf[i] = INIT_SURF_BITS;
	}
}

void CanvasLayer::blendRegion(int xmin, int ymin, int regionWidth, int regionHeight, 
							  unsigned* bufColor, 
							  unsigned* bufSurf, 
							  float*    bufThick,
							  int       isContinuous)
{
	if (!isVisible)
		return;
	unsigned offset = ymin * width + xmin;
	unsigned* layerColor = (unsigned*)colorImage.constBits() + offset;
	unsigned* layerSurf  = (unsigned*)surfaceImage.constBits() + offset;
	float   * layerThickness = (float*)thickness + offset;
	unsigned step = width - regionWidth;
	int       bufOffset  = offset * (1 - isContinuous);
	int       bufStep    = step   * (1 - isContinuous);
	bufColor += bufOffset;
	bufThick += bufOffset;
	bufSurf  += bufOffset;

	for (int y = ymin; y < ymin + regionHeight; ++y)
	{
		for (int x = xmin; x < xmin + regionWidth; 
			 ++x, ++bufColor, ++bufSurf, ++bufThick, ++layerColor, ++layerSurf, ++layerThickness)
		{
			unsigned char* srcColor = (unsigned char*)bufColor;
			unsigned char* srcSurf  = (unsigned char*)bufSurf;
			float   & srcThick = *bufThick;

			unsigned char* curColor = (unsigned char*)layerColor;
			unsigned char* curSurf  = (unsigned char*)layerSurf;
			float   & curThick = *layerThickness;
/*
			float srcAlpha = srcColor[3] / 255.0f;
			float curAlpha = curColor[3] / 255.0f;

			srcColor[0] = srcColor[0] * (1-curAlpha) + curColor[0] * curAlpha;
			srcColor[1] = srcColor[1] * (1-curAlpha) + curColor[1] * curAlpha;
			srcColor[2] = srcColor[2] * (1-curAlpha) + curColor[2] * curAlpha;
			srcColor[3] = (1 - (1 - srcAlpha) * (1 - curAlpha)) * 255;
			srcSurf[0] =  srcSurf[0] * (1-curAlpha) + curSurf[0] * curAlpha;
			srcSurf[1] =  srcSurf[1] * (1-curAlpha) + curSurf[1] * curAlpha;
			srcSurf[2] =  srcSurf[2] * (1-curAlpha) + curSurf[2] * curAlpha;
			srcSurf[3] =  srcSurf[3] * (1-curAlpha) + curSurf[3] * curAlpha;
			srcThick   =  srcThick + curThick * curAlpha;*/

			float curAlpha = curColor[3] / 255.0f;
			float colorBlendFactor = curAlpha * curSurf[2] / 255.0f;
			float invColorBlendFactor = 1 - colorBlendFactor;
			srcColor[0] = srcColor[0] * invColorBlendFactor + curColor[0] * colorBlendFactor;
			srcColor[1] = srcColor[1] * invColorBlendFactor + curColor[1] * colorBlendFactor;
			srcColor[2] = srcColor[2] * invColorBlendFactor + curColor[2] * colorBlendFactor;

			float srcAlpha = srcColor[3] / 255.0f;
			srcColor[3] = (1 - (1 - srcAlpha) * (1 - curAlpha)) * 255;

			float invAlpha = 1 - curAlpha;
			srcSurf[0]  = srcSurf[0]  * invAlpha + curSurf[0] * curAlpha;
			srcSurf[1]  = srcSurf[1]  * invAlpha + curSurf[1] * curAlpha;
			srcSurf[2]  = srcSurf[2]  * invAlpha + curSurf[2] * curAlpha;
			srcSurf[3]  = srcSurf[3]  * invAlpha + curSurf[3] * curAlpha;

			srcThick   =  srcThick + curThick * curAlpha;
		}
		layerColor += step;
		layerSurf  += step;
		layerThickness += step;
		bufColor   += bufStep;
		bufSurf    += bufStep;
		bufThick   += bufStep;
	}
}
void CanvasLayer::blendRegion(int xmin, int ymin, 
							  int regionWidth, int regionHeight, 
							  PixelPos* updatePixels,
							  unsigned updateCount,
							  unsigned* bufColor, 
							  unsigned* bufSurf, 
							  float* bufThick,
							  int isContinuous)
{
	if (!isVisible)
		return;
	unsigned* layerColor = (unsigned*)colorImage.constBits();
	unsigned* layerSurf  = (unsigned*)surfaceImage.constBits();
	for(int ithPixel = 0; ithPixel < updateCount; ++ithPixel, ++updatePixels)
	{
		PixelPos& pixelPos = *updatePixels;
		int globalOffset = pixelPos.y * width + pixelPos.x;
		int regionOffset = (pixelPos.y - ymin) * regionWidth + pixelPos.x - xmin;
		int bufOffset    = (1 - isContinuous) * globalOffset + isContinuous * regionOffset;

		unsigned char* srcColor = (unsigned char*)(bufColor + bufOffset);
		unsigned char* srcSurf  = (unsigned char*)(bufSurf  + bufOffset);
		unsigned char* curColor = (unsigned char*)(layerColor + globalOffset);
		unsigned char* curSurf  = (unsigned char*)(layerSurf + globalOffset);

		float curAlpha = curColor[3] / 255.0f;
		float colorBlendFactor = curAlpha * curSurf[2] / 255.0f;
		float invColorBlendFactor = 1 - colorBlendFactor;
		srcColor[0] = srcColor[0] * invColorBlendFactor + curColor[0] * colorBlendFactor;
		srcColor[1] = srcColor[1] * invColorBlendFactor + curColor[1] * colorBlendFactor;
		srcColor[2] = srcColor[2] * invColorBlendFactor + curColor[2] * colorBlendFactor;

		float srcAlpha = srcColor[3] / 255.0f;
		srcColor[3] = (1 - (1 - srcAlpha) * (1 - curAlpha)) * 255;

		float invAlpha = 1 - curAlpha;
		srcSurf[0]  = srcSurf[0]  * invAlpha + curSurf[0] * curAlpha;
		srcSurf[1]  = srcSurf[1]  * invAlpha + curSurf[1] * curAlpha;
		srcSurf[2]  = srcSurf[2]  * invAlpha + curSurf[2] * curAlpha;
		srcSurf[3]  = srcSurf[3]  * invAlpha + curSurf[3] * curAlpha;

		bufThick[bufOffset] = bufThick[bufOffset] + thickness[globalOffset] * curAlpha;
	}
}
void CanvasLayer::blendRegion(int xmin, int ymin, 
							  int regionWidth, int regionHeight, 
							  float*    bufThick,
							  int isContinuous)
{
	if (!isVisible)
		return;
	unsigned offset = ymin * width + xmin;
	unsigned* layerColor = (unsigned*)colorImage.constBits() + offset;
	float   * layerThickness = (float*)thickness + offset;
	unsigned step = width - regionWidth;
	int       bufOffset  = offset * (1 - isContinuous);
	int       bufStep    = step   * (1 - isContinuous);
	bufThick += bufOffset;

	for (int y = ymin; y < ymin + regionHeight; ++y)
	{
		for (int x = xmin; x < xmin + regionWidth; 
			++x, ++bufThick, ++layerColor,  ++layerThickness)
		{
			float   & srcThick = *bufThick;

			unsigned char* curColor = (unsigned char*)layerColor;
			float   & curThick = *layerThickness;

			float curAlpha = curColor[3] / 255.0f;

			srcThick   =  srcThick + curThick * curAlpha;
		}
		layerColor += step;
		layerThickness += step;
		bufThick += bufStep;
	}
}
void CanvasLayer::blendRegion(int xmin, int ymin, 
							  int regionWidth, int regionHeight, 
							  PixelPos*updatePixels,
							  unsigned updateCount,
							  float* bufThick,
							  int isContinuous)
{
	if (!isVisible)
		return;
	unsigned* layerColor = (unsigned*)colorImage.constBits();
	for(int ithPixel = 0; ithPixel < updateCount; ++ithPixel, ++updatePixels)
	{
		PixelPos& pixelPos = *updatePixels;
		int globalOffset = pixelPos.y * width + pixelPos.x;
		int regionOffset = (pixelPos.y - ymin) * regionWidth + pixelPos.x - xmin;
		unsigned char* curColor = (unsigned char*)(layerColor + globalOffset);
		float curAlpha = curColor[3] / 255.0f;
		float& thickVal = bufThick[(1 - isContinuous) * globalOffset + isContinuous * regionOffset];
		thickVal = thickVal + thickness[globalOffset] * curAlpha;
		bufThick++;
	}
}
void CanvasLayer::setRegion(int xmin, int ymin, int regionWidth, int regionHeight, 
							unsigned* bufColor, 
							unsigned* bufSurf, 
							float*    bufThick,
							int       isContinuous)
{
	if (!isVisible)
		return;
	unsigned offset = ymin * width + xmin;
	unsigned* layerColor = (unsigned*)colorImage.constBits() + offset;
	unsigned* layerSurf  = (unsigned*)surfaceImage.constBits() + offset;
	float   * layerThickness = (float*)thickness + offset;
	unsigned step = width - regionWidth;
	int       bufOffset  = offset * (1 - isContinuous);
	int       bufStep    = step   * (1 - isContinuous);
	bufColor += bufOffset;
	bufThick += bufOffset;
	bufSurf  += bufOffset;

	for (int y = ymin; y < ymin + regionHeight; ++y)
	{
		for (int x = xmin; x < xmin + regionWidth; 
			++x, ++bufColor, ++bufSurf, ++bufThick, ++layerColor, ++layerSurf, ++layerThickness)
		{
			*bufColor = *layerColor;
			*bufSurf  = *layerSurf;
			*bufThick = *layerThickness;
		}
		layerColor += step;
		layerSurf  += step;
		layerThickness += step;
		bufColor   += bufStep;
		bufSurf    += bufStep;
		bufThick   += bufStep;
	}
}

void CanvasLayer::setRegion(int xmin, int ymin, 
							int regionWidth, int regionHeight,
							float*    bufThick,
							int isContinuous)
{
	if (!isVisible)
		return;
	unsigned offset = ymin * width + xmin;
	float   * layerThickness = (float*)thickness + offset;
	unsigned step = width - regionWidth;
	int       bufOffset  = offset * (1 - isContinuous);
	int       bufStep    = step   * (1 - isContinuous);
	bufThick += bufOffset;

	for (int y = ymin; y < ymin + regionHeight; ++y)
	{
		for (int x = xmin; x < xmin + regionWidth; 
			++x, ++bufThick, ++layerThickness)
		{
			*bufThick = *layerThickness;
		}
		unsigned step = width - regionWidth;
		layerThickness += step;
		bufThick   += bufStep;
	}
}

void CanvasLayer::setRegion(int xmin, int ymin, 
							int regionWidth, int regionHeight,
							PixelPos*updatePixels,
							unsigned updateCount,
							float* bufThick,
							int isContinuous)
{
	if (!isVisible)
		return;
	for(int ithPixel = 0; ithPixel < updateCount; ++ithPixel, ++updatePixels)
	{
		PixelPos& pixelPos = *updatePixels;
		int globalOffset = pixelPos.y * width + pixelPos.x;
		int regionOffset = (pixelPos.y - ymin) * regionWidth + pixelPos.x - xmin;
		int bufOffset    = (1 - isContinuous) * globalOffset + isContinuous * regionOffset;
		bufThick[bufOffset] = thickness[globalOffset];
	}
}

void CanvasLayer::setRegion(int xmin, int ymin, 
							int regionWidth, int regionHeight,
							PixelPos*updatePixels,
							unsigned updateCount,
							unsigned* bufColor,
							unsigned* bufSurf,
							float * bufThick,
							int isContinuous)
{
	if (!isVisible)
		return;
	unsigned* layerColor = (unsigned*)colorImage.constBits();
	unsigned* layerSurf  = (unsigned*)surfaceImage.constBits();
	for(int ithPixel = 0; ithPixel < updateCount; ++ithPixel, ++updatePixels)
	{
		PixelPos& pixelPos = *updatePixels;
		int globalOffset = pixelPos.y * width + pixelPos.x;
		int regionOffset = (pixelPos.y - ymin) * regionWidth + pixelPos.x - xmin;
		int bufOffset    = (1 - isContinuous) * globalOffset + isContinuous * regionOffset;
		bufThick[bufOffset] = thickness[globalOffset];
		bufColor[bufOffset] = layerColor[globalOffset];
		bufSurf[bufOffset] = layerSurf[globalOffset];
	}
}
CanvasLayer::~CanvasLayer()
{
	if (thickness)
	{
		delete[] thickness;
	}
}

CanvasLayer& CanvasLayer::operator=( const CanvasLayer& layer )
{
	colorImage = layer.colorImage;
	surfaceImage = layer.surfaceImage;
	width = layer.width;
	height = layer.height;
	thickness = new float[width * height];
	memcpy(thickness, layer.thickness, sizeof(float) * width * height);
	isVisible = layer.isVisible;
	name = layer.name;
	return *this;
}

void CanvasLayer::setLayerFromImage( const QImage& image, const LayerChannel channel )
{
	QImage scaledImg = image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	if (channel == CHNL_COLOR)
	{
		colorImage = scaledImg;
	}
	else if (channel == CHNL_THICKNESS)
	{
		QRgb* srcPixel = (QRgb*)scaledImg.constBits();
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel, ++srcPixel)
		{
			thickness[ithPixel] = qGray(*srcPixel) / 255.0f;
		}
	}
	else
	{	// surface image 的通道
		QRgb* srcPixel = (QRgb*)scaledImg.constBits();
		unsigned* dstPixel = (unsigned*)surfaceImage.bits();
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel, ++srcPixel)
		{
			unsigned char* pChannel = (unsigned char*)(dstPixel + ithPixel);
			pChannel[channel] = qGray(*srcPixel);
		}
	}
}


void CanvasLayer::saveLayer( const QString& fileName, const LayerChannel channel )
{
	QImage saveImg = colorImage;
	if (channel == CHNL_THICKNESS)
	{
		float maxThick = FLT_MIN;
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel)
		{
			maxThick = max(maxThick, thickness[ithPixel]);
		}
		QRgb* srcPixel = (QRgb*)saveImg.bits();
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel, ++srcPixel)
		{
			char grayness = thickness[ithPixel] / maxThick * 255.0f;
			*srcPixel = qRgba(grayness, grayness, grayness, qAlpha (*srcPixel));
		}
	}
	else if (channel != CHNL_COLOR)
	{	// surface image 的通道
		QRgb* dstPixel = (QRgb*)saveImg.bits();
		unsigned* srcPixel = (unsigned*)surfaceImage.bits();
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel, ++srcPixel, ++dstPixel)
		{
			unsigned char* pChannel = (unsigned char*)(srcPixel);
			*dstPixel = qRgba(pChannel[channel], pChannel[channel], pChannel[channel], 255);
		}
	}
	saveImg.save(fileName);
}


Canvas::Canvas()
{
	curLayerColorPointer = curLayerSurfacePointer = NULL;
	curLayerThicknessPointer = NULL;
	curLayerBaseThickness = NULL;
	totalColor = totalSurf = NULL;
	totalThick = NULL;
	updatePixelSet = NULL;
	updatePixelMask = NULL;
	width = height = 0;
	curSelectedLayer = -1;
}

Canvas::~Canvas(void)
{
	this->release();
}

void Canvas::init(
				  QVector<QVector3D>& vertices,
				  QVector<QVector3D>& normals,
				  QVector<QVector2D>& texcoords,
				  QVector<ObjTriangle>& faces)
{
	int &width = this->width;
	int &height = this->height;
	if (width == 0 || height == 0)
	{	// 默认大小
		width = height = TEX_DIM;
	}
	// 清空更新区域
	isDirty = false;

	// 初始化消除裂缝的对象
	seamFiller.init(width, height, vertices, texcoords, faces);

	if (!layers.size())
	{	// 如果当前为空,分配一个背景层
		QSharedPointer<CanvasLayer>pLayer(new CanvasLayer(width, height));
		pLayer->setName("background");
		layers.push_back(pLayer);
	}
	if (!curLayerBaseThickness)
	{
		curLayerBaseThickness = new float[width * height];
	}
	if (!totalColor)
	{
		totalColor = new unsigned[width * height];
	}
	if (!totalSurf)
	{
		totalSurf = new unsigned[width * height];
	}
	if (!totalThick)
	{
		totalThick = new float[width * height];
	}
	if (!updatePixelSet)
	{
		updatePixelSet = new PixelPos[width * height];
	}
	if (!updatePixelMask)
	{
		updatePixelMask = new int[width*height];
	}
	updatePixelCount = 0;

	for (int i = 0; i < width * height; ++i)
	{
		totalColor[i] = INIT_BACKBROUND_COLOR_BITS;
		totalSurf[i] = INIT_SURF_BITS;
		totalThick[i] = INIT_THICKNESS;
		updatePixelMask[i] = -1;
		curLayerBaseThickness[i] = 0;
	}
	setUpdateRegionEmpty();

	// 分配opengl纹理
	glGenTextures(1, &glColorTexObj);
	glBindTexture(GL_TEXTURE_2D, glColorTexObj);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, totalColor);

	glGenTextures(1, &glSurfTexObj);
	glBindTexture(GL_TEXTURE_2D, glSurfTexObj);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, totalSurf);

	glGenTextures(1, &glThicknessTexObj);
	glBindTexture(GL_TEXTURE_2D, glThicknessTexObj);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, width, height, 0, GL_ALPHA, GL_FLOAT, totalThick);



	glGenTextures(1, &glBaseThicknessTexObj);
	glBindTexture(GL_TEXTURE_2D, glBaseThicknessTexObj);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, width, height, 0, GL_ALPHA, GL_FLOAT, curLayerBaseThickness);

	int selectLayerID = curSelectedLayer == -1 ? 0 : curSelectedLayer;
	selectLayer(selectLayerID);
	updateAll();
}

void Canvas::release()
{
	if (curLayerBaseThickness)
	{
		delete[] curLayerBaseThickness;
		curLayerBaseThickness = NULL;
	}
	if (totalColor)
	{
		delete[] totalColor;
		totalColor = NULL;
	}
	if (totalSurf)
	{
		delete[] totalSurf;
		totalSurf = NULL;
	}
	if (totalThick)
	{
		delete[] totalThick;
		totalThick = NULL;
	}
	if (updatePixelSet)
	{
		delete[] updatePixelSet;
		updatePixelSet = NULL;
	}
	if (updatePixelMask)
	{
		delete[] updatePixelMask;
		updatePixelMask = NULL;
	}

	glDeleteTextures(1, &glColorTexObj);
	glDeleteTextures(1, &glSurfTexObj);
	glDeleteTextures(1, &glThicknessTexObj);
	glDeleteTextures(1, &glBaseThicknessTexObj);
}



void Canvas::setUpdateRegionEmpty()
{
	// 把矩形更新区域设置为坐标最大值比最小值小，
	// 这样之后像素更新时就会把更新区域调整到新的像素点位置
	updateRegion[0][0] = UPDATE_REGION_INVALID_COORD;
	updateRegion[0][1] = UPDATE_REGION_INVALID_COORD;
	updateRegion[1][0] = -UPDATE_REGION_INVALID_COORD;
	updateRegion[1][1] = -UPDATE_REGION_INVALID_COORD;
	for (int ithPixel = 0; ithPixel < updatePixelCount; ++ithPixel)
	{
		PixelPos& pos = updatePixelSet[ithPixel];
		updatePixelMask[pos.y * width + pos.x] = -1;
	}
	updatePixelCount = 0;
}

bool Canvas::selectLayer( int ithLayer )
{
	if (ithLayer >= 0 && ithLayer < layers.size())
	{
		curSelectedLayer = ithLayer;
		curLayerColorPointer = layers[ithLayer]->getColorPointer();
		curLayerSurfacePointer = layers[ithLayer]->getSurfPointer();
		curLayerThicknessPointer = layers[ithLayer]->getThicknessPointer();
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel)
		{
			curLayerBaseThickness[ithPixel] = 0;
		}
		for (int ithBaseLayer = 0; ithBaseLayer <= ithLayer; ++ithBaseLayer)
		{
			layers[ithBaseLayer]->blendRegion(0,0,width, height, curLayerBaseThickness,0);
		}

		glBindTexture(GL_TEXTURE_2D, glBaseThicknessTexObj);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, width, height, 0, GL_ALPHA, GL_FLOAT, curLayerBaseThickness);

		return true;
	}
	return false;
}

void Canvas::updateGLTextures()
{
	if (!isDirty)
		return;

	int& xmin = updateRegion[0][0];
	int& ymin = updateRegion[0][1];
	int& xmax = updateRegion[1][0];
	int& ymax = updateRegion[1][1];
	int  regionWidth = xmax - xmin + 1;
	int  regionHeight = ymax - ymin + 1;


	// 更新总图像数据,先复制底层数据
	clearRegion(xmin, ymin, regionWidth, regionHeight, updatePixelSet, updatePixelCount,
		totalColor, totalSurf, totalThick, 0);

	// 逐层往上混合	
	for (QVector<QSharedPointer<CanvasLayer>>::iterator pLayer = layers.begin();
		pLayer != layers.end(); ++pLayer)
		(*pLayer)->blendRegion(xmin, ymin, regionWidth, regionHeight, updatePixelSet, updatePixelCount,
		totalColor, totalSurf, totalThick, 0);

	// 把更新像素放入一个区域中，用于传给openGL
	// 分配暂存混合数据的缓冲区
	float* thickBuf = new float[regionWidth * regionHeight];
	unsigned* colorBuf = new unsigned[regionWidth * regionHeight * 2];
	unsigned* surfBuf = colorBuf + regionWidth * regionHeight;
	for (int y = ymin, ithPixel = 0; y <= ymax;++y)
	{
		for (int x = xmin; x <= xmax; ++x,++ithPixel)
		{
			int globalOffset = y * width + x;
			colorBuf[ithPixel] = totalColor[globalOffset];
			surfBuf[ithPixel] = totalSurf[globalOffset];
			thickBuf[ithPixel] = totalThick[globalOffset];
		}
	}

	// 发送图层数组到openGL
	glBindTexture(GL_TEXTURE_2D, glColorTexObj);
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		xmin, 
		ymin,
		regionWidth, 
		regionHeight, 
		GL_BGRA, GL_UNSIGNED_BYTE, colorBuf);

	glBindTexture(GL_TEXTURE_2D, glSurfTexObj);
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		xmin, 
		ymin,
		regionWidth, 
		regionHeight, 
		GL_RGBA, GL_UNSIGNED_BYTE, colorBuf + regionWidth * regionHeight);

	glBindTexture(GL_TEXTURE_2D, glThicknessTexObj);
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		xmin, 
		ymin,
		regionWidth, 
		regionHeight, 
		GL_ALPHA, GL_FLOAT, thickBuf);

	// 清除数据
	delete[] colorBuf;
	delete[] thickBuf;


	isDirty = false;
	setUpdateRegionEmpty();
}

QSharedPointer<CanvasLayer> Canvas::generateLayer()
{
	QSharedPointer<CanvasLayer> pLayer(new CanvasLayer(width, height));
	pLayer->setName(QString("layer ") + QString::number(layers.size()));
	return pLayer;
}


void Canvas::insertLayer(QSharedPointer<CanvasLayer> pLayer, int position)
{
	layers.insert(position, pLayer);
	selectLayer(position);
	updateAll();
}

bool Canvas::removeLayer(QSharedPointer<CanvasLayer>& pLayer, int position)
{
	if (layers.size() == 1 || position < 0 || position >= layers.size())
		return false;
	pLayer = layers[position];
	layers.erase(layers.begin() + position);
	selectLayer(max(0, position-1));
	updateAll();
	return true;
}

void Canvas::copyCurLayerColorArray( unsigned* colorArray )
{
	memcpy(colorArray, curLayerColorPointer, sizeof(unsigned) * width * height);
}

void Canvas::copyCurLayerSurfaceArray( unsigned* surfArray )
{
	memcpy(surfArray, curLayerSurfacePointer, sizeof(unsigned) * width * height);
}

void Canvas::copyCurLayerThicknessArray( float* thicknessArray )
{
	memcpy(thicknessArray, curLayerThicknessPointer, sizeof(float) * width * height);

}

void Canvas::getLayerNames( QStringList& names )
{
	int i = layers.size() - 1;
	while (i >= 0)
	{
		names.push_back(layers[i]->getName());
		i--;
	}
}

void Canvas::updateAll()
{
	// 更新总图像数据,先复制底层数据
	clearArray(totalColor, totalSurf, totalThick,width*height);

	// 逐层往上混合
	for (
		QVector<QSharedPointer<CanvasLayer>>::iterator pLayer = layers.begin();
		pLayer != layers.end(); ++pLayer)
		(*pLayer)->blendRegion(0, 0, width, height,
		totalColor, totalSurf, totalThick, 0);

	for (int ithPixel = 0; ithPixel < width * height; ++ithPixel)
	{
		curLayerBaseThickness[ithPixel] = 0;
	}
	for (int ithBaseLayer = 0; ithBaseLayer <= curSelectedLayer; ++ithBaseLayer)
	{
		layers[ithBaseLayer]->blendRegion(0,0,width, height, curLayerBaseThickness,0);
	}

	// 发送图层数组到openGL
	glBindTexture(GL_TEXTURE_2D, glColorTexObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, totalColor);

	glBindTexture(GL_TEXTURE_2D, glSurfTexObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, totalSurf);

	glBindTexture(GL_TEXTURE_2D, glThicknessTexObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, width, height, 0, GL_ALPHA, GL_FLOAT, totalThick);

	glBindTexture(GL_TEXTURE_2D, glBaseThicknessTexObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, width, height, 0, GL_ALPHA, GL_FLOAT, curLayerBaseThickness);
}

void Canvas::setUpdateRegionFull()
{
	// 更新整幅opengl纹理
	updateRegion[0][0] = 0;
	updateRegion[0][1] = 0;
	updateRegion[1][0] = width-1;
	updateRegion[1][1] = height-1;

	isDirty = true;
	updatePixelCount = 0;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			updatePixelSet[updatePixelCount].x = x;
			updatePixelSet[updatePixelCount].y = y;
			updatePixelMask[y * width + x] = updatePixelCount;
			++updatePixelCount;
		}
	}
}

bool Canvas::moveUpLayer( int ithLayer )
{
	if (ithLayer >= 0 && ithLayer < layers.size() - 1)
	{
		QSharedPointer<CanvasLayer>layer = layers[ithLayer];
		layers[ithLayer] = layers[ithLayer+1];
		layers[ithLayer+1]=layer;
		selectLayer(ithLayer+1);
		updateAll();
		return true;
	}
	return false;
}

bool Canvas::moveDownLayer( int ithLayer )
{
	if (ithLayer > 0 && ithLayer < layers.size())
	{
		QSharedPointer<CanvasLayer>layer = layers[ithLayer];
		layers[ithLayer] = layers[ithLayer-1];
		layers[ithLayer-1]=layer;
		selectLayer(ithLayer-1);
		updateAll();
		return true;
	}
	return false;
}

void Canvas::setCurLayerFromImage( const QImage& image, CanvasLayer::LayerChannel channel )
{
	layers[curSelectedLayer]->setLayerFromImage(image, channel);
}

bool Canvas::replaceLayer( QSharedPointer<CanvasLayer>& newLayer, QSharedPointer<CanvasLayer>& oldLayer,int ithLayer )
{
	if (ithLayer < 0 || ithLayer >= layers.size())
		return false;
	oldLayer = layers[ithLayer];
	layers[ithLayer] = newLayer;
	updateAll();
	return true;
}

bool Canvas::setLayer( QSharedPointer<CanvasLayer>& newLayer,int ithLayer )
{
	if (ithLayer < 0 || ithLayer >= layers.size())
		return false;
	layers[ithLayer] = newLayer;
	updateAll();
	return true;
}

void Canvas::clearArray( unsigned* color, unsigned* surf, float* thick, int nPixels )
{
	for (int i = 0; i < nPixels; ++i, ++color, ++surf, ++thick)
	{
		*color = INIT_BACKBROUND_COLOR_BITS;
		*surf  = INIT_SURF_BITS;
		*thick = INIT_THICKNESS;
	}
}
void Canvas::clearRegion(int xmin, int ymin, 
							int regionWidth, int regionHeight,
							PixelPos* updatePixels,
							unsigned updateCount,
							unsigned* bufColor,
							unsigned* bufSurf,
							float * bufThick,
							int isContinuous)
{
	for(int ithPixel = 0; ithPixel < updateCount; ++ithPixel, ++updatePixels)
	{
		PixelPos& pixelPos = *updatePixels;
		int globalOffset = pixelPos.y * width + pixelPos.x;
		int regionOffset = (pixelPos.y - ymin) * regionWidth + pixelPos.x - xmin;
		int bufOffset    = (1 - isContinuous) * globalOffset + isContinuous * regionOffset;
		bufThick[bufOffset] = INIT_THICKNESS;
		bufColor[bufOffset] = INIT_BACKBROUND_COLOR_BITS;
		bufSurf[bufOffset] = INIT_SURF_BITS;
	}
}

void Canvas::mergeUpdateRegion( int x, int y )
{
	updateRegion[0][0] = min(updateRegion[0][0], x);
	updateRegion[0][1] = min(updateRegion[0][1], y);
	updateRegion[1][0] = max(updateRegion[1][0], x);
	updateRegion[1][1] = max(updateRegion[1][1], y);
	int& mask = updatePixelMask[y*width+x];
	if (mask == -1)
	{
		mask = updatePixelCount++;
	}
	updatePixelSet[mask].x = x;
	updatePixelSet[mask].y = y;
}

void Canvas::saveCanvas( const QString& fileName, const CanvasLayer::LayerChannel channel )
{
	QImage saveImg(this->width, this->height, QImage::Format_ARGB32);
	QRgb* dstPixel = (QRgb*)saveImg.bits();
	if (channel == CanvasLayer::CHNL_THICKNESS)
	{
		float maxThick = FLT_MIN;
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel)
		{
			maxThick = max(maxThick, totalThick[ithPixel]);
		}
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel, ++dstPixel)
		{
			char grayness = totalThick[ithPixel] / maxThick * 255.0f;
			*dstPixel = qRgba(grayness, grayness, grayness, 255);
		}
	}
	else if (channel != CanvasLayer::CHNL_COLOR)
	{	// surface image 的通道
		unsigned int* srcPixel = totalSurf;
		for (int ithPixel = 0; ithPixel < width * height; ++ithPixel, ++srcPixel, ++dstPixel)
		{			
			unsigned char* pChannel = (unsigned char*)(srcPixel);
			*dstPixel = qRgba(pChannel[channel], pChannel[channel], pChannel[channel],255);
		}
	}
	else
	{
		memcpy(dstPixel, totalColor, sizeof(unsigned) * this->width * this->height);
	}
	saveImg.save(fileName);
}


QDataStream& operator<<(QDataStream& out, const QSharedPointer<CanvasLayer>& layer)
{
	out << quint32(layer->width) << quint32(layer->height)
		<< layer->name << layer->isVisible 		
		<< layer->colorImage << layer->surfaceImage;
	out.writeRawData((char*)layer->thickness, sizeof(float) * layer->width * layer->height);
	return out;
}
QDataStream& operator>>(QDataStream& in, QSharedPointer<CanvasLayer>& layer)
{
	layer = QSharedPointer<CanvasLayer>(new CanvasLayer());
	in  >> layer->width >> layer->height
		>> layer->name >> layer->isVisible 
		>> layer->colorImage >> layer->surfaceImage;
	layer->thickness = new float[layer->width * layer->height];
	in.readRawData((char*)layer->thickness, sizeof(float) * layer->width * layer->height);
	return in;
}

QDataStream& operator<<(QDataStream& out, const Canvas& canvas)
{
	out << canvas.width << canvas.height
		<< canvas.layers << canvas.curSelectedLayer;
	return out;
}

QDataStream& operator>>(QDataStream& in, Canvas& canvas)
{
	int selectedLayer;
	in  >> canvas.width >> canvas.height
		>> canvas.layers >> canvas.curSelectedLayer;
	return in;
}