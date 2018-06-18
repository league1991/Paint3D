#pragma once

#define ENV_MIP_LEVELS 5
class CubeMapLayer
{
public:
	CubeMapLayer() {}
	CubeMapLayer(QImage cubeImg[6]);
	CubeMapLayer(const QSize& size);

	CubeMapLayer& operator=(const CubeMapLayer& other)
	{
		for (int i = 0; i < 6; i++)
		{
			cubeImgs[i] = other.cubeImgs[i];
			cubePixel[i] = (QRgb*)cubeImgs[i].bits();
		}
		return *this;
	}

	QVector3D sample(QVector3D v) const;
	void setPixel(int ithFace, int x, int y, const QVector3D& color);
	QRgb* rawBuffer(int ithFace) const { return cubePixel[ithFace]; }

	QSize size() const
	{
		return cubeImgs[0].size();
	}

	void save(QString fileName);
	bool load(QString fileName);

	static const QVector3D s_beginPos[6];
	static const QVector3D s_offsetU[6];
	static const QVector3D s_offsetV[6];
private:
	QImage cubeImgs[6];
	QRgb* cubePixel[6];
};

class PBRCubeMapGenerator
{
public:
	PBRCubeMapGenerator();

	void generateMipmaps(const CubeMapLayer& layer0);
	const std::vector<CubeMapLayer>& getMipMaps()const { return layers; }
	bool loadCubeMapImages(const QString& path);
	void saveCubeMapImages(const QString& path);
	void saveBRDFTableImage(const QString& path);
	void generateBRDFTable(int size);
	const std::vector<QVector3D>& getBRDFTable()const { return brdfTable; }
	QSize getBRDFTableSize()const { return brdfTableSize; }
private:
	static float D(const QVector3D& h, const QVector3D& n, float alpha);
	static float G1(float k, const QVector3D& n, const QVector3D& v);
	static float G(float roughness, const QVector3D& n, const QVector3D& v, const QVector3D& l);
	static float GIBL(float roughness, const QVector3D& n, const QVector3D& v, const QVector3D& l);
	static float F(float f0, const QVector3D& h, const QVector3D& v);
	static float cookTorranceBRDF(const QVector3D& n, const QVector3D& l, const QVector3D& v, float roughness, float f0);

	static QVector3D importanceSampleGGX(const QVector2D& Xi, float roughness, const QVector3D& N);
	static QVector2D integrateBRDF(float roughness, float NV);
	static QVector3D prefilterEnvMap(float roughness, const QVector3D& R, CubeMapLayer& cubeMap);

	std::vector<CubeMapLayer> layers;
	std::vector<QVector3D> brdfTable;
	QSize brdfTableSize;
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
	GLuint getBRDFTexObj() { return glBRDFMapID; }
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
	// Mipmaps for cubemap
	PBRCubeMapGenerator cubeGenerator;

	static const GLuint glCubeMapTable[];
	GLuint glCubeMapID;
	GLuint glBRDFMapID;
};

