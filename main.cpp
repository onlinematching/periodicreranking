#include "util.hpp"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <boost/math/quadrature/gauss_kronrod.hpp>
#include <boost/numeric/odeint.hpp>
#include <limits>
#include <mutex>
#include <random>
#include <thread>
#include <type_traits>

template <typename F1> f64 min_H(F1 H) {
  f64 min = std::numeric_limits<double>::max();
  for (f64 z1 = 0; z1 < 1 + tol; z1 += step) {
    // std::cout << z1 << std::endl;
    for (f64 z2 = z1; z2 < 1 + tol; z2 += step) {
      for (f64 y = 0; y < 1 + tol; y += step) {
        f64 h = H(z1, z2, y);
        if (h < min) {
          min = h;
          // std::cout << "z1 = " << z1 << " ;z2 = " << z2 << " ;y =  " << y
          //           << "; min = " << min << std::endl;
        }
      }
    }
  }
  return min;
}

f64 calculater(const A10 &a) {
  auto g = [&a](f64 x) -> f64 {
    if (x == 1.)
      return 1;
    const auto n = static_cast<usize>(x / step);
    const f64 res = x - n * step;
    const f64 h = a[n + 1] - a[n];
    const f64 delta = h * res / step;
    return a[n] + delta;
  };
  auto G = [&g](f64 x) -> f64 {
    return boost::math::quadrature::gauss_kronrod<double, 23>::integrate(
        g, 0, x, 9, tol);
  };
  auto H = [&G, &g](f64 z1, f64 z2, f64 x) -> f64 {
    f64 a1 = G(z2) - G(z1);
    f64 a2 = (1 - g(x)) * (1 - z1);
    f64 a3 = (1 - z2) * (G(x) - G(0));
    f64 a4 = z1 * (1 - g(0));
    return a1 + a2 + a3 + a4;
  };
  auto minnow = min_H(H);
  return minnow;
}

template<class Axx>
void print(Axx const &a) {
  std::cout << "a = [";
  for (auto x : a) {
    std::cout << x << ", ";
  }
  std::cout << "]\n";
}

A10 a_generator(const A10 &a) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<double> dist(-step, step);
  A10 a_next{};
  a_next[a.size() - 1] = 1.;
  for (usize i = 0; i < a.size() - 1; ++i) {
    a_next[i] = a[i] + dist(gen);
    if (a_next[i] < 0) {
      a_next[i] = -a_next[i];
    }
    if (a_next[i] > 1) {
      a_next[i] = 2 - a_next[i];
    }
  }
  std::sort(a_next.begin(), a_next.end());
  return a_next;
}

int main(int argc, char *argv[]) {
  print(next_generationa(a_end_10));
  return 0;
  A10 a = a_init_10;
  f64 max = std::numeric_limits<double>::min();
  for (usize i = 0;; i++) {
    if (i % 2000 == 0) {
      std::cout << i << std::endl;
    }

    auto a_next = a_generator(a);
    auto max_now = calculater(a_next);
    if (max_now > max) {
      max = max_now;
      std::cout << i << std::endl;
      std::cout << max << std::endl;
      print(a_next);
      std::cout << "-----------------------------" << std::endl;
      a = a_next;
    }
  }
  //
  return 0;
}
