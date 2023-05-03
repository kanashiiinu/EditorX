#include "ThreadTerminate.hpp"

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

ThreadTerminate::ThreadTerminate(QObject *parent)
  : QObject(parent)
{
//    m_worker = new TerminateWorker(this);
  m_worker = new TerminateWorker();
  m_worker->moveToThread(&workerThread);
  connect(&workerThread, &QThread::finished,
          m_worker, &QObject::deleteLater);
  // 发送命令
  connect(this, &ThreadTerminate::signal_start,
          m_worker, &TerminateWorker::start_a_command);
  connect(this, &ThreadTerminate::signal_run,
          m_worker, &TerminateWorker::run_command);
  // 接收信息
//    connect(m_worker, &TerminateWorker::signal_ready_read_standard_output,
//            this, &ThreadTerminate::slot_read_output);
//    connect(m_worker, &TerminateWorker::signal_ready_read_standard_error,
//            this, &ThreadTerminate::slot_read_error);
  connect(m_worker, &TerminateWorker::signal_finished,
          this, &ThreadTerminate::signal_finished);
  connect(m_worker, &TerminateWorker::signal_output_coming,
          this, &ThreadTerminate::signal_output_coming);
  connect(m_worker, &TerminateWorker::signal_error_coming,
          this, &ThreadTerminate::signal_error_coming);
  //
  workerThread.start();
}

ThreadTerminate::~ThreadTerminate()
{
  workerThread.quit();
  workerThread.wait();
}

// >>
void ThreadTerminate::slot_read_output()
{
  m_output_buffer = m_worker->read_output();
  emit this->signal_ready_read_standard_output();
}

void ThreadTerminate::slot_read_error()
{
  m_error_buffer = m_worker->read_error();
  emit this->signal_ready_read_standard_error();
}

auto ThreadTerminate::read_output()-> QString
{
  return m_output_buffer;
}

auto ThreadTerminate::read_error()-> QString
{
  return m_error_buffer;
}


auto ThreadTerminate::start_a_command(const QString &command_name)->void
{
  emit this->signal_start(command_name);
}

auto ThreadTerminate::run_command(const QString &command, bool wait_for_finished) -> void
{
  emit this->signal_run(command, wait_for_finished);
}
