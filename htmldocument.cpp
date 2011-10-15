#include "htmldocument.h"
#include "document.h"
#include "codeeditor.h"
#include <QtWebKit>
#include "finddialog.h"
#include <QtUiTools>
#include "insertselectmenudialog.h"
#include "insertbuttongroup.h"
#include "insertregistrationform.h"
//#include <QXmlQuery>


//int Document::numberOfNewDocuments;

HTMLDocument::HTMLDocument(QString filepath, Foundry *parent)
{
    this->initialize();
    QUrl url = QUrl::fromLocalFile(filepath);
    this->initialize();
    QFile file(filepath);
    QTextStream reader(&file);
    QString fileContent = reader.readAll();
    this->code = fileContent;
    this->designArea->setUrl(url);
    this->setWindowTitle(this->fileName);
    Document::numberOfNewDocuments++;
    this->savedAtleastOnce = true;
    this->designArea->setFocus();
    this->setAttribute(Qt::WA_DeleteOnClose);
}


void HTMLDocument::initialize()
{
    
    this->tabArea = new QTabWidget(this);
    this->tabArea->setTabPosition(QTabWidget::South);
    this->tabArea->setTabShape(QTabWidget::Rounded);
    //the design view
    this->designArea = new QWebView(this->tabArea);
    this->webPage = this->designArea->page();
    this->webPage->setContentEditable(true);
    this->designArea->setPage(this->webPage);
    this->designArea->setHtml(this->code,QUrl::fromLocalFile("/"));
    this->setAttribute(Qt::WA_DeleteOnClose);
    //this->designArea->show();
    //the coder view
    this->editor = new CodeEditor(this->tabArea);
    this->editor->setPlainText(this->code);
    //the preview area
    this->previewArea = new QWebView(this->tabArea);
    //add all the three areas to the tab.
    this->tabArea->addTab(this->designArea,"Design");
    this->tabArea->addTab(this->editor,"Code");
    this->tabArea->addTab(this->previewArea,"Preview");
    this->setWidget(this->tabArea);
    this->setWindowTitle(QString("Untitled-%1").arg(Document::numberOfNewDocuments));
    this->tabArea->setCurrentIndex(Design);
    this->currentMode = Design;
    this->setWindowState(Qt::WindowMaximized);
    this->savedAtleastOnce = false;
    this->designArea->setFocus();
    this->changedSinceSave = false;
    this->connectSignals();
    this->show();
    this->findDialog = new FindDialog(this);

    connect(this->editor,SIGNAL(copyAvailable(bool)),this,SIGNAL(copyAvailable(bool)));
    connect(this->editor,SIGNAL(textChanged()),this,SIGNAL(documentChanged()));
    connect(this->editor,SIGNAL(textChanged()),this,SLOT(documentModified()) );
    connect(this->editor,SIGNAL(undoAvailable(bool)),this,SIGNAL(undoAvailable(bool)));
    connect(this->editor,SIGNAL(redoAvailable(bool)),this,SIGNAL(redoAvailable(bool)));
    this->connectDesignSignals();

    //signals for teh web page editor



}


QString HTMLDocument::fileContent(QString filePath)
{
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    if (file.exists())
    {
        QTextStream theTextFile(&file);
        QString theContent = theTextFile.readAll();
        return theContent;
    }
    else
    {
        return "File not found"; //for the mooment.
    }

}

void HTMLDocument::setupSave()
{
    switch (this->currentMode)
    {
        case Preview:
            break;
        case Design:
            this->code = this->designArea->page()->mainFrame()->toHtml();
            break;
        case Code:
            this->code = this->editor->toPlainText();
            break;
    }
    //this->designArea->set
}

void HTMLDocument::setContent(QString content)
{

}

void HTMLDocument::setActiveTab(int tabNumber)
{
    this->tabArea->setCurrentIndex(tabNumber);
    this->currentMode = tabNumber;
}

void HTMLDocument::viewModeChanged(int tabNumber)
{
    switch (tabNumber)
    {
        case 0:
        if (this->currentMode == Code)
        {
            this->code = this->editor->toPlainText();
            this->designArea->page()->mainFrame()->setHtml(this->editor->toPlainText());
        }
        else if (this->currentMode == Preview)
        {
            this->designArea->setHtml(this->code);
        }
        this->currentMode = Design;
        break;
        case 1: //going to code mode
        if (this->currentMode ==  Design)
        {
            QWebPage *page = this->designArea->page();
            this->code = page->mainFrame()->toHtml();
            this->editor->setPlainText(this->code);
            this->editor->setFocus();
        }
        else if (this->currentMode == Preview)
        {
            this->editor->setPlainText(this->code);
        }
        this->currentMode = Code;
        break;
        case 2:
        if (this->currentMode == Design)
        {
            QWebPage *page = this->designArea->page();
            this->code = page->mainFrame()->toHtml();
        }
        else if (this->currentMode == Code)
        {
            this->code = this->editor->toPlainText();
        }
        this->currentMode = Preview;
        this->previewArea->setHtml(this->code);
        break;
    }

}

void HTMLDocument::connectSignals()
{
    connect(this->tabArea,SIGNAL(currentChanged(int)),this,SLOT(viewModeChanged(int)));
}


void HTMLDocument::cut()
{
    if (this->currentMode == Design)
    {
        this->designArea->pageAction(QWebPage::Cut)->trigger();
    }
    else
    {
        this->editor->cut();
    }
}

void HTMLDocument::copy()
{
    if (this->currentMode == Design)
    {
        this->designArea->pageAction(QWebPage::Copy)->trigger();
    }
    else
    {
        this->editor->copy();
    }
}

void HTMLDocument::paste()
{
    if (this->currentMode == Design)
    {
        this->designArea->pageAction(QWebPage::Paste)->trigger();
    }
    else
    {
        this->editor->paste();
    }
}


void HTMLDocument::selectAll()
{
     if (this->currentMode == Design)
    {
        this->designArea->triggerPageAction(QWebPage::SelectAll);
    }
    else
    {
        this->editor->selectAll();
    }
}

void HTMLDocument::undo()
{
     if (this->currentMode == Design)
    {
        this->designArea->pageAction(QWebPage::Undo)->trigger();
    }
    else
    {
        this->editor->undo();
    }
}

void HTMLDocument::redo()
{
     if (this->currentMode == Design)
    {
        this->designArea->pageAction(QWebPage::Redo)->trigger();
    }
    else
    {
        this->editor->redo();
    }
}

void HTMLDocument::connectDesignSignals()
{
    connect(this->designArea->pageAction(QWebPage::Cut),SIGNAL(changed()),this,SLOT(adjustActions()));
}


//bug:

/*

  find case sensitively upwards in design mode
  remove case sensitivity and search downwards, case sensitivity seems to persist

  find next in design mode wont work as expected because the flags are not saved when searching
  */
void HTMLDocument::findText(QString searchKeyword,QTextDocument::FindFlags findflags)
{
    if (this->currentMode == Design)
    {
        QWebPage::FindFlags theFlags;
        QTextDocument::FindFlags tmp;
        tmp = findflags & 1;
        if (tmp == QTextDocument::FindBackward)
            theFlags |= QWebPage::FindBackward;
        tmp = findflags & 2;
        if (tmp == QTextDocument::FindCaseSensitively)
            theFlags |= QWebPage::FindCaseSensitively;
        //tmp = findflags & 4;
        //if (tmp == QTextDocument::F)
        //     theFlags |= QWebPage::FindCaseSensitively;
        this->designArea->findText(searchKeyword,theFlags);
        this->lastSearchFlags &= 0;
        this->lastSearchFlags |= findflags;
    }
    else
    {
        this->editor->find(searchKeyword,findflags);
        this->lastSearchKeyword = searchKeyword;
        this->lastSearchFlags &= 0;
        this->lastSearchFlags |= findflags;
    }

}

void HTMLDocument::findNext()
{

        this->findText(this->lastSearchKeyword,this->lastSearchFlags);
}


void HTMLDocument::adjustActions()
{
    if (this->designArea->pageAction(QWebPage::Cut)->isEnabled())
        emit this->copyAvailable(true);
    else
        emit this->copyAvailable(false);
    if (this->designArea->pageAction(QWebPage::Copy)->isEnabled())
        emit this->copyAvailable(true);
    else
        emit this->copyAvailable(false);
    if (this->designArea->pageAction(QWebPage::Undo)->isEnabled())
        emit this->undoAvailable(true);
    else
        emit this->undoAvailable(false);
    if (this->designArea->pageAction(QWebPage::Redo)->isEnabled())
        emit this->redoAvailable(true);
    else
        emit this->redoAvailable(false);
}

void HTMLDocument::execCommand(QString cmd, QString arg)
{
    QWebFrame *frame = this->designArea->page()->mainFrame();
    QString js = QString("document.execCommand(\"%1\", false, \"%2\")").arg(cmd).arg(arg);
    frame->evaluateJavaScript(js);
}


void HTMLDocument::execCommand(QString command)
{
    QWebFrame *frame = this->designArea->page()->mainFrame();
    QString js = QString("document.execCommand(\"%1\", false, null)").arg(command);
    frame->evaluateJavaScript(js);
}

void HTMLDocument::execute(QString operation,QString parameter)
{
    QWebFrame *frame = this->designArea->page()->mainFrame();
    QString js = QString("document.execCommand(\"%1\", false, \"%2\")").arg(operation).arg(parameter);
    frame->evaluateJavaScript(js);

}

void HTMLDocument::execute(QString command)
{
    QWebFrame *frame = this->designArea->page()->mainFrame();
    QString js = QString("document.execCommand(\"%1\", false, null)").arg(command);
    frame->evaluateJavaScript(js);
}


void HTMLDocument::bold()
{
    this->designArea->pageAction(QWebPage::ToggleBold)->trigger();
}

void HTMLDocument::italic()
{
    this->designArea->pageAction(QWebPage::ToggleItalic)->trigger();
}

void HTMLDocument::underline()
{
    this->designArea->pageAction(QWebPage::ToggleUnderline)->trigger();
}

void HTMLDocument::alignleft()
{
    this->execCommand("justifyLeft");
}

void HTMLDocument::alignright()
{
    this->execCommand("justifyRight");
}

void HTMLDocument::aligncenter()
{
    this->execCommand("justifyCenter");
}

void HTMLDocument::alignjustify()
{
    this->execCommand("justifyFull");
}


void HTMLDocument::increaseindent()
{
    execCommand("indent");
}

void HTMLDocument::decreaseindent()
{
    execCommand("outdent");
}
void HTMLDocument::listnumbered()
{
    execCommand("insertOrderedList");
}

void HTMLDocument::listbulleted()
{
    execCommand("insertUnorderedList");
}

void HTMLDocument::styleHeading1()
{
    execCommand("formatBlock", "h1");
}

void HTMLDocument::styleHeading2()
{
    execCommand("formatBlock", "h2");
}

void HTMLDocument::styleHeading3()
{
    execCommand("formatBlock", "h3");
}

void HTMLDocument::styleHeading4()
{
    execCommand("formatBlock", "h4");
}

void HTMLDocument::styleHeading5()
{
    execCommand("formatBlock", "h5");
}

void HTMLDocument::styleHeading6()
{
    execCommand("formatBlock", "h6");
}

void HTMLDocument::insertPasswordField()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/inserttextfield.ui"));
    QWidget *dialog = dialogShower.load(&dialogFile,this);
    QDialog *insertTextFieldDialog = (QDialog *) dialog;
    insertTextFieldDialog->setWindowTitle(QString("Insert Password Field"));
    if (insertTextFieldDialog->exec())
    {
         QLineEdit *nameField = insertTextFieldDialog->findChild<QLineEdit *>(QString("nameField"));
         QLineEdit *defaultTextField = insertTextFieldDialog->findChild<QLineEdit *>(QString("defaultTextField"));
         QLineEdit *classField = insertTextFieldDialog->findChild<QLineEdit *>(QString("classField"));
         QLineEdit *idField = insertTextFieldDialog->findChild<QLineEdit *>(QString("idField"));
         QString name = nameField->text();
         QString defaultText = defaultTextField->text();
         QString cssClass = classField->text();
         QString id = idField->text();
        QString htmlOfTextField = this->passwordField(name,defaultText,cssClass,id);
        this->insertHTMLAtCursor(htmlOfTextField);
    }
}

void HTMLDocument::insertTextField()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/inserttextfield.ui"));
    QWidget *dialog = dialogShower.load(&dialogFile,this);
    QDialog *insertTextFieldDialog = (QDialog *) dialog;
    if (insertTextFieldDialog->exec())
    {
         QLineEdit *nameField = insertTextFieldDialog->findChild<QLineEdit *>(QString("nameField"));
         QLineEdit *defaultTextField = insertTextFieldDialog->findChild<QLineEdit *>(QString("defaultTextField"));
         QLineEdit *classField = insertTextFieldDialog->findChild<QLineEdit *>(QString("classField"));
         QLineEdit *idField = insertTextFieldDialog->findChild<QLineEdit *>(QString("idField"));
         QString name = nameField->text();
         QString defaultText = defaultTextField->text();
         QString cssClass = classField->text();
         QString id = idField->text();
         QString htmlOfTextField = this->textField(name,defaultText,cssClass,id);
        this->insertHTMLAtCursor(htmlOfTextField);
    }
}
void HTMLDocument::insertTextArea()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/inserttextarea.ui"));
    QWidget *dialog = dialogShower.load(&dialogFile,this);
    QDialog *insertTextFieldDialog = (QDialog *) dialog;
    if (insertTextFieldDialog->exec())
    {
         QLineEdit *nameField = insertTextFieldDialog->findChild<QLineEdit *>(QString("nameField"));
         QTextEdit *defaultTextField = insertTextFieldDialog->findChild<QTextEdit *>(QString("defaultTextField"));
         QLineEdit *classField = insertTextFieldDialog->findChild<QLineEdit *>(QString("classField"));
         QLineEdit *idField = insertTextFieldDialog->findChild<QLineEdit *>(QString("idField"));
         QLineEdit *rowField = insertTextFieldDialog->findChild<QLineEdit *>(QString("rowField"));
         QLineEdit *columnField = insertTextFieldDialog->findChild<QLineEdit *>(QString("columnField"));
         QString name = nameField->text();
         QTextDocument *doc = defaultTextField->document();
         QString defaultText = doc->toPlainText();
         QString cssClass = classField->text();
         QString id = idField->text();
         QString rows = rowField->text();
         QString columns = columnField->text();

         QString htmlOfTextArea = this->textArea(name,defaultText,rows,columns,cssClass,id);
         this->insertHTMLAtCursor(htmlOfTextArea);
    }
}

void HTMLDocument::insertRadioButton()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/insertradiobutton.ui"));
    QWidget *dialog = dialogShower.load(&dialogFile,this);
    QDialog *insertRadioButton= (QDialog *) dialog;
    insertRadioButton->setWindowTitle("Insert Radio Button");
    if (insertRadioButton->exec())
    {
        QLineEdit *nameField =  insertRadioButton->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *valueField =  insertRadioButton->findChild<QLineEdit *>(QString("valueField"));
        QLineEdit *idField =  insertRadioButton->findChild<QLineEdit *>(QString("idField"));
        QLineEdit *labelTextField =  insertRadioButton->findChild<QLineEdit *>(QString("labelTextField"));
        QCheckBox *selectedField =  insertRadioButton->findChild<QCheckBox *>(QString("selectedField"));
        QString name = (nameField->text().isEmpty())?"":nameField->text();
        QString value = valueField->text().isEmpty()?"":valueField->text();
        QString id = idField->text().isEmpty()?"":idField->text();
        QString textLabel = labelTextField->text().isEmpty()?"":labelTextField->text();

        QString selected = (selectedField->checkState() == Qt::Checked)?"checked=\"checked\"":"";

        QString htmlOfRadioButton = QString("%1%2").arg(this->radioButton(name,value,"",id,selectedField->checkState() == Qt::Checked)).arg(textLabel);
        this->insertHTMLAtCursor(htmlOfRadioButton);
    }

}

void HTMLDocument::insertSubmitButton()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/insertbutton.ui"));
    QWidget *dialog = dialogShower.load(&dialogFile,this);
    QDialog *insertRadioButton= (QDialog *) dialog;
    insertRadioButton->setWindowTitle("Insert Submit Button");
    if (insertRadioButton->exec())
    {
      QLineEdit *nameField =  insertRadioButton->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *valueField =  insertRadioButton->findChild<QLineEdit *>(QString("valueField"));
        QLineEdit *idField =  insertRadioButton->findChild<QLineEdit *>(QString("idField"));
        QString name = (nameField->text().isEmpty())?"":nameField->text();
        QString value = valueField->text().isEmpty()?"":valueField->text();
        QString id = idField->text().isEmpty()?"":idField->text();
        QString htmlOfSubmitButton = this->submitButton(name,value,"",id);
        this->insertHTMLAtCursor(htmlOfSubmitButton);
    }

}

void HTMLDocument::insertButton()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/insertbutton.ui"));
    QWidget *dialog = dialogShower.load(&dialogFile,this);
    QDialog *insertRadioButton= (QDialog *) dialog;
    insertRadioButton->setWindowTitle("Insert Button");
    if (insertRadioButton->exec())
    {
        QLineEdit *nameField =  insertRadioButton->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *valueField =  insertRadioButton->findChild<QLineEdit *>(QString("valueField"));
        QLineEdit *idField =  insertRadioButton->findChild<QLineEdit *>(QString("idField"));
        QString name = (nameField->text().isEmpty())?"":nameField->text();
        QString value = valueField->text().isEmpty()?"":valueField->text();
        QString id = idField->text().isEmpty()?"":idField->text();
        QString htmlOfButton = this->button(name,value,"",id);

        this->insertHTMLAtCursor(htmlOfButton);
    }

}
void HTMLDocument::insertSelectMenu()
{
    InsertSelectMenuDialog *dialog = new InsertSelectMenuDialog();
    dialog->setWindowTitle(QString("Insert Select Menu"));
    if (dialog->exec())
    {
        QMap <QString,QString> selectOptions = dialog->getData();
        QString name = dialog->nameField->text();
        QString id = dialog->idField->text();
        QString cssClass = dialog->classField->text();
        QString htmlOfSelect = this->selectMenu(selectOptions,name,cssClass,id);
        this->insertHTMLAtCursor(htmlOfSelect);
    }
}

void HTMLDocument::insertJavaApplet()
{

}
void HTMLDocument::insertCheckboxGroup()
{
    InsertButtonGroup buttonGroup;
    buttonGroup.setWindowTitle(QString("Insert Checkbox Group"));
    if (buttonGroup.exec())
    {

        QMap <QString , QString > list = buttonGroup.getList();

        int num = list.size();

        QStringList keys = list.keys();

        QString name = buttonGroup.name->text();

        QString cssclass = buttonGroup.cssclass->text();

        QString html="";

        for (int curr = 0; curr < keys.size(); curr++)
        {
            qDebug(list[keys.at(curr)].toAscii());
            html.append(QString("<input type=\"checkbox\" name=\"%1\" class=\"%2\" value=\"%3\" />%4 <br />").arg(name).arg(cssclass).arg(list[keys.at(curr)]).arg(keys.at(curr)));
        }

        this->insertHTMLAtCursor(html);
    }

}

void HTMLDocument::attachjQuery(bool silent)
{
    if (this->savedAtleastOnce)
    {

        //get the path
        QString fullfilename = this->fileName;
        QStringList pathElements = fullfilename.split("/");
        QString file = pathElements.at(pathElements.size()-1);
        QString path = fullfilename.replace(file,"",Qt::CaseSensitive);

        if (!QFile::copy(":/jquery.js",path + "/jquery.js"))
        {
            QMessageBox::critical(this,"Failed","Attaching jQuery failed as a jquery file could not be written to the directory");
            return;
        }

        //QXmlQuery query;
        QTemporaryFile thefile(this->code);
        //add the markup to the web page
        //the head tag has to be located
        //the script tag has to be added
        //


    }
    else
        qDebug("False!");


}

void HTMLDocument::insertRadioGroup()
{
    InsertButtonGroup buttonGroup;
    buttonGroup.setWindowTitle(QString("Insert Checkbox Group"));
    if (buttonGroup.exec())
    {

        QMap <QString , QString > list = buttonGroup.getList();

        int num = list.size();

        QStringList keys = list.keys();

        QString name = buttonGroup.name->text();

        QString cssclass = buttonGroup.cssclass->text();

        QString html="";

        for (int curr = 0; curr < keys.size(); curr++)
        {
            qDebug(list[keys.at(curr)].toAscii());
            html.append(QString("%1%2<br />").arg(this->radioButton(name,list[keys.at(curr)],"","",false)).arg(keys.at(curr)));
            //html.append(QString("<input type=\"radio\" name=\"%1\" class=\"%2\" value=\"%3\" />%4 <br />").arg(name).arg(cssclass).arg(list[keys.at(curr)]).arg(keys.at(curr)));
        }

        this->insertHTMLAtCursor(html);
    }

}
void HTMLDocument::insertCheckBox()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/insertradiobutton.ui"));
    QWidget *dialog = dialogShower.load(&dialogFile,this);
    QDialog *insertRadioButton= (QDialog *) dialog;
    insertRadioButton->setWindowTitle("Insert CheckBox");
    if (insertRadioButton->exec())
    {
        QLineEdit *nameField =  insertRadioButton->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *valueField =  insertRadioButton->findChild<QLineEdit *>(QString("valueField"));
        QLineEdit *idField =  insertRadioButton->findChild<QLineEdit *>(QString("idField"));
        QLineEdit *labelTextField =  insertRadioButton->findChild<QLineEdit *>(QString("labelTextField"));
        QCheckBox *selectedField =  insertRadioButton->findChild<QCheckBox *>(QString("selectedField"));
        QString name = (nameField->text().isEmpty())?"":nameField->text();
        QString value = valueField->text().isEmpty()?"":valueField->text();
        QString id = idField->text().isEmpty()?"":idField->text();
        QString textLabel = labelTextField->text().isEmpty()?"":labelTextField->text();
        QString selected = (selectedField->checkState() == Qt::Checked)?"checked=\"checked\"":"";
        QString htmlOfRadioButton =  QString("%1<label for=\"%2\">%3</label>").arg(this->checkBox(name,value,"",id,selectedField->checkState())).arg(id).arg(textLabel);
        this->insertHTMLAtCursor(htmlOfRadioButton);
    }

}

void HTMLDocument::insertImage()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Select Image File..",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));

    if (filePath.isEmpty())
        return;
    if (!QFile::exists(filePath))
        return;
    QUrl fp = QUrl::fromLocalFile(filePath);
    this->execCommand("insertImage",fp.toString());
}

void HTMLDocument::insertLink()
{
    if (this->designArea->pageAction(QWebPage::Copy)->isEnabled()) // if text is selected, copy will be available
    {
        QString url = QInputDialog::getText(this,"Link URL","Enter the link address:");
        if (!url.isEmpty())
            this->execCommand("createLink",url);
        else
            return;
    }
    else
    {
        QUiLoader loader;
        QFile uifile(":/insertlink.ui");
        QDialog *dialog = (QDialog *) loader.load(&uifile);
        dialog->setWindowTitle("Insert Link");
        if (dialog->exec())
        {
            QLineEdit *textField = dialog->findChild<QLineEdit *>(QString("linkTextField"));
            QLineEdit *urlField = dialog->findChild<QLineEdit *>(QString("linkAddressField"));
            QString text = textField->text();
            QString url = urlField->text();
            QString html = this->link(text,url);
            this->insertHTMLAtCursor(html);
        }
    }

}

void HTMLDocument::insertSubscript()
{
    this->execCommand("subscript");
}

void HTMLDocument::insertSuperscript()
{
    this->execCommand("superscript");
}

void HTMLDocument::insertAnchor()
{
    QString text = QInputDialog::getText(this,"Anchor","Name of anchor:");
    QString html = this->getSelectedText();


    this->execCommand("delete");

    QString newhtml = "<a name=\""+text+"\">"+html+"</a>";
    //QString newhtml = "";

    this->insertHTMLAtCursor(newhtml);
}

void HTMLDocument::doAction(QAction *actionToBePerformed)
{
    QString whattodo = actionToBePerformed->text();
    if (whattodo == "Bold")
        this->bold();
    if (whattodo == "Italic")
        this->italic();
    if (whattodo == "Underline")
        this->underline();
    if (whattodo == "Align Left")
        this->alignleft();
    if (whattodo == "Align Right")
        this->alignright();;
    if (whattodo == "Align Center")
        this->aligncenter();
    if (whattodo == "Align Justify")
        this->alignjustify();
    if (whattodo == "List Bulleted" || whattodo == "Bulleted List")
        this->listbulleted();
    if (whattodo == "List Numbered" || whattodo == "Numbered List")
        this->listnumbered();
    if (whattodo == "Increase Indent")
        this->increaseindent();
    if (whattodo == "Decrease Indent")
        this->decreaseindent();
    if (whattodo == "Undo")
        this->undo();
    if (whattodo == "Redo")
        this->redo();;
    if (whattodo == "Cut")
        this->cut();
    if (whattodo == "Copy")
        this->copy();
    if (whattodo == "Paste")
        this->paste();
    if (whattodo == "Select All")
        this->selectAll();
    if (whattodo == "Find")
        this->find();
    if (whattodo == "Close")
        this->close();
    if (whattodo == "Text Field")
        this->insertTextField();
    if (whattodo == "Text Area")
        this->insertTextArea();
    if (whattodo == "Password")
        this->insertPasswordField();
    if (whattodo == "Radio Button")
        this->insertRadioButton();
    if (whattodo == "Check Box")
        this->insertCheckBox();
    if (whattodo == "Button")
        this->insertButton();
    if (whattodo == "Submit Button")
        this->insertSubmitButton();
    if (whattodo == "Select Menu")
        this->insertSelectMenu();
    if (whattodo == "Check Box Group")
        this->insertCheckboxGroup();
    if (whattodo == "Radio Button Group")
        this->insertRadioGroup();
    if (whattodo == "Image")
        this->insertImage();
    if (whattodo == "Link")
        this->insertLink();
    if (whattodo == "Named Anchor")
        this->insertAnchor();
    if (whattodo == "Subscript")
            this->insertSubscript();
if (whattodo == "Superscript")
        this->insertSuperscript();
   if (whattodo == "Attach jQuery")
       this->attachjQuery(false);
   if (whattodo == "Registration Form")
       this->insertRegistrationForm();
   if (whattodo == "Comment Form")
       this->insertCommentForm();
    if (whattodo == "Login Form")
       this->insertLoginForm();
    if (whattodo == "Search Form")
       this->insertSearchForm();
    if (whattodo == "Mailing List Optin Form")
       this->insertMailingListOptin();
    if (whattodo == "Table")
       this->insertTable();
}

void HTMLDocument::insertCommentForm()
{
    QString text = QString("<table><tr><td>Name:</td><td>%1</td></tr><tr><td>Website: </td><td>%2</td></tr><tr><td>Email Address:</td><td>%3</td></tr><tr><td colspan=2>Comment:</br>%4</td></tr><tr><td colspan=2>%5</td></table>").arg(this->textField("name","","","")).arg(this->textField("website","","","")).arg(this->textField("email","","","")).arg(this->textArea("comment","","10","50","","")).arg(this->submitButton("Comment","Comment","",""));
    this->insertHTMLAtCursor(text);
}
void HTMLDocument::insertLoginForm()
{
    QString text = QString("<table><tr><td>Username:</td><td>%1</td></tr><tr><td>Password:</td><td>%2</td></tr><tr><td colspan=2>%3</td></tr></table>").arg(this->textField("username","","","")).arg(this->passwordField("password","","","")).arg(this->submitButton("Submit","Submit","",""));
    this->insertHTMLAtCursor(text);
}


void HTMLDocument::insertSearchForm()
{
    QString text = QString("<table><tr><td>Search:</td><td>%1</td><td colspan=2>%2</td></tr></table>").arg(this->textField("keyword","","","")).arg(this->submitButton("Submit","Search","",""));
    this->insertHTMLAtCursor(text);
}

void HTMLDocument::insertMailingListOptin()
{
    QString text = QString("<table><tr><td>Name:</td><td>%1</td></tr><tr><td>Email Address:</td><td>%2</td></tr><tr><td colspan=2>%3</td></tr></table>").arg(this->textField("name","","","")).arg(this->textField("email","","","")).arg(this->submitButton("Submit","Submit","",""));
    this->insertHTMLAtCursor(text);
}

void HTMLDocument::insertTable()
{
    QUiLoader loader;
    QFile dialogfile(":/inserttable.ui");
    dialogfile.open(QIODevice::ReadOnly);
    QDialog *dialog = (QDialog *) loader.load(&dialogfile,this);
    if (dialog->exec())
    {
        QLineEdit *rowsField= dialog->findChild<QLineEdit *> ("rows");
        QLineEdit *colsField= dialog->findChild<QLineEdit *> ("cols");
        QString  rows= rowsField->text();
        QString  cols= colsField->text();
        int rowc =  rows.toInt();
        int colc = cols.toInt();
        QString html = "<table border=\"1\" width=\"500\"  cellspacing=\"1\" cellpadding=\"1\">";
        for (int currrow=0;currrow<rowc;currrow++)
        {
            html.append("<tr>");
            for (int currcol=0;currcol<colc;currcol++)
            {
                html.append("<td>&nbsp;</td>");
            }
            html.append("</tr>");
        }
        html.append("</table>");
        this->insertHTMLAtCursor(html);

    }

}

void HTMLDocument::insertRegistrationForm()
{
    InsertRegistrationForm *dialog = new InsertRegistrationForm();
    if (dialog->exec())
    {
        int numOfItems = dialog->smallList->count();
        QStringList list;
        for (int curr=0;curr<numOfItems;curr++)
        {
            list.append(dialog->smallList->item(curr)->text());

        }
        QString currItem;
        QString html = "<form><table>";
        QStringList oneliners;
        oneliners.append("First Name");
        oneliners.append("First Name");
        oneliners.append("Last Name");
        oneliners.append("Full Name");
        oneliners.append("Surname");
        oneliners.append("Email-Address");
        oneliners.append("City");
        oneliners.append("State");        
        oneliners.append("Username");
        oneliners.append("Website URL");
        oneliners.append("Postal Code");
         QFile file(":/countryfield.txt");
        for (int curr =0;curr <numOfItems;curr++)
        {
            currItem = list.at(curr).trimmed();
            if (oneliners.contains(currItem))
            {
                html.append("<tr>");
                html.append(QString("<td>%1:</td><td>%2</td>").arg(currItem).arg(this->textField(QString("fname"),QString(""),QString(""),QString(""))));
                html.append("</tr>");
            }
            else if (currItem == "Gender")
            {
                html.append("<tr>");
                QString male = this->radioButton(QString("gender"),QString("male"),QString(""),QString(""),true);
                QString female = this->radioButton(QString("gender"),QString("female"),QString(""),QString(""),false);
                html.append(QString("<td>%1:</td><td>%2 Male  %3 Female</td>").arg("Gender").arg(male).arg(female));
                html.append("</tr>");
            }
            else if (currItem == "Password")
            {
                QString pass = this->passwordField(QString("password"),QString(""),QString(""),QString(""));
                QString rpass = this->passwordField(QString("rpassword"),QString(""),QString(""),QString(""));

                html.append("<tr>");
                html.append(QString("<td>Password: </td><td>%1</td>").arg(pass));
                html.append("</tr>");
                html.append("<tr>");
                html.append(QString("<td>Retype Password:</td><td>%2</td>").arg(rpass));
                html.append("</tr>");
            }
            else if (currItem.trimmed() == "Country Field")
            {
               QStringList countryList;
               QFile thefile(":/countrylist.txt");
               thefile.open(QIODevice::ReadOnly);
               QTextStream reader(&thefile);
               QString countryfield;
               while (!reader.atEnd())
               {
                   countryList.append(reader.readLine());
               }
               countryfield = "<select name=\"country\">";
               for (int curr=0;curr<countryList.size();curr++)
               {
                   countryfield.append(QString("<option>%1</option>").arg(countryList.at(curr)));
               }
               countryfield.append("</select>");
                html.append(QString("<tr><td>Country: </td><td>%1</td>").arg(countryfield));
            }
            else if (currItem == "Date Of Birth")
            {
                QStringList numbers;
                for (int curr=1;curr<=31;curr++)
                {
                    numbers.append(QString("%1").arg(curr));
                }
                QString dateField = this->selectMenu(numbers,"dobdate","","");
                QStringList months;
                months << "January" << "February" << "March" << "April" << "May" << "June" << "July" <<
                        "August" << "September" << "October" << "November" << "December";
                QString monthField = this->selectMenu(months,"dobmonth","","");
                int currentYear = QDate::currentDate().year();
                QStringList yearlist;
                for (int curr=1930;curr< currentYear;curr++) //would people want one year old members?
                {
                    yearlist.append(QString("%1").arg(curr));
                }
                QString yearField = this->selectMenu(yearlist,"dobyear","","");
                html.append(QString("<tr><td>Date Of Birth</td><td>%1 %2 %3</td></tr>").arg(dateField).arg(monthField).arg(yearField));


            }
            else if (currItem == "Submit")
            {
                QString submit = this->submitButton(QString("submit"),"Submit","","");
                QString reset = "<input type=\"reset\" value=\"Clear Form\">";
                html.append(QString("<tr><td>%1 %2</td>").arg(submit).arg(reset) );

            }

        }
        html.append("</table></form>");
        this->insertHTMLAtCursor(html);
        //qDebug(html.toAscii());
    }

}



QString HTMLDocument::getSelectedText()
{
     if (this->currentMode == Design)
    {
           QString selectedText = this->designArea->page()->selectedText();
           return selectedText;
    }
     else
     {
         return this->codeArea->textCursor().selectedText();
     }
     return "";
}

void HTMLDocument::insertHTMLAtCursor(QString htmlToInsert)
{
    Modes currentMode = (Modes )this->currentMode;

    switch (currentMode)
    {
        case Design:

            htmlToInsert = htmlToInsert.replace(QRegExp("\""),"\\\"");
            //qDebug(htmlToInsert.toAscii());
            this->execCommand("insertHTML",htmlToInsert);

            break;
        case Code:

            this->editor->insertPlainText(htmlToInsert);
            break;
        case Preview:
            return;
            break;
    }
}



QString HTMLDocument::getType()
{
    return QString("HTML");
}


/***********HTML Functions**************/

QString HTMLDocument::checkBox(QString name,QString value,QString cssclass="",QString id="",bool checked=false)
{
    return QString("<input type=\"checkbox\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" %5 />").arg(name).arg(value).arg(cssclass).arg(id).arg((checked)?"checked=\"checked\"":"");
}


QString HTMLDocument::radioButton(QString name,QString value,QString cssclass="",QString id="",bool checked=false)
{
    return QString("<input type=\"radio\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" %5 />").arg(name).arg(value).arg(cssclass).arg(id).arg((checked)?"checked=\"checked\"":"");
}

QString HTMLDocument::textField(QString name,QString value="",QString cssclass="",QString id="")
{
    return QString("<input type=\"text\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" />").arg(name).arg(value).arg(cssclass).arg(id);
}

QString HTMLDocument::passwordField(QString name,QString value,QString cssclass="",QString id="")
{
    return QString("<input type=\"password\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" />").arg(name).arg(value).arg(cssclass).arg(id);
}


QString HTMLDocument::submitButton(QString name,QString value,QString cssclass="",QString id="")
{
    return QString("<input type=\"submit\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" />").arg(name).arg(value).arg(cssclass).arg(id);
}


QString HTMLDocument::button(QString name,QString value,QString cssclass="",QString id="")
{
    return QString("<input type=\"button\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" />").arg(name).arg(value).arg(cssclass).arg(id);
}

QString HTMLDocument::selectMenu(QStringList items,QString name,QString cssclass="",QString id="")
{
    QString html = QString("<select name=\"%1\" class=\"%2\" id=\"%3\">").arg(name).arg(cssclass).arg(id);
    int num=items.size();
    qDebug("here");
    for (int curr= 0; curr < num;curr++)
       html.append(QString("<option>%1</option>").arg(items.at(curr)));
    qDebug("here");
    html.append("</select>");
    qDebug("here");
    return html;
}

QString HTMLDocument::selectMenu(QMap <QString,QString> list,QString name,QString cssclass,QString id)
{
    QString html = QString("<select name=\"%1\" class=\"%2\" id=\"%3\">").arg(name).arg(cssclass).arg(id);
    QStringList keys = list.keys();
    int num = keys.size();
    for (int curr=0;curr<num;curr++)
    {
        html.append(QString("<option value=\"%1\">%2</option>").arg(list[keys.at(curr)]).arg(keys.at(curr)));
    }
    html.append("</select>");
    return html;


}

QString HTMLDocument::textArea(QString name,QString value,QString rows,QString cols,QString cssclass,QString id)
{

    return QString("<textarea name=\"%1\" rows=\"%2\" cols=\"%3\" class=\"%4\" id =\"%5\">%6</textarea>").arg(name).arg(rows).arg(cols).arg(cssclass).arg(id).arg(value);
}


QString HTMLDocument::link(QString linktext,QString href)
{
    return QString("<a href=\"%1\">%2</a>").arg(href).arg(linktext);
}
/******************/
