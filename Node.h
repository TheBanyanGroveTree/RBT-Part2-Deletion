#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>

using namespace std;

class Node {
 public:
  Node(int);
  
  void setValue(int);
  int getValue();

  void setIsRed(bool);
  bool getIsRed();

  void setLeft(Node*);
  Node* getLeft();

  void setRight(Node*);
  Node* getRight();

  void setParent(Node*);
  Node* getParent();

  Node* getGrandparent();
  Node* getSibling();
  Node* getUncle();

  ~Node();
 private:
  int value;
  bool isRed;
  Node* left;
  Node* right;
  Node* parent;
};

#endif
