//
// Created by jon on 05.05.20.
//

#ifndef DONATIONMINING_BLURLAYER_H
#define DONATIONMINING_BLURLAYER_H

#include <QWidget>
#include <QPixmap>
#include <QtWidgets/QGraphicsBlurEffect>

class BlurLayer : public QWidget {
public:
	explicit BlurLayer(QWidget *parent = nullptr) : QWidget(parent) {
		QGraphicsBlurEffect *p_blur = new QGraphicsBlurEffect(this);
		p_blur->setBlurRadius(10);
		p_blur->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
		setGraphicsEffect(p_blur);
	}

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
