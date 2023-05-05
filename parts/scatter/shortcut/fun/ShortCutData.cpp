#include "ShortCutData.hpp"

ShortCutData::ShortCutData()
{
  m_data = {
    { "Close Tab", "Ctrl + W" },
    { "Save", "Ctrl + S" },
    { "SaveAs", "Ctrl + Shift + S" },
    { "Delete", "Del" },
    { "Cut", "Ctrl + X" },
    { "Copy", "Ctrl + C" },
    { "Paste", "Ctrl + V" },
    { "Undo", "Ctrl + Z" },
    { "Redo", "Ctrl + Shift + Z" },
    { "Increase Font Size", "Ctrl + Plus" },
    { "Decrease Font Size", "Ctrl + Minus" },
    { "AddTab", "Ctrl + T" },
    { "Find", "Ctrl + F" },
    { "Find Previous", "Shift + F3" },
    { "Replace", "Ctrl + H" },
    { "Move To Next Char", "Right" },
    { "Move To Previous Char", "Left" },
    { "Move To Next Word", "Ctrl + Right" },
    { "Move To Previous Word", "Ctrl + Left" },
    { "Move To Next Line", "Down" },
    { "Move To Previous Line", "Up" },
    { "Move To Start Of Line", "PgDown" },
    { "Move To End Of Line", "PgUp" },
    { "Move To Start Of Document", "Ctrl + Home" },
    { "Move To End Of Document", "Ctrl + End" },
    { "Select Next Char", "Shift + Right" },
    { "Select Previous Char", "Shift + Left" },
    { "Select Next Word", "Ctrl + Shift + Right" },
    { "Select Previous Word", "Ctrl + Shift + Left" },
    { "Select Next Line", "Shift + Down" },
    { "Select Previous Line", "Shift + Up" },
    { "Select Start Of Line", "Shift + PgDown" },
    { "Select End Of Line", "Shift + PgUp" },
  };

}

int ShortCutData::row_num() const
{
  return m_row_num;
}

int ShortCutData::column_num() const
{
  return m_column_num;
}

QVariant ShortCutData::get_data(int row_index, int column_index)
{
  const auto &row = m_data.at(m_page * m_row_num + row_index);
  return get_by_column(row, column_index);
}

void ShortCutData::set_page(int page)
{
  m_page = page;
}

QVariant ShortCutData::get_by_column(const std::tuple<QString, QString> &row, int column_index)
{
  switch (column_index) {
    case 0:
    {
      return std::get<0>(row);
    }
    case 1:
    {
      return std::get<1>(row);
    }
    default:
    {
      return QVariant();
    }
  }
  return QVariant();
}

int ShortCutData::get_max_page() const
{
  return m_data.size() / m_row_num;
}
