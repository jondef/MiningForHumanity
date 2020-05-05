//
// Created by jon on 17.04.20.
//

#ifndef DONATIONMINING_LOGINWIDGET_H
#define DONATIONMINING_LOGINWIDGET_H

#include <QtCore>
#include <QObject>
#include <QMainWindow>
#include "ui_LoginWidget.h"
#include "StackedWidget.h"

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

	const QPixmap *pixmap() const;

protected:
	void showEvent(QShowEvent *event) override;

	void paintEvent(QPaintEvent *) override;

private slots:

	void checkCredentials();

	void setPixmap(const QPixmap &);

Q_SIGNALS:

	void userAuthorized(QString username, QString password, AccountType type);

private:
	Ui::uiLogin *ui;
	QPixmap pix;
};


#endif //DONATIONMINING_LOGINWIDGET_H
