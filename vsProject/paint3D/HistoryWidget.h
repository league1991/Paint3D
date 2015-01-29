#pragma once
#include "ui_history.h"

class HistoryWidget: public QWidget, Ui::History
{
	Q_OBJECT
public:
	HistoryWidget(void);
	~HistoryWidget(void);
};
