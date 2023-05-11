#include "ParamPass.hpp"

#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QHash>
#include <QSet>
#include <QDebug>
#include <QList>
//std
//#include <cstring>
#include <cctype>
#include <string>


namespace param {



//int fun(in i: int, out o: std::string, inout x: int, move arr: std::vector<int>, foreward arg, ptr p: int)
//{
//  // TODO
//}


///**
//  * @brief
//  * @param[in]
//  * @param[out]
//  * @param[intout]
//  * @param[move]
//  * @param[foreward]
//  * @param[ptr]
//  */
template<typename T>
int fun(int i, std::string &o, int &x, std::vector<int> &&arr, T &&arg, int *p)
{
  // TODO
  return 0;
}

namespace _detail {

QString extract_params(const QString &str)
{
  QRegularExpression regex(R"(\((.*?)\))");
  QRegularExpressionMatch match = regex.match(str);
  if (match.hasMatch()) {
    QString extracted = match.captured(1);
    return extracted;
  }
  return QString();
}

QString extract_body(const QString &str)
{
  QRegularExpression regex(R"(\((.*?)\))");
  QString replacement = "(%1)";
  QRegularExpressionMatch match = regex.match(str);
  if (match.hasMatch()) {
    QString replacedStr = str;
    replacedStr.replace(match.capturedStart(), match.capturedLength(), replacement);
    return replacedStr;
  }
  return QString();
}

QStringList get_param_list(const QString &extracted_params)
{
  return extracted_params.split(',');
}

enum class Kind
{
  Error = -1,
  Min = 0,
  in,
  out,
  inout,
  move,
  foreward,
  ptr,
  Max
};

struct ParamInfo
{
  Kind kind;
  QString type;
  QString name;
};



static QSet<QString> kinds = {
  "in",
  "out",
  "inout",
  "move",
  "foreward",
  "ptr"
};

static QSet<QString> little_keywords = {
  "int",
  "float",
  "double",
  "bool",
  "int8_t",
  "int16_t",
  "int32_t",
  "int64_t"
};


Kind to_kind(const QString &str)
{
  if (str == "in")
  {
    return Kind::in;
  }
  else if (str == "out")
  {
    return Kind::out;
  }
  else if (str == "inout")
  {
    return Kind::inout;
  }
  else if (str == "move")
  {
    return Kind::move;
  }
  else if (str == "foreward")
  {
    return Kind::foreward;
  }
  else if (str == "ptr")
  {
    return Kind::ptr;
  }
  return Kind::Error;
}

QString to_str(Kind kind)
{
  switch (kind) {
    case Kind::in: return "in"; break;
    case Kind::out: return "out"; break;
    case Kind::inout: return "inout"; break;
    case Kind::move: return "move"; break;
    case Kind::foreward: return "foreward"; break;
    case Kind::ptr: return "ptr"; break;
    default: return ""; break;
  }
  return "";
}


QStringList to_parts(const QString &str_)
{
  QString str = str_.trimmed().replace(QRegularExpression("\\s+"), " ");
  const auto len = str.size();
  for (int i = 0; i < len; ++i)
  {
    // ' '
    if (str[i] == ' ')
    {
      if (i - 1 >= 0)
      {
        if (not str[i - 1].isLetter() and not str[i - 1].isDigit())
        {
          continue;
        }
      }
      if (i + 1 < len)
      {
        if (not str[i + 1].isLetter())
        {
          continue;
        }
      }
      str[i] = '$';
    }
    // :
    if (str[i] == ':')
    {
      if (i - 1 >= 0)
      {
        if (str[i - 1] == ':')
        {
          continue;
        }
      }
      if (i + 1 < len)
      {
        if (str[i + 1] == ':')
        {
          continue;
        }
      }
      str[i] = '$';
    }
  }
  auto parts = str.split("$");
  parts.removeAll("");
  return parts;
}


ParamInfo to_param(const QString &str, int pos = 0)
{
  auto parts = to_parts(str);
  parts.removeAll("");
  ParamInfo param;
  if (kinds.contains(parts[0]))
  {
    param.kind = to_kind(parts[0]);
    param.name = parts[1];
    if (parts.size() == 3)
    {
      param.type = parts[2];
    }
    else
    {
      param.type = QString("_T%1").arg(QString::number(pos));
    }
    return param;
  }
  return ParamInfo();
}

QString to_str(ParamInfo const &param)
{
  return QString("param { kind: %1; type: %2; name: %3; }").arg(to_str(param.kind)).arg(param.type).arg(param.name);
}

QList<ParamInfo> to_params(QString const &extracted_params)
{
  auto param_str_list = extracted_params.split(',');
  QList<ParamInfo> params = QList<ParamInfo>();
  int pos = 0;
  for (auto s : param_str_list)
  {
    params.append(to_param(s, pos));
    ++pos;
  }
  return params;
}

QString to_code(ParamInfo const &param)
{
  QString str = QString("%2 %1").arg(param.name);
  switch (param.kind)
  {
    case Kind::in:
    {
      if (little_keywords.contains(param.type))
      {
        str = str.arg(param.type);
      }
      else
      {
        str = str.arg(param.type + " const&");
      }
      return str;
    }
    case Kind::out:
    {
      str = str.arg(param.type + "&");
      return str;
    }
    case Kind::inout:
    {
      str = str.arg(param.type + "&");
      return str;
    }
    case Kind::move:
    {
      str = str.arg(param.type + "&&");
      return str;
    }
    case Kind::foreward:
    {
      str = str.arg(param.type + "&&");
      return str;
    }
    case Kind::ptr:
    {
      str = str.arg(param.type + "*");
      return str;
    }
    default:
    {
      return "";
    }
  }
  return "";
}

QString template_head(QList<ParamInfo> const &params)
{
  QStringList template_params;
  QString template_param = QString("typename %1");
  for (const ParamInfo &param : params)
  {
    if (param.type.startsWith("_T"))
    {
      template_params.append(template_param.arg(param.type));
    }
  }
  return QString("template<%1>").arg(template_params.join(", "));
}


/**
 * @brief to_code_param_str
 * @param[in] params
 * @return
 */
QString to_code_param_str(QList<ParamInfo> const &params)
{
  auto len = params.size();
  QStringList param_str_list;
  for (int i = 0; i < len; ++i)
  {
    param_str_list.append("");
  }
  std::transform(std::begin(params), std::end(params), std::begin(param_str_list), to_code);
  return param_str_list.join(", ");
}


/**
 * @brief to_comment_str
 * @param params
 * @return
 */
QString to_comment_str(QList<ParamInfo> const &params)
{
  QStringList comment_params;
  QString comment_param = QString(" * @param[%1] %2");
  QString total = QString("/**\n%1\n */\n");
  for (const ParamInfo &param : params)
  {
    comment_params.append(comment_param.arg(to_str(param.kind)).arg(param.name));
  }
  return total.arg(comment_params.join("\n"));
}

}

void set_little_keywords(QString const &s)
{
  _detail::little_keywords.insert(s);
}

QString translate(QString const &head)
{
  using namespace _detail;
  auto extracted_params = _detail::extract_params(head);
  auto params = _detail::to_params(extracted_params);
  return _detail::to_comment_str(params)
         .append(_detail::template_head(params)
                 .append(_detail::extract_body(head)
                         .arg(_detail::to_code_param_str(params))));
}


//in : 小的单位 ;const &
//out|inout : &
//move : &&
//foreward : template<typename Tn> Tn&&
//ptr: *

}

static auto head = R"(int fun(in i:int, out o:std::string, inout x, move arr:std::vector<int>, foreward arg, ptr p:int)\n)";

ParamPass::ParamPass()
{
  qDebug() << param::translate(head);
}
QString ParamPass::translate(QString const &str)
{
  return param::translate(str);
}
