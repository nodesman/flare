#include "syntaxhighlighter.h"
#include "settings.h"
#include <QtGui>

 #include "syntaxhighlighter.h"

 SyntaxHighlighter::SyntaxHighlighter(QString theType, QTextDocument *parent)
     : QSyntaxHighlighter(parent)
 {
     HighlightingRule rule;
     keywordFormat.setForeground(Qt::darkBlue);
     keywordFormat.setFontWeight(QFont::Bold);
     if (theType == "Flex")
        this->flexrules();
     else if (theType == "HTML")
         this->htmlrules();
     else if (theType == "PHP")
         this->phprules();
     else if (theType == "JS")
         this->jsrules();
     else if (theType == "CSS")
         this->cssrules();

     classFormat.setFontWeight(QFont::Bold);
     classFormat.setForeground(Qt::darkMagenta);
     rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
     rule.format = classFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp("//[^\n]*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     multiLineCommentFormat.setForeground(Qt::red);

     quotationFormat.setForeground(Qt::darkGreen);
     rule.pattern = QRegExp("\".*\"");
     rule.format = quotationFormat;
     highlightingRules.append(rule);

     functionFormat.setFontItalic(true);
     functionFormat.setForeground(Qt::blue);
     rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
     rule.format = functionFormat;
     highlightingRules.append(rule);

     commentStartExpression = QRegExp("/\\*");
     commentEndExpression = QRegExp("\\*/");
 }

 void SyntaxHighlighter::highlightBlock(const QString &text)
 {
     foreach (const HighlightingRule &rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
         }
     }
     setCurrentBlockState(0);

     int startIndex = 0;
     if (previousBlockState() != 1)
         startIndex = commentStartExpression.indexIn(text);

     while (startIndex >= 0) {
         int endIndex = commentEndExpression.indexIn(text, startIndex);
         int commentLength;
         if (endIndex == -1) {
             setCurrentBlockState(1);
             commentLength = text.length() - startIndex;
         } else {
             commentLength = endIndex - startIndex
                             + commentEndExpression.matchedLength();
         }
         setFormat(startIndex, commentLength, multiLineCommentFormat);
         startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
     }
 }

 void SyntaxHighlighter::flexrules()
 {
     HighlightingRule rule;
     QStringList keywordPatterns;
     keywordPatterns << ">" << "mx:[a-zA-Z]+";
     keywordFormat.setForeground(Qt::darkGreen);
     keywordFormat.setFontWeight(QFont::Bold);
     foreach (const QString &pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
         rule.format = keywordFormat;
         highlightingRules.append(rule);
     }

 }


 void SyntaxHighlighter::cssrules()
 {
     HighlightingRule rule;
     QStringList keywordPatterns;
     QFile cssrulefile("./settings/cssrules.rul");
     cssrulefile.open(QIODevice::ReadOnly);
     QTextStream reader(&cssrulefile);

     while (!reader.atEnd())
     {
        QString theLine = reader.readLine();
        keywordPatterns.append(theLine);
     }
     //keywordPatterns << ">" << "mx:[a-zA-Z]+";
     keywordFormat.setForeground(Qt::darkGreen);
     keywordFormat.setFontWeight(QFont::Bold);
     foreach (const QString &pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
         rule.format = keywordFormat;
         highlightingRules.append(rule);
     }

 }


 void SyntaxHighlighter::phprules()
 {
 }


 void SyntaxHighlighter::jsrules()
 {
 }


 void SyntaxHighlighter::htmlrules()
 {
 }
