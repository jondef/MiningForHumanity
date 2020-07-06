//
// Created by jon on 18.05.20.
//

#ifndef DONATIONMINING_ABSTRACTTABLEMODEL_H
#define DONATIONMINING_ABSTRACTTABLEMODEL_H

#include <QAbstractTableModel>

class AbstractTableModel : public QAbstractTableModel {
	using QAbstractTableModel::QAbstractTableModel;

public:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override {
		// How many rows are there?
		return rows.length();
	}

	int columnCount(const QModelIndex &parent = QModelIndex()) const override {
		// How many columns?
		return headers.length();
	}

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
		if (role != Qt::DisplayRole) {
			return QVariant();
		}
		// What's the value of the cell at the given index?
		return rows.at(index.row()).at(index.column());
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
		if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
			return QVariant();
		}
		// What's the header for the given column?
		return headers.at(section);
	}

private:
	QVector<QString> headers = { "id", "Name", "Users", "Goal",    "Description"														 };
	QVector<QString> row1 = {    "0",  "WHO",  "24852", "100'000", "This project aims to help people that don't have internet access."	 };
	QVector<QString> row2 = {    "1",  "MSF",  "4693",  "25'000",  "Funding for doctors around the world."								 };
	QVector<QString> row3 = {    "2",  "Docs", "13480", "25'0000", "Fight against Corona Virus."										 };
	QVector<QVector<QString>> rows = QVector<QVector<QString>>() << row1 << row2 << row3;
};

#endif //DONATIONMINING_ABSTRACTTABLEMODEL_H
