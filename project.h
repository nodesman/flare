#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

class Project : public QObject
{
    public:
     struct ftpinfo {
        QString hostname;
        QString port;
        QString username;
        QString password;
        QString mode;
    };
     ftpinfo ftpcredentials;
private:
    QString name;
    QString path;


public:

    Project();
    Project(QString);
    void setFTPInfo(ftpinfo);
    void setPath(QString path);
    void setName(QString name);
    QString getName();
    QString getPath();
    ftpinfo getFTPInfo();
    QString getFTPHostname();
    QString getFTPPort();
    QString getFTPUsername();
    QString getFTPPassword();
    QString getFTPMode();
    QString toString();
};

#endif // PROJECT_H
