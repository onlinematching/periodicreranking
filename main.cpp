#include "hyperparameter.h"
#include "util.hpp"
#include <bits/ranges_algo.h>
#include <boost/math/quadrature/gauss_kronrod.hpp>
#include <boost/numeric/odeint.hpp>

template <typename F1> f64 quick_min_H(F1 H, bool show_detail = false) {
  f64 min = std::numeric_limits<double>::max();
  f64 z1 = 0;
  for (f64 z2 = z1; z2 < 1 + tol; z2 += precise) {
    if (show_detail) {
      std::cout << "z2 = " << z2 << std::endl;
    }
    for (f64 y = 0; y < 1 + tol; y += precise) {
      f64 h = H(z1, z2, y);
      if (h < min) {
        min = h;
        if (show_detail) {
          std::cout << "z1 = " << z1 << " ;z2 = " << z2 << " ;y =  " << y
                    << "; min = " << min << std::endl;
        }
      }
    }
  }
  return min;
}

template <typename F1> f64 min_H(F1 H, bool show_detail = false) {
  f64 min = std::numeric_limits<double>::max();
  f64 z1 = 0;
  for (f64 z1 = 0; z1 < 1 + tol; z1 += step) {
    if (show_detail) {
      std::cout << z1 << std::endl;
    }
    for (f64 z2 = z1; z2 < 1 + tol; z2 += precise) {
      for (f64 y = 0; y < 1 + tol; y += precise) {
        f64 h = H(z1, z2, y);
        if (h < min) {
          min = h;
          if (show_detail) {
            std::cout << "z1 = " << z1 << " ;z2 = " << z2 << " ;y =  " << y
                      << "; min = " << min << std::endl;
          }
        }
      }
    }
  }
  return min;
}

template <class F>
f64 inner_calculater(F g, bool show_detail = false, bool use_quick = false) {
  auto G = [&g](f64 x) -> f64 {
    return boost::math::quadrature::gauss_kronrod<double, 15>::integrate(
        g, 0, x, 9, tol);
  };
  auto H = [&G, &g](f64 z1, f64 z2, f64 x) -> f64 {
    f64 a1 = G(z2) - G(z1);
    f64 a2 = (1 - g(x)) * (1 - z1);
    f64 a3 = (1 - z2) * (G(x) - G(0));
    f64 a4 = z1 * (1 - g(0));
    return a1 + a2 + a3 + a4;
  };
  if (use_quick) {
    return quick_min_H(H, show_detail);
  } else {
    return min_H(H, show_detail);
  }
}

f64 calculater(const A100 &a, bool show_detail = false,
               bool use_quick = false) {
  auto g = [&a](f64 x) -> f64 {
    if (x >= 1.)
      return 1;
    const auto n = static_cast<usize>(x / step);
    const f64 res = x - n * step;
    const f64 h = a[n + 1] - a[n];
    const f64 delta = h * res / step;
    return a[n] + delta;
  };
  return inner_calculater(g, show_detail, use_quick);
}

int main(int argc, char *argv[]) {
  // print(next_generationa(a_end_10));
  // A100 a = g_pre_100;
  A100 a = a_mid_100;
  f64 max = calculater(a, true, false);
  std::cout << max << std::endl;
  using namespace std::chrono_literals;
  std::mutex mutex{};
  usize n_core =
      std::max(std::thread::hardware_concurrency() - 1, static_cast<usize>(1));
  std::cout << "core num: " << n_core << std::endl;
  std::this_thread::sleep_for(3000ms);

  for (usize i = 0;;) {
    std::vector<std::thread> threads{};
    for (usize j = 0; j < n_core; j++) {
      threads.push_back(std::thread([&]() {
        auto a_next = a_generator(a);
        auto max_now = calculater(a_next, false, true);

        mutex.lock();
        i++;
        if (i % 1000 == 0) {
          std::cout << "i = " << i << std::endl;
        }
        if (max_now > max) {
          max = max_now;
          std::cout << "i = " << i << std::endl;
          std::cout << "max = " << max << std::endl;
          print(a_next);
          std::cout << "-----------------------------" << std::endl;
          a = a_next;
        }
        mutex.unlock();
      }));
    }
    for (auto &thread : threads) {
      thread.join();
    }
  }
  //
  return 0;
}
