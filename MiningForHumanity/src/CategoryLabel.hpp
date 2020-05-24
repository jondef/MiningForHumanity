//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_CATEGORYLABEL_HPP
#define MININGFORHUMANITY_CATEGORYLABEL_HPP

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QDebug>

class CategoryLabel : public QLabel {
//Q_OBJECT // you need to have a cpp file to use QObject

public:
	enum Category {
		Education,
		Environment,
		Innovation
	};

public:
	explicit CategoryLabel(QWidget *parent = nullptr) {
		setStyleSheet("color: white; padding: 0px 10px 0px 10px;");
	}

	explicit CategoryLabel(Category category, QWidget *parent = nullptr) {
		setStyleSheet("color: white; padding: 0px 10px 0px 10px;");
		setCategory(category);
	}

	~CategoryLabel() override = default;

public slots:
	void setCategory(Category category) {
		switch (category) {
			case Education:
				setStyleSheet(styleSheet() + "background-color: #353AAE;");
				setText(tr("Education"));
				break;
			case Environment:
				setStyleSheet(styleSheet() + "background-color: #4C9E7E;");
				setText(tr("Environment"));
				break;
			case Innovation:
				setStyleSheet(styleSheet() + "background-color: #3167A0;");
				setText(tr("Innovation"));
				break;
		}
	}
}; // class


#endif //MININGFORHUMANITY_CATEGORYLABEL_HPP
