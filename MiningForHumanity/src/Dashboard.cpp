//
// Created by jon on 25.05.20.
//

#include "Dashboard.hpp"
#include "AbstractTableModel.hpp"
#include "globals.hpp"


Dashboard::Dashboard(QWidget *parent) : QWidget(parent), ui(new Ui::uiDashboard) {
	ui->setupUi(this);

	Campaign *what = new Campaign;
	Campaign *whatt = new Campaign;
	Campaign *whattt = new Campaign;

	dynamic_cast<QVBoxLayout *>(ui->widget_ongoingCampaigns->layout())->insertWidget(1, new CampaignDisplay(what));
	dynamic_cast<QVBoxLayout *>(ui->widget_endedCampaigns->layout())->insertWidget(1, new CampaignDisplay(whattt));

	whatt->associationName = "UNICEF";
	whatt->associationDescription = "Des millions d’enfants sont en danger La Commission économique et sociale des Nations unies pour l’Asie occidentale";
	whatt->associationCreationDate = QDate(2000, 10, 24);
	whatt->associationWebsite = QUrl("https://www.unicef.fr/");
	whatt->associationLogo = QPixmap(50, 50);
	whatt->associationCategories = { CategoryLabel::Innovation, CategoryLabel::Environment };
	whatt->campaignName = "Fight Corona!";
	whatt->campaignDescription = "821 millions de personnes souffrent actuellement de la faim dans le monde et le Programme alimentaire mondial estime que la pandémie de Covid-19 pourrait pousser « au bord de la famine » 130 millions de personnes supplémentaires. Au Yémen, la situation pourrait devenir catastrophique pour les enfants et familles les plus vulnérables. On estime que 2 millions d’enfants yéménites souffrent de malnutrition. Parmi ces derniers, 360 000 enfants de moins de 5 ans souffrent d’une malnutrition aigüe et doivent se battre chaque jour pour survivre. Pour eux, ainsi que pour les 1,2 million de femmes enceintes ou allaitantes malnutries et les millions de familles vivant dans une précarité extrême, l’arrivée de cas de Covid-19 au Yémen pourrait avoir des conséquences dévastatrices. Des millions d’enfants sont en danger La Commission économique et sociale des Nations unies pour l’Asie occidentale (UNESCWA) estime que 1,7 million d’emplois seraient perdus dans la région du Moyen-Orient et de l’Afrique du Nord en raison de la fermeture de la plupart des entreprises, de la suspension des salaires et du blocage quasi-total des marchés";
	whatt->campaignExpirationDate = QDate(2020, 7, 10);
	whatt->campaignCurrentDonators = 25;
	whatt->campaignImage = QPixmap(200, 200);
	whatt->campaignColorTheme = QColor(255, 255, 0);
	whatt->monetaryByUser = 1;
	whatt->monetaryCurrent = 1242;
	whatt->monetaryObjective = 2500;

	CampaignDisplay *camp = new CampaignDisplay(whatt);

	dynamic_cast<QVBoxLayout *>(ui->widget_ongoingCampaigns->layout())->insertWidget(1, camp);
	connect(camp->btn, &QPushButton::clicked, this, [this, camp](){ showCampaignInfo(camp->campaign); });

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

/**
 * This function shows the campaign info page and updates
 * all the widgets accordingly
 */
void Dashboard::showCampaignInfo(Campaign *campaign) {
	// todo: implement a back button to return to the dashboard
	changePage(1);
	ribbonSetCampaign(campaign->associationName, campaign->associationLogo, campaign->associationCategories);

	ui->label_campaignTitle->setText(campaign->campaignName);
	ui->label_campaignExpirationDate->setText(tr("Until the ") + campaign->campaignExpirationDate.toString());
	ui->label_currentDonators->setText(tr("Current donators: ") + QString::number(campaign->campaignCurrentDonators));

	ui->widget_campaignImage->setPixmap(campaign->campaignImage);

	ui->label_aboutAssociation->setText(tr("About ") + campaign->associationName);
	ui->label_descriptionAssociation->setText(tr("Description: ") + campaign->associationDescription);
	ui->label_creationDateAssociation->setText(tr("Created on ") + campaign->associationCreationDate.toString());
	ui->label_websiteAssociation->setText(tr("Website: ") + "<a href=\"" + campaign->associationWebsite.toString() + "\"><span style=\"color:white;\">" + campaign->associationWebsite.toString() + "</span></a>");

	ui->label_campaignDescription->setText(campaign->campaignDescription);

	ui->label_monetaryInfo->setText(QString::number(campaign->monetaryCurrent) + " / " + QString::number(campaign->monetaryObjective) + "$");
}