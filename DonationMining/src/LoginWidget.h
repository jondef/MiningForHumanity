//
// Created by jon on 17.04.20.
//

#ifndef DONATIONMINING_LOGINWIDGET_H
#define DONATIONMINING_LOGINWIDGET_H

#include <QtCore>
#include <QObject>
#include <QMainWindow>
#include "ui_LoginWidget.h"

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

private slots:

	void checkCredentials();

Q_SIGNALS:

	void userAuthorized(QString username, QString password, AccountType type);

private:
	Ui::uiLogin *ui;


};


#endif //DONATIONMINING_LOGINWIDGET_H
