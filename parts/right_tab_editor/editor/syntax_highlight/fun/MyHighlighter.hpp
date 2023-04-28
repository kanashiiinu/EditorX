#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>
class MyHighLighter : public QSyntaxHighlighter
{
  Q_OBJECT

 public:
  explicit MyHighLighter(QTextDocument *parent = 0);

  /**
  * @brief highlightBlock
  * @param text
  * @note 通过调用setFormat()来完成高亮的最后一步,高亮那些单词由自己操作
  */
 protected: void highlightBlock(const QString &text) override;

 private:
  struct HighlightingRule
  {
    QRegExp pattern;
    QTextCharFormat format;
  };
  QVector<HighlightingRule> m_high_lighting_rules;

  QRegExp m_comment_start_expression;
  QRegExp m_comment_end_expression;

  QTextCharFormat m_keyword_format;

  QTextCharFormat m_class_format;

  QTextCharFormat m_single_line_key;

  QTextCharFormat m_single_line_value;

  QTextCharFormat m_single_line_comment_format;

  QTextCharFormat m_multi_line_comment_format;

  QTextCharFormat m_quotation_format;

  QTextCharFormat m_function_format;
};
#endif // MYHIGHLIGHTER_H
