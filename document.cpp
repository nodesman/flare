#include "document.h"
#include "codeeditor.h"
#include "finddialog.h"

int Document::numberOfNewDocuments;
Document::Document()
{
}
Document::Document(QString filename, QWidget *parent)
{
    this->initialize();
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream reader(&file);
        QString content = reader.readAll();
        this->setWidget(this->editor);
        this->editor->setPlainText(content);
        this->setAttribute(Qt::WA_DeleteOnClose);
        this->activateWindow();
    }
    else
    {
        QMessageBox::critical(parent,"Unable to Create File","A file could not be created at the specified location.Please choose a different location.");
        this->destroy(true,true);
    }
}

QString Document::getType()
{
    return QString("Unknown");
}

 void Document::initialize()
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

 void Document::undo()
 {
     this->editor->undo();
 }

 void Document::redo()
 {
     this->editor->redo();
 }

 void Document::cut()
{
    this->editor->cut();
}

 void Document::copy()
{
    this->editor->copy();
}

 void Document::paste()
{
    this->editor->paste();
}

 void Document::selectAll()
{
    this->editor->selectAll();
}

 void Document::setupSave()
{
    this->code = this->editor->toPlainText();
}

 void Document::find()
{
    connect(this->findDialog,SIGNAL(findText(QString,QTextDocument::FindFlags)),this,SLOT(findText(QString,QTextDocument::FindFlags)));
    this->findDialog->show();
}


void Document::findText(QString keyword,QTextDocument::FindFlags theFlags)
{
    this->lastSearchKeyword = keyword;
    this->lastSearchFlags = theFlags;
    this->editor->find(keyword,theFlags);
}

void Document::findNext()
{
    this->editor->find(this->lastSearchKeyword,this->lastSearchFlags);
}


void Document::saveDocument(QString filepath)
{
    this->setupSave();
    QFile theFile(filepath);
    theFile.open(QIODevice::WriteOnly);
    QTextStream writer(&theFile);
    theFile.write(this->code.toAscii());
    this->setWindowTitle(theFile.fileName());
    theFile.close();
    this->savedAtleastOnce = true;
}

void Document::saveDocument()
{
    if (!this->fileName.isEmpty())
        this->saveDocument(this->fileName);
    else
    {
        QString filePath = QFileDialog::getSaveFileName(this,"Save As");
        this->saveDocument(filePath);
        this->fileName = filePath;
    }
}

void Document::setFilePath(QString filepath)
{
    this->fileName = filepath;
}

bool Document::getSaved()
{
    return this->savedAtleastOnce;
}

bool Document::getChanged()
{
    return this->changedSinceSave;
}

void Document::setSaved(bool savedStatus)
{
    this->savedAtleastOnce = savedStatus;
}

void Document::setChanged(bool changeStatus)
{
    this->changedSinceSave = changeStatus;
}


void Document::closeEvent(QCloseEvent *closeEvent)
{
    if (this->changedSinceSave)
    {
        QFile theFile(this->fileName);
        QString filename = theFile.fileName();
        QMessageBox::StandardButton answer = QMessageBox::question(this,"Save Changes","Do you want to save changes to " + this->windowTitle() + "?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (answer == QMessageBox::Yes)

        {
            if (this->savedAtleastOnce)
                this->saveDocument(this->fileName);
            else
            {
                QString fileName = QFileDialog::getSaveFileName(this,"Save File..");
                this->saveDocument(fileName);
            }
            closeEvent->accept();

        }
        else if (answer == QMessageBox::No)
            closeEvent->accept();
        else if (answer == QMessageBox::Cancel)
            closeEvent->ignore();
    }

}

void Document::documentModified()
{
    this->changedSinceSave = true;

}


void Document::insertAtCursor(QString textToInsert)
{
    QTextCursor theCursor(this->editor->textCursor());
    theCursor.select(QTextCursor::WordUnderCursor);
    theCursor.removeSelectedText();
    this->editor->insertPlainText(textToInsert);
}
