#ifndef SWITCHPROJECT_H
#define SWITCHPROJECT_H
#include "ui_switchproject.h"

#include <QDialog>

class SwitchProject : public QDialog, public Ui::SwitchProject
{
    Q_OBJECT
    QStringList list;
public:
    SwitchProject(QWidget *parent);
    SwitchProject(QString currentprojectName,QWidget *parent);
    void initialize();
private slots:
    void switchIt();
signals:
    void switchProject(QString);
};

#endif // SWITCHPROJECT_H
