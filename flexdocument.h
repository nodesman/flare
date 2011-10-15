#ifndef FLEXDOCUMENT_H
#define FLEXDOCUMENT_H

#include "syntaxhighlighter.h"
#include "codeeditor.h"
#include "document.h"

class Foundry;

class FlexDocument : public Document
{
    Q_OBJECT
    SyntaxHighlighter *highlighter;
    CodeEditor *editor;
    QTextCursor *cursor;
    void initialize();
public:
    FlexDocument(QString fileName,QWidget *parent);
signals:
public slots:
};

#endif // FLEXDOCUMENT_H
