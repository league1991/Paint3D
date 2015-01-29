#pragma once
#include <QWidget>
#include "ui_materialLayer.h"

QSharedPointer<Scene> getScene();
class MaterialLayerWidget: public QScrollArea, Ui::MaterialLayer
{
	Q_OBJECT
public:
	MaterialLayerWidget(void);
	~MaterialLayerWidget(void);
public slots:
	void updateList();
	void addLayer();
	void deleteLayer();
	void moveUpLayer();
	void moveDownLayer();
	void selectLayer(int ithLayer);
	void loadLayerFromImage();
	void saveLayerToImage();
	void saveCanvasToImage();
private:
	Mesh* getSelectedMesh();
};
