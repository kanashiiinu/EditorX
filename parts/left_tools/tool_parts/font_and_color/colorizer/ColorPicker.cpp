#include "ColorPicker.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>

ColorPicker::ColorPicker(QWidget *parent)
  : QLabel(parent)
{
  m_checker = new ColorChecker(this);

  m_popup = new ColorWorkbench(this);
  connect(m_popup, &ColorWorkbench::sig_colorChanged, this, &ColorPicker::slot_colorChanged);
  connect(m_popup, &QDialog::finished, this, [this](int result) {
    if (result == QDialog::Accepted) {
      m_oriColor = m_curColor;
    }
    else {
      setColor(m_oriColor);
    }
  });

  m_button = new QPushButton("v", this);
  m_button->setFixedSize(30, 30);
  connect(m_button, &QPushButton::pressed, this, &ColorPicker::slot_showPopup);

  setFixedSize(40, 40);
  setStyleSheet(QString("QLabel{border:1px solid %1; border-radius: 4px; background-color: %2;}")
                .arg(QColor(230, 230, 230).name())
                .arg(QColor(Qt::white).name()));
  setColor(QColor(255, 0, 0, 150));

  auto mainLayout = new QHBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->addWidget(m_button);
}

ColorPicker::~ColorPicker()
{

}

void ColorPicker::resizeEvent(QResizeEvent *)
{
  m_checker->setGeometry(m_button->geometry());
}

void ColorPicker::setColor(const QColor &color)
{
  m_curColor = color;
  m_button->setStyleSheet(QString("QPushButton{border:1px solid %1; background-color:%2}")
                          .arg(QColor(152, 152, 152).name())
                          .arg(color.name(QColor::HexArgb)));
}

void ColorPicker::slot_showPopup()
{
  m_oriColor = m_curColor;
  m_popup->setColor(m_oriColor);

  QPoint tmpPos = mapToGlobal(m_button->geometry().center());
  tmpPos += QPoint(-m_popup->width() / 2, m_button->height() / 2 + 5);
  m_popup->move(tmpPos);

  m_popup->open();
}

void ColorPicker::slot_colorChanged(const QColor &color)
{
  if (m_popup->isVisible()) {
    this->setColor(color);
    emit this->signal_color_changed(color);
  }
}
