#include "hyperparameter.h"

[[gnu::always_inline]] inline f64 _g(f64 x) {
  f64 Q = std::exp(beta * (x - 1));
  return Q;
}

inline A100 next_generationa(A10 a) {
  auto g = [&a](f64 x) -> f64 {
    if (x == 1.)
      return 1;
    const auto n = static_cast<usize>(x / step);
    const f64 res = x - n * step;
    const f64 h = a[n + 1] - a[n];
    const f64 delta = h * res / step;
    return a[n] + delta;
  };
  A100 ans{};
  f64 next_step = 0.01;
  int i = 0;
  for (f64 x = 0; x <= 1 - tol; x += next_step) {
    ans[i++] = g(x);
  }
  ans[100] = 1;
  return ans;
}
