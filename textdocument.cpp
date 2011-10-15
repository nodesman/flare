#include "textdocument.h"
#include <QtGui>

 TextDocument::TextDocument()
 {
 }

TextDocument::TextDocument(QObject *parent): QTextDocument(parent)
{
}
TextDocument::TextDocument(QString text,QObject *parent): QTextDocument(parent)
{

}
