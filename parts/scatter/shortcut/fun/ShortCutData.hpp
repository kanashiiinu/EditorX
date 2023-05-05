#ifndef SHORTCUTDATA_HPP
#define SHORTCUTDATA_HPP

#include <memory>
#include <vector>
#include <tuple>
#include <QString>
#include <QVariant>
#include <QDebug>


class ShortCutData
{
 public:
  ShortCutData();
  ~ShortCutData() = default;
  ShortCutData(const ShortCutData &) = delete;
  ShortCutData &operator =(const ShortCutData &) = delete;

 private:
  using DataType =  std::vector<std::tuple<QString, QString>>;

 public:
  int row_num() const;
  int column_num() const;
  int get_max_page() const;
  QVariant get_data(int row_index, int column_index);
  void set_val(int row_index, int column_index, QVariant val)
  {
    auto &row = m_data.at(m_page * m_row_num + row_index);
    switch (column_index) {
      case 0: std::get<0>(row) = val.toString(); break;
      case 1: std::get<1>(row) = val.toString(); break;
      default: break;
    }
    qDebug() << row_index << column_index << val;
  }
  void set_page(int page);

  void set_data(DataType const &data)
  {
    m_data = data;
  }
  void set_data(DataType &&data)
  {
    m_data = std::move(data);
  }
 public:
 signals:

 private:

  static QVariant get_by_column(std::tuple<QString, QString>const &row, int column_index);
  DataType m_data;
  int m_row_num = 11;
  int m_column_num = 2;
  int m_page = 0;
};

#endif // SHORTCUTDATA_HPP
