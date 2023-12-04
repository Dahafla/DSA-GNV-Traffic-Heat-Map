#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;


template <typename KeyType, typename ValueType>
struct BTreeNode {
    bool isLeaf; // Indicates whether the node is a leaf
    vector<KeyType> keys; // Keys in the node
    vector<ValueType> values; // Corresponding values
    vector<BTreeNode*> children; // Pointers to child nodes
};

// B-tree class
template <typename KeyType, typename ValueType>
class BTree {
public:

    BTree(int order);
    ~BTree();
    void insert(const KeyType& key, const ValueType& value);
    ValueType search(const KeyType& key) const;
    void insertNonFull(BTreeNode<KeyType, ValueType>* node, const KeyType& key, const ValueType& value);
    ValueType searchHelper(const BTreeNode<KeyType, ValueType>* node, const KeyType& key) const;
private:
    int order_; // Order of the B-tree
    BTreeNode<KeyType, ValueType>* root_; // Root of the B-tree
    void splitChild(BTreeNode<KeyType, ValueType>* parent, int index);
};

