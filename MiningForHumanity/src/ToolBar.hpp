//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_TOOLBAR_H
#define MININGFORHUMANITY_TOOLBAR_H

#include <QWidget>
#include <QProcess>
#include "ui_ToolBar.h"
#include "LanguageButtons.h"

class ToolBar : public QWidget {

public:
	explicit ToolBar(QWidget *parent) : ui(new Ui::uiToolBar) {
		ui->setupUi(this);
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		setFixedHeight(75);

		// configure the menu button
		ui->pushButton->setIcon(QIcon(":/images/power_off_icon"));
		ui->pushButton->setIconSize(QSize(20, 20));
		ui->pushButton->setStyleSheet("QPushButton { background: transparent; outline: none; border: 0px solid black; border-radius: 10px; }"
							  "QPushButton::menu-indicator { width: 0px; }");

		QMenu *submenu = new QMenu(this);
		submenu->setStyleSheet("QMenu { color: black; background-color: white; border: 1px solid grey; outline : none; }"
							   "QMenu::item { height: 25px; width: 100px; }" // doesn't seem to work
							   "QMenu::item:selected { color: black; background-color: lightgrey; }"
							   "QMenu::item:pressed { color: white; background-color: grey; }"
							   "QMenu::separator { color: black; }");

		connect(submenu->addAction(tr("Sign out")), &QAction::triggered, [this]() {
//			LoginWidget::deleteRememberMeCookie();
//			showLoginScreen();
		});
		connect(submenu->addAction(tr("Minimize")), &QAction::triggered, [this]() { close(); });
		connect(submenu->addAction(tr("Quit")), &QAction::triggered, []() { QApplication::quit(); });
		ui->pushButton->setMenu(submenu);

		// set the logo on the left of the toolbar
		QPixmap map(":/images/MFH_logo_2");
		QSize pixSize = map.size();
		pixSize.scale(QSize(50, 50), Qt::KeepAspectRatio);
		QPixmap scaledPix = map.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		ui->logo->setPixmap(scaledPix);

		// set the country flags on the buttons
		connect(ui->languageButtons, &LanguageButtons::clicked, this, [this](QString rLanguage){
			QSettings settings;
			settings.setValue("main/language", rLanguage);
			QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Restart?"),
					tr("You must restart the program to change language. Restart now?"), QMessageBox::Yes | QMessageBox::No);

			if (reply == QMessageBox::Yes) {
				QApplication::quit();
				QProcess::startDetached(QApplication::arguments()[0], QApplication::arguments());
			}
		});
	}

	~ToolBar() override = default;

private:
	Ui::uiToolBar *ui;
	QTranslator m_translator;
	QString m_currLang;
};


#endif //MININGFORHUMANITY_TOOLBAR_H
