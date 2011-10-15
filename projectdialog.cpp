#include "projectdialog.h"
#include "settings.h"
#include <QDialog>
ProjectDialog::ProjectDialog(QWidget *parent) :QDialog(parent)
{
    this->initialize();
    this->createButton->setEnabled(false);
}

ProjectDialog::ProjectDialog(QStringList &projectSettings,QWidget *parent)
{
    this->initialize();
    this->projectname->setReadOnly(true);
    this->projectname->setText(projectSettings.at(0));
    this->projectpath->setText(projectSettings.at(1));
    this->ftphostname->setText(projectSettings.at(2));
    this->ftpport->setText(projectSettings.at(3));
    this->ftpusername->setText(projectSettings.at(4));
    this->ftppassword->setText(projectSettings.at(5));
    this->createButton->setText("Save");
}

void ProjectDialog::saveproject()
{
    QStringList theProj;
    theProj.append(this->projectname->text());
    theProj.append(this->projectpath->text());
    theProj.append(this->ftphostname->text());
    theProj.append(this->ftpport->text());
    theProj.append(this->ftpusername->text());
    theProj.append(this->ftppassword->text());
    QString theProjectString = theProj.join("%FL_PROJECT_INFO_SEPARATOR%");
    Settings setObj(this);
    setObj.addProject(theProj.at(0),theProjectString);
}


void ProjectDialog::initialize()
{
    setupUi(this);
    QRegExp nameValidation("[a-zA-Z0-9][a-zA-Z0-9]*");
    QRegExpValidator nameValidator(nameValidation,this);
    this->projectname->setValidator(&nameValidator);

    connect(this->projectname,SIGNAL(textChanged(QString)),this,SLOT(testCurrentValues()));
    connect(this->projectpath,SIGNAL(textChanged(QString)),this,SLOT(testCurrentValues()));
    connect(this->createButton,SIGNAL(clicked()),this,SLOT(saveproject()));
    connect(this->browseButton,SIGNAL(clicked()),this,SLOT(browseForFile()));
    connect(this->createButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(close()));
}

void ProjectDialog::testCurrentValues()
{

    if (this->projectname->hasAcceptableInput() && this->pathIsValid())
        this->createButton->setEnabled(true);
    else
        this->createButton->setEnabled(false);
}

bool ProjectDialog::pathIsValid()
{
    QString path = this->projectpath->text();
    if (path.isEmpty())  //we dont want the current directory to be a valid directory
        return false;
    QDir theDir(path);
    return theDir.exists();
}


void ProjectDialog::browseForFile()
{
    QString theDir = QFileDialog::getExistingDirectory(this,"Select Directory");
    this->projectpath->setText(theDir);
}
