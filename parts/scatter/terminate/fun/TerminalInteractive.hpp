#ifndef TERMINALINTERACTIVE_HPP
#define TERMINALINTERACTIVE_HPP

#include <QLineEdit>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextCursor>
#include <QScrollBar>
#include <QString>
#include <QTextCodec>
#include <QDebug>
#include <memory>

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QMutex>

static QMutex g_mutex;

class TerminalInteractive : public QObject
{
  Q_OBJECT
 public:
  TerminalInteractive(const QString &command_name, QObject *parent = nullptr);

  virtual ~TerminalInteractive() = default;

 private:
  void init(const QString &command_name = "");

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


 private:
  QString m_command_name;
  static QProcess *cmd;
  // >>
 public:
  /**
   * @brief run_command
   * @param command
   * @param wait_for_finished, true has bug
   */
  auto run_command(const QString &command, bool wait_for_finished = false) -> void;

  auto read_output()-> QString;

  auto read_error()-> QString;

 private:
  QString m_output_buffer;
  QString m_error_buffer;
};


#endif // TERMINALINTERACTIVE_HPP
