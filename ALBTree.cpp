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
    BTree(int order): order_(order), root_(nullptr) {}
    //~BTree();
    void insert(const KeyType& key, const ValueType& value){
        if(root_ == nullptr){
            root_ = new BTreeNode<KeyType, ValueType>;
            root_->isLeaf = true;
            root_->keys.push_back(key);
            root_->values.push_back(value);
        }
        else{
            if(root_->keys.size() == (2* order_ - 1)){
                BTreeNode<KeyType, ValueType>* newRoot = new BTreeNode<KeyType, ValueType>;
                newRoot->isLeaf = false;
                newRoot->children.push_back(root_);
                splitChild(newRoot, 0);
                root_ = newRoot;
            }
        }
    }
    ValueType search(const KeyType& key) const{
        return searchHelper(root_, key);
    }
    void insertNonFull(BTreeNode<KeyType, ValueType>* node, const KeyType& key, const ValueType& value){
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
    ValueType searchHelper(const BTreeNode<KeyType, ValueType>* node, const KeyType& key) const{
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
    void printTree() const{
        if(root_ != nullptr){
            printTreeHelper(root_, 0);
        }
    }
private:
    int order_; // Order of the B-tree
    BTreeNode<KeyType, ValueType>* root_; // Root of the B-tree
    void splitChild(BTreeNode<KeyType, ValueType>* parent, int index){
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
    void printTreeHelper(const BTreeNode<KeyType, ValueType>* node, int level) const {
        if (node != nullptr) {
            for (int i = 0; i < node->keys.size(); ++i) {
                // Print the key and value
                cout << "Level " << level << ": Key " << node->keys[i] << ", Value " << node->values[i] << endl;
            }

            // Recursively print the children
            for (const auto& child : node->children) {
                printTreeHelper(child, level + 1);
            }
        }
    }
};

