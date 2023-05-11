#pragma once

#include <QWidget>
#include <QHash>
#include <QString>

class QLineEdit;
class QListWidget;
class QPushButton;
class QComboBox;

class CodeFormatWidget: public QWidget
{
  Q_OBJECT
 public:
  explicit CodeFormatWidget(QWidget *parent = nullptr);
  ~CodeFormatWidget() = default;

  static bool remove_file(QString const &path);
  static QStringList get_default_clang_format();


 public:
 signals:
  signal_want_open_clang_format_file(QString const &);
  // 请不要改动当前参数
  void signal_format_wait_for_file();
  // 请不要切换当前编辑的文件
  void signal_format_finish();

 public slots:

  void slot_deal_format(QString const &file_path);

 private:

  void _init_ui();

  void _init_text();

  QStringList _command_params(QString const &file_path);

 private:
  QLineEdit *filter_input;
  QPushButton *filter_btn;
  //
  QListWidget *note_list;
  //
  QPushButton *m_add_file_btn;
  QPushButton *m_remove_file_btn;
  QPushButton *m_apply_btn;
  QPushButton *m_format_btn;
  QComboBox *m_defaul_style_box;
  //
  QHash<QString, QString> m_names_to_paths;
  QString m_current_style_name;
  QString m_defaul_style_name;
  //

};
