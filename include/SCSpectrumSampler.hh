#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>

class SpectrumSampler {
public:
  // Pass in energies (MeV) and weights w ~ pdf(E) at the same points.
  // E must be strictly increasing; w >= 0.
  SpectrumSampler(std::vector<double> E, std::vector<double> w)
  : E_(std::move(E)), w_(std::move(w)) {
    if (E_.size() < 2 || E_.size() != w_.size())
      throw std::runtime_error("SpectrumSampler: bad input sizes");
    for (size_t i=1;i<E_.size();++i)
      if (!(E_[i] > E_[i-1])) throw std::runtime_error("E must be strictly increasing");
    buildCDF();
  }

  // u in [0,1) -> sample energy (MeV)
  double sample(double u) const {
    if (u <= 0.0) return E_.front();
    if (u >= 1.0) return E_.back();

    // find first cdf[j] >= u
    auto it = std::lower_bound(C_.begin(), C_.end(), u);
    size_t j = std::distance(C_.begin(), it);
    if (j == 0) return E_.front();

    // linearly interpolate E between nodes according to CDF spacing
    double u0 = C_[j-1], u1 = C_[j];
    double t  = (u - u0) / std::max(1e-16, (u1 - u0)); // [0,1]
    double E0 = E_[j-1], E1 = E_[j];
    return E0 + t * (E1 - E0);
  }

private:
  std::vector<double> E_, w_, C_;

  void buildCDF() {
    const size_t n = E_.size();
    C_.assign(n, 0.0);

    // Trapezoidal integral: area_i = 0.5*(w[i-1]+w[i]) * (E[i]-E[i-1])
    for (size_t i=1;i<n;++i) {
      double dE = E_[i] - E_[i-1];
      double area = 0.5 * (w_[i-1] + w_[i]) * dE;
      C_[i] = C_[i-1] + std::max(0.0, area);
    }
    double total = C_.back();
    if (total <= 0.0) throw std::runtime_error("SpectrumSampler: total area <= 0");

    // Normalize to [0,1]
    for (auto &c : C_) c /= total;
  }
};
