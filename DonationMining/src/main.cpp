//
// Created by jon on 30.03.20.
//

#include <iostream>

#include <QApplication>
#include <QtCore>

#include "MainWindow.h"


int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	MainWindow window(argc, argv);
	window.show();

	return QApplication::exec();
}

#include <QApplication>
#include <QStyleOptionTab>
#include <QStylePainter>
#include <QTabBar>
#include <QTabWidget>

class TabBar: public QTabBar{
public:
	QSize tabSizeHint(int index) const{
		QSize s = QTabBar::tabSizeHint(index);
		s.transpose();
		return s;
	}
protected:
	void paintEvent(QPaintEvent * /*event*/){
		QStylePainter painter(this);
		QStyleOptionTab opt;

		for(int i = 0;i < count();i++)
		{
			initStyleOption(&opt,i);
			painter.drawControl(QStyle::CE_TabBarTabShape, opt);
			painter.save();

			QSize s = opt.rect.size();
			s.transpose();
			QRect r(QPoint(), s);
			r.moveCenter(opt.rect.center());
			opt.rect = r;

			QPoint c = tabRect(i).center();
			painter.translate(c);
			painter.rotate(90);
			painter.translate(-c);
			painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
			painter.restore();
		}
	}
};

class TabWidget : public QTabWidget
{
public:
	TabWidget(QWidget *parent=0):QTabWidget(parent){
		setTabBar(new TabBar);
		setTabPosition(QTabWidget::West);
	}
};

//int main(int argc, char *argv[])
//{
//	QApplication a(argc, argv);
//	TabWidget w;
//	w.addTab(new QWidget, "tab1");
//	w.addTab(new QWidget, "tab2");
//	w.addTab(new QWidget, "tab3");
//	w.show();
//
//	return a.exec();
//}