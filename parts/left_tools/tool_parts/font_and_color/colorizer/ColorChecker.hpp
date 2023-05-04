#pragma once

#include <QWidget>

class ColorChecker : public QWidget
{
  Q_OBJECT

 public:
  ColorChecker(QWidget *parent = nullptr);
  ~ColorChecker();

 protected:
  void paintEvent(QPaintEvent *ev) override;

 private:
  int m_checkerSize;
};
