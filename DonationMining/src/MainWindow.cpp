//
// Created by jon on 31.03.20.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::uiMainWindow) {
	ui->setupUi(this);
//	QCoreApplication::setOrganizationName("MySoft"); MiningForHumanity
//	QCoreApplication::setOrganizationDomain("mysoft.com");
//	QCoreApplication::setApplicationName("Star Runner");
	ui->checkBox_SSLSupport->setChecked(settings->value("editor/wrapMargin").toBool());

	connect(ui->checkBox_SSLSupport, &QCheckBox::toggled, this, [&](bool checked) {
		settings->setValue("editor/wrapMargin", checked);
	});

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [&]() {
		// save / connect to pools
		// minimise window
	});
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [&]() { this->close(); });
}

MainWindow::~MainWindow() {

}
