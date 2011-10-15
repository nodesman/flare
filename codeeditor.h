#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include <QtGui>
#include "completer.h"
#include <QFocusEvent>



 class CodeEditor : public QPlainTextEdit
 {
     Q_OBJECT
     Completer *completer;
 public:
     CodeEditor(QWidget *parent = 0);
     void lineNumberAreaPaintEvent(QPaintEvent *event);
     int lineNumberAreaWidth();
     Completer *getCompleter();
    void setCompleterModel(QStringList *theList);

 protected:
     void resizeEvent(QResizeEvent *event);
     void keyPressEvent(QKeyEvent *e);
     void focusInEvent(QFocusEvent *);


 private slots:
     void updateLineNumberAreaWidth(int newBlockCount);
     void highlightCurrentLine();
     void updateLineNumberArea(const QRect &, int);
     void insertCompletion(QString word);

 private:

     QWidget *lineNumberArea;
     QString textUnderCursor() const;
     void initializeCompleter();
 };


 class LineNumberArea : public QWidget
 {
 public:
     LineNumberArea(CodeEditor *editor) : QWidget(editor) {
         codeEditor = editor;
     }

     QSize sizeHint() const {
         return QSize(codeEditor->lineNumberAreaWidth(), 0);
     }

 protected:
     void paintEvent(QPaintEvent *event) {
         codeEditor->lineNumberAreaPaintEvent(event);
     }

 private:
     CodeEditor *codeEditor;
 };

#endif // LINENUMBERAREA_H
