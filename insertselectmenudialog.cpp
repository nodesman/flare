#include "insertselectmenudialog.h"
#include <QtGui>
#include <QUiLoader>

InsertSelectMenuDialog::InsertSelectMenuDialog()
{
    //this->addButton->setEnabled(false);
    setupUi(this);
    QStringList headings;
    headings.append(QString("Text"));
    headings.append(QString("Value"));
    this->listOfItems->setColumnCount(2);
    this->listOfItems->setHorizontalHeaderLabels(headings);
    connect(this->addItemButton,SIGNAL(clicked()),this,SLOT(addItem()));
}
InsertSelectMenuDialog::InsertSelectMenuDialog(QMap <QString,QString> list)
{

}



void InsertSelectMenuDialog::itemSelected(QTableWidgetItem *selectedItem)
{
    //set the value of name and value text boxes

    //set the label of add button to save
    //enable the - button
}

void InsertSelectMenuDialog::addItem()
{
    int newrow = this->listOfItems->rowCount()+1;
    this->listOfItems->setRowCount(newrow);
    this->listOfItems->setCurrentCell(newrow-1,0,QItemSelectionModel::Select );
    this->listOfItems->editItem(this->listOfItems->currentItem());
    this->listOfItems->openPersistentEditor(this->listOfItems->item(newrow-1,0));

}



QMap<QString,QString> InsertSelectMenuDialog::getData()
{
    int rowcount = this->listOfItems->rowCount();
    QMap <QString,QString> data;
    QString name;
    QString value;
    for (int iter = 0;iter< rowcount;iter++)
    {
        name = this->listOfItems->item(iter,0)->text();
        value = this->listOfItems->item(iter,1)->text();
        data.insert(name,value);
    }
    return data;
}


void InsertSelectMenuDialog::deleteItem()

{
}
