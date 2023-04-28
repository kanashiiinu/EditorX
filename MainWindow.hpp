#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//self
//#include  "MenuBar.hpp"
#include "./parts/menu_bar/gui/MenuBar.hpp"
//#include "LeftToolListAndToolWindow.hpp"
#include "./parts/left_tools/gui/LeftToolListAndToolWindow.hpp"
//#include "TabAndTextAreaEtc.hpp"
#include "./parts/right_tab_editor/gui/TabAndTextAreaEtc.hpp"
//#include "MostLeftUnknownWindow.hpp"
#include "./parts/scatter/gui/MostLeftUnknownWindow.hpp"
//Qt
#include <QMainWindow>
#include <QSessionManager>
//std
#include <ciso646>

/**
 * @brief 整个界面
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QMainWindow *parent = nullptr);
  ~MainWindow();


//  记住退出前的设置 >>
 public:

  /**
   * @brief read_settings
   */
  void read_settings();

  /**
   * @brief write_settings
   */
  void write_settings();

  /**
   * @brief commit_data
   * @param manager
   */
  void commit_data(QSessionManager &manager);
//  记住退出前的设置 <<

  /** menu bar; 给菜单栏的actions设置函数
   * @brief 给菜单栏的actions设置函数
   */
  auto set_up_actions_function()->void;
 protected:
  //s
  void closeEvent(QCloseEvent *event) override;   // 关闭事件
 public:
  /**
  * @brief 新建一个mainWindow
  */
  auto menu_bar_action_function_new_window()->void;

  /**
   * @brief 新建文件，在右侧tab显示
   */
  auto menu_bar_action_function_new_file()->void;

  /**
   * @brief 打开已存在的文件，在右侧tab显示
   */
  auto menu_bar_action_function_open_file()->void;

  /**
   * @brief 打开存在的文件夹，在左侧显示
   */
  auto menu_bar_action_function_open_folder()->void;

  /**
   * @brief 保存当前文件
   */
  auto menu_bar_action_function_save()->void;

  /**
   * @brief 将当前文件另存为
   */
  auto menu_bar_action_function_save_as()->void;
  /**
   * @brief 保存当前tab卡中所有文件
   */
  auto menu_bar_action_function_save_all()->void;

  /**
   * @brief 关闭当前tab卡（关闭当前文件）
   */
  auto menu_bar_action_function_close_tab()->void;

  /**
   * @brief 关闭所有tab选项卡
   */
  auto menu_bar_action_function_close_all_tabs()->void;

  /**
   * @brief 退出
   * @note 暂时是直接退出，未保存当前内容
   */
  auto menu_bar_action_function_quit()->void;

  /**
   * @brief undo
   */
  auto menu_bar_action_function_undo()->void;

  /**
   * @brief redo
   */
  auto menu_bar_action_function_redo()->void;

  /**
   * @brief cut
   */
  auto menu_bar_action_function_cut()->void;

  /**
   * @brief copy
   */
  auto menu_bar_action_function_copy()->void;

  /**
   * @brief paste
   */
  auto menu_bar_action_function_paste()->void;

  /**
   * @brief read only
   */
  auto menu_bar_action_function_read_only()->void;

  /**
   * @brief 切换全屏
   */
  auto menu_bar_action_function_full_screen()->void;

  /**
   * @brief 字体变大
   */
  auto menu_bar_action_function_increase_font_size()->void;

  /**
   * @brief 字体变小
   */
  auto menu_bar_action_function_decrease_font_size()->void;

  /**
   * @brief 在当前文件中查找关键词
   */
  auto menu_bar_action_function_find_in_file()->void;

  /**
   * @brief 在当前文件中替换查找到的关键词
   */
  auto menu_bar_action_function_replace_in_file()->void;

  /**
   * @brief 变量重命名
   */
  auto menu_bar_action_function_rename()->void;

  /**
   * @brief xxx
   */
  auto menu_bar_action_function_xxx()->void;

  /**
   * @brief 帮助-关于
   */
  auto menu_bar_action_function_about()->void;

 private:
  MenuBar *m_menu_bar;

 private:
  TabAndTextAreaEtc *m_tab_and_text_area_etc;

 private:
  LeftToolListAndToolWindow *m_left_tool_list_and_tool_window;

 private:
  MostLeftUnknownWindow *m_most_left_unknown_window;

};
#endif // MAINWINDOW_H
