#include "TerminateWorker.hpp"
#include <QLineEdit>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextCursor>
#include <QScrollBar>
#include <QString>
#include <QDebug>
#include <memory>

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QMutex>
//启用线程执行
TerminateWorker::TerminateWorker(QObject *parent)
  : QObject(parent)
{
  /*TODO*/
}
void TerminateWorker::slot_read_output()
{
  m_output_buffer = m_terminal_interactive->read_output();
  emit this->signal_ready_read_standard_output();
}

void TerminateWorker::slot_read_error()
{
  m_error_buffer = m_terminal_interactive->read_error();
  emit this->signal_ready_read_standard_error();
}

//
auto TerminateWorker::start_a_command(const QString &command_name)->void
{
  m_terminal_interactive = new TerminalInteractive(command_name, this);
//    connect(m_terminal_interactive, &TerminalInteractive::signal_ready_read_standard_output,
//            this, &TerminateWorker::slot_read_output);
//    connect(m_terminal_interactive, &TerminalInteractive::signal_ready_read_standard_error,
//            this, &TerminateWorker::slot_read_error);
  connect(m_terminal_interactive, &TerminalInteractive::signal_finished,
          this, &TerminateWorker::signal_finished);
  connect(m_terminal_interactive, &TerminalInteractive::signal_output_coming,
          this, &TerminateWorker::signal_output_coming);
  connect(m_terminal_interactive, &TerminalInteractive::signal_error_coming,
          this, &TerminateWorker::signal_error_coming);
}

auto TerminateWorker::run_command(const QString &command, bool wait_for_finished) -> void
{
  m_terminal_interactive->run_command(command, wait_for_finished);
}

auto TerminateWorker::read_output()->QString
{
  return m_output_buffer;
}

auto TerminateWorker::read_error()-> QString
{
  return m_error_buffer;
}
