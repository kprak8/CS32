//
//  Map.h
//  Maps2
//
//  Created by Rohan Prakash on 1/25/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <stdio.h>
#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
  public:
    Map();         // Create an empty map (i.e., one with no key/value pairs)
    Map(const Map &src); //copy constructor
    Map &operator= (const Map &src); //assignment operator
    ~Map(); //destructor
    bool empty()const;  // Return true if the map is empty, otherwise false.
    int size()const;    // Return the number of key/value pairs in the map.
    bool insert(const KeyType& key, const ValueType& value);
      // If key is not equal to any key currently in the map, add the key/value pair and //return true.
      // Otherwise, make no change to the map and return false (indicating
      // that either the key is already in the map).
    bool update(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // Otherwise, make no change to the map and return false.
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value that it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // If key is not equal to any key currently in the map add the key/value pair and //return true.
    bool erase(const KeyType& key);
      // If key is equal to a key currently in the map, remove the key/value
      // pair with that key from the map and return true.  Otherwise, make
      // no change to the map and return false.
    bool contains(const KeyType& key)const;
      // Return true if key is equal to a key currently in the map, otherwise
      // false.
    bool get(const KeyType& key, ValueType& value)const;
      // If key is equal to a key currently in the map, set value to the
      // value in the map which that key maps to, and return true.  Otherwise,
      // make no change to the value parameter of this function and return
      // false.
    bool get(int i, KeyType& key, ValueType& value)const;
      // If 0 <= i < size(), copy into the key and value parameters the
      // key and value at the ith position in the map and return
      // true.  Otherwise, leave the key and value parameters unchanged and
      // return false.
    void swap(Map& other);
      // Exchange the contents of this map with the other one.
    void dump() const; //printing function for testing purposes
private:
    struct Element{
        KeyType k;
        ValueType v;
        Element* next;
        Element* prev;
    };
    Element *head;
    Element *tail;
    int map_size;
    Element* search(const KeyType& key)const;
    void clear();
};
bool combine(const Map& m1, const Map& m2, Map& result); //combines two maps into result
void reassign(const Map& m, Map& result); //switches around the values of each element in the resulting map

#endif /* Map_h */
