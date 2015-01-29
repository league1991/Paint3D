#include "StdAfx.h"
#include "MaterialLayerWidget.h"
#include "Paint3DFrame.h"
extern Paint3DFrame* paint3DApp;
MaterialLayerWidget::MaterialLayerWidget(void)
{
	setupUi(this);
	connect(addLayerButton, SIGNAL(clicked()), this, SLOT(addLayer()));
	connect(deleteLayerButton, SIGNAL(clicked()), this, SLOT(deleteLayer()));
	connect(moveUpButton, SIGNAL(clicked()), this, SLOT(moveUpLayer()));
	connect(moveDownButton, SIGNAL(clicked()), this, SLOT(moveDownLayer()));

	connect(layerList, SIGNAL(currentRowChanged(int)), this, SLOT(selectLayer(int)));
	connect(importChannelButton,SIGNAL(clicked()), this, SLOT(loadLayerFromImage()));
	connect(exportImageButton,SIGNAL(clicked()), this, SLOT(saveLayerToImage()));
	connect(exportFinalImageButton, SIGNAL(clicked()), this, SLOT(saveCanvasToImage()));
}

MaterialLayerWidget::~MaterialLayerWidget(void)
{
}

void MaterialLayerWidget::updateList()
{
	RenderableObject* obj = paint3DApp->viewWidget->getSelectedObject().data();
	layerList->clear();
	if (obj && obj->getType() & RenderableObject::OBJ_MESH)
	{
		Mesh* mesh = (Mesh*)obj;
		QStringList layerNames;
		Canvas& canvas = mesh->getCanvas();
		canvas.getLayerNames(layerNames);
		layerList->addItems(layerNames);
		layerList->setCurrentRow(canvas.getNumLayers() - canvas.getCurLayerID()- 1);
	}
}

void MaterialLayerWidget::addLayer()
{
	Mesh* mesh = getSelectedMesh();
	if (mesh)
	{
		Canvas& canvas = mesh->getCanvas();
		int curLayerID = canvas.getCurLayerID();
		QSharedPointer<CanvasLayer> layer = canvas.generateLayer();
		AddLayerCommand* cmd = new AddLayerCommand(layer, curLayerID+1, mesh->getObjectID());
		paint3DApp->scene->getUndoStack().push(cmd);
	}
	updateList();
}



Mesh* MaterialLayerWidget::getSelectedMesh()
{
	RenderableObject* obj = paint3DApp->viewWidget->getSelectedObject().data();
	if (obj && obj->getType() & RenderableObject::OBJ_MESH)
	{
		return (Mesh*)obj;
	}
	return NULL;
}

void MaterialLayerWidget::deleteLayer()
{
	Mesh* mesh = getSelectedMesh();
	if (mesh)
	{
		Canvas& canvas = mesh->getCanvas();
		int curLayerID = canvas.getCurLayerID();
		QSharedPointer<CanvasLayer> layer = canvas.getCurLayer();
		DeleteLayerCommand* cmd = new DeleteLayerCommand(layer, curLayerID, mesh->getObjectID());
		paint3DApp->scene->getUndoStack().push(cmd);
	}
	updateList();
	paint3DApp->viewWidget->update();
}

void MaterialLayerWidget::selectLayer( int ithLayer )
{
	//qDebug() << "select layer" << ithLayer << endl;
	if (ithLayer == -1)
	{
		return;
	}
	Mesh* mesh = getSelectedMesh();
	if (mesh)
	{
		Canvas& canvas = mesh->getCanvas();
		int layerPos = canvas.getNumLayers() - 1 - ithLayer;
		if (layerPos != canvas.getCurLayerID())
		{
			canvas.selectLayer(layerPos);
			updateList();
		}
	}
}

void MaterialLayerWidget::moveUpLayer()
{
	Mesh* mesh = getSelectedMesh();
	if (mesh)
	{
		Canvas& canvas = mesh->getCanvas();
		int curLayerID = canvas.getCurLayerID();
		if (curLayerID < canvas.getNumLayers()-1)
		{
			MoveUpLayerCommand* cmd = new MoveUpLayerCommand(curLayerID, mesh->getObjectID());
			paint3DApp->scene->getUndoStack().push(cmd);
			updateList();
			paint3DApp->viewWidget->update();
		}
	}
}

void MaterialLayerWidget::moveDownLayer()
{
	Mesh* mesh = getSelectedMesh();
	if (mesh)
	{
		Canvas& canvas = mesh->getCanvas();
		int curLayerID = canvas.getCurLayerID();
		if (curLayerID > 0)
		{
			MoveDownLayerCommand* cmd = new MoveDownLayerCommand(curLayerID, mesh->getObjectID());
			paint3DApp->scene->getUndoStack().push(cmd);
			updateList();
			paint3DApp->viewWidget->update();
		}
	}
}

void MaterialLayerWidget::loadLayerFromImage()
{
	Mesh* mesh = getSelectedMesh();
	if (!mesh)
		return;


	QString filename = QFileDialog::getOpenFileName(
		this,
		"Select one image to import",
		QDir::currentPath(),
		"Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff)");

	if (!filename.isNull()) 
	{ 	//用户选择了文件
		QImage img(filename);
		Canvas& canvas = mesh->getCanvas();
		int curLayerID = canvas.getCurLayerID();
		QSharedPointer<CanvasLayer> newLayer =canvas.generateLayer();
		*newLayer = *canvas.getCurLayer();
		CanvasLayer::LayerChannel channel;
		if (colorBox->isChecked())
		{
			channel = CanvasLayer::CHNL_COLOR;
		}
		else if (reflectionBox->isChecked())
		{
			channel = CanvasLayer::CHNL_REFLECTION;
		}
		else if (glossnessBox->isChecked())
		{
			channel = CanvasLayer::CHNL_GLOSSNESS;
		}
		else if (refractionBox->isChecked())
		{
			channel = CanvasLayer::CHNL_REFRACTION;
		}
		else if (iorBox->isChecked())
		{
			channel = CanvasLayer::CHNL_IOR;
		}
		else
		{
			channel = CanvasLayer::CHNL_THICKNESS;
		}
		newLayer->setLayerFromImage(img, channel);
		SetLayerFromImageCommand* cmd = 
			new SetLayerFromImageCommand(newLayer, canvas.getCurLayer(), canvas.getCurLayerID(),mesh->getObjectID());
		paint3DApp->scene->getUndoStack().push(cmd);
		updateList();
		paint3DApp->viewWidget->update();

	}
}

void MaterialLayerWidget::saveLayerToImage()
{
	Mesh* mesh = getSelectedMesh();
	if (!mesh)
		return;


	QString filename = QFileDialog::getSaveFileName(this, tr("Save channel to image"),
		QDir::currentPath(),
		"Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff)");

	if (!filename.isNull()) 
	{ 	//用户选择了文件
		Canvas& canvas = mesh->getCanvas();
		CanvasLayer::LayerChannel channel;
		if (colorBox->isChecked())
		{
			channel = CanvasLayer::CHNL_COLOR;
		}
		else if (reflectionBox->isChecked())
		{
			channel = CanvasLayer::CHNL_REFLECTION;
		}
		else if (glossnessBox->isChecked())
		{
			channel = CanvasLayer::CHNL_GLOSSNESS;
		}
		else if (refractionBox->isChecked())
		{
			channel = CanvasLayer::CHNL_REFRACTION;
		}
		else if (iorBox->isChecked())
		{
			channel = CanvasLayer::CHNL_IOR;
		}
		else
		{
			channel = CanvasLayer::CHNL_THICKNESS;
		}
		canvas.getCurLayer()->saveLayer(filename, channel);
	}
}

void MaterialLayerWidget::saveCanvasToImage()
{
	Mesh* mesh = getSelectedMesh();
	if (!mesh)
		return;


	QString filename = QFileDialog::getSaveFileName(this, tr("Save channel to image"),
		QDir::currentPath(),
		"Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff)");

	if (!filename.isNull()) 
	{ 	//用户选择了文件
		Canvas& canvas = mesh->getCanvas();
		CanvasLayer::LayerChannel channel;
		if (colorBox->isChecked())
		{
			channel = CanvasLayer::CHNL_COLOR;
		}
		else if (reflectionBox->isChecked())
		{
			channel = CanvasLayer::CHNL_REFLECTION;
		}
		else if (glossnessBox->isChecked())
		{
			channel = CanvasLayer::CHNL_GLOSSNESS;
		}
		else if (refractionBox->isChecked())
		{
			channel = CanvasLayer::CHNL_REFRACTION;
		}
		else if (iorBox->isChecked())
		{
			channel = CanvasLayer::CHNL_IOR;
		}
		else
		{
			channel = CanvasLayer::CHNL_THICKNESS;
		}
		canvas.saveCanvas(filename, channel);
	}
}
