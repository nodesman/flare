#include "cssdocument.h"
#include "codeeditor.h"
#include "settings.h"
#include "document.h"
#include "completer.h"
#include "globals.h"


CSSDocument::CSSDocument(QString filename,Foundry *parent)
{
    this->setWindowState(Qt::WindowMaximized);
    this->setParent(parent);
    this->initialize();
    QFile theFile(filename);
    theFile.open(QIODevice::ReadOnly);
    QTextStream reader(&theFile);
    QString theText = reader.readAll();
    this->editor->setPlainText(theText);
}

void CSSDocument::initialize()
{
    this->savedAtleastOnce = false;
    this->activateWindow();
    this->changedSinceSave = false;
    this->editor = new CodeEditor(this);
    this->findDialog = new FindDialog(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QString("Untitled-%1").arg(Document::numberOfNewDocuments));
    connect(this->editor,SIGNAL(copyAvailable(bool)),this,SIGNAL(copyAvailable(bool)));
    connect(this->editor,SIGNAL(textChanged()),this,SIGNAL(documentChanged()));
    connect(this->editor,SIGNAL(textChanged()),this,SLOT(documentModified()) );
    connect(this->editor,SIGNAL(undoAvailable(bool)),this,SIGNAL(undoAvailable(bool)));
    connect(this->editor,SIGNAL(redoAvailable(bool)),this,SIGNAL(redoAvailable(bool)));
    this->setWidget(this->editor);
    QStringListModel *words;
    this->setMinimumHeight(FL_DOCUMENT_MINIMUM_HEIGHT);
    this->setMinimumWidth(FL_DOCUMENT_MINIMUM_WIDTH);
    this->highlighter = new SyntaxHighlighter("CSS",this->editor->document());
    QFile theCSSRuleFile(":/cssCodeCompletionRules.txt");
    theCSSRuleFile.open(QIODevice::ReadOnly);
    QTextStream reader(&theCSSRuleFile);
    QStringList *theList = new QStringList();
    while (!reader.atEnd())
    {
        theList->append(reader.readLine());
    }
    this->editor->setCompleterModel(theList);
    this->setWindowState(Qt::WindowMaximized);
}

