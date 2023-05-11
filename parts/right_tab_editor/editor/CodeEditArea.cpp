#include "CodeEditArea.hpp"
#include "LeftLineNumberAreaInCodeEditArea.hpp"
#include <QScrollBar>
#include <QCompleter>
#include <QList>
#include <QString>
#include <QStringListModel>
#include <QListView>

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
}


// 杂项 >>
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

