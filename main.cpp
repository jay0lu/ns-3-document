//
//  main.cpp
//  LinkedList
//
//  Created by Jay Lu on 2016-04-19.
//  Copyright © 2016 Jay Lu. All rights reserved.
//

#include <iostream>
#include "LinkedList.h"

int main(int argc, const char * argv[]) {
    //create some unlinked node
    node * a = new node;
    a -> data = 1;
    
    node * b = new node;
    b -> data = 2;
    
    node * c = new node;
    c -> data = 3;
    
    node * d = new node;
    d -> data = 4;
    
    node * e = new node;
    e -> data = 5;
    
    node * f = new node;
    f -> data = 6;
    
    //build list
    LinkedList myList;
    myList.insertNode(a, 1);
    myList.insertNode(b, 2);
    myList.insertNode(c, 3);
    myList.insertNode(d, 4);
    myList.printList();
    
    //insert a node in to middle
    myList.insertNode(e, 2);
    myList.printList();
    
    //remove the last node
    myList.removeNode(3);
    myList.printList();
    
    return 0;
    
}
