#include "switchproject.h"
#include "settings.h"
#include <QtGui>
SwitchProject::SwitchProject(QWidget *parent):QDialog(parent)
{
    this->initialize();
    this->projectlist->setCurrentRow(0);
}
SwitchProject::SwitchProject(QString currentProjectName,QWidget *parent):QDialog(parent)
{
    this->initialize();
    int pos = this->list.indexOf(QRegExp(currentProjectName));
    this->projectlist->setCurrentRow(pos);

}

void SwitchProject::switchIt()
{
    QString theCurrentSelection = this->projectlist->currentItem()->text();
    emit switchProject(theCurrentSelection);
}
void SwitchProject::initialize()
{
    setupUi(this);
    Settings setObj(this);
    this->list = setObj.listOfProjects(); //list of projects
    this->projectlist->insertItems(0,list);
    connect(this,SIGNAL(accepted()),this,SLOT(switchIt()));

}
