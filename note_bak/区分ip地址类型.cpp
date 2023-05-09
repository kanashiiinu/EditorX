#include "../head.hpp"
using namespace std;

auto solve(std::string IP)->std::string {
    //正则表达式限制0-255 且没有前缀0 四组齐全
    auto ipv4 = std::regex(R"((([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]))");
    //正则表达式限制出现8组，0-9a-fA-F的数，个数必须是1-4个
    auto ipv6 = std::regex(R"(([0-9a-fA-F]{1,4}\:){7}[0-9a-fA-F]{1,4})");
    //调用正则匹配函数
    if      (regex_match(IP, ipv4)) return "IPv4";
    else if (regex_match(IP, ipv6)) return "IPv6";
    else                            return "Neither";
}