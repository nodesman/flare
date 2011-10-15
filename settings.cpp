#include "settings.h"
#include "globals.h"
#include <QtGui>

Settings::Settings(QObject *parent):QSettings(FL_SETTINGS_FILE_LOCATION,QSettings::IniFormat,parent)
{


}

void Settings::addProject(QString name,QString theString)
{
    QMap <QString, QVariant> listOfProjects(this->value("FL_PROJECTS").toMap());
    listOfProjects[name] =  theString;
    this->setValue("FL_PROJECTS",listOfProjects);
    sync();
}

QStringList Settings::listOfProjects()
{
    QMap<QString,QVariant> listOfProj(this->value("FL_PROJECTS").toMap());
    QList <QString> theListOfProjects= listOfProj.keys();
    return theListOfProjects;
}

QString Settings::getProjectString(QString projectName)
{
    QMap <QString,QVariant> listOfProjects(this->value("FL_PROJECTS").toMap());
    QVariant theString = listOfProjects[projectName];
    return theString.toString();
}


QStringList Settings::getSyntaxRules(QString ruleName)
{
   QStringList a;
   QVariant theListOfRules = this->value("FL_SYNTAX_RULES");
   QMap <QString,QVariant> theBigList = theListOfRules.toMap();
   QVariant theParticularRules = theBigList[ruleName];
   QStringList theRules = theParticularRules.toStringList();
   return theRules;
}

