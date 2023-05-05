#pragma once

#include <QAbstractTableModel>
#include <QButtonGroup>
#include <memory>
#include <vector>
#include <QList>
#include <QString>

#include "ShortCutData.hpp"


class ShortcutsModel : public QAbstractTableModel {
  Q_OBJECT
 public:
  ShortcutsModel(QObject *parent = nullptr);
  ShortcutsModel(const ShortcutsModel &) = delete;
  ShortcutsModel &operator =(const ShortcutsModel &) = delete;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  // 重写，回调，更新数据
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  void set_core_data(std::shared_ptr<ShortCutData> data)
  {
    m_data = data;
  }

 private:
  std::shared_ptr<ShortCutData> m_data;
};
