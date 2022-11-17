#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); 
    virtual void remove(const Key& key);  // NEED TO CHECK
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    
    void rotateLeft(AVLNode<Key, Value>* node);  //NEED TO CHECK
    void rotateRight(AVLNode<Key, Value>* node);  //NEED TO CHECK
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);  //NEED TO CHECK 
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);  //NEED TO CHECK
    int childcount(const AVLNode<Key, Value>* current) const;
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if(this->root_ == NULL) //if root is NULL there is no tree so create new node
    {
        AVLNode<Key, Value>* node = new AVLNode<Key,Value>(new_item.first,new_item.second,NULL);//balance autoset to 0
        this->root_=node;
        return;
    }
    else
    {
        AVLNode<Key, Value>* r = static_cast<AVLNode<Key, Value>*>(this->root_);
        while(r != NULL)
        {
            if(new_item.first > r->getKey()) //key is greater than root so move to right subtree
            {
                if(r->getRight() == NULL)
                {
                    AVLNode<Key, Value>* node = new AVLNode<Key,Value>(new_item.first,new_item.second,r);
                    r->setRight(node);
                    if(r->getBalance() == 1 || r->getBalance() ==-1)
                    {
                        r->setBalance(0);
                    }
                    else if(r->getBalance() == 0)
                    {
                        r->setBalance(1);
                        insertFix(r,node);
                    }
                    break;
                }
                else r = r->getRight(); 
            }
            else if(new_item.first < r->getKey())//key is less than root so move to left subtree
            {
                if(r->getLeft() == NULL)
                {
                    AVLNode<Key, Value>* node = new AVLNode<Key,Value>(new_item.first,new_item.second,r);
                    r->setLeft(node);
                    if(r->getBalance() == 1 || r->getBalance() ==-1)
                    {
                        r->setBalance(0);
                    }
                    else if(r->getBalance() == 0)
                    {
                        r->setBalance(-1);
                        insertFix(r,node);
                    }
                    break;
                }
                else r = r->getLeft();
            }
            else if(new_item.first == r->getKey()) //if equal to another key then result value 
            {
                r->setValue(new_item.second);
                break;
            }
        }
    }
    return;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
     if(this->root_ == NULL) return; //check to see if tree exists

    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if(target == NULL) return; //if node does not exhist then return

    AVLNode<Key, Value>* tmp;
    AVLNode<Key,Value>* parent = target->getParent();
    int8_t diff = 0;

    if(childcount(target) == 3) //if node has two children simply swap with pred node
    {
        nodeSwap(target,static_cast<AVLNode<Key, Value>*>(this->predecessor(target)));
        parent = target->getParent();
    }
    if(childcount(target) == 1) //then target only has left child
    {
        tmp = target->getLeft();
    }
    if(childcount(target) == 2) //then target only has right child
    {
        tmp = target->getRight();
    }
    if(childcount(target)==0) //node has no children 
    {
        tmp = NULL;
    }
    
    //difference calculations
    if(parent != NULL)
    {
        if(target->getParent()->getLeft() == target) //target node only has left child
        {
            diff = 1;
        }
        else
        {
            diff = -1;
        }
    }
    
    if(parent == NULL) //if no parent then set root to tmp
    {
        this->root_ = tmp;
    }
    else  //there is a parent
    {
        if(target->getParent()->getRight()==target)
        {
            parent->setRight(tmp);
        }
        else if(target->getParent()->getLeft()==target)
        {
            parent->setLeft(tmp);
        }
    }
    if(tmp != NULL)
    {
        tmp->setParent(parent); 
        
    }
    delete target;
    removeFix(parent, diff);
}

template<class Key, class Value>
int AVLTree<Key, Value>::childcount(const AVLNode<Key, Value>* current) const
{
    if(current->getLeft() != NULL && current->getRight() != NULL) return 3; //has two children
    else if(current->getLeft() == NULL && current->getRight() == NULL ) return 0; //has no children
    else if(current->getLeft() != NULL && current->getRight() == NULL) return 1; //only has left child
    else if(current->getLeft() == NULL && current->getRight() !=NULL) return 2; //only has right child
    return -1; 
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{
    if(parent == NULL || parent->getParent() == NULL || node == NULL) //if parent or grandparent is NULL then return bc it is alrady balanced
    {
        return;
    }
    AVLNode<Key, Value>* grandParent = parent->getParent(); //set grandParent Value
    //node is right child of parent
    if(grandParent->getRight() == parent)
    {
        grandParent->updateBalance(1); //add 1 to balance since node is added to right tree
        if(grandParent->getBalance() == 0) //grandparent is already balanced so return 
        {
            return;
        }
        else if(grandParent->getBalance() == 1)
        {
            insertFix(grandParent, parent); //case where balance is 1 so recurse
        }
        else if(grandParent->getBalance() == 2) //balance needs to be adjusted
        {
            if(parent->getBalance() == -1) //a zig-zag rotation is needed
            {
                rotateRight(parent);
                rotateLeft(grandParent);
                if(node->getBalance() == -1) //balance is -1
                {
                    parent->setBalance(1);
                    grandParent->setBalance(0);
                    node->setBalance(0);
                }
                else if(node->getBalance() == 0)
                {
                    parent->setBalance(0);
                    grandParent->setBalance(0);
                    node->setBalance(0);
                }
                else if(node->getBalance() == 1)
                {
                    parent->setBalance(0);
                    grandParent->setBalance(-1);
                    node->setBalance(0);
                }
            }
            else if(parent->getBalance() == 1) //a zig zig is needed
            {
                rotateLeft(grandParent);
                parent->setBalance(0);
                grandParent->setBalance(0);
            }
        }
    }

    else //case where parent is the left child of grandparent
    {
        grandParent->updateBalance(-1); //update grandparent by -1 since left child is added
        
        if(grandParent->getBalance() == 0) //grandparent is already balanced so return
        {
            return;
        } 
        else if(grandParent->getBalance() == -1) //grandparent's balance is -1 so recurse
        {
            insertFix(grandParent,parent); //case 2 recurse
        }
        else if(grandParent->getBalance() == -2)
        {
            //check if it is a zig zig case
            if(parent->getBalance() == -1) 
            {
                rotateRight(grandParent);
                parent->setBalance(0);
                grandParent->setBalance(0);
            }
            //check if it is a zig-zag case
            else if(node == parent->getRight())
            {
                rotateLeft(parent);
                rotateRight(grandParent);
                //if balance is 1
                if(node->getBalance() == 1)
                {
                    parent->setBalance(-1);
                    grandParent->setBalance(0);
                    node->setBalance(0);
                }
                //if balance is 0
                else if(node->getBalance() == 0)
                {
                    parent->setBalance(0);
                    grandParent->setBalance(0);
                    node->setBalance(0);
                }
                //if balance is -1
                else if(node->getBalance() == -1)
                {
                    parent->setBalance(0);
                    grandParent->setBalance(1);
                    node->setBalance(0);
                }
            }
            else if(node == parent->getLeft()) //a zig zig is needed
            {
                rotateRight(grandParent);
                parent->setBalance(0);
                grandParent->setBalance(0);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{
    if(node == NULL) //if node is NULL then just return
    {
        return;
    }
    AVLNode<Key, Value>* parent = node->getParent();
    int8_t calcedDiff = 0;
    if(parent != NULL)
    {
        if(parent->getLeft() == node) //if node is left child set diff to 1
        {
            calcedDiff = 1;
        }
        else //then node is right child so set diff to -1
        {
            calcedDiff = -1;
        }
    }
    if(diff == -1) //case for diff = -1
    {
        if(node->getBalance() + diff == -2)
        {
            AVLNode<Key,Value>* tallerChild = node->getLeft(); //since diff is -2 left subtree is longer so left child is taller

            if(tallerChild->getBalance() == -1) //zig zig case
            {
                rotateRight(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                removeFix(parent,calcedDiff);
            }
            else if(tallerChild->getBalance() == 0) //zig zig case
            {
                rotateRight(node);
                node->setBalance(-1);
                tallerChild->setBalance(1);
                return;
            }
            else if(tallerChild->getBalance() == 1) //zig-zag case
            {
                AVLNode<Key, Value>* grandChild = tallerChild->getRight();
                rotateLeft(tallerChild);
                rotateRight(node);
                if(grandChild->getBalance() == 1)
                {
                    node->setBalance(0);
                    tallerChild->setBalance(-1);
                    grandChild->setBalance(0);
                }
                else if(grandChild->getBalance() == 0)
                {
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                else if(grandChild->getBalance() == -1)
                {
                    node->setBalance(1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                removeFix(parent,calcedDiff);
            }
        }
        else if(node->getBalance() + diff == -1)
        {
            node->setBalance(-1);
            return;
        }
        else if(node->getBalance() + diff == 0)
        {
            node->setBalance(0);
            removeFix(parent,calcedDiff);
        }
    }
    else if(diff == 1) //case for diff = 1
    {
        if(node->getBalance() + diff == 2)
        {
            AVLNode<Key,Value>* tallerChild = node->getRight(); //since diff is -2 left subtree is longer so left child is taller

            if(tallerChild->getBalance() == 1) //zig zig case
            {
                rotateLeft(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                removeFix(parent,calcedDiff);
            }
            else if(tallerChild->getBalance() == 0) //zig zig case
            {
                rotateLeft(node);
                node->setBalance(1);
                tallerChild->setBalance(-1);
                return;
            }
            else if(tallerChild->getBalance() == -1) //zig-zag case
            {
                AVLNode<Key, Value>* grandChild = tallerChild->getLeft();
                rotateRight(tallerChild);
                rotateLeft(node);
                if(grandChild->getBalance() == -1)
                {
                    node->setBalance(0);
                    tallerChild->setBalance(1);
                    grandChild->setBalance(0);
                }
                else if(grandChild->getBalance() == 0)
                {
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                else if(grandChild->getBalance() == 1)
                {
                    node->setBalance(-1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                removeFix(parent,calcedDiff);
            }
        }
        else if(node->getBalance() + diff == 1)
        {
            node->setBalance(1);
            return;
        }
        else if(node->getBalance() + diff == 0)
        {
            node->setBalance(0);
            removeFix(parent,calcedDiff);
        }
    }
    
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    if(node == NULL) //if node is NULL then return
    {
        return;
    }
    AVLNode<Key, Value>* child = node->getRight(); //y
    AVLNode<Key, Value>* childLeftChild = child->getLeft(); //switched tree
    if(this->root_ == node) //node is root of tree
    {
        this->root_ = child;
        child->setParent(NULL);
    }
    else
    {
        child->setParent(node->getParent());
        if(node->getParent()->getRight() == node) //node is Right child of parent
        {
            node->getParent()->setRight(child);
        }
        else if(node->getParent()->getLeft() == node)
        {
            node->getParent()->setLeft(child);
        }
    }
    node->setParent(child);
    child->setLeft(node);
    node->setRight(childLeftChild);
    if(childLeftChild != NULL)
    {
        childLeftChild->setParent(node);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    if(node == NULL) //if node is NULL then return
    {
        return;
    }
    AVLNode<Key, Value>* child = node->getLeft(); //y
    AVLNode<Key, Value>* childRightChild = child->getRight(); //switched tree
    if(this->root_ == node) //node is root of tree
    {
        this->root_ = child;
        child->setParent(NULL);
    }
    else
    {
        child->setParent(node->getParent());
        if(node->getParent()->getRight() == node) //node is Right child of parent
        {
            node->getParent()->setRight(child);
        }
        else if(node->getParent()->getLeft() == node)
        {
            node->getParent()->setLeft(child);
        }
    }
    node->setParent(child);
    child->setRight(node);
    node->setLeft(childRightChild);
    if(childRightChild != NULL)
    {
        childRightChild->setParent(node);
    }
}

#endif
