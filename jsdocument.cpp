#include "jsdocument.h"
#include "document.h"
#include "codeeditor.h"
#include "syntaxhighlighter.h"
#include "finddialog.h"
#include "settings.h"
JSDocument::JSDocument(QString filename,Foundry *parent)
{
    Settings theSettingsObj(this);
    this->initialize();
    //this->setParent(parent);
    QStringList theWords;
    QFile theFile(filename);
    this->highlighter = new SyntaxHighlighter("JS",this->editor->document());
    theFile.open(QIODevice::ReadWrite);
    QTextStream reader(&theFile);
    QString fileContent = reader.readAll();
    this->editor->setPlainText(fileContent);
    theWords << "Math" << "function" << "split";
    this->thecompleter = new QCompleter(theWords,this->editor);
    this->thecompleter->setCompletionRole(Qt::EditRole);
    this->thecompleter->setCompletionMode(QCompleter::PopupCompletion);
    this->thecompleter->setCompletionPrefix("mx");
    this->thecompleter->setWidget(this->editor);
    connect(this->editor,SIGNAL(cursorPositionChanged()),this,SLOT(changedIt()));
    //this->thecompleter->complete();
}

void JSDocument::initialize()
{

    this->savedAtleastOnce = false;
    this->changedSinceSave = false;
    this->editor = new CodeEditor(this);
    this->findDialog = new FindDialog(this);
    this->setWindowTitle(QString("Untitled-%1").arg(Document::numberOfNewDocuments));
    connect(this->editor,SIGNAL(copyAvailable(bool)),this,SIGNAL(copyAvailable(bool)));
    connect(this->editor,SIGNAL(textChanged()),this,SIGNAL(documentChanged()));
    connect(this->editor,SIGNAL(textChanged()),this,SLOT(documentModified()) );
    connect(this->editor,SIGNAL(undoAvailable(bool)),this,SIGNAL(undoAvailable(bool)));
    connect(this->editor,SIGNAL(redoAvailable(bool)),this,SIGNAL(redoAvailable(bool)));
}



void JSDocument::changedIt()
{
    QRect theRectangle = this->editor->cursorRect();
    QTextCursor theCur(this->editor->textCursor());
    int prevPosition = theCur.position();
    theCur.select(QTextCursor::LineUnderCursor);
    QString theString = theCur.selectedText();
    theCur.setPosition(prevPosition);
    int pos = theCur.position();
    theCur.movePosition(QTextCursor::StartOfLine);
    int startoflinepos = theCur.position();
    int offset = pos-startoflinepos-1;
    QString theContent  = this->editor->toPlainText();
    QChar previousLetter = theContent[startoflinepos+offset];
    if (previousLetter == QChar('<'))
    {
        this->thecompleter->setCompletionPrefix(theString);
        int numberof = this->thecompleter->completionCount();
        thecompleter->complete(QRect(theRectangle.x()+20,theRectangle.y()+numberof*10,500,500));
        connect(thecompleter,SIGNAL(activated(QString)),this->editor,SLOT(insertPlainText(QString)));

    }
}
