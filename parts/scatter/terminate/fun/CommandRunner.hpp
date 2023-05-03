#ifndef COMMANDRUNNER_HPP
#define COMMANDRUNNER_HPP
#include "ThreadTerminate.hpp"

class CommandRunner : public QObject
{
  Q_OBJECT
 public:
 signals:
  void signal_output_coming(QString);
  void signal_error_coming(QString);
  void signal_finished();
 public:
  CommandRunner(QObject *parent = nullptr);

  /**
   * @brief start_a_command
   * @param command_name
   * @note 整套流程建立在start a command正确，否则Undifine Behavor
   */
  auto start_a_command(const QString &command_name)->void;

  auto run_command(const QString &command, bool wait_for_finished = false) -> void ;

 private:
  /*slots*/
  void slot_read_output();

  void slot_read_error();

  virtual ~CommandRunner() = default;

  QString gb2312_to_utf8(QString data);

  QString utf8_to_gb2312(QString data);

  QString gbk_to_utf8(QByteArray &inStr);

  QString utf8_to_gbk(QByteArray &inStr);

 private:
  ThreadTerminate *m_terminate_thread;
};
#endif // COMMANDRUNNER_HPP
