#include "hyperparameter.h"

template <class Axx> Axx _a_generator(const Axx &a) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<double> dist(-step, step);
  Axx a_next{};
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

inline A100 a_generator(const A100 &a) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::normal_distribution<f64> dist(0, step);
  static std::uniform_int_distribution<usize> dis_usize(0, 99);
  A100 a_next = a;
  usize i = dis_usize(gen);
  a_next[i] = a[i] + dist(gen);
  if (a_next[i] < 0) {
    a_next[i] = -a_next[i];
  }
  if (a_next[i] > 1) {
    a_next[i] = 2 - a_next[i];
  }
  std::sort(a_next.begin(), a_next.end());
  return a_next;
}

template <class Axx> void print(Axx const &a) {
  std::cout << "a = [";
  for (auto x : a) {
    std::cout << x << ", ";
  }
  std::cout << "]\n";
}

[[gnu::always_inline]] inline f64 _g(f64 x) {
  f64 Q = std::exp(beta * (x - 1));
  return Q;
}

inline f64 array_g(f64 x) {
  auto a = a_end_10;
  auto g = [&a](f64 x) -> f64 {
    if (x == 1.)
      return 1;
    const auto n = static_cast<usize>(x / step);
    const f64 res = x - n * step;
    const f64 h = a[n + 1] - a[n];
    const f64 delta = h * res / step;
    return a[n] + delta;
  };
  return g(x);
}

template <class G> inline A100 next_generationa(G g) {
  A100 ans{};
  f64 next_step = 0.01;
  int i = 0;
  for (f64 x = 0; x <= 1 - tol; x += next_step) {
    ans[i++] = g(x);
  }
  ans[100] = 1;
  return ans;
}
