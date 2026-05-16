#ifndef HTMLDOCUMENT_H
#define HTMLDOCUMENT_H

#include <QtWidgets>
#include "document.h"
#include "finddialog.h"

class Foundry;

class HTMLDocument : public Document
{
    Q_OBJECT

    QTabWidget *tabArea;
    QTextEdit *designArea;
    QTextBrowser *previewArea;
    int currentMode;
    QString fileContent(const QString &filePath);
    void mergeDesignCharFormat(const QTextCharFormat &format);
    void syncFromCurrentMode();
    void applyBlockTag(const QString &tagName);

public:
    enum Modes { Design, Code, Preview };

    explicit HTMLDocument(QString filePath, Foundry *parent);
    void initialize() override;
    void setContent(QString content);
    void connectSignals();
    void setActiveTab(int tabNumber);
    void connectDesignSignals();
    void setupSave() override;

public slots:
    void execCommand(QString command);
    void execCommand(QString command, QString options);
    void viewModeChanged(int tabNumber);
    void adjustActions();
    void undo() override;
    void redo() override;
    void cut() override;
    void copy() override;
    void paste() override;
    void selectAll() override;
    void findNext() override;

    void bold();
    void italic();
    void underline();

    void alignleft();
    void alignright();
    void aligncenter();
    void alignjustify();

    void listbulleted();
    void listnumbered();

    void increaseindent();
    void decreaseindent();

    void styleHeading1();
    void styleHeading2();
    void styleHeading3();
    void styleHeading4();
    void styleHeading5();
    void styleHeading6();
    void insertTextField();
    void insertTextArea();
    void insertPasswordField();
    void insertRadioButton();
    void insertButton();
    void insertSubmitButton();
    void insertSelectMenu();
    void insertCheckboxGroup();
    void insertRadioGroup();
    void insertImage();
    void insertLink();
    void insertSubscript();
    void insertSuperscript();
    void insertJavaApplet();

    void insertRegistrationForm();
    void insertCommentForm();
    void insertLoginForm();
    void insertSearchForm();
    void insertMailingListOptin();
    void insertTable();

    void attachjQuery(bool silent);
    QString checkBox(QString name, QString value, QString cssclass, QString id, bool checked);
    QString radioButton(QString name, QString value, QString cssclass, QString id, bool checked);
    QString submitButton(QString name, QString value, QString cssclass, QString id);
    QString button(QString name, QString value, QString cssclass, QString id);
    QString textField(QString name, QString value, QString cssclass, QString id);
    QString passwordField(QString name, QString value, QString cssclass, QString id);
    QString selectMenu(QStringList items, QString name, QString cssclass, QString id);
    QString link(QString linktext, QString href);
    QString textArea(QString name, QString value, QString rows, QString cols, QString cssclass, QString id);

    QString selectMenu(QMap<QString, QString> items, QString name, QString cssclass, QString id);
    void insertAnchor();
    QString getSelectedText();
    void insertCheckBox();
    void doAction(QAction *action);

    void insertHTMLAtCursor(QString htmlToInsert);
    void execute(QString command, QString parameter);
    void execute(QString command);
    QString getType() override;

    void findText(QString searchKeyword, QTextDocument::FindFlags findflags) override;
};

#endif // HTMLDOCUMENT_H
