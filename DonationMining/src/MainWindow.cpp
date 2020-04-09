//
// Created by jon on 31.03.20.
//

#include "MainWindow.h"

MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) : QMainWindow(parent), ui(new Ui::uiMainWindow) {
	ui->setupUi(this);
	this->argc = argc;
	this->argv = argv;
	qDebug() << QThread::idealThreadCount();
//	QCoreApplication::setOrganizationName("MySoft"); MiningForHumanity
//	QCoreApplication::setOrganizationDomain("mysoft.com");
//	QCoreApplication::setApplicationName("Star Runner");
	ui->checkBox_SSLSupport->setChecked(settings->value("editor/wrapMargin").toBool());

	connect(ui->checkBox_SSLSupport, &QCheckBox::toggled, this, [&](bool checked) {
		settings->setValue("editor/wrapMargin", checked);
	});

	connect(myProcess, &QProcess::readyReadStandardOutput, [this](){ ui->plainTextEdit->appendPlainText(myProcess->readAllStandardOutput()); });
	connect(myProcess, &QProcess::readyReadStandardError, [this](){ ui->plainTextEdit->appendPlainText(myProcess->readAllStandardError()); });
	connect(myProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &MainWindow::close);
	connect(ui->lineEdit_poolAdress, &QLineEdit::returnPressed, [this](){ myProcess->write(ui->lineEdit_poolAdress->text().toLatin1() + "\n"); ui->lineEdit_poolAdress->clear(); });

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() -> void {
		QString poolId = ui->lineEdit_poolAdress->text();
		QString username = ui->lineEdit_username->text();
		QString password = ui->lineEdit_password->text();
		QString RIGID = "What";
		bool SSLSupport = ui->checkBox_SSLSupport->isChecked();

		QString program = "../../xmr-stak/bin/xmr-stak-rx";
		QStringList arguments;
		arguments << "--noTest" << "--noDevSupport" << "--currency" << "monero" << "--h-print-time" << "5" << "--url" << poolId << "--rigid" << RIGID << "--httpd" << "0" << "--user" << username << "--pass" << password;

		myProcess->start(program, arguments);

//		 save / connect to pools
		// minimise window
	});
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [&]() {
		this->close();
	});
}

MainWindow::~MainWindow() {

}

