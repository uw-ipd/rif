#include <gtest/gtest.h>

#include "numeric/ray_hash.hpp"

namespace rif {
namespace numeric {
namespace ray_hash_test {

using std::cout;
using std::endl;

TEST(ray_hash, align_ray_pair) {
  std::mt19937 rng((unsigned int)time(0) + 267943);
  for (int i = 0; i < 100; ++i) {
    auto a1 = rand_ray_gaussian(rng);
    auto a2 = rand_ray_gaussian(rng);
    auto b1 = Ray<>(V3f(0, 0, 0), V3f(1, 0, 0));
    auto b2 = align_ray_pair(a1, a2);
    ASSERT_LT(fabs(b2.orig[2]), 0.001f);
    ASSERT_NEAR(a1.dirn.dot(a2.dirn), b1.dirn.dot(b2.dirn), 0.0001);
    ASSERT_NEAR((a1.orig - a2.orig).norm(), (b1.orig - b2.orig).norm(), 0.0001);
    ASSERT_NEAR((a1.orig + a1.dirn - a2.orig - a2.dirn).norm(),
                (b1.orig + b1.dirn - b2.orig - b2.dirn).norm(), 0.0001);
  }
}

TEST(ray_hash, ray_ray_hash_bins_of_centers) {
  RayRayBins<> rh(0.25, 1.0, 1.0);
  double nerr = 0;
  for (int key = 0; key < rh.size(); ++key) {
    auto cen = rh.get_center(key);
    auto cen_key = rh.get_key(cen);
    nerr += key != cen_key;
    ASSERT_EQ(key, cen_key);
  }
  ASSERT_LT(nerr / rh.size(), 0.1);
}

TEST(ray_hash, ray_hash_bins_of_centers) {
  RayBins<> rh(0.25, 1.0, 1.0);
  double nerr = 0;
  for (int key = 0; key < rh.size(); ++key) {
    auto cen = rh.get_center(key);
    auto cen_key = rh.get_key(cen);
    nerr += key != cen_key;
    ASSERT_EQ(key, cen_key);
  }
  ASSERT_LT(nerr / rh.size(), 0.001);
}

TEST(ray_hash, ray_ray_hash_cart_ori_spacing) {
  RayRayBins<> rh(0.25, 1.0, 0.3);
  A2<float> nbr_sp = brute_maxmin_nbr(rh, 1);
  // std::cout << "rh.size " << rh.size() << ", cart " << rh.size_cart()
  // << ", qsph " << rh.size_qsph()
  // << ", closest: " << nbr_sp.transpose() << std::endl;
  ASSERT_GE(nbr_sp[0], 1.5 * nbr_sp[1]);
}

TEST(ray_hash, ray_hash_cart_ori_spacing) {
  RayBins<> rh(0.25, 1.0, 0.3);
  A2<float> nbr_sp = brute_maxmin_nbr(rh, 1);
  std::cout << "rh.size " << rh.size() << ", cart " << rh.size_cart()
            << ", qsph " << rh.size_qsph()
            << ", closest: " << nbr_sp.transpose() << std::endl;
  ASSERT_GE(nbr_sp[0], 1.5 * nbr_sp[1]);
}
}
}
}