#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Treasure.h"

using namespace std;
using namespace sf;

class AVLNode {
public:
    Treasure data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Treasure t) : data(t) {
        left = nullptr;
        right = nullptr;
        height = 0;
    }
};

class Inventory {
public:
    AVLNode* root;

    Inventory() {
        root = nullptr;
    }

    int getHeight(AVLNode* root) {
        if (root == nullptr) {
            return -1;
        }
        return root->height;
    }

    int getBalance(AVLNode* root) {
        if (root == nullptr) {
            return 0;
        }
        return getHeight(root->left) - getHeight(root->right);
    }

    AVLNode* right_rotation(AVLNode* root) {
        AVLNode* temp = root->left;
        root->left = temp->right;
        temp->right = root;

        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
        temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;

        return temp; // Return the new root
    }

    AVLNode* left_rotation(AVLNode* root) {
        AVLNode* temp = root->right;
        root->right = temp->left;
        temp->left = root;

        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
        temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;

        return temp; // Return the new root
    }

    void insert(Treasure t) {
        root = recursive_insert(root, t);
    }

    AVLNode* recursive_insert(AVLNode*& root, Treasure t) {
        if (root == nullptr) {
            return new AVLNode(t);
        }

        if (t.score <= root->data.score) {
            root->left = recursive_insert(root->left, t);
        }
        else if (t.score > root->data.score) {
            root->right = recursive_insert(root->right, t);
        }

        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

        int balance = getBalance(root);

        if (balance > 1 && t.score < root->left->data.score) {
            return right_rotation(root);
        }

        if (balance < -1 && t.score > root->right->data.score) {
            return left_rotation(root);
        }

        if (balance > 1 && t.score > root->left->data.score) {
            root->left = left_rotation(root->left);
            return right_rotation(root);
        }

        if (balance < -1 && t.score <= root->right->data.score) {
            root->right = right_rotation(root->right);
            return left_rotation(root);
        }

        return root;
    }

    void remove(int score) {
        root = recursive_remove(root, score);
    }

    AVLNode* recursive_remove(AVLNode* root, int score) {
        if (root == nullptr) {
            return nullptr;
        }

        if (score < root->data.score) {
            root->left = recursive_remove(root->left, score);
        }
        else if (score > root->data.score) {
            root->right = recursive_remove(root->right, score);
        }
        else {
            if (root->left == nullptr || root->right == nullptr) {
                AVLNode* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    *root = *temp;
                }
                delete temp;
            }
            else {
                AVLNode* temp = root->right;
                while (temp->left != nullptr) {
                    temp = temp->left;
                }

                root->data = temp->data;
                root->right = recursive_remove(root->right, temp->data.score);
            }
        }

        if (root == nullptr) {
            return nullptr;
        }

        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0) {
            return right_rotation(root);
        }

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = left_rotation(root->left);
            return right_rotation(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0) {
            return left_rotation(root);
        }

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = right_rotation(root->right);
            return left_rotation(root);
        }

        return root;
    }
    void printInOrderTraversal(AVLNode* root) {
        if (root != nullptr) {
            printInOrderTraversal(root->left);
            std::cout << root->data.score << " ";
            printInOrderTraversal(root->right);
        }
    }
    void inOrder() {
        printInOrderTraversal(root);
    }

    bool search(AVLNode* root, int score) {
        if (root == nullptr) {
            return false; // Element not found
        }

        if (score == root->data.score) {
            return true; // Element found
        }
        else if (score < root->data.score) {
            return search(root->left, score); // Search in the left subtree
        }
        else {
            return search(root->right, score); // Search in the right subtree
        }
    }
    bool search(int score) {
        return search(root, score);
    }
    void showInventory(sf::RenderWindow& window, AVLNode* root, int x, int y) {
        if (root != nullptr) {
            x += 60;
            showInventory(window,root->left,x,y);
            root->data.setPosition(x, y);
            root->data.setSize(40, 40);
            window.draw(root->data.sprite);
            x += 60;
            showInventory(window,root->right,x,y);
        }
    }
};
