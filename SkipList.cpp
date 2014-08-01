//
//  SkipList.cpp
//  SkipList
//
//  Created by lutan on 14-7-25.
//  Copyright (c) 2014年 lutan. All rights reserved.
//

#include "SkipList.h"


//-----------------------------------------------implementation of skiplist---------------------------------------------------
void deleteAnElement(SkipListNode *node)
{
    if (node->preNode) {
        (node->preNode)->nextNode = node->nextNode;
    }
    
    if (node->nextNode) {
        (node->nextNode)->preNode = node->preNode;
    }
    delete node;
}

void linkFirstTwoNodes(SkipListNode *head, SkipListNode* n, int a)
{
    SkipListNode *postLastNode = new SkipListNode;
    head->lowerLevel = NULL;
    head->nextLevel = NULL;
    head->data = INT_MIN;
    n->lowerLevel = NULL;
    n->nextLevel = NULL;
    n->data = a;
    head->preNode = NULL;
    head->nextNode = n;
    n->nextNode = postLastNode;
    n->preNode = head;
    
    postLastNode->data = INT_MAX;
    postLastNode->lowerLevel = NULL;
    postLastNode->nextLevel = NULL;
    postLastNode->nextNode = NULL;
    postLastNode->preNode = n;
}



//////------------------------------------------------constructor and destructor-------------------------------------------------------
SkipList::SkipList(SkipListNode node)
{
    SkipListNode *head = new SkipListNode;
    SkipListNode *n = new SkipListNode;
    linkFirstTwoNodes(head, n, node.data);
    heads.push_back(head);
    srand(time(NULL));
}

SkipList::SkipList(int a)
{
    SkipListNode *head = new SkipListNode;
    SkipListNode *n = new SkipListNode;
    linkFirstTwoNodes(head, n, a);
    heads.push_back(head);
    srand(time(NULL));
}

SkipList::SkipList(const SkipList &skipList)
{
    srand(time(NULL));
    *this = skipList;
}

SkipList::~SkipList()
{
    removeAllNode();
}
//////----------------------------------------------------------------------------------------------------------------------



//////---------------------------------------overloading of operators-------------------------------------------------------
void SkipList::operator=(const SkipList &skipList)
{
    removeAllNode();
    for(SkipListIterator it = skipList.begin(); it != skipList.end(); ++it){
        addNode(*it);
    }
}
//////----------------------------------------------------------------------------------------------------------------------




//////---------------------------------------iterator of the skiplist-------------------------------------------------------
SkipListIterator SkipList::begin() const
{
    int numberOfLevel = heads.size();
    if (numberOfLevel == 0) {
        return NULL;
    }
    return SkipListIterator(heads[0]->nextNode);
}

SkipListIterator SkipList::end() const
{
    int numberOfLevel = heads.size();
    if (numberOfLevel == 0) {
        return NULL;
    }
    
    SkipListNode *lastNode = heads[numberOfLevel - 1];
    while (lastNode) {
        if (lastNode->nextNode) {
            lastNode = lastNode->nextNode;
        }
        else{
            if (lastNode->lowerLevel) {
                lastNode = lastNode->lowerLevel;
            }
            else
            {
                break;
            }
        }
    }
    return SkipListIterator(lastNode);
}
///-------------------------------------------------------------------------------------------------------------------------------



///-------------------------------------------------mulipulation of skiplist-----------------------------------------------------
void SkipList::addNode(int a)
{
    SkipListNode node;
    node.data = a;
    addNode(node);
}

void SkipList::addNode(const SkipListNode &node)
{
    SkipListNode *n = new SkipListNode;
    n->lowerLevel = NULL;
    n->nextLevel = NULL;
    n->data = node.data;
    //SkipList
    int numberOfLevel = heads.size();
    if (numberOfLevel == 0) {
        SkipListNode *head = new SkipListNode;
        linkFirstTwoNodes(head, n, node.data);
        heads.push_back(head);
        return;
    }
    else{
        SkipListNode *tmpNode = heads.at(numberOfLevel - 1);
        
        bool findGreaterOne = false;
        while (!findGreaterOne) {
            while (tmpNode->nextNode != NULL) {
                if ((tmpNode->nextNode)->data > node.data) {
                    //find the first node whose data is greater
                    findGreaterOne = true;
                    if (tmpNode->lowerLevel) {
                        tmpNode = tmpNode->lowerLevel;
                        continue;
                    }
                    else{
                        //if only there is only one level;
                        SkipListNode *next = tmpNode->nextNode;
                        n->nextNode = next;
                        n->preNode = tmpNode;
                        next->preNode = n;
                        tmpNode->nextNode = n;
                        break;
                    }
                }
                tmpNode = tmpNode->nextNode;
            }
            
            if (!findGreaterOne) {
                if (tmpNode->lowerLevel) {
                    tmpNode = tmpNode->lowerLevel;
                }
                else{
                    findGreaterOne = true;
                    //if only there is only one level;
                    n->nextNode = NULL;
                    n->preNode = tmpNode;
                    tmpNode->nextNode = n;
                }
            }
        }
    }
    
    SkipListNode *tmp = n, *tmpNode = n;
    int insertionLevelNumber = 0;
    while (rand() < (RAND_MAX / 2)) {
        if (insertionLevelNumber == numberOfLevel - 1) {
            SkipListNode *n1 = new SkipListNode;
            n1->lowerLevel = tmpNode;
            tmpNode->nextLevel = n1;
            n1->nextLevel = NULL;
            n1->data = node.data;
            
            SkipListNode *head1 = new SkipListNode;
            SkipListNode *head = heads.at(numberOfLevel - 1);
            head1->lowerLevel = head;
            head->nextLevel = head1;
            head1->nextLevel = NULL;
            head1->data = INT_MIN;
            head1->preNode = NULL;
            head1->nextNode = n1;
            n1->nextNode = NULL;
            n1->preNode = head1;
            heads.push_back(head1);
            break;
        }
        
        while ((tmp->nextLevel) == NULL) {
            tmp = tmp->preNode;
        }
        tmp = tmp->nextLevel;
        bool find = true;
        while (tmp->data < node.data) {
            if (tmp->nextNode) {
                tmp = tmp->nextNode;
            }
            else{
                find = false;
                break;
            }
        }
        
        SkipListNode *n1 = new SkipListNode;
        n1->data = node.data;
        n1->nextLevel = NULL;
        n1->lowerLevel = tmpNode;
        tmpNode->nextLevel = n1;
        tmpNode = n1;

        if (find) {
            SkipListNode *tmpPre = tmp->preNode;
            tmpPre->nextNode = n1;
            n1->preNode = tmpPre;
            n1->nextNode = tmp;
            tmp->preNode = n1;
        }
        else{
            n1->nextNode = NULL;
            n1->preNode = tmp;
            tmp->nextNode = n1;
        }
        insertionLevelNumber ++;
    }
}

void SkipList::removeNode(int a)
{
    int numberOfLevel = heads.size();
    if (numberOfLevel == 0) {
        printf("error:empty skiplist\n");
        return;
    }
    SkipListNode *head = heads[numberOfLevel - 1];
    bool hasIt = true;
    while (head->data != a && hasIt) {
        while (head->data < a) {
            if (head->nextNode) {
                head = head->nextNode;
            }
            else{
                if (head->lowerLevel) {
                    head = head->lowerLevel;
                    if (head->nextNode) {
                        head = head->nextNode;
                    }
                }
                else{
                    printf("no element whose value is %d\n", a);
                    hasIt = false;
                    break;
                }
            }
        }
        if (hasIt && head->data != a) {
            head = head->preNode;
            //find an element whose data is greater than a, so we dive into lower level;
            if (head->lowerLevel) {
                head = head->lowerLevel;
            }
            else{
                hasIt = false;
                printf("no element whose value is %d\n", a);
                break;
            }
        }
        else{
            //break because it finds an elements whose value is a or there is no such element
            break;
        }
    }
    
    if (hasIt) {
        //find one
        SkipListNode *nextLevel = head->nextLevel;
        SkipListNode *lowerLevel = head->lowerLevel;
        deleteAnElement(head);
        
        while (nextLevel) {
            SkipListNode *tmp = nextLevel->nextLevel;
            deleteAnElement(nextLevel);
            nextLevel = tmp;
        }
        while (lowerLevel) {
            SkipListNode *tmp = lowerLevel->lowerLevel;
            deleteAnElement(lowerLevel);
            lowerLevel = tmp;
        }
        
        //remove empty levels from top to bottom
        for (int i = numberOfLevel - 1; i > -1; --i) {
            SkipListNode *head = heads[i];
            if (head->nextNode == NULL) {
                heads.pop_back();
                delete head;
            }
            else{
                if (head->nextNode->data == INT_MAX) {
                    heads.pop_back();
                    delete head->nextNode;
                    delete head;
                }
                break;
            }
        }
    }
    
}

void SkipList::removeAllNode()
{
    int numberOfLevel = heads.size();
    for (int i = numberOfLevel - 1; i > -1; --i) {
        SkipListNode *head = heads[i];
        while (head) {
            SkipListNode *tmp = head->nextNode;
            delete head;
            head = tmp;
        }
        heads.pop_back();
    }
}
///------------------------------------------------------------------------------------------------------------------------


///-----------------------------------------------print all nodes just for debug--------------------------------------------------------------
void SkipList::printAllNodes()
{
    int numberOfLevel = heads.size();
    for (int i = 0; i < numberOfLevel; ++i) {
        SkipListNode *head = heads[i];
        head = head->nextNode;
        while (head) {
            SkipListNode *tmp = head->nextNode;
            printf("%d-", head->data);
//            if (head->nextLevel && head->lowerLevel) {
//                printf("%d(%d,%d)-", head->data, head->nextLevel->data, head->lowerLevel->data);
//            }
//            else if(head->nextLevel){
//                printf("%d(%d,%d)-", head->data, head->nextLevel->data, -9);
//            }
//            else if(head->lowerLevel){
//                printf("%d(%d,%d)-", head->data, -9, head->lowerLevel->data);
//            }
//            else{
//                printf("%d(%d,%d)-", head->data, -9, -9);
//            }
            head = tmp;
        }
        printf("\n");
    }
}
