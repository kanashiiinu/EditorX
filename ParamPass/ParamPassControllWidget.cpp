#include "ParamPassControllWidget.hpp"

#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>

#include "ParamPassDemo.hpp"

ParamPassControllWidget::ParamPassControllWidget(QWidget *parent)
  : QWidget(parent)
{
  auto total = new QVBoxLayout(this);
  auto editor = new QTextEdit(this);
  total->addWidget(editor);
  auto input = new QLineEdit(this);
  total->addWidget(input);
  connect(input, &QLineEdit::returnPressed, [ = ]() {
    editor->setText(ParamPassDemo::translate(input->text()));
  });
}

ParamPassControllWidget::~ParamPassControllWidget()
{
}

