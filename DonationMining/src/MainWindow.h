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

#include <future>
#include <memory>


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(int argc, char *argv[], QWidget *parent = nullptr);

	enum SaveFormat {
		Json, Binary
	};
	~MainWindow();

	void read(const QJsonObject &json);
	void write(QJsonObject &json) const;

private:
	bool closing;
	int argc;
	char **argv;
	Ui::uiMainWindow *ui;
	QProcess *myProcess = new QProcess(this);

	SettingsWindow *settingsWindow = new SettingsWindow(this);

protected:
	void closeEvent(QCloseEvent *event) override;

	bool saveGame(SaveFormat saveFormat) const;

	bool loadGame(SaveFormat saveFormat);

};


#endif //DONATIONMINING_MAINWINDOW_H
