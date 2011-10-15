#include "insertbuttongroup.h"
#include <QtGui>
InsertButtonGroup::InsertButtonGroup()
{
    setupUi(this);
    connect(this->addButton,SIGNAL(clicked()),this,SLOT(addItem()));
    QStringList columnlabels;
    columnlabels.append(QString("Text"));
    columnlabels.append(QString("Value"));
    QTableWidgetItem *item = new QTableWidgetItem("Option");
    this->itemlist->setColumnCount(2);
    this->itemlist->setHorizontalHeaderLabels(columnlabels);
}

QMap <QString , QString> InsertButtonGroup::getList()
{
    QMap <QString,QString> list;

    int num = this->itemlist->rowCount();

    for (int curr=0;curr < num; curr++)
    {

        QString text = this->itemlist->item(curr,0)->text();
        QString value = this->itemlist->item(curr,1)->text();

        list.insert(text,value);
    }
    return list;

}




void InsertButtonGroup::addItem()
{
    int num = this->itemlist->rowCount();
    this->itemlist->setRowCount(num+1);
    this->itemlist->setCurrentItem(this->itemlist->item(num+1,1),QItemSelectionModel::Select);

}
