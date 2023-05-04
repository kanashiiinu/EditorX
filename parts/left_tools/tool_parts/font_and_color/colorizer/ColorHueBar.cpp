#include "ColorHueBar.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
ColorHueBar::ColorHueBar(QWidget *parent)
  : QWidget(parent)
{
  m_slider = new QSlider(this);
  m_slider->setMaximum(359);
  m_slider->setFixedWidth(16);
  m_slider->setStyleSheet("QSlider::groove:vertical {width: 12px; \
                                background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \
                                    stop:0 hsv(0,255,255), \
                                    stop:0.17 hsv(59,255,255), \
                                    stop:0.33 hsv(119,255,255), \
                                    stop:0.5 hsv(179,255,255), \
                                    stop:0.67 hsv(239,255,255), \
                                    stop:0.83 hsv(299,255,255), \
                                    stop:1 hsv(359,255,255));} \
                            QSlider::handle:vertical { \
                                background: white; \
                                height: 4px; \
                                margin:0px -2px; \
                                border: 1px solid grey; \
                                border-radius: 2px;} \
                            QSlider::add-page:vertical {background: transparent;} \
                            QSlider::sub-page:vertical {background: transparent;}");
  setValue(0);
  connect(m_slider, &QSlider::valueChanged, this, [this] {
    emit sig_valueChanged(value());
  });

  auto mainLayout = new QHBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setSpacing(0);
  mainLayout->addWidget(m_slider);
}

ColorHueBar::~ColorHueBar()
{

}

void ColorHueBar::setValue(int val)
{
  m_slider->setValue(qAbs(val - m_slider->maximum()));
}

int ColorHueBar::value()
{
  return qAbs(m_slider->value() - m_slider->maximum());
}
