//
//  Map.cpp
//  Map
//
//  Created by Komal Prakash on 1/20/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

#include "Map.h"
#include <array>

Map::Map(){
    map_size=0;
}        // Create an empty map (i.e., one with no key/value pairs)
bool Map::empty()const{
    return map_size==0;
}  // Return true if the map is empty, otherwise false.

int Map::size()const{
    return map_size;
}    // Return the number of key/value pairs in the map.
bool Map::insert(const KeyType& key, const ValueType& value){
    if (map_size==DEFAULT_MAX_ITEMS){
        return false;
    }
    for(int i=0;i<map_size; i++){
        if (myMap[i].k==key){
            return false;
        }
    }
    myMap[map_size].k=key;
    myMap[map_size].v=value;
    map_size++;
    return true;
} // If key is not equal to any key currently in the map, and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that either the key is already in the map, or the map has a fixed
  // capacity and is full).
bool Map::update(const KeyType& key, const ValueType& value){
    for(int i=0;i<map_size; i++){
        if (myMap[i].k==key){
            myMap[i].v=value;
            return true;
        }
    }
    return false;
}
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // Otherwise, make no change to the map and return false.
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    for(int i=0;i<map_size; i++){
        if (myMap[i].k==key){
            myMap[i].v=value;
            return true;
        }
    }
    if (map_size<DEFAULT_MAX_ITEMS){
        myMap[map_size].k=key;
        myMap[map_size].v=value;
        map_size++;
        return true;
    }
    return false;
}
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value that it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // If key is not equal to any key currently in the map and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that the key is not already in the map and the map has a fixed
  // capacity and is full).
bool Map::erase(const KeyType& key){
    for(int i=0;i<map_size; i++){
        if (myMap[i].k==key){
            for (int j=i+1; j<map_size; i++) {
                myMap[j-1]=myMap[j];
            }
            map_size--;
            return true;
        }
    }
    return false;
}
  // If key is equal to a key currently in the map, remove the key/value
  // pair with that key from the map and return true.  Otherwise, make
  // no change to the map and return false.
bool Map::contains(const KeyType& key)const{
    for(int i=0;i<map_size; i++){
        if (myMap[i].k==key){
            return true;
        }
    }
    return false;
}
  // Return true if key is equal to a key currently in the map, otherwise
  // false.
bool Map::get(const KeyType& key, ValueType& value)const{
    for(int i=0;i<map_size; i++){
        if (myMap[i].k==key){
            value=myMap[i].v;
            return true;
        }
    }
    return false;
}
  // If key is equal to a key currently in the map, set value to the
  // value in the map which that key maps to, and return true.  Otherwise,
  // make no change to the value parameter of this function and return
  // false.
bool Map::get(int i, KeyType& key, ValueType& value)const{
    if(i>=0 && i<map_size){
        key=myMap[i].k;
        value=myMap[i].v;
        return true;
    }
    return false;
}
  // If 0 <= i < size(), copy into the key and value parameters the
  // key and value of one of the key/value pairs in the map and return
  // true.  Otherwise, leave the key and value parameters unchanged and
  // return false.
void Map::swap(Map& other){
    map_types temp1;
    for (int i=0; i<DEFAULT_MAX_ITEMS; i++){
        temp1=other.myMap[i];
        other.myMap[i]=myMap[i];
        myMap[i]=temp1;
    }
}
  // Exchange the contents of this map with the other one.

