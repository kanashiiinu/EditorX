#pragma once

#include <QWidget>
#include <QSlider>
#include "ColorChecker.hpp"

class ColorAlphaBar : public QWidget
{
  Q_OBJECT

 public:
  ColorAlphaBar(QWidget *parent = nullptr);
  ~ColorAlphaBar();

  void setColor(QColor color);
  QColor color();

  void setValue(int alpha);

 signals:
  void sig_colorChanged(const QColor &color);

 protected:
  void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;

 private:
  ColorChecker *m_checker;
  QSlider *m_slider;
  QColor m_color;

  int m_grooveHeight;
};
