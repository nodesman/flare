#ifndef TEXTDOCUMENT_H
#define TEXTDOCUMENT_H
#include <QtGui>
#include <QTextDocument>

class TextDocument : public QTextDocument
{
public:
    TextDocument();
    TextDocument(QObject *parent);
    TextDocument(QString text,QObject *parent);
};

#endif // TEXTDOCUMENT_H
