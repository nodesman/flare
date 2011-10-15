#include "flare.h"
#include "ui_flare.h"
//#include "document.h"
#include "foundry.h"
#include "newfiledialog.h"
#include "finddialog.h"
#include "htmldocument.h"
#include "jsdocument.h"
#include "phpdocument.h"
#include "settings.h"
#include "projectdialog.h"
#include "manageprojectsdialog.h"
#include "switchproject.h"
#include "flexdocument.h"
#include <QtGui>
#include <QClipboard>
#include <QUiLoader>
#include "cssdocument.h"


//static data members
QString flare::currentWorkingDirectory;
flare::flare(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::flareClass)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    this->clipboard = QApplication::clipboard();
    this->setWindowState(Qt::WindowMaximized);
    initialize();
    qDebug(QDesktopServices::storageLocation(QDesktopServices::DataLocation).toAscii());

}

void flare::toggleClipboardOperations(bool status)
{
    this->ui->actionCut->setEnabled(status);
    this->ui->actionCopy->setEnabled(status);

}

void flare::edit_togglePaste()
{
    bool enabled = this->ui->actionPaste->isEnabled();
    this->ui->actionPaste->setDisabled(enabled);
}


void flare::find()
{
// to be implemented

}


void flare::documentChanged()
{
    //update the undo menu items
    Document *doc = (Document *) this->editor->activeSubWindow();

    this->updateWindowMenu();
}

void flare::findNext()
{
    if (this->finddiag == 0)
        return;
    this->finddiag->findNext();
}

void flare::findTriggered(QString searchExpression,QTextDocument::FindFlags flags)
{
    //Document *doc = (Document *) this->editor->activeSubWindow();
    //doc->findText(searchExpression,flags);
}


void flare::attachKeyboardShortcuts()
{


}

void flare::initialize()
{
    flare::numberOfDocuments = 0;

    this->editor = new Foundry(this);
    //this->undoStack = new QUndoStack(this);
    this->windowMapper = new QSignalMapper(this);

    QDockWidget *dock = new QDockWidget(tr("Project"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QTreeView *treeView = new QTreeView;
    QDirModel theDirectories;
    treeView->setModel(&theDirectories);
    treeView->setRootIndex(theDirectories.index(QDir::currentPath()));

    dock->setWidget(treeView);
    treeView->show();
    addDockWidget(Qt::RightDockWidgetArea, dock);
    //end dockablewidgetinsertSearchForm
    QSize iconsize(17,17);
    this->standardToolbar = new QToolBar("Standard",this);
    this->standardToolbar->setAllowedAreas(Qt::TopToolBarArea);
    this->standardToolbar->setIconSize(iconsize);
    this->toolbarButtons["New"] = this->standardToolbar->addAction(QIcon(":/new.png"),"New File");
        connect(this->toolbarButtons["New"],SIGNAL(triggered()),this,SLOT(on_actionNew_triggered()));
    this->standardToolbar->addSeparator();
    this->toolbarButtons["Open"] = this->standardToolbar->addAction(QIcon(":/open.png"),"Open File");
        connect(this->toolbarButtons["Open"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Save"] = this->standardToolbar->addAction(QIcon(":/save.png"),"Save");
    connect(this->toolbarButtons["Save"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Save As"] = this->standardToolbar->addAction(QIcon(":/saveas.png"),"Save As");
    this->toolbarButtons["Save All"] = this->standardToolbar->addAction(QIcon(":/saveall.png"),"Save All");
    this->toolbarButtons["Close"] = this->standardToolbar->addAction(QIcon(":/close.png"),"Close");
        connect(this->toolbarButtons["Close"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    //this->toolbarButtons["Close All"] = this->standardToolbar->addAction(QIcon(":/closeall.png"),"Close All"); too obtrusive
    this->standardToolbar->addSeparator();
    this->toolbarButtons["Undo"] = this->standardToolbar->addAction(QIcon(":/undo.png"),"Undo");
        connect(this->toolbarButtons["Undo"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Redo"] = this->standardToolbar->addAction(QIcon(":/redo.png"),"Redo");
        connect(this->toolbarButtons["Redo"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->standardToolbar->addSeparator();
    this->toolbarButtons["Cut"] = this->standardToolbar->addAction(QIcon(":/cut.png"),"Cut");
        connect(this->toolbarButtons["Cut"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Copy"] = this->standardToolbar->addAction(QIcon(":/copy.png"),"Copy");
        connect(this->toolbarButtons["Copy"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Paste"] = this->standardToolbar->addAction(QIcon(":/paste.png"),"Paste");
        connect(this->toolbarButtons["Paste"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Find"] = this->standardToolbar->addAction(QIcon(":/find.png"),"Find");
    this->toolbarButtons["Cascade Windows"] = this->standardToolbar->addAction(QIcon(":/cascade.png"),"Cascade Windows");
        connect(this->toolbarButtons["Cascade Windows"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    //this->toolbarButtons["Open"] = this->standardToolbar->addAction(QIcon("icons/
    this->addToolBar(Qt::TopToolBarArea,this->standardToolbar);
    this->addToolBarBreak(Qt::TopToolBarArea);


    //formattingn toolbar
    this->formattingToolbar = new QToolBar("Formatting ToolBar",this);
    this->formattingToolbar->setAllowedAreas(Qt::TopToolBarArea);
    this->formattingToolbar->setIconSize(iconsize);
    this->toolbarButtons["Font"] = this->formattingToolbar->addWidget(new QFontComboBox(this->formattingToolbar));
    this->toolbarButtons["Bold"] = this->formattingToolbar->addAction(QIcon(":/bold.png"),"Bold");
    connect(this->toolbarButtons["Bold"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Italic"] = this->formattingToolbar->addAction(QIcon(":/italic.png"),"Italic");
    connect(this->toolbarButtons["Italic"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Underline"] = this->formattingToolbar->addAction(QIcon(":/underline.png"),"Underline");
    connect(this->toolbarButtons["Underline"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->formattingToolbar->addSeparator();
    this->toolbarButtons["Align Left"] = this->formattingToolbar->addAction(QIcon(":/alignleft.png"),"Align Left");
    connect(this->toolbarButtons["Align Left"],SIGNAL(triggered()),this,SLOT(htmlCommand()));

    this->toolbarButtons["Align Right"] = this->formattingToolbar->addAction(QIcon(":/alignright.png"),"Align Right");
    connect(this->toolbarButtons["Align Right"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Align Center"] = this->formattingToolbar->addAction(QIcon(":/aligncenter.png"),"Align Center");
    connect(this->toolbarButtons["Align Center"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Align Justify"] = this->formattingToolbar->addAction(QIcon(":/alignjustify.png"),"Align Justify");
    connect(this->toolbarButtons["Align Justify"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->formattingToolbar->addSeparator();
    this->toolbarButtons["List Bulleted"] = this->formattingToolbar->addAction(QIcon(":/listbullets.png"),"BulletinsertSearchFormed List");
    connect(this->toolbarButtons["List Bulleted"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["List Numbered"] = this->formattingToolbar->addAction(QIcon(":/listbullets.png"),"Numbered List");
    connect(this->toolbarButtons["List Numbered"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->formattingToolbar->addSeparator();
    this->toolbarButtons["Increase Indent"] = this->formattingToolbar->addAction(QIcon(":/increaseindent.png"),"Increase Indent");
    connect(this->toolbarButtons["Increase Indent"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Decrease Indent"] = this->formattingToolbar->addAction(QIcon(":/decreaseindent.png"),"Decrease Indent");
    connect(this->toolbarButtons["Decrease Indent"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->formattingToolbar->addSeparator();
    this->toolbarButtons["Heading 1"] = this->formattingToolbar->addAction(QIcon(":/heading1.png"),"Heading 1");
    connect(this->toolbarButtons["Heading 1"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Heading 2"] = this->formattingToolbar->addAction(QIcon(":/heading2.png"),"Heading 2");
    connect(this->toolbarButtons["Heading 2"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Heading 3"] = this->formattingToolbar->addAction(QIcon(":/heading3.png"),"Heading 3");
    connect(this->toolbarButtons["Heading 3"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Heading 4"] = this->formattingToolbar->addAction(QIcon(":/heading4.png"),"Heading 4");
    connect(this->toolbarButtons["Heading 4"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Heading 5"] = this->formattingToolbar->addAction(QIcon(":/heading5.png"),"Heading 5");
    connect(this->toolbarButtons["Heading 5"],SIGNAL(triggered()),this,SLOT(htmlCommand()));
    this->toolbarButtons["Heading 6"] = this->formattingToolbar->addAction(QIcon(":/heading6.png"),"Heading 6");

    this->formattingToolbar->addSeparator();
    this->addToolBar(this->formattingToolbar);
    this->addToolBarBreak(Qt::TopToolBarArea);
    this->insertionToolbar = new QToolBar("Insertion Toolbar");
    this->insertionToolbar->setIconSize(iconsize);
    this->toolbarButtons["Image"] = this->insertionToolbar->addAction(QIcon(":/image.png"),"Image");
    this->toolbarButtons["Special Character"] = this->insertionToolbar->addAction(QIcon(":/charactermap.png"),"Special Character");
    this->insertionToolbar->addSeparator();
    this->toolbarButtons["Table"] = this->insertionToolbar->addAction(QIcon(":/table.png"),"Table");
    this->toolbarButtons["Insert Row"] = this->insertionToolbar->addAction(QIcon(":/inserttablerow.png"),"Insert Row");
    this->toolbarButtons["Insert Column"] = this->insertionToolbar->addAction(QIcon(":/inserttablecolumn.png"),"Insert Column");
    this->toolbarButtons["Delete Row"] = this->insertionToolbar->addAction(QIcon(":/deleterow.png"),"Delete Row");
    this->toolbarButtons["Delete Column"] = this->insertionToolbar->addAction(QIcon(":/deletecolumn.png"),"Delete Column");
    this->addToolBar(this->insertionToolbar);



    this->setCentralWidget(this->editor);
    QCoreApplication::setApplicationName(FL_APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(FL_VERSION);
    QCoreApplication::setOrganizationName(FL_COMPANY_NAME);
    QCoreApplication::setOrganizationDomain(FL_COMPANY_DOMAIN);
    //disable some of the menu items
    this->ui->actionSave->setDisabled(true);
    this->ui->actionSave_As->setDisabled(true);
    this->ui->actionSave_All->setDisabled(true);
    this->ui->actionClose->setDisabled(true);
    this->ui->actionClose_All->setDisabled(true);
    this->ui->actionCut->setDisabled(true);
    this->ui->actionCopy->setDisabled(true);
    this->ui->actionPaste->setDisabled(true);
    this->ui->actionUndo->setDisabled(true);
    this->ui->actionRedo->setDisabled(true);
    this->ui->actionNext_Window->setDisabled(true);
    this->ui->actionPrevious_Window->setDisabled(true);

    //attach all the signals to the slots
    connect(ui->actionPHP,SIGNAL(triggered()),this,SLOT(newPHPFile()));
    connect(ui->actionCSS,SIGNAL(triggered()),this,SLOT(newCSSFile()));
    connect(ui->actionHTML,SIGNAL(triggered()),this,SLOT(newHTMLFile()));
    connect(ui->actionJS,SIGNAL(triggered()),this,SLOT(newJSFile()));
    //connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openfile()));
    connect(ui->actionSave, SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionSave_As,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(ui->actionSave_All,SIGNAL(triggered()),this,SLOT(saveAll()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exitApplication()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(closeDocument()));
    connect(ui->actionClose_All,SIGNAL(triggered()),this,SLOT(closeAllDocuments()));
    connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(find()));
    connect(this,SIGNAL(newdocument()),this,SLOT(documentOpened()));
    //the clipboard object - needed to sense when something is available for pasting and enable the paste option on the edit menu
    connect(this->clipboard,SIGNAL(dataChanged()),this,SLOT(edit_togglePaste()));
    connect(this->editor,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(documentChanged()));
    this->attachKeyboardShortcuts();
    connect(this->windowMapper,SIGNAL(mapped(QWidget*)),this,SLOT(activateSubWindow(QWidget*)));
    connect(this->clipboard,SIGNAL(dataChanged()),this,SLOT(pasteStatus()));
    connect(this->ui->actionText_Field,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionText_Area,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionPassword,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionRadio_Button,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionCheck_Box,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionButton,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionSubmit,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionSelectMenu,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionCheck_Box_Group,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionRadion_Button_Group,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionImage,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionNamed_Anchor,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionImage,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionFlash,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionLink,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionNamed_Anchor,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionAttach_jQuery,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionRegistration_Form,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionComment_Form,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionLogin_Form,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionSearch_Form,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionMailing_List_Optin_Form,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionTable,SIGNAL(triggered()),this,SLOT(htmlCommand()));

    //format menu
    connect(this->ui->actionBold,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionItalic,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionUnderline,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionBulleted_List,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionNumbered_List,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionIndent,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionOutdent,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionSubscript,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionSuperscript,SIGNAL(triggered()),this,SLOT(htmlCommand()));
    connect(this->ui->actionAbout,SIGNAL(triggered()),SLOT(about()));


    this->activeProject  = 0;
}

void flare::dummy()
{
    //QMessageBox::information(this,"yeah!","yeah1");
    this->editor->activatePreviousSubWindow();

}


//the slots go here:
/*newFileDialog *flare::getNewFileDialog()
{
    return this->newDocDiag;
}*/

flare::~flare()
{
    delete ui;
}

int flare::numberOfDocuments;

void flare::exitApplication()
{
    QApplication::quit();
}
void flare::newPHPFile()
{
   /* Document *doc = new PHPDocument(this->editor);
    this->editor->addSubWindow(doc,Qt::SubWindow);
    this->initializeDocument(doc);
    doc->setWindowState(Qt::WindowMaximized);
    doc->setAttribute(Qt::WA_DeleteOnClose);
    doc->show();
    emit newdocument();*/
}

void flare::newCSSFile()
{
    /*Document *doc = new CSSDocument(this->editor);
    this->editor->addSubWindow(doc,Qt::SubWindow);
    this->initializeDocument(doc);
    doc->setWindowState(Qt::WindowMaximized);
    doc->setAttribute(Qt::WA_DeleteOnClose);
    doc->show();
    emit newdocument();*/
}

void flare::newJSFile()
{
    /*Document *doc = new JSDocument(this->editor);
    this->editor->addSubWindow(doc);
    this->initializeDocument(doc);
    doc->setWindowState(Qt::WindowMaximized);
    doc->setAttribute(Qt::WA_DeleteOnClose);
    doc->show();
    emit newdocument();*/

}

void flare::newHTMLFile()
{
    Document *doc = new HTMLDocument(this->editor);
    this->editor->addSubWindow(doc);
    this->initializeDocument(doc);
    doc->setWindowState(Qt::WindowMaximized);
    doc->setAttribute(Qt::WA_DeleteOnClose);
    doc->show();
    emit newdocument();
}

void flare::newFlexFile()
{
   /* Document *document = new FlexDocument(this->editor);
    this->editor->addSubWindow(document);
    this->initializeDocument(document);
    document->setWindowState(Qt::WindowMaximized);
    document->setAttribute(Qt::WA_DeleteOnClose);
    document->show();
    emit newdocument();*/
}

void flare::initializeDocument(Document *document)
{
    //if paste is available, the paste is enabled
    QString clipBoardData  = this->clipboard->text(QClipboard::Clipboard);
    if (!clipBoardData.isEmpty())
    {
        this->ui->actionPaste->setEnabled(true);
    }

    //connect signals
    connect(document,SIGNAL(copyAvailable(bool)),this->ui->actionCut,SLOT(setEnabled(bool)));
    connect(document,SIGNAL(copyAvailable(bool)),this->ui->actionCopy,SLOT(setEnabled(bool)));
    connect(document,SIGNAL(documentChanged()),this,SLOT(documentChanged()));
    connect(document,SIGNAL(undoAvailable(bool)),this->ui->actionUndo,SLOT(setEnabled(bool)));
    connect(document,SIGNAL(redoAvailable(bool)),this->ui->actionRedo,SLOT(setEnabled(bool)));
    connect(document,SIGNAL(destroyed()),this,SLOT(documentClosed()));

}

bool flare::isFileOpen(QString filePath)
{
    QList <QMdiSubWindow *> documentList = this->editor->subWindowList();
/*
    int numberOfItems = documentList.size();
    for (int currDocIndex =0; currDocIndex <=numberOfItems; currDocIndex++)
    {
        doc = (Document *) documentList.at(currDocIndex);
        if (filePath == doc->getFileName())
        {
            return true;
        }
    }*/
    return false;
}


void flare::openfile()
{
    /*
    QString allowedFileTypes = "Web Documents (*.htm *.html *.php *.js *.css)";
    QString currentDirectory = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QStringList filePath = QFileDialog::getOpenFileNames(this,"Open File",currentDirectory,allowedFileTypes);
    if (filePath.isEmpty())
        return;
    //Document *currDoc;
    int numberOfFilesToBeOpened = filePath.size();
    for (int iter=0;iter<numberOfFilesToBeOpened; iter++)
    {
        //check if the file is already open first.
        if (this->isFileOpen(filePath.at(iter)))
        {
            currDoc = this->getDocumentByFilePath(filePath.at(iter));
            currDoc->activateWindow();
            continue;
        }
        currDoc = new Document(this->editor,filePath.at(iter));
        this->connectDocumentSignals(currDoc);
        this->activeDocument = currDoc; //the last element will be the active document

    }
    QString lastOpenDocument = filePath.at(numberOfFilesToBeOpened-1);
    QString theDirectory = lastOpenDocument.mid(0,lastOpenDocument.lastIndexOf("/"));
    this->currentWorkingDirectory = theDirectory;
    this->enableFileActions();*/


}

void flare::save()
{
    Document *document = (Document *)this->editor->activeSubWindow();
    if (document == 0)
        return;

    if (document->getSaved())//check if the document has been previously saved
    {
        document->saveDocument(); //then save it.
    }
    else
        this->saveAs();
}

void flare::saveAs()
{
    QString defaultFilePath;
    if (flare::currentWorkingDirectory=="")
         defaultFilePath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    else
        defaultFilePath = currentWorkingDirectory;
    QString fileTypes;
    Document *activeDocument= (Document *) this->editor->activeSubWindow();
    if (activeDocument == 0) //if this is true, there are no active document
        return;
    QString filePath = QFileDialog::getSaveFileName(this,"Save As" + fileTypes,defaultFilePath,fileTypes);
    QString theDirectory = filePath.mid(0,filePath.lastIndexOf("/"));
    flare::currentWorkingDirectory = theDirectory;
    //update teh current working directory
    activeDocument->setFilePath(filePath);
    activeDocument->saveDocument(); //may have to add an if condition here..
    activeDocument->setSaved(true);

}

void flare::saveAll()
{
}

//this function is activated when the application is closed.
void flare::closeEvent(QCloseEvent *closeEvent)
{
  this->editor->closeAllSubWindows();
  if (this->editor->activeSubWindow() !=0)
      closeEvent->ignore();
  else
        closeEvent->accept();
}

void flare::disableFileActions()
{
    this->ui->actionSave->setDisabled(true);
    this->ui->actionSave_As->setDisabled(true);
    this->ui->actionSave_All->setDisabled(true);
    this->ui->actionClose->setDisabled(true);
    this->ui->actionClose_All->setDisabled(true);
}

void flare::enableFileActions()
{
    this->ui->actionSave->setDisabled(false);
    this->ui->actionSave_As->setDisabled(false);
    this->ui->actionSave_All->setDisabled(false);
    this->ui->actionClose->setDisabled(false);
    this->ui->actionClose_All->setDisabled(false);
}
void flare::documentClosed()
{
      //QObject *document = sender();
      int numberOfWindows = this->editor->subWindowList().size();
      if (numberOfWindows == 0) //if there are no more documents opened disable the save, save as, save all menu items.
      {
          disableFileActions();
          this->ui->actionCut->setEnabled(false);
          this->ui->actionCopy->setEnabled(false);
          this->ui->actionPaste->setEnabled(false);
          this->ui->actionUndo->setEnabled(false);
          this->ui->actionRedo->setEnabled(false);
          this->ui->actionSelect_All->setEnabled(false);
          this->ui->actionFind->setEnabled(false);
          this->ui->actionFind_Next->setEnabled(false);
          this->ui->actionNext_Window->setEnabled(false);
          this->ui->actionPrevious_Window->setEnabled(false);
      }
}

void flare::closeDocument()
{
    this->editor->closeActiveSubWindow();
    this->numberOfDocuments--;
}

bool flare::closeAllDocuments()
{
   bool areAllWindowsClosed=true;
   this->editor->closeAllSubWindows();
   this->numberOfDocuments = this->editor->subWindowList().size();
   if (this->numberOfDocuments != 0)
       areAllWindowsClosed = false;
   return areAllWindowsClosed;
}

void flare::decrementNumberOfDocuments()
{
    this->numberOfDocuments--;
}
//this function is executed whenever files are to be opened by the editor

/*Document *flare::getDocumentByFilePath(QString filePath)
{
    QList <QMdiSubWindow *> docList = this->editor->subWindowList();
    Document *doc;
    int numberOfItems = docList.size();
    for (int currDocIndex =0; currDocIndex <=numberOfItems; currDocIndex++)
    {
        doc = (Document *)docList.at(currDocIndex);
        if (filePath == doc->getFileName())
        {
            return doc;
        }
    }
     return 0;
}*/

void flare::updateWindowMenu()
{
    this->ui->menuWindow->clear();
    ui->menuWindow->addAction(ui->actionNext_Window);
    ui->menuWindow->addAction(ui->actionPrevious_Window);
    ui->menuWindow->addSeparator();
    ui->menuWindow->addAction(ui->actionClose);
    ui->menuWindow->addAction(ui->actionClose_All);

    QAction *separator = ui->menuWindow->addSeparator();
    QList <QMdiSubWindow *> listOfWindows= this->editor->subWindowList();
    bool areThereWindows =(listOfWindows.size() != 0);
    separator->setVisible(areThereWindows);
    QAction *currDocument;
    QMdiSubWindow *window;
    if (areThereWindows)
    {
        //add menus to the list.
        for (int currWindow = 0; currWindow < listOfWindows.size(); currWindow++)
        {

            currDocument = new QAction(ui->menuWindow);
            window = listOfWindows.at(currWindow);
            currDocument->setText(window->windowTitle());
            currDocument->setCheckable(true);
            if (window == this->editor->activeSubWindow())
                currDocument->setChecked(true);
            this->ui->menuWindow->addAction(currDocument);
            connect(currDocument,SIGNAL(triggered()),this->windowMapper,SLOT(map()));
            this->windowMapper->setMapping(currDocument, listOfWindows.at(currWindow));

        }
    }

}


void flare::activateSubWindow(QWidget *theWindow)
{

    QMdiSubWindow *targetWindow = (QMdiSubWindow *) theWindow;
    targetWindow->activateWindow();
    targetWindow->setFocus();
}


void flare::on_actionNew_Project_triggered()
{
    ProjectDialog newFile(this);
    if (newFile.exec())
    {


    }

}

void flare::on_actionManage_Project_triggered()
{
    ManageProjectsDialog dialog(this);
    dialog.exec();
}

void flare::switchProject(QString project)
{
    Settings setobj(this);
    QString theString = setobj.getProjectString(project);
    QMessageBox::information(this,"project switched",theString);
    this->activeProject = new Project(theString);
}

void flare::on_actionSwitch_Project_triggered()
{
    SwitchProject *theDiag;
    if (this->activeProject==0)
    {
        theDiag = new SwitchProject(this);
    }
    else
    {
        theDiag = new SwitchProject(this->activeProject->getName(),this);
    }
    connect(theDiag,SIGNAL(switchProject(QString)),this,SLOT(switchProject(QString)));
    theDiag->exec();

}

void flare::on_actionCut_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc !=0)
    doc->cut();
}

void flare::on_actionCopy_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc !=0)
        doc->copy();
}


void flare::on_actionPaste_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc != 0)
    doc->paste();
}

void flare::on_actionSelect_All_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc !=0)
    doc->selectAll();
}

void flare::on_actionFind_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc!=0)
    doc->find();
}

void flare::on_actionFind_Next_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    doc->findNext();
}

void flare::on_actionSave_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc != 0)
        doc->saveDocument();
}

void flare::on_actionSave_As_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc !=0)
    {
        QString fileName = QFileDialog::getSaveFileName(this,"Save As");
        doc->saveDocument(fileName);
    }
}

void flare::on_actionClose_triggered()
{
    this->editor->closeActiveSubWindow();
}

void flare::on_actionClose_All_triggered()
{
    this->editor->closeAllSubWindows();
}

void flare::on_actionNext_Window_triggered()
{
    this->editor->activateNextSubWindow();
}

void flare::on_actionPrevious_Window_triggered()
{
    this->editor->activatePreviousSubWindow();
}

void flare::on_actionUndo_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc != 0)
        doc->undo();
}


void flare::on_actionRedo_triggered()
{
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc != 0)
        doc->redo();
}

void flare::pasteStatus()
{
    const QMimeData *theData = this->clipboard->mimeData(QClipboard::Clipboard);
    QMdiSubWindow *doc = this->editor->activeSubWindow();
    if (theData->hasText() && doc != 0)
        this->ui->actionPaste->setEnabled(true);

}

void flare::documentOpened()
{
    int numberOfWindows = this->editor->subWindowList().size();
    if (numberOfWindows > 1)
    {
        this->ui->actionNext_Window->setEnabled(true);
        this->ui->actionPrevious_Window->setEnabled(true);
    }
    this->ui->actionClose->setEnabled(true);
    this->ui->actionClose_All->setEnabled(true);
    this->enableFileActions();

}

void flare::alert(int number)
{
    QMessageBox::information(this->editor,"info",QString("%1").arg(number));

}

void flare::alert(QString message)
{
    QMessageBox::information(this->editor,QString("information"),message);
}

void flare::on_actionOpen_triggered()
{
    QFileDialog asker(this,"Open",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
    QString fileFilter = "Web Documents (*.php *.htm *.html *.css *.js);;PHP Script (*.php);;HTML Pages (*.htm *.html);;Javascript File (*.js);;Cascading Stylesheets (*.css)";
    asker.setFilter(fileFilter);
    int fileCount;
    if (asker.exec())
    {
        QStringList listOfFiles = asker.selectedFiles();
        fileCount = listOfFiles.size();
        //alert(fileCount);
        for (int currFile=0;currFile<fileCount;currFile++)
        {
            QFile currentFile(listOfFiles.at(currFile));
            QString filePath = currentFile.fileName();
            QStringList sectionsOfPath = filePath.split(QChar('/'));
            QString fileName = sectionsOfPath.at(sectionsOfPath.size()-1); //extracts the filename
           // alert(fileName);
            QStringList sectionsOfName = fileName.split(".");
            QString fileExtension = sectionsOfName.at(sectionsOfName.size()-1);
            Document *doc;

            if (fileExtension == "html" || fileExtension == "htm")
            {
                doc = new HTMLDocument(filePath,this->editor);
            }
            else if (fileExtension == "php")
            {
                doc = new PHPDocument(filePath,this->editor);
            }
            else if (fileExtension =="css")
            {
                doc =  new CSSDocument(filePath,this->editor);
            }
            else if (fileExtension == "js")
            {
                doc = new JSDocument(filePath,this->editor);
            }
             this->editor->addSubWindow(doc);
             this->initializeDocument(doc);
             doc->setWindowState(Qt::WindowMaximized);
             doc->show();
             emit newdocument();

        }

    }

}

void flare::on_actionNew_triggered()
{
    this->newHTMLFile();
    /*this->newfile = new NewFileDialog(this);

    QListWidgetItem *item;

    newfile->setModal(true);
    if (this->newfile->exec())
    {
        /*item  = this->newfile->filetypelist->currentItem();
        if (item == this->newfile->phpFile)
            this->newPHPFile();
        else if (item == this->newfile->cssFile)
            this->newCSSFile();
        else if (item == this->newfile->htmlFile)
            this->newHTMLFile();
        else if (item == this->newfile->jsFile)
            this->newJSFile();
        else if (item == this->newfile->flexFile)
            this->newFlexFile();

    }*/
}

void flare::on_actionCascade_Windows_triggered()
{
    this->editor->cascadeSubWindows();
}

void flare::on_actionNew_Window_2_triggered()
{
    flare *newWindow = new flare(this);
    this->listOfWindows.append(newWindow);
    newWindow->show();
}

void flare::htmlCommand()
{
    QAction *action = (QAction *) sender();
    Document *doc = (Document *) this->editor->activeSubWindow();
    if (doc == 0)
        return;
    QString typeOfDocument = doc->getType();
    if (typeOfDocument == "HTML")
    {
        HTMLDocument *currentHTMLDocument;
        currentHTMLDocument = (HTMLDocument *) doc;
        currentHTMLDocument->doAction(action);
    }
    else
        return;

}

void flare::about()
{
    QUiLoader loader;
    QFile file(":/about.ui");

    QDialog *dialog = (QDialog *) loader.load(&file);
    connect(dialog->findChild<QPushButton *> ("closeButton"),SIGNAL(clicked()),dialog,SLOT(close()));
    //dialog->move(
    dialog->exec();
}
