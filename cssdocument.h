#ifndef CSSDOCUMENT_H
#define CSSDOCUMENT_H
#include "document.h"
#include "codeeditor.h"
#include "syntaxhighlighter.h"
#include "foundry.h"
class CSSDocument : public Document
{
    Q_OBJECT;
    SyntaxHighlighter *highlighter;
    CodeEditor *editor;
    QTextCursor *cursor;
    void initialize();
public:
    CSSDocument(QString filename, Foundry *foundry);
public slots:
signals:

};

#endif // CSSDOCUMENT_H
