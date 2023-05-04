#ifndef COLORSELECTWIDGET_HPP
#define COLORSELECTWIDGET_HPP
#pragma once

#include <QWidget>
#include <QColor>

class SimpleTextColorScheme;

class ColorSelectWidget : public QWidget
{
  Q_OBJECT
 public:
  explicit ColorSelectWidget(QWidget *parent = nullptr);
  SimpleTextColorScheme *scheme;
 public slots:
  void slot_get_color_panel();
 signals:


};

#endif // COLORSELECTWIDGET_HPP
