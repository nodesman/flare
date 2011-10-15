#include "foundry.h"
#include <QMessageBox>
#include "document.h"
Foundry::Foundry()
{
}

void Foundry::initialize()
{

}


Foundry::Foundry(QWidget *parent) :QMdiArea(parent)
{

}

void Foundry::closeEvent(QCloseEvent *closeEvent)
{
}


int Foundry::getNumberOfSubWindows()
{
    return this->children().count();
}

void Foundry::documentClosed()
{
    //QMessageBox::information(this,"Document CLosed!","The document was closed and i know it.","OK");
}
