#include "MyHighlighter.hpp"
#include <QDebug>
#include <algorithm>

MyHighLighter::MyHighLighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
{

  HighlightingRule rule;
  m_keyword_format.setForeground(QColor("#c678dd"));
//  m_keyword_format.setFontWeight(QFont::Bold);
  rule.format = m_keyword_format;
  // 关键字高亮
  QStringList keywordPatterns = {
    R"(\bchar\b)",
    R"(\bclass\b)",
    R"(\bconst\b)",
    R"(\bdouble\b)",
    R"(\benum\b)",
    R"(\bexplicit\b)",
    R"(\bfriend\b)",
    R"(\binline\b)",
    R"(\bint\b)",
    R"(\blong\b)",
    R"(\bnamespace\b)",
    R"(\boperator\b)",
    R"(\bprivate\b)",
    R"(\bprotected\b)",
    R"(\bpublic\b)",
    R"(\bshort\b)",
    R"(\bsigned\b)",
    R"(\bstatic\b)",
    R"(\bstruct\b)",
    R"(\btemplate\b)",
    R"(\btypename\b)",
    R"(\bunion\b)",
    R"(\bunsigned\b)",
    R"(\bvirtual\b)",
    R"(\bvoid\b)",
    R"(\bvolatile\b)",
    R"(not_eq)",
    R"(and)",
    R"(not)",
    R"(or)",
    R"(for)",
    R"(return)",
    R"(auto)"
  };
  {
    const auto old_size = m_high_lighting_rules.size();
    m_high_lighting_rules.resize(old_size + keywordPatterns.size());
    auto str_to_rule = [&rule](auto pattern) {
      rule.pattern = QRegExp(pattern);
      return rule;
    };
    std::transform(std::cbegin(keywordPatterns), std::cend(keywordPatterns), std::next(std::begin(m_high_lighting_rules), old_size), str_to_rule);
  }

//  m_class_format.setFontWeight(QFont::Bold);
  m_class_format.setForeground(Qt::darkMagenta);
  // Q开头的词
  rule.pattern = QRegExp(R"(\bQ[A-Za-z]+b\)");
  rule.format = m_class_format;
  m_high_lighting_rules.append(rule);

  m_single_line_comment_format.setForeground(Qt::red);
  // 匹配//
  rule.pattern = QRegExp("//[^\n]*");
  rule.format = m_single_line_comment_format;
  m_high_lighting_rules.append(rule);
  m_multi_line_comment_format.setForeground(Qt::green);

  m_quotation_format.setForeground(Qt::darkGreen);
  rule.pattern = QRegExp("\".*\"");
  rule.format = m_quotation_format;
  m_high_lighting_rules.append(rule);

//  m_function_format.setFontItalic(true);
  m_function_format.setForeground(Qt::yellow);
  // 匹配()前的词
  rule.pattern = QRegExp(R"(\b[A-Za-z0-9_]+(?=\())");
  rule.format = m_function_format;
  m_high_lighting_rules.append(rule);

  // namespace
  rule.pattern = QRegExp(R"(\b[a-zA-Z0-9_]+(?=\:))");
  QTextCharFormat t;
  t.setForeground(QBrush(QColor("#4EC9B0")));
  rule.format = t;
  m_high_lighting_rules.append(rule);

  m_comment_start_expression = QRegExp(R"(/\*)");
  m_comment_end_expression = QRegExp(R"(\*/)");
}

void MyHighLighter::highlightBlock(const QString &text)
{
  foreach (const HighlightingRule &rule, m_high_lighting_rules) {
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
  if (previousBlockState() != 1) startIndex = m_comment_start_expression.indexIn(text);


  while (startIndex >= 0) {
    int endIndex = m_comment_end_expression.indexIn(text, startIndex);
    int commentLength;
    if (endIndex == -1) {
      setCurrentBlockState(1);
      commentLength = text.length() - startIndex;
    } else {
      commentLength = endIndex - startIndex + m_comment_end_expression.matchedLength();
    }
    setFormat(startIndex, commentLength, m_multi_line_comment_format);
    startIndex = m_comment_start_expression.indexIn(text, startIndex + commentLength);
  }
}
