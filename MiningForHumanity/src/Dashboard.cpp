//
// Created by jon on 25.05.20.
//

#include "Dashboard.hpp"
#include "AbstractTableModel.hpp"

Dashboard::Dashboard(QWidget *parent) : ui(new Ui::uiDashboard) {
	ui->setupUi(this);

	// populate the about tab in settings
	ui->label_VERSION->setText(VERSION);
	ui->label_BUILDDATE->setText(BUILDDATE);
	ui->label_GIT_COMMIT_DATE->setText(GIT_COMMIT_DATE);
	ui->label_GIT_COMMIT_HASH->setText(GIT_COMMIT_HASH);
	ui->label_GIT_BRANCH->setText(GIT_BRANCH);

	ui->tab_2->layout()->addWidget(settingsWindow);

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
			ui->pushButton_startStopMiner->setText(tr("Wait"));
			ui->label_minerState->setText(tr("Starting"));
			ui->label_minerState->setStyleSheet("border-radius: 10px;\n"
												"color: white;\n"
												"background-color: blue;\n"
												"padding: 2px 5px 2px 5px;");
		} else if (state == MinerManager::Mining) {
			ui->pushButton_startStopMiner->setDisabled(false);
			ui->pushButton_startStopMiner->setText(tr("Stop"));
			ui->label_minerState->setText(tr("Donating"));
			ui->label_minerState->setStyleSheet("border-radius: 10px;\n"
												"color: white;\n"
												"background-color: green;\n"
												"padding: 2px 5px 2px 5px;");
		} else if (state == MinerManager::NotMining) {
			ui->pushButton_startStopMiner->setDisabled(false);
			ui->pushButton_startStopMiner->setText(tr("Start"));
			ui->label_minerState->setText(tr("Inactive"));
			ui->label_minerState->setStyleSheet("border-radius: 10px;\n"
												"color: white;\n"
												"background-color: red;\n"
												"padding: 2px 5px 2px 5px;");
		}
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
}

Dashboard::~Dashboard() {
	delete ui;
}

void Dashboard::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	QStyleOption opt;
	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void Dashboard::changePage(int pageIndex) {
	ui->stackedWidget->setCurrentIndex(pageIndex);
}