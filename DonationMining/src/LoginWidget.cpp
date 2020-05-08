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

	// set up the blur layer
	blurLayer = new BlurLayer(ui->centralWidget);
	QGraphicsBlurEffect *p_blur = new QGraphicsBlurEffect;
	p_blur->setBlurRadius(10);
	p_blur->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
	blurLayer->setGraphicsEffect(p_blur);

	// set the logo at the top
	QPixmap map(":/images/mining_for_humanity_logo");
	QSize pixSize = map.size();
	pixSize.scale(QSize(100, 100), Qt::KeepAspectRatio);
	QPixmap scaledPix = map.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->label_title_logo->setPixmap(scaledPix);

	// set background image
	setPixmap(QPixmap(":/images/new_york"));

	// hide the incorrect login label
	ui->label_incorrectLogin->hide();

	// configure the database
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.0.8");
	db.setPort(3306);
	db.setDatabaseName("userdb");
	db.setUserName("application_user");
	db.setPassword("miningforhumanity");
	if (!db.open()) {
		qDebug() << "FAILED TO CONNECT TO DATABASE";
		return;
	}

	// page switching connections
	connect(ui->pushButton_switchToRegisterPage, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1); });
	connect(ui->pushButton_switchToLoginPage, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(0); });

	// login / register / forgot password buttons
	connect(ui->pushButton_login, &QPushButton::clicked, this, &LoginWidget::checkCredentials);
	connect(ui->pushButton_register, &QPushButton::clicked, this, &LoginWidget::createUserAccount);
	connect(ui->pushButton_forgotPassword, &QPushButton::clicked, this, [this]() {
		QDesktopServices::openUrl(QUrl("https://www.miningforhumanity.org/forgotpassword"));
	});

	// regex validators
	QRegExp emailRegex(
			"(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|\"(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21\\x23-\\x5b\\x5d-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])*\")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+)\\])");
	ui->lineEdit_loginEmail->setValidator(new QRegExpValidator(emailRegex, ui->lineEdit_loginEmail));
	ui->lineEdit_registerEmail->setValidator(new QRegExpValidator(emailRegex, ui->lineEdit_registerEmail));

	QRegExp usernameRegex("[A-Za-z0-9-_.]+");
	ui->lineEdit_username->setValidator(new QRegExpValidator(usernameRegex, ui->lineEdit_username));
}

LoginWidget::~LoginWidget() {
	delete ui;
}

void LoginWidget::paintEvent(QPaintEvent *event) {
	QWidget::paintEvent(event);
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
}

void LoginWidget::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	blurLayer->show();
	ui->centralFrame->raise();
}

const QPixmap *LoginWidget::pixmap() const {
	return &pix;
}

void LoginWidget::setPixmap(const QPixmap &pixmap) {
	pix = pixmap;
}

/*
 * Gets the input from the line edits and
 * verifies them. If they are valid, a signal is emitted
 */
void LoginWidget::checkCredentials() {
	QString email = ui->lineEdit_loginEmail->text();
	QString password = ui->lineEdit_loginPassword->text();
	LoginWidget::AccountType user = Organisation;

	QString command = QString("SELECT password FROM user_login WHERE email = '%1'").arg(email);

	QSqlQuery query(db);
	if (query.exec(command)) {
		if (!query.first()) {
			ui->label_incorrectLogin->show();
			QTimer::singleShot(5000, this, [this]() { ui->label_incorrectLogin->hide(); });
			return;
		}
		QByteArray hash = query.value(0).toByteArray();
		if (!validatePassword(password, hash)) {
			ui->label_incorrectLogin->show();
			QTimer::singleShot(5000, this, [this]() { ui->label_incorrectLogin->hide(); });
			return;
		}
		if (ui->checkBox_rememberMe->isChecked()) {
			QByteArray input;
			input.append(email + "\n" + password);
			writeBinary(accountFileName, input);
		}
		emit userAuthorized(email, password, user);
	}
}

void LoginWidget::createUserAccount() {
	QSqlQuery query(db);
	QString email = ui->lineEdit_registerEmail->text();
	QString password = ui->lineEdit_registerPassword->text();
	QString username = ui->lineEdit_username->text();

	// make sure the email address is valid
	const QRegExpValidator *valid = dynamic_cast<const QRegExpValidator *>(ui->lineEdit_loginEmail->validator());
	int x = 0;
	QValidator::State state = valid->validate(email, x);
	if (state == QValidator::Invalid || state == QValidator::Intermediate) {
		QMessageBox::warning(this, "Warning!", "Invalid email address.");
		return;
	}
	// check if email already exists
	query.exec("SELECT email FROM user_login WHERE email = '" + email + "'");
	if (query.first()) {
		QMessageBox::warning(this, "Warning!", "Email already exists.");
		return;
	}
	// make sure the username is valid
	valid = dynamic_cast<const QRegExpValidator *>(ui->lineEdit_username->validator());
	x = 0;
	state = valid->validate(username, x);
	if (state == QValidator::Invalid || state == QValidator::Intermediate) {
		QMessageBox::warning(this, "Warning!", "Invalid username.");
		return;
	}
	// check if username already exists
	query.exec("SELECT username FROM user_login WHERE username = '" + username + "'");
	if (query.first()) {
		QMessageBox::warning(this, "Warning!", "Username already taken.");
		return;
	}
	// check if password length is good
	if (password.length() < 8) {
		QMessageBox::warning(this, "Warning!", "Password too short.");
		return;
	}
	// check if passwords match
	if (password != ui->lineEdit_registerPasswordRep->text()) {
		QMessageBox::warning(this, "Warning!", "Passwords do not match.");
		return;
	}
	query.exec("INSERT INTO user_login (email,password,username) VALUES ('" + email + "', '" + hashPassword(password) + "', '" + username + "');");
	QMessageBox::information(this, "Success!", "Account created successfully. Please verify your address.");
	ui->stackedWidget->setCurrentIndex(0);
	// todo: send confirmation email
}

/*
 * returns true if an automatic login is possible
 * checks for a credentials file. If one is found,
 * the login is matched against the database
 *
 * if a file is found that doesn't contain the remember field then
 * read the email address and update the line edit and insert it
 *
 * @return true if a credentials file is found with a remember field in it
 * @return false if no file is found or a file without a remember field
 */
bool LoginWidget::autoLogin() {
	QByteArray data = readBinary(accountFileName);
	if (data.isNull()) { return false; }
	if (data.isEmpty()) { // if data in file has been edited by user / is invalid
		LoginWidget::logOutUser();
		return false;
	}
	QString email = data.split('\n').at(0); // todo: use QVariant
	QString password = data.split('\n').at(1);
	QString command = QString("SELECT password FROM user_login WHERE email = '%1'").arg(email);

	QSqlQuery query(db);
	if (query.exec(command)) {
		if (!query.first()) {
			return false;
		}
		QByteArray hash = query.value(0).toByteArray();
		return validatePassword(password, hash);
	}
}

QByteArray LoginWidget::hashPassword(const QString &password) {
	return QString::fromStdString(BCrypt::generateHash(password.toStdString(), 12)).toUtf8().toHex();
}

/*
 * returns true if passwords match, otherwise false
 */
bool LoginWidget::validatePassword(const QString &password, const QByteArray &hash) {
	return BCrypt::validatePassword(password.toStdString(), QByteArray::fromHex(hash).toStdString());
}

void LoginWidget::writeBinary(const QString &fileName, const QByteArray &data) {
	QFile mfile(fileName);
	if (!mfile.open(QFile::WriteOnly)) {
		qDebug() << "Could not open file for writing";
		return;
	}
	QDataStream out(&mfile);
	out.setVersion(QDataStream::Qt_5_14);
	out << data.toHex();
	mfile.close();
}

QByteArray LoginWidget::readBinary(const QString &fileName) {
	QFile mfile(fileName);
	QByteArray data;
	if (!mfile.open(QFile::ReadOnly)) {
		qDebug() << "Could not open file for reading"; // todo: which file? include filename.
		return data;
	}
	QDataStream in(&mfile);
	in.setVersion(QDataStream::Qt_5_14);
	in >> data;
	mfile.close();

	return QByteArray::fromHex(data);
}

void LoginWidget::logOutUser() {
	QFile::remove(accountFileName);
}

//QPair LoginWidget::getUserInfo() {
//	return QPair
//}

