#ifndef TREE_H
#define TREE_H

#include <iostream>

using std::cout;
using std::endl;

template <class T>
class Node
{
public:
    T data;
    Node<T> *left = nullptr;
    Node<T> *right = nullptr;

    Node(T val) { data = val; }
};

template <class T>
class Tree
{
private:
    void insert(T key, Node<T> *leaf);
    Node<T> *search(T key, Node<T> *leaf);
    void destroy_tree(Node<T> *leaf);

    void inorder_print(Node<T> *leaf);
    void postorder_print(Node<T> *leaf);
    void preorder_print(Node<T> *leaf);

    Node<T> *root;

public:
    Tree();
    ~Tree();

    void insert(T key);
    Node<T> *search(T key);
    void destroy_tree();

    void inorder_print();
    void postorder_print();
    void preorder_print();

    // Track the number of comparisons.
    long long int temp_comparisons = 0;
    long long int found_comparisons = 0;
    long long int not_found_comparisons = 0;
    int size = 0;
};

template <class T>
Tree<T>::Tree()
{
    root = nullptr;
}

template <class T>
Tree<T>::~Tree()
{
    destroy_tree();
}

// Destroy / Deallocate all parts of a subtree.
template <class T>
void Tree<T>::destroy_tree(Node<T> *leaf)
{
    if (leaf != nullptr)
    {
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        delete leaf;
    }

    return;
}

// Add a value to the tree using recursion.
template <class T>
void Tree<T>::insert(T key, Node<T> *leaf)
{

    if (key < leaf->data)
    {
        if (leaf->left != nullptr)
        {
            insert(key, leaf->left);
        }
        else
        {
            leaf->left = new Node<T>(key);
            leaf->left->left = nullptr;
            leaf->left->right = nullptr;
            size++;
        }
    }
    else if (key >= leaf->data)
    {
        if (leaf->right != nullptr)
        {
            insert(key, leaf->right);
        }
        else
        {
            leaf->right = new Node<T>(key);
            leaf->right->right = nullptr;
            leaf->right->left = nullptr;
            size++;
        }
    }

    return;
}

// Overloaded insert for recursion.
template <class T>
void Tree<T>::insert(T key)
{
    if (root != nullptr)
    {
        insert(key, root);
    }
    else
    {
        root = new Node<T>(key);
        root->left = nullptr;
        root->right = nullptr;
        size++;
    }
}

// Binary search tree for value recursively.
template <class T>
Node<T> *Tree<T>::search(T key, Node<T> *leaf)
{
    if (leaf != nullptr)
    {
        if (key == leaf->data)
        {
            temp_comparisons++;
            found_comparisons += temp_comparisons;
            temp_comparisons = 0;
            return leaf;
        }
        if (key < leaf->data)
        {
            temp_comparisons++;
            return search(key, leaf->left);
        }
        else
        {
            temp_comparisons++;
            return search(key, leaf->right);
        }
    }
    else
    {
        temp_comparisons++;
        not_found_comparisons += temp_comparisons;
        temp_comparisons = 0;
        return nullptr;
    }
}

// Overloaded search for recursion.
template <class T>
Node<T> *Tree<T>::search(T key)
{
    return search(key, root);
}

// Overloaded to destroy ENTIRE tree.
template <class T>
void Tree<T>::destroy_tree()
{
    destroy_tree(root);
}

// All the various printing methods, overloaded for recursive ease.
template <class T>
void Tree<T>::inorder_print()
{
    inorder_print(root);
    cout << "\n";
}

template <class T>
void Tree<T>::inorder_print(Node<T> *leaf)
{
    if (leaf != nullptr)
    {
        inorder_print(leaf->left);
        cout << leaf->data << endl;
        inorder_print(leaf->right);
    }
}

template <class T>
void Tree<T>::postorder_print()
{
    postorder_print(root);
    cout << "\n";
}

template <class T>
void Tree<T>::postorder_print(Node<T> *leaf)
{
    if (leaf != nullptr)
    {
        inorder_print(leaf->left);
        inorder_print(leaf->right);
        cout << leaf->data << ",";
    }
}

template <class T>
void Tree<T>::preorder_print()
{
    preorder_print(root);
    cout << "\n";
}

template <class T>
void Tree<T>::preorder_print(Node<T> *leaf)
{
    if (leaf != nullptr)
    {
        cout << leaf->data << ",";
        inorder_print(leaf->left);
        inorder_print(leaf->right);
    }
}

#endif
