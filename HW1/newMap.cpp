//
//  newMap.cpp
//  Map
//
//  Created by Komal Prakash on 1/20/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

#include "newMap.h"
#include <array>
Map::Map(){
    myMap = new map_types[DEFAULT_MAX_ITEMS];
    map_size=DEFAULT_MAX_ITEMS;
    occupied=0;
}
Map::Map(int n){
    myMap = new map_types[n];
    map_size=n;
    occupied=0;
}        // Create a map of size n, with nothing filled in.
Map::Map(const Map &src){
    myMap = new map_types[src.map_size];
    map_size=src.map_size;
    occupied = src.occupied;
    for (int i=0; i<map_size; i++){
        myMap[i]=src.myMap[i];
    }
} //copy constructor
Map& Map::operator= (const Map &src){
    delete [] myMap;
    myMap = new map_types[src.map_size];
    map_size=src.map_size;
    occupied = src.occupied;
    for (int i=0; i<map_size; i++){
        myMap[i]=src.myMap[i];
    }
    return (*this);
} //assignment operator

Map::~Map(){
    delete [] myMap;
}//destructs array
bool Map::empty()const{
    return occupied==0;
}  // Return true if the map is empty, otherwise false.

int Map::size()const{
    return occupied;
}    // Return the number of key/value pairs in the map.
bool Map::insert(const KeyType& key, const ValueType& value){
    if (occupied==map_size){
        return false;
    }
    for(int i=0;i<occupied; i++){
        if (myMap[i].k==key){
            return false;
        }
    }
    myMap[occupied].k=key;
    myMap[occupied].v=value;
    occupied++;
    return true;
} // If key is not equal to any key currently in the map, and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that either the key is already in the map, or the map has a fixed
  // capacity and is full).
bool Map::update(const KeyType& key, const ValueType& value){
    for(int i=0;i<occupied; i++){
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
    for(int i=0;i<occupied; i++){
        if (myMap[i].k==key){
            myMap[i].v=value;
            return true;
        }
    }
    if (occupied<map_size){
        myMap[occupied].k=key;
        myMap[occupied].v=value;
        occupied++;
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
    for(int i=0;i<occupied; i++){
        if (myMap[i].k==key){
            for (int j=i+1; j<occupied; i++) {
                myMap[j-1]=myMap[j];
            }
            occupied--;
            return true;
        }
    }
    return false;
}
  // If key is equal to a key currently in the map, remove the key/value
  // pair with that key from the map and return true.  Otherwise, make
  // no change to the map and return false.
bool Map::contains(const KeyType& key)const{
    for(int i=0;i<occupied; i++){
        if (myMap[i].k==key){
            return true;
        }
    }
    return false;
}
  // Return true if key is equal to a key currently in the map, otherwise
  // false.
bool Map::get(const KeyType& key, ValueType& value)const{
    for(int i=0;i<occupied; i++){
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
    if(i>=0 && i<occupied){
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
    Map temp1 = other;
    other=(*this);
    (*this)=temp1;
}
  // Exchange the contents of this map with the other one.

