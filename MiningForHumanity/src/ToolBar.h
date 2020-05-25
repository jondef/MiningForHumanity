//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_TOOLBAR_H
#define MININGFORHUMANITY_TOOLBAR_H

#include <QWidget>
#include <QMenu>
#include <QPainter>
#include <QStyleOption>
#include "ui_ToolBar.h"
#include "LanguageButtons.h"

class ToolBar : public QWidget {
Q_OBJECT
public:
	explicit ToolBar(QWidget *parent = nullptr);

	~ToolBar() override;

protected:
	void paintEvent(QPaintEvent *event) override;

Q_SIGNALS:
	void changePage(int pageIndex);

private:
	Ui::uiToolBar *ui;
};


#endif //MININGFORHUMANITY_TOOLBAR_H
