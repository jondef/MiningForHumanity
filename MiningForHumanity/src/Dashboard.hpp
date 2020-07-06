//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_DASHBOARD_HPP
#define MININGFORHUMANITY_DASHBOARD_HPP

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include "ui_Dashboard.h"
#include "MinerManager.hpp"
#include "Campaign.hpp"
#include "globals.hpp"

class Dashboard : public QWidget {
Q_OBJECT

public:
	explicit Dashboard(QWidget *parent = nullptr);

	~Dashboard() override;

protected:
	void paintEvent(QPaintEvent *event) override;

Q_SIGNALS:
	void changePage(GLOBALS::Page);

	void ribbonSetText(const QString &text);

	void ribbonSetCampaign(const QString &title, const QPixmap &pixmap, const QList<CategoryLabel::Category> &categories);

public slots:
	void showCampaignInfo(Campaign *campaign);

	void changePage(int pageIndex);

private:
	Ui::uiDashboard *ui;
	MinerManager *settingsWindow = new MinerManager();
};


#endif //MININGFORHUMANITY_DASHBOARD_HPP
