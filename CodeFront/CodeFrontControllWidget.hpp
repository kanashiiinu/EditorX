#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QString>

class CodeFrontControllWidget : public QWidget
{
  Q_OBJECT

 public:
  CodeFrontControllWidget(QWidget *parent = nullptr);
  ~CodeFrontControllWidget();
 public:
  static QString translate_line(QString const &line);
};
#endif // WIDGET_H
