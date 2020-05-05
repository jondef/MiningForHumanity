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


LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::uiLogin) {
	ui->setupUi(this);

	layer = new BlurLayer(ui->centralWidget);
	QGraphicsBlurEffect *p_blur = new QGraphicsBlurEffect;
	p_blur->setBlurRadius(10);
	p_blur->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
	layer->setGraphicsEffect(p_blur);

//	layer->setStyleSheet("background-color: red;");
//	layer->setLayout(new QGridLayout);
//	layer->layout()->setContentsMargins(0,0,0,0);
//	layer->layout()->setSpacing(0);
//	layer->setAttribute(Qt::WA_TransparentForMouseEvents);
//	layer->setFocusPolicy(Qt::NoFocus);

//	label = new QLabel();
//	layer->layout()->addWidget(label);
//	setStyleSheet("#centralWidget{ background-color:red; }");


	connect(ui->pushButton_switchToRegisterPage, &QPushButton::clicked, this, [this]() {
		ui->stackedWidget->setCurrentIndex(1);
	});
	connect(ui->pushButton_switchToLoginPage, &QPushButton::clicked, this, [this]() {
		ui->stackedWidget->setCurrentIndex(0);
	});

	connect(ui->pushButton_login, &QPushButton::clicked, this, &LoginWidget::checkCredentials);
	connect(ui->pushButton_register, &QPushButton::clicked, this, [this]() {});
	connect(ui->pushButton_forgotPassword, &QPushButton::clicked, this, [this]() {
		QDesktopServices::openUrl(QUrl("https://www.miningforhumanity.org/forgotpassword"));
	});


	QRegExp emailRegex(
			"(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|\"(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21\\x23-\\x5b\\x5d-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])*\")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+)\\])");
	QValidator *emailValidator = new QRegExpValidator(emailRegex, ui->lineEdit_loginEmail);
	ui->lineEdit_loginEmail->setValidator(emailValidator);

	QRegExp usernameRegex("[A-Za-z0-9-_.]+");
	QValidator *usernameValidator = new QRegExpValidator(usernameRegex, ui->lineEdit_username);
	ui->lineEdit_username->setValidator(usernameValidator);
	// todo: add validator for register email line edit

	// set the logo at the top
	QPixmap map(":/images/mining_for_humanity_logo");
	QSize pixSize = map.size();
	pixSize.scale(QSize(100, 100), Qt::KeepAspectRatio);
	QPixmap scaledPix = map.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->label_title_logo->setPixmap(scaledPix);

	// set background image
	setPixmap(QPixmap(":/images/login_bg"));
}

void LoginWidget::resizeEvent(QResizeEvent *event) {
	QWidget::resizeEvent(event);
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
	layer->setGeometry(position.x(), position.y(), ui->centralFrame->size().rwidth(), ui->centralFrame->size().rheight());
	QPixmap cropped = scaledPix.copy(QRect(position, ui->centralFrame->size()));
	layer->pixmap = cropped;

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
	layer->show();
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
	LoginWidget::AccountType user;

	// create login save file

	emit userAuthorized(email, password, user);
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

