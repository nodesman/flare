#ifndef PHPDOCUMENT_H
#define PHPDOCUMENT_H
#include "document.h"
#include "codeeditor.h"
#include "syntaxhighlighter.h"


class Foundry;

class PHPDocument : public Document
{
    Q_OBJECT;
    SyntaxHighlighter *highlighter;
    CodeEditor *editor;
    QCompleter *thecompleter;
    QTextCursor *cursor;
    void initialize();

public:

    PHPDocument(QString fileName, Foundry *parent);
public slots:
     void changedIt();

};

#endif // PHPDOCUMENT_H
