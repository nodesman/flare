#ifndef FLARE_H
#define FLARE_H

#include <QtGui>

#include "foundry.h"
#include "globals.h"
#include "newfiledialog.h"
#include "finddialog.h"
//#include "cssdocument.h"
#include "htmldocument.h"
#include "jsdocument.h"
#include "phpdocument.h"
#include <QClipboard>
#include "project.h"
#include "newfiledialog.h"
namespace Ui
{
    class flareClass;
    class NewFileDialog;
}

class flare : public QMainWindow
{
    Q_OBJECT
    QList <flare *> listOfWindows;
    static QString currentWorkingDirectory;

    //newFileDialog *newDocDiag;
    FindDialog *finddiag;
    QList <QShortcut *> keyboardShortcutList;
    QClipboard *clipboard;
    QUndoStack *undoStack;
    QSignalMapper *windowMapper;
    QToolBar *standardToolbar;
    QToolBar *formattingToolbar;
    QToolBar *insertionToolbar;
    QToolBar *miscToolbar;
    QMap <QString, QAction *> toolbarButtons;
    Project *activeProject;
public:
    flare(QWidget *parent = 0);
    ~flare();
  //  newFileDialog *getNewFileDialog();
   // void setNewFileDialog(newFileDialog *dialog);
private:
    Ui::flareClass *ui;
    Foundry *editor;
    static int numberOfDocuments;
    void attachKeyboardShortcuts();
    void initialize();
    void initializeDocument(Document *);

protected:
    void closeEvent(QCloseEvent *);
    NewFileDialog *newfile;
public slots:
    void documentClosed();
    void documentChanged();
    void createNewDocument(QString filepath,QString type,QString templateName){}

    void newPHPFile();
    void newCSSFile();
    void newJSFile();
    void newHTMLFile();
    void newFlexFile();

    void save();
    void saveAs();
    void saveAll();
    void openfile();
    void find();
    void findNext();
    void exitApplication();
    void closeDocument();
    bool closeAllDocuments();
    bool isFileOpen(QString filePath);
    void decrementNumberOfDocuments();
    void disableFileActions(); //this disables items in the menu when no documents are open.
    void enableFileActions();
    void updateWindowMenu();
    void findTriggered(QString, QTextDocument::FindFlags);
    void toggleClipboardOperations(bool);
    void edit_togglePaste();
    void switchProject(QString projectName);
    void activateSubWindow(QWidget *);
    void alert(int);
    void alert(QString);

private slots:

    void on_actionNew_Window_2_triggered();
    void on_actionCascade_Windows_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionRedo_triggered();
    void on_actionUndo_triggered();
    void on_actionPrevious_Window_triggered();
    void on_actionNext_Window_triggered();
    void on_actionClose_All_triggered();
    void on_actionClose_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_triggered();
    void on_actionFind_Next_triggered();
    void on_actionFind_triggered();
    void on_actionSelect_All_triggered();
    void on_actionPaste_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionSwitch_Project_triggered();
    void on_actionManage_Project_triggered();
    void on_actionNew_Project_triggered();
    void pasteStatus();
    void about();
    void documentOpened();
    void htmlCommand();
    void dummy();
signals:
    void newdocument();


};

#endif // FLARE_H
