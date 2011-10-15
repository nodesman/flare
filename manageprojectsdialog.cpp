#include "manageprojectsdialog.h"
#include "ui_manageprojectsdialog.h"
#include "settings.h"
#include "projectdialog.h"
#include "project.h"
ManageProjectsDialog::ManageProjectsDialog(QWidget *parent):QDialog(parent)
{
    this->initialize();

}

void ManageProjectsDialog::initialize()
{
    setupUi(this);
    this->updateList();



    //connecting signals
    connect(this->closeButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(this->newButton,SIGNAL(clicked()),this,SLOT(newProject()));
    connect(this->editButton,SIGNAL(clicked()),this,SLOT(editProject()));
}

void ManageProjectsDialog::newProject()
{
    ProjectDialog projectDiag(this);
    projectDiag.exec();
    this->updateList();
}

void ManageProjectsDialog::editProject()
{
    Settings settingsObj(this);
    QListWidgetItem *theItem = this->listOfProjects->currentItem();
    int row = this->listOfProjects->currentRow();
    QString name = theItem->text();
    QStringList theList = settingsObj.getProjectString(name).split("%FL_PROJECT_INFO_SEPARATOR%");
    ProjectDialog dialog(theList,this);
    dialog.exec();
    this->updateList();
    this->listOfProjects->setCurrentRow(row);
}



void ManageProjectsDialog::updateList()
{
    this->listOfProjects->clear();
    Settings settingsObj(this);
    QStringList listOfProjects = settingsObj.listOfProjects();
    if (listOfProjects.size() == 0)
    {
        this->editButton->setDisabled(true);
        this->deleteButton->setDisabled(true);
    }
    this->listOfProjects->insertItems(0,listOfProjects);
    this->listOfProjects->setCurrentRow(0);
}

