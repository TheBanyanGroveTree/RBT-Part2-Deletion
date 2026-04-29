/**
   Description: RBT
   Author: Aahana Sapra
   Date: 04/26/2026
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Node.h"


using namespace std;


// Define function prototypes
void stringUpper(string& input);

void leftRotate(Node* x, Node* NIL, Node*& root);
void rightRotate(Node* x, Node* NIL, Node*& root);
void insert(int value, Node* NIL, Node*& root);
void treeCorrections(Node* n, Node* NIL, Node*& root);

void add(Node* NIL, Node*& root);
void read(Node* NIL, Node*& root);
void print(int depth, Node* NIL, Node* root);


int main() {
  // Initialize NIL (sentinel Node for leaves)
  Node* NIL = new Node(0);
  NIL->setIsRed(false);
  NIL->setLeft(NIL);
  NIL->setRight(NIL);

  Node* root = NIL; // Initialize RBT root Node

  string userCommand = ""; // Declare str for user input

  // Continue prompting user for input until QUIT command
  bool newInput = true;
  while (newInput) {
    // Read in user input
    cout << "Enter a command (ADD, READ, PRINT, QUIT): ";
    getline(cin, userCommand);

    stringUpper(userCommand); // Convert input to uppercase for comparison

    // Validate input and call appropriate method or exit program
    if (userCommand == "ADD") {
      add(NIL, root);
    } else if (userCommand == "READ") {
      read(NIL, root);
    } else if (userCommand == "PRINT") {
      print(0, NIL, root);
    } else if (userCommand == "QUIT") {
      newInput = false; // Change input status
      
    } else { // Invalid input
      cout << "Please input ADD, READ, PRINT, or QUIT." << endl;
    }
  }
  
  return 0;
}


// Convert string to uppercase
void stringUpper(string& input) {
  for (int i = 0; i < input.size(); i++) {
      input[i] = toupper(input[i]);
    }
}


// Implement left rotation (promote root successor)
void leftRotate(Node* x, Node* NIL, Node*& root) {
  Node* y = x->getRight(); // Node to be promoted

  // 1. HANDOVER: detach subtree (NEW x right subtree = OLD y left subtree)
  x->setRight(y->getLeft());
  if (y->getLeft() != NIL) {
    y->getLeft()->setParent(x);
  }

  // 2. PARENT LINK: connect y to tree
  y->setParent(x->getParent()); // NEW y parent = OLD x parent
  
  // Connect NEW x parent to y
  if (x->getParent() == nullptr) { // x = ROOT
    root = y;
  } else if (x == x->getParent()->getLeft()) { // x = LEFT child
    x->getParent()->setLeft(y);
  } else { // x = RIGHT child
    x->getParent()->setRight(y);
  }

  // 3. PIVOT: finalize x-y PARENT-CHILD link
  y->setLeft(x); 
  x->setParent(y);
}


// Implement right rotation (promote root predecessor)
void rightRotate(Node* x, Node* NIL, Node*& root) {
  Node* y = x->getLeft(); // Node to be promoted

  // 1. HANDOVER: detach subtree (NEW x left subtree = OLD y right subtree)
  x->setLeft(y->getRight());
  if (y->getRight() != NIL) {
    y->getRight()->setParent(x);
  }

  // 2. PARENT LINK: connect y to tree
  y->setParent(x->getParent());

  // Connect NEW x parent to y
  if (x->getParent() == nullptr) { // x = ROOT
    root = y;
  } else if (x == x->getParent()->getLeft()) { // x = LEFT child
    x->getParent()->setLeft(y);
  } else { // x = RIGHT child
    x->getParent()->setRight(y);
  }

  // 3. PIVOT: finalize x-y PARENT-CHILD link
  y->setRight(x);
  x->setParent(y);
}


// Insert new Node like in BST then fix RBT violations 
void insert(int value, Node* NIL, Node*& root) {
  // Initialize new Node
  Node* newNode = new Node(value);
  newNode->setLeft(NIL);
  newNode->setRight(NIL);

  Node* parent = nullptr;
  Node* current = root;

  // STEP 1: Insert using BST rules
  while (current != NIL) {
    parent = current;

    if (newNode->getValue() < current->getValue()) { // Traverse LEFT
      current = current->getLeft();
    } else { // Traverse RIGHT
      current = current->getRight();
    }  
  }

  newNode->setParent(parent);

  if (parent == nullptr) { // EMPTY tree
    root = newNode;
  } else if (newNode->getValue() < parent->getValue()) { // LESS than parent
    parent->setLeft(newNode);
  } else { // GREATER than parent
    parent->setRight(newNode);
  }

  // STEP 2: Fix RBT violations
  if (newNode->getParent() == nullptr) { // EMPTY tree
    newNode->setIsRed(false);
    return;
  }

  if (newNode->getGrandparent() == nullptr) { // parent = root
    return;
  }

  treeCorrections(newNode, NIL, root);
}


// Fix RBT violations
void treeCorrections(Node* n, Node* NIL, Node*& root) {
  // Continue while parent is RED
  while ((n != root) && (n->getParent() != nullptr) &&
	 (n->getParent()->getIsRed())) {
    Node* uncle = n->getUncle();

    // Parent = LEFT child
    if (n->getParent() == n->getGrandparent()->getLeft()) {
      if (uncle->getIsRed()) {
	// CASE 1: RED uncle = recolor parent, uncle, and grandparent
	n->getParent()->setIsRed(false);
	uncle->setIsRed(false);
	n->getGrandparent()->setIsRed(true);
	n = n->getGrandparent(); // Move up tree for next iteration
      } else {
	if (n == n->getParent()->getRight()) { // n = RIGHT child
	  // CASE 2: left rotate parent
	  n = n->getParent();
	  leftRotate(n, NIL, root);
	}

	// CASE 3: recolor and right rotate grandparent
	n->getParent()->setIsRed(false);
	n->getGrandparent()->setIsRed(true);
	rightRotate(n->getGrandparent(), NIL, root);
      }
    }
    // MIRROR CASE: parent = RIGHT child
    else {
      if (uncle->getIsRed()) {
	n->getParent()->setIsRed(false);
	uncle->setIsRed(false);
	n->getGrandparent()->setIsRed(true);
	n = n->getGrandparent(); // Move up tree for next iteration
      } else {
	if (n == n->getParent()->getLeft()) { // n = LEFT child
	  // CASE 4: right rotate parent
	  n = n->getParent();
	  rightRotate(n, NIL, root);
	}

	// CASE 5: recolor and left rotate grandparent
	n->getParent()->setIsRed(false);
	n->getGrandparent()->setIsRed(true);
	leftRotate(n->getGrandparent(), NIL, root);
      }
    }
  }

  root->setIsRed(false); // RULE 2: root is always black
}


// Insert by manually inputting numbers
void add(Node* NIL, Node*& root) {
  // Prompt user for manual input to fill RBT
  cout << "Enter integers from 1-999 separated by spaces." << endl;

  string inputLine = "";
  getline(cin, inputLine); // Read entire line

  // Parse line
  stringstream ss(inputLine);
  int value = 0;
  while (ss >> value) {
    insert(value, NIL, root);
  }
}


// Insert by reading a file
void read(Node* NIL, Node*& root) {
  ifstream file("numbers.txt"); // Open file

  // Read in space separated numbers
  int value = 0;
  while (file >> value) {
    insert(value, NIL, root);
  }
  
  file.close();
}


// Traverse RBT recursively to print sideways
void print(int depth, Node* NIL, Node* root) {
  // Base case
  if (root == NIL) {
    return;
  }

  // Print RIGHT child first
  print(depth + 1, NIL, root->getRight());

  // Print current Node with indentation
  for (int i = 0; i < depth; i++) {
    cout << '\t';
  }
  
  if (root->getIsRed()) {
    cout << "[R] " << root->getValue() << endl;
  } else {
    cout << "[B] " << root->getValue() << endl;
  }

  // Then print LEFT child
  print(depth + 1, NIL, root->getLeft());
}
