#include "../include/tree.hpp"
#include "../include/verify_tree.hpp"
#include <gtest/gtest.h>
#include <optional>

using KeyTy = int;

TEST(RB_Tree, InvalidInput) {
  RB_Tree::Tree<KeyTy> tree;

  tree.insert(2);
  tree.insert(5);

  auto lb = tree.lowerBound(6);
  auto ub = tree.upperBound(1);

  EXPECT_EQ(tree.distance(lb, ub), 0);
  EXPECT_EQ(tree.distance(ub, lb), 2);
}

TEST(RB_Tree, NullTree) {
  RB_Tree::Tree<KeyTy> tree;
  EXPECT_EQ(tree.get_root(), std::nullopt);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, EmptyTree) {
  RB_Tree::Tree<KeyTy> tree;

  auto lb = tree.lowerBound(6);
  auto ub = tree.upperBound(1);

  EXPECT_EQ(tree.distance(lb, ub), 0);
  EXPECT_EQ(tree.distance(ub, lb), 0);
}

TEST(RB_Tree, ZeroElements) {
  RB_Tree::Tree<KeyTy> tree1;
  tree1.insert(0);
  auto lb1 = tree1.lowerBound(1);
  auto ub1 = tree1.upperBound(9);
  EXPECT_EQ(tree1.distance(lb1, ub1), 0);
  EXPECT_EQ(tree1.distance(ub1, lb1), 0);

  RB_Tree::Tree<KeyTy> tree2;
  tree2.insert(10);
  auto lb2 = tree2.lowerBound(1);
  auto ub2 = tree2.upperBound(9);
  EXPECT_EQ(tree2.distance(lb2, ub2), 0);
  EXPECT_EQ(tree2.distance(ub2, lb2), 0);
}

TEST(RB_Tree, EdgeElement) {
  RB_Tree::Tree<KeyTy> tree1;
  tree1.insert(1);
  auto lb1 = tree1.lowerBound(1);
  auto ub1 = tree1.upperBound(9);
  EXPECT_EQ(tree1.distance(lb1, ub1), 1);
  EXPECT_EQ(tree1.distance(ub1, lb1), 0);

  RB_Tree::Tree<KeyTy> tree2;
  tree2.insert(9);
  auto lb2 = tree2.lowerBound(1);
  auto ub2 = tree2.upperBound(9);
  EXPECT_EQ(tree2.distance(lb2, ub2), 1);
  EXPECT_EQ(tree2.distance(ub2, lb2), 0);
}

TEST(RB_Tree, SingleInsert) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(42);

  EXPECT_NE(tree.get_root(), std::nullopt);
  EXPECT_EQ((*tree.get_root())->key, 42);
  EXPECT_EQ((*tree.get_root())->color, RB_Tree::Color::black);
  EXPECT_EQ((*tree.get_root())->subtree_size, 1);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, DuplicateInsert) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(42);
  size_t initial_size = (*tree.get_root())->subtree_size;

  tree.insert(42);

  EXPECT_EQ((*tree.get_root())->subtree_size, initial_size);
  EXPECT_EQ((*tree.get_root())->key, 42);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, AscendingInsert) {
  RB_Tree::Tree<KeyTy> tree1;

  for (int i = 0; i < 100; ++i)
    tree1.insert(i);

  EXPECT_EQ((*tree1.get_root())->subtree_size, 100);
  EXPECT_TRUE(tree1.verifyTree());
}

TEST(RB_Tree, DescendingInsert) {
  RB_Tree::Tree<KeyTy> tree;

  for (int i = 99; i >= 0; --i)
    tree.insert(i);

  EXPECT_EQ((*tree.get_root())->subtree_size, 100);
}

TEST(RB_Tree, RandomInsert) {
  RB_Tree::Tree<KeyTy> tree;

  std::vector<int> values = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};

  for (int value : values) {
    tree.insert(value);
  }

  EXPECT_EQ((*tree.get_root())->subtree_size, 10);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, MixedSignNumbers) {
  RB_Tree::Tree<KeyTy> tree;

  tree.insert(-10);
  tree.insert(10);
  tree.insert(0);
  tree.insert(-5);
  tree.insert(5);

  EXPECT_EQ((*tree.get_root())->subtree_size, 5);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, SubtreeSizeConsistency) {
  RB_Tree::Tree<KeyTy> tree;

  tree.insert(5);
  tree.insert(3);
  tree.insert(7);
  tree.insert(2);
  tree.insert(4);
  tree.insert(6);
  tree.insert(8);

  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, RangeQuery) {
  RB_Tree::Tree<KeyTy> tree;
  for (int i = 1; i <= 10; ++i)
    tree.insert(i);
  EXPECT_TRUE(tree.verifyTree());

  auto lb = tree.lowerBound(3);
  auto rb = tree.upperBound(7);

  EXPECT_EQ(tree.distance(lb, rb), 5);
  EXPECT_EQ(tree.distance(tree.lowerBound(1), tree.upperBound(10)), 10);
  EXPECT_EQ(tree.distance(tree.lowerBound(5), tree.upperBound(5)), 1);
}

TEST(RB_Tree, Bounds) {
  RB_Tree::Tree<KeyTy> tree;
  std::vector<int> keys = {2, 4, 6, 8};
  for (int k : keys)
    tree.insert(k);
  ASSERT_TRUE(tree.verifyTree());

  auto lb = tree.lowerBound(5);
  ASSERT_NE(lb, std::nullopt);
  EXPECT_EQ((*lb)->key, 6);

  auto ub = tree.upperBound(5);
  ASSERT_NE(ub, std::nullopt);
  EXPECT_EQ((*ub)->key, 6);

  EXPECT_EQ(tree.upperBound(8), std::nullopt);
  EXPECT_EQ(tree.lowerBound(10), std::nullopt);
}

// TEST(RB_Tree, LargeTree) {
//   RB_Tree::Tree<KeyTy> tree;
//   const int N = 1000;
//   for (int i = 1; i <= N; ++i)
//     tree.insert(i);
//   ASSERT_TRUE(tree.verifyTree());

//   EXPECT_EQ(tree.distance(tree.lowerBound(1), tree.upperBound(N)), N);
//   EXPECT_EQ(tree.distance(tree.lowerBound(N + 1), tree.upperBound(N + 10)),
//   0);

//   auto last = tree.lowerBound(N);
//   ASSERT_NE(last, nullptr);
//   EXPECT_EQ((*last)->key, N);
//   EXPECT_EQ(tree.getRank(last), N - 1);
// }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
