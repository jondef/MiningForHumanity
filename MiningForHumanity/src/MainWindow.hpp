//
// Created by jon on 31.03.20.
//

#ifndef DONATIONMINING_MAINWINDOW_H
#define DONATIONMINING_MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtCharts>
#include "BurgerMenu.hpp"
#include "ui_MainWindow.h"
#include "LoginWidget.hpp"

#include <future>
#include <memory>

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	enum Page {
		Dashboard,
		OngoingCampaigns,
		FinishedCampaigns,
		Settings
	};

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

protected:
	void closeEvent(QCloseEvent *event) override;

	void showEvent(QShowEvent *event) override;

	void paintEvent(QPaintEvent *event) override;

public slots:

private slots:

	void showLoginScreen();

	void showDashboard();

	void setBackgroundPixmap(const QPixmap &pixmap) { backgroundPixmap = pixmap; }

public:

private:
	Ui::uiMainWindow *ui;
	bool closing = false;

	LoginWidget *loginWindow = new LoginWidget();
	QWidget *centralWidget = nullptr;
	QPixmap backgroundPixmap;
};


#endif //DONATIONMINING_MAINWINDOW_H
