//
// Created by jon on 17.04.20.
//

#include <QtGui/QDesktopServices>
#include <QtGui/QRegExpValidator>
#include <QPainter>
#include <QPaintEvent>
#include "LoginWidget.hpp"
#include <QDesktopWidget>
#include <QtWidgets/QMessageBox>
#include "bcrypt/BCrypt.hpp"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::uiLogin) {
	ui->setupUi(this);

	blurLayer = new BlurLayer(ui->centralWidget);

	// set the logo at the top
	QPixmap map(":/images/MFH_logo_2");
	QSize pixSize = map.size();
	pixSize.scale(QSize(150, 150), Qt::KeepAspectRatio);
	QPixmap scaledPix = map.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->label_title_logo->setPixmap(scaledPix);

	// set padlock image
	map = QPixmap(":/images/padlock");
	pixSize = map.size();
	pixSize.scale(QSize(25, 25), Qt::KeepAspectRatio);
	scaledPix = map.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->label_padlock_icon->setPixmap(scaledPix);

	// set background image
	setPixmap(QPixmap(":/images/new_york"));

	// hide the incorrect login label
	ui->label_incorrectLogin->hide();

	// configure the database
	db = QSqlDatabase::addDatabase("QMYSQL", "loginManager");
	db.setHostName("192.168.0.8");
	db.setPort(3306);
	db.setDatabaseName("userdb");
	db.setUserName("application_user");
	db.setPassword("miningforhumanity");
	if (!db.open()) {
		db.setHostName("62.203.57.210");
		if (!db.open()) {
			qDebug() << "FAILED TO CONNECT TO DATABASE";
			QMessageBox::warning(this, "Warning", "Failed to connect to database!");
		}
	}

	// page switching connections
	connect(ui->pushButton_switchToRegisterPage, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1); });
	connect(ui->pushButton_switchToLoginPage, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(0); });

	// login / register / forgot password buttons
	connect(ui->pushButton_login, &QPushButton::clicked, this, &LoginWidget::LoginButtonPressed);
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
	if (backgroundPixmap.isNull()) { return; }

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// set the background image of the login screen
	QSize pixSize = backgroundPixmap.size();
	pixSize.scale(size(), Qt::KeepAspectRatioByExpanding); // ! don't use event->rect().size() instead of size()
	QPixmap scaledPix = backgroundPixmap.scaled(pixSize, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
	painter.drawPixmap(QPoint(), scaledPix);

	// send the cropped pix map to the blur layer
	QPoint position = ui->centralFrame->mapTo(this, ui->centralFrame->rect().topLeft());
	blurLayer->setGeometry(position.x(), position.y(), ui->centralFrame->size().rwidth(), ui->centralFrame->size().rheight());
	blurLayer->pixmap = scaledPix.copy(QRect(position, ui->centralFrame->size()));
}

void LoginWidget::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	blurLayer->show();
	ui->centralFrame->raise();
}

[[maybe_unused]] const QPixmap &LoginWidget::pixmap() const {
	return backgroundPixmap;
}

void LoginWidget::setPixmap(const QPixmap &pixmap) {
	backgroundPixmap = pixmap;
}

/*
 * Gets the input from the line edits and
 * verifies them. If they are valid, a signal is emitted
 */
void LoginWidget::LoginButtonPressed() {
	QString email = ui->lineEdit_loginEmail->text();
	QString password = ui->lineEdit_loginPassword->text();
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
			generateLoginCookie(email);
		}
		emit userAuthorized(getUsername(email));
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
 * returns true if an automatic login is possible otherwise false
 *
 * Checks if a cookie file exists. If one is found,
 * the single use password is checked against the one
 * in the database, and then a new one is generated
 *
 * todo: if password is empty/not saved, read the email and insert it in the line edit
 */
bool LoginWidget::autoLogin() {
	if (QFile::exists(accountFileName)) {
		QByteArray data = readBinary(accountFileName);
		if (data.isNull() || data.isEmpty()) { // if data in file has been edited by user / is invalid
			LoginWidget::deleteRememberMeCookie();
			return false;
		}
		QString email = data.split('\n').at(0); // todo: use QVariant
		QString password = data.split('\n').at(1);
		QString command = QString("SELECT remember_me FROM user_login WHERE email = '%1'").arg(email);

		QSqlQuery query(db);
		if (!query.exec(command)) { return false; }
		if (!query.first()) { return false; }

		QByteArray hash = query.value(0).toByteArray();
		if (!validatePassword(password, hash)) { // password is wrong
			LoginWidget::deleteRememberMeCookie();
			return false;
		}
		m_username = getUsername(email);
		generateLoginCookie(email);
		return true;
	}
	//<editor-fold desc="Description">
	return false;
	//</editor-fold>
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
		qDebug() << "Could not open file" << fileName << "for writing";
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
		qDebug() << "Could not open file" << fileName << "for reading";
		return QByteArray();
	}
	QDataStream in(&mfile);
	in.setVersion(QDataStream::Qt_5_14);
	in >> data;
	mfile.close();

	return QByteArray::fromHex(data);
}

/*
 * Removes the cookie file that contains user login info used
 * for automatic login
 */
void LoginWidget::deleteRememberMeCookie() {
	QFile::remove(accountFileName);
}

QString LoginWidget::getUsername(const QString &email) {
	QString command = QString("SELECT username FROM user_login WHERE email = '%1'").arg(email);
	QSqlQuery query(db);
	if (!query.exec(command)) { return QString(); }
	if (!query.first()) { return QString(); }
	return query.value(0).toString();
}

quint64 LoginWidget::generateRandomNumber() {
	return QRandomGenerator::global()->generate64();
}

/*
 * This function creates a second - single use password for the account.
 * This password is saved in plain text in a file on the user's pc and
 * a hashed version thereof on the database. Every time the user logs in,
 * a new password is generated and saved
 */
void LoginWidget::generateLoginCookie(const QString &email) {
	QSqlQuery query(db);
	QString rememberMePW = QString::number(generateRandomNumber());

	query.exec("UPDATE user_login SET remember_me = '" + hashPassword(rememberMePW) + "' WHERE email = '" + email + "';");

	QByteArray input;
	input.append(email + "\n" + rememberMePW);
	writeBinary(accountFileName, input);
}
