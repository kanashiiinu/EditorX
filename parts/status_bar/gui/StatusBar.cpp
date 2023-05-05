#include "StatusBar.hpp"

#include <QToolButton>
#include <QLabel>
#include <QSpacerItem>
#include <QComboBox>
#include <QString>
#include <QDebug>
#include <QList>

StatusBar::StatusBar(QWidget *parent)
  : QStatusBar(parent)
{
  auto lang_box = new QComboBox(this);
  lang_box->addItems(QStringList() << "c++" << "json" << "txt" << "xml");
  this->addPermanentWidget(lang_box);
  auto tab_label = new QLabel("tab: ", this);
  this->addPermanentWidget(tab_label);
  auto tab_box = new QComboBox(this);
  tab_box->addItems(QStringList() << "2" << "4" << "8");
  this->addPermanentWidget(tab_box);
//  auto row_label = new QLabel(this);
//  row_label->setFixedWidth(20);
//  this->addPermanentWidget(row_label);
//  auto column_label = new QLabel(this);
//  column_label->setFixedWidth(20);
//  this->addPermanentWidget(column_label);
  cursor_pos_label = new QLabel(this);
  this->addPermanentWidget(cursor_pos_label);

  // connect
  {
    using fun_type = void (QComboBox::*)(const QString &);
    connect(lang_box, (fun_type)&QComboBox::currentIndexChanged,
            this, &StatusBar::slot_lang_change);
    connect(tab_box, (fun_type)&QComboBox::currentIndexChanged,
            this, &StatusBar::slot_tab_width_change);
  }
}

void StatusBar::set_cursor_pos(int line, int column)
{
  cursor_pos_label->setText(tr("line %1, column %2").arg(line).arg(column));
}

void StatusBar::slot_tab_width_change(const QString &tab_with)
{
  emit this->signal_tab_width_change(tab_with);
}

void StatusBar::slot_lang_change(const QString &lang)
{
  emit this->signal_lang_change(lang);
}
