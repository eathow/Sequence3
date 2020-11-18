//
//  sequence3.cpp
//  DS Project 4
//
//  Created by Emma Athow on 3/19/20.
//  Copyright © 2020 Emma Athow. All rights reserved.
//  Invariant for private member variables:
//  1) many_nodes - keeps track of the number of nodes in the list like used
//  2) head_ptr - the head pointer of the linked list. If the sequence has no items, then these pointers are both NULL, like data
//  3) tail_ptr - the tail pointer of the linked list. If the sequence has no items, then these pointers are both NULL
//  4) cursor - points to hte node with the current item (or NULL if there is no current item)
//  5) precursor - points to the node before the current item (or NULL if there is no current item or if the current item is the first node)

#include "sequence3.h"
#include <cstdlib>
#include <cassert>
using namespace std;


namespace main_savitch_5 {
sequence :: sequence() {
//Using this constructor, we initialize private member variables
//Big-0 Notation: O(1)
    head_ptr = NULL;
    tail_ptr = NULL;
    precursor = NULL;
    cursor = NULL;
    many_nodes = 0;
}
sequence :: sequence(const sequence& source) {
//Here we are copying all data from source into our dynamic array, data
//Big-0 Notation: O(n)
    list_copy(source.head_ptr, head_ptr, tail_ptr);
    many_nodes = source.many_nodes;
    node *copy_ptr;
    copy_ptr = source.head_ptr;
    precursor = head_ptr;
    if(source.cursor == NULL) {
        if(source.precursor == NULL) {
            cursor = NULL;
            precursor = NULL;
        }
        else {
            precursor = tail_ptr;
            cursor = NULL;
        }
    }
    else {
        if(source.precursor == NULL) {
            cursor = head_ptr;
            precursor = NULL;
        }
        else {
            while(copy_ptr != source.precursor) {
                copy_ptr = copy_ptr->link();
                precursor = precursor->link();
            }
            cursor = precursor->link();
        }
    }
}

sequence :: ~sequence() {
//Destructor returns data to heap
//Big-0 Notation: O(n)
    list_clear(head_ptr);
    many_nodes = 0;
}

void sequence :: start() {
//Postcondition: The first item in the sequence becomes the current item (but if the sequence is empty, then there is no current item).
//Big-0 Notation: O(1)
    cursor = head_ptr;
    precursor = NULL;
}

void sequence :: advance() {
//Precondition: is_item returns true.
//Postcondition: If the current item was already the last item in the sequence, then there is no longer any current item. Otherwise, the new item is the item immediately after the original current item.
//Big-0 Notation: O(1)
    assert(is_item() == true);
    precursor = cursor;
    cursor = cursor->link();
}

void sequence :: insert(const value_type& entry) {
//We must change this function so that it adds the new item after the precursor node, which is just before the cursor node
//Precondition: size( ) < CAPACITY.
//Postcondition: A new copy of entry has been inserted in the sequence before the current item. If there was no current item, then the new entry has been inserted at the front. In either case, the new item is now the current item of the sequence.
//Big-0 Notation: O(1)
    if(is_item() == false) {
        list_head_insert(head_ptr, entry);
        many_nodes += 1;
        cursor = head_ptr;
    }
    else {
        if(cursor == head_ptr) {
            node *insert_ptr;
            insert_ptr = new node;
            insert_ptr->set_data(entry);
            insert_ptr->set_link(cursor);
            head_ptr = insert_ptr;
            cursor = insert_ptr;
            many_nodes += 1;
        }
        else {
            node *insert_ptr;
            insert_ptr = new node;
            insert_ptr->set_data(entry);
            insert_ptr->set_link(precursor->link());
            precursor->set_link(insert_ptr);
            cursor = insert_ptr;
            many_nodes += 1;
        }
    }
}

void sequence :: attach(const value_type& entry) {
//We must updat the attach function so that if there is no current node, the attach places its new item at the tail of the list
//Precondition: size( ) < CAPACITY.
//Postcondition: A new copy of entry has been inserted in the sequence after the current item. If there was no current item, then the new entry has been attached to the end of the sequence. In either case, the new item is now the current item of the sequence.
//Big-0 Notation: O(1)
    if(is_item() == false) {
        if(size() == 0) {
            list_head_insert(head_ptr, entry);
            precursor = NULL;
            many_nodes += 1;
            cursor = head_ptr;
            tail_ptr = head_ptr;
        }
        else {
            precursor = tail_ptr;
            list_insert(tail_ptr, entry);
            tail_ptr = tail_ptr->link();
            cursor = tail_ptr;
            many_nodes += 1;
        }
    }
    else {
        if(cursor->link() == NULL) {
            list_insert(cursor, entry);
            precursor = cursor;
            cursor = cursor->link();
            tail_ptr = cursor;
            many_nodes += 1;
        }
        else {
            list_insert(cursor, entry);
            precursor = cursor;
            cursor = cursor->link();
            many_nodes += 1;
        }
    }
}

void sequence :: remove_current() {
//Precondition: is_item returns true.
//Postcondition: The current item has been removed from the sequence, and the item after this (if there is one) is now the new current item.
//Big-0 Notation: O(1)
    assert(is_item() == true);
    if(precursor != NULL) {
        cursor = cursor->link();
        list_remove(precursor);
        many_nodes -= 1;
    }
    else {
        list_head_remove(head_ptr);
        cursor = head_ptr;
        many_nodes -= 1;
    }
}

void sequence :: operator=(const sequence& source){
//Big-0 Notation: O(n)
    if(this == &source) {
    //Here we first stop self-assignment in its tracks
        return;
   }
    list_clear(head_ptr);
    many_nodes = 0;
    many_nodes = source.many_nodes;
    list_copy(source.head_ptr, head_ptr, tail_ptr);
    node *copy_ptr;
    copy_ptr = source.head_ptr;
    precursor = head_ptr;
    if(source.cursor == NULL) {
        if(source.precursor == NULL) {
            cursor = NULL;
            precursor = NULL;
        }
        else {
            precursor = tail_ptr;
            cursor = NULL;
        }
    }
    else {
        if(source.precursor == NULL) {
            cursor = head_ptr;
            precursor = NULL;
        }
        else {
            while(copy_ptr != source.precursor) {
                copy_ptr = copy_ptr->link();
                precursor = precursor->link();
            }
            cursor = precursor->link();
        }
    }
}

sequence :: size_type sequence :: size() const {
 //Postcondition: The return value is the number of items in the sequence.
 //Big-0 Notation: O(1)
   return many_nodes;
}

bool sequence :: is_item() const {
//Postcondition: A true return value indicates that there is a valid “current” item that may be retrieved by the current member function (listed below). A false return value indicates that there is no valid current item.
//Big-0 Notation: O(1)
    if(head_ptr == NULL || cursor == NULL) {
        //This is necessary because we need to make sure there is a current number to begin with
        return false;
    }
    else {
        return true;
    }
}

sequence :: value_type sequence :: current() const {
 //Precondition: is_item( ) returns true.
 //Postcondition: The item returned is the current item in the sequence.
 //Big-0 Notation: O(1)
    assert(is_item() == true);
    return cursor->data();
}
}
