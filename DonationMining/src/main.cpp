//
// Created by jon on 30.03.20.
//

#include <iostream>

#include <QApplication>
#include <QtCore>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	MainWindow window;
	window.show();

	return QApplication::exec();
}
