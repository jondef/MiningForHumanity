//
// Created by jon on 11.04.20.
//

#ifndef DONATIONMINING_MINERMANAGER_H
#define DONATIONMINING_MINERMANAGER_H

#include "ui_SettingsWindow.h"
#include <QtCore>
#include <QObject>
#include <QtNetwork>

class MinerManager : public QWidget {
Q_OBJECT

private:
	Ui::uiSettingsWindow *ui;

public:
	explicit MinerManager(QWidget *parent = nullptr);

	~MinerManager();

	static QJsonObject readConfigFile(const QString &config);

	static bool writeConfigFile(const QString &fileName, const QJsonObject &jsonConfig);

public slots:

	void startMiner();

Q_SIGNALS:

	void userAuthorized(QString username, QString password);

protected:
	void showEvent(QShowEvent *event) override;

	void updateWidgets();

	void updateConfigFile();

	QJsonObject getAvailablePools();

	QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
	QNetworkReply *reply;

	QProcess *myProcess = new QProcess(this);
	QString mMinerExecutable = "xmrig"; // this works on both Windows and Unix, no need to specify file extension.
	QStringList mMinerArgs;

	void networkReplyOrganizations();
};


#endif //DONATIONMINING_MINERMANAGER_H
