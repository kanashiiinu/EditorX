#pragma once

#include <QLabel>
#include <QPushButton>
#include "ColorChecker.hpp"
#include "ColorWorkbench.hpp"

class ColorPicker : public QLabel
{
  Q_OBJECT

 public:
  ColorPicker(QWidget *parent = nullptr);
  ~ColorPicker();

 protected:
  void resizeEvent(QResizeEvent *ev) override;

 private:
  void setColor(const QColor &color);

 private slots:
  void slot_showPopup();
  void slot_colorChanged(const QColor &color);

 public:
 signals:
  void signal_color_changed(const QColor &color);
 private:
  QPushButton *m_button;
  ColorChecker *m_checker;
  ColorWorkbench *m_popup;

  QColor m_curColor;
  QColor m_oriColor;
};
