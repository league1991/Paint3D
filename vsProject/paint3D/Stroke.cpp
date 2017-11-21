#include "StdAfx.h"
#include "Stroke.h"

Stroke::Stroke(void)
{
}

Stroke::~Stroke(void)
{
}

float StandardStroke::getOpacity( const QVector2D& uv )
{
	float distSq = min((uv - QVector2D(0.5,0.5)).lengthSquared() / 0.25, 1.0);
	return (distSq-1) / (a * distSq -1);
}

void StandardStroke::buildIcon()
{
	QImage img(256, 256, QImage::Format_ARGB32);
	unsigned *data = (unsigned*)img.bits();
	for (int y = 0, ithPixel = 0; y < 256; ++y)
		for (int x = 0; x < 256; ++x, ++ithPixel)
		{
			QVector2D uv = QVector2D(x / 255.0f, y / 255.0f);
			float value = getOpacity(uv);
			unsigned char value255 = (1 - value) * 255;
			data[ithPixel] = 0xff000000 | (value255 << 16) | (value255 << 8) | (value255);
		}

	QPixmap pixImg = QPixmap::fromImage(img);
	icon = QIcon(pixImg);
}

StandardStroke::StandardStroke()
{
	name = "Default Stroke";
	type = STROKE_ANALYTIC;
	setHardness(0.0);
	buildIcon();
}

void StandardStroke::setHardness( float hardness )
{
	this->hardness = hardness;
	a = min(5 * hardness - 4,0.99999f);
}

EmptyStroke::EmptyStroke()
{
	name = "Empty Stroke";
	type = STROKE_ANALYTIC;
	buildIcon();
}

void EmptyStroke::buildIcon()
{
	QImage img(256, 256, QImage::Format_ARGB32);
	img.fill(0xff000000);
	QPixmap pixImg = QPixmap::fromImage(img);
	icon = QIcon(pixImg);
}

BitmapStroke::BitmapStroke(const unsigned* bitsARGB, int width, int height, const QString& name)
{
	this->name = name;
	this->width = width;
	this->height = height;
	this->bits = new float[width * height];
	this->type = STROKE_BITMAP;
	for (int i = 0; i < width * height; ++i)
	{
		bits[i] = (qGray(QRgb(bitsARGB[i]))) / 255.0f;
	}
	buildIcon();
}

BitmapStroke::BitmapStroke( const unsigned char* bitsA, int width, int height, const QString& name )
{
	this->name = name;
	this->width = width;
	this->height = height;
	this->bits = new float[width * height];
	this->type = STROKE_BITMAP;
	for (int i = 0; i < width * height; ++i)
	{
		bits[i] = bitsA[i] / 255.0f;
	}
	buildIcon();
}

BitmapStroke::~BitmapStroke()
{
	if (bits)
		delete[] bits;
}

float BitmapStroke::getOpacity( const QVector2D& uv )
{
	int x = uv.x() * (width - 1);
	int y = uv.y() * (height - 1);
	return bits[x + y * width];
}

void BitmapStroke::buildIcon()
{
	QImage img(width, height, QImage::Format_ARGB32);
	unsigned *data = (unsigned*)img.bits();
	for (int y = 0, ithPixel = 0; y < height; ++y)
		for (int x = 0; x < width; ++x, ++ithPixel)
		{
			unsigned char value255 = (1 - bits[ithPixel]) * 255;
			data[ithPixel] = 0xff000000 | (value255 << 16) | (value255 << 8) | (value255);
		}

	icon = QIcon(QPixmap::fromImage(img));
}

StrokeLib::StrokeLib()
{
}

bool StrokeLib::loadFromImage( const QString& fileName )
{
	QImage img(fileName);
	QSharedPointer<Stroke> pstroke = QSharedPointer<Stroke>
		(new BitmapStroke((const unsigned*)img.bits(), img.width(), img.height(), fileName));
	strokes.push_back(pstroke);
	return true;
}

QSharedPointer<Stroke> StrokeLib::getStroke( const QString& name )
{
	for (int i = 0; i < strokes.size(); ++i)
	{
		if (strokes[i]->getName() == name)
		{
			return strokes[i];
		}
	}
	return QSharedPointer<Stroke>();
}

QSharedPointer<Stroke> StrokeLib::getStroke( const int ithStroke )
{
	if (ithStroke >= 0 && ithStroke < strokes.size())
	{
		return strokes[ithStroke];
	}
	return QSharedPointer<Stroke>();
}

bool StrokeLib::loadFromAbr( const QString& fileName )
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QDataStream in(&file);
	quint16 version;
	in >> version;
	switch (version)
	{
	case 1:
	case 2:
		loadFromAbrVer12(in, version);	break;
	case 6:
		loadFromAbrVer6(in, version, file); break;
	default:
		qDebug("no such version.\n");
		return false;
	}
	return true;
}

void StrokeLib::loadFromAbrVer12( QDataStream& in, quint16 version )
{
	quint16 brushCount;
	in >> brushCount;
	for (quint16 ithBrush = 0; ithBrush < brushCount; ++ithBrush)
	{
		quint16 brushType;
		quint32 brushBytes;
		QString brushName = "brush " + QString::number(ithBrush);

		in >> brushType >> brushBytes;
		//tipo: 1 computed; 2 sampled
		if (brushType == 1)
		{
			if (version == 1)
			{
				in.skipRawData(14);
			}
			else
			{
				in.skipRawData(brushBytes);
			}
		}
		else if (brushType == 2)
		{
			quint32 miscellaneous;
			quint16 spacing;
			in >> miscellaneous >> spacing;
			if (version == 1)
			{
				quint8 antialias;
				in >> antialias;
			}
			else if (version == 2)
			{
				// override brush name
				quint32 nameLen;
				in >> nameLen;
				char* name = new char[nameLen * 2 + 1];
				in.readRawData(name, nameLen * 2 + 1);
				for (int i = 0; i < nameLen * 2; i+= 2)
				{
					char tmp = name[i];
					name[i] = name[i+1];
					name[i+1] = tmp;
				}
				//in.skipRawData(1);
				brushName = QString((QChar*)name, nameLen);
				delete[] name;
			}
			quint16 top, left, bottom, right, depth;
			quint32 ltop, lleft, lbottom, lright;
			quint8  compression;
			in >> top >> left >> bottom >> right;
			in >> ltop >> lleft >> lbottom >> lright;
			in >> depth >> compression;

			int br_width = lright - lleft;
			int br_height = lbottom - ltop;

			if (compression == 0)
			{
				unsigned char* rawData = new unsigned char[br_width * br_height];
				in.readRawData((char*)rawData, br_width * br_height);
				strokes.push_back(QSharedPointer<Stroke>(new BitmapStroke(rawData, br_width, br_height, brushName)));
				delete[] rawData;
			}
			else
			{
				// get length
				int byteForScanlines = 0;
				for (int i = 0; i < br_height; i++)
				{
					quint16 scanByte;
					in >> scanByte;
					byteForScanlines += scanByte;
				}

				// read compressed data and decompress it
				char* imgCompressa = new char[byteForScanlines];
				in.readRawData(imgCompressa, byteForScanlines);
				QVector<unsigned char>rawData;
				unpackData(imgCompressa, byteForScanlines, rawData);

				strokes.push_back(
					QSharedPointer<Stroke>(
					new BitmapStroke(&rawData[0], br_width, br_height, 
					brushName)));
			}
		} 
	}
}

void StrokeLib::loadFromAbrVer6( QDataStream& in, quint16 version , QFile& file)
{
	int imgW = 0, imgH = 0;

	//difference between 6.1 and 6.2
	//differenza tra 6.1 e 6.2
	quint16 subVer;
	in >> subVer;

	in.skipRawData(8);

	//dimension in bytes of the sample section
	qint32 sampleSectionSize;
	in >> sampleSectionSize;
	int endSampleSection = sampleSectionSize+12;

	//loop until the end of the section
	for (int ithBrush = 0; file.pos() < endSampleSection - 1; ++ithBrush)
	{
		//brushSize
		QString brushName = "brush " + QString::number(ithBrush);
		quint32 brushSize;
		in >> brushSize;

		//we calculate the aim address brush that it must complementary to 4
		int brushEnd = brushSize;
		while(brushEnd%4!=0){ brushEnd++; }
		int complementToFourBytes = brushEnd - brushSize; 

		// read 7 bit encoded int, which is the length of string
		int length = 0;
		for (int shift = 0; shift < 35; shift+=7)
		{
			qint8 b;
			in >> b;
			length = length | ((b&0x7f) << shift);
			if ((b & 0x80) == 0)
				break;
		}/*
		char* name = new char[length * 2 + 1];
		in.readRawData(name, length);
		brushName = QString((QChar*)name, length);
		delete[] name;*/

		in.skipRawData(length);

		if (subVer == 1)
		{
			quint16 top, left, bottom, right, unknown;
			quint32 ltop, lleft, lbottom, lright;
			quint8  compression;
			in >> top >> left >> bottom >> right;
			in >> ltop >> lleft >> lbottom >> lright;

			imgW = lright - lleft;
			imgH = lbottom - ltop;
		}
		else if (subVer == 2)
		{
			//skip 264 unuseful bytes
			/*char data[264];
			in.readRawData(data, 264);*/
			in.skipRawData(264);
			
			quint32 padHeight, padWidth, height, width;
			in >> padHeight >> padWidth >> height >> width;
			
			imgW = width - padWidth;
			imgH = height - padHeight;
		}
		quint16 depth;
		in >> depth;

		quint8 compression;
		in >> compression;

		if (compression == 0)
		{
			unsigned char* rawData = new unsigned char[imgW * imgH];
			in.readRawData((char*)rawData, imgW * imgH);
			strokes.push_back(QSharedPointer<Stroke>(new BitmapStroke(rawData, imgW, imgH, brushName)));
			delete[] rawData;
		}
		else
		{
			// get total data length
			int byteForScanlines = 0;
			for (int i = 0; i < imgH; i++)
			{
				quint16 lineLength;
				in >> lineLength;
				byteForScanlines += lineLength;
			}

			// read compressed data and decompress it
			char* imgCompressa = new char[byteForScanlines];
			in.readRawData(imgCompressa, byteForScanlines);
			QVector<unsigned char> rawData;
			unpackData(imgCompressa, byteForScanlines, rawData);


			strokes.push_back(QSharedPointer<Stroke>(new BitmapStroke(&rawData[0], imgW, imgH, brushName)));
		}

		if (subVer == 1)
		{
			in.skipRawData(complementToFourBytes);
		}
		else if (subVer == 2)
		{
			//8 bytes finali unknown
			in.skipRawData(8);
			//skip the bytes that make complement to 4 of the size of the brush
			in.skipRawData(complementToFourBytes);
		}
	}
}

void StrokeLib::unpackData(const char* imgCompressa, 
						   int length, 
						   QVector<unsigned char>& rawData)
{
	int readPos = 0;
	rawData.clear();
	while (readPos < length - 1)
	{
		char count = imgCompressa[readPos++];
		if (count >= 0)
		{
			for (int i = 0; i < count + 1; i++)
				rawData.push_back(imgCompressa[readPos++]);
		}
		else
		{
			char y = imgCompressa[readPos++];
			for (int i = 0; i < -count + 1; i++)
				rawData.push_back((unsigned char)y);
		}
	}
}

void StrokeLib::init()
{
	QSharedPointer<Stroke> pstroke = QSharedPointer<Stroke>(new StandardStroke);
	strokes.push_back(pstroke);
	pstroke = QSharedPointer<Stroke>(new EmptyStroke);
	strokes.push_back(pstroke);
}
