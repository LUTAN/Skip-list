//
//  SkipList.h
//  SkipList
//
//  Created by lutan on 14-7-25.
//  Copyright (c) 2014年 lutan. All rights reserved.
//

#ifndef __SkipList__SkipList__
#define __SkipList__SkipList__

#include <iostream>
#include <iterator>     // std::iterator, std::input_iterator_tag
#include <vector>
#include <list>



using namespace std;

struct SkipListNode {
    int data;
    SkipListNode *nextLevel;
    SkipListNode *lowerLevel;
    SkipListNode *nextNode;
    SkipListNode *preNode;
};

class SkipListIterator : public std::iterator<std::input_iterator_tag, SkipListNode>
{
    SkipListNode* node;
public:
    SkipListIterator(SkipListNode* x) :node(x) {}
    SkipListIterator(const SkipListIterator& mit) : node(mit.node) {}
    SkipListIterator& operator++() {node = node->nextNode; return *this;}
    SkipListIterator& operator+(int a) {for(int i = 0; i < a; ++i)operator++(); return *this;}
    bool operator==(const SkipListIterator& rhs) {return node == rhs.node;}
    bool operator!=(const SkipListIterator& rhs) {return node != rhs.node;}
    int operator*() {return node->data;}
};

//-----------------------------------------------------skiplist----------------------------------------------------------
class SkipList
{
private:
    vector<SkipListNode*> heads;
    
public:
    SkipList(){};
    SkipList(SkipListNode node);
    SkipList(int a);
    SkipList(const SkipList &skipList);
    ~SkipList();
    
    void operator=(const SkipList &skipList);
    void addNode(const SkipListNode &node);
    void addNode(int a);
    void removeNode(int a);
    void removeAllNode();
    void printAllNodes();
    
    SkipListIterator begin() const;
    SkipListIterator end() const;
};

#endif /* defined(__SkipList__SkipList__) */
