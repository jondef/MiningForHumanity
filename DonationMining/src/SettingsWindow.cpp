//
// Created by jon on 11.04.20.
//

#include "SettingsWindow.h"

// ! FOR POOLS WIDGET SETTINGS USE A TABLE

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent), ui(new Ui::uiSettingsWindow) {
	ui->setupUi(this);

	// populate the listWidget with the available pages
	QListWidgetItem *generalTab = new QListWidgetItem("General Settings");
	QListWidgetItem *cpuTab = new QListWidgetItem("CPU Settings");
	QListWidgetItem *poolTab = new QListWidgetItem("Pool Settings");
	// The data needs to match the index of the widgets in the stackedWidget
	generalTab->setData(Qt::UserRole, QVariant::fromValue(0));
	cpuTab->setData(Qt::UserRole, QVariant::fromValue(1));
	poolTab->setData(Qt::UserRole, QVariant::fromValue(2));
	ui->listWidget->addItem(generalTab);
	ui->listWidget->addItem(cpuTab);
	ui->listWidget->addItem(poolTab);

	connect(ui->listWidget, &QListWidget::currentItemChanged, this, [this](QListWidgetItem *current){
		ui->stackedWidget->setCurrentIndex(current->data(Qt::UserRole).toInt());
	});

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() { updateConfigFile(); });
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [this]() { this->close(); });


	connect(networkManager, &QNetworkAccessManager::finished, this, &SettingsWindow::GetAvailablePools);

	QNetworkRequest request(QUrl("https://httpbin.org/get"));
	networkManager->get(request);
}

SettingsWindow::~SettingsWindow() {
	delete ui;
}

void SettingsWindow::showEvent(QShowEvent *event) {
	QDialog::showEvent(event);
	updateWidgets();
}

/*
 * Reads the state of every widget
 * and writes the changes to the file
 */
void SettingsWindow::updateConfigFile() {
	QJsonObject jsonConfig = readConfigFile(QStringLiteral("config.txt"));
	jsonConfig["h_print_time"] = ui->hashraterateReportFreqencySpinBox->value();


	writeConfigFile("config.txt", jsonConfig);
}

/*
 * reads the config file and updates the widget accordingly
 */
void SettingsWindow::updateWidgets() {
	QJsonObject jsonConfig = readConfigFile(QStringLiteral("config.txt"));
	ui->callTimeoutSpinBox->setValue(jsonConfig["call_timeout"].toInt());
	ui->hashraterateReportFreqencySpinBox->setValue(jsonConfig["h_print_time"].toInt());
}

QJsonObject SettingsWindow::readConfigFile(const QString &config) {
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

bool SettingsWindow::writeConfigFile(const QString &fileName, const QJsonObject& jsonConfig) {
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

QJsonObject SettingsWindow::GetAvailablePools(QNetworkReply *reply) {
	if (reply->error()) {
		qWarning() << __FILE__ <<  __LINE__ << Q_FUNC_INFO << reply->error();
	}
	QJsonObject jsonRoot = QJsonDocument::fromJson(reply->readAll()).object();
	reply->deleteLater();
	return jsonRoot;
}
