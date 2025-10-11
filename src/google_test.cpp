#include "../include/tree.hpp"
#include <gtest/gtest.h>

using KeyTy = int;

TEST(RB_Tree, EmptyTree) {
  RB_Tree::Tree<KeyTy> tree;
  EXPECT_EQ(tree.root, nullptr);
  EXPECT_TRUE(tree.verify());
}

TEST(RB_Tree, SingleInsert) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(42);

  EXPECT_NE(tree.root, nullptr);
  EXPECT_EQ(tree.root->key, 42);
  EXPECT_EQ(tree.root->color, RB_Tree::Color::black);
  EXPECT_EQ(tree.root->subtree_size, 1);
  EXPECT_TRUE(tree.verify());
}

TEST(RB_Tree, DuplicateInsert) {
  RB_Tree::Tree<KeyTy> tree;
  tree.insert(42);
  size_t initial_size = tree.root->subtree_size;

  tree.insert(42);

  EXPECT_EQ(tree.root->subtree_size, initial_size);
  EXPECT_EQ(tree.root->key, 42);
  EXPECT_TRUE(tree.verify());
}

TEST(RB_Tree, AscendingInsert) {
  RB_Tree::Tree<KeyTy> tree;

  for (int i = 0; i < 100; ++i) {
    tree.insert(i);
  }

  EXPECT_EQ(tree.root->subtree_size, 100);
  EXPECT_TRUE(tree.verify());
}

TEST(RB_Tree, DescendingInsert) {
  RB_Tree::Tree<KeyTy> tree;

  for (int i = 99; i >= 0; --i) {
    tree.insert(i);
  }

  EXPECT_EQ(tree.root->subtree_size, 100);
}

TEST(RB_Tree, RandomInsert) {
  RB_Tree::Tree<KeyTy> tree;

  std::vector<int> values = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};

  for (int value : values) {
    tree.insert(value);
  }

  EXPECT_EQ(tree.root->subtree_size, 10);
  EXPECT_TRUE(tree.verify());
}

TEST(RB_Tree, MixedSignNumbers) {
  RB_Tree::Tree<KeyTy> tree;
  
  tree.insert(-10);
  tree.insert(10);
  tree.insert(0);
  tree.insert(-5);
  tree.insert(5);
  
  EXPECT_EQ(tree.root->subtree_size, 5);
  EXPECT_TRUE(tree.verify());
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

  EXPECT_TRUE(tree.verify());
}

TEST(RB_Tree, RootAlwaysBlack) {
  RB_Tree::Tree<KeyTy> tree;
  
  tree.insert(10);
  EXPECT_EQ(tree.root->color, RB_Tree::Color::black);
  
  tree.insert(20);
  EXPECT_EQ(tree.root->color, RB_Tree::Color::black);
  
  tree.insert(5);
  EXPECT_EQ(tree.root->color, RB_Tree::Color::black);
  
  tree.insert(15);
  EXPECT_EQ(tree.root->color, RB_Tree::Color::black);
  
  tree.insert(25);
  EXPECT_EQ(tree.root->color, RB_Tree::Color::black);

  EXPECT_TRUE(tree.verify());
}

TEST(RB_Tree, EndPointerConsistency) {
  RB_Tree::Tree<KeyTy> tree;
  
  tree.insert(5);
  tree.insert(3);
  tree.insert(7);
  
  EXPECT_NE(tree.end.left, nullptr);
  EXPECT_EQ(tree.end.left->key, 7);
  
  tree.insert(10);
  EXPECT_EQ(tree.end.left->key, 10);
  
  tree.insert(8);
  EXPECT_EQ(tree.end.left->key, 10);

  EXPECT_TRUE(tree.verify());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
