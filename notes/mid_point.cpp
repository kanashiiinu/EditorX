#include "heads.hpp"

/**
 * @brief get mid point of two integer
 * @param[in] lhs
 * @param[in] rhs
 * @return mid point of lhs and rhs
 * @exception noexcept
 */
[[nodiscard]] inline auto mid_point_of_integer(int lhs, int rhs) noexcept -> int {
    return ((lhs bitand rhs) + ((lhs xor rhs) >> 1));
}