//
// Created by jon on 29.04.20.
//

#include "BurgerButton.h"


BurgerButton::BurgerButton(QAction *action, QWidget *parent) : QPushButton(parent), mIconSize(QSize(64, 64)), mAction(action), hovered(false) {
	setObjectName(BurgerButtonObjectName);
	connect(action, &QAction::destroyed, this, &BurgerButton::deleteLater);
	setCursor(Qt::PointingHandCursor);

	connect(mAction, SIGNAL(changed()), this, SLOT(update()));
	connect(this, &BurgerButton::clicked, this, [&] {
		if (mAction->isCheckable() && !mAction->isChecked()) {
			mAction->toggle();
		}
		mAction->trigger();
	});
}

void BurgerButton::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	QStyleOptionButton opt;
	opt.initFrom(this);
	opt.state |= (mAction->isChecked() ? QStyle::State_On : QStyle::State_Off);

	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	const QRect contentsRect = style()->subElementRect(QStyle::SE_PushButtonContents, &opt, this);
	if (!mAction->icon().isNull()) {
		QIcon::Mode mode = ((opt.state & QStyle::State_MouseOver) == 0) ? QIcon::Normal : QIcon::Active;
		if (!isEnabled()) {
			mode = QIcon::Disabled;
		}
		QIcon::State state = mAction->isChecked() ? QIcon::On : QIcon::Off;
		painter.drawPixmap(QRect(contentsRect.topLeft(), mIconSize), mAction->icon().pixmap(mIconSize, mode, state));
	}

	opt.rect = contentsRect.adjusted(mIconSize.width() + 1, 0, 0, 0);
	opt.text = fontMetrics().elidedText(mAction->iconText(), Qt::ElideRight, opt.rect.width(), Qt::TextShowMnemonic);
	style()->drawControl(QStyle::CE_CheckBoxLabel, &opt, &painter, this);
}

void BurgerButton::setIconSize(const QSize &size) {
	mIconSize = size;
	setFixedHeight(mIconSize.height());
	update();
}

bool BurgerButton::event(QEvent *event) {
	if (event->type() == QEvent::HoverEnter) {
		if (!hovered) {
			hovered = true;
			emit mouseEnterButton(this);
		}
	} else if (event->type() == QEvent::HoverLeave) {
		if (hovered) {
			hovered = false;
			emit mouseLeaveButton(this);
		}
	}
	return QPushButton::event(event);
}

QAction *BurgerButton::action() const { return mAction; }

QHBoxLayout *BurgerButton::getLayout() const { return layout; }

void BurgerButton::setLayout(QHBoxLayout *layout) { this->layout = layout; }
