#include "StdAfx.h"
#include "CubeMap.h"
#include <QtWidgets/QMessageBox>

const QVector3D CubeMapLayer::s_beginPos[6] = {   QVector3D(1,1,1), QVector3D(-1,1,-1),
													QVector3D(-1,1,-1),QVector3D(-1,-1,1),
													QVector3D(-1,1,1),QVector3D(1,1,-1)};

const QVector3D CubeMapLayer::s_offsetU[6] = {    QVector3D(0,0,-2),QVector3D(0,0,2),
													QVector3D(2,0,0),QVector3D(2,0,0),
													QVector3D(2,0,0),QVector3D(-2,0,0)};

const QVector3D CubeMapLayer::s_offsetV[6] = {    QVector3D(0,-2,0),QVector3D(0,-2,0),
													QVector3D(0,0,2),QVector3D(0,0,-2),
													QVector3D(0,-2,0),QVector3D(0,-2,0)};

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

	CubeMapLayer layer0(cubeImgs);
	PBRCubeMapGenerator generator(layer0);
	generator.generateMipmaps();
	generator.saveImages("i:/Programs/CG2008/paint3D/vsProject/paint3D/");
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

CubeMapLayer::CubeMapLayer(QImage cubeImg[6])
{
	for (int i = 0; i < 6; i++)
	{
		m_cubeImgs[i] = cubeImg[i];
		m_cubePixel[i] = (QRgb*)m_cubeImgs[i].bits();
	}
}

CubeMapLayer::CubeMapLayer(const QSize & size)
{
	for (int i = 0; i < 6; i++)
	{
		m_cubeImgs[i] = QImage(size, QImage::Format_ARGB32);
		m_cubePixel[i] = (QRgb*)m_cubeImgs[i].bits();
	}
}

QVector3D CubeMapLayer::sample(QVector3D v) const
{
	float pv[3] = { v.x(), v.y(), v.z() };
	int faceIdx = 0;
	float maxAbsV = 0;
	for (int i = 0; i < 3; i++)
	{
		if (qAbs(pv[i]) > maxAbsV)
		{
			maxAbsV = qAbs(pv[i]);
			faceIdx = i*2;
			faceIdx += pv[i] < 0?1:0;
		}
	}

	v /= maxAbsV;
	const QImage& srcImg = m_cubeImgs[faceIdx];
	QSize srcSize = srcImg.size();
	QRgb* pSrcImg = m_cubePixel[faceIdx];
	float uOffset = QVector3D::dotProduct(v, s_offsetU[faceIdx]) / 2.f;
	float vOffset = QVector3D::dotProduct(v, s_offsetV[faceIdx]) / 2.f;
	uOffset = (uOffset + 1.0) * 0.5f;
	vOffset = (vOffset + 1.0) * 0.5f;
	int uOffseti = qMax(0, qMin(int(uOffset * srcSize.width()), srcSize.width() - 1));
	int vOffseti = qMax(0, qMin(int(vOffset * srcSize.height()), srcSize.height() - 1));

	QRgb rgb = pSrcImg[vOffseti * srcSize.width() + uOffseti];
	return QVector3D(qRed(rgb) / 255.f, qGreen(rgb) / 255.f, qBlue(rgb) / 255.f);
}

void CubeMapLayer::setPixel(int faceIdx, int x, int y, const QVector3D& color)
{
	const QImage& srcImg = m_cubeImgs[faceIdx];
	QSize srcSize = srcImg.size();
	QRgb* pSrcImg = m_cubePixel[faceIdx];
	int uOffseti = qMax(0, qMin(x, srcSize.width() - 1));
	int vOffseti = qMax(0, qMin(y, srcSize.height() - 1));

	QRgb rgb = qRgb(color.x() * 255, color.y() * 255, color.z() * 255);
	pSrcImg[vOffseti * srcSize.width() + uOffseti] = rgb;
}

PBRCubeMapGenerator::PBRCubeMapGenerator(const CubeMapLayer & layer0)
{
	QSize s0 = layer0.size();
	QSize s = s0;
	m_layers.clear();
	while (s.width() > 0 || s.height() > 0)
	{
		QSize layerSize(qMax(s.width(), 1), qMax(s.height(), 1));
		m_layers.emplace_back(layerSize);
		s.setWidth(s.width() / 2);
		s.setHeight(s.height() / 2);
	}
	m_layers[0] = layer0;
}

void PBRCubeMapGenerator::generateMipmaps()
{
	float dRough = 1.0 / (m_layers.size() - 1);
	float roughness = dRough;

	for (int i = 1; i < m_layers.size(); i++, roughness += dRough)
	{
		// 把二维贴图转化成立方体贴图
		QSize srcSize = m_layers[i - 1].size();
		QSize dstSize = m_layers[i].size();
		for (int ithMap = 0; ithMap < 6; ++ithMap)
		{
			const QVector3D offsetU = CubeMapLayer::s_offsetU[ithMap] / dstSize.width();
			const QVector3D offsetV = CubeMapLayer::s_offsetV[ithMap] / dstSize.height();

			// 转化立方体贴图的一个面
			for (int y = 0; y < dstSize.height(); ++y)
			{
				QVector3D cubePos = CubeMapLayer::s_beginPos[ithMap] + offsetV *float(y);

				for (int x = 0; x < dstSize.width(); ++x, cubePos += offsetU)
				{
					QVector3D norCubePos = cubePos;
					norCubePos.normalize();

					auto filteredColor = prefilterEnvMap(roughness, norCubePos, m_layers[i - 1]);
					m_layers[i].setPixel(ithMap, x, y, filteredColor);
				}
			}
		}
	}
}

inline float PBRCubeMapGenerator::D(const QVector3D & h, const QVector3D & n, float alpha)
{
	float cosine = QVector3D::dotProduct(n, h);
	float alpha2 = alpha * alpha;
	float denominator = (cosine*cosine*(alpha2 - 1) + 1);
	denominator *= denominator;
	return alpha2 / (M_PI * denominator);
}

inline float PBRCubeMapGenerator::G1(float roughness, const QVector3D & n, const QVector3D & v)
{
	float k = (roughness + 1)*(roughness + 1) / 8.f;
	float NV = QVector3D::dotProduct(n, v);
	return NV / (NV*(1 - k) + k);
}

inline float PBRCubeMapGenerator::G(float roughness, const QVector3D & n, const QVector3D & v, const QVector3D & l)
{
	return G1(roughness, n, v) * G1(roughness, n, l);
}

inline float PBRCubeMapGenerator::F(float f0, const QVector3D & h, const QVector3D & v)
{
	float VH = QVector3D::dotProduct(h, v);
	return f0 + (1 - f0) * pow(2.f, (-5.55473f*VH - 6.98316f)*VH);
}

inline float PBRCubeMapGenerator::cookTorranceBRDF(const QVector3D & n, const QVector3D & l, const QVector3D & v, float roughness, float f0)
{
	QVector3D h = l + v;
	h.normalize();
	float NL = QVector3D::dotProduct(n, l);
	float NV = QVector3D::dotProduct(n, v);
	float alpha = roughness * roughness;
	return D(h, n, alpha) * G(roughness, n, v, l) * F(f0, h, v) / (4 * NL * NV);
}

inline QVector3D PBRCubeMapGenerator::importanceSampleGGX(const QVector2D & Xi, float roughness, const QVector3D & N)
{
	float a = roughness * roughness;

	float phi = 2 * M_PI * Xi.x();
	float cosTheta = sqrt((1 - Xi.y()) / (1 + (a*a - 1)*Xi.y()));
	float sinTheta = sqrt(1 - cosTheta * cosTheta);

	QVector3D H;
	H.setX(sinTheta * cos(phi));
	H.setY(sinTheta * sin(phi));
	H.setZ(cosTheta);

	QVector3D upVector = abs(N.z()) < 0.999f?QVector3D(0, 0, 1):QVector3D(1, 0, 0);
	QVector3D tangentX = QVector3D::crossProduct(upVector, N);
	tangentX.normalize();
	QVector3D tangentY = QVector3D::crossProduct(N, tangentX);

	return tangentX * H.x() + tangentY * H.y() + N * H.z();
}

QVector3D PBRCubeMapGenerator::prefilterEnvMap(float roughness, const QVector3D & R, CubeMapLayer & cubeMap)
{
	auto N = R;
	auto V = R;
	QVector3D prefilteredColor(0, 0, 0);
	const int numSamples = 1024;
	float totalWeight = 0.f;
	for (int i = 0; i < numSamples; i++)
	{
		float randU = rand() / float(RAND_MAX);
		float randV = rand() / float(RAND_MAX);
		QVector2D uv(randU, randV);
		QVector3D H = importanceSampleGGX(uv, roughness, N);
		QVector3D L = 2.f * QVector3D::dotProduct(V, H) * H - V;
		float NL = qMin(1.f, QVector3D::dotProduct(N, L));
		if (NL > 0.f)
		{
			prefilteredColor += cubeMap.sample(L) * NL;
			totalWeight += NL;
		}
	}

	return prefilteredColor / totalWeight;
}
