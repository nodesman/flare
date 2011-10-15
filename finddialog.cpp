#include "finddialog.h"
#include <QtGui>

FindDialog::FindDialog(QWidget* parent):QDialog(parent)
{
    this->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->connectSignals();
}

void FindDialog::connectSignals()
{
    connect(this->findButton,SIGNAL(clicked()),this,SLOT(transmitFindArguments()));
    connect(this->closeButton,SIGNAL(clicked()),this,SLOT(close()));
}

void FindDialog::transmitFindArguments()
{
    this->searchExpression = this->keyword->toPlainText();
    bool CaseSensitive = this->caseSensitive->isChecked();
    //this->isRegularExpression= this->regularExpression->isChecked();
    bool searchUp = this->directionUp->isChecked(); //confused here.. what does it do exactly?
    if (CaseSensitive)
        this->flags= QTextDocument::FindCaseSensitively;
    if (searchUp)
        this->flags = this->flags | QTextDocument::FindBackward;
    emit findText(this->searchExpression,this->flags);

}

void FindDialog::findNext()
{
    emit findText(this->searchExpression,this->flags);
}

void FindDialog::findPrevious()
{
    QTextDocument::FindFlags flag = this->flags;
    flag = flag  ^  QTextDocument::FindBackward;
    emit findText(this->searchExpression,flag);
}

