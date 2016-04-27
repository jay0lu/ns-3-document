/*
 * controllerCheck.h
 *
 *  Created on: Apr 21, 2016
 *      Author: ns3
 */

//
// Created by Jay Lu on 2016-04-21.
// Copyright © 2016 Jay Lu. All rights reserved.
//

#ifndef OPENFLOW_CONTROLLER_CONTROLLERCHECK_H_
#define OPENFLOW_CONTROLLER_CONTROLLERCHECK_H_

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

void insert(string ipv6);

void remove(string ipv6);

//bool isInTable(string ipv6);

void display();

void reverse();

bool search(string searchIp);

//Destructor de-allocates memory used by the list
~LinkedList();

};


#endif /* OPENFLOW_CONTROLLER_CONTROLLERCHECK_H_ */
