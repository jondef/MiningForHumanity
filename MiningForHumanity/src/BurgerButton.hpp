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

static const QString MainBurgerButtonObjectName("MainBurgerButton");
static const QString BurgerButtonObjectName("BurgerButton");
static const QString BurgerMenuName("BurgerMenu");

class BurgerButton : public QPushButton {
Q_OBJECT
public:
	BurgerButton(QAction *action, QWidget *parent);

	void paintEvent(QPaintEvent *) override;

	void setIconSize(const QSize &size);

	bool event(QEvent *event) override;

	QAction *action() const;

	QHBoxLayout *getLayout() const;

	void setLayout(QHBoxLayout *layout);

signals:

	void mouseEnterButton(BurgerButton *button);

	void mouseLeaveButton(BurgerButton *button);

private:
	QSize mIconSize;
	QAction *mAction;
	QHBoxLayout *layout;
	bool hovered;
};


#endif //DONATIONMINING_BUGERBUTTON_H
