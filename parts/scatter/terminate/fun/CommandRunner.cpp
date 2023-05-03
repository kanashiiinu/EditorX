#include "CommandRunner.hpp"
#include <QTextCodec>
#include <cstring>

CommandRunner::CommandRunner(QObject *parent)
  : QObject(parent)
  , m_terminate_thread(nullptr)
{
  m_terminate_thread = new ThreadTerminate(this);
}

auto CommandRunner::start_a_command(const QString &command_name)->void
{
  //command_name = "cmd"; // windows
  m_terminate_thread->start_a_command(command_name);
//    connect(m_terminate_thread, &ThreadTerminate::signal_ready_read_standard_output,
//            this, &CommandRunner::slot_read_output);
//    connect(m_terminate_thread, &ThreadTerminate::signal_ready_read_standard_error,
//            this, &CommandRunner::slot_read_error);
  connect(m_terminate_thread, &ThreadTerminate::signal_finished,
          this, &CommandRunner::signal_finished);
  connect(m_terminate_thread, &ThreadTerminate::signal_output_coming,
          this, &CommandRunner::signal_output_coming);
  connect(m_terminate_thread, &ThreadTerminate::signal_error_coming,
          this, &CommandRunner::signal_error_coming);
  qDebug() << "c r";
}

auto CommandRunner::run_command(const QString &command, bool wait_for_finished) -> void
{
  m_terminate_thread->run_command(command, wait_for_finished);
}


/*slots*/
void CommandRunner::slot_read_output()
{
  const QString &output =  (m_terminate_thread->read_output());
  emit this->signal_output_coming(output);
}

void CommandRunner::slot_read_error()
{
  const auto &error =  (m_terminate_thread->read_error());
  emit this->signal_error_coming(error);
}


QString CommandRunner::gb2312_to_utf8(QString data)
{
  QTextCodec *utf8Codec = QTextCodec::codecForName("utf-8");
  QTextCodec *gb2312Codec = QTextCodec::codecForName("gb2312");

  QString strUnicode = gb2312Codec ->toUnicode(data.toLocal8Bit());    //无编码
  QByteArray ByteUTF8 = utf8Codec ->fromUnicode(strUnicode);     //无编码转换

  return QString::fromLocal8Bit(ByteUTF8);
}

QString CommandRunner::utf8_to_gb2312(QString data)
{
  QTextCodec *utf8Codec = QTextCodec::codecForName("utf-8");
  QTextCodec *gb2312Codec = QTextCodec::codecForName("gb2312");

  QString strUnicode = utf8Codec->toUnicode(data.toLocal8Bit());    //无编码
  QByteArray ByteGb2312 = gb2312Codec->fromUnicode(strUnicode);     //无编码转换

  char *pTmp = new char[ByteGb2312.length() + 1];
  memset(pTmp, 0, ByteGb2312.length() + 1);
  strncpy(pTmp, ByteGb2312.data(), ByteGb2312.length());
  delete pTmp;
  return QString::fromLocal8Bit(ByteGb2312);
}

QString CommandRunner::gbk_to_utf8(QByteArray &inStr)
{
  QTextCodec *gbk = QTextCodec::codecForName("gbk");
  QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

  char *p = inStr.data();
  QString str = gbk->toUnicode(p);

  QByteArray utf8_bytes = utf8->fromUnicode(str);
  p = utf8_bytes.data();
  str = p;

  return str;

}

QString CommandRunner::utf8_to_gbk(QByteArray &inStr)
{
  QTextCodec *gbk = QTextCodec::codecForName("gbk");
  QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

  char *p = inStr.data();
  QString str = utf8->toUnicode(p);

  QByteArray utf8_bytes = gbk->fromUnicode(str);
  p = utf8_bytes.data();
  str = p;

  return str;
}
