#include "htmldocument.h"

#include <QSignalBlocker>
#include <QStandardPaths>
#include <QtUiTools/QUiLoader>

#include "insertbuttongroup.h"
#include "insertregistrationform.h"
#include "insertselectmenudialog.h"

HTMLDocument::HTMLDocument(QString filepath, Foundry *parent)
{
    Q_UNUSED(parent);

    initialize();

    if (!filepath.isEmpty()) {
        this->fileName = filepath;
        this->code = fileContent(filepath);
        this->savedAtleastOnce = true;
        this->changedSinceSave = false;
        this->designArea->setHtml(this->code);
        this->editor->setPlainText(this->code);
        this->previewArea->setHtml(this->code);
        this->setWindowTitle(QFileInfo(filepath).fileName());
    }
}

void HTMLDocument::initialize()
{
    ++Document::numberOfNewDocuments;

    this->savedAtleastOnce = false;
    this->changedSinceSave = false;
    this->currentMode = Design;
    this->findDialog = new FindDialog(this);

    this->tabArea = new QTabWidget(this);
    this->tabArea->setTabPosition(QTabWidget::South);
    this->tabArea->setTabShape(QTabWidget::Rounded);

    this->designArea = new QTextEdit(this->tabArea);
    this->designArea->setAcceptRichText(true);

    this->editor = new CodeEditor(this->tabArea);

    this->previewArea = new QTextBrowser(this->tabArea);
    this->previewArea->setOpenExternalLinks(true);

    this->tabArea->addTab(this->designArea, "Design");
    this->tabArea->addTab(this->editor, "Code");
    this->tabArea->addTab(this->previewArea, "Preview");

    this->setWidget(this->tabArea);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle(QString("Untitled-%1").arg(Document::numberOfNewDocuments));

    connectSignals();
    connectDesignSignals();
}

QString HTMLDocument::fileContent(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    QTextStream reader(&file);
    return reader.readAll();
}

void HTMLDocument::setContent(QString content)
{
    this->code = content;
    {
        QSignalBlocker blockDesign(this->designArea);
        QSignalBlocker blockCode(this->editor);
        this->designArea->setHtml(content);
        this->editor->setPlainText(content);
    }
    this->previewArea->setHtml(content);
}

void HTMLDocument::setActiveTab(int tabNumber)
{
    this->tabArea->setCurrentIndex(tabNumber);
}

void HTMLDocument::connectSignals()
{
    connect(this->tabArea, SIGNAL(currentChanged(int)), this, SLOT(viewModeChanged(int)));
    connect(this->editor, SIGNAL(copyAvailable(bool)), this, SIGNAL(copyAvailable(bool)));
    connect(this->editor, SIGNAL(textChanged()), this, SIGNAL(documentChanged()));
    connect(this->editor, SIGNAL(textChanged()), this, SLOT(documentModified()));
    connect(this->editor, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(this->editor, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
}

void HTMLDocument::connectDesignSignals()
{
    connect(this->designArea, SIGNAL(copyAvailable(bool)), this, SIGNAL(copyAvailable(bool)));
    connect(this->designArea->document(), SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(this->designArea->document(), SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
    connect(this->designArea, SIGNAL(textChanged()), this, SIGNAL(documentChanged()));
    connect(this->designArea, SIGNAL(textChanged()), this, SLOT(documentModified()));
}

void HTMLDocument::syncFromCurrentMode()
{
    switch (this->currentMode) {
    case Design:
        this->code = this->designArea->toHtml();
        break;
    case Code:
        this->code = this->editor->toPlainText();
        break;
    case Preview:
        break;
    }
}

void HTMLDocument::setupSave()
{
    syncFromCurrentMode();
}

void HTMLDocument::viewModeChanged(int tabNumber)
{
    syncFromCurrentMode();

    if (tabNumber == Design) {
        QSignalBlocker blocker(this->designArea);
        this->designArea->setHtml(this->code);
        this->designArea->setFocus();
    } else if (tabNumber == Code) {
        QSignalBlocker blocker(this->editor);
        this->editor->setPlainText(this->code);
        this->editor->setFocus();
    } else if (tabNumber == Preview) {
        this->previewArea->setHtml(this->code);
    }

    this->currentMode = tabNumber;
    adjustActions();
}

void HTMLDocument::cut()
{
    if (this->currentMode == Design) {
        this->designArea->cut();
    } else if (this->currentMode == Code) {
        this->editor->cut();
    }
}

void HTMLDocument::copy()
{
    if (this->currentMode == Design) {
        this->designArea->copy();
    } else if (this->currentMode == Code) {
        this->editor->copy();
    } else {
        this->previewArea->copy();
    }
}

void HTMLDocument::paste()
{
    if (this->currentMode == Design) {
        this->designArea->paste();
    } else if (this->currentMode == Code) {
        this->editor->paste();
    }
}

void HTMLDocument::selectAll()
{
    if (this->currentMode == Design) {
        this->designArea->selectAll();
    } else if (this->currentMode == Code) {
        this->editor->selectAll();
    } else {
        this->previewArea->selectAll();
    }
}

void HTMLDocument::undo()
{
    if (this->currentMode == Design) {
        this->designArea->undo();
    } else if (this->currentMode == Code) {
        this->editor->undo();
    }
}

void HTMLDocument::redo()
{
    if (this->currentMode == Design) {
        this->designArea->redo();
    } else if (this->currentMode == Code) {
        this->editor->redo();
    }
}

void HTMLDocument::findText(QString searchKeyword, QTextDocument::FindFlags findflags)
{
    this->lastSearchKeyword = searchKeyword;
    this->lastSearchFlags = findflags;

    if (this->currentMode == Design) {
        this->designArea->find(searchKeyword, findflags);
    } else if (this->currentMode == Code) {
        this->editor->find(searchKeyword, findflags);
    } else {
        this->previewArea->find(searchKeyword, findflags);
    }
}

void HTMLDocument::findNext()
{
    findText(this->lastSearchKeyword, this->lastSearchFlags);
}

void HTMLDocument::adjustActions()
{
    if (this->currentMode == Design) {
        emit this->copyAvailable(this->designArea->textCursor().hasSelection());
        emit this->undoAvailable(this->designArea->document()->isUndoAvailable());
        emit this->redoAvailable(this->designArea->document()->isRedoAvailable());
    } else if (this->currentMode == Code) {
        emit this->copyAvailable(this->editor->textCursor().hasSelection());
        emit this->undoAvailable(this->editor->document()->isUndoAvailable());
        emit this->redoAvailable(this->editor->document()->isRedoAvailable());
    } else {
        emit this->copyAvailable(this->previewArea->textCursor().hasSelection());
        emit this->undoAvailable(false);
        emit this->redoAvailable(false);
    }
}

void HTMLDocument::mergeDesignCharFormat(const QTextCharFormat &format)
{
    if (this->currentMode != Design) {
        return;
    }

    QTextCursor cursor = this->designArea->textCursor();
    cursor.mergeCharFormat(format);
    this->designArea->mergeCurrentCharFormat(format);
}

void HTMLDocument::applyBlockTag(const QString &tagName)
{
    if (this->currentMode != Design) {
        return;
    }

    QTextCursor cursor = this->designArea->textCursor();
    QString selected = cursor.selectedText();
    if (selected.isEmpty()) {
        selected = tr("Heading");
    }

    cursor.insertHtml(QString("<%1>%2</%1>").arg(tagName, selected.toHtmlEscaped()));
}

void HTMLDocument::execCommand(QString command, QString options)
{
    if (command == "formatBlock") {
        applyBlockTag(options);
        return;
    }

    if (command == "insertImage") {
        this->designArea->textCursor().insertImage(options);
        return;
    }

    if (command == "createLink") {
        QString selected = getSelectedText();
        if (selected.isEmpty()) {
            selected = options;
        }
        insertHTMLAtCursor(link(selected, options));
        return;
    }

    if (command == "insertHTML") {
        insertHTMLAtCursor(options);
    }
}

void HTMLDocument::execCommand(QString command)
{
    if (command == "justifyLeft") {
        this->designArea->setAlignment(Qt::AlignLeft);
    } else if (command == "justifyRight") {
        this->designArea->setAlignment(Qt::AlignRight);
    } else if (command == "justifyCenter") {
        this->designArea->setAlignment(Qt::AlignHCenter);
    } else if (command == "justifyFull") {
        this->designArea->setAlignment(Qt::AlignJustify);
    } else if (command == "indent" || command == "outdent") {
        QTextCursor cursor = this->designArea->textCursor();
        QTextBlockFormat format = cursor.blockFormat();
        int indent = format.indent();
        format.setIndent(command == "indent" ? indent + 1 : qMax(0, indent - 1));
        cursor.mergeBlockFormat(format);
    } else if (command == "insertOrderedList" || command == "insertUnorderedList") {
        QTextListFormat format;
        format.setStyle(command == "insertOrderedList"
                            ? QTextListFormat::ListDecimal
                            : QTextListFormat::ListDisc);
        this->designArea->textCursor().createList(format);
    } else if (command == "subscript" || command == "superscript") {
        QTextCharFormat format;
        format.setVerticalAlignment(command == "subscript"
                                        ? QTextCharFormat::AlignSubScript
                                        : QTextCharFormat::AlignSuperScript);
        mergeDesignCharFormat(format);
    } else if (command == "delete") {
        this->designArea->textCursor().removeSelectedText();
    }
}

void HTMLDocument::execute(QString operation, QString parameter)
{
    execCommand(operation, parameter);
}

void HTMLDocument::execute(QString command)
{
    execCommand(command);
}

void HTMLDocument::bold()
{
    QTextCharFormat format;
    format.setFontWeight(this->designArea->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    mergeDesignCharFormat(format);
}

void HTMLDocument::italic()
{
    QTextCharFormat format;
    format.setFontItalic(!this->designArea->fontItalic());
    mergeDesignCharFormat(format);
}

void HTMLDocument::underline()
{
    QTextCharFormat format;
    format.setFontUnderline(!this->designArea->fontUnderline());
    mergeDesignCharFormat(format);
}

void HTMLDocument::alignleft()
{
    execCommand("justifyLeft");
}

void HTMLDocument::alignright()
{
    execCommand("justifyRight");
}

void HTMLDocument::aligncenter()
{
    execCommand("justifyCenter");
}

void HTMLDocument::alignjustify()
{
    execCommand("justifyFull");
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
    applyBlockTag("h1");
}

void HTMLDocument::styleHeading2()
{
    applyBlockTag("h2");
}

void HTMLDocument::styleHeading3()
{
    applyBlockTag("h3");
}

void HTMLDocument::styleHeading4()
{
    applyBlockTag("h4");
}

void HTMLDocument::styleHeading5()
{
    applyBlockTag("h5");
}

void HTMLDocument::styleHeading6()
{
    applyBlockTag("h6");
}

void HTMLDocument::insertPasswordField()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/inserttextfield.ui"));
    QDialog *insertTextFieldDialog = qobject_cast<QDialog *>(dialogShower.load(&dialogFile, this));
    insertTextFieldDialog->setWindowTitle(QString("Insert Password Field"));
    if (insertTextFieldDialog->exec()) {
        QLineEdit *nameField = insertTextFieldDialog->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *defaultTextField = insertTextFieldDialog->findChild<QLineEdit *>(QString("defaultTextField"));
        QLineEdit *classField = insertTextFieldDialog->findChild<QLineEdit *>(QString("classField"));
        QLineEdit *idField = insertTextFieldDialog->findChild<QLineEdit *>(QString("idField"));
        QString htmlOfTextField = this->passwordField(
            nameField->text(), defaultTextField->text(), classField->text(), idField->text());
        this->insertHTMLAtCursor(htmlOfTextField);
    }
}

void HTMLDocument::insertTextField()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/inserttextfield.ui"));
    QDialog *insertTextFieldDialog = qobject_cast<QDialog *>(dialogShower.load(&dialogFile, this));
    if (insertTextFieldDialog->exec()) {
        QLineEdit *nameField = insertTextFieldDialog->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *defaultTextField = insertTextFieldDialog->findChild<QLineEdit *>(QString("defaultTextField"));
        QLineEdit *classField = insertTextFieldDialog->findChild<QLineEdit *>(QString("classField"));
        QLineEdit *idField = insertTextFieldDialog->findChild<QLineEdit *>(QString("idField"));
        QString htmlOfTextField = this->textField(
            nameField->text(), defaultTextField->text(), classField->text(), idField->text());
        this->insertHTMLAtCursor(htmlOfTextField);
    }
}

void HTMLDocument::insertTextArea()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/inserttextarea.ui"));
    QDialog *insertTextFieldDialog = qobject_cast<QDialog *>(dialogShower.load(&dialogFile, this));
    if (insertTextFieldDialog->exec()) {
        QLineEdit *nameField = insertTextFieldDialog->findChild<QLineEdit *>(QString("nameField"));
        QTextEdit *defaultTextField = insertTextFieldDialog->findChild<QTextEdit *>(QString("defaultTextField"));
        QLineEdit *classField = insertTextFieldDialog->findChild<QLineEdit *>(QString("classField"));
        QLineEdit *idField = insertTextFieldDialog->findChild<QLineEdit *>(QString("idField"));
        QLineEdit *rowField = insertTextFieldDialog->findChild<QLineEdit *>(QString("rowField"));
        QLineEdit *columnField = insertTextFieldDialog->findChild<QLineEdit *>(QString("columnField"));

        QString htmlOfTextArea = this->textArea(
            nameField->text(),
            defaultTextField->toPlainText(),
            rowField->text(),
            columnField->text(),
            classField->text(),
            idField->text());
        this->insertHTMLAtCursor(htmlOfTextArea);
    }
}

void HTMLDocument::insertRadioButton()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/insertradiobutton.ui"));
    QDialog *insertRadioButton = qobject_cast<QDialog *>(dialogShower.load(&dialogFile, this));
    insertRadioButton->setWindowTitle("Insert Radio Button");
    if (insertRadioButton->exec()) {
        QLineEdit *nameField = insertRadioButton->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *valueField = insertRadioButton->findChild<QLineEdit *>(QString("valueField"));
        QLineEdit *idField = insertRadioButton->findChild<QLineEdit *>(QString("idField"));
        QLineEdit *labelTextField = insertRadioButton->findChild<QLineEdit *>(QString("labelTextField"));
        QCheckBox *selectedField = insertRadioButton->findChild<QCheckBox *>(QString("selectedField"));

        QString htmlOfRadioButton = QString("%1%2")
                                        .arg(this->radioButton(nameField->text(),
                                                               valueField->text(),
                                                               "",
                                                               idField->text(),
                                                               selectedField->checkState() == Qt::Checked))
                                        .arg(labelTextField->text());
        this->insertHTMLAtCursor(htmlOfRadioButton);
    }
}

void HTMLDocument::insertSubmitButton()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/insertbutton.ui"));
    QDialog *insertRadioButton = qobject_cast<QDialog *>(dialogShower.load(&dialogFile, this));
    insertRadioButton->setWindowTitle("Insert Submit Button");
    if (insertRadioButton->exec()) {
        QLineEdit *nameField = insertRadioButton->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *valueField = insertRadioButton->findChild<QLineEdit *>(QString("valueField"));
        QLineEdit *idField = insertRadioButton->findChild<QLineEdit *>(QString("idField"));
        QString htmlOfSubmitButton = this->submitButton(nameField->text(), valueField->text(), "", idField->text());
        this->insertHTMLAtCursor(htmlOfSubmitButton);
    }
}

void HTMLDocument::insertButton()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/insertbutton.ui"));
    QDialog *insertRadioButton = qobject_cast<QDialog *>(dialogShower.load(&dialogFile, this));
    insertRadioButton->setWindowTitle("Insert Button");
    if (insertRadioButton->exec()) {
        QLineEdit *nameField = insertRadioButton->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *valueField = insertRadioButton->findChild<QLineEdit *>(QString("valueField"));
        QLineEdit *idField = insertRadioButton->findChild<QLineEdit *>(QString("idField"));
        QString htmlOfButton = this->button(nameField->text(), valueField->text(), "", idField->text());
        this->insertHTMLAtCursor(htmlOfButton);
    }
}

void HTMLDocument::insertSelectMenu()
{
    InsertSelectMenuDialog *dialog = new InsertSelectMenuDialog();
    dialog->setWindowTitle(QString("Insert Select Menu"));
    if (dialog->exec()) {
        QMap<QString, QString> selectOptions = dialog->getData();
        QString htmlOfSelect = this->selectMenu(
            selectOptions, dialog->nameField->text(), dialog->classField->text(), dialog->idField->text());
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
    if (buttonGroup.exec()) {
        QMap<QString, QString> list = buttonGroup.getList();
        QStringList keys = list.keys();
        QString html;

        for (int curr = 0; curr < keys.size(); curr++) {
            html.append(QString("<input type=\"checkbox\" name=\"%1\" class=\"%2\" value=\"%3\" />%4 <br />")
                            .arg(buttonGroup.name->text(), buttonGroup.cssclass->text(), list[keys.at(curr)], keys.at(curr)));
        }

        this->insertHTMLAtCursor(html);
    }
}

void HTMLDocument::attachjQuery(bool silent)
{
    Q_UNUSED(silent);

    if (!this->savedAtleastOnce || this->fileName.isEmpty()) {
        qDebug("False!");
        return;
    }

    QFileInfo fileInfo(this->fileName);
    QString path = fileInfo.absolutePath();
    if (!QFile::copy(":/jquery.js", path + "/jquery.js")) {
        QMessageBox::critical(this, "Failed", "Attaching jQuery failed as a jquery file could not be written to the directory");
    }
}

void HTMLDocument::insertRadioGroup()
{
    InsertButtonGroup buttonGroup;
    buttonGroup.setWindowTitle(QString("Insert Checkbox Group"));
    if (buttonGroup.exec()) {
        QMap<QString, QString> list = buttonGroup.getList();
        QStringList keys = list.keys();
        QString html;

        for (int curr = 0; curr < keys.size(); curr++) {
            html.append(QString("%1%2<br />")
                            .arg(this->radioButton(buttonGroup.name->text(), list[keys.at(curr)], "", "", false))
                            .arg(keys.at(curr)));
        }

        this->insertHTMLAtCursor(html);
    }
}

void HTMLDocument::insertCheckBox()
{
    QUiLoader dialogShower(this);
    QFile dialogFile(QString(":/insertradiobutton.ui"));
    QDialog *insertRadioButton = qobject_cast<QDialog *>(dialogShower.load(&dialogFile, this));
    insertRadioButton->setWindowTitle("Insert CheckBox");
    if (insertRadioButton->exec()) {
        QLineEdit *nameField = insertRadioButton->findChild<QLineEdit *>(QString("nameField"));
        QLineEdit *valueField = insertRadioButton->findChild<QLineEdit *>(QString("valueField"));
        QLineEdit *idField = insertRadioButton->findChild<QLineEdit *>(QString("idField"));
        QLineEdit *labelTextField = insertRadioButton->findChild<QLineEdit *>(QString("labelTextField"));
        QCheckBox *selectedField = insertRadioButton->findChild<QCheckBox *>(QString("selectedField"));

        QString htmlOfRadioButton = QString("%1<label for=\"%2\">%3</label>")
                                        .arg(this->checkBox(nameField->text(),
                                                            valueField->text(),
                                                            "",
                                                            idField->text(),
                                                            selectedField->checkState() == Qt::Checked))
                                        .arg(idField->text(), labelTextField->text());
        this->insertHTMLAtCursor(htmlOfRadioButton);
    }
}

void HTMLDocument::insertImage()
{
    QString defaultDirectory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = QFileDialog::getOpenFileName(this, "Select Image File..", defaultDirectory);

    if (filePath.isEmpty() || !QFile::exists(filePath)) {
        return;
    }

    QUrl fp = QUrl::fromLocalFile(filePath);
    this->execCommand("insertImage", fp.toString());
}

void HTMLDocument::insertLink()
{
    if (this->designArea->textCursor().hasSelection()) {
        QString url = QInputDialog::getText(this, "Link URL", "Enter the link address:");
        if (!url.isEmpty()) {
            this->execCommand("createLink", url);
        }
    } else {
        QUiLoader loader;
        QFile uifile(":/insertlink.ui");
        QDialog *dialog = qobject_cast<QDialog *>(loader.load(&uifile));
        dialog->setWindowTitle("Insert Link");
        if (dialog->exec()) {
            QLineEdit *textField = dialog->findChild<QLineEdit *>(QString("linkTextField"));
            QLineEdit *urlField = dialog->findChild<QLineEdit *>(QString("linkAddressField"));
            QString html = this->link(textField->text(), urlField->text());
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
    QString text = QInputDialog::getText(this, "Anchor", "Name of anchor:");
    QString html = this->getSelectedText();

    this->execCommand("delete");
    this->insertHTMLAtCursor(QString("<a name=\"%1\">%2</a>").arg(text, html));
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
        this->alignright();
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
        this->redo();
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
    QString text = QString("<table><tr><td>Name:</td><td>%1</td></tr><tr><td>Website: </td><td>%2</td></tr><tr><td>Email Address:</td><td>%3</td></tr><tr><td colspan=2>Comment:</br>%4</td></tr><tr><td colspan=2>%5</td></table>")
                       .arg(this->textField("name", "", "", ""))
                       .arg(this->textField("website", "", "", ""))
                       .arg(this->textField("email", "", "", ""))
                       .arg(this->textArea("comment", "", "10", "50", "", ""))
                       .arg(this->submitButton("Comment", "Comment", "", ""));
    this->insertHTMLAtCursor(text);
}

void HTMLDocument::insertLoginForm()
{
    QString text = QString("<table><tr><td>Username:</td><td>%1</td></tr><tr><td>Password:</td><td>%2</td></tr><tr><td colspan=2>%3</td></tr></table>")
                       .arg(this->textField("username", "", "", ""))
                       .arg(this->passwordField("password", "", "", ""))
                       .arg(this->submitButton("Submit", "Submit", "", ""));
    this->insertHTMLAtCursor(text);
}

void HTMLDocument::insertSearchForm()
{
    QString text = QString("<table><tr><td>Search:</td><td>%1</td><td colspan=2>%2</td></tr></table>")
                       .arg(this->textField("keyword", "", "", ""))
                       .arg(this->submitButton("Submit", "Search", "", ""));
    this->insertHTMLAtCursor(text);
}

void HTMLDocument::insertMailingListOptin()
{
    QString text = QString("<table><tr><td>Name:</td><td>%1</td></tr><tr><td>Email Address:</td><td>%2</td></tr><tr><td colspan=2>%3</td></tr></table>")
                       .arg(this->textField("name", "", "", ""))
                       .arg(this->textField("email", "", "", ""))
                       .arg(this->submitButton("Submit", "Submit", "", ""));
    this->insertHTMLAtCursor(text);
}

void HTMLDocument::insertTable()
{
    QUiLoader loader;
    QFile dialogfile(":/inserttable.ui");
    dialogfile.open(QIODevice::ReadOnly);
    QDialog *dialog = qobject_cast<QDialog *>(loader.load(&dialogfile, this));
    if (dialog->exec()) {
        QLineEdit *rowsField = dialog->findChild<QLineEdit *>("rows");
        QLineEdit *colsField = dialog->findChild<QLineEdit *>("cols");
        int rowc = rowsField->text().toInt();
        int colc = colsField->text().toInt();
        QString html = "<table border=\"1\" width=\"500\"  cellspacing=\"1\" cellpadding=\"1\">";
        for (int currrow = 0; currrow < rowc; currrow++) {
            html.append("<tr>");
            for (int currcol = 0; currcol < colc; currcol++) {
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
    if (dialog->exec()) {
        int numOfItems = dialog->smallList->count();
        QStringList list;
        for (int curr = 0; curr < numOfItems; curr++) {
            list.append(dialog->smallList->item(curr)->text());
        }

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

        for (int curr = 0; curr < numOfItems; curr++) {
            QString currItem = list.at(curr).trimmed();
            if (oneliners.contains(currItem)) {
                html.append("<tr>");
                html.append(QString("<td>%1:</td><td>%2</td>")
                                .arg(currItem)
                                .arg(this->textField(QString("fname"), QString(""), QString(""), QString(""))));
                html.append("</tr>");
            } else if (currItem == "Gender") {
                html.append("<tr>");
                QString male = this->radioButton(QString("gender"), QString("male"), QString(""), QString(""), true);
                QString female = this->radioButton(QString("gender"), QString("female"), QString(""), QString(""), false);
                html.append(QString("<td>%1:</td><td>%2 Male  %3 Female</td>").arg("Gender").arg(male).arg(female));
                html.append("</tr>");
            } else if (currItem == "Password") {
                QString pass = this->passwordField(QString("password"), QString(""), QString(""), QString(""));
                QString rpass = this->passwordField(QString("rpassword"), QString(""), QString(""), QString(""));

                html.append("<tr>");
                html.append(QString("<td>Password: </td><td>%1</td>").arg(pass));
                html.append("</tr>");
                html.append("<tr>");
                html.append(QString("<td>Retype Password:</td><td>%1</td>").arg(rpass));
                html.append("</tr>");
            } else if (currItem.trimmed() == "Country Field") {
                QStringList countryList;
                QFile thefile(":/countrylist.txt");
                thefile.open(QIODevice::ReadOnly);
                QTextStream reader(&thefile);
                while (!reader.atEnd()) {
                    countryList.append(reader.readLine());
                }
                QString countryfield = "<select name=\"country\">";
                for (int country = 0; country < countryList.size(); country++) {
                    countryfield.append(QString("<option>%1</option>").arg(countryList.at(country)));
                }
                countryfield.append("</select>");
                html.append(QString("<tr><td>Country: </td><td>%1</td>").arg(countryfield));
            } else if (currItem == "Date Of Birth") {
                QStringList numbers;
                for (int day = 1; day <= 31; day++) {
                    numbers.append(QString("%1").arg(day));
                }
                QString dateField = this->selectMenu(numbers, "dobdate", "", "");
                QStringList months;
                months << "January" << "February" << "March" << "April" << "May" << "June" << "July" << "August"
                       << "September" << "October" << "November" << "December";
                QString monthField = this->selectMenu(months, "dobmonth", "", "");
                int currentYear = QDate::currentDate().year();
                QStringList yearlist;
                for (int year = 1930; year < currentYear; year++) {
                    yearlist.append(QString("%1").arg(year));
                }
                QString yearField = this->selectMenu(yearlist, "dobyear", "", "");
                html.append(QString("<tr><td>Date Of Birth</td><td>%1 %2 %3</td></tr>")
                                .arg(dateField)
                                .arg(monthField)
                                .arg(yearField));
            } else if (currItem == "Submit") {
                QString submit = this->submitButton(QString("submit"), "Submit", "", "");
                QString reset = "<input type=\"reset\" value=\"Clear Form\">";
                html.append(QString("<tr><td>%1 %2</td>").arg(submit).arg(reset));
            }
        }

        html.append("</table></form>");
        this->insertHTMLAtCursor(html);
    }
}

QString HTMLDocument::getSelectedText()
{
    if (this->currentMode == Design) {
        return this->designArea->textCursor().selectedText();
    }

    return this->editor->textCursor().selectedText();
}

void HTMLDocument::insertHTMLAtCursor(QString htmlToInsert)
{
    Modes currentMode = static_cast<Modes>(this->currentMode);
    switch (currentMode) {
    case Design:
        this->designArea->textCursor().insertHtml(htmlToInsert);
        break;
    case Code:
        this->editor->insertPlainText(htmlToInsert);
        break;
    case Preview:
        break;
    }
}

QString HTMLDocument::getType()
{
    return QString("HTML");
}

QString HTMLDocument::checkBox(QString name, QString value, QString cssclass, QString id, bool checked)
{
    return QString("<input type=\"checkbox\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" %5 />")
        .arg(name, value, cssclass, id, checked ? "checked=\"checked\"" : "");
}

QString HTMLDocument::radioButton(QString name, QString value, QString cssclass, QString id, bool checked)
{
    return QString("<input type=\"radio\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" %5 />")
        .arg(name, value, cssclass, id, checked ? "checked=\"checked\"" : "");
}

QString HTMLDocument::textField(QString name, QString value, QString cssclass, QString id)
{
    return QString("<input type=\"text\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" />")
        .arg(name, value, cssclass, id);
}

QString HTMLDocument::passwordField(QString name, QString value, QString cssclass, QString id)
{
    return QString("<input type=\"password\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" />")
        .arg(name, value, cssclass, id);
}

QString HTMLDocument::submitButton(QString name, QString value, QString cssclass, QString id)
{
    return QString("<input type=\"submit\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" />")
        .arg(name, value, cssclass, id);
}

QString HTMLDocument::button(QString name, QString value, QString cssclass, QString id)
{
    return QString("<input type=\"button\" name=\"%1\" value=\"%2\" class=\"%3\" id=\"%4\" />")
        .arg(name, value, cssclass, id);
}

QString HTMLDocument::selectMenu(QStringList items, QString name, QString cssclass, QString id)
{
    QString html = QString("<select name=\"%1\" class=\"%2\" id=\"%3\">").arg(name, cssclass, id);
    for (int curr = 0; curr < items.size(); curr++) {
        html.append(QString("<option>%1</option>").arg(items.at(curr)));
    }
    html.append("</select>");
    return html;
}

QString HTMLDocument::selectMenu(QMap<QString, QString> list, QString name, QString cssclass, QString id)
{
    QString html = QString("<select name=\"%1\" class=\"%2\" id=\"%3\">").arg(name, cssclass, id);
    QStringList keys = list.keys();
    for (int curr = 0; curr < keys.size(); curr++) {
        html.append(QString("<option value=\"%1\">%2</option>").arg(list[keys.at(curr)], keys.at(curr)));
    }
    html.append("</select>");
    return html;
}

QString HTMLDocument::textArea(QString name, QString value, QString rows, QString cols, QString cssclass, QString id)
{
    return QString("<textarea name=\"%1\" rows=\"%2\" cols=\"%3\" class=\"%4\" id =\"%5\">%6</textarea>")
        .arg(name, rows, cols, cssclass, id, value);
}

QString HTMLDocument::link(QString linktext, QString href)
{
    return QString("<a href=\"%1\">%2</a>").arg(href, linktext);
}
