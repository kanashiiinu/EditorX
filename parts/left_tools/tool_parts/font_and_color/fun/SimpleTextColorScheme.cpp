#include "SimpleTextColorScheme.hpp"

SimpleTextColorScheme::SimpleTextColorScheme()
  : foreground_color(QColor("lightGray"))
  , background_color(QColor("black"))
  , font_color(QColor("white"))
  , comment_color(QColor("lightGray").darker(150))
  , keyword_color(QColor("green").lighter(130))
  , primitive_type_color(QColor("yellow"))
{
}

QJsonObject SimpleTextColorScheme::to_json_object() const
{
  QJsonObject json;
  json["foreground_color"] = foreground_color.name();
  json["background_color"] = background_color.name();
  json["font_color"] = font_color.name();
  json["comment_color"] = comment_color.name();
  json["keyword_color"] = keyword_color.name();
  json["primitive_type_color"] = primitive_type_color.name();
  return json;
}

void SimpleTextColorScheme::from_json_object(QJsonObject const &json)
{
  if (json.contains("foreground_color") && json["foreground_color"].isString())
  {
    foreground_color = QColor(json["foreground_color"].toString());
  }
  if (json.contains("background_color") && json["background_color"].isString())
  {
    background_color = QColor(json["background_color"].toString());
  }
  if (json.contains("font_color") && json["font_color"].isString())
  {
    font_color = QColor(json["font_color"].toString());
  }
  if (json.contains("comment_color") && json["comment_color"].isString())
  {
    comment_color = QColor(json["comment_color"].toString());
  }
  if (json.contains("keyword_color") && json["keyword_color"].isString())
  {
    keyword_color = QColor(json["keyword_color"].toString());
  }
  if (json.contains("primitive_type_color") && json["primitive_type_color"].isString())
  {
    primitive_type_color = QColor(json["primitive_type_color"].toString());
  }
}
