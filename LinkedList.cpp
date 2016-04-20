//
//  LinkedList.cpp
//  LinkedList
//
//  Created by Jay Lu on 2016-04-19.
//  Copyright © 2016 Jay Lu. All rights reserved.
//

#include <stdio.h>
#include "LinkedList.h"

//Default constructor creates the head node.
LinkedList::LinkedList()
{
//    head -> data = NULL;
    head -> next = NULL;
    listLength = 0;
}

//Add node to the list at a given position.
//Return true if successful
bool LinkedList::insertNode(node * newNode, int position)
{
    if (position<=0 || position > listLength+1)
    {
        cout<< "Error: the given position is out of range"<<endl;
        return false;
    }
    
    if (head -> next == NULL)
    {
        head -> next = newNode;
        listLength++;
        return true;
    }
    
    int count = 0;
    node * p = head;
    node * q = head;
    while(q)
    {
        if (count == position)   //add newNode to empty linked list
        {
            p -> next = newNode;
            newNode -> next = q;
            listLength++;
            return true;
        }
        p = q;
        q = p -> next;
        count++;
    }
    if (count == position)   //add newNode to not empty linked list
    {
        p -> next = newNode;
        newNode -> next =q;
        listLength++;
        return true;
    }
    cout<<"Error: node was not added to list"<<endl;
    return false;
}

//Remove a node by its given position
//Return true is successful
bool LinkedList::removeNode(int position)
{
    if(position <= 0 || position > listLength +1)
    {
        cout<<"Error: the given position is out of range."<<endl;
        return false;
    }
    if(head -> next == NULL)
    {
        cout<<"Error: the is nothing in the list to remove."<<endl;
        return false;
    }
    int count = 0;
    node * p = nullptr;
    node * q = nullptr;
    while(q)
    {
        if(count == position)
        {
            p -> next = q -> next;
            delete q;
            listLength--;
            return true;
        }
        p = q;
        q = p -> next;
        count++;
    }
    
    cout<<"Error: nothing was removed from the list"<<endl;
    return false;
}

//Print the linked list
void LinkedList::printList()
{
    node * p = head;
    node * q = head;
    int count = 0;
    
    cout<<"-----------------------"<<endl;
    cout<<"Linked list"<<endl;
    while(q)
    {
        p = q;
        cout<<"-----------------------"<<endl;
        cout<<"position: "<< count <<endl;
        cout<<"data: "<< p -> data <<endl;
        q = p -> next;
        count++;
    }
}

//Destructor de-allocates memory used by the list
LinkedList::~LinkedList()
{
    node * p = head;
    node * q = head;
    while(q)
    {
        p = q;
        q = p -> next;
        if (q) delete p;
    }
}




