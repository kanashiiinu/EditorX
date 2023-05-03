#include "CodeEditArea.hpp"
//#include "LeftLineNumberAreaInCodeEditArea.hpp"
#include "./parts/right_tab_editor/editor/left_num_area/gui/LeftLineNumberAreaInCodeEditArea.hpp"
#include <QScrollBar>
#include <QCompleter>
#include <QList>
#include <QString>
#include <QPushButton>
#include <QStringListModel>
#include <QListView>
#include <QDebug>
//self
#include "./tools/tools.hpp"
// std
#include <vector>
#include <stack>
#include <algorithm>

static QVector<QPair<QString, QString>> parentheses = {
  {"(", ")"},
  {"{", "}"},
  {"[", "]"},
  {"\"", "\""},
  {"'", "'"}
};

CodeEditArea::CodeEditArea(QWidget *parent)
  : QPlainTextEdit(parent)
  , m_my_highlight(nullptr)
  , m_line_number_area(nullptr)
{
  this->init_font_style();

  this->init_semtic_highlight();
  //
  this->init_of_highlight_current_line();
  //
  this->init_num_area();
  // completer
  this->init_completer_function();

  connect(this, &QPlainTextEdit::textChanged, this, [this]() {
    qDebug() << this->get_cursor_pos();
  });

  auto btn = new QPushButton(this);
//  connect(btn, &QPushButton::clicked,
//          this, &CodeEditArea::comment);
//  connect(btn, &QPushButton::clicked,
//          this, &CodeEditArea::normal_indent_format);
  connect(btn, &QPushButton::clicked,
          this, &CodeEditArea::select_the_content_in_parentheses);
}

// format

// simple format

auto CodeEditArea::normal_indent_format() -> void
{ // 简单一键格式化 (不处理特意修改过的文本)
  // 获取当前文本并拆分成列表
  QString cur_text = this->toPlainText();
  auto remove_extra_line_breaks = [](auto & text) {
    text.replace(QRegExp("\n+"), "\n");
  };
  remove_extra_line_breaks(cur_text);
  QStringList line_list = cur_text.split("\n");
  const auto tab_replace_num = 4;
  auto get_tab_str = [](auto num) {
    auto tab = QString(num, ' ');
    return tab;
  };
  auto a_tab = get_tab_str(tab_replace_num);
  const auto indent_start = QString("{");
  const auto indent_end = QString("}");

  // 每一行缩进('{','}')的层数
  // 第一行一定是0
  // 最后一行不用算
  auto get_init_levels = [&line_list]() {
    auto levels = std::vector<int>(line_list.size() + 1);
    std::fill(std::begin(levels), std::end(levels), 0);
    return levels;
  };
  auto levels = get_init_levels();
  {
    auto indent_level = 0;
    auto compute_level_by_ = [&indent_level](auto indent_start, auto indent_end) {
      return [indent_start, indent_end, &indent_level](const QString & line) {
        auto count_of_start = line.count(indent_start, Qt::CaseInsensitive);
        auto count_of_end = line.count(indent_end, Qt::CaseInsensitive);
        indent_level = indent_level + count_of_start - count_of_end;
        indent_level = std::max(indent_level, 0);
        return indent_level;
      };
    };
    auto compute_level = compute_level_by_(indent_start, indent_end);
    // 第一行计算的是第二秆的level
    std::transform(std::begin(line_list), std::end(line_list), std::next(std::begin(levels)), compute_level);
  }
  // 计算缩进的程度
  {
    auto compute_indent_level_by_ = [](auto indent_start, auto indent_end) {
      return [indent_start, indent_end](const QString & line, auto indent_level) {
        auto is_end_mark_first = [&]() {
          const auto start_index = line.indexOf(indent_start);
          const auto end_index = line.indexOf(indent_end);
          if (end_index == -1)
          {
            return false;
          }
          if (start_index == -1)
          {
            return true;
          }
          if (start_index < end_index)
          {
            return false;
          }
          return true;
        };
        if (is_end_mark_first())
        {
          return indent_level - 1;
        }
        return indent_level;
      };
    };
    auto compute_indent_level = compute_indent_level_by_(indent_start, indent_end);
    std::transform(std::begin(line_list), std::end(line_list), std::begin(levels), std::begin(levels), compute_indent_level);
  }


// 按缩进程度处理每行
  {
    auto get_indent_space_ = [](auto space_num)
    {
      return [space_num](auto indent_level)
      {
        return QString(indent_level * space_num, ' ');
      };
    };
    auto get_indent_space_default = get_indent_space_(tab_replace_num);
    auto make_line_indent = [&get_indent_space_default](QString & line, auto indent_level) {
      line = line.trimmed();
      line.prepend(get_indent_space_default(indent_level));
      line.append("\n");
      return line;
    };
    std::transform(std::begin(line_list), std::end(line_list), std::begin(levels), std::begin(line_list), make_line_indent);
  }
// 替换文本
  {
    auto indent_made_text = std::accumulate(std::begin(line_list), std::end(line_list), QString());
    this->setPlainText(indent_made_text);
  }
}


// 选中括号中的内容
auto CodeEditArea::_start_position_in_parentheses(const QHash<QString, QString> &right_to_left)-> int
{
  auto is_parentheses = [&](QChar ch) {
    if (right_to_left.keys().contains(ch))
    {
      return true;
    }
    if (right_to_left.values().contains(ch))
    {
      return true;
    }
    return false;
  };
  auto cursor = this->textCursor();
  std::stack<QString> st = std::stack<QString>();
  cursor.movePosition(QTextCursor::PreviousCharacter);
  auto ch = this->char_under_cursor(cursor);
  for (; true;)
  {
    if (is_parentheses(ch))
    {
      if (right_to_left.contains(ch))
      {
        st.push(ch);
      }
      else
      {
        if (st.empty())
        {
          break;
        }
        else
        {
          st.pop();
        }
      }
    }
    if (cursor.atStart())
    {
      return -1;
    }
    cursor.movePosition(QTextCursor::PreviousCharacter);
    ch = this->char_under_cursor(cursor);
  }
  const auto start_position = cursor.position();
  return start_position;
}

auto CodeEditArea::_end_position_in_parentheses(const QHash<QString, QString> &left_to_right)-> int
{
  auto is_parentheses = [&](QChar ch) {
    if (left_to_right.keys().contains(ch))
    {
      return true;
    }
    if (left_to_right.values().contains(ch))
    {
      return true;
    }
    return false;
  };
  auto cursor = this->textCursor();
  std::stack<QString> st = std::stack<QString>();
  auto ch = this->char_under_cursor(cursor);
  for (; true ;)
  {
    if (is_parentheses(ch))
    {
      if (left_to_right.contains(ch))
      {
        st.push(ch);
      }
      else
      {
        if (st.empty())
        {
          break;
        }
        else
        {
          st.pop();
        }
      }
    }
    if (cursor.atEnd())
    {
      return -1;
    }
    cursor.movePosition(QTextCursor::NextCharacter);
    ch = this->char_under_cursor(cursor);
  }
  const auto end_position = cursor.position();
  return end_position;
}

auto CodeEditArea::select_the_content_in_parentheses()-> void
{
  QHash<QString, QString> left_to_right = {{"{", "}"}, {"[", "]"}, {"(", ")"}};
  QHash<QString, QString> right_to_left = {{"}", "{"}, {"]", "["}, {")", "("}};
  const auto start_position = _start_position_in_parentheses(right_to_left);
  if (start_position == -1)
  {
    return;
  }
  const auto end_position = _end_position_in_parentheses(left_to_right);
  if (end_position == -1)
  {
    return;
  }
  auto cursor = this->textCursor();
  cursor.setPosition(start_position + 1, QTextCursor::MoveAnchor);
  this->setTextCursor(cursor);
  cursor.setPosition(end_position, QTextCursor::KeepAnchor);
  this->setTextCursor(cursor);
}

// 注释 >>
void CodeEditArea::comment()
{
  // 初始化注释格式
  QString comment_format = R"(//)";
  QString multi_comment_begin = R"(/*)";
  QString multi_comment_end = R"(*/)";
  // 根据当前是否选中文本以下处理情况
  // 1.未选择任何内容，注释单选(//)
  // 2.选择的内容可单行注释，//
  // 3.选择的内容可单行注释 /**/
  // 多行
  // 4. 可多行 //
  // 5. 不可多选/**/
  // 解
  // 6. 已注释就解
  auto operate_type = [](QString comment_format, QString multi_comment_begin, QString multi_comment_end, QPlainTextEdit & editor) {
    auto document = editor.document();
    auto cursor = editor.textCursor();
    auto selected_text = cursor.selectedText();
    if (selected_text.isEmpty())
    {
      if (cursor.block().text().trimmed().startsWith(comment_format))
      {
        return CommentType::SingleCommentDelete;
      }
      return CommentType::SingleComment; // 单行 //
    }
    const auto &whole_text = document->toPlainText();
    const auto selected_start = cursor.selectionStart();
    const auto selected_end = cursor.selectionEnd();
    if (whole_text.mid(selected_start, multi_comment_begin.size()) == multi_comment_begin)
    {
      if (whole_text.mid(selected_end - multi_comment_end.size(), multi_comment_end.size()) == multi_comment_end)
      {
        return CommentType::CoupleCommentDelete ; // /**/ 解
      }
    }
    const auto blocks_start = whole_text.lastIndexOf("\n", selected_start);
    const auto blocks_end = whole_text.indexOf("\n", selected_end);
    auto front_str = tool::sub_by_index(document->toPlainText(), blocks_start, selected_start);
    auto back_str = tool::sub_by_index(document->toPlainText(), selected_end, blocks_end);
    qDebug() << front_str << "," << back_str;
    if (not front_str.remove(" ").remove("\n").isEmpty())
    {
      return CommentType::CoupleComment; // /**/ 注释
    }
    if (not back_str.remove(" ").remove("\n").isEmpty())
    {
      return CommentType::CoupleComment;
    }

    auto list = tool::sub_by_index(whole_text, blocks_start, blocks_end).split("\n");
    list.removeAll("");
    for (const QString &line : list)
    {
      if (not line.trimmed().startsWith(comment_format))
      {
        return CommentType::MultiComment;
      }
    }
    return CommentType::MultiCommentDelete;
//    return -1;
  };

  // 若当前未选中文本，直接在光标前添加注释符号
  auto cursor_move_to_start_of_line = [](CodeEditArea & editor) {
    auto current_position = editor.get_cursor_pos();
    current_position.setX(0);
    editor.set_cursor_pos(current_position);
  };
  Q_UNUSED(cursor_move_to_start_of_line);

  auto ret = operate_type(comment_format, multi_comment_begin, multi_comment_end, *this);
  switch (ret)
  {
    case CommentType::SingleComment :
    {
      auto current_position = this->get_cursor_pos();
      current_position.setX(0);
      this->set_cursor_pos(current_position);
      this->textCursor().insertText(comment_format);
      return;
    }
    case CommentType::SingleCommentDelete :
    {
      auto current_position = this->get_cursor_pos();
      current_position.setX(0);
      this->set_cursor_pos(current_position);
      auto cursor = this->textCursor();
      cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
      auto selected_text = cursor.selectedText();
      const auto comment_begin = selected_text.indexOf(comment_format);
      const auto comment_end = comment_begin + comment_format.size();
      selected_text = tool::out_sub_by_index(selected_text, comment_begin, comment_end);
      qDebug() << selected_text;
      cursor.insertText(selected_text);
      return;
    }
    case CommentType::CoupleComment :
    {
      auto cursor = this->textCursor();
      const auto selected_start = cursor.selectionStart();
      const auto selected_end = cursor.selectionEnd();
      cursor.setPosition(selected_end, QTextCursor::MoveAnchor);
      cursor.insertText(multi_comment_end);
      cursor.setPosition(selected_start, QTextCursor::MoveAnchor);
      cursor.insertText(multi_comment_begin);
      return;
    }
    case CommentType::CoupleCommentDelete :
    {
      auto cursor = this->textCursor();
      const auto selected_start = cursor.selectionStart();
      const auto selected_end = cursor.selectionEnd();
      cursor.setPosition(selected_end, QTextCursor::MoveAnchor);
      cursor.setPosition(selected_end - multi_comment_end.size(), QTextCursor::KeepAnchor);
      cursor.removeSelectedText();
      cursor.setPosition(selected_start, QTextCursor::MoveAnchor);
      cursor.setPosition(selected_start + multi_comment_begin.size(), QTextCursor::KeepAnchor);
      cursor.removeSelectedText();
      return;
    }
    case CommentType::MultiComment :
    {
      auto cursor = this->textCursor();
      const auto selected_start = cursor.selectionStart();
      const auto selected_end = cursor.selectionEnd();
      cursor.setPosition(selected_start);
      this->setTextCursor(cursor);
      auto pos_begin = this->get_cursor_pos();
      cursor.setPosition(selected_end);
      this->setTextCursor(cursor);
      auto pos_end = this->get_cursor_pos();
      QPoint pos = QPoint{0, 0};
      qDebug() << pos_begin;
      qDebug() << pos_end;
      for (int y = pos_begin.y(); y <= pos_end.y(); ++y)
      {
        pos.setY(y);
        this->set_cursor_pos(pos);
        this->textCursor().insertText(comment_format);
      }
      return;
    }
    case CommentType::MultiCommentDelete :
    {
      //
      auto cursor = this->textCursor();
      const auto selected_start = cursor.selectionStart();
      const auto selected_end = cursor.selectionEnd();
      cursor.setPosition(selected_start);
      this->setTextCursor(cursor);
      auto pos_begin = this->get_cursor_pos();
      cursor.setPosition(selected_end);
      this->setTextCursor(cursor);
      auto pos_end = this->get_cursor_pos();
      QPoint pos = QPoint{0, 0};
      for (int y = pos_begin.y(); y <= pos_end.y(); ++y)
      {
        pos.setY(y);
        this->set_cursor_pos(pos);
        auto new_cursor = this->textCursor();
        new_cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        auto selected_text = new_cursor.selectedText();
        const auto comment_begin = selected_text.indexOf(comment_format);
        const auto comment_end = comment_begin + comment_format.size();
        selected_text = tool::out_sub_by_index(selected_text, comment_begin, comment_end);
        new_cursor.insertText(selected_text);
      }
      return;
    }
    default:
    {
      return;
    }
  }
  return;
}

// 杂项 >>
void CodeEditArea::document_was_modified()
{ // 判断文件是否改变
  this->setWindowModified(this->document()->isModified());
}

void CodeEditArea::set_cursor_pos(int row, int col)
{
  // row: 1...; col:0...;
  // 设置光标位置
  const QTextBlock block = this->document()->findBlockByLineNumber(row - 1);
  if (block.isValid())
  {
    QTextCursor cursor = this->textCursor();
//    qDebug() << row;
//    qDebug() << col;
//    qDebug() << block.position();
    cursor.setPosition(block.position() + col);
    this->setTextCursor(cursor);
    this->ensureCursorVisible();
  }
}

auto CodeEditArea::set_cursor_pos(QPoint point)-> void {
  this->set_cursor_pos(point.y(), point.x());
}

auto CodeEditArea::get_cursor_pos() const -> QPoint {
  const auto &cursor = this->textCursor();
  const auto row = cursor.blockNumber() + 1;
  const auto col = this->textCursor().columnNumber();
  return QPoint(col, row);
}

QChar CodeEditArea::char_under_cursor(const QTextCursor &cursor_) const
{
  auto cursor = cursor_;
  const auto current_position = cursor.position();
  cursor.setPosition(current_position);
  cursor.setPosition(current_position + 1, QTextCursor::KeepAnchor);
  return cursor.selectedText()[0];
}

QChar CodeEditArea::char_under_cursor(int offset) const {
  auto block = textCursor().blockNumber();
  auto index = textCursor().positionInBlock();
  auto text = document()->findBlockByNumber(block).text();
  index += offset;
  if (index < 0 || index >= text.size()) { return {}; }
  return text[index];
}

QString CodeEditArea::word_under_cursor() const {
  auto tc = textCursor();
  tc.select(QTextCursor::WordUnderCursor);
  return tc.selectedText();
}
// 杂项 <<

// 字体设置 >>
auto CodeEditArea::init_font_style()->void
{
  QTextCharFormat fmt;
  //字体色
  QColor font_color;
  font_color.setRgb(130, 170, 160);
  // 字符的设置
  fmt.setForeground(QBrush(font_color));
  //背景色
//  fmt.setBackground(QBrush(QColor{60, 60, 60}));
  QFont font = QFont("Fira Code", 18);
  this->setFont(font);
  //文本框使用以上设定
  this->setCurrentCharFormat(fmt);
  // 编辑区的设置
//  this->setStyleSheet("QPlainTextEdit{background: rgb(60, 60, 60)}");
//  this->setStyleSheet("QPlainTextEdit{background: white};");
}
// 字体设置 <<

// 语法高亮设置 >>
auto CodeEditArea::init_semtic_highlight()->void
{
  m_my_highlight = new MyHighLighter(this->document());
}
// 语法高亮设置 <<

// 高亮当前行 >>
void CodeEditArea::init_of_highlight_current_line()
{
  // 设置背景色和文本颜色
  auto p = this->palette();
  p.setColor(QPalette::Base, QColor(240, 240, 240));
  p.setColor(QPalette::Text, QColor(Qt::black));
  this->setPalette(p);
  // 连接信号和槽
  connect(this, &QPlainTextEdit::cursorPositionChanged,
          this, &CodeEditArea::highlight_current_line);
}

void CodeEditArea::highlight_current_line()
{
  //
  QList<QTextEdit::ExtraSelection> extraSelections;
  if (not isReadOnly())
  {
    QTextEdit::ExtraSelection selection;
    // 设置当前行的背景色
    auto line_color = QColor(100, 100, 100);
//    QColor line_color = QColor(Qt::yellow).lighter(160);
    selection.format.setBackground(line_color);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    // 获取当前光标所在的块
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }
  // 暂时把括号放在这里 >>
  auto charUnderCursor = [this](int offset = 0)->QChar {
    auto block = textCursor().blockNumber();
    auto index = textCursor().positionInBlock();
    auto text = document()->findBlockByNumber(block).text();
    index += offset;
    if (index < 0 || index >= text.size()) { return {}; }
    return text[index];
  };
  auto currentSymbol = charUnderCursor();
  auto prevSymbol = charUnderCursor(-1);
  for (auto &pair : parentheses)
  {
    int direction;
    QChar counterSymbol;
    QChar activeSymbol;
    auto position = textCursor().position();
    if (pair.first == currentSymbol)
    {
      direction = 1;
      counterSymbol = pair.second[0];
      activeSymbol = currentSymbol;
    }
    else if (pair.second == prevSymbol)
    {
      direction = -1;
      counterSymbol = pair.first[0];
      activeSymbol = prevSymbol;
      position--;
    }
    else
    {
      continue;
    }
    auto counter = 1;
    while (counter != 0 && position > 0 && position < (document()->characterCount() - 1))
    {
      // Moving position
      position += direction;
      auto character = document()->characterAt(position);
      // Checking symbol under position
      if (character == activeSymbol) { ++counter; }
      else if (character == counterSymbol) { --counter; }
    }
//    auto format = m_syntaxStyle->getFormat("Parentheses");
    QTextCharFormat format;
    format.setBackground(QBrush(QColor("lightblue")));
    // Found
    if (counter == 0)
    {
      QTextEdit::ExtraSelection selection{};
      auto directionEnum = direction < 0 ? QTextCursor::MoveOperation::Left : QTextCursor::MoveOperation::Right;
      selection.format = format;
      selection.cursor = textCursor();
      selection.cursor.clearSelection();
      selection.cursor.movePosition( directionEnum, QTextCursor::MoveMode::MoveAnchor, std::abs(textCursor().position() - position) );
      selection.cursor.movePosition( QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor, 1 );
      extraSelections.append(selection);
      selection.cursor = textCursor();
      selection.cursor.clearSelection();
      selection.cursor.movePosition( directionEnum, QTextCursor::MoveMode::KeepAnchor, 1 );
      extraSelections.append(selection);
    }
    break;
  }
  // <<
  this->setExtraSelections(extraSelections);
}
// 高亮当前行 <<

// left number area>>
auto CodeEditArea::init_num_area()->void
{
  m_line_number_area = new LeftLineNumberAreaInCodeEditArea(this);
  // 行数变化更新左侧宽度
  connect(this, &CodeEditArea::blockCountChanged,
          this, &CodeEditArea::update_line_number_area_width);
  // d
  connect(this->verticalScrollBar(), &QScrollBar::valueChanged,
  [this](int) { m_line_number_area->update(); });
}

void CodeEditArea::paintEvent(QPaintEvent *e) {
  this->update_line_number_area(e->rect());
  QPlainTextEdit::paintEvent(e);
}

void CodeEditArea::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);
  this->update_line_geometry();
}

int CodeEditArea::get_first_visible_block() {
  QScrollBar *verticalScrollBar = this->verticalScrollBar();
  int firstVisibleLine = verticalScrollBar->value();
  QTextDocument *document = this->document();
  return document->findBlockByLineNumber(firstVisibleLine).blockNumber();
}

void CodeEditArea::update_line_geometry() {
  // 内容区的Rect
  QRect cr = contentsRect();
  // 固定在左边
  m_line_number_area->setGeometry(QRect(cr.left(), cr.top(), m_line_number_area->sizeHint().width(), cr.height()) );
}

void CodeEditArea::update_line_number_area_width(int) {
  this->setViewportMargins(m_line_number_area->sizeHint().width(), 0, 0, 0);
}

void CodeEditArea::update_line_number_area(const QRect &rect) {
  m_line_number_area->update(0, rect.y(), m_line_number_area->sizeHint().width(), rect.height());
  this->update_line_geometry();
  if (rect.contains(viewport()->rect())) {
    update_line_number_area_width(0);
  }
}

// completer function >>

auto CodeEditArea::init_completer_function()->void
{
  m_completer = new QCompleter();
  m_completer->setFilterMode(Qt::MatchStartsWith);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  QStringList valueList;
  valueList << "plan 1" << "rrm" << "what" << "plan 2" << "what plan" << "rr_MM";
  QStringListModel *listModel;
  listModel = new QStringListModel(valueList, this);
  m_completer->setModel(listModel);
  m_completer->setCompletionColumn(0);
  m_completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
  m_completer->setCaseSensitivity(Qt::CaseSensitive);
  m_completer->setWrapAround(true);
  this->set_completer(m_completer);
}

void CodeEditArea::focusInEvent(QFocusEvent *e) {
  if (m_completer) { m_completer->setWidget(this); }
  QPlainTextEdit::focusInEvent(e);
}

void CodeEditArea::insert_completion(QString s)
{
  if (m_completer->widget() != this) { return; }
  auto tc = textCursor();
  tc.select(QTextCursor::SelectionType::WordUnderCursor);
  tc.insertText(s);
  this->setTextCursor(tc);
}

void CodeEditArea::set_completer(QCompleter *completer)
{
  if (m_completer) {
    disconnect(m_completer, nullptr, this, nullptr);
  }
  m_completer = completer;
  if (!m_completer) { return; }
  m_completer->setWidget(this);
  m_completer->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);
  connect(m_completer, QOverload<const QString &>::of(&QCompleter::activated), this, &CodeEditArea::insert_completion);
}

bool CodeEditArea::proceedCompleterBegin(QKeyEvent *e) {
  if (m_completer && m_completer->popup()->isVisible()) {
    switch (e->key()) {
      case Qt::Key_Enter:
      case Qt::Key_Return:
      case Qt::Key_Escape:
      case Qt::Key_Tab:
      case Qt::Key_Backtab:
        e->ignore();
        return true; // let the completer do default behavior
      default:
        break;
    }
  }
  // todo: Replace with modifiable QShortcut
  auto isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);
  return !(!m_completer || !isShortcut);
}

void CodeEditArea::proceedCompleterEnd(QKeyEvent *e) {
  auto ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if (!m_completer ||
      (ctrlOrShift && e->text().isEmpty()) ||
      e->key() == Qt::Key_Delete)
  {
    return;
  }

  static QString eow(R"(~!@#$%^&*()_+{}|:"<>?,./;'[]\-=)");

  auto isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);
  auto completionPrefix = word_under_cursor();

  if (!isShortcut &&
      (e->text().isEmpty() ||
       completionPrefix.length() < 2 ||
       eow.contains(e->text().right(1))))
  {
    m_completer->popup()->hide();
    return;
  }

  if (completionPrefix != m_completer->completionPrefix())
  {
    m_completer->setCompletionPrefix(completionPrefix);
    m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
  }

  auto cursRect = cursorRect();
  cursRect.setWidth(
    m_completer->popup()->sizeHintForColumn(0) +
    m_completer->popup()->verticalScrollBar()->sizeHint().width()
  );

  m_completer->complete(cursRect);
}

void CodeEditArea::keyPressEvent(QKeyEvent *e) {
  const int defaultIndent = tabStopDistance() / fontMetrics().averageCharWidth();

  auto completerSkip = proceedCompleterBegin(e);

  if (!completerSkip) {
    if (m_replaceTab && e->key() == Qt::Key_Tab &&
        e->modifiers() == Qt::NoModifier) {
      insertPlainText(m_tabReplace);
      return;
    }

    // Auto indentation
    int indentationLevel = getIndentationSpaces();

    int tabCounts = indentationLevel * fontMetrics().averageCharWidth() / tabStopDistance();

    // Have Qt Edior like behaviour, if {|} and enter is pressed indent the two
    // parenthesis
    if (m_autoIndentation &&
        (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) &&
        char_under_cursor() == '}' && char_under_cursor(-1) == '{')
    {
      int charsBack = 0;
      insertPlainText("\n");

      if (m_replaceTab)
        insertPlainText(QString(indentationLevel + defaultIndent, ' '));
      else
        insertPlainText(QString(tabCounts + 1, '\t'));

      insertPlainText("\n");
      charsBack++;

      if (m_replaceTab)
      {
        insertPlainText(QString(indentationLevel, ' '));
        charsBack += indentationLevel;
      }
      else
      {
        insertPlainText(QString(tabCounts, '\t'));
        charsBack += tabCounts;
      }

      while (charsBack--)
        moveCursor(QTextCursor::MoveOperation::Left);
      return;
    }

    // Shortcut for moving line to left
    if (m_replaceTab && e->key() == Qt::Key_Backtab) {
      indentationLevel = std::min(indentationLevel, m_tabReplace.size());

      auto cursor = textCursor();

      cursor.movePosition(QTextCursor::MoveOperation::StartOfLine);
      cursor.movePosition(QTextCursor::MoveOperation::Right,
                          QTextCursor::MoveMode::KeepAnchor, indentationLevel);

      cursor.removeSelectedText();
      return;
    }

    QPlainTextEdit::keyPressEvent(e);

    if (m_autoIndentation && (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)) {
      if (m_replaceTab)
        insertPlainText(QString(indentationLevel, ' '));
      else
        insertPlainText(QString(tabCounts, '\t'));
    }

    if (m_autoParentheses)
    {
      for (auto &&el : parentheses)
      {
        // Inserting closed brace
        if (el.first == e->text())
        {
          insertPlainText(el.second);
          moveCursor(QTextCursor::MoveOperation::Left);
          break;
        }

        // If it's close brace - check parentheses
        if (el.second == e->text())
        {
          auto symbol = char_under_cursor();

          if (symbol == el.second)
          {
            textCursor().deletePreviousChar();
            moveCursor(QTextCursor::MoveOperation::Right);
          }

          break;
        }
      }
    }
  }

  proceedCompleterEnd(e);
}

int CodeEditArea::getIndentationSpaces()
{
  auto blockText = textCursor().block().text();
  int indentationLevel = 0;
  for (auto i = 0; i < blockText.size() && QString("\t ").contains(blockText[i]); ++i)
  {
    if (blockText[i] == ' ')
    {
      indentationLevel++;
    }
    else
    {
      indentationLevel += tabStopDistance() / fontMetrics().averageCharWidth();
    }
  }

  return indentationLevel;
}

