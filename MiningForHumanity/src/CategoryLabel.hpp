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
		Innovation,
		Energy,
		Media,
		Foundations,
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
				setStyleSheet(styleSheet() + "background-color: #4D4DC3;");
				setText(tr("Education"));
				break;
			case Environment:
				setStyleSheet(styleSheet() + "background-color: #3BB194;");
				setText(tr("Environment"));
				break;
			case Innovation:
				setStyleSheet(styleSheet() + "background-color: #1E7FC2;");
				setText(tr("Innovation"));
				break;
			case Energy:
				setStyleSheet(styleSheet() + "background-color: #BFBFBF;");
				setText(tr("Energy"));
				break;
			case Media:
				setStyleSheet(styleSheet() + "background-color: #82D6C0;");
				setText(tr("Media"));
				break;
			case Foundations:
				setStyleSheet(styleSheet() + "background-color: #A5B5A2;");
				setText(tr("Foundations"));
				break;
		}
	}
}; // class


#endif //MININGFORHUMANITY_CATEGORYLABEL_HPP
