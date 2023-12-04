#include <iostream>
#include <string>
#include <vector>
#include "ALBTree.h"
using namespace std;


template <typename KeyType, typename ValueType>
BTree<KeyType, ValueType>::BTree(int order) : order_(order), root_(nullptr) {}

template <typename KeyType, typename ValueType>
BTree<KeyType, ValueType>::~BTree() {
    // Cleanup: Delete all nodes in the B-tree (recursive)
}

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
    if(root_ == nullptr){
        root_ = new BTreeNode<KeyType, ValueType>;
        root_->isLeaf = true;
        root_->keys.push_back(key);
        root_->values.push_back(value);
    }
    else{
        if(root_->keys.size() == (2* order_ - 1)){
            BTreeNode<KeyType, ValueType>* newRoot = new BTree<KeyType, ValueType>;
            newRoot->isLeaf = false;
            newRoot->children.push_back(root_);
            splitChild(newRoot, 0);
            root_ = newRoot;
        }
    }
}

template<typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::insertNonFull(BTreeNode<KeyType, ValueType> *node, const KeyType &key,
                                              const ValueType &value) {
    int i = node->keys.size() - 1;
    if (node->isLeaf) {
        // Insert into a leaf node
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        node->keys.insert(node->keys.begin() + i, key);
        node->values.insert(node->values.begin() + i, value);
    } else {
        // Insert into an internal node
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        if (node->children[i]->keys.size() == (2 * order_ - 1)) {
            splitChild(node, i);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key, value);
    }
}

template <typename KeyType, typename ValueType>
ValueType BTree<KeyType, ValueType>::search(const KeyType& key) const {
    return searchHelper(root_, key);
}

template <typename KeyType, typename ValueType>
ValueType BTree<KeyType, ValueType>::searchHelper(const BTreeNode<KeyType, ValueType>* node, const KeyType& key) const {
    if (node == nullptr) {
        throw std::out_of_range("Key not found in the B-tree");
    }
    int i = 0;
    while (i < node->keys.size() && key > node->keys[i]) {
        i++;
    }
    if (i < node->keys.size() && key == node->keys[i]) {
        return node->values[i];
    }

    if (node->isLeaf) {
        throw std::out_of_range("Key not found in the B-tree");
    }

    // Recursively search in the appropriate child
    return searchHelper(node->children[i], key);
}


template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::splitChild(BTreeNode<KeyType, ValueType>* parent, int index) {
    BTreeNode<KeyType, ValueType>* child = parent->children[index];
    BTreeNode<KeyType, ValueType>* newChild = new BTreeNode<KeyType, ValueType>;

    newChild->isLeaf = child->isLeaf;

    parent->keys.insert(parent->keys.begin() + index, child->keys[order_ - 1]);
    parent->values.insert(parent->values.begin() + index, child->values[order_ - 1]);

    newChild->keys.assign(child->keys.begin() + order_, child->keys.end());
    newChild->values.assign(child->values.begin() + order_, child->values.end());
    child->keys.resize(order_ - 1);
    child->values.resize(order_ - 1);

    if (!child->isLeaf) {
        newChild->children.assign(child->children.begin() + order_, child->children.end());
        child->children.resize(order_);
    }

    parent->children.insert(parent->children.begin() + index + 1, newChild);
}

