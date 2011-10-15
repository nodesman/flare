#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H
 #include <QSyntaxHighlighter>

 class QTextDocument;

 class SyntaxHighlighter : public QSyntaxHighlighter
 {
     Q_OBJECT

 public:
     SyntaxHighlighter(QString type,QTextDocument *parent = 0);

 protected:
     void highlightBlock(const QString &text);

 private:
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> highlightingRules;
     QRegExp commentStartExpression;
     QRegExp commentEndExpression;
     void cssrules();
     void jsrules();
     void phprules();
     void htmlrules();
     void flexrules();
     QTextCharFormat keywordFormat;
     QTextCharFormat classFormat;
     QTextCharFormat singleLineCommentFormat;
     QTextCharFormat multiLineCommentFormat;
     QTextCharFormat quotationFormat;
     QTextCharFormat functionFormat;
 };

#endif // SYNTAXHIGHLIGHTER_H
