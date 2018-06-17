#pragma once

class CubeMapLayer
{
public:
	CubeMapLayer(QImage cubeImg[6]);

	CubeMapLayer(const QSize& size);

	CubeMapLayer& operator=(const CubeMapLayer& other)
	{
		for (int i = 0; i < 6; i++)
		{
			m_cubeImgs[i] = other.m_cubeImgs[i];
			m_cubePixel[i] = (QRgb*)m_cubeImgs[i].bits();
		}
		return *this;
	}

	QVector3D sample(QVector3D v) const;
	void setPixel(int ithFace, int x, int y, const QVector3D& color);

	QSize size() const
	{
		return m_cubeImgs[0].size();
	}

	void save(QString fileName)
	{
		int idx = fileName.lastIndexOf(".");

		for (int i = 0; i < 6; i++)
		{
			QString faceName = fileName;
			faceName.insert(idx, QString("_%1").arg(i));
			m_cubeImgs[i].save(faceName);
		}
	}

	static const QVector3D s_beginPos[6];
	static const QVector3D s_offsetU[6];
	static const QVector3D s_offsetV[6];
private:
	QImage m_cubeImgs[6];
	QRgb* m_cubePixel[6];
};

class CubeMap
{
public:
	CubeMap(void);
	~CubeMap(void);
	void load(const QString& fileName,
		int cubeMapSize);
	void initGLBuffer();
	GLuint getGLTexObj(){return glCubeMapID;}
	void releaseGLBuffer();
	void saveCubeMap(const QString& fileName);
private:
	// 直角坐标转球坐标
	inline void cartesian2Spherical(const QVector3D& car, float& alpha, float& beta);

	int cubeMapSize;
	// 从文件读入的图片
	QImage srcImg;
	// 把图片转化成立方体贴图的6个面
	QImage cubeImgs[6];

	static const GLuint glCubeMapTable[];
	GLuint glCubeMapID;

};

class PBRCubeMapGenerator
{
public:
	PBRCubeMapGenerator(const CubeMapLayer& layer0);

	void generateMipmaps();
	void saveImages(const QString& path)
	{
		for (int i = 0; i < m_layers.size(); i++)
		{
			QString layerPath = path + QString("/cubeMap_layer%1.png").arg(i);
			m_layers[i].save(layerPath);
		}
	}
private:
	static float D(const QVector3D& h, const QVector3D& n, float alpha);
	static float G1(float roughness, const QVector3D& n, const QVector3D& v);
	static float G(float roughness, const QVector3D& n, const QVector3D& v, const QVector3D& l);
	static float F(float f0, const QVector3D& h, const QVector3D& v);
	static float cookTorranceBRDF(const QVector3D& n, const QVector3D& l, const QVector3D& v, float roughness, float f0);

	static QVector3D importanceSampleGGX(const QVector2D& Xi, float roughness, const QVector3D& N);
	static QVector3D prefilterEnvMap(float roughness, const QVector3D& R, CubeMapLayer& cubeMap);
	std::vector<CubeMapLayer> m_layers;
};
