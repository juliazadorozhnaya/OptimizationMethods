#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using base_type = double;
using solver_result = std::pair<base_type, std::vector<base_type>>;
using matrix3d = std::vector<std::vector<std::vector<base_type>>>;
using ivector = std::vector<base_type>;
using ivector2d = std::vector<std::vector<base_type>>;
using ipair = std::pair<base_type, base_type>;
using vec_p_int_vec = std::vector<std::pair<base_type, ivector>>;
using vec_pairs = std::vector<std::pair<base_type, base_type>>;

vec_p_int_vec enumerate(ivector2d a);
vec_pairs enumerate(ivector a);

//#define DEBUG

inline void log(std::string s) {
#ifdef DEBUG
  std::cerr << s << std::endl;
#endif // DEBUG
}

class ListSolver {
public:
  ListSolver(int n, int C) : n(n), C(C){/*log("solver inited");*/};

  solver_result solve(ivector weight, ivector price);

private:
  int n, C;
};

std::vector<ipair> zip(ivector a, ivector b) {
  std::vector<ipair> target;
  target.reserve(a.size());
  std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(target),
                 [](base_type a, base_type b) { return std::make_pair(a, b); });
  return target;
}

vec_p_int_vec enumerate(ivector2d a) {
  vec_p_int_vec target;
  // ivector b(a.size());

  // std::iota(b.begin(), b.end(), 0);

  // target.reserve(a.size());
  // std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(target),
  //            [](int a, int b) { return std::make_pair(a, b); });
  int ind = 0;

  for (auto i : a) {
    target.push_back({ind, i});
    ind++;
  }

  return target;
}

vec_pairs enumerate(ivector a) {
  vec_pairs target;
  ivector b(a.size());

  std::iota(b.begin(), b.end(), 0);

  target.reserve(a.size());
  std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(target),
                 [](base_type a, base_type b) { return std::make_pair(b, a); });
  return target;
}

// ivector max(ivector2d a, ivector2d b) {
//     int size = a.size() > b.size() ? b.size() : a.size();

//     for(int i = 0; i < size; i++) {
//         for(int j = 0; j < size; j++)
//             if(a[i][j] > b[i][j])
//                 return a;
//     }

//     return b;
// }

ivector max(ivector2d a, std::function<base_type(ivector)> f) {
  // log("max begin");
  int size = a.size();
  ivector ind;

  for (auto i : a) {
    auto b = f(i);
    ind = i;

    for (auto j : i) {
      if (j > b)
        return i;
    }
  }

  // log("max end");
  return ind;
}

ivector max_last_val(ivector2d a, int mind) {
  double m = a[0][mind];
  int ind = 0;

  for (int i = 0; i < a.size(); i++) {
    double t = a[i][mind];

    if (t > m) {
      m = t;
      ind = i;
    }
  }

  return a[ind];
}

solver_result ListSolver::solve(ivector weights, ivector price) {
  // log("solver::solver begin");
  matrix3d layers = {{{0, 0, 0, 0}}};

  for (auto wp : zip(weights, price)) {
    // log("for zip begin");
    auto weight = wp.first;
    auto p = wp.second;

    ivector2d new_layer, push_layer;

    int i = 0, j = 0;

    for (auto idxs : enumerate(layers[layers.size() - 1])) {
      // log("for enumerate begin");
      auto idx = idxs.first;
      auto solution = idxs.second;

      ivector sol{1, idx, solution[2] + weight, solution[3] + p};

      push_layer.push_back(sol);
      // std::cout << "sol: ";

      // for(auto i : sol)
      //     std::cout << "(" << i << ", ";
      // std::cout << std::endl;
    }

    while ((i < layers[layers.size() - 1].size()) &&
           (j < layers[layers.size() - 1].size())) {
      // log("while begin");
      if (push_layer[j][2] > this->C)
        j += 1;
      else if ((layers[layers.size() - 1][i][2] <= push_layer[j][2]) &&
               (layers[layers.size() - 1][i][3] >= push_layer[j][3]))
        j += 1;
      else if ((layers[layers.size() - 1][i][2] >= push_layer[j][2]) &&
               (layers[layers.size() - 1][i][3] <= push_layer[j][3]))
        i += 1;
      else if (layers[layers.size() - 1][i][2] < push_layer[j][2]) {
        double t = (double)i;
        ivector tmp{0, t, layers[layers.size() - 1][i][2],
                    layers[layers.size() - 1][i][3]};
        new_layer.push_back(tmp);
        i += 1;
      } else {
        new_layer.push_back(push_layer[j]);
        j += 1;
      }
      // log("while end");
    }

    while (i < layers[layers.size() - 1].size()) {
      double t = (double)i;
      ivector tmp{0, t, layers[layers.size() - 1][i][2],
                  layers[layers.size() - 1][i][3]};
      new_layer.push_back(tmp);
      i += 1;
    }

    while (j < layers[layers.size() - 1].size()) {
      if (push_layer[j][2] <= this->C)
        new_layer.push_back(push_layer[j]);
      j += 1;
    }

    layers.push_back(new_layer);
    // log("for zip end");
    // log("layers: ");

    // for(auto i1 : layers) {
    //     std::cout << "[";
    //     for(auto j1 : i1) {
    //         std::cout << ", [";
    //         for(auto k1 : j1)
    //             std::cout << k1 << " ";
    //         std::cout << "]";
    //     }
    //     std::cout << "]";
    // }
    // std::cout << std::endl;
  }

  ivector x;
  // ivector best_solution = max(layers[layers.size()-1], [](ivector x){ return
  // x[3]; });
  ivector best_solution = max_last_val(layers[layers.size() - 1], 3);

  // for(auto i : best_solution)
  //     std::cout << "(" << i << ", ";
  // std::cout << std::endl;

  double prev_index = best_solution[1];

  if (best_solution[0])
    x.push_back(this->n - 1);

  for (int ii = this->n - 1; ii > 0; ii--) {
    // log("for ii begin");
    auto prev_solution = layers[ii][prev_index];
    if (prev_solution[0])
      x.push_back(ii - 1);
    prev_index = prev_solution[1];
    // log("for ii end");
  }

  // log("solver::solver end");
  return {best_solution[3], x};
}

double absolute(ivector D) {
  std::vector<base_type> D1(D.size());
  std::transform(D.begin(), D.end(), D1.begin(),
                 [](base_type x) { return x * x; });

  log(std::to_string(std::sqrt(std::accumulate(D1.begin(), D1.end(), 0.0))));

  return std::sqrt(std::accumulate(D1.begin(), D1.end(), 0.0));
}

ivector input_array_of_nums() {
  ivector numbers;
  std::string line;
  double number;

  std::getline(std::cin, line);
  std::istringstream stream(line);
  while (stream >> number)
    numbers.push_back(number);
  return numbers;
}

int main() {
  auto nm = input_array_of_nums();

  int n = nm[0];
  int m = nm[1];
  ivector c = input_array_of_nums();
  ivector prices = input_array_of_nums();

  ivector2d weights;

  for (int i = 0; i < m; i++)
    weights.push_back({});

  for (int i = 0; i < n; i++) {
    ivector w = input_array_of_nums();

    for (int j = 0; j < m; j++)
      weights[j].push_back(w[j]);
  }

  ivector l(m - 1, 1);

  ivector L;
  ivector x;

  for (int iter = 0; iter < 9100; iter++) {
    L.clear();

    for (int i = 0; i < n; i++) {
      L.push_back(prices[i]);

      for (int j = 0; j < m - 1; j++)
        L[L.size() - 1] -= weights[j][i] * l[j];
    }
    log("before solver");
    ListSolver solver(n, c[c.size() - 1]);
    auto bx = solver.solve(weights[weights.size() - 1], L);

    log("after solver");

    auto best = bx.first;
    auto x_indices = bx.second;
    L.push_back(0);
    for (int j = 0; j < m - 1; j++)
      L[L.size() - 1] += c[j] * l[j];

    x.clear();

    for (int i = 0; i < n; i++) {
      x.push_back(std::find(x_indices.begin(), x_indices.end(), i) !=
                  x_indices.end());
    }

    // std::cout << "x_ind: ";

    // for(auto i : x)
    //     std::cout << i << " ";
    // std::cout << std::endl;

    ivector D;
    for (int i = 0; i < m - 1; i++) {
      auto D_i = c[i];

      for (auto jxj : enumerate(x)) {
        auto j = jxj.first;
        auto x_j = jxj.second;
        // std::cout << "j: " << j << std::endl;
        // std::cout << "xj: " << x_j << std::endl;
        // std::cout << "weights: " << weights[i][j] << std::endl;
        D_i -= x_j * weights[i][j];
      }
      D.push_back(D_i);
    }

    for (int i = 0; i < m - 1; i++) {
      // std::cout << "D: ";
      // for(auto i : D)
      //     std::cout << i << " ";
      // std::cout << std::endl;

      if (absolute(D) > 0.0001)
        l[i] -= (D[i] / absolute(D)) / (iter + 1);
      l[i] = std::max({l[i], 0.0});
    }
  }

  double result = 0;
  for (int i = 0; i < n; i++)
    result += L[i] * x[i];
  result += L[L.size() - 1];
  std::cout << std::fixed << result << std::endl;
  for (int i = 0; i < m - 1; i++)
    std::cout << l[i] << std::endl;
}

