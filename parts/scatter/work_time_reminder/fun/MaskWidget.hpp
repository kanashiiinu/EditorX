#ifndef MASKWIDGET_H
#define MASKWIDGET_H

#include <QWidget>

/**
 * @brief The MaskWidget class
 * @note 实例一个mask_widget,用来mask主窗口,当特定dialog出现时mask出现
 */
class MaskWidget : public QWidget
{
  Q_OBJECT
 public:
  static MaskWidget *Instance();
  explicit MaskWidget(QWidget *parent = 0);

 protected:
  // 重点
  void showEvent(QShowEvent *);

  bool eventFilter(QObject *obj, QEvent *event);

 private:

  static QScopedPointer<MaskWidget> self;

  //需要遮罩的主窗体
  QWidget *m_main_widget_need_masked;
  //需要弹窗的窗体对象名称集合链表
  QStringList m_dialog_names;

 public slots:

  //e
  //设置需要遮罩的主窗体
  void set_main_widget(QWidget *m_main_widget_need_masked);

  //e
  //设置需要弹窗的窗体对象名称集合链表
  void set_dialog_names(const QStringList &m_dialog_names);

  //e
  //设置遮罩颜色
  void set_background_color(const QColor &back_ground_color);

  //e
  //设置颜色透明度
  void set_opacity(double opacity);
};

#endif // MASKWIDGET_H
