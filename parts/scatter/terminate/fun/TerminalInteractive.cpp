#include "TerminalInteractive.hpp"
QProcess *TerminalInteractive::cmd = nullptr;

TerminalInteractive::TerminalInteractive(const QString &command_name, QObject *parent)
  : QObject(parent)
  , m_command_name(command_name)
{
  /*TODO*/
  m_output_buffer = QString();
  m_error_buffer = QString();
  this->init(m_command_name);
}

void TerminalInteractive::init(const QString &command_name)
{
  /*TODO*/
  if (command_name.isEmpty())
  {
    return;
  }
  cmd = new QProcess(this);
//    cmd->start(command_name);
//    cmd->execute("cmd", QStringList() << "dir" << "\n");
  qDebug() << command_name;
  cmd->start("cmd");
  if (cmd->waitForStarted()) {
    qDebug() << "ww";
  }
  qDebug() << "ee";
  // 与QProcess之间接收信息的联接
  connect(cmd, &QProcess::readyReadStandardOutput,
          this, &TerminalInteractive::slot_read_output);
  connect(cmd, &QProcess::readyReadStandardError,
          this, &TerminalInteractive::slot_read_error);
  {
    using finishede_function_type = void (QProcess::*)(int);
    connect(cmd, (finishede_function_type)(&QProcess::finished),
            this, &TerminalInteractive::signal_finished);
  }
}


void TerminalInteractive::slot_read_output()
{
//    g_mutex.lock();
  auto byte_arr = cmd->readAllStandardOutput();
  QString output = QTextCodec::codecForName("System")->toUnicode(byte_arr);
  m_output_buffer = output;
//    emit this->signal_ready_read_standard_output();
  emit this->signal_output_coming(m_output_buffer);
//    g_mutex.unlock();
}

void TerminalInteractive::slot_read_error()
{
//    g_mutex.lock();
  auto byte_arr = cmd->readAllStandardError();
  QString error = QTextCodec::codecForName("System")->toUnicode(byte_arr);
  m_error_buffer = error;
//    emit this->signal_ready_read_standard_error();
  emit this->signal_error_coming(m_error_buffer);
//    g_mutex.unlock();
}


/**
 * @brief run_command
 * @param command
 * @param wait_for_finished, true has bug
 */
auto TerminalInteractive::run_command(const QString &command, bool wait_for_finished) -> void
{
  auto c = (command + "\n").toUtf8();
  cmd->write((command + "\n").toUtf8());
  qDebug() << "cc1";
  if (wait_for_finished)
  {
    cmd->waitForFinished();
  }
}

auto TerminalInteractive::read_output()-> QString
{
  g_mutex.lock();
  return m_output_buffer;
  g_mutex.unlock();
}

auto TerminalInteractive::read_error()-> QString
{
  g_mutex.lock();
  return m_error_buffer;
  g_mutex.unlock();
}

