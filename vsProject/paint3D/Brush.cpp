#include "stdafx.h"
#include "Brush.h"

float TriangleProjData::expTable[30] = 
{1,0.905,0.819,0.741,0.67,0.607,0.549,0.497,0.449,0.407,0.368,0.333,0.301,0.273,
0.247,0.223,0.202,0.183,0.165,0.15,0.135,0.122,0.111,0.1,0.091,0.082,0.074,0.067,
0.061,0.055};

// ��̬����
PixelUpdateArray Brush::updateArray;


Brush::Brush(Scene* scene)
{
	purePaintPicker = QSharedPointer<PurePaintPicker>(new PurePaintPicker());
	resolution[0] = resolution[1] = 50;
	purePaintPicker->setResolution(resolution[0], resolution[1]);
	isActive = true;
	this->scene = scene;
	sceneCamera = &scene->getCamera();
	sceneExposer = &scene->getScreenExposer();

	nearPlane = BRUSH_CAM_NEAR_PLANE;
	farPlane  = BRUSH_CAM_FAR_PLANE;
	intervalRatio = 0.2f;
	size     = 1.0f;
	resRatio = 50;
	angle    = 0.0f;
	ratio    = 1.0f;

	sizeJitter = 0.0;
	angleJitter = 0.0;
	ratioJitter = 0.0;
	offsetJitter = 0.0f;

	isAffectColor = 1.0f;
	isAffectReflLevel = 1.0f;
	isAffectRefrLevel = 1.0f;
	isAffectGlossness = 1.0f;
	isAffectIOR = 1.0f;
	isAffectThickness = 1.0f;

	isBrushMode = 1.0f;

	randInt  = 12345;

	updateCurParams();

	curPaintPicker = purePaintPicker.toWeakRef();

	brushExposer.init(resolution[0], resolution[1]);
	//QMessageBox::information(NULL, QObject::tr("Info"), QObject::tr("brush exposer initialized."));
	brushExposer.setProjectionType(GeometryExposer::GE_PROJ_ORTHOGRAPHIC);
	brushExposer.setDepthType(GeometryExposer::GE_DEPTH_THICKNESS);

	int totalPixels = resolution[0] * resolution[1];
	GeometryExposer::NormalTexPixelData *norArrayBuffer;
	GeometryExposer::UVTexPixelData * uvArrayBuffer;
	norArrayBuffer = new GeometryExposer::NormalTexPixelData[totalPixels * BRUSH_MAX_GE_BUFFERS];
	uvArrayBuffer = new GeometryExposer::UVTexPixelData[totalPixels * BRUSH_MAX_GE_BUFFERS];
	for (int ithBuf = 0; ithBuf < BRUSH_MAX_GE_BUFFERS; ++ithBuf)
	{
		geoInfoBuf[ithBuf].norArray = norArrayBuffer + totalPixels * ithBuf;
		geoInfoBuf[ithBuf].uvArray = uvArrayBuffer + totalPixels * ithBuf;
	}

	setNormalBlendCurveCoef(1,1,1,1);
	setDepthBlendCurveCoef (1,1,1,1);

}

Brush::~Brush()
{
	delete[] geoInfoBuf[0].norArray;
	delete[] geoInfoBuf[0].uvArray;
}

void Brush::updateCurParams()
{
	curAngle = angle  + getRandomFloat_m11() * angleJitter * M_PI;
	float sizeTmp  = std::max(size  * (1 + getRandomFloat_m11() * sizeJitter), 0.05f);
	float ratioTmp = std::max(ratio * (1 + getRandomFloat_m11() * ratioJitter), 0.05f);
	curSize = QVector2D(sizeTmp, sizeTmp * ratioTmp);
	curOffset[0] = size * offsetJitter * getRandomFloat_m11();
	curOffset[1] = size * offsetJitter * getRandomFloat_m11();

	float cosAngle = cos(curAngle);
	float sinAngle = sin(curAngle);
	paintOrigin = dispOrigin + curOffset[0] * dispLocalCoord[0] + curOffset[1] * dispLocalCoord[1];
	paintLocalCoord[0] = dispLocalCoord[0] * cosAngle + dispLocalCoord[1] * sinAngle;
	paintLocalCoord[1] = dispLocalCoord[0] * -sinAngle + dispLocalCoord[1] * cosAngle;
	paintLocalCoord[2] = dispLocalCoord[2];

	QVector3D end = paintOrigin + paintLocalCoord[2];
	viewMatrix.setToIdentity();
	viewMatrix.lookAt(paintOrigin, end, paintLocalCoord[1]);
	projMatrix.setToIdentity();
	QVector3D halfSize = curSize / 2.0f;
	projMatrix.ortho(-halfSize.x(), halfSize.x(), 
		-halfSize.y(), 
		halfSize.y(), 
		nearPlane, 
		farPlane);
}

void Brush::draw()
{
	if ( isActive && curObj)
	{
		glColor3f(0.3,0.3,0.3);

		const float deltaAngle = M_PI * 2.0f / BRUSH_DRAW_SEGMENTS;
		glLineWidth(1.0f);
		QVector3D point;
		glBegin(GL_LINE_LOOP);
		for (float agl = 0; agl < M_PI * 2; agl += deltaAngle)
		{
			point = dispOrigin + (dispLocalCoord[0] * cos(agl) + dispLocalCoord[1] * sin(agl)) * (size * 0.5);
			glVertex3f(point.x(), point.y(), point.z());
		}
		glEnd();

		QVector3D begin = dispOrigin + dispLocalCoord[2] * nearPlane;
		QVector3D end = dispOrigin + dispLocalCoord[2] * farPlane;
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glVertex3f(begin.x(), begin.y(), begin.z());
		glVertex3f(end.x(), end.y(), end.z());
		glEnd();

		glPointSize(10);
		glBegin(GL_POINTS);
		glVertex3f(dispOrigin.x(), dispOrigin.y(), dispOrigin.z());
		glVertex3f(end.x(), end.y(), end.z());
		glEnd();
	}
}

void Brush::setResolution( int x, int y )
{
	x = max(x,1);
	y = max(y,1);
	resolution[0] = x; resolution[1] = y;
	brushExposer.setResolution(x, y);

	int totalPixels = resolution[0] * resolution[1];
	delete[] geoInfoBuf[0].norArray;
	delete[] geoInfoBuf[0].uvArray;
	GeometryExposer::NormalTexPixelData *norArrayBuffer;
	GeometryExposer::UVTexPixelData * uvArrayBuffer;
	norArrayBuffer = new GeometryExposer::NormalTexPixelData[totalPixels * BRUSH_MAX_GE_BUFFERS];
	uvArrayBuffer = new GeometryExposer::UVTexPixelData[totalPixels * BRUSH_MAX_GE_BUFFERS];
	for (int ithBuf = 0; ithBuf < BRUSH_MAX_GE_BUFFERS; ++ithBuf)
	{
		geoInfoBuf[ithBuf].norArray = norArrayBuffer + totalPixels * ithBuf;
		geoInfoBuf[ithBuf].uvArray = uvArrayBuffer + totalPixels * ithBuf;
	}
	purePaintPicker->setResolution(x,y);
}

void Brush::setPosWorld( const QVector3D& ori, const QVector3D& x, const QVector3D& y, const QVector3D& z )
{
	dispOrigin = ori;
	dispLocalCoord[0] = x;
	dispLocalCoord[1] = y;
	dispLocalCoord[2] = z;
}


void PaintThread::run()
{	
	for (int ithPoint = 0; ithPoint < brush->paintPos.size(); ++ithPoint)
	{
		brush->fullSlots->acquire(1);
		brush->paintToObject(ithPoint);
		brush->emptySlots->release(1);
	}
}

void Brush::worldPaint()
{
	if (stroke.isNull() || !paintPos.size())
		return;
	
	emptySlots = new QSemaphore(BRUSH_MAX_GE_BUFFERS);
	fullSlots = new QSemaphore(0);


	PaintThread renderThread(this);
	renderThread.start();

	// �������ж�ȡ��Ϣ,��д����ͼ
	int totalPixels = resolution[0] * resolution[1];
	for (int ithPoint = 0; ithPoint < paintPos.size(); ++ithPoint)
	{
		// ����������Ϣ
		// ���ȼ�������������
		Brush::PaintPosition& pos = paintPos[ithPoint];
		dispOrigin = pos.ori;
		dispLocalCoord[0] = pos.localCoord[0];
		dispLocalCoord[1] = pos.localCoord[1];
		dispLocalCoord[2] = pos.localCoord[2];

		updateCurParams();
		pos.paintOri = paintOrigin;
		pos.paintLocalCoord[0] = paintLocalCoord[0] * curSize.x() / 2.0f;
		pos.paintLocalCoord[1] = paintLocalCoord[1] * curSize.y() / 2.0f;
		pos.paintLocalCoord[2] = paintLocalCoord[2];

		// ��Ⱦһ������
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(viewMatrix.constData());

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadMatrixf(projMatrix.constData());

		brushExposer.exposeGeometry();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		// ���Ի�÷���Ȩ
		emptySlots->acquire(1);
		// ��÷���Ȩ�󣬰Ѽ�����Ϣд��buffer�У���Brushʹ��
		// ����Ϣд��brush��buffer
		const GeometryExposer::NormalTexPixelData* norArray = brushExposer.getNormalAndDepthArray();
		const GeometryExposer::UVTexPixelData* uvArray = brushExposer.getUVAndIDArray();
		int curBuf = ithPoint % BRUSH_MAX_GE_BUFFERS;

		memcpy(geoInfoBuf[curBuf].norArray, norArray, totalPixels * sizeof(GeometryExposer::NormalTexPixelData));
		memcpy(geoInfoBuf[curBuf].uvArray, uvArray, totalPixels * sizeof(GeometryExposer::UVTexPixelData));
		fullSlots->release(1);
	}

	renderThread.wait();
	delete emptySlots;
	delete fullSlots;
	paintPos.clear();
}

void Brush::setObject( QWeakPointer<Mesh> obj )
{
	curObj = obj;
	brushExposer.setRenderObject(curObj);
}

void Brush::paintToObject(int ithPoint)
{
	if (curPaintPicker.isNull())
	{
		return;
	}
	int ithBuffer = ithPoint % BRUSH_MAX_GE_BUFFERS;
	GeometryExposer::NormalTexPixelData* norArray = geoInfoBuf[ithBuffer].norArray;
	GeometryExposer::UVTexPixelData* uvArray = geoInfoBuf[ithBuffer].uvArray;
	GeometryExposer::UVTexPixelData* pUVIDItem = uvArray;
	GeometryExposer::NormalTexPixelData* pNorItem = norArray;

	int meshTexRes[2];
	Mesh* mesh = curObj.data();
	unsigned int objID = mesh->getObjectID();
	Canvas& canvas = mesh->getCanvas();
	const QSet<int>& faceIDSet = mesh->getSelectFaceIDSet();
	int isPaintPart = faceIDSet.size() != 0;
	canvas.getResolution(meshTexRes[0], meshTexRes[1]);

	// ͳ�������С��ȣ����ڱ���������Ч��
	float maxDepth = FLT_MIN;
	float minDepth = FLT_MAX;
	
	// ��ɨ��һ�Σ���¼Ҫ���µ�����
	int ithBrushPixel = 0;
	QVector2D brushUV(0,0);			// ��ˢ����ϵ��uv����
	typedef QHash<unsigned, TriangleProjData> TriangleMap;
	TriangleMap triangleDataMap;	// ��¼��������Ϣ(��ֵ�����������)
	int curUpdatePixel = 0;			// ��ǰ��Ҫ���µ�������
	int totalPixels = meshTexRes[0] * meshTexRes[1];
	PaintPicker* picker = curPaintPicker.data();
	for (int brushPixelY = 0; brushPixelY < resolution[1]; ++brushPixelY)
	{
		brushUV.setY(float(brushPixelY + 0.5) / (resolution[1]));
		for (int brushPixelX = 0; brushPixelX < resolution[0]; ++brushPixelX, ++ithBrushPixel, ++pUVIDItem, ++pNorItem)
		{
			// ȥ��������
			if (pUVIDItem->objectID != objID )
				continue;

			// ���ѡ��һЩ�棬ֻ��ѡ�е����򻭻�
			int faceID = pUVIDItem->faceID & FACE_ID_MASK;
			if (isPaintPart && !faceIDSet.contains(faceID))
				continue;

			// ��ö�Ӧ��ˢ���صĲ�������
			// brushUV (0 ~ 1) brushUV2  (-1 ~ 1) 
			brushUV.setX(float(brushPixelX + 0.5) / (resolution[0]));
			QVector2D brushUV2 = brushUV * 2 - QVector2D(1,1);				
			QVector3D sampleOri = paintPos[ithPoint].paintOri + 
				brushUV2.x() * paintPos[ithPoint].paintLocalCoord[0] + 
				brushUV2.y() * paintPos[ithPoint].paintLocalCoord[1];
			QVector3D& sampleDir = paintPos[ithPoint].paintLocalCoord[2];
			QVector4D colorParam, surfParam;
			float thicknessParam;
			if(!picker->pickPaint(sampleOri, sampleDir, brushUV, colorParam, surfParam, thicknessParam))
				continue;
			colorParam.setW(colorParam.w() * stroke->getOpacity(brushUV));
			if (colorParam.w() < 0.0005f)
				continue;

			// �ҳ�ÿ����ˢ���ض�Ӧ�ķ��ߺ������Ϣ,ͬʱͳ�������С���
			float norProj = (pNorItem->nz - 32767.5) / 32767.5;
			float normalizedDepth = pNorItem->depth / 65535.0;
			float depth = normalizedDepth / (1 - normalizedDepth);
			maxDepth = max(depth, maxDepth);
			minDepth = min(depth, minDepth);

			// �ҳ�ÿ����ˢ���ض�Ӧ��������Ϣ��
			// ���û���ҵ���˵�������µ������Σ�Ӧ�ò����µ���Ϣ
			TriangleMap::iterator iTri = triangleDataMap.find(faceID);
			if (iTri == triangleDataMap.end())
			{
				unsigned triID = faceID;
				iTri = triangleDataMap.insert(triID, getTriangleData(triID, pUVIDItem->objectID));
			}
			TriangleProjData& triData = iTri.value();

			// Ŀ��λ�õ�������������ط�Χ
			float meshPixUVCenter[2] = {pUVIDItem->u / USHORT_NORMALIZE_FACTOR, pUVIDItem->v / USHORT_NORMALIZE_FACTOR};
			QVector2D meshPixUVExtend = triData.getCoordRange();

			// ������µľ������� {{xmin, ymin},{xmax, ymax}},������Ӱ������ؼ���
			// ȷ�����δ�С����Ϊ1������
			unsigned int meshPixRect[2][2];
			meshPixRect[0][0] = unsigned(std::max((meshPixUVCenter[0] - meshPixUVExtend.x()) * meshTexRes[0],0.f));
			meshPixRect[0][1] = unsigned(std::max((meshPixUVCenter[1] - meshPixUVExtend.y()) * meshTexRes[1],0.f));
			meshPixRect[1][0] = unsigned(std::min((meshPixUVCenter[0] + meshPixUVExtend.x()) * meshTexRes[0], meshTexRes[0]-1.f));
			meshPixRect[1][1] = unsigned(std::min((meshPixUVCenter[1] + meshPixUVExtend.y()) * meshTexRes[1], meshTexRes[1]-1.f));
			float invMeshResX = 1.0f /  (meshTexRes[0] - 1);
			float invMeshResY = 1.0f /	(meshTexRes[1] - 1);
			for (unsigned short meshPixelY = meshPixRect[0][1]; meshPixelY <= meshPixRect[1][1]; ++meshPixelY)
			{
				for (unsigned short meshPixelX = meshPixRect[0][0]; meshPixelX <= meshPixRect[1][0]; ++meshPixelX)
				{
					// ͨ��EWA����ֵ������Ȩ��, 
					// ����Ҫ���µ�����ֵ = ������ֵ1 * Ȩ��1 + ����ֵ2 * Ȩ��2 + ... + ����ֵn * Ȩ��n�� / ��Ȩ��1 + Ȩ��2 + ... + Ȩ��n��
					// ��Ϻ������ֵд��PixelUpdateItem��
					float meshPixelUVX = float(meshPixelX) * invMeshResX;
					float meshPixelUVY = float(meshPixelY) * invMeshResY;
					float blendWeight = triData.getWeightInEWA(
						meshPixelUVX - meshPixUVCenter[0],
						meshPixelUVY - meshPixUVCenter[1]);
					
					if (blendWeight < BRUSH_PIXEL_BLEND_THRESHOLD)
						continue;

					PixelUpdateItem item;
					item.rgba      = colorParam * blendWeight;
					item.surf      = surfParam  * blendWeight;
					item.thickness = thicknessParam * blendWeight;
					item.norProj   = norProj    * blendWeight;
					item.depth     = depth      * blendWeight;
					item.weight = blendWeight;
					item.x = meshPixelX; item.y = meshPixelY;

					unsigned int meshPixID = meshPixelY * meshTexRes[0] + meshPixelX;
					updateArray.insertItem(meshPixID, item);
				}
			}
		}
	}

	// ��Ҫ���µ�����д�����
	float invDepthRange = 1.0f / (maxDepth - minDepth);
	if (abs(maxDepth - minDepth) < 1e-4f)
		invDepthRange = 0.0f;

	float colorMask     = isAffectColor * isBrushMode;
	float reflLevelMask = isAffectReflLevel * isBrushMode;
	float glosLevelMask = isAffectGlossness * isBrushMode;
	float refrLevelMask = isAffectRefrLevel * isBrushMode;
	float iorMask       = isAffectIOR       * isBrushMode;
	float thicknessMask = isAffectThickness * isBrushMode;

	CanvasSeamFiller& seamFiller = canvas.getSeamFiller();
	for (int ithPixel = 0; ithPixel < updateArray.getUpdatePixelCount(); 
		 ++ithPixel)
	{
		// ������ջ�ϵ�����ֵ
		PixelUpdateItem& item = updateArray.getUpdateItem(ithPixel);
		float invSampleWeight = 1.0f / item.weight;
		item.rgba *= invSampleWeight;
		item.surf *= invSampleWeight;
		item.thickness *= invSampleWeight;
		item.norProj *= invSampleWeight;
		item.depth *= invSampleWeight;

		// ���¸�λλͼ
		unsigned int meshPixID = item.y * meshTexRes[0] + item.x;
		updateArray.clearBitmapItem(meshPixID);

		// ��������λ��
		unsigned short tarX = std::max(0,std::min(int(item.x), meshTexRes[0] - 1));
		unsigned short tarY = std::max(0,std::min(int(item.y), meshTexRes[1] - 1));

		// ���㷨�ߺ���ȹ���ֵ
		float norProjValue = abs(item.norProj);
		norProjValue *= norProjValue;
		norProjValue *= norProjValue;
		norProjValue *= norProjValue;
		float norFilter = normalBlendCurve[int(norProjValue * (BRUSH_BLEND_CURVE_SEGMENTS - 1))];
		float depthProjValue = (item.depth - minDepth) * invDepthRange;
		float depthFilter = depthBlendCurve[int(depthProjValue * (BRUSH_BLEND_CURVE_SEGMENTS - 1))];
		//qDebug() << norFilter << endl;

		// ������Դ���ػ�Ϻ����ɫ
		QVector4D curColor = canvas.getCurLayerColorPixel(tarX, tarY);
		QVector4D curSurf  = canvas.getCurLayerSurfacePixel(tarX, tarY);
		float     curThick = canvas.getCurLayerThicknessPixel(tarX, tarY);
		float curAlpha     = curColor.w();
		float paintAlpha   = item.rgba.w() * norFilter * depthFilter;	// ����alpha
		float relAlpha     = paintAlpha/* / (paintAlpha + curAlpha + 1e-5f)*/;	// ���ڻ�ϵ�alpha�ǻ���alphaռ��alpha����Ա���

		float colorFilter   = relAlpha * colorMask;
		QVector4D newRGB   = curColor * (1- colorFilter) + item.rgba * colorFilter;	
		float     newAlpha = paintAlpha * (isBrushMode - curAlpha) + curAlpha;

		QVector4D surfFilter(relAlpha * reflLevelMask,
			                 relAlpha * glosLevelMask,
							 relAlpha * refrLevelMask,
							 relAlpha * iorMask);
		QVector4D newSurf  = curSurf * (QVector4D(1,1,1,1) - surfFilter) + item.surf * surfFilter;

		float thicknessFilter = relAlpha * thicknessMask;
		float     newThick = curThick * (1-thicknessFilter) + item.thickness * thicknessFilter;


		// д��
		canvas.setCurLayerColorPixel(tarX, tarY, QVector4D(newRGB.toVector3D(), newAlpha));
		canvas.setCurLayerSurfacePixel(tarX, tarY, newSurf);
		canvas.setCurLayerThicknessPixel(tarX, tarY, newThick);

		// ����Ե
		QVector<PixelPos>& extraPos = seamFiller.getExtraFillPixels(tarX, tarY);
		for (QVector<PixelPos>::iterator pPixel = extraPos.begin();
			 pPixel != extraPos.end() ; ++pPixel)
		{
			int extraX = pPixel->x;
			int extraY = pPixel->y;
			canvas.setCurLayerColorPixel(extraX, extraY, QVector4D(newRGB.toVector3D(), newAlpha));
			canvas.setCurLayerSurfacePixel(extraX, extraY, newSurf);
			canvas.setCurLayerThicknessPixel(extraX, extraY, newThick);
		}
	}
	updateArray.reset();
}


TriangleProjData Brush::getTriangleData( int triID, int objID)
{
	TriangleProjData data;
	// �������
	Mesh* mesh = scene->getMesh(objID);	
	mesh->getTriangleData(triID, data.triData);
	data.objID = objID;
	data.faceID = triID;
	data.modelMatrix = mesh->getTransform().getTransformMatrix();

	// ���������ռ��������ӳ�䵽�ֲ��ռ�������
	TriangleData& triData = data.triData;
	QMatrix4x4 mvpMatrix = projMatrix * viewMatrix * data.modelMatrix;
	// ������ӳ�䵽��ˢ�ֲ��ռ�������ֵ
	QVector3D dP1 = (mvpMatrix * QVector4D(triData.vertex[1] - triData.vertex[0],0)).toVector3D();	
	QVector3D dP2 = (mvpMatrix * QVector4D(triData.vertex[2] - triData.vertex[0],0)).toVector3D();
	QVector2D dT1 = triData.texCoord[1] - triData.texCoord[0];
	QVector2D dT2 = triData.texCoord[2] - triData.texCoord[0];

	// ��������ռ�Ļ�����ӳ�䵽��ˢ�ֲ��ռ�������
	const float det = dT1.x() * dT2.y() - dT2.x() * dT1.y();
	QVector2D texU = QVector2D(
		(dP1.x() * dT2.y() - dP2.x() * dT1.y()) / det,
		(dP1.y() * dT2.y() - dP2.y() * dT1.y()) / det);
	QVector2D texV = QVector2D(
		(dP2.x() * dT1.x() - dP1.x() * dT2.x()) / det,
		(dP2.y() * dT1.x() - dP1.y() * dT2.x()) / det);

	// һ�������ھֲ�����ϵ�Ĵ�С
	QVector2D pixelCoord(2.0f / resolution[0] , 2.0f / resolution[1] );
	
	const float det2 = 1.0f / (texU.x() * texV.y() - texV.x() * texU.y());
	data.pixelCoordInUVSpace[0][0] = (pixelCoord.x() * texV.y()) * det2;
	data.pixelCoordInUVSpace[0][1] = (-pixelCoord.x() * texU.y()) * det2;
	data.pixelCoordInUVSpace[1][0] = (-pixelCoord.y() * texV.x()) * det2;
	data.pixelCoordInUVSpace[1][1] = (pixelCoord.y() * texU.x()) * det2;

	data.computeParam();
	return data;
}

void Brush::setColor( const QVector4D& colorParam, const QVector4D& surfParam, const float depth )
{
	purePaintPicker->setColorParam(colorParam);
	purePaintPicker->setSurfParam(surfParam);
	purePaintPicker->setDepth(depth);
}

void Brush::setAffectColor( int isAffect )
{
	isAffectColor = isAffect ? 1.0f : 0.0f;
}

void Brush::setAffectReflLevel( int isAffect )
{
	isAffectReflLevel = isAffect ? 1.0f : 0.0f;
}

void Brush::setAffectRefrLevel( int isAffect )
{
	isAffectRefrLevel = isAffect ? 1.0f : 0.0f;
}

void Brush::setAffectGlossness( int isAffect )
{
	isAffectGlossness = isAffect ? 1.0f : 0.0f;
}

void Brush::setAffectIOR( int isAffect )
{
	isAffectIOR = isAffect ? 1.0f : 0.0f;
}

void Brush::setAffectThickness( int isAffect )
{
	isAffectThickness = isAffect ? 1.0f : 0.0f;
}

void Brush::computeBlendCurve( float* curve, int nSegs, float zeroPos, float onePos, float zeroWeight, float oneWeight )
{
	float onePower = zeroWeight * zeroWeight * zeroWeight;
	float zeroPower = oneWeight * oneWeight * oneWeight;
	float delta = 1.0f / float(nSegs);
	float x = 0;
	for (int ithSeg = 0; ithSeg < nSegs; ++ithSeg, x += delta)
	{
		float w0 = pow(1 - x, zeroPower);
		float w1 = pow(x, onePower);
		curve[ithSeg] = (zeroPos * w0 + onePos * w1) / (w0 + w1);
	}
}

void Brush::setDepthBlendCurveCoef( float zeroPos, float onePos, float zeroWeight, float oneWeight )
{
	computeBlendCurve(depthBlendCurve, BRUSH_BLEND_CURVE_SEGMENTS, zeroPos, onePos, zeroWeight, oneWeight);
}

void Brush::setNormalBlendCurveCoef( float zeroPos, float onePos, float zeroWeight, float oneWeight )
{
	computeBlendCurve(normalBlendCurve, BRUSH_BLEND_CURVE_SEGMENTS, zeroPos, onePos, zeroWeight, oneWeight);
}

void Brush::setDepthAffectMode(GeometryExposer::DepthType mode)
{
	brushExposer.setDepthType(mode);
}

void Brush::addPaintPos( const QVector3D& ori, const QVector3D& x, const QVector3D& y, const QVector3D& z )
{
	PaintPosition pos;
	pos.ori = ori;
	pos.localCoord[0] = x;
	pos.localCoord[1] = y;
	pos.localCoord[2] = z;
	paintPos.push_back(pos);
}

void Brush::setCustomPaintPicker( QSharedPointer<PaintPicker> picker )
{
	curPaintPicker = picker.toWeakRef();
}

void Brush::setSize( float size )
{
	this->size = size;
	setResolution(size * resRatio, size * resRatio);
}
void Brush::setResolutionRatio( float ratio )
{
	resRatio = ratio;
	setResolution(size * resRatio, size * resRatio);
}
void Brush::beginPaint()
{
	if (curObj.isNull())
		return;
	Canvas& canvas = curObj.data()->getCanvas();
	int w, h;
	canvas.getResolution(w,h);
	int resSize = w * h;
	oldColorBuf.resize(resSize);
	oldSurfBuf.resize(resSize);
	oldThicknessBuf.resize(resSize);
	canvas.copyCurLayerColorArray(&oldColorBuf[0]);
	canvas.copyCurLayerSurfaceArray(&oldSurfBuf[0]);
	canvas.copyCurLayerThicknessArray(&oldThicknessBuf[0]);
}

void Brush::endPaint()
{
	if (curObj.isNull())
		return;
	Canvas& canvas = curObj.data()->getCanvas();
	int w, h;
	canvas.getResolution(w,h);
	int resSize = w * h;
	QVector<unsigned> newColorBuf(resSize), newSurfBuf(resSize);
	QVector<float> newThicknessBuf(resSize);
	canvas.copyCurLayerColorArray(&newColorBuf[0]);
	canvas.copyCurLayerSurfaceArray(&newSurfBuf[0]);
	canvas.copyCurLayerThicknessArray(&newThicknessBuf[0]);

	PaintCommand* cmd = new PaintCommand(curObj.data()->getObjectID(), canvas.getCurLayerID());
	for (int i = 0; i < resSize; ++i)
	{
		if (oldColorBuf[i] != newColorBuf[i] ||
			oldSurfBuf[i] != newSurfBuf[i] ||
			oldThicknessBuf[i] != newThicknessBuf[i])
		{
			PaintCommand::PixelRecord record;
			record.oldBGRAi = oldColorBuf[i];
			record.newBGRAi = newColorBuf[i];
			record.oldSurfi = oldSurfBuf[i];
			record.newSurfi = newSurfBuf[i];
			record.oldThickness = oldThicknessBuf[i];
			record.newThickness = newThicknessBuf[i];
			record.index = i;
			cmd->addRecord(record);
		}
	}
	scene->getUndoStack().push(cmd);
}

int Brush::getPlacerObjectID()
{
	// ����ǰ���ʲ��ô�ɫ�������IDΪҪ���������ID�������ʸ�����������滭
	// ���������ø��ּ��������PaintPicker������IDΪ��PaintPicker��Mesh�����ID
	if (curPaintPicker == purePaintPicker)
	{
		if (curObj)
		{
			return curObj.data()->getObjectID();
		}
	}
	else
	{
		ImagePicker* imgPkr = (ImagePicker*)curPaintPicker.data();
		return imgPkr->getObjectID();
	}
	return -1;
}

void Brush::setMode( BrushMode mode )
{
	switch (mode)
	{
	case MODE_BRUSH:
		isBrushMode = 1.0f;break;
	case MODE_ERASER:
		isBrushMode = 0.0f; break;;
	}
}

void Brush::setDefaultPaintPicker()
{
	curPaintPicker = purePaintPicker.toWeakRef();

}




PaintThread::PaintThread( Brush* brush, QObject *parent /*= 0*/ ) :QThread(parent)
{
	this->brush = brush;
}
