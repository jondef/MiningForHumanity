//
// Created by jon on 17.04.20.
//

#include "LoginWidget.h"


LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::uiLogin) {
	ui->setupUi(this);

	connect(ui->pushButton_switchToRegisterPage, &QPushButton::clicked, this, [this](){
		ui->stackedWidget->setCurrentIndex(1);
	});
	connect(ui->pushButton_switchToLoginPage, &QPushButton::clicked, this, [this](){
		ui->stackedWidget->setCurrentIndex(0);
	});

	connect(ui->pushButton_login, &QPushButton::clicked, this, &LoginWidget::checkCredentials);
	connect(ui->pushButton_register, &QPushButton::clicked, this, [this](){});
	connect(ui->pushButton_forgotPassword, &QPushButton::clicked, this, [this](){});
}

LoginWidget::~LoginWidget() {
	delete ui;
}

/*
 * Gets the input from the line edits and
 * verifies them. If they are valid, a signal is emitted
 */
void LoginWidget::checkCredentials() {
	QString email = ui->lineEdit_loginEmail->text();
	QString password = ui->lineEdit_loginPassword->text();
	bool rememberMe = ui->checkBox_rememberMe->isChecked();

	// create login save file

	emit userAuthorized(email, password);
}
