#include "StdAfx.h"
#include "CanvasSeamFiller.h"

CanvasSeamFiller::CanvasSeamFiller(void)
{
	fillWidth = 0.02;
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
	QImage triangleMaskImage;
	drawOutlineImage(texEdgeArray, texVtxArray, texcoords, faces, triangleMaskImage);

	// 建立像素查找索引
	buildPixelLookupTable(triangleMaskImage, texEdgeArray);
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
		const EdgePointID& curVtxID = pCurEdge.key();
		const EdgePointID& nextVtxID = pNextEdge.key();
		if(curVtxID == nextVtxID)
		{
			const EdgeTriID& curEdgeTriID = pCurEdge.value();
			const EdgeTriID& nextEdgeTriID = pNextEdge.value();

			// 找出这两条公共边的贴图坐标顶点序号
			EdgePointID curPointID, nextPointID;
			curPointID.v1ID = faces[curEdgeTriID.faceID].texcoordIndex[curEdgeTriID.edgeID];
			curPointID.v2ID = faces[curEdgeTriID.faceID].texcoordIndex[(curEdgeTriID.edgeID+1)%3];

			// 确保curPointID的v1ID 与 nextPointID的v1ID是同一个顶点
			if (curVtxID.v1ID == nextVtxID.v1ID)
			{
				nextPointID.v1ID = faces[nextEdgeTriID.faceID].texcoordIndex[nextEdgeTriID.edgeID];
				nextPointID.v2ID = faces[nextEdgeTriID.faceID].texcoordIndex[(nextEdgeTriID.edgeID+1)%3];
			}
			else
			{
				nextPointID.v2ID = faces[nextEdgeTriID.faceID].texcoordIndex[nextEdgeTriID.edgeID];
				nextPointID.v1ID = faces[nextEdgeTriID.faceID].texcoordIndex[(nextEdgeTriID.edgeID+1)%3];
			}

			// 如果顶点不一致，说明这两条边在纹理空间不相邻
			if (!(curPointID == nextPointID))
			{
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
/************************************************************************/
/* buildTexcoordEdge    构建边表和顶点表                                                      
/* 输入：
/* edgeMap				存放边界边的在贴图坐标数组的索引到其在三角形数组的索引的映射
                        这个数组中边的起止点按照逆时针方向排序
/* edgeCommonPointMap	存放存在对应边的边界边，一条边的在贴图坐标数组的索引
						到其对应边在贴图坐标数组的索引的映射
/************************************************************************/
void CanvasSeamFiller::buildTexcoordEdge(
	const QVector<QVector2D>& texcoords,
	const QVector<ObjTriangle>& faces,
	const QMultiHash<EdgePointID, EdgeTriID>& edgeMap,
	const QHash<EdgePointID, EdgePointID>& edgeCommonPointMap,
	QVector<TexcoordEdge>& edgeArray, 
	QVector<TexcoordVertex>& vertexArray, 
	QVector<int>& edgeSetArray )
{
	QVector<int> texcoord2vtxMap(texcoords.size(), -1);// 记录从texcoords数组下标到vertexArray下标的映射

	QHash<EdgePointID, int> texcoord2edgeMap;		 // 记录从EdgePointID到该边在edgeArray中的下标的映射
	// 首先构建边表和顶点表
	for (QMultiHash<EdgePointID, EdgeTriID>::const_iterator pEdge = edgeMap.begin();
		 pEdge != edgeMap.end(); ++pEdge)
	{
		const EdgePointID& edgeKey = pEdge.key();

		// 如果边的任意一端顶点未添加入输出的顶点数组，
		// 先把顶点添加进去
		if (texcoord2vtxMap[edgeKey.v1ID] == -1)
		{
			texcoord2vtxMap[edgeKey.v1ID] = vertexArray.size();
			vertexArray.push_back(TexcoordVertex(texcoords[edgeKey.v1ID]));
		}
		if (texcoord2vtxMap[edgeKey.v2ID] == -1)
		{
			texcoord2vtxMap[edgeKey.v2ID] = vertexArray.size();
			vertexArray.push_back(TexcoordVertex(texcoords[edgeKey.v2ID]));
		}

		// 设置顶点的边序号
		TexcoordVertex& v1 = vertexArray[texcoord2vtxMap[edgeKey.v1ID]];
		TexcoordVertex& v2 = vertexArray[texcoord2vtxMap[edgeKey.v2ID]];

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
		normal *= fillWidth / normal.length();
		texcoord2edgeMap.insert(edgeKey, edgeArray.size());						// 记录当前边在边数组的位置
		edgeArray.push_back(TexcoordEdge(
			edgeKey.v1ID,
			edgeKey.v2ID, 
			edgeTri.faceID,
			edgeTri.edgeID,
			normal));
	}
	// 处理存在公共边的边
	int ithEdge = 0;
	for (QVector<TexcoordEdge>::iterator pEdge = edgeArray.begin();
		 pEdge != edgeArray.end(); ++pEdge, ++ithEdge)
	{
		EdgePointID edgeID;
		edgeID.v1ID = pEdge->startVtxID;								// 注意这两个ID值为在贴图坐标数组的ID
		edgeID.v2ID = pEdge->endVtxID;
		// 检查当前边是否有公共边
		QHash<EdgePointID, EdgePointID>::const_iterator pComEdge = edgeCommonPointMap.find(edgeID);
		if (pComEdge != edgeCommonPointMap.end())
		{
			const EdgePointID& commonEdgeID = pComEdge.value();
			const EdgePointID& thisEdgeID   = pComEdge.key();
			int commonEdgeOffset = texcoord2edgeMap[commonEdgeID];
			TexcoordEdge& commonEdge = edgeArray[commonEdgeOffset];

			int directionFlag = (thisEdgeID.v1ID == edgeID.v1ID && commonEdgeID.v1ID == commonEdge.startVtxID) ||
				                (thisEdgeID.v1ID == edgeID.v2ID && commonEdgeID.v1ID == commonEdge.endVtxID);

			pEdge->commonEdgeID = (commonEdgeOffset & 0x7fffffff) | (~directionFlag << 31);
			edgeArray[commonEdgeOffset].commonEdgeID = (ithEdge & 0x7fffffff) | (~directionFlag << 31);
		}
	}
	// 替换边在贴图坐标数组的偏移值为在新建的顶点数组的偏移值
	for (QVector<TexcoordEdge>::iterator pEdge = edgeArray.begin();
		pEdge != edgeArray.end(); ++pEdge)
	{
		pEdge->startVtxID = texcoord2vtxMap[pEdge->startVtxID];
		pEdge->endVtxID = texcoord2vtxMap[pEdge->endVtxID];
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
						TexcoordVertex newVtx;
						newVtx.vertex = vtx.vertex;
						newVtx.normal = vtx.normal;
						vertexArray.push_back(newVtx);
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
			float factor = fillWidth * min(sqrt(2.0f / (QVector2D::dotProduct(normal1, normal2)/(fillWidth*fillWidth) + 1)), 3.0f);
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
		edge.mapper.setSrcShape(srcO1, srcO2, srcV1, srcV2,edge.normal);

		if (edge.commonEdgeID == -1)
		{
			edge.mapper.setDstShape(srcO1, srcO2, -srcV1, -srcV2, -edge.normal);
		}
		else
		{
			int dstEdgeID = edge.commonEdgeID & 0x7fffffff;
			int direction = (edge.commonEdgeID >> 31) & 0x1;
			TexcoordEdge& dstEdge = edgeArray[dstEdgeID];
			QVector2D& dstO1 = vertexArray[dstEdge.startVtxID].vertex;
			QVector2D& dstO2 = vertexArray[dstEdge.endVtxID].vertex;
			QVector2D& dstV1 = vertexArray[dstEdge.startVtxID].normal;
			QVector2D& dstV2 = vertexArray[dstEdge.endVtxID].normal;
			if (direction)
				edge.mapper.setDstShape(dstO2, dstO1, -dstV2, -dstV1, -dstEdge.normal);
			else
				edge.mapper.setDstShape(dstO1, dstO2, -dstV1, -dstV2, -dstEdge.normal);
		}
	}
}

void CanvasSeamFiller::drawOutlineImage(const QVector<TexcoordEdge>& texEdgeArray,
										const QVector<TexcoordVertex>& texVtxArray,
										const QVector<QVector2D>& texcoords,
										const QVector<ObjTriangle>& faces,
									   QImage& triangleMaskImage)
{
	triangleMaskImage = QImage(width, height,QImage::Format_ARGB32);
	triangleMaskImage.fill(0x0);
	QPainter painter(&triangleMaskImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	for (unsigned ithEdge = 0; ithEdge < texEdgeArray.size(); ++ithEdge)
	{
		QRgb edgeColor = ithEdge;
		painter.setPen(QPen(QColor(edgeColor)));
		painter.setBrush(QBrush(QColor(edgeColor)));
		const TexcoordEdge& edge = texEdgeArray[ithEdge];
		QVector2D v1,v2,v3,v4;
		edge.mapper.getSrcQuad(v1,v2,v3,v4);
		QPoint points[4] = 
		{
			QPoint(v1.x() * width, v1.y() * height),
			QPoint(v2.x() * width, v2.y() * height),
			QPoint(v3.x() * width, v3.y() * height),
			QPoint(v4.x() * width, v4.y() * height)
		};
		painter.drawPolygon(points, 4);
	}
	painter.setPen(QPen(QColor(0,0,0,0)));
	painter.setBrush(QBrush(QColor(0,0,0,0)));
	for (int ithTri = 0; ithTri < faces.size(); ++ithTri)
	{
		const QVector2D& t1 = texcoords[faces[ithTri].texcoordIndex[0]];
		const QVector2D& t2 = texcoords[faces[ithTri].texcoordIndex[1]];
		const QVector2D& t3 = texcoords[faces[ithTri].texcoordIndex[2]];

		QPoint points[3] = 
		{
			QPoint(t1.x() * width, (t1.y()) * height),
			QPoint(t2.x() * width, (t2.y()) * height),
			QPoint(t3.x() * width, (t3.y()) * height)
		};
		painter.drawPolygon(points, 3);
	}
	//triangleMaskImage.save("triangleMask.png");
}
QVector<PixelPos>& CanvasSeamFiller::getExtraFillPixels( int x, int y )
{
	return fillIDMap[pixelMap[y * width + x]];
}

void CanvasSeamFiller::buildPixelLookupTable(const QImage& triangleMaskImage,
											 const QVector<TexcoordEdge>& texEdgeArray)
{
	QImage backup(width, height,QImage::Format_ARGB32);
	fillIDMap.push_back(QVector<PixelPos>());				// 加入一个空向量
	pixelMap.insert(0,width * height, 0);	// 默认所有像素都指向这个空向量
	const unsigned* pPixel = (unsigned*)triangleMaskImage.constBits();
	const float stepSize = 0.5f;
	for (float y = 0; y < height; y += stepSize)
	{
		for (float x = 0; x < width; x += stepSize)
		{
			// 像素在边界附近，且被填充
			unsigned pixelVal = pPixel[(int)y * width + (int)x];
			unsigned edgeID = pixelVal & 0xffffff;
			union{float colorF; unsigned colorI;} color;
			color.colorF = exp(float(edgeID)*0.001);
			if (pixelVal != 0)
			{
				QVector2D uvSrcCoord(float(x) / (width),
					float(y) / (height)), uvDstCoord;
				const TexcoordEdge& edge = texEdgeArray[edgeID];

				// 找出颜色填充到这个像素的原像素位置
				if (edge.mapper.convert(uvSrcCoord, uvDstCoord))
				{
					int dstX = uvDstCoord.x() * (width);
					int dstY = uvDstCoord.y() * (height);
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
						backup.setPixel(dstX,dstY,color.colorI);
					}
				}		
				backup.setPixel(x,y,color.colorI);		
			}
		}
	}
	//backup.save("backup.png");
}
void CoordMapper::setSrcShape( const QVector2D& srcO1, const QVector2D& srcO2, const QVector2D& srcV1, const QVector2D& srcV2, const QVector2D& edgeV)
{
	this->srcO = srcO1;
	this->srcU = srcO2 - srcO1;
	this->srcV1 = QVector2D::dotProduct(srcV1, srcU) < 0 ? edgeV : srcV1;
	this->srcV21 = (QVector2D::dotProduct(srcV2, -srcU) < 0 ? edgeV : srcV2) - this->srcV1;
	A = srcU.x() * srcV21.y() - srcU.y() * srcV21.x();
	B = srcV1.x() * srcV21.y() - srcV1.y() * srcV21.x();
	a = srcU.y() * srcV21.x() - srcU.x() * srcV21.y();
	b1 = srcU.y() * srcV1.x() - srcU.x() * srcV1.y();
}

void CoordMapper::setDstShape( const QVector2D& dstO1, const QVector2D& dstO2, const QVector2D& dstV1, const QVector2D& dstV2, const QVector2D& edgeV )
{
	this->dstO = dstO1;
	this->dstU = dstO2 - dstO1;
	this->dstV1 =   QVector2D::dotProduct(dstV1, dstU) > 0 ?  edgeV : dstV1;
	this->dstV21 = (QVector2D::dotProduct(dstV2, -dstU) > 0 ? edgeV : dstV2) - this->dstV1;
}

bool CoordMapper::convert( const QVector2D& srcPoint, QVector2D& dstPoint )const
{
	QVector2D srcD = srcPoint - srcO;
	float C = srcD.x() * srcV21.y() - srcD.y() * srcV21.x();
	float b = b1 + C;
	float c = srcV1.y() * srcD.x() - srcV1.x() * srcD.y();
	float delta = b * b - 4 * a * c;
	if (delta < 0)
		return false;
	float u,v;
	if (abs(A) < 1e-4)		// 特殊情形，原四边形是个梯形
	{
		if (abs(B) < 1e-4)  // 是个平行四边形
		{
			u = c / -b1;
			v = (srcU.y() * srcD.x() - srcU.x() * srcD.y()) / b1;
		}
		else
		{
			u = -c / b;
			v = (C - A * u) / B;
		}
	}
	else
	{
		float det = sqrt(delta);
		float u1 = (-b + det) / (2 * a);
		float u2 = (-b - det) / (2 * a);
		float v1 = (C - A * u1) / B;
		float v2 = (C - A * u2) / B;
		if (u1 > 0 && u1 < 1 && v1 > 0 && v1 < 1)
		{	u = u1;v = v1;}
		else
		{	u = u2;v = v2;}
	}

	dstPoint = dstO + u * dstU + u * v * dstV21 + v * dstV1;
	return true;
}

void CoordMapper::getSrcQuad( QVector2D& p1, QVector2D& p2, QVector2D& p3, QVector2D& p4 )const
{
	p1 = srcO;
	p2 = srcO + srcU;
	p4 = srcO + srcV1;
	p3 = p4 + srcU + srcV21;
}

void CoordMapper::getDstQuad( QVector2D& p1, QVector2D& p2, QVector2D& p3, QVector2D& p4 )const
{
	p1 = dstO;
	p2 = dstO + dstU;
	p4 = dstO + dstV1;
	p3 = p4 + dstU + dstV21;
}


