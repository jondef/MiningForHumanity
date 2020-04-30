#ifndef DONATIONMINING_BURGERMENU_H
#define DONATIONMINING_BURGERMENU_H

#include <QWidget>
#include <QList>
#include <QSize>
#include <QIcon>
#include "BurgerButton.h"

class QPushButton;

class QActionGroup;

class QAction;

class QString;

class BurgerMenu : public QWidget {
Q_OBJECT
	Q_PROPERTY(QIcon icon READ burgerIcon WRITE setBurgerIcon NOTIFY iconChanged)
	Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
	Q_PROPERTY(int menuWidth READ menuWidth WRITE setMenuWidth NOTIFY menuWidthChanged)
	Q_PROPERTY(bool animated READ animated WRITE setAnimated NOTIFY animatedChanged)
	Q_PROPERTY(bool expanded READ expanded WRITE setExpanded NOTIFY expandedChanged)

public:
	BurgerMenu(QWidget *parent = nullptr);

	QIcon burgerIcon() const;

	QSize iconSize() const;

	int menuWidth() const;

	QList<QAction *> actions() const;

	bool animated() const;

	bool expanded() const;


signals:

	void mouseEnter(BurgerButton *button);

	void iconChanged() const;

	void iconSizeChanged(const QSize &size) const;

	void menuWidthChanged(int width) const;

	void animatedChanged(bool animated) const;

	void expandedChanged(bool expanded) const;

	void triggered(QAction *action) const;

public slots:

	QAction *addMenuAction(QAction *action, bool top = true);

	QAction *addMenuAction(const QString &label, bool top = true);

	QAction *addMenuAction(const QIcon &icon, const QString &label, bool top = true);

	void removeMenuAction(QAction *action);

	void setBurgerIcon(const QIcon &icon);

	void setIconSize(const QSize &size);

	void setMenuWidth(int width);

	void setAnimated(bool animated);

	void setExpanded(bool expanded);

protected:
	void paintEvent(QPaintEvent *) override;

private:
	void registerAction(QAction *action, bool top = true);

	void unRegisterAction(QAction *action);

	void setExpansionState(bool expanded);

	QActionGroup *mActions;
	QPushButton *mBurgerButton;
	int mMenuWidth;
	bool mAnimated;
};

#endif //DONATIONMINING_BURGERMENU_H