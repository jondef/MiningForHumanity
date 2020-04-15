//
// Created by jon on 11.04.20.
//

#ifndef DONATIONMINING_SETTINGSWINDOW_H
#define DONATIONMINING_SETTINGSWINDOW_H

#include "ui_SettingsWindow.h"
#include <QtCore>
#include <QObject>

class SettingsWindow : public QDialog {
Q_OBJECT

private:
	Ui::uiSettingsWindow *ui;

public:
	explicit SettingsWindow(QWidget *parent = nullptr);

	~SettingsWindow();

	QVariantHash readConfigFile(const QString &config);

	bool writeConfigFile(const QString &config, QVariantHash hash);

protected:
	void showEvent(QShowEvent* event);
};


#endif //DONATIONMINING_SETTINGSWINDOW_H
