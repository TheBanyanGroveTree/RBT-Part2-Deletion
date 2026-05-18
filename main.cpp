/**
   Description: RBT
   Author: Aahana Sapra
   Date: 05/15/2026
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <ios>
#include "Node.h"


using namespace std;


// Define function prototypes
void stringUpper(string& input);

void leftRotate(Node* x, Node* NIL, Node*& root);
void rightRotate(Node* x, Node* NIL, Node*& root);
void insert(int value, Node* NIL, Node*& root);
void insertionTreeCorrections(Node* n, Node* NIL, Node*& root);
void transplant(Node* rem, Node* rep, Node* NIL, Node*& root);
void deleteNode(int value, Node* NIL, Node* root);
void deletionTreeCorrections(Node* n, Node* NIL, Node*& root);

void add(Node* NIL, Node*& root);
void read(Node* NIL, Node*& root);
bool search(int value, Node* NIL, Node* root);
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
    cout << "Enter a command (ADD, READ, SEARCH, DELETE, PRINT, or QUIT): ";
    getline(cin, userCommand);

    stringUpper(userCommand); // Convert input to uppercase for comparison

    // Validate input and call appropriate method or exit program
    if (userCommand == "ADD") {
      add(NIL, root);
      
    } else if (userCommand == "READ") {
      read(NIL, root);
      
    } else if (userCommand == "SEARCH") {
      // Prompt user for number to search for
      int numSearch = 0;
      cout << "Enter the number to search for: " << endl;
      cin >> numSearch;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      // Output search results
      bool searchResult = search(numSearch, NIL, root);
      if (searchResult) {
	cout << "YAY, I found it!" << endl;
      } else if (!searchResult) {
	cout << "Hmmm... No encontre." << endl;
      }
      
    } else if (userCommand == "DELETE") {
      // Prompt user for number to delete
      int numDelete = 0;
      cout << "Enter the number to delete: " << endl;
      cin >> numDelete;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      // delete();

    } else if (userCommand == "PRINT") {
      print(0, NIL, root);
      
    } else if (userCommand == "QUIT") {
      newInput = false; // Change input status
      
    } else { // Invalid input
      cout << "Please input ADD, READ, SEARCH, DELETE, PRINT, or QUIT."
	   << endl;
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

  insertionTreeCorrections(newNode, NIL, root);
}


// Fix RBT insertion violations
void insertionTreeCorrections(Node* n, Node* NIL, Node*& root) {
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


// Replace subtree at Node rem with subtree at Node rep
void transplant(Node* rem, Node* rep, Node* NIL, Node*& root) {
  // Node to remove = root
  if (rem->getParent() == NIL) {
    root = rep;
  }
  // Node to remove = left child
  else if (rem == rem->getParent()->getLeft()) {
    rem->getParent()->setLeft(rep);
  }
  // Node to remove = right child
  else {
    rem->getParent()->setRight(rep);
  }
  
  rep->setParent(rem->getParent()); // Update parent
}


// Delete given value from RBT
void deleteNode(int value, Node* NIL, Node* root) {
  // Iteratively find target Node
  Node* target;
  while (root != NIL) {
    if (value == root->getValue()) {
      target = root;
    }
    // Traverse left subtree if value less than current
    else if (value < root->getValue()) {
      target = root->getLeft();
    }
    // Traverse right subtree if value greater than current
    else {
      target = root->getRight();
    }
  }

  if (target == NIL) { // Target NOT found
    return;
  }

  Node* replacement; // Replacement Node
  bool removedOriginalColor = target->getIsRed(); // Store original color

  // CASE 1: Left child is NIL
  if (target->getLeft() == NIL) {
    replacement = target->getRight();
    transplant(target, replacement, NIL, root);
  }
  // CASE 2: Right child is NIL
  else if (target->getRight() == NIL) {
    replacement = target->getLeft();
    transplant(target, replacement, NIL, root);
  }
  // CASE 3: BOTH children are NIL
  else {
    // Find min of right subtree
    Node* successor = target->getRight();
    while (successor->getLeft() != NIL) {
      successor = successor->getLeft();
    }

    replacement = successor->getRight();
    removedOriginalColor = successor->getIsRed(); // Store color of Node actually removed

    // CASE A: Immediate right child of target
    if (successor->getParent() == target) {
      replacement->setParent(successor); // Update replacement parent
    }
    // CASE B: Deeper in tree
    else {
      // Replace successor with its right child
      transplant(successor, successor->getRight(), NIL, root);

      // Attach target's right subtree to successor
      successor->setRight(target->getRight());
      successor->getRight()->setParent(successor);
    }

    // Remove target node from tree
    transplant(target, successor, NIL, root);

    // Attach target's left subtree to successor
    successor->setLeft(target->getLeft());
    successor->getLeft()->setParent(successor);

    // Inherit color of removed Node
    successor->setIsRed(removedOriginalColor);
  }

  delete target; // Delete dynamically allocated memory

  // Call correction function is OG color is black
  if (!removedOriginalColor) {
    deletionTreeCorrections(replacement, NIL, root);
  }
}


// CASE 1: current Node N = new root (cleaned up in last line)
// CASE 2: P, S & S children = black (similar to 4)
// CASE 3: S = red
// CASE 4: S & S children = black; P = red (similar to 2)
// CASE 5: P color doesn't matter (different color sibling children)
// CASE 6: P color doesn't matter (")


// Fix RBT deletion violations
void deletionTreeCorrections(Node* n, Node* NIL, Node*& root) {
  // Continue until root or red Node
  while (n != root && !n->getIsRed()) {
    // n = LEFT child
    if (n == n->getParent()->getLeft()) {
      Node* s = n->getSibling();

      // CASE 1: S = red
      if (s->getIsRed()) {
	// Recolor
	s->setIsRed(false);
	n->getParent()->setIsRed(true);
	// Rotate to make S black -> CASE 2/3/4
	leftRotate(n->getParent(), NIL, root);
	s = n->getSibling(); // Update sibling
      }

      // CASE 2: BOTH S children = black
      if (!s->getLeft()->getIsRed() && !s->getRight()->getIsRed()) {
	// Move blackness up tree
	s->setIsRed(true);
	n = n->getParent();
      }
      else {
	// CASE 3: S right child = black
	if (!s->getRight()->getIsRed()) {
	  // Recolor
	  s->getLeft()->setIsRed(false);
	  s->setIsRed(true);
	  // Rotate -> CASE 4
	  rightRotate(s, NIL, root);
	  s = n->getSibling(); // Update sibling
	}

	// CASE 4: S right child = red
	// Recolor
	s->setIsRed(n->getParent()->getIsRed());
	n->getParent()->setIsRed(false);
	s->getRight()->setIsRed(false);
	// Rotate to balance black height
	leftRotate(n->getParent(), NIL, root);
	n = root; // Remove extra blackness
      }
    }

    // MIRROR: n = RIGHT child
    else {
      Node* s = n->getSibling();  // Sibling

      // MIRROR: CASE 1
      if (s->getIsRed()) {
	n->setIsRed(false);
	n->getParent()->setIsRed(true);
	rightRotate(n->getParent(), NIL, root);
	s = n->getSibling();
      }

      // MIRROR: CASE 2
      if (!s->getLeft()->getIsRed() && !s->getLeft()->getIsRed()) {
	s->setIsRed(true);
	n = n->getParent();
      }
      else {
	// MIRROR: CASE 3
	if (!s->getLeft()->getIsRed()) {
	  s->getRight()->setIsRed(false);
	  s->setIsRed(true);
	  leftRotate(s, NIL, root);
	  s = n->getSibling();
	}

	// MIRROR: CASE 4
	s->setIsRed(n->getParent()->getIsRed());
	n->getParent()->setIsRed(false);
	s->getLeft()->setIsRed(false);
	leftRotate(n->getParent(), NIL, root);
	n = root; 
      }
    }
  }
  
  n->setIsRed(false); // Absorb extra blackness
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


// Search for a particular value in RBT
bool search(int value, Node* NIL, Node* root) {
  // Base Case 1: root is NIL = NOT found
  if (root == NIL) {
    return false;
  }
  
  // Base Case 2: target value and current value match
  if (root->getValue() == value) {
    return true;
  }

  // Recursive call
  // Traverse left if value smaller than root's value
  if (value < root->getValue()) {
    return search(value, NIL, root->getLeft());
  }

  // Traverse right if value greater than root's value
  return search(value, NIL, root->getRight()); // general return statement
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
