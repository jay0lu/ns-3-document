//
//  LinkedList.hpp
//  LinkedList
//
//  Created by Jay Lu on 2016-04-21.
//  Copyright © 2016 Jay Lu. All rights reserved.
//

#ifndef LinkedList_hpp
#define LinkedList_hpp

#include <iostream>
#include <string>

using namespace std;

//Node structs
struct Node
{
    int data;
    Node * next;
};

class LinkedList
{
private:
    //linked list header, pointer that point to the linked list
    Node * head;
    Node * current;
    Node * temp;
    int listLength;
    
public:
    //default constructor creates the head node.
    LinkedList();
    
    void insert(int add);
    
    void remove(int remove);
    
    void display();
    
    void reverse();
    
    void search(int searchNum);
    
    //Destructor de-allocates memory used by the list
    ~LinkedList();
};


#endif /* LinkedList_hpp */
