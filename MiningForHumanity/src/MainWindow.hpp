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
#include "globals.hpp"

#include <future>
#include <memory>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

protected:
	void closeEvent(QCloseEvent *event) override;

	void showEvent(QShowEvent *event) override;

	void paintEvent(QPaintEvent *event) override;

public slots:

private slots:
	void changePage(GLOBALS::Page page);

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
