#include <tuple>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

using f64 = double;
using usize = std::uint32_t;
using i64 = std::int64_t;
using i32 = std::int32_t;

constexpr f64 pi = 3.141592653589793;

constexpr f64 tol = 1e-9;
constexpr f64 step = 0.00001;
constexpr f64 beta = 0.89;
