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

#include <future>
#include <memory>


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(int argc, char *argv[], QWidget *parent = nullptr);

	~MainWindow();

private:
	int argc;
	char **argv;
	Ui::uiMainWindow *ui;
	QSettings *settings = new QSettings("MySoft", "Star Runner");
	QProcess *myProcess = new QProcess(this);
};


#endif //DONATIONMINING_MAINWINDOW_H
