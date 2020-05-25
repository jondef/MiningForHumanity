//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_DASHBOARD_H
#define MININGFORHUMANITY_DASHBOARD_H

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include "ui_Dashboard.h"
#include "MinerManager.h"

class Dashboard : public QWidget {
Q_OBJECT

public:
	explicit Dashboard(QWidget *parent = nullptr);

	~Dashboard() override;

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void changePage(int pageIndex);

private:
	Ui::uiDashboard *ui;
	MinerManager *settingsWindow = new MinerManager();
};


#endif //MININGFORHUMANITY_DASHBOARD_H
