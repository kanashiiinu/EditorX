#include "CodeAnalysisWidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QTextEdit>
#include <QDebug>
#include <QPushButton>

CodeAnalysisWidget::CodeAnalysisWidget(QWidget *parent)
  : QWidget(parent)
{
  this->_init_ui();
  this->_init_text();
  //
  connect(m_grammar_check_btn, &QPushButton::clicked,
  [this]() {
    m_command = "";
    emit this->signal_analysis_wait_for_file();
  });
  connect(m_cppcoreguidelines_check_btn, &QPushButton::clicked,
  [this]() {
    m_command = "--checks=-*,cppcoreguidelines-*";
    emit this->signal_analysis_wait_for_file();
  });

}

void CodeAnalysisWidget::_init_ui()
{
  //
  auto total_layout = new QVBoxLayout();
  this->setLayout(total_layout);
  //
  m_output_editor = new QTextEdit(this);
  total_layout->addWidget(m_output_editor);
  //
  auto btn_layout = new QHBoxLayout();
  total_layout->addLayout(btn_layout);
  //
  m_grammar_check_btn = new QPushButton(this);
  m_cppcoreguidelines_check_btn = new QPushButton(this);
  btn_layout->addWidget(m_grammar_check_btn);
  btn_layout->addWidget(m_cppcoreguidelines_check_btn);
}

void CodeAnalysisWidget::_init_text()
{
  //
  m_grammar_check_btn->setText("grammar");
  m_cppcoreguidelines_check_btn->setText("core guidelines");
}

void CodeAnalysisWidget::slot_deal_analysis(QString const &file_path)
{
//  QString command = "clang-tidy";
//  QStringList arguments;
//  arguments << file_path << "--";

  m_command = m_command;
  qDebug() << m_command;
  QProcess process;
  process.start("clang-tidy", this->_command_params(file_path));
  process.waitForFinished(-1);

  QByteArray output = process.readAllStandardOutput();
  QString outputString = QString::fromUtf8(output);

  qDebug() << "Output:" << outputString;
}

QStringList CodeAnalysisWidget::_command_params(QString const &file_path) const
{
  QStringList params;
  if (not m_command.isEmpty())
  {
    params << m_command;
  }
  params << file_path
         << "--";
  return params;
}
