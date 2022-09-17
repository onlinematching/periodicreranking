#pragma once
#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <mutex>
#include <random>
#include <thread>
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
constexpr f64 step = 0.01;
constexpr f64 precise = 0.01;
constexpr f64 beta = 0.8899578581408;

constexpr const A10 a_init_10 = {0.,  0.1, 0.2, 0.3, 0.4, 0.5,
                                 0.6, 0.7, 0.8, 0.9, 1.};

constexpr const A10 a_end_10 = {0.377164, 0.449303, 0.487346, 0.546791,
                                0.606325, 0.640417, 0.729627, 0.740469,
                                0.877023, 0.911392, 1};

constexpr const A100 a_init_100 = {
    0.377164, 0.384378, 0.391592, 0.398806, 0.40602,  0.413234, 0.420447,
    0.427661, 0.434875, 0.442089, 0.449303, 0.453107, 0.456912, 0.460716,
    0.46452,  0.468324, 0.472129, 0.475933, 0.479737, 0.483542, 0.487346,
    0.493291, 0.499235, 0.50518,  0.511124, 0.517069, 0.523013, 0.528958,
    0.534902, 0.540847, 0.546791, 0.552744, 0.558698, 0.564651, 0.570605,
    0.576558, 0.582511, 0.588465, 0.594418, 0.600372, 0.606325, 0.609734,
    0.613143, 0.616553, 0.619962, 0.623371, 0.62678,  0.630189, 0.633599,
    0.637008, 0.640417, 0.649338, 0.658259, 0.66718,  0.676101, 0.685022,
    0.693943, 0.702864, 0.711785, 0.720706, 0.729627, 0.730711, 0.731795,
    0.73288,  0.733964, 0.735048, 0.736132, 0.737216, 0.738301, 0.739385,
    0.740469, 0.754124, 0.76778,  0.781435, 0.795091, 0.808746, 0.822401,
    0.836057, 0.849712, 0.863368, 0.877023, 0.88046,  0.883897, 0.887334,
    0.890771, 0.894208, 0.897644, 0.901081, 0.904518, 0.907955, 0.911392,
    0.920253, 0.929114, 0.937974, 0.946835, 0.955696, 0.964557, 0.973418,
    0.982278, 0.991139, 1,
};

// just g(x) = exp(beta * (x - 1))
constexpr const A100 g_pre_100 = {
    0.41067306, 0.41434419, 0.41804813, 0.42178519, 0.42555565, 0.42935982,
    0.433198,   0.43707048, 0.44097759, 0.44491962, 0.44889689, 0.45290971,
    0.4569584,  0.46104329, 0.46516469, 0.46932294, 0.47351836, 0.47775128,
    0.48202204, 0.48633098, 0.49067843, 0.49506475, 0.49949029, 0.50395538,
    0.50846038, 0.51300566, 0.51759157, 0.52221848, 0.52688675, 0.53159674,
    0.53634884, 0.54114343, 0.54598087, 0.55086155, 0.55578587, 0.5607542,
    0.56576695, 0.57082451, 0.57592728, 0.58107567, 0.58627008, 0.59151092,
    0.59679861, 0.60213357, 0.60751622, 0.61294699, 0.61842631, 0.6239546,
    0.62953232, 0.6351599,  0.64083778, 0.64656642, 0.65234627, 0.65817779,
    0.66406143, 0.66999768, 0.67598698, 0.68202983, 0.6881267,  0.69427807,
    0.70048443, 0.70674627, 0.71306408, 0.71943837, 0.72586965, 0.73235841,
    0.73890518, 0.74551048, 0.75217482, 0.75889873, 0.76568275, 0.77252742,
    0.77943327, 0.78640086, 0.79343073, 0.80052344, 0.80767956, 0.81489965,
    0.82218428, 0.82953403, 0.83694948, 0.84443122, 0.85197984, 0.85959594,
    0.86728013, 0.875033,   0.88285518, 0.89074729, 0.89870994, 0.90674378,
    0.91484943, 0.92302754, 0.93127876, 0.93960374, 0.94800313, 0.95647762,
    0.96502785, 0.97365452, 0.98235831, 0.99113991, 1.,
};

constexpr const A100 a_mid_100 = {
    0.403797, 0.408024, 0.411692, 0.416276, 0.420323, 0.423678, 0.428883,
    0.433178, 0.437376, 0.442117, 0.446102, 0.451144, 0.451277, 0.45886,
    0.461218, 0.468726, 0.473995, 0.477553, 0.482967, 0.483977, 0.49125,
    0.498379, 0.500799, 0.50194,  0.51052,  0.515537, 0.521523, 0.52505,
    0.530703, 0.533025, 0.541339, 0.541741, 0.546448, 0.547283, 0.564123,
    0.567571, 0.577361, 0.582446, 0.5843,   0.5939,   0.600217, 0.603182,
    0.610576, 0.611508, 0.612136, 0.631283, 0.637618, 0.642159, 0.649475,
    0.651664, 0.664536, 0.666588, 0.676839, 0.677992, 0.6815,   0.691204,
    0.691335, 0.698681, 0.705824, 0.706725, 0.724096, 0.727,    0.729484,
    0.729779, 0.735242, 0.736664, 0.750936, 0.753874, 0.75409,  0.762854,
    0.765015, 0.77463,  0.77979,  0.78652,  0.808739, 0.817688, 0.820806,
    0.823062, 0.849944, 0.851998, 0.853955, 0.861481, 0.87099,  0.891242,
    0.893837, 0.907998, 0.908081, 0.917678, 0.92594,  0.927102, 0.928545,
    0.946424, 0.949704, 0.950383, 0.951094, 0.974032, 0.97574,  0.976315,
    0.981147, 0.994785, 1,
};
