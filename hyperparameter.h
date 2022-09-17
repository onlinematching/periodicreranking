#pragma once
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>

using f64 = double;
using usize = std::uint32_t;
using i64 = std::int64_t;
using i32 = std::int32_t;
using A10 = std::array<f64, 11>;
using A100 = std::array<f64, 101>;
using A1000 = std::array<f64, 1001>;

constexpr f64 pi = 3.141592653589793;

constexpr f64 tol = 1e-9;
constexpr f64 step = 0.1;
constexpr f64 beta = 0.89;

constexpr const A10 a_init_10 = {
    0. , 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1. 
};

constexpr const A10 a_end_10 = {
    0.377164, 0.449303, 0.487346, 0.546791, 0.606325, 0.640417, 0.729627, 0.740469, 0.877023, 0.911392, 1
};
