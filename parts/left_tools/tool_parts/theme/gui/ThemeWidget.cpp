#include "ThemeWidget.hpp"
#include "./parts/left_tools/tool_parts/theme/fun/ThemeOperation.hpp"
#include "./parts/left_tools/tool_parts/theme/fun/ThemesManager.hpp"
// Qt
#include <QDirIterator>
#include <QFileDialog>
#include <QVariant>
#include <QDebug>
#include <QApplication>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QListWidget>


ThemeWidget::ThemeWidget(QWidget *parent)
  : QWidget(parent)
{
  this->_init_ui();


  m_theme_manager = new ThemesManager(this);
  m_theme_list->addItems(m_theme_manager->get_theme_names());


  connect(m_add_path_btn, &QPushButton::clicked, this, [ = ]() {
    QString dir = QFileDialog::getExistingDirectory(nullptr, "select directory", "", QFileDialog::ShowDirsOnly);
    if (not dir.isEmpty()) {
      qDebug() << "选择的目录：" << dir;
      QString dirPath = dir;
      QDirIterator it(dirPath, QStringList() << "*.css" << "*.qss", QDir::Files, QDirIterator::Subdirectories);
      while (it.hasNext()) {
        it.next();
        QString file_path = it.filePath();
        auto file_info = QFileInfo(file_path);
        QString file_base_name = file_info.baseName();
        if ( m_theme_manager->add_theme(file_base_name, file_path))
        {
          m_theme_list->addItem(file_base_name);
        }
      }
    } else {
      qDebug() << "未选择任何目录";
    }
  });

  connect(m_remove_path_btn, &QPushButton::clicked, this, [ = ]() {
    QString dir = QFileDialog::getExistingDirectory(nullptr, "select directory", "", QFileDialog::ShowDirsOnly);
    if (not dir.isEmpty()) {
      qDebug() << "选择的目录：" << dir;
      QString dirPath = dir;
      QDirIterator it(dirPath, QStringList() << "*.css" << "*.qss", QDir::Files, QDirIterator::Subdirectories);
      while (it.hasNext()) {
        it.next();
        QString file_path = it.filePath();
        auto file_info = QFileInfo(file_path);
        QString file_base_name = file_info.baseName();
        if (m_theme_manager->remove_theme_by_path(file_path))
        {
          m_theme_list->clear();
          m_theme_list->addItems(m_theme_manager->get_theme_names());
        }
      }
    } else {
      qDebug() << "未选择任何目录";
    }
  });

  connect(m_apply_btn, &QPushButton::clicked, this, [ = ]() {
    // 创建一个 QFile 对象，并传入 QSS 文件的路径
    auto name = m_theme_list->currentItem()->data(Qt::DisplayRole).toString();
    QFile file(m_theme_manager->get_path(name));
    // 检查文件是否成功打开
    if (not file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return;
    }
    // 创建一个 QTextStream 对象，并将其与 QFile 关联
    QTextStream stream(&file);
    // 读取文件内容
    QString qss = stream.readAll();

    // 关闭文件
    file.close();
    qApp->setStyleSheet(qss);
  });

}

void ThemeWidget::_init_ui()
{

  auto total_layout = new QVBoxLayout();
  this->setLayout(total_layout);
  //
  m_theme_list = new QListWidget(this);
  total_layout->addWidget(m_theme_list);

  auto buttons_layout = new QHBoxLayout();
  total_layout->addLayout(buttons_layout);

  m_add_path_btn = new QPushButton(this);
  m_remove_path_btn = new QPushButton(this);
  m_cancel_btn = new QPushButton(this);
  m_apply_btn = new QPushButton(this);

  buttons_layout->addWidget(m_add_path_btn);
  buttons_layout->addWidget(m_remove_path_btn);
  buttons_layout->addWidget(m_cancel_btn);
  buttons_layout->addWidget(m_apply_btn);
}

void ThemeWidget::_init_text()
{
  m_add_path_btn->setText("add path");
  m_remove_path_btn->setText("remove path");
  m_cancel_btn->setText("cancel");
  m_apply_btn->setText("apply");
}
