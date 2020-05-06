//
// Created by jon on 17.04.20.
//

#include <QtWidgets/QGraphicsBlurEffect>
#include <QtGui/QDesktopServices>
#include <QtGui/QRegExpValidator>
#include <QPainter>
#include <QPaintEvent>
#include "LoginWidget.h"
#include <QDesktopWidget>
#include <QtWidgets/QMessageBox>
#include "bcrypt/BCrypt.hpp"

// todo: add a small padlock at the bottom of the login widget

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::uiLogin) {
	ui->setupUi(this);

	blurLayer = new BlurLayer(ui->centralWidget);
	QGraphicsBlurEffect *p_blur = new QGraphicsBlurEffect;
	p_blur->setBlurRadius(10);
	p_blur->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
	blurLayer->setGraphicsEffect(p_blur);

	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.0.8");
	db.setPort(3306);
	db.setDatabaseName("userdb");
	db.setUserName("application_user");
	db.setPassword("miningforhumanity");
	if (!db.open()) {
		qDebug() << "FAILED TO CONNECT";
		return;
	}


	// hide the incorrect combination label
	ui->label_incorrectLogin->hide();

//	QString command = "SELECT name FROM department";
//	QSqlQuery query(db);
//	if (query.exec(command)) {
//		while (query.next()) {
//			QString name = query.value("name").toString();
//			qDebug() << name;
//		}
//	}

	connect(ui->pushButton_switchToRegisterPage, &QPushButton::clicked, this, [this]() {
		ui->stackedWidget->setCurrentIndex(1);
	});
	connect(ui->pushButton_switchToLoginPage, &QPushButton::clicked, this, [this]() {
		ui->stackedWidget->setCurrentIndex(0);
	});


	connect(ui->pushButton_login, &QPushButton::clicked, this, &LoginWidget::checkCredentials);
	connect(ui->pushButton_register, &QPushButton::clicked, this, [this]() {
		if (ui->lineEdit_registerPassword->text() != ui->lineEdit_registerPasswordRep->text()) {
			QMessageBox::warning(this, "Warning!", "Passwords do not match.");
			return;
		}
		if (ui->lineEdit_registerPassword->text().length() < 8) {
			QMessageBox::warning(this, "Warning!", "Password too short.");
			return;
		}
//		const QRegExpValidator *x = static_cast<const QRegExpValidator*>(ui->lineEdit_registerEmail->validator());
//		auto xx = ui->lineEdit_registerEmail->text();
//		x->validate(xx, 0);

		QString email = ui->lineEdit_registerEmail->text();
		QString password = ui->lineEdit_registerPassword->text();

		QString command = "INSERT INTO user_login (email, password) VALUES ('" + email + "', '" + hashPassword(password) + "');";
		QSqlQuery query(db);
		query.exec(command);
		QMessageBox::information(this, "Success!", "Account created successfully.");
		ui->stackedWidget->setCurrentIndex(0);
	});
	connect(ui->pushButton_forgotPassword, &QPushButton::clicked, this, [this]() {
		QDesktopServices::openUrl(QUrl("https://www.miningforhumanity.org/forgotpassword"));
	});


	QRegExp emailRegex(
			"(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|\"(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21\\x23-\\x5b\\x5d-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])*\")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+)\\])");
	ui->lineEdit_loginEmail->setValidator(new QRegExpValidator(emailRegex, ui->lineEdit_loginEmail));
	ui->lineEdit_registerEmail->setValidator(new QRegExpValidator(emailRegex, ui->lineEdit_registerEmail));

	QRegExp usernameRegex("[A-Za-z0-9-_.]+");
	ui->lineEdit_username->setValidator(new QRegExpValidator(usernameRegex, ui->lineEdit_username));
	// todo: add validator for register email line edit

	// set the logo at the top
	QPixmap map(":/images/mining_for_humanity_logo");
	QSize pixSize = map.size();
	pixSize.scale(QSize(100, 100), Qt::KeepAspectRatio);
	QPixmap scaledPix = map.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->label_title_logo->setPixmap(scaledPix);

	// set background image
	setPixmap(QPixmap(":/images/new_york"));
}

void LoginWidget::paintEvent(QPaintEvent *event) {
	if (pix.isNull()) { return; }

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// set the background image of the login screen
	QSize pixSize = pix.size();
	pixSize.scale(size(), Qt::KeepAspectRatioByExpanding); // ! don't use event->rect().size() instead of size()
	QPixmap scaledPix = pix.scaled(pixSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
	painter.drawPixmap(QPoint(), scaledPix);

	// send the cropped pix map to the blur layer
	QPoint position = ui->centralFrame->mapTo(this, ui->centralFrame->rect().topLeft());
	blurLayer->setGeometry(position.x(), position.y(), ui->centralFrame->size().rwidth(), ui->centralFrame->size().rheight());
	QPixmap cropped = scaledPix.copy(QRect(position, ui->centralFrame->size()));
	blurLayer->pixmap = cropped;

	QWidget::paintEvent(event);
}

const QPixmap *LoginWidget::pixmap() const {
	return &pix;
}

void LoginWidget::setPixmap(const QPixmap &pixmap) {
	pix = pixmap;
}

void LoginWidget::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	blurLayer->show();
	ui->centralFrame->raise();
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
	LoginWidget::AccountType user = Organisation;

	QString command = "SELECT * FROM user_login WHERE email = '" + email + "' AND password = '" + hashPassword(password) + "'";

	QSqlQuery query(db);
	if (query.exec(command)) {
		if (query.size() != 1) {
			ui->label_incorrectLogin->show();
			QTimer::singleShot(5000, this, [this]() { ui->label_incorrectLogin->hide(); });
			return;
		}
		emit userAuthorized(email, password, user);
	}

	// create login save file
}

/*
 * returns true if an automatic login is possible
 * checks if a credentials file exists and with a remember field in it
 *
 * if a file is found that doesn't contain the remember field then
 * read the email address and update the line edit and insert it
 *
 * @return true if a credentials file is found with a remember field in it
 * @return false if no file is found or a file without a remember field
 */
bool LoginWidget::autoLogin() {
	return false;
}

std::string LoginWidget::hashPassword(QString password) {
	return BCrypt::generateHash(password.toStdString(), 12);
//	return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex();
}


