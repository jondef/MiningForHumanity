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
#include "SettingsWindow.h"
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
	QProcess *myProcess = new QProcess(this);

	SettingsWindow *settingsWindow = new SettingsWindow();
	LoginWidget *loginWindow = new LoginWidget();
	QWidget *centralWidget;

	void showDashboard(const QString &username, const QString &password);

protected:
	void closeEvent(QCloseEvent *event) override;

	void showEvent(QShowEvent *event) override;

	void showLoginScreen();
};


#endif //DONATIONMINING_MAINWINDOW_H
