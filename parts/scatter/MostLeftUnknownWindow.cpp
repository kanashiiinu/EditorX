#include "MostLeftUnknownWindow.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>

MostLeftUnknownWindow::MostLeftUnknownWindow(QWidget *parent) : QWidget(parent)
{
  auto h_box = new QHBoxLayout(this);
  h_box->setContentsMargins(0, 0, 0, 0);
  this->setLayout(h_box);
  auto setupGroup = new QGroupBox("Setup", this);
  h_box->addWidget(setupGroup);
  // left
  auto set_up_layout = new QVBoxLayout(setupGroup);
  setupGroup->setLayout(set_up_layout);
  setupGroup->setMaximumWidth(300);

  // left details
  auto code_sample_combo_box  = new QComboBox(setupGroup);
  auto highlighter_combo_box = new QComboBox(setupGroup);
  auto completer_combo_box   = new QComboBox(setupGroup);
  auto style_combo_box       = new QComboBox(setupGroup);

  auto read_only_check_box           = new QCheckBox("Read Only", setupGroup);
  auto word_wrap_check_box           = new QCheckBox("Word Wrap", setupGroup);
  auto parentheses_enabled_check_box = new QCheckBox("Auto Parentheses", setupGroup);
  auto tab_replace_enabled_check_box  = new QCheckBox("Tab Replace", setupGroup);
  auto tab_replace_number_spin_box    = new QSpinBox(setupGroup);
  auto auto_indentation_check_box    = new QCheckBox("Auto Indentation", setupGroup);


  // Adding widgets
  set_up_layout->addWidget(new QLabel(tr("Code sample"), setupGroup));
  set_up_layout->addWidget(code_sample_combo_box);
  set_up_layout->addWidget(new QLabel(tr("Completer"), setupGroup));
  set_up_layout->addWidget(completer_combo_box);
  set_up_layout->addWidget(new QLabel(tr("Highlighter"), setupGroup));
  set_up_layout->addWidget(highlighter_combo_box);
  set_up_layout->addWidget(new QLabel(tr("Style"), setupGroup));
  set_up_layout->addWidget(style_combo_box);
  set_up_layout->addWidget(read_only_check_box);
  set_up_layout->addWidget(word_wrap_check_box);
  set_up_layout->addWidget(parentheses_enabled_check_box);
  set_up_layout->addWidget(tab_replace_enabled_check_box);
  set_up_layout->addWidget(tab_replace_number_spin_box);
  set_up_layout->addWidget(auto_indentation_check_box);
  set_up_layout->addSpacerItem(new QSpacerItem(1, 2, QSizePolicy::Minimum, QSizePolicy::Expanding));
}
