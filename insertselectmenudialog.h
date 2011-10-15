#ifndef INSERTSELECTMENUDIALOG_H
#define INSERTSELECTMENUDIALOG_H
#include <QtGui>
#include <QDialog>
#include "ui_insertselectmenu.h"


class InsertSelectMenuDialog : public QDialog,public Ui::InsertSelectMenu
{
    Q_OBJECT;
    QStringListModel theModel;
    QMap <QString, QString> list;
public:
    InsertSelectMenuDialog();
    QMap <QString,QString> getData();
    InsertSelectMenuDialog(QMap <QString,QString> map);
public slots:
    void itemSelected(QTableWidgetItem *);
    void addItem();
    void deleteItem();

signals:

};

#endif // INSERTSELECTMENUDIALOG_H
