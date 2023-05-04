#include "ColorWorkbench.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
ColorWorkbench::ColorWorkbench(QWidget *parent)
  : QDialog(parent, Qt::Popup)
{
  setFixedSize(320, 250);
  setAttribute(Qt::WA_StyledBackground);
  setObjectName("workbench");
  setStyleSheet("#workbench{background-color:white; border:1px solid rgb(245,245,245); border-radius: 6px;}");

  m_canvas = new ColorSVCanvas(this);
  m_canvas->setFixedSize(280, 180);

  m_hsvBar = new ColorHueBar(this);
  m_hsvBar->setFixedHeight(m_canvas->availabilityRect().height());

  m_alphaSlider = new ColorAlphaBar(this);
  m_alphaSlider->setFixedWidth(m_canvas->availabilityRect().width());

  m_lineEdit = new QLineEdit(this);

  m_clearBtn = new QPushButton("clear", this);
  m_clearBtn->setVisible(false);

  m_confirmBtn = new QPushButton("confirm", this);
  connect(m_confirmBtn, &QPushButton::clicked, this, [this] {
    accept();
  });

  connect(m_hsvBar, &ColorHueBar::sig_valueChanged, this, [this](int val) {
    m_canvas->setHue(val);
  });
  connect(m_canvas, &ColorSVCanvas::sig_colorChanged, this, [this](const QColor & color) {
    m_alphaSlider->setColor(color);
  });
  connect(m_alphaSlider, &ColorAlphaBar::sig_colorChanged, this, &ColorWorkbench::slot_colorDisplay);
  connect(m_lineEdit, &QLineEdit::textEdited, this, &ColorWorkbench::slot_colorEdit);

  // 布局
  auto handleLayout = new QHBoxLayout;
  handleLayout->addSpacing(m_canvas->margin());
  handleLayout->addWidget(m_lineEdit);
  handleLayout->addStretch();
  handleLayout->addWidget(m_clearBtn);
  handleLayout->addWidget(m_confirmBtn);

  auto alphaLayout = new QHBoxLayout;
  alphaLayout->setMargin(0);
  alphaLayout->addSpacing(m_canvas->margin());
  alphaLayout->addWidget(m_alphaSlider);

  auto mainLayout = new QGridLayout(this);
  mainLayout->addWidget(m_canvas, 0, 0);
  mainLayout->addWidget(m_hsvBar, 0, 1);
  mainLayout->addLayout(alphaLayout, 1, 0);
  mainLayout->addLayout(handleLayout, 2, 0, 1, 2);
}

ColorWorkbench::~ColorWorkbench()
{

}

void ColorWorkbench::setColor(QColor color)
{
  m_hsvBar->setValue(color.hsvHue());
  m_canvas->setSaturationValue(color.hsvSaturation(), color.value());
  m_alphaSlider->setValue(color.alpha());
}

QColor ColorWorkbench::colorFromStr(QString str)
{
  QColor color(str);
  if (!color.isValid()) {
    QString tmpStr = str;
    QRegExp rx("[^\\d+^,^.]");
    if (str.contains(rx)) {
      tmpStr.remove(rx);
    }
    if (!tmpStr.isEmpty()) {
      QStringList strList = tmpStr.split(",");
      if (str.contains("rgba") && strList.count() == 4) {
        color.setRgb(strList.at(0).toInt(), strList.at(1).toInt(), strList.at(2).toInt());
        if (strList.at(3).toDouble() > 1) {
          color.setAlpha(strList.at(3).toInt());
        } else {
          color.setAlphaF(strList.at(3).toDouble());
        }
      }
      else if (str.contains("rgb") && strList.count() == 3) {
        color.setRgb(strList.at(0).toInt(), strList.at(1).toInt(), strList.at(2).toInt());
      }
      else if (str.contains("hsv") && strList.count() == 3) {
        color.setHsv(strList.at(0).toInt(), strList.at(1).toInt(), strList.at(2).toInt());
      }
    }
  }

  return color;
}

void ColorWorkbench::slot_colorDisplay(const QColor &color)
{
  QRegExp reg("(\\.){0,1}0+$");// 去除末尾0
  m_lineEdit->setText(QString("rgba(%1, %2, %3, %4)")
                      .arg(color.red())
                      .arg(color.green())
                      .arg(color.blue())
                      .arg(QString::number(color.alphaF(), 'f', 2).replace(reg, "")));

  emit sig_colorChanged(color);
}

void ColorWorkbench::slot_colorEdit(const QString &text)
{
  QColor color = colorFromStr(text);
  if (color.isValid()) {
    disconnect(m_alphaSlider, &ColorAlphaBar::sig_colorChanged, this, &ColorWorkbench::slot_colorDisplay);
    setColor(color);
    connect(m_alphaSlider, &ColorAlphaBar::sig_colorChanged, this, &ColorWorkbench::slot_colorDisplay);

    emit sig_colorChanged(color);
  }
}
