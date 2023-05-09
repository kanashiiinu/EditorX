#pragma once

#include <QWidget>
#include <QString>


class QTextEdit;
class QPushButton;



class CodeAnalysisWidget: public QWidget
{
  Q_OBJECT
 public:
  explicit CodeAnalysisWidget(QWidget *parent = nullptr);
  ~CodeAnalysisWidget() = default;

 public:
 signals:
  void signal_analysis_wait_for_file();

 public slots:
  void slot_deal_analysis(QString const &file_path);

 private:
  void _init_ui();
  void _init_text();
 private:
  QStringList _command_params(QString const &file_path) const;

 private:
  QTextEdit *m_output_editor;
  QPushButton *m_grammar_check_btn;
  QPushButton *m_cppcoreguidelines_check_btn;
 private:
  QString m_command;
};
