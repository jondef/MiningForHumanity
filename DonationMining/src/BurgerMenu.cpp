#include "BurgerMenu.h"
#include <QStackedWidget>
#include <QtCore>
#include <QtGui>
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


BurgerMenu::BurgerMenu(QWidget *parent) : QWidget(parent), mActions(new QActionGroup(this)), mBurgerButton(new QPushButton(this)), mMenuWidth(200),
										  mAnimated(true) {
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	mBurgerButton->setObjectName(MainBurgerButtonObjectName);
	mBurgerButton->setFlat(true);
	mBurgerButton->setIconSize(QSize(48, 48));
	mBurgerButton->setFixedSize(48, 48);
	mBurgerButton->setCheckable(true);
	mBurgerButton->setCursor(Qt::PointingHandCursor);
	mActions->setExclusive(true);

	QWidget *burgerWidget = new QWidget(this);
	burgerWidget->setObjectName(BurgerMenuName);
	QHBoxLayout *burgerLay = new QHBoxLayout();
	burgerLay->setContentsMargins(0, 0, 0, 0);
	burgerLay->setSpacing(0);
	burgerLay->addWidget(mBurgerButton);
	burgerLay->addWidget(burgerWidget);

	setLayout(new QVBoxLayout());
	static_cast<QVBoxLayout *>(layout())->setContentsMargins(0, 0, 0, 0);
	static_cast<QVBoxLayout *>(layout())->setSpacing(0);

	static_cast<QVBoxLayout *>(layout())->addLayout(burgerLay);
	static_cast<QVBoxLayout *>(layout())->addStretch();
	setFixedWidth(48);

	connect(mBurgerButton, &QPushButton::toggled, this, &BurgerMenu::setExpansionState);
	connect(mBurgerButton, &QPushButton::toggled, this, &BurgerMenu::expandedChanged);
	connect(mActions, &QActionGroup::triggered, this, &BurgerMenu::triggered);

	m_tooltipWidget->setLayout(new QHBoxLayout());
	m_tooltipWidget->layout()->setContentsMargins(0, 0, 0, 0);
	m_tooltipWidget->layout()->setSpacing(0);
}

QIcon BurgerMenu::burgerIcon() const {
	return mBurgerButton->icon();
}

QSize BurgerMenu::iconSize() const {
	return mBurgerButton->iconSize();
}

int BurgerMenu::menuWidth() const {
	return mMenuWidth;
}

QList<QAction *> BurgerMenu::actions() const {
	return mActions->actions();
}

QAction *BurgerMenu::addMenuAction(QAction *action, bool top) {
	mActions->addAction(action);
	registerAction(action, top);
	return action;
}

QAction *BurgerMenu::addMenuAction(const QString &label, bool top) {
	auto action = mActions->addAction(label);
	action->setCheckable(true);
	registerAction(action, top);
	return action;
}

QAction *BurgerMenu::addMenuAction(const QIcon &icon, const QString &label, bool top) {
	auto action = mActions->addAction(icon, label);
	action->setCheckable(true);
	registerAction(action, top);
	return action;
}

void BurgerMenu::removeMenuAction(QAction *action) {
	mActions->removeAction(action);
	unRegisterAction(action);
}

void BurgerMenu::setBurgerIcon(const QIcon &icon) {
	mBurgerButton->setIcon(icon);
	emit iconChanged();
}

void BurgerMenu::setIconSize(const QSize &size) {
	if (size == mBurgerButton->iconSize())
		return;

	mBurgerButton->setIconSize(size);
	mBurgerButton->setFixedSize(size);
	auto buttons = findChildren<BurgerButton *>(BurgerButtonObjectName);
	for (auto btn : buttons)
		btn->setIconSize(size);

	if (mBurgerButton->isChecked())
		setFixedWidth(mBurgerButton->width() + mMenuWidth);
	else
		setFixedWidth(mBurgerButton->width());

	emit iconSizeChanged(size);
}

void BurgerMenu::setMenuWidth(int width) {
	if (width == mMenuWidth)
		return;

	mMenuWidth = width;

	if (mBurgerButton->isChecked())
		setFixedWidth(mBurgerButton->width() + mMenuWidth);
	else
		setFixedWidth(mBurgerButton->width());

	emit menuWidthChanged(mMenuWidth);
}

void BurgerMenu::setExpansionState(bool expanded) {
	if (mAnimated) {
		auto anim = new QPropertyAnimation(this, "minimumWidth", this);
		anim->setDuration(250);
		anim->setStartValue(mBurgerButton->width());
		anim->setEndValue(mBurgerButton->width() + mMenuWidth);
		anim->setDirection(expanded ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
		anim->setEasingCurve(expanded ? QEasingCurve::OutCubic : QEasingCurve::InCubic);
		anim->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		if (expanded)
			setFixedWidth(mBurgerButton->width() + mMenuWidth);
		else
			setFixedWidth(mBurgerButton->width());
	}
}

void BurgerMenu::registerAction(QAction *action, bool top) {
	BurgerButton *button = new BurgerButton(action, this);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(button);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	button->setLayout(buttonLayout);

	connect(button, &BurgerButton::mouseEnterButton, this, [this](BurgerButton *button) {
		if (expanded()) { return; }
		QPoint position = button->mapTo(this, button->rect().topLeft());
		m_tooltipWidget->setGeometry(position.x(), position.y(), button->size().rwidth() + mMenuWidth, button->size().rheight());
		button->getLayout()->setContentsMargins(0, 0, 0, button->height());
		m_tooltipWidget->layout()->addWidget(button);
		m_tooltipWidget->setParent(parentWidget()->parentWidget()); // cannot call parentWidget() in the constructor
		m_tooltipWidget->show();
	});
	connect(button, &BurgerButton::mouseLeaveButton, this, [this](BurgerButton *button) {
		if (expanded()) { return; }
		button->getLayout()->addWidget(button);
		button->getLayout()->setContentsMargins(0, 0, 0, 0);
		m_tooltipWidget->hide(); // ! order matters! Seg fault can happen if at the wrong place!
	});

	button->setIconSize(mBurgerButton->iconSize());
	auto lay = static_cast<QVBoxLayout *>(layout());

	if (top) {
		lay->insertLayout(lay->count() - 1, buttonLayout);
	} else {
		lay->insertLayout(lay->count(), buttonLayout);
	}
}

void BurgerMenu::unRegisterAction(QAction *action) {
	auto buttons = findChildren<BurgerButton *>(BurgerButtonObjectName);
	auto btn = std::find_if(buttons.begin(), buttons.end(), [&](BurgerButton *btn) { return btn->action() == action; });
	if (btn != buttons.end())
		delete *btn;
}

bool BurgerMenu::animated() const {
	return mAnimated;
}

bool BurgerMenu::expanded() const {
	return mBurgerButton->isChecked();
}

void BurgerMenu::setAnimated(bool animated) {
	if (mAnimated == animated)
		return;

	mAnimated = animated;
	emit animatedChanged(mAnimated);
}

void BurgerMenu::setExpanded(bool expanded) {
	mBurgerButton->setChecked(expanded);
}

void BurgerMenu::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	QStyleOption opt;
	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

