#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H
#include <QtGui>
#include <QDialog>
#include "ui_projectdialog.h"

class ProjectDialog : public QDialog, Ui::Dialog
{
    Q_OBJECT

public:
    ProjectDialog(QWidget *parent);
    ProjectDialog(QStringList &projectSettings, QWidget *parent);
    void initialize();
    bool pathIsValid();
public slots:
    void testCurrentValues();
private slots:
    void saveproject();
    void browseForFile();
signals:
    void dummy();
};

#endif // PROJECTDIALOG_H
