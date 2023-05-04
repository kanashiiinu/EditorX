#pragma once

#include <QEvent>
#include <QWidget>

class ColorSVCanvas : public QWidget
{
  Q_OBJECT

 public:
  ColorSVCanvas(QWidget *parent = nullptr);
  ~ColorSVCanvas();

  bool setHue(int hue);
  bool setSaturationValue(int saturation, int value);
  bool setSaturationValue(QPoint saturationValue);

  QColor color();
  QRect availabilityRect();
  int margin();

 signals:
  void sig_colorChanged(const QColor &color);

 protected:
  void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;
  bool eventFilter(QObject *obj, QEvent *ev) Q_DECL_OVERRIDE;

 private:
  QPoint valueFromPos(QPoint &pos);
  QPoint posFromValue(QPoint &val);

 private:
  int m_margin;
  int m_radius;
  int m_saturationMax;
  int m_valueMax;

  int m_hue;
  QPoint m_pos;
};
