#include "ShortCutModel.hpp"

static int strToKey(QString str)
{

  int i = str.lastIndexOf(" ");
  QKeySequence seq = QKeySequence(str.mid(i + 1, str.size()));

  if (seq.count() != 1)
    return -1;

  return seq[0];
}

static Qt::KeyboardModifiers strToModifiers(QString str)
{
  Qt::KeyboardModifiers modifiers;

  if (str.contains("Alt"))
    modifiers |= Qt::AltModifier;

  if (str.contains("Shift"))
    modifiers |= Qt::ShiftModifier;

  if (str.contains("Ctrl"))
    modifiers |= Qt::ControlModifier;

  return modifiers;
}

ShortcutsModel::ShortcutsModel(QObject *parent)
  : QAbstractTableModel(parent)
{
}

int ShortcutsModel::rowCount(const QModelIndex &idx) const
{
  Q_UNUSED(idx);
  return m_data->row_num();
}

int ShortcutsModel::columnCount(const QModelIndex &idx) const
{
  Q_UNUSED(idx);
  return m_data->column_num();
}

QVariant ShortcutsModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    return m_data->get_data(index.row(), index.column());
  }
  return QVariant();
}

bool ShortcutsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role == Qt::EditRole) {
    const int row = index.row();
    m_data->set_val(row, index.column(), value);
    //发送信号触发刷新
    emit this->dataChanged(index, index, QVector<int>() << role);
    return true;
  }
  return false;
}

Qt::ItemFlags ShortcutsModel::flags(const QModelIndex &index) const
{
  if (not index.isValid())
  {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

// 写的不对, 数据显示不出来
QVariant ShortcutsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal)
  {
    switch (section)
    {
      case 0:
        return QString("op");
      case 1:
        return QString("keys");
      default:
        return QVariant();
    }
  }
  return QVariant();
}
