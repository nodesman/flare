#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H
 #include <QRegularExpression>
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
         QRegularExpression pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> highlightingRules;
     QRegularExpression commentStartExpression;
     QRegularExpression commentEndExpression;
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
