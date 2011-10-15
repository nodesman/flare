#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include <QtGui>
#include "ui_FindDiag.h"

class FindDialog : public QDialog, public Ui::finddialog
{
    Q_OBJECT
    QTextDocument::FindFlags flags;
    QString searchExpression;
    bool isRegularExpression;
    bool direction;
public:
    FindDialog(QWidget *parent);
    void connectSignals();
    void findNext();
    void findPrevious();
public slots:
    void transmitFindArguments();
signals:
    void findText(QString ,QTextDocument::FindFlags);

};

#endif // FINDDIALOG_H
