#pragma once

class PaintPicker
{
public:
	PaintPicker(void);
	~PaintPicker(void);

	// 拾取材质属性
	virtual bool pickPaint(const QVector3D& ori, const QVector3D& dir, 
						   const QVector2D& uv,
							QVector4D& colorParam, QVector4D& surfParam, float& depth) = 0;
};

// 拾取纯材质属性（不包括位图）
class PurePaintPicker: public PaintPicker
{
public:
	PurePaintPicker();
	~PurePaintPicker();

	void setColorParam(const QVector4D& color){colorParam = color;}
	void setSurfParam(const QVector4D& surf){surfParam = surf;}
	void setDepth(float depth){this->depth = depth;}

	bool pickPaint(const QVector3D& ori, const QVector3D& dir, 
					const QVector2D& uv,
		           QVector4D& colorParam, QVector4D& surfParam, float& depth);

	void setResolution(int w, int h);

	void setColorTexture(const QImage&img);
	void setReflectionTexture(const QImage& img);
	void setGlossnessTexture(const QImage& img);
	void setRefractionTexture(const QImage& img);
	void setIORTexture(const QImage& img);
	void setThicknessTexture(const QImage& img);

	void clearColorTexture();
	void clearReflectionTexture();
	void clearGlossnessTexture();
	void clearRefractionTexture();
	void clearIORTexture();
	void clearThicknessTexture();
private:
	QVector4D colorParam;
	QVector4D surfParam;
	float depth;

	QImage colorImg;
	QImage reflImg;
	QImage glosImg;
	QImage refrImg;
	QImage iorImg;
	QImage thicknessImg;

	float* colorArray;
	float* reflArray;
	float* glosArray;
	float* refrArray;
	float* iorArray;
	float* thicknessArray;

	int width, height;
};

class ImagePicker: public PaintPicker, public Mesh
{
public:
	ImagePicker();
};

class PlanePicker: public ImagePicker
{
public:
	PlanePicker();
	bool pickPaint( const QVector3D& ori, const QVector3D& dir, 
		const QVector2D& uv, QVector4D& colorParam, QVector4D& surfParam, float& depth );
	void drawAppearance();
	friend QDataStream& operator<<(QDataStream& out, const PlanePicker&picker);
	friend QDataStream& operator>>(QDataStream& in, PlanePicker&picker);
private:
};