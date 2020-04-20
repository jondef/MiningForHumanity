//
// Created by jon on 11.04.20.
//

#ifndef DONATIONMINING_SETTINGSWINDOW_H
#define DONATIONMINING_SETTINGSWINDOW_H

#include "ui_SettingsWindow.h"
#include <QtCore>
#include <QObject>
#include <QtNetwork>

class SettingsWindow : public QWidget {
Q_OBJECT

private:
	Ui::uiSettingsWindow *ui;

public:
	explicit SettingsWindow(QWidget *parent = nullptr);

	~SettingsWindow();

	static QJsonObject readConfigFile(const QString &config);

	static bool writeConfigFile(const QString &fileName, const QJsonObject& jsonConfig);

protected:
	void showEvent(QShowEvent* event) override;

	void updateWidgets();

	void updateConfigFile();

	QJsonObject getAvailablePools();

	QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
	QNetworkReply *reply;

	void networkReplyOrganizations();
};


#endif //DONATIONMINING_SETTINGSWINDOW_H
