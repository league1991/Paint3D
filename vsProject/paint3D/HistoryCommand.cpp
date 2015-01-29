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
	RenderableObject* obj = paint3DApp->scene->getObject(objID).data();
	if (obj)
	{
		obj->setTransform(oldTransform);
		paint3DApp->scene->updateGeometryImage();
		paint3DApp->updateGLView();
	}
}

void ManipulateCommand::redo()
{
	RenderableObject* obj = paint3DApp->scene->getObject(objID).data();
	if (obj)
	{
		obj->setTransform(newTransform);
		paint3DApp->scene->updateGeometryImage();
		paint3DApp->updateGLView();
	}
}

void PaintCommand::undo()
{
	RenderableObject* obj = paint3DApp->scene->getObject(objID).data();
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
		paint3DApp->updateGLView();
	}
}

void PaintCommand::redo()
{
	RenderableObject* obj = paint3DApp->scene->getObject(objID).data();
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
		paint3DApp->updateGLView();
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
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		QSharedPointer<CanvasLayer> dummy;
		mesh->getCanvas().removeLayer(dummy, layerPos);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}

void AddLayerCommand::redo()
{
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().insertLayer(layer, layerPos);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
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
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().insertLayer(layer, layerPos);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}

void DeleteLayerCommand::redo()
{
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		QSharedPointer<CanvasLayer> dummy;
		mesh->getCanvas().removeLayer(dummy, layerPos);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}

void MoveUpLayerCommand::undo()
{
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().moveDownLayer(layerOriPos+1);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}

void MoveUpLayerCommand::redo()
{
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().moveUpLayer(layerOriPos);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}


void MoveDownLayerCommand::undo()
{
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().moveUpLayer(layerOriPos-1);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}

void MoveDownLayerCommand::redo()
{
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().moveDownLayer(layerOriPos);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}

void SetLayerFromImageCommand::undo()
{
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().setLayer(oldLayer, layerPos);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}

void SetLayerFromImageCommand::redo()
{
	Mesh* mesh = (Mesh*)paint3DApp->scene->getObject(objID).data();
	if (mesh)
	{
		mesh->getCanvas().setLayer(newLayer, layerPos);
		paint3DApp->updateGLView();
		paint3DApp->layerEditor->updateList();
	}
}

void RemoveObjectCommand::undo()
{
	paint3DApp->scene->insertObject(object);
	Manipulator* curTool = paint3DApp->viewWidget->getCurTool();
	if (curTool)
	{
		curTool->captureObject(object);
	}
}

void RemoveObjectCommand::redo()
{
	QWeakPointer<RenderableObject>curSelectObj = paint3DApp->viewWidget->getSelectedObject();
	Manipulator*& curTool = paint3DApp->viewWidget->getCurTool();
	if (object == curSelectObj)
	{
		paint3DApp->viewWidget->clearSelectedObject();
	}
	if (curTool && curTool->getCurObject() == object)
	{
		curTool->captureObject(curSelectObj);
		paint3DApp->viewWidget->setTool(GLViewWidget::TOOL_SELECT);
	}
	paint3DApp->scene->removeObject(object);
}
