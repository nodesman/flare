#include "flexdocument.h"
#include "codeeditor.h"
#include "settings.h"
#include "document.h"
#include "completer.h"
#include "globals.h"

FlexDocument::FlexDocument(QString filename,QWidget *parent)
{
    this->initialize();
    this->setParent(parent);
    QStringList theWords;
    QFile theFile(filename);
    theFile.open(QIODevice::ReadWrite);
    QTextStream reader(&theFile);
    QString fileContent = reader.readAll();
    this->editor->setPlainText(fileContent);
}

void FlexDocument::initialize()
{

    this->savedAtleastOnce = false;
    this->changedSinceSave = false;
    this->editor = new CodeEditor(this);
    this->setWidget(this->editor);
    this->findDialog = new FindDialog(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QString("Untitled-%1").arg(Document::numberOfNewDocuments));
    connect(this->editor,SIGNAL(copyAvailable(bool)),this,SIGNAL(copyAvailable(bool)));
    connect(this->editor,SIGNAL(textChanged()),this,SIGNAL(documentChanged()));
    connect(this->editor,SIGNAL(textChanged()),this,SLOT(documentModified()) );
    connect(this->editor,SIGNAL(undoAvailable(bool)),this,SIGNAL(undoAvailable(bool)));
    connect(this->editor,SIGNAL(redoAvailable(bool)),this,SIGNAL(redoAvailable(bool)));
    this->highlighter = new SyntaxHighlighter("Flex",this->editor->document());
    QStringListModel *words;
    this->setMinimumHeight(FL_DOCUMENT_MINIMUM_HEIGHT);
    this->setMinimumWidth(FL_DOCUMENT_MINIMUM_WIDTH);
    QFile FlexRuleFile(":/flexCodeCompletionRules.txt");
    FlexRuleFile.open(QIODevice::ReadOnly);
    QTextStream reader(&FlexRuleFile);
    QStringList *theList = new QStringList();
    while (!reader.atEnd())
    {
        theList->append(reader.readLine());
    }
    this->editor->setCompleterModel(theList);

}
