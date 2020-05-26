//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_CAMPAIGN_H
#define MININGFORHUMANITY_CAMPAIGN_H

#include <QString>
#include <QDate>
#include <QImage>


class Campaign {

public:
	explicit Campaign() {

	}

	~Campaign() = default;

private:
	QString title;
	QString campaignLongDescription;
	int currentDonators;
	QImage image;
	QColor campaignColorTheme;
	QDate expiryDate;
	int monetaryCurrent;
	int monetaryObjective;

	QString campaignShortDescription;
	QString associationCreationDate;
	QUrl associationWebsite;
};


class CampaignDisplay : public QWidget {
public:
	explicit CampaignDisplay(QWidget *parent = nullptr) : QWidget(parent), campaign(new Campaign), mainWidget(new QWidget), btn(new QPushButton("Go to campaign", this)) {
		setLayout(new QVBoxLayout);
		layout()->setContentsMargins(9, 9, 9, 9);
		layout()->setSpacing(0);

		mainWidget->setLayout(new QVBoxLayout);
		mainWidget->layout()->addWidget(new QLabel("fuckfuck"));
		mainWidget->setObjectName("mainWidget");
		mainWidget->setStyleSheet("#mainWidget { border: 2px solid white; } "
								  "* { background-color: #F39B86; } ");
		layout()->addWidget(mainWidget);

		btn->setStyleSheet("background-color: #26348B; color: white;");

		QVBoxLayout *spacingBelow = new QVBoxLayout();
		layout()->setContentsMargins(0, 0, 0, btn->height() / 2);
		layout()->setSpacing(0);
		layout()->addItem(spacingBelow);
	}

protected:
	void paintEvent(QPaintEvent *event) override {
		QWidget::paintEvent(event);
		btn->setParent(parentWidget());
		QPoint widgetPos = mapToParent(mainWidget->rect().bottomRight());
		btn->setGeometry(widgetPos.x() - btn->width() - 10, widgetPos.y() - btn->height() / 2,btn->width(), btn->height());
		btn->show();
	}

private:
	Campaign *campaign;
	QWidget *mainWidget;

public:
	QPushButton *btn;
};


#endif //MININGFORHUMANITY_CAMPAIGN_H
