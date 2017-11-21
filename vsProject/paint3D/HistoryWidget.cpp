#include "StdAfx.h"
#include "HistoryWidget.h"
#include "Paint3DFrame.h"
HistoryWidget::HistoryWidget(void)
{
	setupUi(this);
	historyView->setEmptyLabel("<start>");
}

HistoryWidget::~HistoryWidget(void)
{
}
