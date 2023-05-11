#ifndef FINDANDREPLACEWIDGET_HPP
#define FINDANDREPLACEWIDGET_HPP
//QT
#include <QWidget>
#include <vector>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTextDocument>
#include <QPushButton>
#include <QTextBlock>
#include <QString>
#include <QTextCursor>
#include <QDebug>
#include <QLabel>
#include <QRegularExpression>
//std
#include <functional>
//self
#include "CodeEditArea.hpp"

class FindAndReplaceWidget : public QWidget
{
  Q_OBJECT

 public:

  explicit FindAndReplaceWidget(QWidget *parent = nullptr);

  ~FindAndReplaceWidget() = default;

 signals:

 public:

  enum State {
    unstart, started
  };

 private:

  int m_total_document_block;					//文档中的文本块的数量,回车符是一个block

  int m_current_document_block_index;				//当前遍历到loginDocNum的第几个数据

  QLineEdit *m_find_input;

  QLineEdit *m_replace_input;

  QPlainTextEdit *m_code_area;
//
  QVector<int> m_positions;
// 当前光标位置
  int m_position;
  QTextCharFormat m_origin_format;
// 默认颜色
  QTextCharFormat m_prev_format;
// 高亮颜色
  QTextCharFormat m_highlight_format;

  QString m_current_search_word;
  //
  bool m_is_case_sensitive = false;

  bool m_is_whole_match = false;

  bool m_is_regular_match = false;

  bool m_is_preserve_case = false;

 public:

  /**
  * @brief 设置code_edit_area,后续对接入的code_edit_area进行操作
  * @param code_edit_area 代码编辑区
  */
  auto set_code_edit_area(CodeEditArea *code_edit_area)->void;

  /**
  * @brief 返回代码编辑区的QTextDocument对象的指针
  */
  auto text_document() noexcept ->QTextDocument*;

  /**
   * @brief 获取查找关键词的可选项，如大小写敏感，使用正则匹配等
   * @return 可选项
   */
  auto get_find_options()->QTextDocument::FindFlags ;

  /**
   * @brief inner_funciton, 根据search_input的输入，返回对应模式（正则|直接匹配）的更新游标的函数
   * @param search_word
   * @return
   */
  auto _update_cursor_when_find(const QString &search_word)-> std::function<QTextCursor(QTextCursor)> ;

  /**
   * @brief 新起一个查找
   */
  auto slot_of_refresh_button()->void ;

  /**
   * @brief 高亮下一个单词
   */
  auto slot_of_next_button()->void ;

  /**
   * @brief 高亮上一个单词
   */
  auto slot_of_previous_button()->void ;

  /**
   * @brief 清除查找
   */
  auto slot_of_clear_button()->void ;

  /**
   * @brief slot_of_replace_one
   */
  auto slot_of_replace_one()->void ;

  /**
   * @brief slot_of_replace_all
   */
  auto slot_of_replace_all()->void ;

//d 可能是给查找到的词上色
  void select_search_word(QTextCursor &cursor, int n, int offset = 0) ;

  //d 可能是把查找到的词去除颜色
  void clear_selection_format(QTextCursor &cursor, int n) ;

//处理上次查找的遗留，为此次查找提供位置信息和颜色信息
  void find_initial(QTextCursor &cursor, const QString &search_word) ;

  //大概是高亮下一个查找到的词
  void find_next(QTextCursor &cursor, const QString &search_word, int offset = 0) ;

//大概是高亮下一个查找到的词
  void find_previous(QTextCursor &cursor, const QString &search_word, int offset = 0) ;

// 通过positions判断有found的单词，再把当前的单词换成replaceWord
  void replace_single_word(QTextCursor &cursor, const QString &replace_word) ;

};

#endif // FINDANDREPLACEWIDGET_HPP
