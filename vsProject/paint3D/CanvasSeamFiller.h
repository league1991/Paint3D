#pragma once

// 这个结构记录从obj文件读取得到的数据
struct ObjTriangle
{
	unsigned int vertIndex[3];			//vertex index
	unsigned int texcoordIndex[3];		//texture coordinate index
	unsigned int norIndex[3];			//normal index
	unsigned int mtlIndex;				//material index
	friend QDataStream& operator<<(QDataStream& out, const ObjTriangle&tri);
	friend QDataStream& operator<<(QDataStream& in, ObjTriangle&tri);
};

// 这个结构代表三角形的一条边
struct EdgePointID
{
	EdgePointID(){v1ID = v2ID = -1;}
	int v1ID, v2ID;						// 两个顶点的索引
	bool operator==(const EdgePointID& e)const
	{
		return (v1ID == e.v1ID && v2ID == e.v2ID) ||
			   (v1ID == e.v2ID && v2ID == e.v1ID);
	}
};

inline uint qHash(const EdgePointID &key)
{
	return uint(key.v1ID ^ key.v2ID);
}

// 记录一条边所在位置的结构
struct EdgeTriID
{
	unsigned faceID, edgeID;
};
struct TexcoordVertex
{
	TexcoordVertex(){}
	TexcoordVertex(const QVector2D& texcoord)
	{
		this->vertex = texcoord;
	}
	QVector2D vertex;
	QVector2D normal;
	QVector<unsigned> enterEdgeID;			// 入口边的ID,一个顶点可能与多条边相连接
	QVector<unsigned> exitEdgeID;			// 出口边的ID
};
// 像素位置
struct PixelPos
{
	PixelPos(){}
	PixelPos(short x, short y)
	{
		this->x = x;
		this->y = y;
	}
	bool operator==(const PixelPos& pixel)const
	{return x == pixel.x && y == pixel.y;}
	short x, y;
};
inline uint qHash(const PixelPos& pixel)
{
	return (pixel.y << 16) | pixel.x; 
}
// 这个类负责四边形插值
class CoordMapper
{
public:
	CoordMapper(){}
	void setSrcShape(
		const QVector2D& srcO1,
        const QVector2D& srcO2,
		const QVector2D& srcV1,
		const QVector2D& srcV2, 
		const QVector2D& edgeV);

	void setDstShape(
		const QVector2D& dstO1,
		const QVector2D& dstO2,
		const QVector2D& dstV1,
		const QVector2D& dstV2, 
		const QVector2D& edgeV);

	void getSrcQuad(QVector2D& p1, QVector2D& p2, 
		            QVector2D& p3, QVector2D& p4)const;
	void getDstQuad(QVector2D& p1, QVector2D& p2, 
					QVector2D& p3, QVector2D& p4)const;
	// 进行四边形插值
	bool convert( const QVector2D& srcPoint, QVector2D& dstPoint )const;
private:
	// 两个四边形的形状
	QVector2D srcO, dstO,
		      srcU, dstU,
			  srcV1, dstV1,
			  srcV21, dstV21;
	float A,B,a,b1;			// 预计算的数据
};
struct TexcoordEdge
{
	TexcoordEdge(){commonEdgeID = -1;}
	TexcoordEdge(const unsigned startVtxID,
		         const unsigned endVtxID,
				 const unsigned faceID,
				 const unsigned edgeID,
				 const QVector2D& normal)
	{
		this->startVtxID = startVtxID;
		this->endVtxID = endVtxID;
		this->commonEdgeID = -1;
		this->normal = normal;
		this->faceID = faceID;
		this->edgeID = edgeID;
	}
	unsigned startVtxID, endVtxID;				// 表示边两端连接的顶点ID，定义边的方向为从startVtx到endVtx
	unsigned commonEdgeID;						// 表示在纹理空间中，与这条边对应的边的ID,没有则为-1，
												// 若另一条边的方向相反，
												// 即this->startVtxID == commonEdge.endVtxID,
												// 则把最高位设为1，否则设为0
	unsigned faceID;							// 三角形ID
	unsigned edgeID;							// 边在三角形的位置
	QVector2D normal;							// 边法线
	CoordMapper mapper;							// 把坐标转换到与之对应的四边形上
};
class CanvasSeamFiller
{
public:
	CanvasSeamFiller(void);
	~CanvasSeamFiller(void);
	void init(int width, int height,
		QVector<QVector3D>& vertices,
		QVector<QVector2D>& texcoords,
		QVector<ObjTriangle>& faces);

	QVector<PixelPos>& getExtraFillPixels(int x, int y);

private:
	// 把世界空间的公共边提取出来
	void extractCommonPositionEdge( 
		const QVector<ObjTriangle>& faces, 
		const QVector<QVector2D>texcoords, 
		QHash<EdgePointID, EdgePointID>& edgePointMap );
	// 把纹理空间的边界边提取出来
	void extractBorderTexcoordEdge( 
		const QVector<ObjTriangle>& faces, 
		const QVector<QVector2D>texcoords, 
		QMultiHash<EdgePointID, EdgeTriID>& edgeMap );
	// 建立纹理空间中的边界边和顶点数组
	void buildTexcoordEdge( 
		const QVector<QVector2D>& texcoords, 
		const QVector<ObjTriangle>& faces, 
		const QMultiHash<EdgePointID, EdgeTriID>& edgeMap, 
		const QHash<EdgePointID, EdgePointID>& edgeCommonPointMap, 
		QVector<TexcoordEdge>& edgeArray, 
		QVector<TexcoordVertex>& vertexArray, 
		QVector<int>& edgeSetArray );
	
	// 分开被两条以上边共享的顶点
	void splitCommonVertex( QVector<TexcoordEdge>& edgeArray, QVector<TexcoordVertex>& vertexArray);

	// 把边连起来，同时理顺startVtx 同 endVtx,并计算顶点法线
	void linkTexcoordEdge( QVector<TexcoordEdge>& edgeArray, QVector<TexcoordVertex>& vertexArray, QVector<int>& edgeSetArray);
	// 构建四边形插值类
	void buildCoordMapper( QVector<TexcoordEdge>& edgeArray, QVector<TexcoordVertex>& vertexArray);
	// 画出轮廓图形
	void drawOutlineImage(const QVector<TexcoordEdge>& texEdgeArray, const QVector<TexcoordVertex>& texVtxArray, const QVector<QVector2D>& texcoords, const QVector<ObjTriangle>& faces, QImage& triangleMaskImage);
	// 建立纹理查找索引
	void buildPixelLookupTable(const QImage& triangleMaskImage, const QVector<TexcoordEdge>& texEdgeArray);
	float fillWidth;
	int width, height;
	QVector<int> pixelMap;
	QVector<QVector<PixelPos>> fillIDMap;

};
