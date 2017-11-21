#include "StdAfx.h"
#include "HistoryWidget.h"
#include "Paint3DFrame.h"
HistoryWidget::HistoryWidget(void)
{
	setupUi(this);
	historyView->setStack(&Paint3DFrame::getInstance()->scene->getUndoStack());
	historyView->setEmptyLabel("<start>");
}

HistoryWidget::~HistoryWidget(void)
{
}
