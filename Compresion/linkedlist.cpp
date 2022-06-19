#include "linkedlist.h"

Linkedlist::Linkedlist()
{
    lastNode = nullptr;

}

Linkedlist::~Linkedlist()
{
    Node* temp;
    while(lastNode != nullptr){
        temp = lastNode;
        lastNode = lastNode->prev;
        delete temp;
    }
}

void Linkedlist::PushBack(unsigned int val)
{
   Node* myNode = new Node;
   myNode->index = val;
   myNode->prev = lastNode;
   lastNode = myNode;

}
