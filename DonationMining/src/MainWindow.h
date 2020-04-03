//
// Created by jon on 31.03.20.
//

#ifndef DONATIONMINING_MAINWINDOW_H
#define DONATIONMINING_MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QObject>
#include <QSettings>
#include "ui_MainWindow.h"

#include "xmrstak/backend/backendConnector.hpp"
#include "xmrstak/backend/globalStates.hpp"
#include "xmrstak/backend/miner_work.hpp"
#include "xmrstak/jconf.hpp"
#include "xmrstak/misc/configEditor.hpp"
#include "xmrstak/misc/console.hpp"
#include "xmrstak/misc/executor.hpp"
#include "xmrstak/misc/utility.hpp"
#include "xmrstak/params.hpp"
#include "xmrstak/version.hpp"

#ifndef CONF_NO_HTTPD

#include "xmrstak/http/httpd.hpp"

#endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

#ifndef CONF_NO_TLS

#include <openssl/err.h>
#include <openssl/ssl.h>

#endif

#ifdef _WIN32
#define strcasecmp _stricmp
#include "xmrstak/misc/uac.hpp"
#include <windows.h>
#endif // _WIN32


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow();

private:
	Ui::uiMainWindow *ui;
	QSettings *settings = new QSettings("MySoft", "Star Runner");
};


#endif //DONATIONMINING_MAINWINDOW_H
