#pragma once

#include <QWidget>
#include <memory>
#include <QTableView>
#include "./parts/scatter/shortcut/fun/ShortCutData.hpp"
#include "./parts/scatter/shortcut/fun/ShortCutModel.hpp"

class ShortCutWidget : public QWidget
{

  Q_OBJECT

 public:

  ShortCutWidget(QWidget *parent = nullptr);
 private:
  std::shared_ptr<ShortCutData> m_data;
  QTableView *m_shortcut_table_view;
  ShortcutsModel *model;
 protected:
  // 就是resize
  void resizeEvent(QResizeEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
};
