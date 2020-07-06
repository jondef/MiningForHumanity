//
// Created by jon on 31.03.20.
//

#include "MainWindow.hpp"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>

// todo: add reload information button next to shutdown button

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::uiMainWindow) {
	ui->setupUi(this);

	QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
	resize(screenGeometry.width() / 1.5, screenGeometry.height() / 1.5);

	if (loginWindow->autoLogin()) {
		showDashboard();
	} else {
		showLoginScreen();
	}

	setWindowIcon(QIcon(":/images/MFH_logo_2"));
	setBackgroundPixmap(QPixmap(":/images/new_york"));

	// region configure the system tray
	auto exitAction = new QAction(tr("&Exit"), this);
	connect(exitAction, &QAction::triggered, [this]() {
		closing = true;
		close();
	});
	auto trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(exitAction);

	auto sysTrayIcon = new QSystemTrayIcon(this);
	sysTrayIcon->setContextMenu(trayIconMenu);
	sysTrayIcon->setIcon(QIcon(":/images/MFH_logo_2"));
	sysTrayIcon->setToolTip("MiningForHumanity");
	sysTrayIcon->show();

	connect(sysTrayIcon, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason) {
		if (reason == QSystemTrayIcon::Trigger) {
			if (isVisible()) {
				hide();
			} else {
				show();
				activateWindow();
			}
		}
	});
	// endregion

	connect(loginWindow, &LoginWidget::userAuthorized, this, &MainWindow::showDashboard);
	connect(ui->toolBar, &ToolBar::userLogOut, this, &MainWindow::showLoginScreen);

	connect(ui->toolBar, &ToolBar::changePage, this, &MainWindow::changePage);

	connect(ui->dashboard, &Dashboard::ribbonSetCampaign, ui->ribbon, &Ribbon::showCampaign);

}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::showEvent(QShowEvent *event) {
	// If the user opens the app for the first time,
	// ask him to register / login
}

void MainWindow::paintEvent(QPaintEvent *event) {
	QWidget::paintEvent(event);
	if (backgroundPixmap.isNull()) { return; }

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// set the background image of the login screen
	QSize pixSize = backgroundPixmap.size();
	pixSize.scale(size(), Qt::KeepAspectRatioByExpanding); // ! don't use event->rect().size() instead of size()
	QPixmap scaledPix = backgroundPixmap.scaled(pixSize, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
	painter.drawPixmap(QPoint(), scaledPix);
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (closing) {
		event->accept();
	} else {
		this->hide();
		event->ignore();
	}
}

/*
 * This function hides the login widget and shows the dashboard
 */
void MainWindow::showDashboard() {
	if (centralWidget != nullptr) {
		takeCentralWidget(); // don't need to take ownership of the login widget because it is already a member
		setCentralWidget(centralWidget);
	}
	changePage(GLOBALS::Dashboard);
}

/*
 * This function is used when logging out the user
 */
void MainWindow::showLoginScreen() {
	LoginWidget::deleteRememberMeCookie();
	centralWidget = takeCentralWidget();
	loginWindow->resetInputFields();
	setCentralWidget(loginWindow);
}

void MainWindow::changePage(GLOBALS::Page page) {
	switch(page) {
		case GLOBALS::Dashboard:
			ui->ribbon->showLabel(tr("Welcome back, ") + loginWindow->getUsername() + "!");
			ui->dashboard->changePage(0);
			break;
		case GLOBALS::CampaignPreview: // campaignPreview is not managed here
			break;
		case GLOBALS::Settings:
			ui->ribbon->showLabel(tr("Settings"));
			ui->dashboard->changePage(3);
			break;
		case GLOBALS::FinishedCampaigns:
			break;
		case GLOBALS::OngoingCampaigns:
			ui->ribbon->showLabel(tr("Ongoing Campaigns"));
			ui->dashboard->changePage(2);
			break;
	}
}
