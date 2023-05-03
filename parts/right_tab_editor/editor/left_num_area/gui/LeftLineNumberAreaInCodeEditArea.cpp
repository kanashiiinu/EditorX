#include "LeftLineNumberAreaInCodeEditArea.hpp"
#include "./parts/right_tab_editor/editor/gui/CodeEditArea.hpp"
// Qt
#include <QTextEdit>
#include <QPainter>
#include <QPaintEvent>
#include <QTextBlock>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include <QDebug>
//std
#include <algorithm>

LeftLineNumberAreaInCodeEditArea::LeftLineNumberAreaInCodeEditArea(CodeEditArea *parent):
  QWidget(parent),
  m_code_edit_area(parent)
{
}


// code_editor的行数除以10即推荐宽度
QSize LeftLineNumberAreaInCodeEditArea::sizeHint() const
{
  if (m_code_edit_area == nullptr) { return QWidget::sizeHint(); }
  // Calculating width
  int digits = 1;
  int max = std::max(1, m_code_edit_area->document()->blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }
  int space = 13 + m_code_edit_area->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
  return {space, 0};
}

void LeftLineNumberAreaInCodeEditArea::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  // Clearing rect to update
//  painter.fillRect(
//    event->rect(),
//    m_syntaxStyle->getFormat("Text").background().color()
//  );
  //
  auto blockNumber = m_code_edit_area->get_first_visible_block();
  auto block       = m_code_edit_area->document()->findBlockByNumber(blockNumber);
  int top = (int) m_code_edit_area->blockBoundingGeometry(block).translated(m_code_edit_area->contentOffset()).top();
  int bottom = top + (int) m_code_edit_area->blockBoundingRect(block).height();
//  auto currentLine = m_syntaxStyle->getFormat("CurrentLineNumber").foreground().color();
//  auto otherLines  = m_syntaxStyle->getFormat("LineNumber").foreground().color();
  auto currentLine = QColor("lightblue");
  auto otherLines  = QColor("white");
  painter.setFont(m_code_edit_area->font());
  while (block.isValid() && top <= event->rect().bottom())
  {
    if (block.isVisible() && bottom >= event->rect().top())
    {
      QString number = QString::number(blockNumber + 1);
      auto isCurrentLine = m_code_edit_area->textCursor().blockNumber() == blockNumber;
      painter.setPen(isCurrentLine ? currentLine : otherLines);
      painter.drawText(
        -6,
        top,
        sizeHint().width(),
        m_code_edit_area->fontMetrics().height(),
        Qt::AlignRight,
        number
      );
    }
    block = block.next();
    top = bottom;
    bottom = top + (int) m_code_edit_area->document()->documentLayout()->blockBoundingRect(block).height();
    ++blockNumber;
  }
}
