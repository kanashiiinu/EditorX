#ifndef FILECALLTOMD5_H
#define FILECALLTOMD5_H
#include <QObject>

class FileMd5Worker : public QObject
{
  Q_OBJECT
 public:
  explicit FileMd5Worker(QObject *parent = nullptr);
  ~FileMd5Worker() = default;

  QString open_directory();
  static QString get_file_md5(const QString &path);
  QStringList getFiles(const QString &path);
 signals:
  void signalGetFileMd5();
};

#endif // FILECALLTOMD5_H
