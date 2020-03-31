//
// Created by jon on 31.03.20.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::uiMainWindow) {
	ui->setupUi(this);
	QString x("damn");
//	QCoreApplication::setOrganizationName("MySoft");
//	QCoreApplication::setOrganizationDomain("mysoft.com");
//	QCoreApplication::setApplicationName("Star Runner");
	ui->checkBox->setChecked(settings->value("editor/wrapMargin/checked").toBool());

	connect(ui->checkBox, &QCheckBox::toggled, this, [&](bool checked){
		settings->setValue("editor/wrapMargin/checked", checked);
	});
}

MainWindow::~MainWindow() {

}
