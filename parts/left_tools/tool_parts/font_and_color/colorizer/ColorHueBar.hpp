#pragma once

#include <QWidget>
#include <QSlider>

class ColorHueBar : public QWidget
{
  Q_OBJECT

 public:
  ColorHueBar(QWidget *parent = nullptr);
  ~ColorHueBar();

  void setValue(int val);
  int value();

 signals:
  void sig_valueChanged(int val);

 private:
  QSlider *m_slider;
};
