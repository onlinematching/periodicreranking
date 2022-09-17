#include "hyperparameter.h"
#include <boost/math/quadrature/gauss_kronrod.hpp>
#include <boost/numeric/odeint.hpp>
#include <mutex>
#include <thread>
#include <type_traits>

f64 _g(f64 x) {
  f64 Q = std::exp(beta * (x - 1));
  return Q;
}

template <typename F1> f64 min_H(F1 H) {
  f64 min = std::numeric_limits<double>::max();
  for (f64 z1 = 0; z1 < 1; z1 += step) {
    std::cout << z1 << std::endl;
    for (f64 z2 = z1; z2 < 1; z2 += step) {
      for (f64 y = 0; y < 1; y += step) {
        f64 h = H(z1, z2, y);
        if (h < min) {
          min = h;
          std::cout << "z1 = " << z1 << " ;z2 = " << z2 << " ;y =  " << y
                    << "; min = " << min << std::endl;
        }
      }
    }
  }
  return min;
}

void g_generator(const A &a) {
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
  std::cout << minnow << std::endl;
}

int main(int argc, char *argv[]) {
  // min_H(H);
  A a = a_init;
  g_generator(a);
  return 0;
}
