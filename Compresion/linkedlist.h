#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include "node.h"

class Linkedlist
{
public:
    Linkedlist();
    ~Linkedlist();
    Node* lastNode;
    void PushBack(unsigned int val);


};

#endif // LINKEDLIST_H
