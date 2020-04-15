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
//	readConfigFile(QString("cpu.txt"));
	writeConfigFile(QString("cpuz.txt"), readConfigFile(QString("cpu.txt")));
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
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [&]() {
		this->close();
		//saveGame(Json);
	});
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

void MainWindow::read(const QJsonObject &json) {
	qDebug() << json["cpu_threads_conf"].toString();
	qDebug() << json["level"].toDouble();
}

QVariantHash MainWindow::readConfigFile(const QString& config){
	QFile loadFile(config);

	if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning("Couldn't open save file.");
		return QVariantHash(); // todo: return something else?
	}

	QByteArray fileData = loadFile.readAll();
	loadFile.close();

	// remove single line comments
	while (fileData.contains("//")) {
		int commentStartIndex = fileData.indexOf("//");
		int commentEndIndex = fileData.indexOf("\n", commentStartIndex);
		fileData.remove(commentStartIndex, commentEndIndex - commentStartIndex);
	}

	// remove multi-line comments
	while (fileData.contains("/*") || fileData.contains("*/")) {
		int commentStartIndex = fileData.indexOf("/*");
		int commentEndIndex = fileData.indexOf("*/") + 2; // + 2 because of the 2 characters that we're looking for
		fileData.remove(commentStartIndex, commentEndIndex - commentStartIndex);
	}

	// prepend and append the curly brackets
	fileData.prepend("{");
	fileData.append("}");

	// remove trailing commas
	int commaIndex = -1;
	int commaCount = fileData.count(",");
	for(int x = 0; x < commaCount; ++x) {
		commaIndex = fileData.indexOf(",", commaIndex + 1);
		int closingSquareBracketIndex = fileData.indexOf("]", commaIndex);
		int closingCurleyBracketIndex = fileData.indexOf("}", commaIndex);
		int doublePointIndex = fileData.indexOf(":", commaIndex);

		// if there is no ":" after a comma
		if (doublePointIndex == -1) {
			fileData.remove(commaIndex, 1);
			continue;
		}
		// if there's (closingCurleyBracket before a ":") after a comma
		if (closingCurleyBracketIndex != -1) {
			if (closingCurleyBracketIndex < doublePointIndex) {
				fileData.remove(commaIndex, 1);
				continue;
			}
		}
		// if there's (closingSquareBracket before a ":") after a comma
		if (closingSquareBracketIndex != -1) {
			if (closingSquareBracketIndex < doublePointIndex) {
				fileData.remove(commaIndex, 1);
				continue;
			}
		}
	}

	QJsonParseError jsonError;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData, &jsonError);

	if (jsonError.error != QJsonParseError::NoError){
		qDebug() << jsonError.errorString();
	}

	QVariantHash x = jsonDocument.object().toVariantHash();
	qDebug() << x["cpu_threads_conf"].toJsonArray().count();
	return jsonDocument.object().toVariantHash();
}

bool MainWindow::writeConfigFile(const QString &config, QVariantHash hash) {
	QJsonObject jsonObject = QJsonObject::fromVariantHash(hash);

	QJsonDocument jsonDocument;
	jsonDocument.setObject(jsonObject);
	QByteArray fileData = jsonDocument.toJson(QJsonDocument::Indented); // ! Compact makes it crash

	// remove the curley brackets at the beginning and at the end
	fileData.remove(fileData.indexOf("{"), 1);
	fileData.remove(fileData.lastIndexOf("}"), 1);

	// add trailing comma after squared brackets
	int closingSquareBracketCount = fileData.count("]");
	int bracketIndex = -1;
	for (int i = 0; i < closingSquareBracketCount; ++i) {
		bracketIndex = fileData.indexOf("]", bracketIndex + 1);

		if (fileData.at(bracketIndex + 1) != ',') {
			fileData.insert(bracketIndex + 1, ",");
		}
	}

	// add trailing comma after curley brackets
	int closingCurleyBracketCount = fileData.count("}");
	bracketIndex = -1;
	for (int i = 0; i < closingCurleyBracketCount; ++i) {
		bracketIndex = fileData.indexOf("}", bracketIndex + 1);

		if (fileData.at(bracketIndex + 1) != ',') {
			fileData.insert(bracketIndex + 1, ",");
		}
	}

	// adding a trailing comma after a line ending
	// is not required because the miner will successfully
	// read the config files anyway

	QFile file(config);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	file.write(fileData);
	file.close();
	return true;
}


void MainWindow::write(QJsonObject &json) const {
	json["name"] = true;
	json["level"] = 13;
}

bool MainWindow::saveGame(MainWindow::SaveFormat saveFormat) const {
	QFile saveFile(saveFormat == Json ? QStringLiteral("save.json") : QStringLiteral("save.dat"));

	if (!saveFile.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
		return false;
	}

	QJsonObject gameObject;
	write(gameObject);
	QJsonDocument saveDoc(gameObject);
	saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());

	return true;
}

bool MainWindow::loadGame(MainWindow::SaveFormat saveFormat) {
//	QFile loadFile(QStringLiteral("save.json"));
//
//	if (!loadFile.open(QIODevice::ReadOnly)) {
//		qWarning("Couldn't open save file.");
//		return false;
//	}
//
//	QByteArray saveData = loadFile.readAll();
//
//	QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
//
//	this->read(loadDoc.object());
//
//	return true;

	QFile file;
	file.setFileName("save.json");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray fcontent = file.readAll();
	auto x = QString::fromStdString(fcontent.toStdString());

	QJsonParseError jsonError;
	QJsonDocument flowerJson = QJsonDocument::fromJson(fcontent, &jsonError);
	if (jsonError.error != QJsonParseError::NoError) {
		qDebug() << jsonError.errorString();
	}
	QList<QVariant> list = flowerJson.toVariant().toList();
	QMap<QString, QVariant> map = list[0].toMap();
	qDebug() << map["name"].toString();
}