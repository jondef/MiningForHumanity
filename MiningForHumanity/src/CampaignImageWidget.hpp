//
// Created by jon on 07.06.20.
//

#include <QWidget>
#include <QPixmap>

class CampaignImageWidget : public QWidget {
	using QWidget::QWidget;

protected:
	void paintEvent(QPaintEvent *event) override {
		QWidget::paintEvent(event);
		if (pixmap.isNull()) { return; }

		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);

		QSize pixSize = pixmap.size();
		pixSize.scale(size(), Qt::KeepAspectRatioByExpanding);
		QPixmap scaledPix = pixmap.scaled(pixSize, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
		painter.drawPixmap(QPoint(), scaledPix);
	}

public:
	void setPixmap(QPixmap aPixmap) {
		pixmap = aPixmap;
	}

private:
	QPixmap pixmap;
};