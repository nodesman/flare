#include "insertregistrationform.h"
#include <QtGui>
InsertRegistrationForm::InsertRegistrationForm()
{

    setupUi(this);
    connect(this->addButton,SIGNAL(clicked()),SLOT(addItem()));
    connect(this->moveUpButton,SIGNAL(clicked()),SLOT(moveUp()));
    connect(this->moveDownButton,SIGNAL(clicked()),SLOT(moveDown()));
    connect(this->deleteButton,SIGNAL(clicked()),SLOT(deleteItem()));
    QStringList listOfItems;
    listOfItems.append("First Name");
    listOfItems.append("Last Name");
    listOfItems.append("Full Name");
    listOfItems.append("Surname");
    listOfItems.append("Email-Address");
    listOfItems.append("Date Of Birth");
    listOfItems.append("City");
    listOfItems.append("State");
    listOfItems.append("Country Field");
    listOfItems.append("Username");
    listOfItems.append("Password");
    listOfItems.append("Website URL");
    listOfItems.append("Gender");
    listOfItems.append("Postal Code");
    listOfItems.append("Submit");
    this->bigList->insertItems(0,listOfItems);
    this->setWindowTitle("Insert Registration Form");


}


void InsertRegistrationForm::addItem()
{

    if (this->bigList->currentItem() != 0)
    {

        QListWidgetItem *item = this->bigList->currentItem();
        QListWidgetItem *newitem = new QListWidgetItem(this->smallList);
        newitem->setText(item->text());
        this->smallList->insertItem(this->smallList->count()-1,newitem);
    }
}


void InsertRegistrationForm::moveUp()
{

    if (this->smallList->currentItem() !=0)
    {

        int currentOffset = this->smallList->currentRow();

        QListWidgetItem *item = this->smallList->currentItem();
        this->smallList->takeItem(currentOffset);
        int newoffset;
        newoffset  = currentOffset-1;
        if (currentOffset < 0)
            newoffset = 0;
        this->smallList->insertItem(newoffset,item);
        this->smallList->setCurrentRow(newoffset);
    }

}

void InsertRegistrationForm::moveDown()
{
    if (this->smallList->currentItem() !=0)
    {
        int currentOffset = this->smallList->currentRow();
        QListWidgetItem *item = this->smallList->currentItem();
        this->smallList->takeItem(currentOffset);
        int newoffset;
        newoffset  = currentOffset+1;
        if (currentOffset < 0)
            newoffset = 0;
        this->smallList->insertItem(newoffset,item);
        this->smallList->setCurrentRow(newoffset);
    }
}


void InsertRegistrationForm::deleteItem()
{
    if (this->smallList->currentItem() !=0)
    {
        this->smallList->takeItem(this->smallList->currentRow());
    }
}
