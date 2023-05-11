#ifndef CODEEDITAREA_HPP
#define CODEEDITAREA_HPP

//self
//#include "CodeEditArea.hpp"
#include "./parts/right_tab_editor/editor/gui/CodeEditArea.hpp"
// Qt
#include <QWidget>
#include <QTabWidget>
#include <QHash>
#include <QPlainTextEdit>
//std
#include <vector>
#include <unordered_map>


class TabAndTextAreaEtc : public QWidget
{

  Q_OBJECT
 public:
//  using CodeEditArea = QPlainTextEdit;
  // 后续用cpp方式实现
//  enum class TextOperation {
//    Undo = 0,
//    Redo,
//    Cut,
//    Copy,
//    Paste
//  };
 public:
  explicit TabAndTextAreaEtc(QWidget *parent = nullptr);


  QString get_current_file_path();

  void reload_current_file();

//  auto get_code_edit_area()-> void;
  /**
   * @brief add_new_file
   */
  auto add_new_file()->void;

  /**
   * @brief 根据路径打开文件，已打开调整位置，未打开则打开
   * @param file_path
   */
  auto open_file_by_path(const QString &file_path)-> void;

  /**
   * @brief get_all_code_of_current_editor
   * @return content in code editor
   */
  auto get_all_code_of_current_editor() -> QString;

  /**
   * @brief 关闭对应tab卡
   * @param index
   */
  auto close_tab(int index)->void;

  /**
   * @brief 操作 (undo|redo|cut|copy|paste) 当前编辑区
   * @param command
   */
  auto operate_current_area(const QString &command)->void;

  /**
   * @brief set_current_area_read_only
   * @param f true read_only
   */
  auto set_current_area_read_only(bool f)->void;

  /**
   * @brief has_content
   * @return
   */
  inline auto has_content() noexcept -> bool {
    return	m_tab_widget->count() not_eq 0;
  }
 private:

  /**
   * @brief 把打开的文件，其部分信息加入父类，以用来管理
   * @param file_path
   * @param opened_file
   */
  auto add_opened_file_into_data(const QString &file_path, CodeEditArea *opened_file)-> void;

  /**
   * @brief 把已打开的文件，其部分信息从父类移除，
   * @param file_path
   */
  auto remove_opened_file_from_data(const QString &file_path)-> void;

  /**
   * @brief get 当前的CodeEditArea
   */
  auto current_code_edit_area()->CodeEditArea*;

  // 文件是否修改 >>
 public:

  /**
   * @brief is_any_file_modified
   * @return
   * @note 给MainWindow使用
   */
  auto is_any_file_modified()const-> bool;

  bool m_any_file_is_modified = false;
  // 文件是否修改 >>
 signals:
  private: public:

  QTabWidget *m_tab_widget;
//  QHash<int, QString> m_file_pos;
  QHash<QString, CodeEditArea *> m_opened_files;

 public:
  enum TextOperation {
    Undo = 0,
    Redo,
    Cut,
    Copy,
    Paste
  };
  Q_ENUM(TextOperation)
};

#endif // CODEEDITAREA_HPP
