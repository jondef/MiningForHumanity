//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_CAMPAIGN_H
#define MININGFORHUMANITY_CAMPAIGN_H

#include <QString>
#include <QDate>
#include <QImage>
#include "CategoryLabel.hpp"


class Campaign {

public:
	explicit Campaign() {

	}

	~Campaign() = default;

public:
	QString associationName;
	QString associationDescription;
	QDate associationCreationDate;
	QUrl associationWebsite;
	QPixmap associationLogo;
	QList<CategoryLabel::Category> associationCategories;

	QString campaignName;
	QString campaignDescription;
	QDate campaignExpirationDate;
	unsigned int campaignCurrentDonators;
	QPixmap campaignImage;
	QColor campaignColorTheme;
	unsigned int monetaryByUser; // amount mined by user todo: maybe remove?
	unsigned int monetaryCurrent;
	unsigned int monetaryObjective;

};

/*
 * Note: connections to the button are not managed by this class.
 * It is public for this reason.
 */
class CampaignDisplay : public QWidget {
public:
	explicit CampaignDisplay(Campaign *aCampaign, QWidget *parent = nullptr) : QWidget(parent), btn(new QPushButton("Go to campaign", this)) {
		campaign = aCampaign;
		setLayout(new QVBoxLayout);
		layout()->setContentsMargins(9, 9, 9, 9);
		layout()->setSpacing(0);

		mainWidget->setLayout(new QVBoxLayout);
		mainWidget->layout()->addWidget(line_1);
		mainWidget->layout()->addWidget(line_2);
		mainWidget->layout()->addWidget(line_3);
		mainWidget->setObjectName("mainWidget");
		mainWidget->setStyleSheet("#mainWidget { border: 2px solid white; } "
								  "* { background-color: #F39B86; } ");
		layout()->addWidget(mainWidget);

		btn->setStyleSheet("background-color: #26348B; color: white;");

		QVBoxLayout *spacingBelow = new QVBoxLayout();
		layout()->setContentsMargins(0, 0, 0, btn->height() / 2);
		layout()->setSpacing(0);
		layout()->addItem(spacingBelow);

		updateLabels();
	}

	~CampaignDisplay() override {
		delete campaign;
	}

protected:
	void paintEvent(QPaintEvent *event) override {
		QWidget::paintEvent(event);
		btn->setParent(parentWidget());
		QPoint widgetPos = mapToParent(mainWidget->rect().bottomRight());
		btn->setGeometry(widgetPos.x() - btn->width() - 10, widgetPos.y() - btn->height() / 2,btn->width(), btn->height());
		btn->show();
	}

public slots:
	void updateLabels() {
		line_1->setText(campaign->campaignName + tr("(until ") + campaign->campaignExpirationDate.toString() + ")");
		line_2->setText(tr("Funds collected by you: ") + campaign->monetaryByUser + "$");
		line_3->setText(tr("Funds collected: ") + campaign->monetaryCurrent + " / " + campaign->monetaryObjective + "$");
	}

private:
	QWidget *mainWidget = new QWidget();
	QLabel *line_1 = new QLabel();
	QLabel *line_2 = new QLabel();
	QLabel *line_3 = new QLabel();

public:
	Campaign *campaign;
	QPushButton *btn;
};


#endif //MININGFORHUMANITY_CAMPAIGN_H
