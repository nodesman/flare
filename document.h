#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <QtGui>
#include <QFileDialog>
#include "codeeditor.h"
#include "syntaxhighlighter.h"
#include "finddialog.h"
class Document: public QMdiSubWindow
{

    Q_OBJECT
protected:
    static int numberOfNewDocuments;
    CodeEditor *editor;
    SyntaxHighlighter *highlighter;
    Completer *thecompleter;
    FindDialog *findDialog;
    QString lastSearchKeyword;
    QTextDocument::FindFlags lastSearchFlags;
    QString code;
    QString fileName;
    bool savedAtleastOnce;
    bool changedSinceSave;
    Document();
public:
    Document(QString filename,QWidget *parent);
    bool isUndoAvailable();
    void setSaved(bool savedStatus);
    void setChanged(bool changedStatus);
    void setFilePath(QString filePath);
    bool getSaved();
    bool getChanged();
    void closeEvent(QCloseEvent *closeEvent);
    virtual QString getType();
public slots:
    virtual void undo();
     virtual void redo();
     virtual void cut();
     virtual void copy();
     virtual void paste();
     virtual void selectAll();
     virtual void setupSave();
     virtual void initialize();
     virtual void saveDocument(QString filepath);
     virtual void saveDocument();
     virtual void find();
     virtual void findNext();
     virtual void findText(QString,QTextDocument::FindFlags);
     virtual void insertAtCursor(QString textToInsert);
     virtual void documentModified();
signals:
    void copyAvailable(bool);
    void documentChanged();
    void undoAvailable(bool);
    void redoAvailable(bool);

};

#endif // DOCUMENT_H
