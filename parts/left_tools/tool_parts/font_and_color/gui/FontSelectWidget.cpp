#include "FontSelectWidget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QGroupBox>
#include <QToolButton>
#include <QFontDialog>
#include <QGraphicsDropShadowEffect>

static const QString getFontInfo(const QFont &font)
{
  const QString fontInfo = font.toString();
  return fontInfo.split(",")[0] + " " + fontInfo.split(",")[1];
}

FontSelectWidget::FontSelectWidget(QWidget *parent) : QWidget(parent)
{
  auto total_layout = new QVBoxLayout();
  this->setLayout(total_layout);

  auto font_layout = new QHBoxLayout();
  total_layout->addLayout(font_layout);

  // font label
  auto font_label = new QLabel("font", this);
  font_layout->addWidget(font_label);
  auto font_spacer_item = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
  font_layout->addSpacerItem(font_spacer_item);
  // font content
  auto group_layout = new QHBoxLayout();
  total_layout->addLayout(group_layout);

  auto font_group_box = new QGroupBox(this);
  group_layout->addWidget(font_group_box);
  auto font_group_box_layout = new QVBoxLayout();
  font_group_box->setLayout(font_group_box_layout);
  auto font_choose_layout = new QHBoxLayout();
  font_group_box_layout->addLayout(font_choose_layout);
  font_choose_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
  auto font_type_label = new QLabel("font type:");
  font_choose_layout->addWidget(font_type_label);
  font_choose_btn = new QToolButton();
  connect(font_choose_btn, &QToolButton::clicked,
          this, &FontSelectWidget::slot_show_font_dialog);
  font_choose_btn->setText("font");
  font_choose_layout->addWidget(font_choose_btn);
  font_choose_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void FontSelectWidget::slot_show_font_dialog()
{
  bool font_is_exists = false;
  QFont font = QFontDialog::getFont(&font_is_exists, QFont("Helvetica [Cronyx]", 10), this);
  if (font_is_exists) {
    font_choose_btn->setText(getFontInfo(font));
    m_font = font;
    emit this->signal_font_change(m_font);
  }
}
