//
// Created by jon on 24.05.20.
//

#ifndef MININGFORHUMANITY_LANGUAGEBUTTONS_H
#define MININGFORHUMANITY_LANGUAGEBUTTONS_H

#include <QWidget>
#include <QString>
#include <QRadioButton>

class LanguageButtons : public QWidget {
Q_OBJECT
public:

	explicit LanguageButtons(QWidget *parent = nullptr);

	~LanguageButtons() override = default;

private slots:
	void changedLanguage(const QString &language);

public:
	QRadioButton *button_en;
	QRadioButton *button_fr;
	QRadioButton *button_de;
};


#endif //MININGFORHUMANITY_LANGUAGEBUTTONS_H
