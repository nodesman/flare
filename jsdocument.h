#ifndef JSDOCUMENT_H
#define JSDOCUMENT_H
#include "document.h"
#include "codeeditor.h"
#include "syntaxhighlighter.h"
class Foundry;


class JSDocument: public Document
{
    Q_OBJECT
     SyntaxHighlighter *highlighter;
    CodeEditor *editor;
        QCompleter *thecompleter;
    QTextCursor *cursor;
    void initialize();
public:

    JSDocument(QString filename, Foundry *foundry);
public slots:
    void changedIt();

signals:
};

#endif // JSDOCUMENT_H
