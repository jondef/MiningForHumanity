//
// Created by jon on 24.05.20.
//

#include "LanguageButtons.hpp"
#include <QHBoxLayout>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>

#define STYLESHEET(flag) \
"QRadioButton::indicator::unchecked { image: url(" flag "); }" \
"QRadioButton::indicator::checked { image: url(" flag "); }"
//"QRadioButton::indicator:unchecked:hover { image: url(:/icons/countryFlags/en); }"

LanguageButtons::LanguageButtons(QWidget *parent) : button_en(new QRadioButton), button_fr(new QRadioButton), button_de(new QRadioButton) {

	button_fr->setStyleSheet(STYLESHEET(":/icons/countryFlags/fr"));
	button_en->setStyleSheet(STYLESHEET(":/icons/countryFlags/en"));
	button_de->setStyleSheet(STYLESHEET(":/icons/countryFlags/de"));

	connect(button_en, &QRadioButton::clicked, this, [this]() { changedLanguage("en"); });
	connect(button_fr, &QRadioButton::clicked, this, [this]() { changedLanguage("fr"); });
	connect(button_de, &QRadioButton::clicked, this, [this]() { changedLanguage("de"); });

	setLayout(new QHBoxLayout);
	layout()->setContentsMargins(0, 0, 0, 0);
	layout()->setSpacing(0);
	layout()->addWidget(button_en);
	layout()->addWidget(button_fr);
	layout()->addWidget(button_de);
}

void LanguageButtons::changedLanguage(const QString &language) {
	QSettings settings;
	settings.setValue("main/language", language);
	QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Restart?"),
			tr("You must restart the program to change language. Restart now?"), QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		QApplication::exit(1337);
	}
}
