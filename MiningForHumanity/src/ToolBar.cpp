//
// Created by jon on 25.05.20.
//

#include "ToolBar.hpp"

// todo: put the start/stop miner button and label instead of the language buttons

ToolBar::ToolBar(QWidget *parent) : QWidget(parent), ui(new Ui::uiToolBar) {
	ui->setupUi(this);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setFixedHeight(75);
	// the style sheet the the white background of this widget is set in MainWindow.ui

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

	connect(submenu->addAction(tr("Dashboard")), &QAction::triggered, [this]() { emit changePage(GLOBALS::Dashboard); });
	connect(submenu->addAction(tr("Ongoing campaigns")), &QAction::triggered, [this]() { emit changePage(GLOBALS::OngoingCampaigns); });
	connect(submenu->addAction(tr("Finished campaigns")), &QAction::triggered, []() {});
	connect(submenu->addAction(tr("My profile")), &QAction::triggered, []() {});
	connect(submenu->addAction(tr("My campaigns")), &QAction::triggered, []() {});
	submenu->addSeparator();
	connect(submenu->addAction(tr("Settings")), &QAction::triggered, [this]() { emit changePage(GLOBALS::Settings); });
	connect(submenu->addAction(tr("Contact")), &QAction::triggered, []() {});
	connect(submenu->addAction(tr("About MFH")), &QAction::triggered, []() {});
	connect(submenu->addAction(tr("Log out")), &QAction::triggered, [this]() { emit userLogOut(); });
	ui->pushButton->setMenu(submenu);

	// set the logo on the left of the toolbar
	QPixmap map(":/images/MFH_logo_2");
	QSize pixSize = map.size();
	pixSize.scale(QSize(50, 50), Qt::KeepAspectRatio);
	QPixmap scaledPix = map.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->logo->setPixmap(scaledPix);
}

ToolBar::~ToolBar() {
	delete ui;
}

void ToolBar::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	QStyleOption opt;
	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}