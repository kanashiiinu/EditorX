#include "CompareFileWidget.hpp"
#include "./parts/left_tools/tool_parts/compare_file_in_dir/fun/FileMd5Worker.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QFileDialog>
#include <QListWidget>
#include <QDebug>
#include "./tools/tools.hpp"

auto get_file_name(QString const &file_path) {
  return tool::sub_by_index(file_path, file_path.lastIndexOf("/") + 1, file_path.size());
}

CompareFileWidget::CompareFileWidget(QWidget *parent)
  : QWidget(parent)
{

  this->_init_ui();
  this->_init_text();
  // connect
  connect(m_left_btn, &QPushButton::clicked,
          this, &CompareFileWidget::slot_left_btn_clicked);
  connect(m_right_btn, &QPushButton::clicked,
          this, &CompareFileWidget::slot_right_btn_clicked);
  connect(m_compare_btn, &QPushButton::clicked,
          this, &CompareFileWidget::slot_compare_btn_clicked);
}

CompareFileWidget::~CompareFileWidget()
{
}



QStringList s_get_files(const QString &path)
{
  if (path.isEmpty())
    return QStringList();

  QStringList fileListRet, fileNames;
  QDir dir(path);
  QFileInfoList infoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
  for (int i = 0; i < infoList.size(); i++)
  {
    QFileInfo info = infoList[i];
    if (info.isDir()) {
      QString dir = info.absoluteFilePath();
      fileNames = s_get_files(dir);
      fileListRet.append(fileNames);
    } else {
      QString fileName = info.absoluteFilePath();
      fileListRet.append(fileName);
    }
  }
  return fileListRet;
}


void CompareFileWidget::_get_files_info(QMap<QString, QString> &mp, QStringList &names)
{
  if (m_path.isEmpty())
  {
    return;
  }
  names.clear();
  QStringList fileNames = s_get_files(m_path);
  auto file_name = QString();
  for (const auto &fileNamePath : fileNames)
  {
    QString md5 = FileMd5Worker::get_file_md5(fileNamePath);
    file_name = get_file_name(fileNamePath);
//    qDebug() << file_name;
    mp[file_name] = md5;
    names.append(fileNamePath);
  }
}

/*
    打开文件1
*/
void CompareFileWidget::slot_left_btn_clicked()
{
  m_left_output->clear();
  m_compareNames.clear();
  QString path = QFileDialog::getExistingDirectory(nullptr, "打开文件夹", "*");
  m_path = path;
  this->_get_files_info(m_map1, m_fileNames1);
  //数据展示到界面
  m_left_input->setText(path);
  m_left_output->addItems(m_fileNames1);
}

/*
    打开文件2
*/
void CompareFileWidget::slot_right_btn_clicked()
{
  m_right_output->clear();
  m_compareNames.clear();
  QString path = QFileDialog::getExistingDirectory(nullptr, "打开文件夹", "*");
  m_path = path;
  this->_get_files_info(m_map2, m_fileNames2);
  m_right_input->setText(path);
  m_right_output->addItems(m_fileNames2);
}

/*
    比较文件
*/
void CompareFileWidget::slot_compare_btn_clicked()
{
  for (auto iter1 = m_map1.begin(); iter1 != m_map1.end(); ++iter1)
  {
    const auto &file_1_name = iter1.key();
    const auto &file_1_md5 = iter1.value();
    for (auto iter2 = m_map2.begin(); iter2 != m_map2.end(); ++iter2)
    {
      const auto &file_2_name = iter2.key();
      const auto &file_2_md5 = iter2.value();
      qDebug() << file_1_name;
      qDebug() << file_2_name;

      if (file_1_md5 == file_2_md5 and file_1_name == file_2_name)
      {
        m_compareNames.append(file_1_name);
        break;
      }
    }
  }
  qDebug() << m_compareNames;

  for (int i = 0; i < m_right_output->count(); i++)
  {
    m_right_output->item(i)->setTextColor(QColor("green"));
    if (not m_compareNames.contains(get_file_name(m_right_output->item(i)->text())))
    {
      m_right_output->item(i)->setTextColor(QColor("red"));
    }
  }
}


void CompareFileWidget::_init_ui()
{
  auto total_layout = new QVBoxLayout();
  this->setLayout(total_layout);
  auto left_right_layout = new QHBoxLayout();
  total_layout->addLayout(left_right_layout);
  // left
  auto left_layout = new QVBoxLayout();
  left_right_layout->addLayout(left_layout);
  auto left_input_layout = new QHBoxLayout();
  left_layout->addLayout(left_input_layout);
  m_left_input = new QLineEdit(this);
  left_input_layout->addWidget(m_left_input);
  m_left_btn = new QPushButton(this);
  left_input_layout->addWidget(m_left_btn);
  m_left_output = new QListWidget(this);
  left_layout->addWidget(m_left_output);

  // right
  auto right_layout = new QVBoxLayout();
  left_right_layout->addLayout(right_layout);
  auto right_input_layout = new QHBoxLayout();
  right_layout->addLayout(right_input_layout);
  m_right_input = new QLineEdit(this);
  right_input_layout->addWidget(m_right_input);
  m_right_btn = new QPushButton(this);
  right_input_layout->addWidget(m_right_btn);
  m_right_output = new QListWidget(this);
  right_layout->addWidget(m_right_output);
  //
  m_compare_btn = new QPushButton(this);
  total_layout->addWidget(m_compare_btn);
}

void CompareFileWidget::_init_text()
{
  m_left_btn->setText("src");
  m_right_btn->setText("des");
  m_compare_btn->setText("compare");
}
