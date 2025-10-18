#include "../include/tree.hpp"
#include <gtest/gtest.h>

using KeyTy = int;

TEST(RB_Tree, InvalidInput) {
  RB_Tree::Tree<KeyTy> tree;

  tree.insert(2);
  tree.insert(5);

  auto l = tree.lowerBound(6);
  auto r = tree.upperBound(1);

  EXPECT_EQ(tree.distance(l, r), 0);
  EXPECT_EQ(tree.distance(r, l), 2);
}

TEST(RB_Tree, NullTree) {
  RB_Tree::Tree<KeyTy> tree;
  EXPECT_EQ(tree.get_root(), nullptr);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, EmptyTree) {
  RB_Tree::Tree<KeyTy> tree;

  auto l = tree.lowerBound(6);
  auto r = tree.upperBound(1);

  EXPECT_EQ(tree.distance(l, r), 0);
  EXPECT_EQ(tree.distance(r, l), 0);
}

TEST(RB_Tree, SingleInsert) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(42);

  EXPECT_NE(tree.get_root(), nullptr);
  EXPECT_EQ(tree.get_root()->key, 42);
  EXPECT_EQ(tree.get_root()->color, RB_Tree::Color::black);
  EXPECT_EQ(tree.get_root()->subtree_size, 1);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, DuplicateInsert) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(42);
  size_t initial_size = tree.get_root()->subtree_size;

  tree.insert(42);

  EXPECT_EQ(tree.get_root()->subtree_size, initial_size);
  EXPECT_EQ(tree.get_root()->key, 42);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, AscendingInsert) {
  RB_Tree::Tree<KeyTy> tree;

  for (int i = 0; i < 100; ++i)
    tree.insert(i);

  EXPECT_EQ(tree.get_root()->subtree_size, 100);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, DescendingInsert) {
  RB_Tree::Tree<KeyTy> tree;

  for (int i = 99; i >= 0; --i)
    tree.insert(i);

  EXPECT_EQ(tree.get_root()->subtree_size, 100);
}

TEST(RB_Tree, RandomInsert) {
  RB_Tree::Tree<KeyTy> tree;

  std::vector<int> values = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};

  for (int value : values) {
    tree.insert(value);
  }

  EXPECT_EQ(tree.get_root()->subtree_size, 10);
  EXPECT_TRUE(tree.verifyTree());
}

TEST(RB_Tree, MixedSignNumbers) {
  RB_Tree::Tree<KeyTy> tree;

  tree.insert(-10);
  tree.insert(10);
  tree.insert(0);
  tree.insert(-5);
  tree.insert(5);

  EXPECT_EQ(tree.get_root()->subtree_size, 5);
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

TEST(RB_Tree, EndPointerConsistency) {
  RB_Tree::Tree<KeyTy> tree;

  tree.insert(5);
  tree.insert(3);
  tree.insert(7);

  EXPECT_NE(tree.get_end().left, nullptr);
  EXPECT_EQ(tree.get_end().left->key, 7);

  tree.insert(10);
  EXPECT_EQ(tree.get_end().left->key, 10);

  tree.insert(8);
  EXPECT_EQ(tree.get_end().left->key, 10);

  EXPECT_TRUE(tree.verifyTree());
}

TEST(RBTreeIterator, EmptyTree) {
  RB_Tree::Tree<KeyTy> tree;
  EXPECT_TRUE(tree.begin() == tree.end());
  EXPECT_EQ(tree.distance(tree.begin(), tree.end()), 0);
}

TEST(RBTreeIterator, SingleElement) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(42);
  ASSERT_TRUE(tree.verifyTree());

  auto it = tree.begin();
  EXPECT_NE(it, tree.end());
  EXPECT_EQ(*it, 42);
  ++it;
  EXPECT_EQ(it, tree.end());

  auto it2 = tree.end();
  --it2;
  EXPECT_EQ(*it2, 42);
  EXPECT_EQ(it2, tree.begin());
}

TEST(RBTreeIterator, ForwardTraversal) {
  RB_Tree::Tree<KeyTy> tree;
  std::vector<int> keys = {5, 3, 7, 2, 4, 6, 8};
  for (int k : keys)
    tree.insert(k);
  ASSERT_TRUE(tree.verifyTree());

  std::vector<int> expected = {2, 3, 4, 5, 6, 7, 8};
  std::vector<int> actual;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    actual.push_back(*it);
  }
  EXPECT_EQ(actual, expected);
}

TEST(RBTreeIterator, ReverseTraversal) {
  RB_Tree::Tree<KeyTy> tree;

  std::vector<int> keys = {5, 3, 7, 2, 4, 6, 8};
  for (int k : keys)
    tree.insert(k);
  ASSERT_TRUE(tree.verifyTree());

  std::vector<int> expected = {8, 7, 6, 5, 4, 3, 2};
  std::vector<int> actual;

  auto it = tree.end();
  while (it != tree.begin()) {
    --it;
    actual.push_back(*it);
  }

  EXPECT_EQ(actual, expected);
}

TEST(RBTreeIterator, IncrementOperators) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(10);
  tree.insert(20);

  auto it1 = tree.begin();
  auto it2 = ++it1;
  EXPECT_EQ(*it1, 20);
  EXPECT_EQ(*it2, 20);
}

TEST(RBTreeIterator, DecrementOperators) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(10);
  tree.insert(20);

  auto it1 = tree.end();
  auto it2 = --it1;
  EXPECT_EQ(*it1, 20);
  EXPECT_EQ(*it2, 20);
}

TEST(RBTreeIterator, RangeQuery) {
  RB_Tree::Tree<KeyTy> tree;

  for (int i = 1; i <= 10; ++i)
    tree.insert(i);

  auto l = tree.lowerBound(3);
  auto r = tree.upperBound(7);

  std::vector<int> expected = {3, 4, 5, 6, 7};
  std::vector<int> actual;

  for (auto it = l; it != r; ++it)
    actual.push_back(*it);

  EXPECT_EQ(actual, expected);
  EXPECT_EQ(tree.distance(l, r), 5);
}

TEST(RBTreeIterator, LargeTreeTraversal) {
  RB_Tree::Tree<KeyTy> tree;
  const int N = 1000;
  for (int i = N; i >= 1; --i)
    tree.insert(i);
  ASSERT_TRUE(tree.verifyTree());

  int count = 0;
  int expected = 1;
  for (auto it = tree.begin(); it != tree.end(); ++it, ++count) {
    EXPECT_EQ(*it, expected++);
  }
  EXPECT_EQ(count, N);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
