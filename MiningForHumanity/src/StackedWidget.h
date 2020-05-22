//
// Created by jon on 05.05.20.
//

#ifndef DONATIONMINING_STACKEDWIDGET_H
#define DONATIONMINING_STACKEDWIDGET_H

#include <QStackedWidget>

class StackedWidget : public QStackedWidget {
	using QStackedWidget::QStackedWidget;

	QSize sizeHint() const override {
		return currentWidget()->sizeHint();
	}

	QSize minimumSizeHint() const override {
		return currentWidget()->minimumSizeHint();
	}
};

#endif //DONATIONMINING_STACKEDWIDGET_H
