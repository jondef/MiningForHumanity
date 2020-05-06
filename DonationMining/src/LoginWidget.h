//
// Created by jon on 17.04.20.
//

#ifndef DONATIONMINING_LOGINWIDGET_H
#define DONATIONMINING_LOGINWIDGET_H

#include <QtCore>
#include <QObject>
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "ui_LoginWidget.h"
#include "BlurLayer.h"

class LoginWidget : public QWidget {
Q_OBJECT

public:
	enum AccountType {
		Organisation,
		Donator
	};

public:
	explicit LoginWidget(QWidget *parent = nullptr);

	~LoginWidget();

	bool autoLogin();

protected:
	void showEvent(QShowEvent *event) override;

	void paintEvent(QPaintEvent *) override;

private slots:

	void checkCredentials();

	void setPixmap(const QPixmap &);

	static QByteArray hashPassword(const QString &password);

	static bool validatePassword(const QString &password, const QByteArray &hash);

	const QPixmap *pixmap() const;

	void createUserAccount();

Q_SIGNALS:

	void userAuthorized(QString username, QString password, AccountType type);

private:
	Ui::uiLogin *ui;
	QPixmap pix;
	BlurLayer *blurLayer;
	QSqlDatabase db;

};


#endif //DONATIONMINING_LOGINWIDGET_H
