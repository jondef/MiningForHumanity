//
// Created by jon on 11.04.20.
//

#include "MinerManager.hpp"
#include <QScrollBar>

// ! FOR POOLS WIDGET SETTINGS USE A TABLE

MinerManager::MinerManager(QWidget *parent) : QWidget(parent), ui(new Ui::uiSettingsWindow) {
	ui->setupUi(this);

//	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() { updateConfigFile(); });
//	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [this]() { this->close(); });

//	connect(networkManager, &QNetworkAccessManager::finished, this, &MinerManager::getAvailablePools);
//	connect(button, clicked, this, [&]() { networkManager->get(QNetworkRequest(QUrl("https://httpbin.org/get"))); }
//	connect(myProcess, &QProcess::readyReadStandardOutput, [this]() { qDebug() << myProcess->readAllStandardOutput().trimmed(); });
//	connect(myProcess, &QProcess::readyReadStandardOutput, [this]() { ui->plainTextEdit->appendPlainText(myProcess->readAllStandardOutput().trimmed()); });
	connect(myProcess, &QProcess::readyReadStandardError, [this]() { ui->plainTextEdit->appendPlainText(myProcess->readAllStandardError().trimmed()); });
	connect(myProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [this](){ setMinerState(MinerState::NotMining); });
//	connect(ui->lineEdit_poolAdress, &QLineEdit::returnPressed, [this](){ myProcess->write(ui->lineEdit_poolAdress->text().toLatin1() + "\n"); ui->lineEdit_poolAdress->clear(); });
//	connect(myProcess, &QProcess::started, this, [](){ qDebug() << "Process successfully started!"; });
	QString poolId = "xmr.pool.minergate.com:45700";
	QString username = "ludwig.landji@gmail.com";
	QString password = "";
	QString RIGID = "TestRig";
//	bool SSLSupport = false;
	mMinerArgs  << "--url" << poolId << "--user" << username << "--pass" << password << "--coin" << "monero" << "--rig-id" << RIGID << "--print-time" << "5" << "--keepalive" << "--no-color";
}

MinerManager::~MinerManager() {
	delete ui;
}

void MinerManager::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
//	getAvailablePools();
//	updateWidgets();
}

void MinerManager::stopMiner() {
	myProcess->kill();
	ui->plainTextEdit->appendPlainText("----- Process killed -----");
	if (minerConnection != nullptr) {
		disconnect(minerConnection);
	}
}

void MinerManager::startMiner() {
	setMinerState(MinerState::Starting);
	connect(myProcess, &QProcess::readyReadStandardOutput, this, &MinerManager::startingMiner);
	myProcess->start(mMinerExecutable, mMinerArgs);
}

void MinerManager::startingMiner() {
	QByteArray output = myProcess->readAllStandardOutput().trimmed();
	ui->plainTextEdit->appendPlainText(output);
	if (output.contains("READY")) {
		setMinerState(MinerState::Mining);
		disconnect(myProcess, &QProcess::readyReadStandardOutput, this, &MinerManager::startingMiner);
		minerConnection = connect(myProcess, &QProcess::readyReadStandardOutput, this, [this]() {
			ui->plainTextEdit->appendPlainText(myProcess->readAllStandardOutput().trimmed()); });
	}
}

void MinerManager::setMinerState(MinerManager::MinerState aState) {
	state = aState;
	emit minerChangedState(state);
}

MinerManager::MinerState MinerManager::getMinerState() {
	return state;
}

/*
 * Reads the state of every widget
 * and writes the changes to the file
 */
void MinerManager::updateConfigFile() {
	QJsonObject jsonConfig = readConfigFile(QStringLiteral("config.txt"));
	jsonConfig["h_print_time"] = ui->hashraterateReportFreqencySpinBox->value();

	writeConfigFile("config.txt", jsonConfig);
}

/*
 * reads the config file and updates the widget accordingly
 */
void MinerManager::updateWidgets() {
	QJsonObject jsonConfig = readConfigFile(QStringLiteral("config.txt"));
	ui->callTimeoutSpinBox->setValue(jsonConfig["call_timeout"].toInt());
	ui->hashraterateReportFreqencySpinBox->setValue(jsonConfig["h_print_time"].toInt());
}

QJsonObject MinerManager::readConfigFile(const QString &config) {
	QFile loadFile(config);

	if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning("Couldn't open save file.");
		return QJsonObject(); // todo: return something else?
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
	for (int x = 0; x < commaCount; ++x) {
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

	if (jsonError.error != QJsonParseError::NoError) {
		qDebug() << jsonError.errorString();
	}

	return jsonDocument.object();
}

bool MinerManager::writeConfigFile(const QString &fileName, const QJsonObject &jsonConfig) {
	QJsonDocument jsonDocument;
	jsonDocument.setObject(jsonConfig);
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

	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	file.write(fileData);
	file.close();
	return true;
}

/*
 * Fetches the json data from the website
 * and returns a json object (or an empty one if an error occurred)
 */
QJsonObject MinerManager::getAvailablePools() {
	reply = networkManager->get(QNetworkRequest(QUrl("https://httpbin.org/get")));

	connect(reply, &QIODevice::readyRead, this, &MinerManager::networkReplyOrganizations);
//	connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &MyClass::slotError);
//	connect(reply, &QNetworkReply::sslErrors, this, &MyClass::slotSslErrors);

//	if (!reply->error()) {
//		qWarning() << __FILE__ <<  __LINE__ << Q_FUNC_INFO << reply->error();
//		return QJsonObject();
//	}
//	QJsonObject jsonRoot = QJsonDocument::fromJson(reply->readAll()).object();
//	reply->deleteLater();
//	return jsonRoot;
	return QJsonObject();
}

void MinerManager::networkReplyOrganizations() {
	QString string("{ \"automatic\": 5 }");
	QByteArray a;
	a += string;
	QJsonParseError jsonError;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(a, &jsonError);

	if (jsonError.error != QJsonParseError::NoError) {
		qDebug() << jsonError.errorString();
	}
//	QJsonObject jsonRoot = QJsonDocument::fromJson(a).object(); // reply->readAll()
	reply->deleteLater();
	disconnect(reply, &QIODevice::readyRead, this, &MinerManager::networkReplyOrganizations);
	qDebug() << jsonDocument.object();
}

