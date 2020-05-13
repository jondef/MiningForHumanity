//
// Created by jon on 31.03.20.
//

#ifndef DONATIONMINING_MAINWINDOW_H
#define DONATIONMINING_MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtCharts>
#include "BurgerMenu.h"
#include "ui_MainWindow.h"
#include "MinerManager.h"
#include "LoginWidget.h"

#include <future>
#include <memory>

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(int argc, char *argv[], QWidget *parent = nullptr);

	~MainWindow();

private:
	bool closing = false;
	int argc;
	char **argv;
	Ui::uiMainWindow *ui;

	MinerManager *settingsWindow = new MinerManager();
	LoginWidget *loginWindow = new LoginWidget();
	QWidget *centralWidget = nullptr;

	void showDashboard();

protected:
	void closeEvent(QCloseEvent *event) override;

	void showEvent(QShowEvent *event) override;

	void showLoginScreen();

	void updateDashboard(const QString &username);
};


#endif //DONATIONMINING_MAINWINDOW_H
