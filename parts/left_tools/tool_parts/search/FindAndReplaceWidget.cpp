//self
#include "FindAndReplaceWidget.hpp"
#include "tools.hpp"
// Qt
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>



FindAndReplaceWidget::FindAndReplaceWidget(QWidget *parent)
  : QWidget(parent)
{
  auto v_layout = new QVBoxLayout(this);
  //
  auto most_top_layout = new QHBoxLayout();
  v_layout->addLayout(most_top_layout);
  auto search_label = new QLabel("search", this);
  search_label->setObjectName("search_label");
  search_label->setStyleSheet("QFrame#search_label{border:0;}");
  search_label->setFrameShape(QFrame::NoFrame);
  most_top_layout->addWidget(search_label);
  most_top_layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
  auto refresh_button = new QPushButton("refresh", this);
  most_top_layout->addWidget(refresh_button);
  auto clear_button = new QPushButton("clear", this);
  most_top_layout->addWidget(clear_button);
  auto fold_button = new QPushButton("fold", this);
  most_top_layout->addWidget(fold_button);
  //
  auto find_function_layout = new QHBoxLayout();
  v_layout->addLayout(find_function_layout);
  auto find_input = new QLineEdit();
  find_function_layout->addWidget(find_input);
  // 大小写敏感
  auto case_sensitive = new QPushButton("case sensitive");
  case_sensitive->setCheckable(true);
  find_function_layout->addWidget(case_sensitive);
  // 全字匹配
  auto whole_match = new QPushButton("whole match");
  whole_match->setCheckable(true);
  find_function_layout->addWidget(whole_match);
  // 正则匹配
  auto regular_match = new QPushButton("regular match");
  regular_match->setCheckable(true);
  find_function_layout->addWidget(regular_match);
  //
  auto previous = new QPushButton("previous");
  find_function_layout->addWidget(previous);
  auto next = new QPushButton("next");
  find_function_layout->addWidget(next);
//
  auto replace_function_layout = new QHBoxLayout();
  v_layout->addLayout(replace_function_layout);
  auto replace_input = new QLineEdit();
  replace_function_layout->addWidget(replace_input);
  // 保留大小写
  auto preserve_case = new QPushButton("preserve case");
  preserve_case->setCheckable(true);
  replace_function_layout->addWidget(preserve_case);
  // 替换
  auto replace_one = new QPushButton("replace one");
  replace_function_layout->addWidget(replace_one);
  // 替换全部
  auto replace_all = new QPushButton("replace all");
  replace_function_layout->addWidget(replace_all);

//
  auto result_info_layout = new QHBoxLayout();
  v_layout->addLayout(result_info_layout);
//  auto result_info = new QLabel("xxx");
//  result_info_layout->addWidget(result_info);
  //
  auto result_show_layout = new QHBoxLayout();
  v_layout->addLayout((result_show_layout));
  auto widget = new QWidget();
  result_show_layout->addWidget(widget);


  //功能连接

  m_find_input = find_input;
  m_replace_input = replace_input;
//  m_code_area = code_area;
//  m_find_input->setText("main");
//  m_code_area->setPlainText(QString("main\nMain\nMAIN\nmaine\nxxma\nin\nmain\n"));
//  m_origin_format = QTextCursor(this->text_document()).charFormat();
  m_highlight_format.setBackground(QBrush(QColor("green")));
  m_highlight_format.setForeground(QBrush(QColor("red")));
//    auto cursor = new QTextCursor(m_text_document);
//    cursor->insertText(QString("main\nMain\nMAIN\nmaine\nxxma\nin\nmain\n"));
  connect(refresh_button, &QPushButton::clicked,
          this, &FindAndReplaceWidget::slot_of_refresh_button);
  connect(clear_button, &QPushButton::clicked,
          this, &FindAndReplaceWidget::slot_of_clear_button);
  connect(fold_button, &QPushButton::clicked,
  this, []() { qDebug() << "nothing now"; });
  connect(previous, &QPushButton::clicked,
          this, &FindAndReplaceWidget::slot_of_previous_button);
  connect(next, &QPushButton::clicked,
          this, &FindAndReplaceWidget::slot_of_next_button);
  connect(case_sensitive, &QPushButton::clicked,
  this, [this]() { m_is_case_sensitive = not m_is_case_sensitive; });
  connect(whole_match, &QPushButton::clicked,
  this, [this]() { m_is_whole_match = not m_is_whole_match; });
  connect(regular_match, &QPushButton::clicked,
  this, [this]() { m_is_regular_match = not m_is_regular_match; });
  connect(preserve_case, &QPushButton::clicked,
  this, [this]() { m_is_preserve_case = not m_is_preserve_case; });
  connect(replace_one, &QPushButton::clicked,
          this, &FindAndReplaceWidget::slot_of_replace_one);
  connect(replace_all, &QPushButton::clicked,
          this, &FindAndReplaceWidget::slot_of_replace_all);
}


auto FindAndReplaceWidget::set_code_edit_area(CodeEditArea *code_edit_area)->void
{
  m_code_area = code_edit_area;
  m_origin_format = QTextCursor(this->text_document()).charFormat();
}

auto FindAndReplaceWidget::text_document() noexcept -> QTextDocument *
{
  return m_code_area->document();
}

auto FindAndReplaceWidget::get_find_options()->QTextDocument::FindFlags
{
  auto flag  = QTextDocument::FindFlags();
  if (m_is_case_sensitive) { flag |= QTextDocument::FindCaseSensitively; }
  if (m_is_whole_match) { flag |= QTextDocument::FindWholeWords; }
  return flag;
}

auto FindAndReplaceWidget::_update_cursor_when_find(const QString &search_word)-> std::function<QTextCursor(QTextCursor)>
{
  if (m_is_regular_match)
  {
    return [ = ](QTextCursor cursor) {
      return this->text_document()->find(QRegularExpression(search_word), cursor, this->get_find_options());
    };
  }
  return [ = ](QTextCursor cursor) {
    return this->text_document()->find(search_word, cursor, this->get_find_options());
  };

}

auto FindAndReplaceWidget::slot_of_refresh_button()->void
{
  m_code_area->setFocus();
  auto search_word = m_find_input->text().trimmed();
  QTextCursor new_cursor(this->text_document());
  this->find_initial(new_cursor, search_word);
  qDebug() << m_positions;
}

auto FindAndReplaceWidget::slot_of_next_button()->void
{
  m_code_area->setFocus();
  auto search_word = m_find_input->text().trimmed();
  QTextCursor new_cursor(this->text_document());
  this->find_next(new_cursor, search_word);
}

auto FindAndReplaceWidget::slot_of_previous_button()->void
{
  m_code_area->setFocus();
  auto search_word = m_find_input->text().trimmed();
  QTextCursor new_cursor(this->text_document());
  this->find_previous(new_cursor, search_word);
}

auto FindAndReplaceWidget::slot_of_clear_button()->void
{
  m_code_area->setFocus();
  auto search_word = m_find_input->text().trimmed();
  QTextCursor new_cursor(this->text_document());
  auto _cursor = new_cursor;
  auto clear_selection = [this](QTextCursor & cursor, int n) {
    if (m_positions.isEmpty()) { return; }
    if (m_position < 0) { return; }
    if (cursor.isNull()) { return; }
    cursor.setPosition(cursor.position() + n, QTextCursor::KeepAnchor);
    cursor.setCharFormat(m_origin_format);
  };
  for (auto position : m_positions)
  {
    _cursor.setPosition(position);
    clear_selection(_cursor, search_word.size());
  }
  m_positions.clear();
  m_position = -1;
}

auto FindAndReplaceWidget::slot_of_replace_one()->void
{
  m_code_area->setFocus();
  auto search_word = m_find_input->text();
  auto replace_word = m_replace_input->text();
  auto new_cursor = m_code_area->textCursor();
  //
  this->slot_of_clear_button();
  this->replace_single_word(new_cursor, replace_word);
  new_cursor.setPosition(0);
  this->find_initial(new_cursor, search_word);
}

auto FindAndReplaceWidget::slot_of_replace_all()->void
{

}
//  auto
//d 可能是给查找到的词上色
void FindAndReplaceWidget::select_search_word(QTextCursor &cursor, int n, int offset)
{
  cursor.setPosition(m_positions[++m_position] - offset);
  if (not cursor.isNull()) {
    cursor.clearSelection();
    cursor.setPosition(m_positions[m_position] - offset + n, QTextCursor::KeepAnchor);
    m_prev_format = cursor.charFormat();
    cursor.setCharFormat(m_highlight_format);
    m_code_area->setTextCursor(cursor);
  }
}
//d 可能是把查找到的词去除颜色
void FindAndReplaceWidget::clear_selection_format(QTextCursor &cursor, int n)
{
  if (m_positions.isEmpty()) { return; }
  if (m_position < 0) { return; }
  cursor.setPosition(m_positions[m_position]);
  if (cursor.isNull()) { return; }
  cursor.setPosition(m_positions[m_position] + n, QTextCursor::KeepAnchor);
  cursor.setCharFormat(m_prev_format);
  m_code_area->setTextCursor(cursor);
}
//处理上次查找的遗留，为此次查找提供位置信息和颜色信息
void FindAndReplaceWidget::find_initial(QTextCursor &cursor, const QString &search_word)
{
//    qDebug() << cursor.charFormat().foreground() << cursor.charFormat().background();
  // 重置查找
  if (not m_positions.isEmpty()) {
    // 如果之前查找了，清除
    this->clear_selection_format(cursor, search_word.size());
    m_positions.clear();
  }
  // 收集查找目标出现的位置
  {
    const int word_len = search_word.size();
    auto new_cursor = cursor;
    new_cursor.setPosition(0);
//      auto text_document = this->text_document();
    //
//      auto options = this->get_find_options();
    auto update_cursor = this->_update_cursor_when_find(search_word);
    while (not (new_cursor = update_cursor(new_cursor)).isNull())
    {
      m_positions.push_back(new_cursor.position() - word_len);
    }
  }
  //
  if (not m_positions.isEmpty())
  {
    m_position = -1;
    while (m_position < m_positions.size() - 1)
    {
      this->select_search_word(cursor, search_word.size());
    }
    m_position = -1;
    this->select_search_word(cursor, search_word.size());
  }
}
//大概是高亮下一个查找到的词
void FindAndReplaceWidget::find_next(QTextCursor &cursor, const QString &search_word, int offset)
{
//    m_code_area->setReadOnly(true);
  this->clear_selection_format(cursor, search_word.size());
  if (m_positions.isEmpty()) { return; }
  if (m_position >= m_positions.size() - 1)
  {
    m_position = -1;
  }
  this->select_search_word(cursor, search_word.size(), offset);
  qDebug() << cursor.position();
}
//大概是高亮下一个查找到的词
void FindAndReplaceWidget::find_previous(QTextCursor &cursor, const QString &search_word, int offset)
{
  this->clear_selection_format(cursor, search_word.size());
  if (m_positions.isEmpty()) { return; }
  if (m_position <= 0)
  {
    m_position = m_positions.size();
  }
  //
  cursor.setPosition(m_positions[--m_position] - offset);
  if (not cursor.isNull()) {
    cursor.clearSelection();
    cursor.setPosition(m_positions[m_position] - offset + search_word.size(), QTextCursor::KeepAnchor);
    m_prev_format = cursor.charFormat();
    cursor.setCharFormat(m_highlight_format);
    m_code_area->setTextCursor(cursor);
  }
}

// 通过positions判断有found的单词，再把当前的单词换成replaceWord
void FindAndReplaceWidget::replace_single_word(QTextCursor &cursor, const QString &replace_word)
{
  if (cursor.selectedText().isEmpty())
  {
    qDebug() << "selected empty" << cursor.position();
    cursor.setPosition(cursor.position());
    cursor.setPosition(cursor.position() - m_find_input->text().trimmed().size(), QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(replace_word);
    return;
  }
  // 其实这个if不加也对
  if (m_positions.isEmpty())
  {
    qDebug() << "positions empty";
    cursor.setPosition(cursor.position());
    cursor.setPosition(cursor.position() - m_find_input->text().trimmed().size(), QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(replace_word);
    return;
  }
  cursor.removeSelectedText();
  cursor.insertText(replace_word);
  m_code_area->setTextCursor(cursor);
}

