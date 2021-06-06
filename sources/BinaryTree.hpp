/**
 * BinaryTree Assignment
 * C++ Course @Ariel_University
 * Part B
 * Author: Jordan Perez
 * https://www.github.com/jewgah
 * 
 * */

//includes
#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;
namespace ariel {

  // Node<T>
  template<typename T> class Node{
    public:
      T value;
      Node* left;          
      Node* right;

      // Constructor as Initialization List
      Node(T& value_) : value(value_), right(nullptr),  left(nullptr) {}
            
      Node(Node &src) = default;
      Node(Node&& other) = delete;

      ~Node() = default;

      // for make tidy
      Node& operator=(Node& other) = delete;
      Node& operator=(Node&& other) = delete;
  };

  // BinaryTree<T>
  template<typename T> class BinaryTree {

    private:

      Node<T>* root;

      //recursive function to print the tree
      static void print(const string& previous, Node<T>* node, bool is_Left)
      {
        if(node != nullptr){

          cout << previous;
          string parent = "├──";
          string child = "└──";

          if (is_Left){
            cout << parent;
          }
          else{
            cout << child;
          }
          cout << node->value << endl;
          print( previous + (is_Left ? "│   " : "    "), node->right, true);
          print( previous + (is_Left ? "│   " : "    "), node->left, false);
          }
      }

      //recursive function to copy tree
      void copytree(const Node<T>& src, Node<T>& dest){

          if(src.right!=nullptr) {
              dest.right = new Node<T>(src.right->value);
              copytree(*src.right,*dest.right);
          }
            
          if(src.left!=nullptr) {
              dest.left = new Node<T>(src.left->value);
              copytree(*src.left,*dest.left);
          }
      }

      // Recursive function to find a node by its value
      Node<T>* find_node( T& node_value , Node<T>* root){

        if(root == nullptr) {return nullptr;}
        if(root->value == node_value) {return root;}

        //check left wing
        Node<T>*  left_node = find_node(node_value,root->left);
        if(left_node != nullptr){return left_node;}

        //check right wing
        Node<T>*  right_node  = find_node(node_value,root->right);
        return right_node;
      }

  public:

    // default constructor
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() {};

    // Move constructor : Shallow copy
    BinaryTree(BinaryTree<T>&& otherTree) noexcept {
      root = otherTree.root; //copies other's root
      otherTree.root = nullptr; // erase other's root
    }

    // deep copy constructor
    BinaryTree(const BinaryTree<T> &otherTree){
      if(otherTree.root!=nullptr) {
          root = new Node<T>(otherTree.root->value); // create new root with same otherTree root's value
          copytree(*otherTree.root,*root); // recursively deep copies it
      }
    }

    // Operator Overload
    BinaryTree& operator=(BinaryTree otherTree){
        if(this == &otherTree){
            return *this;
        }
        if(root != nullptr){
            delete this->root;
        }
        root = new Node<T>(otherTree.root->value);
        copytree(*otherTree.root,*this->root);

        return *this;
    }

    BinaryTree& operator=(BinaryTree&& otherTree) noexcept{
        *this->root = otherTree.root;
    }

    //  adds a root to the tree with a given value. If there's already a root it replaces it.
    BinaryTree& add_root(T value){
      if(root!=nullptr){
        root->value= value;
      }
      else{
        root = new Node<T>(value);
      }
      return *this;
    }

    // adds a node to the left
    BinaryTree &add_left( T parent_value,  T left_value){
      Node<T> * found = find_node(parent_value,root);

      if(found == nullptr){
        throw invalid_argument("node not found");
      }
      if(found->left == nullptr){
        found ->left = new Node<T>(left_value);
      }
      else{
        found ->left ->value = left_value;
      }
      return *this;
    }

    //add_right - receives 2 nodes First is the parent that i will place the Second as right Child
    BinaryTree &add_right( T parent_value,  T right_value){
      Node<T> * ans = find_node(parent_value,root);
      if(ans == nullptr){
        throw invalid_argument("node not found");
      }
      if(ans->right == nullptr){
        ans ->right = new Node<T>(right_value);
      }
      else{
          ans ->right ->value = right_value;
      }
      return *this;
    }

    //Output operator - prints the tree nicely
    friend std::ostream &operator<<(std::ostream &out, const BinaryTree<T> &tree){
      print("", tree.root, false);
      return out;
    }

  // Inner iterator class
  class iter{

    private:

    void preorder_fill(Node<T>* n){
      if(n == nullptr){return;}

      vec.push_back(n);
      preorder_fill(n->left);
      preorder_fill(n->right);
    }

    void inorder_fill(Node<T>* n){
      if(n == nullptr){return;}
      
      inorder_fill(n->left);
      vec.push_back(n);      
      inorder_fill(n->right);
    }

    void postorder_fill(Node<T>* n){
      if(n == nullptr){return;}
      
      postorder_fill(n->left);
      postorder_fill(n->right);
      vec.push_back(n); 
    }

    public:

      Node<T>* current_node;
      vector<Node<T>*> vec;

      iter():current_node(nullptr){}

      iter(Node<T>* t):current_node(t){}

      iter(Node<T>* root ,const string &type){
        if(root != nullptr){
          if(type == "preorder") {preorder_fill(root);}
          if(type == "inorder") {inorder_fill(root);}
          if(type == "postorder") {postorder_fill(root);}
          current_node = vec.at(0);
        }
        else{
          current_node =nullptr;
        }
      }

    T &operator*() const {
      return current_node->value;
    }

    T *operator->() const {
      return &(current_node->value);
    }

    iter &operator++(){
      if(vec.size()>1){
        vec.erase(vec.begin());
        current_node = vec[0];
        }
      else{
        current_node =nullptr;
      }
      return *this;
    }

    iter operator++(int){
      iter t(vec[0]);
      ++*this;
      return t;
    }

    bool operator==(const iter &other) const {
      return current_node == nullptr;
    }

    bool operator!=(const iter &other) const {
      return !(*this == other);
    }
  }; // end of iter class

    //Default - Inorder
    iter begin(){
      return iter{root,"inorder"};
    };
    iter end(){
      return iter{};
    };

    // preorder (parent - left - right).
    iter begin_preorder(){
      return iter{root,"preorder"};
    }
    iter end_preorder(){
      return iter{};
    }

    // inorder (left - parent - right).
    iter begin_inorder(){
      return iter{root,"inorder"};
    }
    iter end_inorder(){
      return iter{};
    }

    // postorder (left - right - parent).
    iter begin_postorder(){
      return iter{root,"postorder"};
    }
    iter end_postorder(){
      return iter{};
    }
  }; //end of BinaryTree class
}// end of namespace std