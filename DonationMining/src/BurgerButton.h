//
// Created by jon on 29.04.20.
//

#ifndef DONATIONMINING_BUGERBUTTON_H
#define DONATIONMINING_BUGERBUTTON_H

#include <QtCore>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QString>
#include <QStyleOption>
#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>
#include <QEvent>
#include <QPaintEvent>

static const QString BurgerButtonObjectName("BurgerButton");
static const QString BurgerMenuName("BurgerMenu");
static const QString MainBurgerButtonObjectName("MainBurgerButton");

class BurgerButton : public QPushButton {
Q_OBJECT
public:
	BurgerButton(QAction *action, QWidget *parent);

	void paintEvent(QPaintEvent *) override;

	void setIconSize(const QSize &size);

	bool event(QEvent *event) override;

	QAction *action() const;

	QHBoxLayout *getLayout();

	void setLayout(QHBoxLayout *layout);

	bool hovered = false;

private:
	QSize mIconSize;
	QAction *mAction;
	QHBoxLayout *layout;


signals:

	void mouseEnterButton(BurgerButton *button);

	void mouseLeaveButton(BurgerButton *button);


};


#endif //DONATIONMINING_BUGERBUTTON_H
