#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H
#include "ui_newfiledialog.h"
#include <QtGui>
class NewFileDialog: public QDialog, public Ui::NewFileDialog
{
    Q_OBJECT;

    QWidget *clientside;
    QWidget *clientsidedetail;
    QWidget *serverside;

    QListWidget *clientSideFileTypes;

    QListWidget *serverSideFileTypes;
public:
    NewFileDialog(QWidget *parent);
        QVBoxLayout *layout;
        QStackedLayout *detailLayout;
    QListWidgetItem *phpFile;
    QListWidgetItem *htmlFile;
    QListWidgetItem *cssFile;
    QListWidgetItem *jsFile;
    QListWidgetItem *flexFile;
    QDialogButtonBox *buttonBox;
    QListWidget *filetypelist;
public slots:
    void setButtonStatus();
    void browseFile();
   // void changeFileType();


signals:
    void newphpfile();
    void newhtmlfile();
    void newcssfile();
    void newjsfile();
    void newflexfile();
};

#endif // NEWFILEDIALOG_H
