#ifndef CODEEIDTAREA_H
#define CODEEIDTAREA_H
#pragma once

//QT
#include <QWidget>
#include <QPlainTextEdit>
#include <QFont>
#include <QPoint>
//self
//#include "MyHighlighter.hpp"
#include "./parts/right_tab_editor/editor/syntax_highlight/fun/MyHighlighter.hpp"

class QCompleter;
class LeftLineNumberAreaInCodeEditArea;

class CodeEditArea : public QPlainTextEdit
{
  Q_OBJECT
  // friend class >>
  friend class LeftLineNumberAreaInCodeEditArea;

 public:
  explicit CodeEditArea(QWidget *parent = nullptr);
  ~CodeEditArea() = default;

  // file is modify >>
 public:
  bool m_file_is_modified = false;
  auto is_file_modified() const -> bool {
    return this->isWindowModified();
  }
  // file is modify <<

  // format >>
 public:
  // simple format
  auto normal_indent_format()->void;
  // format <<

  // 选中括号中的内容 >>
 public:
  auto select_the_content_in_parentheses()-> void;
 private:
  auto _start_position_in_parentheses(const QHash<QString, QString> &righ_to_left)-> int;
  auto _end_position_in_parentheses(const QHash<QString, QString> &left_to_right)-> int;
  // 选中括号中的内容 <<

 public: QString m_file_path = "";
 public: QString m_file_name = "";


 signals:

 protected:
  //?
  void focusInEvent(QFocusEvent *e) override;

 public:

  //? completer
  bool proceedCompleterBegin(QKeyEvent *e) ;
  //? completer
  void proceedCompleterEnd(QKeyEvent *e) ;

  // completer
  void set_completer(QCompleter *completer);

  // completer
  void insert_completion(QString s);

  //?
  int getIndentationSpaces();

 protected:
  //?
  void keyPressEvent(QKeyEvent *e) override;

 public:
  // ?
  bool m_autoIndentation;
  bool m_autoParentheses;
  bool m_replaceTab;
  QString m_tabReplace;

  // 注释 >>
  enum class CommentType
  {
    SingleComment = 1,
    SingleCommentDelete,
    CoupleComment,
    CoupleCommentDelete,
    MultiComment,
    MultiCommentDelete
  };
  void comment();
  // 注释 >>

  // 字体 >>
 public:
  /**
   * @brief 字体设置
   */
  auto init_font_style()->void;

  // 杂项 >>
 public:

  void document_was_modified(); // 文本是否修改

  auto set_cursor_pos(int row, int col)-> void;            // 设置光标位置

  auto set_cursor_pos(QPoint point)-> void;            // 设置光标位置

  auto get_cursor_pos() const -> QPoint;            // 设置光标位置

  QChar char_under_cursor(int offset = 0) const ;
  QChar char_under_cursor(const QTextCursor &cursor_) const ;

  QString word_under_cursor() const ;

  // highlight 当前行 >>

  /** 高亮当前行
   * @brief 高亮当前行
   */
 protected: void highlight_current_line();

 protected:
  void init_of_highlight_current_line();

  // 语法高亮 >>
 public:
  MyHighLighter *m_my_highlight;
  auto init_semtic_highlight()->void;

  // num area >>

  /** 左侧数字行号
   * @brief m_lineNumberArea
   */
 public: LeftLineNumberAreaInCodeEditArea *m_line_number_area;

  auto init_num_area()->void;

 protected:
  /** num area
   * @brief Method, that's called on editor painting. This method if overloaded for line number area redraw.
   */
  void paintEvent(QPaintEvent *e) override;

  /** num area
   * @brief Method, that's called on any widget resize. This method if overloaded for line number area resizing.
   */
  void resizeEvent(QResizeEvent *e) override;
 public:

  /** num area
   * @brief Method for getting first visible block index.
   * @return Index.
   */
  int get_first_visible_block();

  /** num area
   * @brief Method for updating geometry of line number area.
   */
  void update_line_geometry();

  /** number area
   * @brief Slot, that performs update of internal editor viewport based on line number area width.
   */
  void update_line_number_area_width(int);

  /** number area
   * @brief Slot, that performs update of some part of line number area.
   * @param rect Area that has to be updated.
   */
  void update_line_number_area(const QRect &rect);


  // completer function >>
 public:
  QCompleter *m_completer;
  auto init_completer_function()->void;
};
#endif // CODEEIDTAREA_H
