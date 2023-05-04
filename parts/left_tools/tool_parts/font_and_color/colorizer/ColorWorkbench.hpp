#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

#include "ColorAlphaBar.hpp"
#include "ColorHueBar.hpp"
#include "ColorSVCanvas.hpp"

class ColorWorkbench : public QDialog
{
  Q_OBJECT

 public:
  ColorWorkbench(QWidget *parent = nullptr);
  ~ColorWorkbench();

  void setColor(QColor color);

 signals:
  void sig_colorChanged(const QColor &color);

 private:
  QColor colorFromStr(QString str);

 private slots:
  void slot_colorDisplay(const QColor &color);
  void slot_colorEdit(const QString &text);

 private:
  ColorSVCanvas *m_canvas;
  ColorHueBar *m_hsvBar;
  ColorAlphaBar *m_alphaSlider;
  QLineEdit *m_lineEdit;
  QPushButton *m_clearBtn;
  QPushButton *m_confirmBtn;
};
