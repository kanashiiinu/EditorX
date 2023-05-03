#ifndef TERMINATEWORKER_HPP
#define TERMINATEWORKER_HPP

#include "TerminalInteractive.hpp"
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
class TerminateWorker : public QObject
{
  Q_OBJECT
 public:
  TerminateWorker(QObject *parent = nullptr);

  virtual ~TerminateWorker() = default;
 public:
 signals:
  void signal_ready_read_standard_output();
  void signal_ready_read_standard_error();
  void signal_finished();
  void signal_output_coming(QString);
  void signal_error_coming(QString);

 private slots:
  void slot_read_output();

  void slot_read_error();

  //
 private:
  TerminalInteractive *m_terminal_interactive = nullptr;
 public:
  auto start_a_command(const QString &command_name)->void;

  auto run_command(const QString &command, bool wait_for_finished = false) -> void;

  auto read_output()->QString;

  auto read_error()-> QString;

 private:
  QString m_output_buffer;
  QString m_error_buffer;
};
#endif // TERMINATEWORKER_HPP
