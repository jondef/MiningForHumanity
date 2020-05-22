//
// Created by jon on 31.03.20.
//

#include "MainWindow.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>
#include "AbstractTableModel.h"

QChart *createSplineChart();

// todo: add reload information button next to shutdown button

MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) : QMainWindow(parent), ui(new Ui::uiMainWindow) {
	ui->setupUi(this);
	this->argc = argc;
	this->argv = argv;

	if (loginWindow->autoLogin()) {
		showDashboard(loginWindow->m_username);
	} else {
		showLoginScreen();
	}

	setWindowIcon(QIcon(":/images/icon"));
	QSettings settings;

	// populate the about tab in settings
	ui->label_VERSION->setText(VERSION);
	ui->label_BUILDDATE->setText(BUILDDATE);
	ui->label_GIT_COMMIT_DATE->setText(GIT_COMMIT_DATE);
	ui->label_GIT_COMMIT_HASH->setText(GIT_COMMIT_HASH);
	ui->label_GIT_BRANCH->setText(GIT_BRANCH);

	ui->tab_2->layout()->addWidget(settingsWindow);

	auto exitAction = new QAction(tr("&Exit"), this);
	connect(exitAction, &QAction::triggered, [this]() {
		closing = true;
		close();
	});

	auto trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(exitAction);

	auto sysTrayIcon = new QSystemTrayIcon(this);
	sysTrayIcon->setContextMenu(trayIconMenu);
	sysTrayIcon->setIcon(QIcon(":/images/icon"));
	sysTrayIcon->setToolTip(tr("MiningForHumanity"));
	sysTrayIcon->show();

	connect(sysTrayIcon, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason) {
		if (reason == QSystemTrayIcon::Trigger) {
			if (isVisible()) {
				hide();
			} else {
				show();
				activateWindow();
			}
		}
	});
	connect(loginWindow, &LoginWidget::userAuthorized, this, &MainWindow::showDashboard);

	// * miner start / stop connections
	connect(ui->pushButton_startStopMiner, &QPushButton::clicked, this, [this]() {
		if (settingsWindow->getMinerState() == MinerManager::NotMining) {
			settingsWindow->startMiner();
		} else if (settingsWindow->getMinerState() == MinerManager::Mining) {
			settingsWindow->stopMiner();
		}
	});
	connect(settingsWindow, &MinerManager::minerChangedState, this, [this](MinerManager::MinerState state) {
		if (state == MinerManager::Starting) {
			ui->pushButton_startStopMiner->setDisabled(true);
			ui->pushButton_startStopMiner->setText("Wait");
			ui->label_minerState->setText("Starting");
			ui->label_minerState->setStyleSheet("border-radius: 10px;\n"
												"color: white;\n"
												"background-color: blue;\n"
												"padding: 2px 5px 2px 5px;");
		} else if (state == MinerManager::Mining) {
			ui->pushButton_startStopMiner->setDisabled(false);
			ui->pushButton_startStopMiner->setText("Stop");
			ui->label_minerState->setText("Donating");
			ui->label_minerState->setStyleSheet("border-radius: 10px;\n"
												"color: white;\n"
												"background-color: green;\n"
												"padding: 2px 5px 2px 5px;");
		} else if (state == MinerManager::NotMining) {
			ui->pushButton_startStopMiner->setDisabled(false);
			ui->pushButton_startStopMiner->setText("Start");
			ui->label_minerState->setText("Inactive");
			ui->label_minerState->setStyleSheet("border-radius: 10px;\n"
												"color: white;\n"
												"background-color: red;\n"
												"padding: 2px 5px 2px 5px;");
		}
	});


	// toolbar
	QPushButton *button = new QPushButton;
	button->setIcon(QIcon(":/images/power_off_icon"));
	button->setIconSize(QSize(20, 20));
	button->setStyleSheet("QPushButton { background: transparent; outline: none; border: 0px solid black; border-radius: 10px; }"
						  "QPushButton::menu-indicator { width: 0px; }");

	QMenu *submenu = new QMenu(this);
	submenu->setStyleSheet("QMenu { color: black; background-color: white; border: 1px solid grey; outline : none; }"
						   "QMenu::item { height: 25px; width: 100px; }" // doesn't seem to work
						   "QMenu::item:selected { color: black; background-color: lightgrey; }"
						   "QMenu::item:pressed { color: white; background-color: grey; }"
						   "QMenu::separator { color: black; }");

	connect(submenu->addAction(tr("Sign out")), &QAction::triggered, [this]() {
		LoginWidget::deleteRememberMeCookie();
		showLoginScreen();
	});
	connect(submenu->addAction(tr("Minimize")), &QAction::triggered, [this]() { close(); });
	connect(submenu->addAction(tr("Quit")), &QAction::triggered, []() { qApp->quit(); });
	button->setMenu(submenu);

	QHBoxLayout *lay = static_cast<QHBoxLayout *>(ui->toolBar->layout());
	lay->insertWidget(lay->count() + 1, button);

	// BURGERMENU
	BurgerMenu *menu = new BurgerMenu();
	ui->BurgermenuWidget->layout()->addWidget(menu);

	menu->setMenuWidth(100);
	menu->setBurgerIcon(QIcon(":/icons/burger/burger"));
	menu->addMenuAction(QIcon(":/icons/burger/collections"), "Dashboard");
	menu->addMenuAction(QIcon(":/icons/burger/albums"), "Projects");
	menu->addStretch();
	menu->addMenuAction(QIcon(":/icons/burger/twitter_icon"), "Twitter");
	menu->addMenuAction(QIcon(":/icons/burger/facebook_icon"), "Facebook");
	menu->addMenuAction(QIcon(":/icons/burger/instagram_icon"), "Instagram");
	menu->addStretch();
	menu->addMenuAction(QIcon(":/icons/burger/settings_icon"), "Settings");

#define BURGERMENU_BACKGROUND_COLOR "#1B2430"

	setStyleSheet("BurgerMenu                  { background-color: " BURGERMENU_BACKGROUND_COLOR ";   }"
				  "#BurgerMenu                 { background-color: " BURGERMENU_BACKGROUND_COLOR ";   }"
				  "#BurgerButton               { background-color: " BURGERMENU_BACKGROUND_COLOR ";   color: white; font-size: 18px; }" // buttons
				  "#BurgerButton:hover         { background-color: #3480D2; }"
				  "#BurgerButton:checked       { background-color: #106EBE; }"
				  "#BurgerButton:checked:hover { background-color: #3480D2; }"
				  "#MainBurgerButton           { background-color: " BURGERMENU_BACKGROUND_COLOR ";   border: none; } "
				  "#MainBurgerButton:hover     { background-color: #333;    } "
	);

	connect(menu, &BurgerMenu::triggered, [this, menu](QAction *action) {
		uint8_t index = menu->actions().indexOf(action);
		ui->stackedWidget->setCurrentIndex(index);
	});

	// PROJECT TAB
	AbstractTableModel *model = new AbstractTableModel();
	ui->tableView_projectList->setModel(model);
	ui->tableView_projectList->setColumnHidden(0, true);

	connect(ui->pushButton_projectConfirm, &QPushButton::clicked, this, [this]() {
		QItemSelectionModel *select = ui->tableView_projectList->selectionModel();
		if (select->hasSelection()) {
			qDebug() << select->selectedRows(0).at(0).data();
		}
	});


	//////////////////////////////////

	//<editor-fold desc="Graph">
	// Assign names to the set of bars used
	QBarSet *set0 = new QBarSet("Altuve");


	// Assign values for each bar
	*set0 << 283 << 341 << 313 << 338 << 346 << 335;


	// Add all sets of data to the chart as a whole
	// 1. Bar Chart
	QBarSeries *series = new QBarSeries();

	// 2. Stacked bar chart
	// QHorizontalStackedBarSeries *series = new QHorizontalStackedBarSeries();
	series->append(set0);

	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle("Funds Collected by Day");
	chart->setAnimationOptions(QChart::SeriesAnimations);

	QStringList categories;
	categories << "2013" << "2014" << "2015" << "2016" << "2017" << "2018";

	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	chart->createDefaultAxes();

	chart->setAxisX(axis, series);

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	chart->setBackgroundVisible(false);

	QChart *splineChart = createSplineChart();
	QChartView *chartView = new QChartView(splineChart);
	chartView->setRenderHint(QPainter::Antialiasing);

	/*// Used to change the palettes
	QPalette pal = qApp->palette();

	// Change the color around the chart widget and text
	pal.setColor(QPalette::Window, QRgb(0xffffff));
	pal.setColor(QPalette::WindowText, QRgb(0x404044));

	// Apply palette changes to the application
	qApp->setPalette(pal);*/

	ui->chartsLayout->addWidget(chartView, 0, 1, 1, 1);
	//</editor-fold>
}

MainWindow::~MainWindow() {
	delete ui;
}

//<editor-fold desc="Graph">
DataTable generateRandomData(int listCount, int valueMax, int valueCount) {
	DataTable dataTable;

	// generate random data
	for (int i(0); i < listCount; i++) {
		DataList dataList;
		qreal yValue(0);
		for (int j(0); j < valueCount; j++) {
			yValue = yValue + QRandomGenerator::global()->bounded(valueMax / (qreal) valueCount);
			QPointF value((j + QRandomGenerator::global()->generateDouble()) * ((qreal) 10 / (qreal) valueCount), yValue);
			QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
			dataList << Data(value, label);
		}
		dataTable << dataList;
	}
	return dataTable;
}

QChart *createSplineChart() {
	QChart *chart = new QChart();
	chart->setTitle("Spline chart");
	QString name("Series ");
	int nameIndex = 0;
	for (const DataList &list : generateRandomData(3, 10, 7)) {
		QSplineSeries *series = new QSplineSeries(chart);
		for (const Data &data : list)
			series->append(data.first);
		series->setName(name + QString::number(nameIndex));
		nameIndex++;
		chart->addSeries(series);
	}

	chart->createDefaultAxes();
	chart->axes(Qt::Horizontal).first()->setRange(0, 10);
	chart->axes(Qt::Vertical).first()->setRange(0, 7);

	// Add space to label to add space between labels and axis
	QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).first());
	Q_ASSERT(axisY);
	axisY->setLabelFormat("%.1f  ");
	return chart;
}
//</editor-fold>

void MainWindow::showEvent(QShowEvent *event) {
	// If the user opens the app for the first time,
	// ask him to register / login
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (closing) {
		event->accept();
	} else {
		this->hide();
		event->ignore();
	}
}

/*
 * This function hides the login widget and shows the dashboard
 */
void MainWindow::showDashboard(const QString &username) {
	if (centralWidget != nullptr) {
		takeCentralWidget(); // don't need to take ownership of the login widget because it is already a member
		setCentralWidget(centralWidget);
	}
	updateDashboard(username);
}

/*
 * Updates the widgets on the dashboard: username, date, etc...
 */
void MainWindow::updateDashboard(const QString &username) {
	ui->label_welcomeBack->setText("Welcome back, " + username);
	ui->label_date->setText(QDateTime::currentDateTime().date().toString());
}

/*
 * This function must also be used when logging the
 * user out
 */
void MainWindow::showLoginScreen() {
	centralWidget = takeCentralWidget();
	setCentralWidget(loginWindow);
}


