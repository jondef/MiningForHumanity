//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_RIBBON_HPP
#define MININGFORHUMANITY_RIBBON_HPP

#include <QWidget>
#include <QObject>
#include <QString>
#include <QLabel>
#include <QIcon>
#include <QStackedWidget>
#include <QHBoxLayout>
#include "CategoryLabel.hpp"
#include <QHBoxLayout>
#include <QGridLayout>
#include "ui_Ribbon.h"

class Ribbon : public QWidget {

public:
	explicit Ribbon(QWidget *parent) : ui(new Ui::uiRibbon) {
		ui->setupUi(this);
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		setFixedHeight(100);
	}

	~Ribbon() override {
		delete ui;
	}

public slots:

	void showLabel(const QString &text) {
		ui->stackedWidget->setCurrentIndex(0);
		ui->page0_title->setText(text);
	}

	void showCampaign(const QString &title, const QPixmap &pixmap, const QList<CategoryLabel::Category> &categories) {
		// delete the previously displayed categories
		for (CategoryLabel *i : labelList) { delete i; }
		labelList = QList<CategoryLabel *>();
		// update the widgets
		ui->stackedWidget->setCurrentIndex(1);
		ui->page1_title->setText(title);
		ui->page1_icon->setPixmap(pixmap);
		for (const CategoryLabel::Category &category : categories) {
			CategoryLabel *label = new CategoryLabel(category);
			labelList.append(label);
			ui->categoriesLayout->addWidget(label);
		}
	}

private:
	Ui::uiRibbon *ui;
	QList<CategoryLabel *> labelList;
};


#endif //MININGFORHUMANITY_RIBBON_HPP
