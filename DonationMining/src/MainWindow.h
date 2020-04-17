//
// Created by jon on 31.03.20.
//

#ifndef DONATIONMINING_MAINWINDOW_H
#define DONATIONMINING_MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QObject>
#include <QString>
#include <QSettings>
#include "ui_MainWindow.h"
#include "SettingsWindow.h"
#include "LoginWidget.h"

#include <future>
#include <memory>


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(int argc, char *argv[], QWidget *parent = nullptr);
	~MainWindow();

private:
	bool closing;
	int argc;
	char **argv;
	Ui::uiMainWindow *ui;
	QProcess *myProcess = new QProcess(this);

	SettingsWindow *settingsWindow = new SettingsWindow(this);
	LoginWidget *loginWindow = new LoginWidget();
	QWidget *centralWidget;

	void showDashboard(const QString &username, const QString &password);

protected:
	void closeEvent(QCloseEvent *event) override;

	void showEvent(QShowEvent *event) override;

};


#endif //DONATIONMINING_MAINWINDOW_H
