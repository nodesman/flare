#ifndef HTMLDOCUMENT_H
#define HTMLDOCUMENT_H
#include <QtGui>
#include <QTextEdit>
#include <QWebView>
#include "document.h"
#include "finddialog.h"
class Foundry;

class HTMLDocument : public Document
{

    Q_OBJECT

    QTabWidget *tabArea;
    QWebView *previewArea;
    CodeEditor *codeArea;
    QWebView *designArea;
    QString lastDesignSearchKeyword;
    QWebPage::FindFlags lastDesignSearchFlags;
    QWebPage *webPage;
    int currentMode; //0 = design, 1 = code, 2 = preview
    QTemporaryFile *tmpfile;
    QString fileContent(QString filePath);
public:
    enum Modes { Design, Code, Preview};

    virtual void initialize();
    HTMLDocument(QString filePath, Foundry *parent);
    void setContent(QString content);
    void connectSignals();
    void setActiveTab(int tabNumber);
    void connectDesignSignals();


    virtual void setupSave();
public slots:
    void execCommand(QString command);
    void execCommand(QString command, QString options);
    void viewModeChanged(int);
    virtual void adjustActions();
    virtual void undo();
    virtual void redo();
    virtual void cut();
    virtual void copy();
    virtual void paste();
    virtual void selectAll();
    virtual void findNext();

    virtual void bold();
    virtual void italic();
    virtual void underline();

    virtual void alignleft();
    virtual void alignright();
    virtual void aligncenter();
    virtual void alignjustify();

    virtual void listbulleted();
    virtual void listnumbered();

    virtual void increaseindent();
    virtual void decreaseindent();

    virtual void styleHeading1();
    virtual void styleHeading2();
    virtual void styleHeading3();
    virtual void styleHeading4();
    virtual void styleHeading5();
    virtual void styleHeading6();
    virtual void insertTextField();
    virtual void insertTextArea();
    virtual void insertPasswordField();
    virtual void insertRadioButton();
    virtual void insertButton();
    virtual void insertSubmitButton();
    virtual void insertSelectMenu();
    virtual void insertCheckboxGroup();
    virtual void insertRadioGroup();
    virtual void insertImage();
    virtual void insertLink();
    virtual void insertSubscript();
    virtual void insertSuperscript();
    virtual void insertJavaApplet();


    //form templates
    virtual void insertRegistrationForm();
    virtual void insertCommentForm();
    virtual void insertLoginForm();
    virtual void insertSearchForm();
    virtual void insertMailingListOptin();


    virtual void insertTable();


    virtual void attachjQuery(bool);
    virtual QString checkBox(QString name,QString value, QString cssclass, QString id, bool checked);
    virtual QString radioButton(QString name,QString value, QString cssclass, QString id, bool checked);
    virtual QString submitButton(QString name,QString value, QString cssclass, QString id);
    virtual QString button(QString name,QString value, QString cssclass, QString id);
    virtual QString textField(QString name,QString value,QString cssclass,QString id);
    virtual QString passwordField(QString name,QString value,QString cssclass,QString id);
    virtual QString selectMenu(QStringList items, QString name,QString cssclass,QString id);
    virtual QString link(QString linktext,QString href);
    virtual QString textArea(QString name,QString value,QString rows,QString cols,QString cssclass,QString id);


    virtual QString selectMenu(QMap <QString,QString> items,QString name,QString cssclass,QString id);
    virtual void insertAnchor();
    virtual QString getSelectedText();
    virtual void insertCheckBox();
    virtual void doAction(QAction *action);


    virtual void insertHTMLAtCursor(QString htmlToInsert);
    void execute(QString,QString);
    void execute(QString);
    virtual QString getType();

    void findText(QString, QTextDocument::FindFlags);
signals:
    void dummy();
};

#endif // HTMLDOCUMENT_H
