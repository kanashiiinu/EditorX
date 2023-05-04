#include "FontAndColorWidget.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QGroupBox>
#include <QToolButton>
#include <QGraphicsDropShadowEffect>
#include "FontSelectWidget.hpp"
#include "ColorSelectWidget.hpp"


FontANdColorWidget::FontANdColorWidget(QWidget *parent)
  : QWidget(parent)
{
  auto total_layout = new QVBoxLayout();
  this->setLayout(total_layout);
  total_layout->addWidget(new FontSelectWidget(this));
  // color label
  total_layout->addWidget(new ColorSelectWidget(this));

}
