//
// Created by jon on 05.05.20.
//

#ifndef DONATIONMINING_FRAME_H
#define DONATIONMINING_FRAME_H

#include <QFrame>

class Frame : public QFrame {
	using QFrame::QFrame;

protected:
	void paintEvent(QPaintEvent *event) override {
		QFrame::paintEvent(event);
		QColor backgroundColor = palette().light().color();
		backgroundColor.setAlpha(150);
		QPainter customPainter(this);
		customPainter.fillRect(rect(), backgroundColor);
	}
};

#endif //DONATIONMINING_FRAME_H
