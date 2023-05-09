#ifndef COMPAREFILEWIDGET_HPP
#define COMPAREFILEWIDGET_HPP

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QListWidget>
#include <QMap>
#include <QString>
#include <QList>

class CompareFileWidget : public QWidget
{
  Q_OBJECT

  // left
  QLineEdit *m_left_input;
  QPushButton *m_left_btn;
  QListWidget *m_left_output;

  // right
  QLineEdit *m_right_input;
  QPushButton *m_right_btn;
  QListWidget *m_right_output;
  //
  QPushButton *m_compare_btn;


  //
  QString m_path;
  QStringList m_fileNames1;
  QStringList m_fileNames2;
  QStringList m_compareNames;
  QMap<QString, QString> m_map1;
  QMap<QString, QString> m_map2;
 public:
  CompareFileWidget(QWidget *parent = nullptr);
  ~CompareFileWidget();

 private:
  void _init_ui();
  void _init_text();

 private slots:
  void slot_left_btn_clicked();

  void slot_right_btn_clicked();

  void slot_compare_btn_clicked();

 private:

  void _get_files_info(QMap<QString, QString> &mp, QStringList &names);
};
#endif // COMPAREFILEWIDGET_HPP
