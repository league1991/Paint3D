#include "StdAfx.h"
#include "HistoryCommand.h"
#include "Paint3DFrame.h"

ManipulateCommand::ManipulateCommand( 
									 unsigned objID, 
									 const ObjectTransform& oldTransform, 
									 const ObjectTransform& newTransform ,
									 ManipulateCommandType type)
{
	this->type = type;
	this->objID = objID;
	this->oldTransform = oldTransform;
	this->newTransform = newTransform;
	switch(type)
	{
	case MANCMD_TRANSLATE:
		this->setText(QObject::tr("Translate Object")); break;
	case MANCMD_ROTATE:
		this->setText(QObject::tr("Rotate Object")); break;
	case MANCMD_SCALE:
		this->setText(QObject::tr("Scale Object")); break;
	case MANCMD_ALL:
		this->setText(QObject::tr("Change Object Transform"));
	}
}

void ManipulateCommand::undo()
{
	RenderableObject* obj = Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (obj)
	{
		obj->setTransform(oldTransform);
		Paint3DFrame::getInstance()->scene->updateGeometryImage();
		Paint3DFrame::getInstance()->updateGLView();
	}
}

void ManipulateCommand::redo()
{
	RenderableObject* obj = Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (obj)
	{
		obj->setTransform(newTransform);
		Paint3DFrame::getInstance()->scene->updateGeometryImage();
		Paint3DFrame::getInstance()->updateGLView();
	}
}

void PaintCommand::undo()
{
	RenderableObject* obj = Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (obj && obj->getType() & RenderableObject::CMP_MESH)
	{
		Mesh* mesh = (Mesh*)obj;
		int width, height;
		Canvas& canvas = mesh->getCanvas();
		canvas.getResolution(width, height);
		canvas.selectLayer(curLayer);
		for (QVector<PixelRecord>::iterator pR = pixelRecords.begin(); pR != pixelRecords.end(); ++pR)
		{
			int y = pR->index / width;
			int x = pR->index - y * width;
			canvas.setCurLayerColorPixel(x, y, pR->oldBGRAi);
			canvas.setCurLayerSurfacePixel(x, y, pR->oldSurfi);
			canvas.setCurLayerThicknessPixel(x, y, pR->oldThickness);
		}
		canvas.updateGLTextures();
		Paint3DFrame::getInstance()->updateGLView();
	}
}

void PaintCommand::redo()
{
	RenderableObject* obj = Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (obj && obj->getType() == RenderableObject::CMP_MESH)
	{
		Mesh* mesh = (Mesh*)obj;
		int width, height;
		Canvas& canvas = mesh->getCanvas();
		canvas.getResolution(width, height);
		canvas.selectLayer(curLayer);
		for (QVector<PixelRecord>::iterator pR = pixelRecords.begin(); pR != pixelRecords.end(); ++pR)
		{
			int y = pR->index / width;
			int x = pR->index - y * width;
			canvas.setCurLayerColorPixel(x, y, pR->newBGRAi);
			canvas.setCurLayerSurfacePixel(x, y, pR->newSurfi);
			canvas.setCurLayerThicknessPixel(x, y, pR->newThickness);
		}
		canvas.updateGLTextures();
		Paint3DFrame::getInstance()->updateGLView();
	}
}

void PaintCommand::addRecord( const PixelRecord& record )
{
	pixelRecords.push_back(record);
}

PaintCommand::PaintCommand( unsigned objID, int curLayer )
{
	this->objID = objID;
	this->curLayer = curLayer;
	this->setText(QObject::tr("Paint Object"));
}

void AddLayerCommand::undo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		QSharedPointer<CanvasLayer> dummy;
		mesh->getCanvas().removeLayer(dummy, layerPos);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}

void AddLayerCommand::redo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().insertLayer(layer, layerPos);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}


AddLayerCommand::AddLayerCommand( QSharedPointer<CanvasLayer>layer, int layerPos, int objID )
{
	this->layer = layer;
	this->setText(QObject::tr("Add Layer"));
	this->layerPos = layerPos;
	this->objID = objID;
}

DeleteLayerCommand::DeleteLayerCommand( QSharedPointer<CanvasLayer>layer, int layerPos, int objID )
{
	this->layer = layer;
	this->setText(QObject::tr("Delete Layer"));
	this->layerPos = layerPos;
	this->objID = objID;
}

void DeleteLayerCommand::undo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().insertLayer(layer, layerPos);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}

void DeleteLayerCommand::redo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		QSharedPointer<CanvasLayer> dummy;
		mesh->getCanvas().removeLayer(dummy, layerPos);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}

void MoveUpLayerCommand::undo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().moveDownLayer(layerOriPos+1);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}

void MoveUpLayerCommand::redo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().moveUpLayer(layerOriPos);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}


void MoveDownLayerCommand::undo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().moveUpLayer(layerOriPos-1);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}

void MoveDownLayerCommand::redo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().moveDownLayer(layerOriPos);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}

void SetLayerFromImageCommand::undo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().setLayer(oldLayer, layerPos);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}

void SetLayerFromImageCommand::redo()
{
	Mesh* mesh = (Mesh*)Paint3DFrame::getInstance()->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().setLayer(newLayer, layerPos);
		Paint3DFrame::getInstance()->updateGLView();
		Paint3DFrame::getInstance()->layerEditor->updateList();
	}
}

void RemoveObjectCommand::undo()
{
	Paint3DFrame::getInstance()->scene->insertObject(object);
	Manipulator* curTool = Paint3DFrame::getInstance()->viewWidget->getCurTool();
	if (curTool)
	{
		curTool->captureObject(object);
	}
}

void RemoveObjectCommand::redo()
{
	QWeakPointer<RenderableObject>curSelectObj = Paint3DFrame::getInstance()->viewWidget->getSelectedObject();
	Manipulator*& curTool = Paint3DFrame::getInstance()->viewWidget->getCurTool();
	if (object == curSelectObj)
	{
		Paint3DFrame::getInstance()->viewWidget->clearSelectedObject();
	}
	if (curTool && curTool->getCurObject() == object)
	{
		curTool->captureObject(curSelectObj);
		Paint3DFrame::getInstance()->viewWidget->setTool(GLViewWidget::TOOL_SELECT);
	}
	Paint3DFrame::getInstance()->scene->removeObject(object);
}
