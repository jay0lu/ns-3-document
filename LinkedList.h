//
//  LinkedList.h
//  LinkedList
//
//  Created by Jay Lu on 2016-04-19.
//  Copyright © 2016 Jay Lu. All rights reserved.
//

#ifndef LinkedList_h
#define LinkedList_h

#include <iostream>
#include <string>

using namespace std;

//Node structs
struct node
{
    int data;
    node * next;
};

class LinkedList
{
private:
    //linked list header, pointer that point to the linked list
    node * head;
    int listLength;
    
public:
    //default constructor creates the head node.
    LinkedList();
    
    bool insertNode(node * newNode, int position);
    
    bool removeNode(int position);
    
    void printList();
    
    //Destructor de-alocates memory used by the list
    ~LinkedList();
};

#endif /* LinkedList_h */
