#include "CodeFrontControllWidget.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <unordered_set>
#include <functional>
#include <cassert>



/*
//
// cppfront 声明 1
  i:int = 10; => int i = 10;
  v:= std::vector<int>{0, 1, 2}; => auto v = std::vector<int>{0, 1, 2};
  f:(i:int, inout x:int, -> :int)={}; => auto f(int i, int& x)->int{};
  - - - - - - - - - -
  f:(i:int, x:int, -> :int)={}; => auto f(int i, int& x)->int{}
  pub static friend virtual constexpr override noexcpet
  f:(i:int, inout x:int, -> res:int)={}; => auto f(int i, int& x)->int{ int res; }

// 函数式 管道 2
  x|round => round(x);
  x|round|add_2; => add_2(round(x));

函数结合顺序
  retry:(f);  retry f => retry(f)
  do_:(n:int, f); f|do_(3) <=>  f do_ 3 => do_(3, f) <=> do_ 3 f <=> do_(3)(f)

// lang
  is:(x:T, pred:(T, ->:bool))->bool;
  as:(x:T, f:(T, ->:S))->S;
  to:(x:T, f:(T, ->:S))->S;

  x is 3            => x == 3
  x is not 3        => x is not :(_) = { return _ == 3;} => x is :(_){ return not :(_){retrun _==3;}(_);} => not_equal_3(x)
  x is "ee"         => x == std::string("ee")
  x is not "ee"     => not(x == std::string("ee"))
  x is empty        => empty(x)
  x is not empty    => not(empty(x))
  x as std::string  => as_<std::string>(x)
  x to std::string  => to_<std::string>(std::move(x))
- - - - - -- - - - -- - - - - - -- - - - - - - -- - -
  x|from_rect_to_points
  x as points => as_(points, x)<=>as_(points)(x)<=>as_points(x)<=>points(x);
  x|std::move|remove_space => remove_space(std::move(x))
  x to no_space => no_space(std::move(x))<=>to_(no_space, std::move(x))

// 函数式与对stl的使用 3
  res = xs | transform(f); => std::transform(std::begin(xs), std::end(xs), std::begin(res), f);
  res = xs | filter(f) => for(const auto& x:xs){ if(f(x){res.push_back(x);} }
  res = xs | foldl(f, 0) => res = std::accumulate(std::begin(xs), std::end(xs), 0, f);
  res = xs | foldr(f, 0) => res = std::accumulate(std::rbegin(xs), std::rend(xs), 0, f);

// cppfront lambda 4
  :(x)={}       => [](auto x){}
  :(in x)={}    => [](auto const& x){}
  :(inout x)={} => [](auto& x){}
  :(move x)={}  => [](auto&& x){}
xxx => x : x = {}
x is empty;


// self for 5
  xs do :(x) => for (auto x : xs)
  xs do :(in x) => for (auto const& x : xs)
  xs do :(inout x) => for(auto& x : xs)
- - - - - - - - - - - -- - - - - - - -- - -
  for (x : xs) {} => for_each (xs, :(x){})


---------------------------------------------------

//self 函数式 place+holder 6
  f(_, _, 3) => [=](auto p_1, auto p_2){ return f(p_1, p_2, 3); }
  f(_2, _1, 3) => [=](auto p_1, auto p_2){ return f(p_2, p_1, 3); }

  x:= f(_, _, 3); => auto x = [=](auto p_1, auto p_2){ return f(p_1, p_2, 3); };

//self 函数式 柯里化 7
// f必须是函数
  f(_c2) => [](auto p_1){ return [=](auto p_2){ return f(p_1, p_2);  };  }


// condition
condition -> xxx;
if (condition) {} => do_while(condition, :(){})
if (condition) {} else {} => if_else(condition, :(){}, :(){})

if (xxx){ return } <=> xxx -> return


// pattern match

     if () {}
else if () {}
else if () {}
else       {}

*/

namespace front {

void print(std::string const &s)
{
  std::cout << s;
}

void println(std::string const &s)
{
  print(s);
  std::cout << std::endl;
}

std::string trim(std::string &&s)
{
  if (s.empty())
  {
    return s;
  }
  s.erase(0, s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ") + 1);
  return s;
}

// 可以再改
std::string trim(std::string const &s_)
{
  if (s_.empty())
  {
    return s_;
  }
  auto s = s_;
  return trim(std::move(s));
}

//注意：当字符串为空时，也会返回一个空字符串
std::vector<std::string> split(std::string const &s, std::string const &delim)
{
  auto ret = std::vector<std::string>();
  size_t last = 0;
  size_t index = s.find_first_of(delim, last);
  while (index != std::string::npos)
  {
    ret.push_back(s.substr(last, index - last));
    last = index + 1;
    index = s.find_first_of(delim, last);
  }
  if (index - last > 0)
  {
    ret.push_back(s.substr(last, index - last));
  }
  return ret;
}

std::vector<std::string> split(std::string &&s, std::string const &delim)
{
  auto ret = std::vector<std::string>();
  size_t last = 0;
  size_t index = s.find_first_of(delim, last);
  while (index != std::string::npos)
  {
    ret.push_back(s.substr(last, index - last));
    last = index + 1;
    index = s.find_first_of(delim, last);
  }
  if (index - last > 0)
  {
    ret.push_back(s.substr(last, index - last));
  }
  return ret;
}

std::string join(std::vector<std::string> const &strings, std::string const &separator)
{
  auto res = std::string();
  if (strings.empty())
  {
    return res;
  }
  res.append(strings.front());
  std::for_each(std::next(std::begin(strings)), std::end(strings), [&](auto const & str) {
    res.append(separator);
    res.append(str);
  });
  return res;
}

std::string replace_all(std::string &&src, std::string const &old_value, std::string const &new_value) {
  using namespace  std;
  // 每次重新定位起始位置，防止上轮替换后的字符串形成新的old_value
  for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
    if ((pos = src.find(old_value, pos)) != string::npos) {
      src.replace(pos, old_value.length(), new_value);
    } else {
      break;
    }
  }
  return src;
}
// 确保" {}"第一个补一个空格再传入
std::string _insert_place_hold(std::string &&s, std::vector<std::string> &&vals, size_t left, size_t right)
{
  if (vals.empty()) { return s; }
  left = s.find_first_of('{', left);
  while (left != std::string::npos) {
    if (s[left - 1] ==  '\\') {
//      s[left - 1] = ' ';
      left = s.find_first_of('{', left + 1);
    }
    else { break; }
  }
  right = s.find_first_of('}', right);
  while (right != std::string::npos) {
    if (s[right - 1] ==  '\\') {
//      s[right - 1] = ' ';
      right = s.find_first_of('}', right + 1);
    }
    else { break; }
  }
  if (left == right)
  {
    return s;
  }
  s.replace(left, right + 1 - left, vals.back());
  vals.pop_back();
  return _insert_place_hold(std::move(s), std::move(vals), left, right);
}

std::string insert_place_hold(std::string &&s, std::vector<std::string> &&vals)
{
  s = " " + s;
  std::reverse(std::begin(vals), std::end(vals));
  auto str = _insert_place_hold(std::move(s), std::move(vals), 0, 0);
  str.erase(0, 1);
  return replace_all(replace_all(std::move(str), "\\{", "{"), "\\}", "}");
}

static std::unordered_set<std::string> s_front_keys =
{
  "public",
  "protect",
  "private",
  "static",
  "friend",
  "virtual",
  "constexpr",
  "override",
  "noexcept",
  "in",
  "out",
  "inout",
  "move",
  "forward",
  "ptr",
  "->",
  "var",
  "fun"
};

struct Sentence_0
{
  std::vector<std::string> keys;
  std::vector<std::string> separators;
  std::string name;
  std::string what;
  std::string body;
};


struct Declare
{
  std::string name;
  std::string what;
  std::string body;
};


std::string sub_by_index(std::string const &s, int begin, int end)
{
  return s.substr(begin, end - begin);
}

std::string get_sentence_content(std::string const &s)
{
  int begin = s.find_first_not_of(" ");
  int end  = s.find_last_of(";");
  return sub_by_index(s, begin, end);
}

std::string get_sentence_content(std::string &&s)
{
  s.erase(0, s.find_first_not_of(" "));
  s.erase(s.find_last_of(";"));
  return s;
}

Sentence_0 to_sentence(std::string const &declare_str_)
{
  // i:int=10; : = => declare: name:what=body;
  // res = xxx; = => val: name = body;
  // x is empty; => call: body;


  auto declare_str = get_sentence_content(declare_str_);
  Sentence_0 sentence;
  int body_end = declare_str.size();
  int what_end = declare_str.size();
  size_t what_body_separator = declare_str.find_last_of('=');
  if (what_body_separator != std::string::npos)
  {
    sentence.body = trim(sub_by_index(declare_str, what_body_separator + 1, body_end));
    sentence.separators.push_back("=");
    what_end = what_body_separator;
  }
  size_t name_what_separator = declare_str.find_first_of(':');
  if (name_what_separator != std::string::npos)
  {
    sentence.what = trim(sub_by_index(declare_str, name_what_separator + 1, what_end));
    sentence.separators.push_back(":");
  }
  auto words = split(sub_by_index(declare_str, 0, name_what_separator), " ");
  for (const auto &word : words)
  {
    if (s_front_keys.find(word) != s_front_keys.end())
    {
      sentence.keys.push_back(word);
    }
    else
    {
      sentence.name = word;
      break;
    }
  }
  std::reverse(std::begin(sentence.separators), std::end(sentence.separators));
  return sentence;
}

Sentence_0 to_sentence_new_version(std::string const &declare_str_)
{
  // 1:declare; 2:use={get_val|discard_val};
  // 有:一定是定义;(暂时)没有是使用,有=get_val,无=discard_val;
  // v:int; => declare "int v"
  // i:int=10;
  // => declare "int i" and  init "i = 10";
  // => i:int;i=10; declare and use
  // res = get_val(); => use and get val;
  // x is empty; => use but discard val;

  // i:int=10; : = => declare: name:what=body;
  // res = xxx; = => val: name = body;
  // x is empty; => call: body;
  auto content_str = get_sentence_content(declare_str_);
  Sentence_0 sentence;
  int body_end = content_str.size();
  int what_end = content_str.size();
  bool has_body_separator = false;
  size_t what_body_separator = content_str.find_last_of('=');
  if (what_body_separator != std::string::npos)
  {
    sentence.body = trim(sub_by_index(content_str, what_body_separator + 1, body_end));
    sentence.separators.push_back("=");
    what_end = what_body_separator;
    has_body_separator = true;
  }
  size_t name_what_separator = content_str.find_first_of(':');
  bool has_declare_separator = false;
  if (name_what_separator != std::string::npos)
  {
    sentence.what = trim(sub_by_index(content_str, name_what_separator + 1, what_end));
    sentence.separators.push_back(":");
    has_declare_separator = true;
  }
  else
  {
    has_declare_separator = false;
    if (not has_body_separator)
    {
      sentence.body = trim(content_str);
      name_what_separator = 0;
    }
  }
  auto words = split(sub_by_index(content_str, 0, name_what_separator), " ");
  for (const auto &word : words)
  {
    if (s_front_keys.find(word) != s_front_keys.end())
    {
      sentence.keys.push_back(word);
    }
    else
    {
      sentence.name = word;
      break;
    }
  }
  std::reverse(std::begin(sentence.separators), std::end(sentence.separators));
  return sentence;
}


std::string to_str(Sentence_0 const &sentence)
{
  auto str = std::string("Sentence{");
  str.append("keys:");
  for (auto const &s : sentence.keys)
  {
    str.append(s);
    str.push_back('$');
  }
  str.push_back(';');
  str.append("name:" + sentence.name + ";what:" + sentence.what + ";body:" + sentence.body + ";spector$");
  for (auto const &s : sentence.separators)
  {
    str.append(s);
    str.push_back('$');
  }
  str.append(";}");
  return str;
}


/**
 * @brief to_declare
 * @param declare_str
 * @return
 * @note 确保str是一个声明
 */
Declare to_declare(std::string const &declare_str)
{
  Declare declare;
  int body_end = declare_str.size();
  int what_end = declare_str.size();
  int name_end = declare_str.size();
  size_t what_body = declare_str.find_last_of('=');
  if (what_body != std::string::npos)
  {
    declare.body = trim(sub_by_index(declare_str, what_body + 1, body_end));
    what_end = what_body;
  }
  size_t name_what = declare_str.find_first_of(':');
  if (name_what != std::string::npos)
  {
    declare.what = trim(sub_by_index(declare_str, name_what + 1, what_end));
    name_end = name_what;
  }
  declare.name = trim(sub_by_index(declare_str, 0, name_end));
  return declare;
}

std::string to_str(Declare const &d)
{
  return "Declare{ name: " + d.name + "; what: " + d.what + "; body: " + d.body + "; }";
}

bool declare_is_fun(Declare const &d)
{
  if (d.what.front() == '(' and d.what.back() == ')')
  {
    return true;
  }
  return false;
}

bool declare_is_lambda(Declare const &d)
{
  if (declare_is_fun(d) and d.name.empty())
  {
    return true;
  }
  return false;
}

namespace declare {

std::string to_what_str(std::string const &s) {
  auto d = to_declare(get_sentence_content(s));
  if (declare_is_fun(d))
  {
    if (d.name.empty())
    {
      return "lambda";
    }
    else
    {
      return "fun";
    }
  }
  return "var";
}

std::string to_var_str(Declare const &d)
{

  auto what = d.what;
  if (d.what.empty())
  {
    what = "auto";
  }
  auto body = d.body;
  if (not d.body.empty())
  {
    body = " = " + body;
  }
  return what + " " + d.name + body;
}

void to_fun_str(Declare const &d)
{

}


}

namespace pipe {

std::string from_pipe_order(std::string const &input)
{
  auto val_funs = front::split(front::trim(input), "|");
  // val
  // remove first>>
  auto val = std::move(val_funs.front());
  val_funs.erase(std::begin(val_funs));
  // <<
  // funs
  auto funs = std::move(val_funs);
  // res
  auto result = std::string();
  auto f = [&result](auto const & s) {
    result.append(s);
    result.push_back('(');
  };
  std::for_each(std::rbegin(funs), std::rend(funs), f);
//  { cpp20
//    auto f =     [](auto res, auto const & s) {
//      res.append(s);
//      res.push_back('(');
//      return res;
//    };
//    front_str = std::accumulate(std::rbegin(sss), std::rend(sss), "", f);
//  }
  result.append(val);
  auto right_brakets = std::string(funs.size(), ')');
  result.append(right_brakets);
  return result;
}

}

namespace lang_order {

static std::unordered_set<std::string> g_lang_op = std::unordered_set<std::string> {"is", "as", "to"};

bool lang_op_contains(std::string const &s)
{
  if (g_lang_op.find(s) not_eq g_lang_op.end())
  {
    return true;
  }
  return false;
}



bool is_num(std::string const &str)
{
  std::stringstream sin(str);
  double d;
  char c;
  if (not (sin >> d))
  {
    return false;
  }
  if (sin >> c)
  {
    return false;
  }
  return true;
}

bool is_cstring(std::string const &str)
{
  if (str.front() != str.back())
  {
    return false;
  }
  if (str.back() != '"')
  {
    return false;
  }
  return true;
}

std::string to_is_str(std::vector<std::string> const &list)
{
  auto head = std::string();
  auto tail = std::string();
  auto val = list.front();
  auto body = list.back();
  if (list.at(2) == "not")
  {
    head = "not(";
    tail = ")";
  }
  if (is_num(body))
  {
    return head + val + " == " + body + tail;
  }
  if (is_cstring(body))
  {
    return head + val + " == std::string(" + body + ")" + tail;
  }
  return head + body + "(" + val + ")" + tail;

}

std::string to_as_str(std::vector<std::string> const &list)
{
  return list.at(1) + "_<" + list.at(2) + ">(" + list .at(0) + ")";
}

std::string to_to_str(std::vector<std::string> const &list)
{
  return list.at(1) + "_<" + list.at(2) + ">(std::move(" + list .at(0) + "))";
}

// 现在用的逻辑不通用，等待修改
std::string from_lang(std::string const &s)
{

  std::vector<std::string> list = split(get_sentence_content(s), " ");
  if (list.at(1) == "is")
  {
    return to_is_str(list);
  }
  else if (list.at(1) == "as")
  {
    return to_as_str(list);
  }
  else if (list.at(1) == "to")
  {
    return to_to_str(list);
  }
  return{};
}
}

namespace stl {
// 函数式与对stl的使用 3
//  res = xs | transform(f); => std::transform(std::begin(xs), std::end(xs), std::begin(res), f);
//  res = xs | filter(f) => for(const auto& x:xs){ if(f(x){res.push_back(x);} }
//  res = xs | foldl(f, 0) => res = std::accumulate(std::begin(xs), std::end(xs), 0, f);
//  res = xs | foldr(f, 0) => res = std::accumulate(std::rbegin(xs), std::rend(xs), 0, f);
std::string transform_op(std::string const &var, std::string const &rng, std::string const &fun, std::vector<std::string> const &params)
{
  Q_UNUSED(fun);
//  "std::transform(std::begin(rng$), std::end(rng$), std::begin(var$), params[0$)";
  return front::insert_place_hold("std::transform(std::begin({}), std::end({}), std::begin({}), {})", {rng, rng, var, params[0]});
}

std::string filter_op(std::string const &var, std::string const &rng, std::string const &fun, std::vector<std::string> const &params)
{
  Q_UNUSED(fun);
//  "for(auto& const item:rng$){ if(params[0]$(item){var$.push_back(item);} }";
  return front::insert_place_hold(R"(for(auto& const item:{})\{ if({}(item))\{{}.push_back(item);\} \})", {rng, params[0], var});
}

std::string foldl_op(std::string const &var, std::string const &rng, std::string const &fun, std::vector<std::string> const &params)
{
  Q_UNUSED(fun);
//  "var$ = std::accumulate(std::begin(rng$), std::end(rng$), params[1]$, params[0]$)";
  return front::insert_place_hold("{} = std::accumulate(std::begin({}), std::end({}), {}, {})", {var, rng, rng, params[1], params[0]});
}

std::string foldr_op(std::string const &var, std::string const &rng, std::string const &fun, std::vector<std::string> const &params)
{
  Q_UNUSED(fun);
//  "var$ = std::accumulate(std::rbegin(rng$), std::rend(rng$), params[1]$, params[0]$)";
  return front::insert_place_hold("{} = std::accumulate(std::rbegin({}), std::rend({}), {}, {})", {var, rng, rng, params[1], params[0]});
}

// 确保输入是transfrom|filter|fold
std::string from_transform_filter_fold(Sentence_0 const &sentence)
{
  std::string s = sentence.body;
  size_t pipe_mark = s.find_first_of('|');
  auto range = trim(sub_by_index(s, 0, pipe_mark));
  auto fun_and_params = trim(sub_by_index(s, pipe_mark + 1, s.size()));
  auto left_b = fun_and_params.find_first_of('(');
  auto fun_name = sub_by_index(fun_and_params, 0, left_b);
  auto right_b = fun_and_params.find_last_of(')');
  auto params = split(sub_by_index(fun_and_params, left_b + 1, right_b), ",");
  std::transform(std::begin(params), std::end(params), std::begin(params), (std::string (*)(std::string const &))trim);

//  std::cout << "range:" + range << ";fun name:" << fun_name << ";params:";
//  for (auto param : params) { std::cout << param << ";";}
//  std::cout << std::endl;

  if (fun_name == "transform") {
    return transform_op(sentence.name, range, fun_name, params);
  } else if (fun_name == "filter")  {
    return filter_op(sentence.name, range, fun_name, params);
  } else if (fun_name == "foldl")  {
    return foldl_op(sentence.name, range, fun_name, params);
  } else if (fun_name == "foldr") {
    return foldr_op(sentence.name, range, fun_name, params);
  }
  return std::string();
}


}

namespace lambda {

void to_lambda_str(Declare const &d)
{

}

}

namespace for_range {

}

namespace placeholder {

}

namespace curry {

}

}


namespace param {

namespace _detail {

static std::unordered_set<std::string> little_keywords = {
  "int",
  "float",
  "double",
  "bool",
  "int8_t",
  "int16_t",
  "int32_t",
  "int64_t"
};



void set_little_keywords(std::string const &s)
{
  _detail::little_keywords.insert(s);
}



struct ParamInfo_0
{
  std::string kind;
  std::string type;
  std::string name;
};

ParamInfo_0 to_param(front::Sentence_0 const &sentence, int pos = 0)
{
  ParamInfo_0 param;
  auto kind = std::string();
  if (sentence.keys.empty())
  {
    kind = "in";
  }
  else
  {
    kind = sentence.keys.front();
  }
  auto name = sentence.name;
  auto type = sentence.what;
  if (type.empty())
  {
    if (kind != "->")
    {
      type = "_T" + std::to_string(pos);
    }
  }
  param.kind = kind;
  param.name = name;
  param.type = type;
  return param;
}

ParamInfo_0 to_lambda_param(front::Sentence_0 const &sentence)
{
  ParamInfo_0 param;
  auto kind = std::string();
  if (not sentence.keys.empty())
  {
    kind = sentence.keys.front();
  }
  auto name = sentence.name;
  auto type = sentence.what;
  if (type.empty())
  {
    if (kind != "->")
    {
      type = "auto";
    }
  }
  param.kind = kind;
  param.name = name;
  param.type = type;
  return param;
}

std::string to_code(ParamInfo_0 const &param)
{
  auto kind = param.kind;
  auto type = param.type;
  if (kind == "in") {
    if (little_keywords.find(type) == little_keywords.end()) {
      type.append(" const&");
    }
  } else if (kind == "out") {
    type.append("&");
  } else if (kind == "inout") {
    type.append("&");
  } else if (kind == "move") {
    type.append("&&");
  } else if (kind == "forward") {
    type.append("&&");
  } else if (kind == "ptr") {
    type.append("*");
  } else if (kind == "") {
    type = "auto";
  }
  return type + " " + param.name;
}

std::string get_template_head(std::vector<ParamInfo_0> const &params)
{
  auto template_params = std::vector<std::string>();
  auto template_param = std::string("typename ");
  for (auto const &param : params)
  {
    if (param.type.find_first_of("_T") == 0)
    {
      template_params.push_back(template_param + param.type);
    }
  }
  if (template_params.empty())
  {
    return "";
  }
  return "template<" + front::join(template_params, ", ") + ">";
}

std::string get_return_part(std::vector<ParamInfo_0> const &params)
{
  for (auto const &param : params)
  {
    if (param.kind == "->")
    {
      auto type = param.type;
      if (param.type.empty())
      {
        type = "auto";
      }
      return param.kind + type;
    }
  }
  return "->auto";
}

std::string get_head_part(front::Sentence_0 const &fun_sentence)
{
  if (fun_sentence.name.empty())
  {
    return "[]";
  }
  return "auto " + fun_sentence.name;
}

std::vector<ParamInfo_0> filter_params(std::vector<ParamInfo_0> &&params, std::function<bool(ParamInfo_0 const &)> pred)
{
  auto end = std::remove_if(std::begin(params), std::end(params), pred);
  params.erase(end, std::end(params));
  return  params;
}

bool is_return_param(ParamInfo_0 const &param)
{
  if (param.kind == "->")
  {
    return true;
  }
  return false;
}

std::string translate_lambda(front::Sentence_0 const &sentence)
{
  auto param_str_s = front::split(front::sub_by_index(sentence.what, 1, sentence.what.size() - 1), ",");
  auto param_sentence_s = std::vector<front::Sentence_0>(param_str_s.size());
  std::transform(std::begin(param_str_s), std::end(param_str_s), std::begin(param_sentence_s), front::to_sentence);
  auto params = std::vector<ParamInfo_0>(param_sentence_s.size());
  std::transform(std::begin(param_sentence_s), std::end(param_sentence_s), std::begin(params), to_lambda_param);
  auto return_part = get_return_part(params);
  auto head_part = get_head_part(sentence);
  params = filter_params(std::move(params), is_return_param);
  auto param_code_s = std::vector<std::string>(params.size());
  std::transform(std::begin(params), std::end(params), std::begin(param_code_s), (std::string(*)(ParamInfo_0 const &))to_code);
  return head_part + "(" + front::join(param_code_s, ", ") + ")" + return_part + sentence.body;
}

std::string translate_normal_fun(front::Sentence_0 const &sentence)
{
  auto param_str_s = front::split(front::sub_by_index(sentence.what, 1, sentence.what.size() - 1), ",");
  auto param_sentence_s = std::vector<front::Sentence_0>(param_str_s.size());
  std::transform(std::begin(param_str_s), std::end(param_str_s), std::begin(param_sentence_s), front::to_sentence);
  auto params = std::vector<ParamInfo_0>(param_sentence_s.size());
  auto pos = 0;
  auto f = [&pos](auto const & sentence) {
    return to_param(sentence, pos++);
  };
  std::transform(std::begin(param_sentence_s), std::end(param_sentence_s), std::begin(params), f);
  auto return_part = get_return_part(params);
  auto head_part = get_head_part(sentence);
  params = filter_params(std::move(params), is_return_param);
  auto template_part = get_template_head(params);
  auto param_code_s = std::vector<std::string>(params.size());
  std::transform(std::begin(params), std::end(params), std::begin(param_code_s), (std::string(*)(ParamInfo_0 const &))to_code);
  return template_part + head_part + "(" + front::join(param_code_s, ", ") + ")" + return_part + sentence.body;
}
//
std::string translate(std::string const &head)
{
  auto sentence = front::to_sentence(head);
  bool is_declare = false;
  for (auto sep : sentence.separators)
  {
    if (sep == ":")
    {
      is_declare = true;
      break;
    }
  }
  if (is_declare)
  {
    auto d =  front::to_declare(front::get_sentence_content(head));
    auto what = front::declare::to_what_str(head);
    if (what == "var")
    {
      return front::declare::to_var_str(d);
    }
    else if (what == "fun")
    {
      // is normal fun
      return translate_normal_fun(sentence);
    }
    else if (what == "lambda")
    {
      //is lambda
      return translate_lambda(sentence);
    }
  }
  return "error";
}

}

}

std::string translate_from_line(std::string const &line)
{
  auto content = front::get_sentence_content(line);
  bool has_lang = false;
  for (auto const &word : front::split(content, " "))
  {
    if (front::lang_order::lang_op_contains(word))
    {
      has_lang = true;
      break;
    }
  }
  if (has_lang)
  {
    return front::lang_order::from_lang(content);
  }
  auto sentence = front::to_sentence_new_version(line);
  bool is_declare = false;
  bool has_body_sep = false;
  for (auto sep : sentence.separators)
  {
    if (sep == ":")
    {
      is_declare = true;
    }
    if (sep == "=")
    {
      has_body_sep = true;
    }
  }
  if (is_declare)
  {
    auto d =  front::to_declare(front::get_sentence_content(line));
    auto what = front::declare::to_what_str(line);
    if (what == "var")
    {
      return front::declare::to_var_str(d);
    }
    else if (what == "fun")
    {
      // is normal fun
      return param::_detail::translate_normal_fun(sentence);
    }
    else if (what == "lambda")
    {
      //is lambda
      return param::_detail::translate_lambda(sentence);
    }
  }
  if (has_body_sep)
  {
    return front::stl::from_transform_filter_fold(sentence);
  }
  return front::pipe::from_pipe_order(sentence.body);
//  return std::string("error");
}

void test_pipe_order()
{
  auto s = std::string("x|round|add_2|map");
  std::cout << front::pipe::from_pipe_order(s) << std::endl;
}

void test_var()
{
  auto declare_i_int = std::string("i:int = 10;");
  auto declare_v_no = std::string("v:= std::vector<int>{0, 1, 2};");
  std::cout << front::declare::to_var_str(front::to_declare(front::get_sentence_content(declare_i_int))) << std::endl;
  std::cout << front::declare::to_var_str(front::to_declare(front::get_sentence_content(declare_v_no))) << std::endl;
}

void test_sentence_struct()
{
  auto declare_i_int = std::string("i:int = 10;");
  auto declare_v_no = std::string("v:= std::vector<int>{0, 1, 2};");
  auto declare_f_fun = std::string("f:(i:int, inout x:int, -> :int)");
  auto declare_lambda_no = std::string(":(x)={};");
  auto declare_lambda_in = std::string(":(in x)={};");
  auto declare_lambda_inout = std::string(":(inout x)={};");
  // 暂时不行
  auto lang_is_3 = std::string("x is 3");
  auto lang_is_cstring = std::string("x is \"ee\"");
  auto lang_is_not_3 = std::string("x is not 3");
  auto lang_is_not_cstring = std::string("x is not \"ee\"");
  auto lang_is_empty = std::string("x is empty");
  auto lang_is_not_empty = std::string("x is not empty");
  auto lang_as_string = std::string("x as std::string");
  auto lang_to_string = std::string("x to std::string");

  std::cout << front::to_str(front::to_sentence(declare_i_int)) << std::endl;
  std::cout << front::to_str(front::to_sentence(declare_v_no)) << std::endl;

  std::cout << front::to_str(front::to_sentence(declare_f_fun)) << std::endl;

  std::cout << front::to_str(front::to_sentence(declare_lambda_no)) << std::endl;
  std::cout << front::to_str(front::to_sentence(declare_lambda_in)) << std::endl;
  std::cout << front::to_str(front::to_sentence(declare_lambda_inout)) << std::endl;
}

void test_fun()
{
  auto declare_fun_int_inout = std::string("f:(i:int, inout x:int, -> :int)");
  auto declare_fun_string = std::string("f:(i:std::string, inout x:int)");
  auto declare_fun_no_inout = std::string("f:(i, inout x, ->)");
  front::println(param::_detail::translate(declare_fun_int_inout));
  front::println(param::_detail::translate(declare_fun_string));
  front::println(param::_detail::translate(declare_fun_no_inout));

}

void test_lambda()
{

  auto declare_lambda_no = std::string(":(x)={};");
  auto declare_lambda_in = std::string(":(in x)={};");
  auto declare_lambda_inout = std::string(":(inout x)={};");
  front::println(param::_detail::translate(declare_lambda_in));
  front::println(param::_detail::translate(declare_lambda_inout));
}

void test_lang_order()
{
  // lang
  auto lang_is_3 = std::string("x is 3");
  auto lang_is_cstring = std::string("x is \"ee\"");
  auto lang_is_not_3 = std::string("x is not 3");
  auto lang_is_not_cstring = std::string("x is not \"ee\"");
  auto lang_is_empty = std::string("x is empty");
  auto lang_is_not_empty = std::string("x is not empty");
  auto lang_as_string = std::string("x as std::string");
  auto lang_to_string = std::string("x to std::string");
  std::cout << front::lang_order::from_lang(lang_is_3) << std::endl;
  std::cout << front::lang_order::from_lang(lang_is_not_3) << std::endl;
  std::cout << front::lang_order::from_lang(lang_is_cstring) << std::endl;
  std::cout << front::lang_order::from_lang(lang_is_not_cstring) << std::endl;
  std::cout << front::lang_order::from_lang(lang_is_empty) << std::endl;
  std::cout << front::lang_order::from_lang(lang_is_not_empty) << std::endl;
  std::cout << front::lang_order::from_lang(lang_as_string) << std::endl;
  std::cout << front::lang_order::from_lang(lang_to_string) << std::endl;
}

void test_fun_stl()
{
//  res = xs | transform(f); => std::transform(std::begin(xs), std::end(xs), std::begin(res), f);
//  res = xs | filter(f) => for (const auto &x : xs) { if (f(x) {res.push_back(x);} }
//  res = xs | foldl(f, 0) => res = std::accumulate(std::begin(xs), std::end(xs), 0, f);
//  res = xs | foldr(f, 0) => res = std::accumulate(std::rbegin(xs), std::rend(xs), 0, f);
  auto stl_transform = std::string("res = xs | transform(f);");
  auto stl_filter = std::string("res = xs | filter(f);");
  auto stl_foldl = std::string("res = xs | foldl(f, 0);");
  auto stl_foldr = std::string("res = xs | foldr(f, 0);");

  std::cout << front::to_str(front::to_sentence(stl_transform)) << std::endl;
  std::cout << front::to_str(front::to_sentence(stl_filter)) << std::endl;
  std::cout << front::to_str(front::to_sentence(stl_foldl)) << std::endl;
  std::cout << front::to_str(front::to_sentence(stl_foldr)) << std::endl;
  std::cout << front::stl::from_transform_filter_fold(front::to_sentence(stl_transform))
            << std::endl
            << front::stl::from_transform_filter_fold(front::to_sentence(stl_filter))
            << std::endl
            << front::stl::from_transform_filter_fold(front::to_sentence(stl_foldl))
            << std::endl
            << front::stl::from_transform_filter_fold(front::to_sentence(stl_foldr))
            << std::endl;

}

void test_various_string()
{

//1 i:int = 10; v:= std::vector<int>(10);
  auto declare_i = std::string("i:int = 10;");
  assert(translate_from_line(declare_i) == "int i = 10");
  auto declare_v = std::string("v:= std::vector<int>(10);");
  assert(translate_from_line(declare_v) == "auto v = std::vector<int>(10)");
//2 f:(i:int, y, ->:int)
  auto declare_fun_f = std::string("f:(i:int, y, ->:int)");
  assert(translate_from_line(declare_fun_f) == "template<typename _T1>auto f(int i, _T1 const& y)->int");
//3 :(x)={}
  auto declare_lambda = std::string(":(x)={}");
  assert(translate_from_line(declare_lambda) == "[](auto x)->auto{}");
//4 x|add_2|less_0_become_0|to_str|add_prefix;
  auto fun_pipe = std::string("x|add_2|less_0_become_0|to_str|add_prefix;");
  assert(translate_from_line(fun_pipe) == "add_prefix(to_str(less_0_become_0(add_2(x))))");
//5 text is empty;
//6 i as std::string;
//7 utf8_str to gpk;
  auto lang_order_is = std::string("text is empty;");
  auto lang_order_as = std::string("i as std::string;");
  auto lang_order_to = std::string("vec to std::string");
  assert(translate_from_line(lang_order_is) == "empty(text)");
//  front::println(translate_from_line(lang_order_as));
  assert(translate_from_line(lang_order_as) == "as_<std::string>(i)");
  assert(translate_from_line(lang_order_to) == "to_<std::string>(std::move(vec))");
//8 res = xs | transform(f); => std::transform(std::begin(xs), std::end(xs), std::begin(res), f);
//9 res = xs | filter(f) => for(const auto& x:xs){ if(f(x){res.push_back(x);} }
//10 res = xs | foldr(f, 0) => res = std::accumulate(std::rbegin(xs), std::rend(xs), 0, f);
  auto stl_transform = std::string("res = xs | transform(f);");
  auto stl_filter = std::string("res = xs | filter(f);");
  auto stl_foldl = std::string("res = xs | foldl(f, 0);");
  auto stl_foldr = std::string("res = xs | foldr(f, 0);");
  assert(translate_from_line(stl_transform) == "std::transform(std::begin(xs), std::end(xs), std::begin(res), f)");
  assert(translate_from_line(stl_filter) == "for(auto& const item:xs){ if(f(item)){res.push_back(item);} }");
  assert(translate_from_line(stl_foldl) == "res = std::accumulate(std::begin(xs), std::end(xs), 0, f)");
  assert(translate_from_line(stl_foldr) == "res = std::accumulate(std::rbegin(xs), std::rend(xs), 0, f)");

}

void test()
{
//  test_lang_order();
//  test_sentence_struct();
//  test_fun_stl();
//  test_fun();
//  test_lambda();
  test_various_string();
}


CodeFrontControllWidget::CodeFrontControllWidget(QWidget *parent)
  : QWidget(parent)

{
  test();
}

CodeFrontControllWidget::~CodeFrontControllWidget()
{
}

QString CodeFrontControllWidget::translate_line(QString const &line)
{
  auto s = translate_from_line(line.toStdString());
  return QString::fromStdString(s);
}
