#include <iostream>
#include <string>
#include "Node.h"

using namespace std;

// Constructor
Node::Node(int value) {
  this->value = value;
  isRed = true;
  left = nullptr;
  right = nullptr;
  parent = nullptr;
}

// Set value
void Node::setValue(int value) {
  this->value = value;
}

// Get value
int Node::getValue() {
  return value;
}

// Set color
void Node::setIsRed(bool isRed) {
  this->isRed = isRed;
}

// Get color
bool Node::getIsRed() {
  return isRed;
}

// Set left Node ptr
void Node::setLeft(Node* left) {
  this->left = left;
}

// Get left Node ptr
Node* Node::getLeft() {
  return left;
}

// Set right Node ptr
void Node::setRight(Node* right) {
  this->right = right;
}

// Get right Node ptr
Node* Node::getRight() {
  return right;
}

// Set parentNode ptr
void Node::setParent(Node* parent) {
  this->parent = parent;
}

// Get parent Node ptr
Node* Node::getParent() {
  return parent;
}

// Get grandparent Node ptr (parent of parent)
Node* Node::getGrandparent() {
  // NO parent = NO grandparent
  if (parent == nullptr) {
    return nullptr;
  }
  return parent->parent;
}

// Get sibling Node ptr (share parent)
Node* Node::getSibling() {
  // NO parent = NO sibling
  if (parent == nullptr) {
    return nullptr;
  }
  // if LEFT child, sibling = RIGHT
  else if (this == parent->left) {
    return parent->right;
  }
  // if RIGHT child, sibling = LEFT
  else {
    return parent->left;
  }
}

// Get uncle (OR aunt) Node ptr (sibling of parent)
Node* Node::getUncle() {
  // NO grandparent OR NO parent = NO uncle
  if ((this->getGrandparent() == nullptr) || (parent == nullptr)) {
    return nullptr;
  }
  // uncle = SIBLING of parent
  return parent->getSibling();
}

// Destructor
Node::~Node() {
  left = nullptr;
  right = nullptr;
  parent = nullptr;
}
