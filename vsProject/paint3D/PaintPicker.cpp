#include "StdAfx.h"
#include "paintPicker.h"

PaintPicker::PaintPicker(void)
{
}

PaintPicker::~PaintPicker(void)
{
	
}

PurePaintPicker::PurePaintPicker():colorParam(0,0,0,1), surfParam(0,0,1,0),depth(0)
{
	colorArray = NULL;
	reflArray = NULL;
	glosArray = NULL;
	refrArray = NULL;
	iorArray  = NULL;
	thicknessArray = NULL;
	setResolution(50,50);
}


bool PurePaintPicker::pickPaint( const QVector3D& ori, const QVector3D& dir, const QVector2D& uv, QVector4D& colorParam, QVector4D& surfParam, float& depth )
{
	int xy[2] = {uv.x() * (width-1), uv.y() * (height-1)};
	int offset = xy[1] * width + xy[0];
	if (colorArray)
	{
		float* color = colorArray + offset * 4;
		colorParam = QVector4D(color[0], color[1], color[2], color[3]);
	}
	else
		colorParam = this->colorParam;
	if (reflArray)
	{
		surfParam.setX(reflArray[offset]);
	}
	else
		surfParam.setX(this->surfParam.x());
	if (glosArray)
	{
		surfParam.setY(glosArray[offset]);
	}
	else
		surfParam.setY(this->surfParam.y());
	if (refrArray)
	{
		surfParam.setZ(refrArray[offset]);
	}
	else
		surfParam.setZ(this->surfParam.z());
	if (iorArray)
	{
		surfParam.setW(iorArray[offset]);
	}
	else
		surfParam.setW(this->surfParam.w());
	if (thicknessArray)
	{
		depth = thicknessArray[offset];
	}
	else
		depth = this->depth;
	return true;
}

void PurePaintPicker::setColorTexture( const QImage&img )
{
	colorImg = img;
	QImage tmpImg = img.scaled(width, height);
	if (colorArray)
		delete[] colorArray;
	colorArray = new float[width * height * 4];
	for (int y = 0, ithPixel = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++ithPixel)
		{
			float* clr = colorArray + 4 * ithPixel;
			QRgb rgba = tmpImg.pixel(x,y);
			clr[0] = qRed(rgba) / 255.0f;
			clr[1] = qGreen(rgba) / 255.0f;
			clr[2] = qBlue(rgba) / 255.0f;
			clr[3] = qAlpha(rgba) / 255.0f;
		}
	}
}

void PurePaintPicker::setReflectionTexture( const QImage& img )
{
	reflImg = img;
	QImage tmpImg = img.scaled(width, height);
	if (reflArray)
		delete[] reflArray;
	reflArray = new float[width * height];
	for (int y = 0, ithPixel = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++ithPixel)
		{
			QRgb rgba = tmpImg.pixel(x,y);
			reflArray[ithPixel] = qGray(rgba) / 255.0f;
		}
	}
}

void PurePaintPicker::setGlossnessTexture( const QImage& img )
{
	glosImg = img;
	QImage tmpImg = img.scaled(width, height);
	if (glosArray)
		delete[] glosArray;
	glosArray = new float[width * height];
	for (int y = 0, ithPixel = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++ithPixel)
		{
			QRgb rgba = tmpImg.pixel(x,y);
			glosArray[ithPixel] = qGray(rgba) / 255.0f;
		}
	}
}

void PurePaintPicker::setRefractionTexture( const QImage& img )
{
	refrImg = img;
	QImage tmpImg = img.scaled(width, height);
	if (refrArray)
		delete[] refrArray;
	refrArray = new float[width * height];
	for (int y = 0, ithPixel = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++ithPixel)
		{
			QRgb rgba = tmpImg.pixel(x,y);
			refrArray[ithPixel] = qGray(rgba) / 255.0f;
		}
	}
}

void PurePaintPicker::setIORTexture( const QImage& img )
{
	iorImg = img;
	QImage tmpImg = img.scaled(width, height);
	if (iorArray)
		delete[] iorArray;
	iorArray = new float[width * height];
	for (int y = 0, ithPixel = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++ithPixel)
		{
			QRgb rgba = tmpImg.pixel(x,y);
			iorArray[ithPixel] = qGray(rgba) / 255.0f;
		}
	}
}

void PurePaintPicker::setThicknessTexture( const QImage& img )
{
	thicknessImg = img;
	QImage tmpImg = img.scaled(width, height);
	if (thicknessArray)
		delete[] thicknessArray;
	thicknessArray = new float[width * height];
	for (int y = 0, ithPixel = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++ithPixel)
		{
			QRgb rgba = tmpImg.pixel(x,y);
			thicknessArray[ithPixel] = qGray(rgba) / 255.0f;
		}
	}
}

void PurePaintPicker::clearColorTexture()
{
	if (colorArray)
	{
		delete[] colorArray;
		colorArray = NULL;
	}
}

void PurePaintPicker::clearReflectionTexture()
{
	if (reflArray)
	{
		delete[] reflArray;
		reflArray = NULL;
	}
}

void PurePaintPicker::clearGlossnessTexture()
{
	if (glosArray)
	{
		delete[] glosArray;
		glosArray = NULL;
	}
}

void PurePaintPicker::clearRefractionTexture()
{
	if (refrArray)
	{
		delete[] refrArray;
		refrArray = NULL;
	}
}

void PurePaintPicker::clearIORTexture()
{
	if (iorArray)
	{
		delete[] iorArray;
		iorArray = NULL;
	}
}

void PurePaintPicker::clearThicknessTexture()
{
	if (thicknessArray)
	{
		delete[] thicknessArray;
		thicknessArray = NULL;
	}
}

PurePaintPicker::~PurePaintPicker()
{
	clearColorTexture();
	clearReflectionTexture();
	clearGlossnessTexture();
	clearRefractionTexture();
	clearIORTexture();
	clearThicknessTexture();
}

void PurePaintPicker::setResolution( int w, int h )
{
	width = w; height = h;
	if (colorArray)
	{
		setColorTexture(colorImg);
	}
	if (reflArray)
	{
		setReflectionTexture(reflImg);
	}
	if (refrArray)
	{
		setRefractionTexture(refrImg);
	}
	if (glosArray)
	{
		setGlossnessTexture(glosImg);
	}
	if (iorArray)
	{
		setIORTexture(iorImg);
	}
	if (thicknessArray)
	{
		setThicknessTexture(thicknessImg);
	}
}

PlanePicker::PlanePicker():ImagePicker()
{
	type = OBJ_PICKER_OBJECT;
	name = QString("picker") + QString::number(objectID);
	alphaForAppearance = 0.4f;
	// 准备几何数据（是一个平面）
	vertices.push_back(QVector3D(-1,-1,0));
	vertices.push_back(QVector3D( 1,-1,0));
	vertices.push_back(QVector3D( 1, 1,0));
	vertices.push_back(QVector3D(-1, 1,0));

	normals.push_back(QVector3D(0,0,-1));

	texcoords.push_back(QVector2D(0,0));
	texcoords.push_back(QVector2D(1,0));
	texcoords.push_back(QVector2D(1,1));
	texcoords.push_back(QVector2D(0,1));

	ObjTriangle tri1;
	tri1.vertIndex[0] = 0;	    tri1.vertIndex[1] = 1;	    tri1.vertIndex[2] = 2;
	tri1.texcoordIndex[0] = 0;	tri1.texcoordIndex[1] = 1;	tri1.texcoordIndex[2] = 2;
	tri1.norIndex[0] = tri1.norIndex[1] = tri1.norIndex[2] = 0;
	tri1.mtlIndex = 0;
	faces.push_back(tri1);

	ObjTriangle tri2;
	tri2.vertIndex[0] = 0;	    tri2.vertIndex[1] = 2;	    tri2.vertIndex[2] = 3;
	tri2.texcoordIndex[0] = 0;	tri2.texcoordIndex[1] = 2;	tri2.texcoordIndex[2] = 3;
	tri2.norIndex[0] = tri2.norIndex[1] = tri2.norIndex[2] = 0;
	tri2.mtlIndex = 0;
	faces.push_back(tri2);
}

bool PlanePicker::pickPaint( const QVector3D& ori, const QVector3D& dir, const QVector2D& uv,QVector4D& colorParam, QVector4D& surfParam, float& depth )
{
	const QMatrix4x4 invTrans = transform.getInvTransformMatrix();
	QVector3D localOri = invTrans * ori;
	QVector3D localDir = invTrans * dir;
	float t = - localOri.z() / localDir.z();
	float x = localOri.x() + t * localDir.x();
	float y = localOri.y() + t * localDir.y();
	if (x < -1.0f || x > 1.0f || y < -1.0f || y > 1.0f || t < -1e-2)
		return false;

	int w, h;
	canvas.getResolution(w, h);
	float u = (x + 1.0f) / 2.0f;
	float v = (y + 1.0f) / 2.0f;
	int xPixel = u * (w - 1);
	int yPixel = v*  (h - 1);
	//qDebug() << "u" << (x + 1.0f) * 0.5f << "v" << (y + 1.0f) * 0.5f;
	colorParam = canvas.getTotalColorPixel(xPixel,yPixel);
	surfParam  = canvas.getTotalSurfacePixel(xPixel, yPixel);
	depth = canvas.getTotalThicknessPixel(xPixel,yPixel);
	return true;
}

void PlanePicker::drawAppearance()
{
	Mesh::drawAppearance();


	appearProgram->release();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixd(transform.getTransformMatrix().constData());
	glLineWidth(3.0f);
	if (isObjSelected)
	{
		glColor3f(116.0 / 255.0f,190.0 / 255.0f,160.0 / 255.0f);
	}
	else
	{
		glColor3f(0.3,0.3,0.3);
	}
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1,-1,0);
	glVertex3f(-1,1,0);
	glVertex3f(1,1,0);
	glVertex3f(1,-1,0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-1,-1,0);
	glVertex3f(-1,-1,-0.5);

	glVertex3f(-1,1,0);
	glVertex3f(-1,1,-0.5);

	glVertex3f(1,1,0);
	glVertex3f(1,1,-0.5);

	glVertex3f(1,-1,0);
	glVertex3f(1,-1,-0.5);
	glEnd();
	glPopMatrix();
	appearProgram->bind();
}


ImagePicker::ImagePicker() :PaintPicker(), Mesh()
{

}


QDataStream& operator<<(QDataStream& out, const PlanePicker&picker)
{
	operator<<(out, (Mesh&)picker);
	return out;
}
QDataStream& operator>>(QDataStream& in, PlanePicker&picker)
{
	operator>>(in, (Mesh&)picker);
	return in;
}