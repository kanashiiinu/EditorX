#ifndef THREADTERMINATE_HPP
#define THREADTERMINATE_HPP
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
class ThreadTerminate : public QObject
{
  Q_OBJECT
  QThread workerThread;
 public:
  ThreadTerminate(QObject *parent = nullptr);

  virtual ~ThreadTerminate();

  // >>
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

 public:
  auto read_output()-> QString;

  auto read_error()-> QString;

 private:
  TerminateWorker *m_worker = nullptr;
  QString m_output_buffer;
  QString m_error_buffer;
 public:
  auto start_a_command(const QString &command_name)->void;

 public:
 signals:
  void signal_start(const QString &command_name);

 public:
  auto run_command(const QString &command, bool wait_for_finished = false) -> void;

 public:
 signals:
  void signal_run(const QString &command, bool wait_for_finished);

};
#endif // THREADTERMINATE_HPP
