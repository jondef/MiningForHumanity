//
// Created by jon on 05.05.20.
//

#ifndef DONATIONMINING_BLURLAYER_H
#define DONATIONMINING_BLURLAYER_H

#include <QWidget>
#include <QPixmap>

class BlurLayer : public QWidget {
	using QWidget::QWidget;

protected:
	void paintEvent(QPaintEvent *event) override {
		QWidget::paintEvent(event);
		if (pixmap.isNull()) { return; }

		QPainter painter(this);
		painter.drawPixmap(QPoint(), pixmap);
	}

public:
	QPixmap pixmap;
};

#endif //DONATIONMINING_BLURLAYER_H
