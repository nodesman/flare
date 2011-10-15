#ifndef FOUNDRY_H
#define FOUNDRY_H
#include <QMdiArea>

extern class Document *as;
/*
  This class acts as the MdiArea.

  Bad name for a class, bash me all you want. But this is where all the
  heavy work happens - the actual work done by the web developer.
  This is the forefront.

*/

class Foundry : public QMdiArea
{
    Q_OBJECT
    QList <Document *> theList;
    Document *activeDocument;
public:
    Foundry();
    void initialize();
    Foundry(QWidget *parent);
    void closeEvent(QCloseEvent *closeEvent);
    int getNumberOfSubWindows();
    bool isFileOpen(QString filePath);
public slots:
    void documentClosed();

signals:
    void someSignal();
    void edit_cut();
    void edit_copy();
    void edit_paste();
};

#endif // FOUNDRY_H
