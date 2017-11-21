#include "StdAfx.h"
#include "CubeMap.h"
#include <QtWidgets/QMessageBox>
const GLuint CubeMap::glCubeMapTable[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

CubeMap::CubeMap(void)
{
}

CubeMap::~CubeMap(void)
{
}

void CubeMap::load( const QString& fileName, int cubeMapSize )
{
	srcImg = QImage(fileName);
/*	if (srcImg.format() != QImage::Format_ARGB32)
	{
		srcImg = srcImg.convertToFormat(QImage::Format_ARGB32);
	}*/
	if (srcImg.size().isEmpty())
	{	// 如果为空，建一个128*128的空白图片
		QMessageBox::information(NULL, QObject::tr("Info"), 
			QObject::tr("Fail to read env image. Use a blank image instead."));
		srcImg = QImage(128,128,QImage::Format_ARGB32);
		srcImg.fill(0xffffffff);
	}
	this->cubeMapSize = cubeMapSize;

	QVector3D beginPos[6] = {QVector3D(1,1,1), QVector3D(-1,1,-1),
	                         QVector3D(-1,1,-1),QVector3D(-1,-1,1),
	                         QVector3D(-1,1,1),QVector3D(1,1,-1)};

	QVector3D offsetU[6] = {QVector3D(0,0,-2),QVector3D(0,0,2),
							QVector3D(2,0,0),QVector3D(2,0,0),
							QVector3D(2,0,0),QVector3D(-2,0,0)};

	QVector3D offsetV[6] = {QVector3D(0,-2,0),QVector3D(0,-2,0),
							QVector3D(0,0,2),QVector3D(0,0,-2),
							QVector3D(0,-2,0),QVector3D(0,-2,0)};

	// 把二维贴图转化成立方体贴图
	QSize srcSize = srcImg.size();
	//QMessageBox::information(NULL, QObject::tr("Info"), QString::number(srcSize.width()) + "-"+QString::number(srcSize.height()));
	QRgb* pSrcImg = (QRgb*)srcImg.scanLine(0);
	for (int ithMap = 0; ithMap < 6; ++ithMap)
	{
		cubeImgs[ithMap] = QImage(QSize(cubeMapSize, cubeMapSize), QImage::Format_ARGB32);
		offsetU[ithMap] /= cubeMapSize;
		offsetV[ithMap] /= cubeMapSize;

		// 转化立方体贴图的一个面
		for (int y = 0; y < cubeMapSize; ++y)
		{
			QRgb* pCubeImg= (QRgb*)cubeImgs[ithMap].scanLine(y);
			QVector3D cubePos = beginPos[ithMap] + offsetV[ithMap] * y;

			for (int x = 0; x < cubeMapSize; ++x, cubePos += offsetU[ithMap])
			{
				QVector3D norCubePos = cubePos;
				norCubePos.normalize();
				float alpha, beta;
				cartesian2Spherical(norCubePos, alpha, beta);

				int srcPosU = min(max(0.0, alpha / (M_PI * 2) * srcSize.width() ), srcSize.width ()-1.0);
				int srcPosV = min(max(0.0, beta  / (M_PI) * srcSize.height()), srcSize.height()-1.0);

				pCubeImg[x] = srcImg.pixel(srcPosU, srcPosV);//[srcPosV * srcSize.width() + srcPosU];
			}
		}
	}

	//QMessageBox::information(NULL, QObject::tr("Error"), QObject::tr("convert to cube completed."));
}

void CubeMap::cartesian2Spherical( const QVector3D& car, float& alpha, float& beta )
{
	beta = acos(car.z());
	alpha = atan2(car.x(), car.y()) + M_PI;
}

void CubeMap::initGLBuffer()
{
	glGenTextures(1, &glCubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, glCubeMapID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);

	GLenum errorMsg = glGetError();
	if (errorMsg != GL_NO_ERROR)
	{
		QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("OpenGL error occurs when generating cube map object."));
	}

	//QMessageBox::information(NULL, QObject::tr("Info"), QObject::tr("begin init env map."));
	QGLFunctions* gl = GLContext::instance()->getQGLFunctions();
	for (int ithMap = 0; ithMap < 6; ++ithMap)
	{
		gluBuild2DMipmaps(glCubeMapTable[ithMap], 4, cubeMapSize, cubeMapSize, GL_BGRA, GL_UNSIGNED_BYTE, cubeImgs[ithMap].scanLine(0));
		// QMessageBox::information(NULL, QObject::tr("Info"), QString::number(ithMap) + "th map completed");

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum errorMsg = glGetError();
		if (errorMsg != GL_NO_ERROR)
		{
			QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("OpenGL error occurs when initializing cube map."));
			qDebug() << "OpenGL error occurs when initializing cube map" << endl;
		}

	}
}

void CubeMap::saveCubeMap(const QString& fileName )
{
	for (int i = 0; i < 6; ++i)
	{
		QString fName = fileName;
		fName.replace(".", "_" + QString::number(i) + ".");
		cubeImgs[i].save(fName);
	}
}

void CubeMap::releaseGLBuffer()
{
	glDeleteTextures(1, &glCubeMapID);
}
