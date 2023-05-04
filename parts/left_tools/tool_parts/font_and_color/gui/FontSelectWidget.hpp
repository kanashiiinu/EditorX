#ifndef FONTSELECTWIDGET_HPP
#define FONTSELECTWIDGET_HPP

#include <QWidget>
#include <QFont>
#include <QToolButton>

class FontSelectWidget : public QWidget
{
  Q_OBJECT
 public:
  explicit FontSelectWidget(QWidget *parent = nullptr);
 private:
  QToolButton *font_choose_btn;
  QFont m_font;


 public slots:
  void slot_show_font_dialog();
 public:
 signals:
  void signal_font_change(const QFont &);
};

#endif // FONTSELECTWIDGET_HPP
