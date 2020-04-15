//
// Created by jon on 31.03.20.
//

#include "MainWindow.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>


MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) : QMainWindow(parent), ui(new Ui::uiMainWindow) {
	ui->setupUi(this);
	this->argc = argc;
	this->argv = argv;
//	qDebug() << QThread::idealThreadCount();
	// these are used to qsettings
	QCoreApplication::setOrganizationName("MiningForHumanity");
	QCoreApplication::setOrganizationDomain("MiningForHumanity.org");
	QCoreApplication::setApplicationName("DonationMining");
	QSettings settings;

	ui->checkBox_SSLSupport->setChecked(settings.value("editor/wrapMargin").toBool());

	connect(ui->checkBox_SSLSupport, &QCheckBox::toggled, this, [this](bool checked) {
		QSettings settings;
		settings.setValue("editor/wrapMargin", checked);
	});

	connect(ui->actionSettings, &QAction::triggered, this, [this]() {
		settingsWindow->show();
	});

	closing = false;

	auto exitAction = new QAction(tr("&Exit"), this);
	connect(exitAction, &QAction::triggered, [this]() {
		closing = true;
		close();
	});

	auto trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(exitAction);

	auto sysTrayIcon = new QSystemTrayIcon(this);
	sysTrayIcon->setContextMenu(trayIconMenu);
	sysTrayIcon->setIcon(QIcon(":/images/icon"));
	sysTrayIcon->setToolTip(tr("DonationMining"));
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

	connect(myProcess, &QProcess::readyReadStandardOutput, [this]() { ui->plainTextEdit->appendPlainText(myProcess->readAllStandardOutput()); });
	connect(myProcess, &QProcess::readyReadStandardError, [this]() { ui->plainTextEdit->appendPlainText(myProcess->readAllStandardError()); });
	connect(myProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &MainWindow::close);
	connect(ui->lineEdit_poolAdress, &QLineEdit::returnPressed, [this]() {
		myProcess->write(ui->lineEdit_poolAdress->text().toLatin1() + "\n");
		ui->lineEdit_poolAdress->clear();
	});

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() -> void {
		QString poolId = ui->lineEdit_poolAdress->text();
		QString username = ui->lineEdit_username->text();
		QString password = ui->lineEdit_password->text();
		QString RIGID = "workstation1";
		bool SSLSupport = ui->checkBox_SSLSupport->isChecked();

		QString program = "../../xmr-stak/bin/xmr-stak-rx";
		QStringList arguments;
		arguments << "--noTest" << "--noDevSupport" << "--currency" << "monero" << "--h-print-time" << "60" << "--url" << poolId << "--rigid" << RIGID
		          << "--httpd" << "0" << "--user" << username << "--pass" << password;

		myProcess->start(program, arguments);
		//loadGame(Json);
//		readConfigFile(QString("config.txt"));
	});
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [&]() { this->close(); });
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (closing) {
		event->accept();
	} else {
		this->hide();
		event->ignore();
	}
}