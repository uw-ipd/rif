#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <iostream>
#include <sampling/orientations.hpp>

#include <sys/param.h>
#include <unistd.h>

// fails on travis-ci with llvm
// shouldn't touch the fs from the c++ layer
TEST(Orientation, read_karney_datasets) {
  std::string str(
      "# Orientation set c48u1, number = 24, radius = 62.80 degrees\n"
      "# $Id: c48u1.grid 6102 2006-02-21 19:45:40Z ckarney $\n"
      "# For more information, See http://charles.karney.info/orientation/\n"
      "format grid\n"
      "0.70000 0.00     24    1   1 62.80  1.57514\n"
      " 0  0  0  1.000000  62.80   1\n");
  // todo: unzip data files

  // fill in data structure instead of stream
  std::cout << "TEST read_karney_datasets" << std::endl;

  // std::string s;
  // while(in >> s) std::cout << s << endl;
  // todo: assuming run from project root dir
  auto tuple = read_karney_orientations(str);
  Eigen::MatrixXd quats = std::get<0>(tuple);
  Eigen::VectorXd cover = std::get<1>(tuple);

  ASSERT_EQ(cover.size(), 24);
  // for(int i = 0; i < quats.rows(); ++i){
  // std::cout << quats(i,0) << " " << quats(i,1) << " " << quats(i,2) << " " <<
  // quats(i,3) << " " << cover(i) << std::endl;
  // }
  ASSERT_NEAR(quats(0, 0), 1, 0.0001);
  ASSERT_NEAR(quats(0, 1), 0, 0.0001);
  ASSERT_NEAR(quats(0, 2), 0, 0.0001);
  ASSERT_NEAR(quats(0, 3), 0, 0.0001);

  ASSERT_NEAR(quats(1, 0), 0, 0.0001);
  ASSERT_NEAR(quats(1, 1), 1, 0.0001);
  ASSERT_NEAR(quats(1, 2), 0, 0.0001);
  ASSERT_NEAR(quats(1, 3), 0, 0.0001);

  ASSERT_NEAR(quats(2, 0), 0, 0.0001);
  ASSERT_NEAR(quats(2, 1), 0, 0.0001);
  ASSERT_NEAR(quats(2, 2), 1, 0.0001);
  ASSERT_NEAR(quats(2, 3), 0, 0.0001);

  ASSERT_NEAR(quats(3, 0), 0, 0.0001);
  ASSERT_NEAR(quats(3, 1), 0, 0.0001);
  ASSERT_NEAR(quats(3, 2), 0, 0.0001);
  ASSERT_NEAR(quats(3, 3), 1, 0.0001);

  ASSERT_NEAR(quats(23, 2), 0.707107, 0.0001);

  // ASSERT_TRUE(false);
}
