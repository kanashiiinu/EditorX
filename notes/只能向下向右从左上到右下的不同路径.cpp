#include "../head.hpp"
using namespace std;

auto unique_paths(int m, int n)-> int {
    auto dp = std::vector<std::vector<int>>(m, std::vector<int>(n));
    // init 第1行、第1列
    // std::fill(std::begin(dp.front()), std::end(dp.front()), 1);
    for (auto & num : dp.front()) { num = 1; }
    for (auto & row : dp) { row.front() = 1; }
    // 动态规划函数，递推求解
    for (size_t i = 1; i < m; ++i) {
        for (size_t j = 1; j < n; ++j) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    return dp.back().back();
}