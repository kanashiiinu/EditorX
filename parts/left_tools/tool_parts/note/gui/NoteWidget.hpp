#pragma once

#include <QWidget>
#include <QHash>
#include <QString>

class QLineEdit;
class QListWidget;
class QPushButton;

class NoteWidget: public QWidget
{
  Q_OBJECT
 public:
  explicit NoteWidget(QWidget *parent = nullptr);
  ~NoteWidget() = default;

  static bool remove_file(QString const &path);
 public:
 signals:
  signal_want_open_note(QString const &);
 private:
  void _init_ui();
  void _init_text();
 private:
  QLineEdit *filter_input;
  QPushButton *filter_btn;
  //
  QListWidget *note_list;
  //
  QPushButton *add_file_btn;
  QPushButton *remove_file_btn;
  //
  QHash<QString, QString> m_names_to_paths;
};
