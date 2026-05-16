#include "syntaxhighlighter.h"
#include "settings.h"
#include <QtWidgets>

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
     rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
     rule.format = classFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegularExpression("//[^\\n]*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     multiLineCommentFormat.setForeground(Qt::red);

     quotationFormat.setForeground(Qt::darkGreen);
     rule.pattern = QRegularExpression("\".*\"");
     rule.format = quotationFormat;
     highlightingRules.append(rule);

     functionFormat.setFontItalic(true);
     functionFormat.setForeground(Qt::blue);
     rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
     rule.format = functionFormat;
     highlightingRules.append(rule);

     commentStartExpression = QRegularExpression("/\\*");
     commentEndExpression = QRegularExpression("\\*/");
 }

 void SyntaxHighlighter::highlightBlock(const QString &text)
 {
     for (const HighlightingRule &rule : highlightingRules) {
         QRegularExpressionMatchIterator matches = rule.pattern.globalMatch(text);
         while (matches.hasNext()) {
             QRegularExpressionMatch match = matches.next();
             setFormat(match.capturedStart(), match.capturedLength(), rule.format);
         }
     }
     setCurrentBlockState(0);

     int startIndex = 0;
     if (previousBlockState() != 1) {
         QRegularExpressionMatch startMatch = commentStartExpression.match(text);
         startIndex = startMatch.hasMatch() ? startMatch.capturedStart() : -1;
     }

     while (startIndex >= 0) {
         QRegularExpressionMatch endMatch = commentEndExpression.match(text, startIndex);
         int endIndex = endMatch.hasMatch() ? endMatch.capturedStart() : -1;
         int commentLength;
         if (endIndex == -1) {
             setCurrentBlockState(1);
             commentLength = text.length() - startIndex;
         } else {
             commentLength = endIndex - startIndex + endMatch.capturedLength();
         }
         setFormat(startIndex, commentLength, multiLineCommentFormat);
         QRegularExpressionMatch startMatch = commentStartExpression.match(text, startIndex + commentLength);
         startIndex = startMatch.hasMatch() ? startMatch.capturedStart() : -1;
     }
 }

 void SyntaxHighlighter::flexrules()
 {
     HighlightingRule rule;
     QStringList keywordPatterns;
     keywordPatterns << ">" << "mx:[a-zA-Z]+";
     keywordFormat.setForeground(Qt::darkGreen);
     keywordFormat.setFontWeight(QFont::Bold);
     for (const QString &pattern : keywordPatterns) {
         rule.pattern = QRegularExpression(pattern);
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
     for (const QString &pattern : keywordPatterns) {
         rule.pattern = QRegularExpression(pattern);
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
