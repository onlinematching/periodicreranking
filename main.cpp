#include "hyperparameter.h"
#include <boost/math/quadrature/gauss_kronrod.hpp>
#include <boost/numeric/odeint.hpp>
#include <mutex>
#include <thread>
#include <type_traits>

f64 g(f64 t) {
  f64 Q = beta * std::exp(t - 1);
  return Q;
}

f64 G(f64 x) {
  f64 Q = boost::math::quadrature::gauss_kronrod<double, 15>::integrate(g, 0, x,
                                                                        5, tol);
  return Q;
}

f64 H(f64 z1, f64 z2, f64 y) {
  f64 a1 = G(z2) - G(z1);
  f64 a2 = (1 - g(y)) * (1 - z1);
  f64 a3 = (1 - z2) * (G(y) - G(0));
  f64 a4 = z1 * (1 - g(0));
  return a1 + a2 + a3 + a4;
}

template <typename F> f64 min_H(F H) {
  f64 min = std::numeric_limits<double>::max();
  f64 _z1, _z2, _y;
  for (f64 z1 = 0; z1 < 1; z1 += step) {
    std::cout << z1 << std::endl;
    for (f64 z2 = z1; z2 < 1; z2 += step) {
      for (f64 y = 0; y < 1; y += step) {
        f64 h = H(z1, z2, y);
        if (h < min) {
          min = h;
          _z1 = z1;
          _z2 = z2;
          _y = y;
          std::cout << "z1 = " << z1 << " ;z2 = " << z2 << " ;y =  " << y
                    << "; min = " << min << std::endl;
        }
      }
    }
  }
  return min;
}

int main(int argc, char *argv[]) {
  min_H(H);
  // std::cout << H(1, 0, 0) << std::endl;
  return 0;
}
