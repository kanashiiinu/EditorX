#include "CodeFormatWidget.hpp"


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QProcess>
#include <QVariant>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QDir>
#include <QDebug>
#include <QFileDialog>

constexpr auto format_files_path = "../X/format/";

static auto s_g_default_clang_format_style_name = QStringList
{
  "LLVM",
  "Google",
  "Chromium",
  "Mozilla",
  "WebKit",
  "Microsoft",
  "GNU",
};

CodeFormatWidget::CodeFormatWidget(QWidget *parent)
  : QWidget(parent)
{
  this->_init_ui();
  this->_init_text();
  QString directoryPath = format_files_path;
  QDir directory(directoryPath);
  if (directory.exists()) {
    QStringList name_file;
    //需要过滤的文件的格式，这边只筛选txt文件
    name_file << "*.clang_format";
    QFileInfoList fileInfoList = directory.entryInfoList(name_file, QDir::Files | QDir::NoDotAndDotDot);
    for (auto const &file_info : fileInfoList)
    {
      auto file_name = file_info.fileName();
      auto file_path = file_info.filePath();
      m_names_to_paths[file_name] = file_path;
    }
    this->note_list->addItems(m_names_to_paths.keys());
  } else {
    qDebug() << "Directory does not exist!";
  }

  connect(filter_btn, &QPushButton::clicked, [this]() {
    auto word = filter_input->text();
    auto file_names = m_names_to_paths.keys();
    auto rest_names = QStringList();
    for (auto const &file_name : file_names)
    {
      if (file_name.contains(word))
      {
        rest_names.append(file_name);
      }
    }
    note_list->clear();
    note_list->addItems(rest_names);
  });

  connect(m_add_file_btn, &QPushButton::clicked, [this]() {
    QFileDialog dialog;
    dialog.setNameFilter("clang format files(*.clang-format)");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.exec();
    QStringList selected_file_paths = dialog.selectedFiles();

    for (auto const &file_path : selected_file_paths)
    {
      QFileInfo file_info(file_path);
      auto file_name = file_info.fileName();

      QString sourceFilePath = file_path;
      QString destinationFilePath = format_files_path + file_name;
      qDebug() << destinationFilePath;
      QFile::copy(sourceFilePath, destinationFilePath);
    }
    QString directoryPath = format_files_path;
    QDir directory(directoryPath);
    if (directory.exists()) {
      QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
      for (auto const &file_info : fileInfoList)
      {
        auto file_name = file_info.fileName();
        auto file_path = file_info.filePath();
        m_names_to_paths[file_name] = file_path;
      }
      this->note_list->addItems(m_names_to_paths.keys());
    } else {
      qDebug() << "Directory does not exist!";
    }
    note_list->clear();
    note_list->addItems(m_names_to_paths.keys());
  });

  connect(m_remove_file_btn, &QPushButton::clicked, [this]() {
    auto seleced_file = note_list->currentItem()->data(Qt::DisplayRole).toString();
    qDebug() << seleced_file;
    auto seleced_file_path = m_names_to_paths[seleced_file];
    if (not remove_file(seleced_file_path))
    {
      qDebug() << "ee";
      return;
    }
    m_names_to_paths.remove(seleced_file);
    note_list->clear();
    note_list->addItems(m_names_to_paths.keys());
  });

  connect(note_list, &QListWidget::itemDoubleClicked, [this](QListWidgetItem * item) {
    auto file_name = item->text();
    auto file_path = m_names_to_paths[file_name];
//    emit this->signal_want_open_clang_format_file(file_path)
    qDebug() << file_path;
  });


  connect(m_apply_btn, &QPushButton::clicked,
  [this]() {
    m_current_style_name = note_list->currentItem()->text();
  });

  connect(m_defaul_style_box, &QComboBox::currentTextChanged,
  [this](auto const & style_name) {
    m_defaul_style_name = style_name;
  });

  connect(m_format_btn, &QPushButton::clicked,
  [this]() {
    emit this->signal_format_wait_for_file();
  });
}

void CodeFormatWidget::_init_ui()
{
  //
  auto total_layout = new QVBoxLayout();
  this->setLayout(total_layout);
  //
  auto filter_layout = new QHBoxLayout();
  total_layout->addLayout(filter_layout);
  //
  filter_input = new QLineEdit(this);
  filter_btn = new QPushButton(this);
  filter_layout->addWidget(filter_input);
  filter_layout->addWidget(filter_btn);
  //
  note_list = new QListWidget(this);
  total_layout->addWidget(note_list);
  //
  auto btn_layout = new QHBoxLayout();
  total_layout->addLayout(btn_layout);
  //
  m_add_file_btn = new QPushButton(this);
  m_remove_file_btn = new QPushButton(this);
  m_apply_btn = new QPushButton(this);
  m_defaul_style_box = new QComboBox(this);
  m_format_btn = new QPushButton(this);
  btn_layout->addWidget(m_add_file_btn);
  btn_layout->addWidget(m_remove_file_btn);
  btn_layout->addWidget(m_apply_btn);
  btn_layout->addWidget(m_defaul_style_box);
  btn_layout->addWidget(m_format_btn);
}

bool CodeFormatWidget::remove_file(QString const &path)
{
  QFile file(path);
  if (file.remove()) {
    return true;
  }
  return false;
}

void CodeFormatWidget::_init_text()
{
  filter_btn->setText("filter");
  m_add_file_btn->setText("add");
  m_remove_file_btn->setText("remove");
  m_apply_btn->setText("apply");
  m_format_btn->setText("format");
  //
  m_defaul_style_box->addItems(get_default_clang_format() << "Custom");
}

QStringList CodeFormatWidget::get_default_clang_format()
{
  return s_g_default_clang_format_style_name;
}


void CodeFormatWidget::slot_deal_format(QString const &file_path)
{
  if (file_path.isEmpty())
  {
    return;
  }

  QProcess process;
  process.start("clang-format", this->_command_params(file_path));
  process.waitForFinished(-1);

  QByteArray output = process.readAllStandardOutput();
  QString outputString = QString::fromUtf8(output);
  qDebug() << "Output:" << outputString;
  emit this->signal_format_finish();
}

QStringList CodeFormatWidget::_command_params(QString const &file_path)
{
  auto params = QStringList();
  auto style_param = QString("-style=");
  auto write_into_file_param = QString("-i");
  auto file_path_param = file_path;
  if (m_defaul_style_name == "Custom")
  {
    style_param.append("file:" + m_names_to_paths[m_current_style_name]);
  }
  else
  {
    style_param.append(m_defaul_style_name);
  }
  return params << style_param << write_into_file_param << file_path_param;
}
