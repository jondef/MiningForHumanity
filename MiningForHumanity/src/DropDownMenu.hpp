//
// Created by jon on 24.05.20.
//

#ifndef MININGFORHUMANITY_DROPDOWNMENU_HPP
#define MININGFORHUMANITY_DROPDOWNMENU_HPP

#include <QWidget>
#include <QMenu>

class Widget: public QWidget{
public:
	Widget(QWidget *parent=nullptr): QWidget(parent){
		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &QWidget::customContextMenuRequested, this, &Widget::on_customContextMenuRequested);
	}
private:
	void on_customContextMenuRequested(const QPoint &pos){
		QMenu menu;
		menu.addAction("Debug Log");
		menu.addAction("Website");
		menu.addAction("Changelog");
		menu.addAction("About");
		menu.exec(mapToGlobal(pos));
	}
};


#endif //MININGFORHUMANITY_DROPDOWNMENU_HPP
