#pragma once

#include <QColor>
#include <QJsonObject>

class SimpleTextColorScheme
{
 public:
  SimpleTextColorScheme();
  QColor foreground_color;
  QColor background_color;
  QColor font_color;
  QColor comment_color;
  QColor keyword_color;
  QColor primitive_type_color;

  QJsonObject to_json_object() const;
  void from_json_object(QJsonObject const &json);
};
