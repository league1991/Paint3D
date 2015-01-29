#pragma once

class ManipulateCommand : public QUndoCommand
{
public:
	enum ManipulateCommandType{
		MANCMD_TRANSLATE = 0,
		MANCMD_ROTATE,
		MANCMD_SCALE,
		MANCMD_ALL
	};
	ManipulateCommand(unsigned objID,
		const ObjectTransform& oldTransform,
		const ObjectTransform& newTransform,
		ManipulateCommandType type);

	void undo();
	void redo();
protected:
private:
	ManipulateCommandType type;
	unsigned objID;
	ObjectTransform oldTransform, newTransform;
};

class PaintCommand: public QUndoCommand
{
public:
	PaintCommand(unsigned objID, int curLayer);
	struct PixelRecord
	{
		unsigned index;						// 像素下标
		union{char oldBGRAc[4]; unsigned oldBGRAi;};
		union{char oldSurfc[4]; unsigned oldSurfi;};
		union{char newBGRAc[4]; unsigned newBGRAi;};
		union{char newSurfc[4]; unsigned newSurfi;};
		float oldThickness, newThickness;	// 本来的厚度和更新后的厚度
	};
	void addRecord(const PixelRecord& record);
	void undo();
	void redo();
private:
	QVector<PixelRecord> pixelRecords;
	int curLayer;	// 当前层的序号
	unsigned objID;	// 当前物体
};

class AddLayerCommand:public QUndoCommand
{
public:
	AddLayerCommand(QSharedPointer<CanvasLayer>layer, int layerPos, int objID);
	void undo();
	void redo();
private:
	QSharedPointer<CanvasLayer> layer;
	int layerPos;
	int objID;
};

class DeleteLayerCommand:public QUndoCommand
{
public:
	DeleteLayerCommand(QSharedPointer<CanvasLayer>layer, int layerPos, int objID);
	void undo();
	void redo();
private:
	QSharedPointer<CanvasLayer> layer;
	int layerPos;
	int objID;
};

class MoveUpLayerCommand:public QUndoCommand
{
public:
	MoveUpLayerCommand(int layerPos, int objID)
	{
		this->layerOriPos = layerPos;
		this->objID = objID;
		this->setText(QObject::tr("Move Up Layer"));
	}
	void undo();
	void redo();
private:
	int layerOriPos;
	int objID;
};

class MoveDownLayerCommand:public QUndoCommand
{
public:
	MoveDownLayerCommand(int layerPos, int objID)
	{
		this->layerOriPos = layerPos;
		this->objID = objID;
		this->setText(QObject::tr("Move Down Layer"));
	}
	void undo();
	void redo();
private:
	int layerOriPos;
	int objID;
};

class SetLayerFromImageCommand:public QUndoCommand
{
public:
	SetLayerFromImageCommand(QSharedPointer<CanvasLayer>newLayer,
		                     QSharedPointer<CanvasLayer>oldLayer,
							 int layerPos,int objID)
	{
		this->newLayer = newLayer;
		this->oldLayer = oldLayer;
		this->layerPos = layerPos;
		this->objID = objID;
		this->setText(QObject::tr("Set Layer From Image"));
	}
	void undo();
	void redo();
private:
	QSharedPointer<CanvasLayer> newLayer, oldLayer;
	int layerPos;
	int objID;
};

class RemoveObjectCommand:public QUndoCommand
{
public:
	RemoveObjectCommand(QSharedPointer<RenderableObject>obj)
	{
		object = obj;
		this->setText(QObject::tr("Remove Object"));
	}
	void undo();
	void redo();
private:
	QSharedPointer<RenderableObject> object;
};