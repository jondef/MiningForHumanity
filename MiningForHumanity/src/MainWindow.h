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

	~MainWindow() override;

protected:
	void closeEvent(QCloseEvent *event) override;

	void showEvent(QShowEvent *event) override;

	void paintEvent(QPaintEvent *event) override;

public slots:

private slots:
	void showLoginScreen();

	void updateDashboard(const QString &username);

	void showDashboard(const QString &username);

	void setPixmap(const QPixmap &pixmap) { backgroundPixmap = pixmap; }

public:

private:
	Ui::uiMainWindow *ui;
	int argc;
	char **argv;
	bool closing = false;

	LoginWidget *loginWindow = new LoginWidget();
	QWidget *centralWidget = nullptr;
	QPixmap backgroundPixmap;
};


#endif //DONATIONMINING_MAINWINDOW_H
