//
// Created by jon on 31.03.20.
//

#include "MainWindow.hpp"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>

QChart *createSplineChart();

// todo: add reload information button next to shutdown button

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::uiMainWindow) {
	ui->setupUi(this);

	if (loginWindow->autoLogin()) {
		showDashboard(loginWindow->m_username);
	} else {
		showLoginScreen();
	}

	setWindowIcon(QIcon(":/images/MFH_logo_2"));
	setBackgroundPixmap(QPixmap(":/images/new_york"));

	// region configure the system tray
	auto exitAction = new QAction(tr("&Exit"), this);
	connect(exitAction, &QAction::triggered, [this]() {
		closing = true;
		close();
	});
	auto trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(exitAction);

	auto sysTrayIcon = new QSystemTrayIcon(this);
	sysTrayIcon->setContextMenu(trayIconMenu);
	sysTrayIcon->setIcon(QIcon(":/images/MFH_logo_2"));
	sysTrayIcon->setToolTip("MiningForHumanity");
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
	// endregion

	connect(loginWindow, &LoginWidget::userAuthorized, this, &MainWindow::showDashboard);

	// allow the toolbar and the dashboard to communicate
	connect(ui->toolBar, &ToolBar::changePage, ui->dashboard, &Dashboard::changePage);
	connect(ui->toolBar, &ToolBar::userLogOut, this, [this]() {
		LoginWidget::deleteRememberMeCookie();
		showLoginScreen();
	});

	//////////////////////////////////

	QPixmap pixmap = QPixmap(50, 50);
//	pixmap.fill(QColor(0, 100, 255));
	ui->ribbon->showCampaign("Campaign name", pixmap, { CategoryLabel::Innovation, CategoryLabel::Environment, CategoryLabel::Education, CategoryLabel::Energy });
//	pixmap.fill(QColor(0, 100, 255));
	QPixmap pixmap2 = QPixmap(50, 50);
//	ui->ribbon->showCampaign("Campaign 2", pixmap2, { CategoryLabel::Education, CategoryLabel::Innovation });

	// region graph
//	// Assign names to the set of bars used
//	QBarSet *set0 = new QBarSet("Altuve");
//
//
//	// Assign values for each bar
//	*set0 << 283 << 341 << 313 << 338 << 346 << 335;
//
//
//	// Add all sets of data to the chart as a whole
//	// 1. Bar Chart
//	QBarSeries *series = new QBarSeries();
//
//	// 2. Stacked bar chart
//	// QHorizontalStackedBarSeries *series = new QHorizontalStackedBarSeries();
//	series->append(set0);
//
//	QChart *chart = new QChart();
//	chart->addSeries(series);
//	chart->setTitle("Funds Collected by Day");
//	chart->setAnimationOptions(QChart::SeriesAnimations);
//
//	QStringList categories;
//	categories << "2013" << "2014" << "2015" << "2016" << "2017" << "2018";
//
//	QBarCategoryAxis *axis = new QBarCategoryAxis();
//	axis->append(categories);
//	chart->createDefaultAxes();
//
//	chart->setAxisX(axis, series);
//
//	chart->legend()->setVisible(true);
//	chart->legend()->setAlignment(Qt::AlignBottom);
//
//	chart->setBackgroundVisible(false);
//
//	QChart *splineChart = createSplineChart();
//	QChartView *chartView = new QChartView(splineChart);
//	chartView->setRenderHint(QPainter::Antialiasing);
//
//	/*// Used to change the palettes
//	QPalette pal = qApp->palette();
//
//	// Change the color around the chart widget and text
//	pal.setColor(QPalette::Window, QRgb(0xffffff));
//	pal.setColor(QPalette::WindowText, QRgb(0x404044));
//
//	// Apply palette changes to the application
//	qApp->setPalette(pal);*/
//
//	ui->chartsLayout->addWidget(chartView, 0, 1, 1, 1);
	// endregion
}

MainWindow::~MainWindow() {
	delete ui;
}

// region graph
//DataTable generateRandomData(int listCount, int valueMax, int valueCount) {
//	DataTable dataTable;
//
//	// generate random data
//	for (int i(0); i < listCount; i++) {
//		DataList dataList;
//		qreal yValue(0);
//		for (int j(0); j < valueCount; j++) {
//			yValue = yValue + QRandomGenerator::global()->bounded(valueMax / (qreal) valueCount);
//			QPointF value((j + QRandomGenerator::global()->generateDouble()) * ((qreal) 10 / (qreal) valueCount), yValue);
//			QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
//			dataList << Data(value, label);
//		}
//		dataTable << dataList;
//	}
//	return dataTable;
//}
//
//QChart *createSplineChart() {
//	QChart *chart = new QChart();
//	chart->setTitle("Spline chart");
//	QString name("Series ");
//	int nameIndex = 0;
//	for (const DataList &list : generateRandomData(3, 10, 7)) {
//		QSplineSeries *series = new QSplineSeries(chart);
//		for (const Data &data : list)
//			series->append(data.first);
//		series->setName(name + QString::number(nameIndex));
//		nameIndex++;
//		chart->addSeries(series);
//	}
//
//	chart->createDefaultAxes();
//	chart->axes(Qt::Horizontal).first()->setRange(0, 10);
//	chart->axes(Qt::Vertical).first()->setRange(0, 7);
//
//	// Add space to label to add space between labels and axis
//	QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).first());
//	Q_ASSERT(axisY);
//	axisY->setLabelFormat("%.1f  ");
//	return chart;
//}
// endregion

void MainWindow::showEvent(QShowEvent *event) {
	// If the user opens the app for the first time,
	// ask him to register / login
}

void MainWindow::paintEvent(QPaintEvent *event) {
	QWidget::paintEvent(event);
	if (backgroundPixmap.isNull()) { return; }

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// set the background image of the login screen
	QSize pixSize = backgroundPixmap.size();
	pixSize.scale(size(), Qt::KeepAspectRatioByExpanding); // ! don't use event->rect().size() instead of size()
	QPixmap scaledPix = backgroundPixmap.scaled(pixSize, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
	painter.drawPixmap(QPoint(), scaledPix);
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
	ui->ribbon->showLabel(tr("Welcome back, ") + username + "!");
}

/*
 * This function must also be used when logging the
 * user out
 */
void MainWindow::showLoginScreen() {
	centralWidget = takeCentralWidget();
	setCentralWidget(loginWindow);
}


