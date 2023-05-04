#include "ColorDialog.hpp"
#include "ColorPicker.hpp"
#include "ColorWidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>

ColorDialog::ColorDialog(QWidget *parent)
  : QWidget(parent)
{
  auto total_layout = new QHBoxLayout();
  this->setLayout(total_layout);

  auto e1 = new ColorPicker(this);
  auto e2 = new ColorWidget(this);
//  auto e2 = new ColorWorkbench(this);

  total_layout->addWidget(e1);
  total_layout->addWidget(e2);

  connect(e1, &ColorPicker::signal_color_changed,
          e2, &ColorWidget::set_color);

//  QColor color = image.pixel(0, 0);
//  red = color.red();
//  green = color.green();
//  blue = color.blue();
//  QString strRed = tr("%1").arg(red & 0xFF, 2, 16, QChar('0'));
//  QString strGreen = tr("%1").arg(green & 0xFF, 2, 16, QChar('0'));
//  QString strBlue = tr("%1").arg(blue & 0xFF, 2, 16, QChar('0'));

//  strDecimalValue = tr("%1, %2, %3").arg(red).arg(green).arg(blue);
//  strHex = tr("#%1%2%3").arg(strRed.toUpper()).arg(strGreen.toUpper()).arg(strBlue.toUpper());
}

ColorDialog::~ColorDialog()
{
}

