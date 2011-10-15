#ifndef SETTINGS_H
#define SETTINGS_H
#include "globals.h"
#include <QSettings>

class Settings : public QSettings
{
    QDataStream *theStream;
    QMap <QString,QVariant> globalSettings;
    void load();

public:
    Settings(QObject *parent);
    void addProject(QString name,QString theString);
    QStringList listOfProjects();
    QString getProjectString(QString);
    //syntax highlighting rules
    QStringList getSyntaxRules(QString name);

};

#endif // SETTINGS_H
