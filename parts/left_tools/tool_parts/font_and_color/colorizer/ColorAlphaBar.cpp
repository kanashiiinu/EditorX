#include "ColorAlphaBar.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>

ColorAlphaBar::ColorAlphaBar(QWidget *parent)
  : QWidget(parent)
  , m_grooveHeight(12)
{
  m_checker = new ColorChecker(this);

  m_slider = new QSlider(Qt::Horizontal, this);
  m_slider->setMaximum(255);
  m_slider->setValue(m_slider->maximum());
  m_slider->setFixedHeight(16);
  setColor(Qt::red);

  connect(m_slider, &QSlider::valueChanged, this, [this] {
    emit sig_colorChanged(color());
  });

  auto mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setSpacing(0);
  mainLayout->addWidget(m_slider);
}

ColorAlphaBar::~ColorAlphaBar()
{

}

void ColorAlphaBar::setColor(QColor oriColor)
{
  m_color = oriColor;
  m_color.setAlpha(255);

  QColor tmpColor(oriColor);
  tmpColor.setAlpha(0);

  m_slider->setStyleSheet(QString("QSlider::groove:horizontal { \
                                        height: %3px; \
                                        background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, \
                                        stop:0 %1, \
                                        stop:1 %2);} \
                                    QSlider::handle:horizontal { \
                                        background: white; \
                                        width: 4px; \
                                        margin: -2px 0; \
                                        border: 1px solid grey; \
                                        border-radius: 2px;} \
                                    QSlider::add-page:horizontal {background: transparent;} \
                                    QSlider::sub-page:horizontal {background: transparent;}")
                          .arg(tmpColor.name(QColor::HexArgb))
                          .arg(m_color.name())
                          .arg(m_grooveHeight));

  emit sig_colorChanged(color());
}

QColor ColorAlphaBar::color()
{
  QColor tmpColor(m_color);
  tmpColor.setAlpha(m_slider->value());
  return tmpColor;
}

void ColorAlphaBar::setValue(int alpha)
{
  m_slider->setValue(alpha);
}

void ColorAlphaBar::resizeEvent(QResizeEvent *)
{
  m_checker->setGeometry(0, (height() - m_grooveHeight) / 2, width(), m_grooveHeight);
}
