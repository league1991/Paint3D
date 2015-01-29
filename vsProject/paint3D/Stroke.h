#pragma once

class Stroke
{
public:
	enum StrokeType{STROKE_ANALYTIC = 0, STROKE_BITMAP = 1};
	Stroke(void);
	~Stroke(void);

	// 根据笔刷局部坐标返回透明度,注意uv值在 0-1 之间
	virtual float getOpacity(const QVector2D& uv) = 0;
	QString getName(){return name;}
	QIcon& getIcon(){return icon;}
	Stroke::StrokeType getType(){return type;}
protected:
	virtual void buildIcon() = 0;

	QString name;
	StrokeType  type;
	QIcon   icon;
};

class StandardStroke:public Stroke
{
public:
	StandardStroke();
	~StandardStroke(){};
	void setHardness(float hardness);
	float getOpacity(const QVector2D& uv);

private:
	void buildIcon();

	float hardness;
	float a;			// 暂存
};
class EmptyStroke:public Stroke
{
public:
	EmptyStroke();
	float getOpacity(const QVector2D& uv){return 1.0f;};
private:
	void buildIcon();
};
class BitmapStroke: public Stroke
{
public:
	//BitmapStroke(){bits = NULL; width = height = 0;type = STROKE_BITMAP;}
	BitmapStroke(const unsigned* bitsARGB, int width, int height, const QString& name);
	BitmapStroke(const unsigned char* bitsA, int width, int height, const QString& name);
	~BitmapStroke();
	float getOpacity(const QVector2D& uv);

private:
	void buildIcon();

	float* bits;
	int width, height;
};

class StrokeLib
{
public:
	StrokeLib();
	~StrokeLib(){};
	void init();
	bool loadFromAbr(const QString& fileName);
	bool loadFromImage(const QString& fileName);
	QSharedPointer<Stroke> getStroke(const QString& name);
	QSharedPointer<Stroke> getStroke(const int ithStroke);
	QSharedPointer<Stroke> getDefaultStroke(){return strokes[0];}
	int getNumStrokes(){return strokes.size();}
private:
	void loadFromAbrVer12(QDataStream& in, quint16 version);
	void loadFromAbrVer6( QDataStream& in, quint16 version , QFile& file);
	void unpackData(const char* imgCompressa, int length, QVector<unsigned char>& rawData);
	QVector<QSharedPointer<Stroke>> strokes;
};