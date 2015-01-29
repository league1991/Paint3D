#include "StdAfx.h"
#include "CanvasSeamFiller.h"

CanvasSeamFiller::CanvasSeamFiller(void)
{
	fillWidth = 0.01;
}

CanvasSeamFiller::~CanvasSeamFiller(void)
{
}

void CanvasSeamFiller::init( int width, int height, QVector<QVector3D>& vertices, QVector<QVector2D>& texcoords, QVector<ObjTriangle>& faces )
{
	this->width = width;
	this->height = height;

	QMultiHash<EdgePointID, EdgeTriID> borderTexcoordEdge;
	QHash<EdgePointID, EdgePointID> commonPosEdgeMap;

	extractCommonPositionEdge(faces, texcoords, commonPosEdgeMap);
	extractBorderTexcoordEdge(faces, texcoords, borderTexcoordEdge);
	
	QVector<TexcoordEdge> texEdgeArray;
	QVector<TexcoordVertex> texVtxArray;
	QVector<int> texEdgeSetArray;
	// 构建边集合
	buildTexcoordEdge(
		texcoords, faces, borderTexcoordEdge, commonPosEdgeMap,
		texEdgeArray, texVtxArray, texEdgeSetArray );

	// 填充到一张分辨率等于贴图大小的位图中
	QImage triangleMaskImage = QImage(width, height,QImage::Format_ARGB32);
	triangleMaskImage.fill(0x0);
	QPainter painter(&triangleMaskImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	for (unsigned ithEdge = 0; ithEdge < texEdgeArray.size(); ++ithEdge)
	{
		QRgb edgeColor = ithEdge;
		painter.setPen(QPen(QColor(edgeColor)));
		painter.setBrush(QBrush(QColor(edgeColor)));
		TexcoordEdge& edge = texEdgeArray[ithEdge];
		TexcoordVertex& v1 = texVtxArray[edge.startVtxID];
		TexcoordVertex& v2 = texVtxArray[edge.endVtxID];
		QVector2D v1n = v1.vertex + v1.normal;
		QVector2D v2n = v2.vertex + v2.normal;
		QPointF points[4] = 
		{
			QPointF(v1.vertex.x() * width, (1-v1.vertex.y()) * height),
			QPointF(v1n.x() * width, (1-v1n.y()) * height),
			QPointF(v2n.x() * width, (1-v2n.y()) * height),
			QPointF(v2.vertex.x() * width, (1-v2.vertex.y()) * height)
		};
		painter.drawPolygon(points, 4);
	}
	painter.setPen(QPen(QColor(0,0,0,0)));
	painter.setBrush(QBrush(QColor(0,0,0,0)));
	for (int ithTri = 0; ithTri < faces.size(); ++ithTri)
	{
		QVector2D& t1 = texcoords[faces[ithTri].texcoordIndex[0]];
		QVector2D& t2 = texcoords[faces[ithTri].texcoordIndex[1]];
		QVector2D& t3 = texcoords[faces[ithTri].texcoordIndex[2]];

		QPointF points[3] = 
		{
			QPointF(t1.x() * width, (1-t1.y()) * height),
			QPointF(t2.x() * width, (1-t2.y()) * height),
			QPointF(t3.x() * width, (1-t3.y()) * height)
		};
		painter.drawPolygon(points, 3);
	}
	triangleMaskImage.save("triangleMask.png");
/*
	// 建立像素查找索引
	fillIDMap.push_back(QVector<PixelPos>());				// 加入一个空向量
	pixelMap.insert(0,width * height, 0);	// 默认所有像素都指向这个空向量
	const unsigned* pPixel = (unsigned*)triangleMaskImage.constBits();
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x,++pPixel)
		{
			// 像素在边界附近，且被填充
			if (*pPixel != 0)
			{
				QVector2D uvSrcCoord(float(x) / (width  - 1),
					                 float(y) / (height - 1)), uvDstCoord;
				TexcoordEdge& edge = texEdgeArray[*pPixel];
				
				// 找出颜色填充到这个像素的原像素位置
				if (edge.mapper.convert(uvSrcCoord, uvDstCoord))
				{
					int dstX = uvDstCoord.x() * (width - 1);
					int dstY = uvDstCoord.y() * (height - 1);
					if (dstX >= 0 && dstX < width &&
						dstY >= 0 && dstY < height)
					{
						int& pixelID = pixelMap[dstY * width + dstX];
						if (pixelID == 0)
						{	// 插入一个新点
							pixelID = fillIDMap.size();
							fillIDMap.push_back(QVector<PixelPos>(1,PixelPos(x, y)));
						}
						else
						{
							fillIDMap[pixelID].push_back(PixelPos(x, y));
						}
					}
				}
				
			}
		}
	}*/
}

void CanvasSeamFiller::extractCommonPositionEdge( const QVector<ObjTriangle>& faces, const QVector<QVector2D>texcoords, QHash<EdgePointID, EdgePointID>& edgePointMap )
{
	QMultiHash<EdgePointID, EdgeTriID> edgeMap;
	// 把所有边以顶点ID为key加到HashMap
	for (int ithTri = 0; ithTri < faces.size(); ++ithTri)
	{
		const ObjTriangle& tri = faces[ithTri];
		for (int ithEdge = 0; ithEdge < 3; ++ithEdge)
		{
			EdgePointID triEdge;
			triEdge.v1ID = tri.vertIndex[ithEdge];
			triEdge.v2ID = tri.vertIndex[(ithEdge + 1) % 3];

			EdgeTriID edgePos;
			edgePos.edgeID = ithEdge;
			edgePos.faceID = ithTri;
			edgeMap.insertMulti(triEdge, edgePos);
		}
	}

	// 找出在世界空间中被两个三角形共享，
	// 而在纹理空间中属于边界的边
	// 把它在纹理空间中的一组顶点索引加到一个edgeMap里面
	for (QMultiHash<EdgePointID, EdgeTriID>::iterator pCurEdge = edgeMap.begin(),pNextEdge = pCurEdge + 1;
		 pNextEdge != edgeMap.end();++pCurEdge, ++pNextEdge)
	{
		// 在世界空间中是公共边
		if(pNextEdge.key() == pCurEdge.key())
		{
			const EdgeTriID& curEdgeTriID = pCurEdge.value();
			const EdgeTriID& nextEdgeTriID = pNextEdge.value();

			// 找出这两条公共边的贴图坐标顶点序号
			EdgePointID curPointID, nextPointID;
			curPointID.v1ID = faces[curEdgeTriID.faceID].texcoordIndex[curEdgeTriID.edgeID];
			curPointID.v2ID = faces[curEdgeTriID.faceID].texcoordIndex[(curEdgeTriID.edgeID+1)%3];
			nextPointID.v1ID = faces[nextEdgeTriID.faceID].texcoordIndex[nextEdgeTriID.edgeID];
			nextPointID.v2ID = faces[nextEdgeTriID.faceID].texcoordIndex[(nextEdgeTriID.edgeID+1)%3];

			// 如果顶点不一致，说明这两条边在纹理空间不相邻
			if (!(curPointID == nextPointID))
			{

				// 调整边的顺序，使得其总是沿着逆时针方向
				QVector2D v1  = texcoords[faces[curEdgeTriID.faceID].texcoordIndex[0]];
				QVector2D v21 = texcoords[faces[curEdgeTriID.faceID].texcoordIndex[1]] - v1;
				QVector2D v31 = texcoords[faces[curEdgeTriID.faceID].texcoordIndex[2]] - v1;
				if (v21.x()*v31.y() - v21.y()*v31.x() < 0)
				{
					// v21 x v31 < 0 说明边沿顺时针方向
					int tmp = curPointID.v1ID;
					curPointID.v1ID = curPointID.v2ID;
					curPointID.v2ID = tmp;
				}

				v1  = texcoords[faces[nextEdgeTriID.faceID].texcoordIndex[0]];
				v21 = texcoords[faces[nextEdgeTriID.faceID].texcoordIndex[1]] - v1;
				v31 = texcoords[faces[nextEdgeTriID.faceID].texcoordIndex[2]] - v1;
				if (v21.x()*v31.y() - v21.y()*v31.x() < 0)
				{
					// v21 x v31 < 0 说明边沿顺时针方向
					int tmp = nextPointID.v1ID;
					nextPointID.v1ID = nextPointID.v2ID;
					nextPointID.v2ID = tmp;
				}

				edgePointMap.insert(curPointID, nextPointID);
				edgePointMap.insert(nextPointID, curPointID);
			}
		}
	}
}

void CanvasSeamFiller::extractBorderTexcoordEdge( const QVector<ObjTriangle>& faces, const QVector<QVector2D>texcoords, QMultiHash<EdgePointID, EdgeTriID>& edgeMap )
{
	// 把所有边以顶点ID为key加到HashMap
	for (int ithTri = 0; ithTri < faces.size(); ++ithTri)
	{
		const ObjTriangle& tri = faces[ithTri];
		for (int ithEdge = 0; ithEdge < 3; ++ithEdge)
		{
			EdgePointID triEdge;

			// 调整边的顺序，使得其总是沿着逆时针方向
			QVector2D v1 = texcoords[tri.texcoordIndex[0]];
			QVector2D v2 = texcoords[tri.texcoordIndex[1]];
			QVector2D v3 = texcoords[tri.texcoordIndex[2]];
			QVector2D v21 = v2 - v1;
			QVector2D v31 = v3 - v1;
			if (v21.x()*v31.y() - v21.y()*v31.x() < 0)
			{
				// v21 x v31 < 0 说明边沿顺时针方向
				triEdge.v2ID = tri.texcoordIndex[ithEdge];
				triEdge.v1ID = tri.texcoordIndex[(ithEdge + 1) % 3];
			}
			else
			{	// 边沿逆时针方向
				triEdge.v1ID = tri.texcoordIndex[ithEdge];
				triEdge.v2ID = tri.texcoordIndex[(ithEdge + 1) % 3];
			}

			EdgeTriID edgePos;
			edgePos.edgeID = ithEdge;
			edgePos.faceID = ithTri;
			edgeMap.insertMulti(triEdge, edgePos);
		}
	}

	// 去除被多个三角形共享的边
	QList<EdgePointID> keys = edgeMap.uniqueKeys();
	for (QList<EdgePointID>::iterator pKey = keys.begin();
		pKey != keys.end(); ++pKey)
	{
		if (edgeMap.count(*pKey) != 1)
		{
			edgeMap.remove(*pKey);
		}
	}
}

void CanvasSeamFiller::buildTexcoordEdge(
	const QVector<QVector2D>& texcoords,
	const QVector<ObjTriangle>& faces,
	const QMultiHash<EdgePointID, EdgeTriID>& edgeMap,
	const QHash<EdgePointID, EdgePointID>& edgeCommonPointMap,
	QVector<TexcoordEdge>& edgeArray, 
	QVector<TexcoordVertex>& vertexArray, 
	QVector<int>& edgeSetArray )
{
	QVector<int> vertexMap(texcoords.size(), -1);// 记录从texcoords数组下标到vertexArray下标的映射

	QHash<EdgePointID, int> edgeOffsetMap;		 // 记录从EdgePointID到该边在edgeArray中的下标的映射
	// 首先构建边表和顶点表
	for (QMultiHash<EdgePointID, EdgeTriID>::const_iterator pEdge = edgeMap.begin();
		 pEdge != edgeMap.end(); ++pEdge)
	{
		const EdgePointID& edgeKey = pEdge.key();

		// 如果边的任意一端顶点未添加入输出的顶点数组，
		// 先把顶点添加进去
		if (vertexMap[edgeKey.v1ID] == -1)
		{
			vertexMap[edgeKey.v1ID] = vertexArray.size();
			vertexArray.push_back(TexcoordVertex(texcoords[edgeKey.v1ID]));
		}
		if (vertexMap[edgeKey.v2ID] == -1)
		{
			vertexMap[edgeKey.v2ID] = vertexArray.size();
			vertexArray.push_back(TexcoordVertex(texcoords[edgeKey.v2ID]));
		}

		// 设置顶点的边序号
		TexcoordVertex& v1 = vertexArray[vertexMap[edgeKey.v1ID]];
		TexcoordVertex& v2 = vertexArray[vertexMap[edgeKey.v2ID]];

		// 把边加入起止点
		int edgeID = edgeArray.size();
		v1.exitEdgeID.push_back(edgeID);
		v2.enterEdgeID.push_back(edgeID);

		// 添加新边,同时设置新边的顶点序号
		const EdgeTriID& edgeTri = pEdge.value();
		QVector2D start = texcoords[edgeKey.v1ID];
		QVector2D end   = texcoords[edgeKey.v2ID];
		QVector3D edgeDir = (end - start).toVector3D();
		QVector2D normal  = QVector2D(edgeDir.y(), -edgeDir.x());
		normal.normalize();
		edgeOffsetMap.insert(edgeKey, edgeArray.size());						// 记录当前边在边数组的位置
		edgeArray.push_back(TexcoordEdge(
			vertexMap[edgeKey.v1ID],
			vertexMap[edgeKey.v2ID], 
			edgeTri.faceID,
			edgeTri.edgeID,
			normal));
	}
	// 找出边的公共边
	int ithEdge = 0;
	for (QVector<TexcoordEdge>::iterator pEdge = edgeArray.begin();
		 pEdge != edgeArray.end(); ++pEdge, ++ithEdge)
	{
		EdgePointID edgeID;
		edgeID.v1ID = pEdge->startVtxID;
		edgeID.v2ID = pEdge->endVtxID;
		// 检查当前边是否有公共边
		if (edgeCommonPointMap.contains(edgeID))
		{
			const EdgePointID& commonEdgeID = edgeCommonPointMap[edgeID];
			int commonEdgeOffset = edgeOffsetMap[commonEdgeID];
			edgeArray[ithEdge].commonEdgeID = commonEdgeOffset;
			edgeArray[commonEdgeOffset].commonEdgeID = ithEdge;
		}
	}
	splitCommonVertex(edgeArray, vertexArray);
	// 把边连起来，同时理顺startVtx 同 endVtx,并计算顶点法线
	linkTexcoordEdge(edgeArray, vertexArray, edgeSetArray);
	buildCoordMapper(edgeArray, vertexArray);
}
void CanvasSeamFiller::splitCommonVertex(
						QVector<TexcoordEdge>& edgeArray,
						QVector<TexcoordVertex>& vertexArray)
{
	struct CommonEdge
	{
		int edgeID;			// 边的序号
		float angle;		// 边的角度
		int type;			// 边的类型，入边为0，出边为1
	};
	int vertexArrayLength = vertexArray.size();
	for (int ithVert = 0; ithVert < vertexArrayLength; ++ithVert)
	{
		TexcoordVertex& vtx = vertexArray[ithVert];
		// 发现被两条以上边共享的顶点
		if (vtx.enterEdgeID.size() > 1 && vtx.exitEdgeID.size() > 1 &&
			vtx.enterEdgeID.size() == vtx.exitEdgeID.size())
		{
			// 把边都添加到向量中
			QVector<CommonEdge> comEdges;
			for (int ithEnter = 0; ithEnter < vtx.enterEdgeID.size();++ithEnter)
			{
				int enterEdgeID = vtx.enterEdgeID[ithEnter];
				TexcoordEdge& enterEdge = edgeArray[enterEdgeID];
				QVector2D dir = vertexArray[enterEdge.startVtxID].vertex - vtx.vertex;
				CommonEdge comEdge;
				comEdge.edgeID = enterEdgeID;
				comEdge.angle = atan2(dir.y(), dir.x());
				comEdge.type = 0;
				comEdges.push_back(comEdge);
			}
			for (int ithExit = 0; ithExit < vtx.exitEdgeID.size();++ithExit)
			{
				int exitEdgeID = vtx.exitEdgeID[ithExit];
				TexcoordEdge& exitEdge = edgeArray[exitEdgeID];
				QVector2D dir = vertexArray[exitEdge.endVtxID].vertex - vtx.vertex;
				CommonEdge comEdge;
				comEdge.edgeID = exitEdgeID;
				comEdge.angle = atan2(dir.y(), dir.x());
				comEdge.type = 1;
				comEdges.push_back(comEdge);
			}
			// 根据角度对边排序
			for (int i = 0; i < comEdges.size() - 1; ++i)
			{
				for (int j = i+1; j < comEdges.size(); ++j)
				{
					if (comEdges[i].angle > comEdges[j].angle)
					{
						const CommonEdge tmp = comEdges[i];
						comEdges[i] = comEdges[j];
						comEdges[j] = tmp;
					}
				}
			}
			
			for (int edge0ID = 0, count = 0; 
				 edge0ID < comEdges.size(); ++edge0ID)
			{
				int edge1ID = (edge0ID + 1) % comEdges.size();
				CommonEdge& edge0 = comEdges[edge0ID];
				CommonEdge& edge1 = comEdges[edge1ID];
				// 遇到相邻的边，且0为入边， 1为出边
				if (edge0.type == 0 && edge1.type == 1)
				{
					int curVtxID;
					if (count == 0)
						curVtxID = ithVert;
					else
					{
						curVtxID = vertexArray.size();
						vertexArray.push_back(TexcoordVertex(vtx.vertex));
					}
					// 使得顶点指向新的边
					TexcoordVertex& curVertex = vertexArray[curVtxID];
					curVertex.enterEdgeID.clear();
					curVertex.exitEdgeID.clear();
					curVertex.enterEdgeID.push_back(edge0.edgeID);
					curVertex.exitEdgeID.push_back(edge1.edgeID);

					// 使得边指向新的顶点
					edgeArray[edge0.edgeID].endVtxID = curVtxID;
					edgeArray[edge1.edgeID].startVtxID = curVtxID;
					count++;
				}
			}
		}
	}
}
void CanvasSeamFiller::linkTexcoordEdge(
					  QVector<TexcoordEdge>& edgeArray,
					  QVector<TexcoordVertex>& vertexArray,
					  QVector<int>& edgeSetArray)
{
	QSet<unsigned> edgeIDSet;								// 存储未处理的edgeArray的下标的集合
	for (int i = 0; i < edgeArray.size(); ++i)
		edgeIDSet.insert(i);

	while (!edgeIDSet.isEmpty())
	{
		// 建立新的边集
		int startEdgeID = *edgeIDSet.begin();
		edgeSetArray.push_back(startEdgeID);
		for (int edgeID = startEdgeID;;)
		{
			TexcoordEdge& edge = edgeArray[edgeID];
			TexcoordVertex& endVtx = vertexArray[edge.endVtxID];

			// 改正当前边所指向的顶点
			if (edgeID != endVtx.enterEdgeID[0])
			{
				endVtx.exitEdgeID[0] = endVtx.enterEdgeID[0];
				endVtx.enterEdgeID[0] = edgeID;
			}

			// 计算顶点法线
			TexcoordEdge& nextEdge = edgeArray[endVtx.exitEdgeID[0]];
			QVector2D& normal1 = edge.normal;
			QVector2D& normal2 = nextEdge.normal;
			QVector2D  vtxNormal = normal1 + normal2;
			float length = vtxNormal.length();
			float factor = fillWidth * min(sqrt(2.0f / (QVector2D::dotProduct(normal1, normal2) + 1)), 3.0f);
			endVtx.normal = factor / length * vtxNormal;

			edgeIDSet.remove(edgeID);
			// 到达起始边
			if (endVtx.exitEdgeID[0] == startEdgeID)
				break;

			// 改正下一条边
			if (nextEdge.startVtxID != edge.endVtxID)
			{
				nextEdge.endVtxID = nextEdge.startVtxID;
				nextEdge.startVtxID = edge.endVtxID;
			}

			edgeID = endVtx.exitEdgeID[0];
		}
	}
}
void CanvasSeamFiller::buildCoordMapper(
					QVector<TexcoordEdge>& edgeArray,
					QVector<TexcoordVertex>& vertexArray)
{
	for (int ithEdge = 0; ithEdge < edgeArray.size(); ++ithEdge)
	{
		TexcoordEdge& edge = edgeArray[ithEdge];
		QVector2D& srcO1 = vertexArray[edge.startVtxID].vertex;
		QVector2D& srcO2 = vertexArray[edge.endVtxID].vertex;
		QVector2D& srcV1 = vertexArray[edge.startVtxID].normal;
		QVector2D& srcV2 = vertexArray[edge.endVtxID].normal;
		edge.mapper.setSrcShape(srcO1, srcO2, srcV1, srcV2);

		if (edge.commonEdgeID == -1)
		{
			edge.mapper.setDstShape(srcO1, srcO2, -srcV1, -srcV2);
		}
		else
		{
			TexcoordEdge& dstEdge = edgeArray[edge.commonEdgeID];
			QVector2D& dstO1 = vertexArray[dstEdge.startVtxID].vertex;
			QVector2D& dstO2 = vertexArray[dstEdge.endVtxID].vertex;
			QVector2D& dstV1 = vertexArray[dstEdge.startVtxID].normal;
			QVector2D& dstV2 = vertexArray[dstEdge.endVtxID].normal;
			edge.mapper.setDstShape(dstO1, dstO2, -dstV1, -dstV2);
		}
	}
}

QVector<PixelPos>& CanvasSeamFiller::getExtraFillPixels( int x, int y )
{
	return fillIDMap[pixelMap[y * width + x]];
}

void CoordMapper::setSrcShape( const QVector2D& srcO1, const QVector2D& srcO2, const QVector2D& srcV1, const QVector2D& srcV2 )
{
	this->srcO = srcO1;
	this->srcU = srcO2 - srcO1;
	this->srcV1 = srcV1;
	this->srcV21 = srcV2 - srcV1;
	A = srcU.x() * srcV21.y() - srcU.y() * srcV21.x();
	B = srcV1.x() * srcV21.y() - srcV1.y() * srcV21.x();
	a = srcU.y() * srcV21.x() - srcU.x() * srcV21.y();
	b1 = srcU.y() * srcV1.x() - srcU.x() * srcV1.y();
}

void CoordMapper::setDstShape( const QVector2D& dstO1, const QVector2D& dstO2, const QVector2D& dstV1, const QVector2D& dstV2 )
{
	this->dstO = dstO1;
	this->dstU = dstO2 - dstO1;
	this->dstV1 = dstV1;
	this->dstV21 = dstV2 - dstV1;
}

bool CoordMapper::convert( const QVector2D& srcPoint, QVector2D& dstPoint )
{
	QVector2D srcD = srcPoint - srcO;
	float C = srcD.x() * srcV21.y() - srcD.y() * srcV21.x();
	float b = b1 + C;
	float c = srcV1.y() * srcD.x() - srcV1.x() * srcD.y();
	float delta = b * b - 4 * a * c;
	if (delta < 0)
		return false;
	float det = sqrt(delta);
	float u1 = (-b + det) / (2 * a);
	float u2 = (-b - det) / (2 * a);
	float u  = (u1 >= 0 && u1 <= 1) ? u1 : u2;
	float v  = (C - A * u) / B;

	dstPoint = dstO + u * dstU + u * v * dstV21 + v * dstV1;
	return true;
}


