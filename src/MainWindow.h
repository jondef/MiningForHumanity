//
// Created by jon on 31.03.20.
//

#ifndef DONATIONMINING_MAINWINDOW_H
#define DONATIONMINING_MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QObject>
#include <QSettings>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow();

private:
	Ui::uiMainWindow *ui;
	QSettings *settings = new QSettings("MySoft", "Star Runner");
};


#endif //DONATIONMINING_MAINWINDOW_H
