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

CompareFileWidget::CompareFileWidget(QWidget *parent)
  : QWidget(parent)
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
  left_input = new QLineEdit(this);
  left_input_layout->addWidget(left_input);
  left_btn = new QPushButton(this);
  left_input_layout->addWidget(left_btn);
  left_output = new QListWidget(this);
  left_layout->addWidget(left_output);

  // right
  auto right_layout = new QVBoxLayout();
  left_right_layout->addLayout(right_layout);
  auto right_input_layout = new QHBoxLayout();
  right_layout->addLayout(right_input_layout);
  right_input = new QLineEdit(this);
  right_input_layout->addWidget(right_input);
  right_btn = new QPushButton(this);
  right_input_layout->addWidget(right_btn);
  right_output = new QListWidget(this);
  right_layout->addWidget(right_output);
  //
  compare_btn = new QPushButton("compare", this);
  total_layout->addWidget(compare_btn);

  // connect
  connect(left_btn, &QPushButton::clicked,
          this, &CompareFileWidget::slot_left_btn_clicked);
  connect(right_btn, &QPushButton::clicked,
          this, &CompareFileWidget::slot_right_btn_clicked);
  connect(compare_btn, &QPushButton::clicked,
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
  for (const auto &fileNamePath : fileNames)
  {
    QString md5 = FileMd5Worker::get_file_md5(fileNamePath);
    mp[fileNamePath] = md5;
    names.append(fileNamePath);
  }
}

/*
    打开文件1
*/
void CompareFileWidget::slot_left_btn_clicked()
{
  left_output->clear();
  m_compareNames.clear();
  QString path = QFileDialog::getExistingDirectory(nullptr, "打开文件夹", "*");
  m_path = path;
  this->_get_files_info(m_map1, m_fileNames1);
  //数据展示到界面
  left_input->setText(path);
  left_output->addItems(m_fileNames1);
}

/*
    打开文件2
*/
void CompareFileWidget::slot_right_btn_clicked()
{
  right_output->clear();
  m_compareNames.clear();
  QString path = QFileDialog::getExistingDirectory(nullptr, "打开文件夹", "*");
  m_path = path;
  _get_files_info(m_map2, m_fileNames2);
  right_input->setText(path);
  right_output->addItems(m_fileNames2);
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
      if (file_1_md5 == file_2_md5 and file_1_name == file_2_name)
      {
        m_compareNames.append(file_1_name);
        break;
      }
    }
  }

  for (int i = 0; i < right_output->count(); i++)
  {
    right_output->item(i)->setTextColor(QColor("green"));
    if (not m_compareNames.contains(right_output->item(i)->text()))
    {
      right_output->item(i)->setTextColor(QColor("red"));
    }
  }
}


