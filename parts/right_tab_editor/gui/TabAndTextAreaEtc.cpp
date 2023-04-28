#include "TabAndTextAreaEtc.hpp"
//Qt
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QMetaEnum>

TabAndTextAreaEtc::TabAndTextAreaEtc(QWidget *parent) : QWidget(parent)
{
  //
  auto h_box = new QHBoxLayout(this);
  h_box->setContentsMargins(0, 0, 0, 0);
  this->setLayout(h_box);
  m_tab_widget = new QTabWidget(this);
//
  m_tab_widget->setTabsClosable(true);
  m_tab_widget->setMovable(true);
  h_box->addWidget(m_tab_widget);
  connect(m_tab_widget->tabBar(), &QTabBar::tabCloseRequested, this, &TabAndTextAreaEtc::close_tab);
}

auto TabAndTextAreaEtc::add_new_file()->void {
  auto untitled_file_num = m_tab_widget->count() - m_opened_files.size(); // assert(untitled_file_num >= 0)
  auto new_tab_name = QString("untitled%1");
  switch (untitled_file_num) {
    case 0: new_tab_name = "untitled"; break;
    default: new_tab_name = new_tab_name.arg("-" + QString::number(untitled_file_num)); break;
  }
  auto code_edit_area = new CodeEditArea(this);
  code_edit_area->m_file_name = new_tab_name;
  m_tab_widget->addTab(code_edit_area, new_tab_name);
}

auto TabAndTextAreaEtc::open_file_by_path(const QString &file_path)-> void {
  //[pre = file_path is legal]
  //assert:=file_path is legal

  QFile file(file_path);
  // 如果有直接调整位置
  if (m_opened_files.contains(file_path)) {
    m_tab_widget->setCurrentIndex(m_tab_widget->indexOf(m_opened_files.value(file_path)));
    return;
  }
  // 否则再打开一个
  file.open(QFile::ReadOnly | QFile::Text);
  this->add_new_file();
  auto new_file_index = m_tab_widget->count() - 1;
  auto code_edit_area = qobject_cast<CodeEditArea *>(m_tab_widget->widget(new_file_index));
//  connect(code_edit_area->document(), &QTextDocument::contentsChanged, this, [this]() {m_any_file_is_modified = true;});

  code_edit_area->setPlainText(QString::fromUtf8(file.readAll()));
  QFileInfo file_info(file_path);
  code_edit_area->m_file_path = file_info.absoluteFilePath();
  code_edit_area->m_file_name = file_info.fileName();
  m_tab_widget->setTabText(new_file_index, code_edit_area->m_file_name);
  m_tab_widget->setCurrentIndex(new_file_index);
  this->add_opened_file_into_data(file_path, code_edit_area);
}

auto TabAndTextAreaEtc::add_opened_file_into_data(const QString &file_path, CodeEditArea *opened_file)-> void {
  m_opened_files[file_path] = opened_file;
}

auto TabAndTextAreaEtc::remove_opened_file_from_data(const QString &file_path)-> void {
  m_opened_files.remove(file_path);
}


auto TabAndTextAreaEtc::get_all_code_of_current_editor() -> QString {
  return this->current_code_edit_area()->toPlainText();
}

auto TabAndTextAreaEtc::current_code_edit_area()->CodeEditArea* {
  return qobject_cast<CodeEditArea *>(m_tab_widget->widget(m_tab_widget->currentIndex()));
}

auto TabAndTextAreaEtc::close_tab(int index)->void {
  auto file_path = qobject_cast<CodeEditArea *>(m_tab_widget->widget(index))->m_file_path;
  if (not file_path.isEmpty()) {
    this->remove_opened_file_from_data(file_path);
  }
  m_tab_widget->removeTab(index);
}

auto TabAndTextAreaEtc::operate_current_area(const QString &command)->void
{
  QMetaEnum metaColor = QMetaEnum::fromType<TabAndTextAreaEtc::TextOperation>();
  auto c = metaColor.keyToValue(command.toStdString().c_str());
  switch (c)
  {
    break; case Undo: {this->current_code_edit_area()->undo();}
    break; case Redo: {this->current_code_edit_area()->redo();}
    break; case Cut: {this->current_code_edit_area()->cut();}
    break; case Copy: {this->current_code_edit_area()->copy();}
    break; case Paste: {this->current_code_edit_area()->paste();}
    break; default: {}
  }
}

auto TabAndTextAreaEtc::set_current_area_read_only(bool f)->void
{
  this->current_code_edit_area()->setReadOnly(f);
}

// 文件是否修改 >>
auto TabAndTextAreaEtc::is_any_file_modified() const -> bool {
  auto tab_widget = this->m_tab_widget;
  const auto total_tab = tab_widget->count();
  if (total_tab > this->m_opened_files.size())
  {
    return true;
  }
  for (auto current_file_index = 0; current_file_index < total_tab; ++current_file_index)
  {
//    auto code_edit_area = qobject_cast<CodeEditArea *>(tab_widget->widget(current_file_index));
    auto code_edit_area = tab_widget->widget(current_file_index);
    if (code_edit_area->isWindowModified())
    {
      return true;
    }
  }
  return false;
}
// 文件是否修改 <<
