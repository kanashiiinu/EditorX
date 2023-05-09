#include "ThemeOperation.hpp"

// Qt
#include <QFileInfo>
#include <QString>
// std
#include <algorithm>

namespace theme {

QStringList get_project_default_theme_path()
{
  QStringList qss_s = QStringList{":/qss/blacksoft.css", ":/qss/flatgray.css", ":/qss/lightblue.css"};
  return qss_s;
}

QString from_url_to_name(QString const &path)
{
  // 创建 QFileInfo 对象，并传入路径
  QFileInfo file_info(path);
  // 获取文件名（去除后缀）
  auto file_name = file_info.baseName();
  return file_name;
}

}
