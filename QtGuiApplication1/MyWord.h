#pragma once

#include <QtWidgets/QWidget>
#include "ui_MyWord.h"

class MyWord : public QWidget
{
	Q_OBJECT

public:
	MyWord(QWidget *parent = Q_NULLPTR);

private:
	Ui::MyWordClass ui;
};
