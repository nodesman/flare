#ifndef MANAGEPROJECTSDIALOG_H
#define MANAGEPROJECTSDIALOG_H
#include <QtGui>
#include <QDialog>
#include "ui_manageprojectsdialog.h"
#include "project.h"
#include "projectdialog.h"
class ManageProjectsDialog : public QDialog, public Ui::manageprojectsdialog
{
     Q_OBJECT
    int val;
public:
    ManageProjectsDialog(QWidget *);
    void initialize();
    void updateList();
public slots:
    void newProject();
    void editProject();
signals:
    void dummy();
};

#endif // MANAGEPROJECTSDIALOG_H
