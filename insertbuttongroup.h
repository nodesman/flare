#ifndef INSERTBUTTONGROUP_H
#define INSERTBUTTONGROUP_H
#include <QtGui>
#include <QDialog>
#include "ui_insertbuttongroup.h"

class InsertButtonGroup : public QDialog, public Ui::insertbuttongroup
{
    Q_OBJECT;
public:
    InsertButtonGroup();
    QMap <QString,QString> getList();
public slots:
    void addItem();
};

#endif // INSERTBUTTONGROUP_H
