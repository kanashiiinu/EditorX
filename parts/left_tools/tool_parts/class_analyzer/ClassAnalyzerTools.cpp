#include "ClassAnalyzerTools.hpp"

auto class_text(OrignalText const &text, int from_index, QVector<QString> class_list)->QVector<QString> {
  auto start_index = text.indexOf("class", from_index);
  auto maybe_claim = true;
  auto count_of_left = 0;
  auto count_of_right = 0;
  for (int i = start_index; i < text.size(); ++i) {
    if (text.at(i) == '{') {
      ++count_of_left;
      maybe_claim = false;
    }
    if (text.at(i) == QChar('}')) {
      ++count_of_right;
      if (count_of_left == count_of_right) {
        class_list.push_back(text.mid(start_index, i));
        return class_text(text, i, class_list);
      }
    }
    if (text.at(i) == ';' and maybe_claim) {
      class_list.push_back(text.mid(start_index, i));
      return class_text(text, i, class_list);
    };
    if (text.at(i) == 'c'/*c:class,e:enum,s:struct,u:union,e:enum class*/) {
      if (i + 4 > text.size()) {continue;}
      auto word = text.mid(i, i + 4);
      if (word not_eq "class ") {continue;}
      return class_text(text, i, class_list);
    }
  }
  return class_list;
}

auto class_name(OrignalText const &text)-> ClassName {
  for (int i = 0; i < text.size(); ++i) {
    if (text.at(i) == ':' or text.at(i) == '{') {
      return text.mid(5, i - 5).trimmed();
    }
  }
  return  text.mid(5, text.size() - 2).trimmed();
}

auto remove_comment(OrignalText const &text)-> QString {
  auto re_1 = QRegularExpression(R"(//[^\n]*)");
  auto re_2 = QRegularExpression(R"(/\*(\s|.)*?\*/)");
  auto no_comment_text = text;
  return no_comment_text.remove(re_1).remove(re_2);
}

auto class_inner_str(QString const &text) {
//  QString res;
  auto begin_index = text.indexOf('{');
  auto end_index = text.size() - 1;
  for (int i = end_index; i >= 0; --i) {
    if (text.at(i) == '}') {
      end_index = i;
      break;
    }
  }
//  return text.mid(begin_index, end_index - begin_index + 1); // {}
  return text.mid(begin_index + 1, end_index - begin_index - 1); // no {}
}

auto split_to_lines(QString class_str)-> QStringList {
  auto list = QStringList();
  auto is_space = [](const auto & s) {
    for (auto c : s) {
      if (c != ' ') { return false; }
    }
    return true;
  };
  for (QString l : class_str.split(QRegularExpression("\n")))  {
    if (l.isEmpty()) {continue; }
    if (is_space(l)) {continue; }
    list.append(l);
  }
  return list;
}

auto analyze_line(const QString &line)->QString {
  if (line.contains("private")) {return"access";}
  if (line.contains("public")) {return"access";}
  if (line.contains("protected")) {return"access";}
  if (line.contains("(")) {return"function like";}
  return "field";
}

//field: [access][static][constexpr][const]name[= default_value];
//funciotn: [access][friend][static|virturl][inline][static][constexpr][return_type]func_name([param])[const][override;

auto analyze_field(QString const &line, QString const &access)->FieldInfo {
  QSet<QString> words = QSet<QString> {"static", "constexpr"};
  FieldInfo field;
  auto list = line.trimmed().remove(';').split(' ');
//  auto may_name_or_default_val = list.size() - 1;
  auto default_val_index = -1;
  auto name_index = list.size() - 1;
  auto may_type_begin_index = 0;
  auto may_type_end_index = name_index;
  auto may_word_begin_index = 0;
  auto may_word_end_index = 0;
  for (int i = list.size() - 1; i >= 0 ; --i) {
    if (list.at(i) == '=') {
      name_index = i - 1;
      default_val_index = i + 1;
    }
    if (words.contains(list.at(i))) {
      may_word_end_index = i + 1;
      may_type_begin_index = i + 1;
      break;
    }
  }
  may_type_end_index = name_index;
  field.access = access;
  field.name = list.at(name_index);
  for (int i = may_type_begin_index; i != may_type_end_index; ++i) {
    field.type.append(list.at(i) + " ");
  }
  field.type = field.type.trimmed();
  for (int i = may_word_begin_index; i != may_word_end_index; ++i) {
    field.front_words.append(list.at(i));
  }
  if (default_val_index != -1) {
    field.default_value = list.at(default_val_index);
  }
  return field;
}

//funciotn: [access][friend][static|virturl][inline][static][constexpr][return_type]func_name([param])[const][override;
auto analyze_function(QString const &line, QString const &access)->FunctionInfo {
  QRegularExpression re_1(R"((\([^\)]*\)))");
  QRegularExpression re_2(R"((\{[^\}]*\}))");
  auto param = QString();
  auto match_1 = re_1.match(line);
  if (match_1.hasMatch()) {
    param = match_1.captured();
  }
  auto body = QString();
  auto match_2 = re_2.match(line);
  if (match_2.hasMatch()) {
    body = match_2.captured();
  }

  auto res = line.trimmed();
  if (res.back() == ';') {
    res = res.left(res.size() - 1);
  }
  res.remove(re_2);
  res.replace(re_1, " ");
  auto list = res.split(' ', QString::SkipEmptyParts);
  //  qDebug() << list;
  auto front_words = QSet<QString> {"explicit", "static", "friend", "virtual", "constexpr"};
  auto back_words = QSet<QString> {"noexcept", "const", "override", "=", "0"};
  auto back_words_begin_index = list.size();
  auto back_words_end_index = list.size();
  auto name_index = list.size() - 1;
  auto type_begin_index = 0;
  auto type_end_index = name_index;
  auto front_words_begin_index = 0;
  auto front_words_end_index = type_begin_index;
  for (int i = list.size() - 1; i >= 0; --i) {
    if (back_words.contains(list.at(i))) {
      back_words_begin_index = i;
    }
    if (front_words.contains(list.at(i))) {
      type_begin_index = i + 1;
      break;
    }
  }
  name_index = back_words_begin_index - 1;
  type_end_index = name_index;
  front_words_end_index = type_begin_index;
  FunctionInfo func;
  func.access = access;
  func.name = list.at(name_index);
  func.param = param;
  func.body = body;
  for (int i = front_words_begin_index; i != front_words_end_index; ++i) {
    func.front_words.append(list.at(i));
  }
  for (int i = type_begin_index; i != type_end_index; ++i) {
    func.type.append(list.at(i) + " ");
  }
  func.type = func.type.trimmed();
  for (int i = back_words_begin_index; i != back_words_end_index; ++i) {
    func.back_words.append(list.at(i));
  }
  return func;
}

auto class_total_info(const QString &s)->ClassInfo {
  ClassInfo class_info;
  class_info.class_name = class_name(s);
  auto access = QString("private");
  for (auto l : split_to_lines(class_inner_str(s))) {
    auto info = analyze_line(l);
    if (info == "access") {
      l = l.trimmed();
      access = l.left(l.size() - 1);
    }
    if (info == "field") {
      auto f = analyze_field(l, access);
      class_info.fields.append(f);
    }
    if (info == "function like") {
      auto f = analyze_function(l, access);
      class_info.functions.append(f);
    }
  }
  return class_info;
}
