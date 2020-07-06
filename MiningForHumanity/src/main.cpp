//
// Created by jon on 30.03.20.
//

#include <QApplication>
#include <QtCore>

#if (defined(_WIN32) || defined(WIN32)) && defined(QT_STATICPLUGIN)

#include <QtPlugin>

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

Q_IMPORT_PLUGIN(QGenericEnginePlugin)

Q_IMPORT_PLUGIN(QICOPlugin)

Q_IMPORT_PLUGIN(QJpegPlugin)

Q_IMPORT_PLUGIN(QGifPlugin)

#endif

#include "MainWindow.hpp"


int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	// these are used for qSettings
	QCoreApplication::setOrganizationName("MiningForHumanity");
	QCoreApplication::setOrganizationDomain("miningforhumanity.org");
	QCoreApplication::setApplicationName("MiningForHumanity");

	// set application style
	qDebug() << QStyleFactory::keys();
	qDebug() << "Plugins loaded from: " << QCoreApplication::libraryPaths();
	QApplication::setStyle(QStyleFactory::create("Fusion"));

	uint16_t result;
	do {
		// read language string from settings and load the translations
		QSettings settings;
		QString language = settings.value("main/language").toString();
		if (language.isEmpty()) { language = "en"; settings.setValue("main/language", language); }
		QTranslator myappTranslator;
		myappTranslator.load(QString(":/translations/MiningForHumanity_%1.qm").arg(language));
		if (myappTranslator.isEmpty()) { qDebug() << "Failed to load language file:" << language; }
		QApplication::installTranslator(&myappTranslator);

		MainWindow window;
		window.show();
		result = QCoreApplication::exec();
	} while(result == 1337);

	return result;
}
