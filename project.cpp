#include "project.h"
#include <QString>
#include <QStringList>

Project::Project()
{
}

Project::Project(QString stringToConstructFrom)
{
    QStringList theSettings = stringToConstructFrom.split("%FL_PROJECT_INFO_SEPARATOR%");
    this->name = theSettings.at(0);
    this->path = theSettings.at(1);
    if (theSettings.size() >2)
    {
        this->ftpcredentials.hostname = theSettings.at(2);
        this->ftpcredentials.port = theSettings.at(3);
        this->ftpcredentials.username = theSettings.at(4);
        this->ftpcredentials.password = theSettings.at(5);
        //this->ftpcredentials.mode = theSettings.at(6);
    }
}

Project::ftpinfo Project::getFTPInfo()
{
    return this->ftpcredentials;
}

void Project::setFTPInfo(ftpinfo credentials)
{
    this->ftpcredentials = credentials;
}

void Project::setName(QString name)
{
    this->name = name;
}

void Project::setPath(QString path)
{
    this->path = path;
}

QString Project::getName()
{
    return this->name;
}

QString Project::getPath()
{
    return this->path;
}

QString Project::getFTPHostname()
{
    return this->ftpcredentials.hostname;
}

QString Project::getFTPPort()
{
    return this->ftpcredentials.port;
}

QString Project::getFTPPassword()
{
    return this->ftpcredentials.password;
}

QString Project::getFTPUsername()
{
    return this->ftpcredentials.username;
}

QString Project::getFTPMode()
{
    return this->ftpcredentials.mode;
}

QString Project::toString()
{
    QStringList theSettings;
    theSettings.append(this->name);
    theSettings.append(this->path);
    theSettings.append(this->ftpcredentials.hostname);
    theSettings.append(this->ftpcredentials.port);
    theSettings.append(this->ftpcredentials.username);
    theSettings.append(this->ftpcredentials.password);
    //theSettings.append(this->ftpcredentials.mode);
    QString theString = theSettings.join("%FL_PROJECT_INFO_SEPARATOR%");
    return theString;
}
