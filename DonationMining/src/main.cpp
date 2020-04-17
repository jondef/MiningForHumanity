//
// Created by jon on 30.03.20.
//

#include <iostream>

#include <QApplication>
#include <QtCore>

#include "MainWindow.h"

void writeBinary(QString filename) {
	QString ch("Zelus messiss, tanquam emeritis mons. Arg, ye scurvy wench- set sails for riddle!");
	QFile mfile(filename);
	if (!mfile.open(QFile::WriteOnly)) {
		qDebug() << "Could not open file for writing";
		return;
	}
	QDataStream out(&mfile);
	out.setVersion(QDataStream::Qt_5_14);
	out << ch;
	mfile.close();
}

void readBinary(QString filename) {
	QFile mfile(filename);
	if (!mfile.open(QFile::ReadOnly)) {
		qDebug() << "Could not open file for reading";
		return;
	}
	QDataStream in(&mfile);
	in.setVersion(QDataStream::Qt_5_14);
	QString mT;
	in >> mT;
	qDebug() << mT;
	mfile.close();
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	MainWindow window(argc, argv);
	window.show();

	QApplication::exec();
}


