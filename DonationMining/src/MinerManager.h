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
	enum MinerState {
		NotMining,
		Starting,
		Mining
	};

	explicit MinerManager(QWidget *parent = nullptr);

	~MinerManager();

	static QJsonObject readConfigFile(const QString &config);

	static bool writeConfigFile(const QString &fileName, const QJsonObject &jsonConfig);

public slots:

	void startMiner();

	void startingMiner();

	void stopMiner();

Q_SIGNALS:

	void minerChangedState(MinerState);

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
	MinerState state = MinerState::NotMining;
	QMetaObject::Connection minerConnection;

	void networkReplyOrganizations();

	void setMinerState(MinerState aState);

public:
	MinerState getMinerState();
};


#endif //DONATIONMINING_MINERMANAGER_H
