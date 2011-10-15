#include "newfiledialog.h"
#include <QtGui>

NewFileDialog::NewFileDialog(QWidget *parent):QDialog(parent)
{

  this->setupUi(this);
  this->createButton->setDisabled(true);
  connect(this->browse,SIGNAL(clicked()),SLOT(browseFile()));
  connect(this->filepath,SIGNAL(textChanged(QString)),SLOT(setButtonStatus()));
  this->filetypes->addItem(QString("Client-Side Files"));
  this->filetypes->addItem(QString("Server-Side Files"));
  this->filetypes->setCurrentRow(0);

  //the client side widgets);
  this->clientside = new QWidget(this);
  QHBoxLayout *clientsidelayout = new QHBoxLayout(this->clientside);
  this->clientside->setLayout(clientsidelayout);
  clientsidelayout->setMargin(0);
  this->clientSideFileTypes = new QListWidget(this);
  this->clientSideFileTypes->addItem(tr("HTML File"));
  this->clientSideFileTypes->addItem(tr("CSS File"));
  this->clientSideFileTypes->addItem(tr("Javascript File"));
  this->clientSideFileTypes->addItem(tr("Adobe Flex MXML File"));
  this->clientSideFileTypes->setCurrentRow(0);

  this->clientsidedetail = new QWidget(this);
  QStackedLayout *clientsidedetaillayout = new QStackedLayout(this->clientsidedetail);
  this->clientsidedetail->setLayout(clientsidedetaillayout);
  clientsidedetaillayout->setMargin(0);

  QListWidget *htmlTemplates = new QListWidget(this);
  htmlTemplates->addItem(tr("Plain HTML File"));
  htmlTemplates->setCurrentRow(0);

  QListWidget *cssTemplates = new QListWidget(this);
  cssTemplates->addItem(tr("Plain CSS File"));
  cssTemplates->setCurrentRow(0);

  QListWidget *jsTemplates = new QListWidget(this);
  jsTemplates->addItem(tr("Plain JS File"));
  jsTemplates->setCurrentRow(0);

  QListWidget *mxmlTemplates = new QListWidget(this);
  mxmlTemplates->addItem(tr("Plain MXML File"));
  mxmlTemplates->setCurrentRow(0);

  clientsidedetaillayout->addWidget(htmlTemplates);
  clientsidedetaillayout->addWidget(cssTemplates);
  clientsidedetaillayout->addWidget(jsTemplates);
  clientsidedetaillayout->addWidget(mxmlTemplates);
  connect(this->clientSideFileTypes,SIGNAL(currentRowChanged(int)),clientsidedetaillayout,SLOT(setCurrentIndex(int)));


  clientsidelayout->addWidget(this->clientSideFileTypes);
  clientsidelayout->addWidget(this->clientsidedetail);

  this->serverSideFileTypes = new QListWidget(this);
  this->serverSideFileTypes->addItem(tr("PHP File"));
  this->serverSideFileTypes->setCurrentRow(0);
  //this->serverSideFileTypes->addItem(tr("Python File"));

  this->detailLayout = new QStackedLayout();
  detailLayout->setMargin(0);
  this->detail->setLayout(this->detailLayout);
  detailLayout->addWidget(this->clientside);
  detailLayout->addWidget(this->serverSideFileTypes);

  connect(this->filetypes,SIGNAL(currentRowChanged(int)),this->detailLayout,SLOT(setCurrentIndex(int)));


}

/*void NewFileDialog::changeFileType()
{
    QString newtype = this->filetypes->currentItem()->text();
    if (newtype == tr("Client-Side Files"))
    {
        this->detailLayout->currentIndex(2);

    }
}*/

void NewFileDialog::browseFile()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save As");


    if (!filename.isEmpty())
    {
        this->filepath->setText(filename);
    }
}


void NewFileDialog::setButtonStatus()
{
    qDebug("here");
    QString filename = this->filepath->text();
    qDebug("here");
    QStringList list = filename.split("/");
    qDebug("here");
    filename = list.at(list.size()-1);
    qDebug("here");
    QString filepath = this->filepath->text().replace(filename,"");
    qDebug("here");
    qDebug(filepath.toAscii());
    qDebug("here");
    QDir dir(filepath);
    if (dir.exists()) //doesn't make sense to me either.
    {
        this->createButton->setEnabled(true);
    }
    else
        this->createButton->setDisabled(true);
}
