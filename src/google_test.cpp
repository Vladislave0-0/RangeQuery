#include <gtest/gtest.h>

TEST(TriangleWithTriangle, Intersection2D_1) {
  // Point t1p1{0.4, -5.0, 0.0};
  // Point t1p2{1.0, 2.0, 0.0};
  // Point t1p3{5.0, 0.0, 0.0};
  // Point t2p1{-1.0, 0.0, 0.0};
  // Point t2p2{3.0, 2.0, 0.0};
  // Point t2p3{6.0, 2.0, 0.0};
  // Triangle t1{t1p1, t1p2, t1p3};
  // Triangle t2{t2p1, t2p2, t2p3};

  // ASSERT_TRUE(check_intersection(t1, t2));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
