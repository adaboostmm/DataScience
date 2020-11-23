/*
* This file provides the implementation for reversing a vector of integers
* using LinkedList and Stack. This populates the linked list first from the
* vector of integers, push the integers from linked list to a stack abd then
* pop it off from the stack and you get a Linked List of reverse integers.
* This program uses Stanford C++ libraries.
*/
#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "node.h"
#include "error.h"
#include "stack.h"
#include "set.h"

using namespace std;

// A helper function to print a linked list
void printList(Node* list){
    for (Node* cur = list; cur != nullptr; cur = cur->next){
        cout << cur->data << endl;
    }
}

// This function builds a linked list from the vector of integers
Node*  createListFromVector(Vector<int> nums){
    if (nums.isEmpty()) return nullptr;
    Node* head = new Node;
    head->data = nums[0];
    head->next = nullptr;

    Node* cur = head;
    for (int i = 1; i < nums.size(); i++){
        Node* newNode = new Node;
        newNode->data = nums[i];
        newNode->next = nullptr;
        cur->next = newNode;
        cur = newNode;
    }

    return head;

}

// This function pushes the integers to the stack from linked list.
void pushIntoStack(Node*& front, Stack<int> &numStack){
    Node* head = front;
    while (head !=  nullptr) {
            numStack.push(head->data);
            head = head->next;
    }
}

//This function pops the integers from the stack. We get the inrtegers in reverse
// order
void popFromStack(Node*& front, Stack<int> &numStack){
    Node* head = front;
    Node* cur = head;

    while (!numStack.isEmpty()){
        Node* newNode = new Node;
        newNode->data = numStack.pop();
        newNode->next = nullptr;
        cur->next = newNode;
        cur = newNode;
    }
    head = head->next;
    front = head;
}

// Primary function that calls other helper functions to reverse a vector
// of integers.
void reverse(Node*& front){
    (void) front;
    Stack<int> numStack;
    pushIntoStack(front, numStack);
    popFromStack(front, numStack);
}


//Test function
STUDENT_TEST("Test push into stack"){
    Node* vecList = createListFromVector({2,5,6,7});
    cout << "Original " << endl;
    printList(vecList);
    reverse(vecList);
     cout << "reversed " << endl;
    printList(vecList);
}

