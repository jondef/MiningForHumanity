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

public:
	enum MinerState {
		NotMining,
		Starting,
		Mining
	};

public:
	explicit MinerManager(QWidget *parent = nullptr);

	~MinerManager();

protected:
	void showEvent(QShowEvent *event) override;

public slots:
	MinerState getMinerState();

	void startMiner();

	void startingMiner();

	void stopMiner();

private slots:
	void setMinerState(MinerState aState);

	static QJsonObject readConfigFile(const QString &config);

	static bool writeConfigFile(const QString &fileName, const QJsonObject &jsonConfig);

	void updateWidgets();

	void updateConfigFile();

	QJsonObject getAvailablePools();

	void networkReplyOrganizations();

Q_SIGNALS:
	void minerChangedState(MinerState);

private:
	Ui::uiSettingsWindow *ui;

	QProcess *myProcess = new QProcess(this);
	QString mMinerExecutable = "xmrig"; // this works on both Windows and Unix, no need to specify file extension.
	QStringList mMinerArgs;

	MinerState state = MinerState::NotMining;
	QMetaObject::Connection minerConnection;

	QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
	QNetworkReply *reply;

public:

};


#endif //DONATIONMINING_MINERMANAGER_H
