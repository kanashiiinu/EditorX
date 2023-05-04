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
  QLineEdit *left_input;
  QPushButton *left_btn;
  QListWidget *left_output;

  // right
  QLineEdit *right_input;
  QPushButton *right_btn;
  QListWidget *right_output;
  //
  QPushButton *compare_btn;


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


 private slots:
  void slot_left_btn_clicked();

  void slot_right_btn_clicked();

  void slot_compare_btn_clicked();

 private:

  void _get_files_info(QMap<QString, QString> &mp, QStringList &names);
};
#endif // COMPAREFILEWIDGET_HPP
