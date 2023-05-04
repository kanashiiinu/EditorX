#include "ColorSelectWidget.hpp"

#include "./parts/left_tools/tool_parts/font_and_color/fun/SimpleTextColorScheme.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QGroupBox>
#include <QColorDialog>
#include <QToolButton>
#include <QGraphicsDropShadowEffect>


class DropShadowEffect : public QGraphicsDropShadowEffect {
  using QGraphicsDropShadowEffect::QGraphicsDropShadowEffect;

 public:
  DropShadowEffect(QWidget *parent = nullptr)
    : QGraphicsDropShadowEffect(parent)
  {
    this->setBlurRadius(2);
    this->setOffset(2, 2);
    this->setColor(QColor("black"));
  }
};

ColorSelectWidget::ColorSelectWidget(QWidget *parent)
  : QWidget(parent)
{
  auto total_layout = new QVBoxLayout();
  this->setLayout(total_layout);

  auto color_layout = new QHBoxLayout();
  total_layout->addLayout(color_layout);
  // color label
  auto color_label = new QLabel("color", this);
  color_layout->addWidget(color_label);
  color_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
  //color content
  auto group_color_layout = new QHBoxLayout();
  total_layout->addLayout(group_color_layout);

  auto color_group_box = new QGroupBox(this);
  group_color_layout->addWidget(color_group_box);

  auto color_group_layout = new QVBoxLayout();
  color_group_box->setLayout(color_group_layout);

  // foreground >>
  auto foreground_color_layout = new QHBoxLayout();
  color_group_layout->addLayout(foreground_color_layout);

  foreground_color_layout->addWidget(new QLabel("foreground"));
  auto foreground_color_btn = new QToolButton();
  foreground_color_btn->setStyleSheet("border-radius: 12px; background-color:red;");
  foreground_color_btn->setGraphicsEffect(new DropShadowEffect());
  foreground_color_layout->addWidget(foreground_color_btn);
  connect(foreground_color_btn, &QToolButton::clicked,
          this, &ColorSelectWidget::slot_get_color_panel);

  // background >>
  auto background_color_layout = new QHBoxLayout();
  color_group_layout->addLayout(background_color_layout);

  background_color_layout->addWidget(new QLabel("background"));
  auto background_color_btn = new QToolButton();
  background_color_btn->setStyleSheet("border-radius: 12px; background-color:red;");
  background_color_btn->setGraphicsEffect(new DropShadowEffect());
  background_color_layout->addWidget(background_color_btn);
  connect(background_color_btn, &QToolButton::clicked,
          this, &ColorSelectWidget::slot_get_color_panel);

  // font >>
  auto font_color_layout = new QHBoxLayout();
  color_group_layout->addLayout(font_color_layout);

  font_color_layout->addWidget(new QLabel("font"));
  auto font_color_btn = new QToolButton();
  font_color_btn->setStyleSheet("border-radius: 12px; background-color:red;");
  font_color_btn->setGraphicsEffect(new DropShadowEffect());
  font_color_layout->addWidget(font_color_btn);
  connect(font_color_btn, &QToolButton::clicked,
          this, &ColorSelectWidget::slot_get_color_panel);

  // comment >>
  auto comment_color_layout = new QHBoxLayout();
  color_group_layout->addLayout(comment_color_layout);

  comment_color_layout->addWidget(new QLabel("comment"));
  auto comment_color_btn = new QToolButton();
  comment_color_btn->setStyleSheet("border-radius: 12px; background-color:red;");
  comment_color_btn->setGraphicsEffect(new DropShadowEffect());
  comment_color_layout->addWidget(comment_color_btn);
  connect(comment_color_btn, &QToolButton::clicked,
          this, &ColorSelectWidget::slot_get_color_panel);

  // keyword >>
  auto keyword_color_layout = new QHBoxLayout();
  color_group_layout->addLayout(keyword_color_layout);

  keyword_color_layout->addWidget(new QLabel("keyword"));
  auto keyword_color_btn = new QToolButton();
  keyword_color_btn->setStyleSheet("border-radius: 12px; background-color:red;");
  keyword_color_btn->setGraphicsEffect(new DropShadowEffect());
  keyword_color_layout->addWidget(keyword_color_btn);
  connect(keyword_color_btn, &QToolButton::clicked,
          this, &ColorSelectWidget::slot_get_color_panel);

  // primitive_type >>
  auto primitive_type_color_layout = new QHBoxLayout();
  color_group_layout->addLayout(primitive_type_color_layout);

  primitive_type_color_layout->addWidget(new QLabel("primitive type"));
  auto primitive_type_color_btn = new QToolButton();
  primitive_type_color_btn->setStyleSheet("border-radius: 12px; background-color:red;");
  primitive_type_color_btn->setGraphicsEffect(new DropShadowEffect());
  primitive_type_color_layout->addWidget(primitive_type_color_btn);
  connect(primitive_type_color_btn, &QToolButton::clicked,
          this, &ColorSelectWidget::slot_get_color_panel);

  //

}

void ColorSelectWidget::slot_get_color_panel()
{
  auto btn = qobject_cast<QToolButton *>(this->sender());
  if (not btn)
  {
    return;
  }
  QColor color = QColorDialog::getColor(QColor(), this);
  if (not color.isValid())
  {
    return;
  }
  btn->setStyleSheet("border-radius: 12px; background: " + color.name() + ";");
}
