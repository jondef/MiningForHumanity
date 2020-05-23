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

	~Campaign() override = default;

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




#endif //MININGFORHUMANITY_CAMPAIGN_H
