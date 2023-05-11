#ifndef METAINFOWIDGET_HPP
#define METAINFOWIDGET_HPP

#include <QWidget>
class ClassAnalyzer;
class MetaInfoWidget : public QWidget
{
  Q_OBJECT
 public:
  explicit MetaInfoWidget(QWidget *parent = nullptr);
  ClassAnalyzer *m_class_analyzer;
 signals:

};

#endif // METAINFOWIDGET_HPP
