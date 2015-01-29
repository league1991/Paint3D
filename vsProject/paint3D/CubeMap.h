#pragma once

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
