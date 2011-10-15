#ifndef INSERTREGISTRATIONFORM_H
#define INSERTREGISTRATIONFORM_H

#include <QtGui>
#include "ui_insertregistrationform.h"
class InsertRegistrationForm : public QDialog, public Ui::insertRegistrationForm
{
    Q_OBJECT
public:
    InsertRegistrationForm();
public slots:
    void addItem();
    void moveUp();
    void moveDown();
    void deleteItem();
signals:
};

#endif // INSERTREGISTRATIONFORM_H
