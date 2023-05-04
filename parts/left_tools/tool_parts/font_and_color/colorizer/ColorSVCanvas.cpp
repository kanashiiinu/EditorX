#include "ColorSVCanvas.hpp"

#include <QPainter>


ColorSVCanvas::ColorSVCanvas(QWidget *parent)
  : QWidget(parent)
  , m_margin(5)
  , m_radius(m_margin - 1)
  , m_saturationMax(255)
  , m_valueMax(255)
  , m_hue(0)
  , m_pos(QPoint(-1, -1))
{
  installEventFilter(this);
}

ColorSVCanvas::~ColorSVCanvas()
{

}

bool ColorSVCanvas::setHue(int hue)
{
  if (hue < 0 || hue > 359)
    return false;

  m_hue = hue;

  update();
  emit sig_colorChanged(color());

  return true;
}

bool ColorSVCanvas::setSaturationValue(int saturation, int value)
{
  return setSaturationValue(QPoint(saturation, value));
}

bool ColorSVCanvas::setSaturationValue(QPoint saturationValue)
{
  if (!QRect(0, 0, 256, 256).contains(saturationValue))
    return false;

  m_pos = posFromValue(saturationValue);

  update();
  emit sig_colorChanged(color());

  return true;
}

QColor ColorSVCanvas::color()
{
  QPoint tmpVal = valueFromPos(m_pos);
  QColor tmpColor;
  tmpColor.setHsv(m_hue, tmpVal.x(), tmpVal.y());
  return tmpColor;
}

QRect ColorSVCanvas::availabilityRect()
{
  QRect tmpRect(m_margin, m_margin, width() - m_margin * 2, height() - m_margin * 2);
  return tmpRect;
}

int ColorSVCanvas::margin()
{
  return m_margin;
}

void ColorSVCanvas::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QRect rect = availabilityRect();

  // 绘制从左到右RGB(255,255,255)到HSV(h,255,255)的渐变
  QLinearGradient linearGradientH(rect.topLeft(), rect.topRight());
  linearGradientH.setColorAt(0, Qt::white);
  QColor color;
  color.setHsv(m_hue, m_saturationMax, m_valueMax);
  linearGradientH.setColorAt(1, color);
  painter.fillRect(rect, linearGradientH);

  // 绘制顶部颜色值为RGBA(0,0,0,0)到最底部RGBA(0,0,0,255)的渐变
  QLinearGradient linearGradientV(rect.topLeft(), rect.bottomLeft());
  linearGradientV.setColorAt(0, QColor(0, 0, 0, 0));
  linearGradientV.setColorAt(1, QColor(0, 0, 0, 255));
  painter.fillRect(rect, linearGradientV);

  painter.setPen(QColor(Qt::darkGray));
  painter.drawEllipse(m_pos, m_radius, m_radius);
}

void ColorSVCanvas::resizeEvent(QResizeEvent *)
{
  if (m_pos == QPoint(-1, -1)) {
    setSaturationValue(255, 255);
  }
}

bool ColorSVCanvas::eventFilter(QObject *obj, QEvent *ev)
{
  if (obj == this) {
    if (ev->type() == QEvent::MouseButtonPress) {
      if (availabilityRect().contains(mapFromGlobal(QCursor::pos()))) {
        m_pos = mapFromGlobal(QCursor::pos());
        update();
        emit sig_colorChanged(color());
      }
    }
  }
  return QWidget::eventFilter(obj, ev);
}

QPoint ColorSVCanvas::valueFromPos(QPoint &pos)
{
  QRect tmpRect = availabilityRect();

  QPoint tmpPos = pos - tmpRect.topLeft();
  int saturation = tmpPos.x() * m_saturationMax / tmpRect.width();
  int value = qAbs(tmpPos.y() * m_valueMax / tmpRect.height() - m_valueMax);

  return QPoint(saturation, value);
}

QPoint ColorSVCanvas::posFromValue(QPoint &val)
{
  QRect tmpRect = availabilityRect();

  int tmpX = val.x() * tmpRect.width() / m_saturationMax;
  int tmpY = qAbs(val.y() - m_valueMax) * tmpRect.height() / m_valueMax;

  return QPoint(tmpX, tmpY) + tmpRect.topLeft();
}
