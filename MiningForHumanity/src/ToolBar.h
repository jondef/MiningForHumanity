//
// Created by jon on 23.05.20.
//

#ifndef MININGFORHUMANITY_TOOLBAR_H
#define MININGFORHUMANITY_TOOLBAR_H

#include <QWidget>
#include <QProcess>
#include <QTranslator>
#include "ui_ToolBar.h"
#include <QMenu>
#include "LanguageButtons.h"

class ToolBar : public QWidget {

public:
	explicit ToolBar(QWidget *parent = nullptr);

	~ToolBar() override;


private:
	Ui::uiToolBar *ui;
	QTranslator m_translator;
	QString m_currLang;
};


#endif //MININGFORHUMANITY_TOOLBAR_H
