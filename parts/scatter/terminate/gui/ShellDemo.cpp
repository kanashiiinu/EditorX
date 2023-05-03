#include "ShellDemo.hpp"
#include <QLineEdit>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextCursor>
#include <QScrollBar>
#include <QString>
#include <QDebug>
#include <QPushButton>
#include <memory>

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QMutex>

#include "./parts/scatter/terminate/fun/CommandRunner.hpp"

ShellDemo::ShellDemo(QWidget *parent)
  : QPlainTextEdit(parent)
{

  setReadOnly(true);

  QFont font = this->font();
  font.setPointSize(font.pointSize() + 2);
  this->setFont(font);

  appendPlainText(">>> ");
  edit = new QLineEdit(this->viewport());
  edit->setStyleSheet("border-style:none; background-color:transparent;");
//  edit->setStyleSheet("border-style:none; background-color:lightblue;");

  connect(edit, &QLineEdit::returnPressed,
          this, &ShellDemo::on_edit_finished);

  connect(this->verticalScrollBar(), &QScrollBar::valueChanged,
          this, &ShellDemo::on_scroll_bar_value_changed);
  // fun >>
  m_command_runner = new CommandRunner(this);
  m_command_runner->start_a_command("cmd");

//  QProcess *p = new QProcess(this);
//  p->start("cmd");
//  p->waitForStarted();
  auto p_int = new int(0);
  connect(m_command_runner, &CommandRunner::signal_finished, this, [this]() {qDebug() << "finished";});
//  auto btn = new QPushButton(this);
//  connect(btn, &QPushButton::clicked, this, [this]() {
//    qDebug() << "btn";
//    m_command_runner->run_command("chcp");
//  });
  connect(m_command_runner, &CommandRunner::signal_output_coming, this, [this, p_int](const QString & s) mutable {
    qDebug() << s;
    this->appendPlainText(s);
    this->appendPlainText(">>> ");
    this->update_edit_position();
    edit->show();
    *p_int += 1;
    qDebug() << s;
    qDebug() << "c_" << *p_int;
    this->setFocus();
    edit->setFocus();
  });
  qDebug() << "sh d";
}

void ShellDemo::resizeEvent(QResizeEvent *e)
{
  Q_UNUSED(e);
  update_edit_position();
}

void ShellDemo::on_scroll_bar_value_changed()
{
  update_edit_position();
}

QString ShellDemo::run_command(const QString &cmd)
{
  m_command_runner->run_command(cmd);
  return "xxx";
}

void ShellDemo::on_edit_finished()
{
  QString cmd = edit->text();
  if (cmd.isEmpty())
  {
    return;
  }
  this->moveCursor(QTextCursor::End);
  this->insertPlainText(cmd);
  edit->hide();
  edit->clear();

  run_command(cmd);

//  this->appendPlainText(run_command(cmd));

//  this->appendPlainText(">>> ");
//  this->update_edit_position();
//  edit->show();
//  edit->setFocus();
}

void ShellDemo::update_edit_position()
{
  const auto pos = this->_compute_edit_position();
  edit->move(pos);
  edit->resize(this->viewport()->size().width(),
               edit->size().height());
}

auto ShellDemo::_compute_edit_position()->QPoint
{
  auto cursor = this->textCursor();
  auto cursor_rect = this->cursorRect(cursor);
  auto edit_height = edit->height();
  auto mid_value_of_integer = [](int lhs, int rhs) noexcept -> int {
    return ((lhs bitand rhs) + ((lhs xor rhs) >> 1));
  };
  const auto x = cursor_rect.right();
  const auto top = cursor_rect.top();
  const auto bottom = cursor_rect.bottom();
  const auto y = mid_value_of_integer(top, bottom ) - edit_height / 2;
  return QPoint{x, y};
}
