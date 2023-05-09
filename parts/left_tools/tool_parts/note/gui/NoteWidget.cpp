#include "NoteWidget.hpp"


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QVariant>
#include <QListWidget>
#include <QPushButton>
#include <QDir>
#include <QDebug>
#include <QFileDialog>

constexpr auto note_path = "../X/notes/";

NoteWidget::NoteWidget(QWidget *parent)
  : QWidget(parent)
{
  this->_init_ui();
  this->_init_text();
  QString directoryPath = note_path;
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

  connect(add_file_btn, &QPushButton::clicked, [this]() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.exec();
    QStringList selected_file_paths = dialog.selectedFiles();

    for (auto const &file_path : selected_file_paths)
    {
      QFileInfo file_info(file_path);
      auto file_name = file_info.fileName();

      QString sourceFilePath = file_path;
      QString destinationFilePath = note_path + file_name;
      qDebug() << destinationFilePath;
      QFile::copy(sourceFilePath, destinationFilePath);
    }
    QString directoryPath = note_path;
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

  connect(remove_file_btn, &QPushButton::clicked, [this]() {
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
    emit this->signal_want_open_note(file_path);
    qDebug() << file_path;
  });

}

void NoteWidget::_init_ui()
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
  add_file_btn = new QPushButton(this);
  remove_file_btn = new QPushButton(this);
  btn_layout->addWidget(add_file_btn);
  btn_layout->addWidget(remove_file_btn);
}

bool NoteWidget::remove_file(QString const &path)
{
  QFile file(path);
  if (file.remove()) {
    return true;
  }
  return false;
}

void NoteWidget::_init_text()
{
  filter_btn->setText("filter");
  add_file_btn->setText("add");
  remove_file_btn->setText("remove");
}
