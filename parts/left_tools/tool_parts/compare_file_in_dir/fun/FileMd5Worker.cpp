#include "FileMd5Worker.hpp"
#include <QFile>
#include <QCryptographicHash>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QByteArray>

FileMd5Worker::FileMd5Worker(QObject *parent)
  : QObject(parent)
{
}

QString FileMd5Worker::open_directory()
{
  return QFileDialog::getExistingDirectory(nullptr, "打开文件夹", "*");
}

QStringList FileMd5Worker::getFiles(const QString &path)
{
  if (path.isEmpty())
  {
    return QStringList();
  }
  QStringList file_list;
  QStringList file_names;
  QDir dir(path);
  auto info_list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
  for (auto info : info_list)
  {
    if (info.isDir())
    {
      auto dir = info.absoluteFilePath();
      file_names = getFiles(dir);
      file_list.append(file_names);
    }
    else
    {
      auto fileName = info.absoluteFilePath();
      file_list.append(fileName);
    }
  }
  return file_list;;
}

QString FileMd5Worker::get_file_md5(const QString &path)
{
  if (path.isEmpty())
  {
    return QString();
  }

  QCryptographicHash md5Hash(QCryptographicHash::Md5);
  QFile file(path);
  if (file.open(QIODevice::ReadWrite))
  {
    while (!file.atEnd())
    {
      QString data = file.read(1024 * 1024);
      QByteArray curData = data.toLatin1();
      md5Hash.addData(curData, curData.size());
      qApp->processEvents();
//      qDebug() << md5Hash.result().toHex();
    }
  }
  file.close();
  return md5Hash.result();
}
