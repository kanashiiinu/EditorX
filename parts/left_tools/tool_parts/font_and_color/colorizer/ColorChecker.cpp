#include "ColorChecker.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QRect>

ColorChecker::ColorChecker(QWidget *parent)
  : QWidget(parent)
  , m_checkerSize(6)
{

}

ColorChecker::~ColorChecker()
{

}

void ColorChecker::paintEvent(QPaintEvent *ev)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  int rowCount = ev->rect().height() / m_checkerSize + 1;
  int colCount = ev->rect().width() / m_checkerSize + 1;

  for (int i = 0; i < rowCount; i++) {
    for (int j = 0; j < colCount; j++) {
      QRect checkerRect(m_checkerSize * j, m_checkerSize * i, m_checkerSize, m_checkerSize);

      QColor checkerColor;
      if (((j + i % 2) % 2) == 0) {
        checkerColor = Qt::darkGray;
      } else {
        checkerColor = Qt::white;
      }
      painter.fillRect(checkerRect, checkerColor);
    }
  }
}
