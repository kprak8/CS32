//
//  Map.cpp
//  Maps2
//
//  Created by Komal Prakash on 1/25/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

#include "Map.h"
#include <iostream>
using namespace std;

Map::Map(){ //default constructor
    head = new Element();   //intializes dummy head
    tail = new Element();   //initializes dummy tail
    
    head->next=tail;    //points head to tail
    head->prev=nullptr;
    tail->next=nullptr;
    tail->prev=head; //points tail to head
    map_size=0;
}
Map::Map(const Map &src){ //copy constructor
    head = new Element(); //constructs new Map in which to put copy
    tail = new Element();
    
    head->next=tail;
    head->prev=nullptr;
    tail->next=nullptr;
    tail->prev=head;
    map_size=0;
    
    Element *p=(src.head)->next;    //create pointer variable to traverse
    while (p!=src.tail){    //stop when you reach dummy tail
        insert(p->k,p->v);  //insert the value and tail from source
        p=p->next;          //point to next element in source
    }
}

Map& Map::operator= (const Map &src){ //assignment operator
    clear();    //calls function to delete everything in target Map
    Element *p=(src.head)->next; //create pointer to traverse src
    while (p!=src.tail){
        insert(p->k,p->v);  //add each key and val in each element of src into target
        p=p->next;
    }
    map_size=src.map_size; //make target size equal to src size
    return (*this); //return target map
}

Map::~Map(){ //destructs each element of the Map
    Element*p;
    p=head->next;
    while (p!=tail){ //deletes each element within map
        Element* n=p->next;
        delete p;
        p=n;
    }
    delete head; //delete the head and tail after inside of Map is destructed
    delete tail;
}
void Map::clear(){ //destructs Map without removing head and tail
    Element*p=head->next;
    while (p!=tail){
        Element* n=p->next;
        delete p;
        p=n;
    }
    head->next=tail;
    tail->prev=head;
}

bool Map::empty()const{ // Return true if the map is empty, otherwise false.
    return map_size==0;
}

int Map::size()const{ // Return the number of key/value pairs in the map.
    return map_size;
}

Map::Element* Map::search(const KeyType& key)const{ //traverses through map elements and
    Element* p=head->next;                          //checks if there is a key in the map
    while (p!=tail){                                //that matches parameter "key"
        if (p->k==key){
            return p; //returns the element, if it is found
        }
        p=p->next;
    }
    return nullptr; //returns nullptr if key not found
}

bool Map::insert(const KeyType& key, const ValueType& value){
    Element*p=search(key); //uses search function
    if (p== nullptr){ //if p==nullptr, the key did not exist in map, so need to add it
        Element *ptr= new Element(); //new element made
        ptr->k=key;
        ptr->v=value;
        ptr->next=tail; //element added to end, so points to tail
        ptr->prev=tail->prev;
        tail->prev=ptr; //make tail point to added element
        ptr->prev->next=ptr; //make the previous element point to added element
        map_size++;
        return true;
    }
    return false;
}
bool Map::update(const KeyType& key, const ValueType& value){
    Element*p=search(key);
    if(p!=nullptr){ //if p!=nullptr, key exists in list and needs to be updated
        p->v=value;
        return true;
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    if (!insert(key,value)){ //insert true, it will insert
        update(key,value);  //insert is false means key exists and needs to be updated
    }
    return true;
}

bool Map::erase(const KeyType& key){
    Element*p=search(key); //searches for presence of key in list
    if (p!=nullptr){
        p->prev->next=p->next; //points previous element to the one after the erased one
        p->next->prev=p->prev;  //points next element to the one before the erased one
        delete p;
        map_size--;
        return true;
        
    }
    return false;
}

bool Map::contains(const KeyType& key)const{  // Return true if key is equal to a key
    return (search(key)!=nullptr);            //currently in the map, otherwise false.
}

bool Map::get(const KeyType& key, ValueType& value)const{
    Element*p=search(key);
    if (p!=nullptr){
        value=p->v; //when key is there,put its value into variable given
        return true;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value)const{
    if(i>=0&&i<map_size){ //checks that i inputted is less than size and greater than 0
        Element*p=head->next; //p points to first element
        for(int n=0; n<=i; n++){
            if(i==n){ //puts the key and value at position i into the parameter variables
                key= p->k;
                value= p->v;
                return true;
            }
            p=p->next;
        }
    }
    return false;
}

void Map::swap(Map& other){
    Element* temp1 = other.head; //temp for other dummy head
    Element* temp2 = other.tail; //temp for other dummy tail
    other.head=this->head; //switch the head and tail of other to those of this object
    other.tail=this->tail;
    this->head=temp1;
    this->tail=temp2;
    int tempSize;
    tempSize=other.map_size;
    other.map_size=this->map_size; //switch the map sizes of the two
    this->map_size=tempSize;
}

bool combine(const Map& m1, const Map& m2, Map& result){
    while(result.size()>0){ //while result is not empty, erase each element
        KeyType key;
        ValueType val;
        result.get(0,key,val);
        result.erase(key);
    }
    result=m1;  //add everything from m1 into result
    KeyType key;
    ValueType m2Val,m1Val;
    bool output=true;
    
    for (int i=0; i<m2.size();i++){ //continues through all elements of m2
        m2.get(i,key,m2Val);
        if (result.get(key,m1Val)){ //checks if key in m2 is in m1
            if (m2Val!=m1Val){ //if vals are not equal, erase element
                result.erase(key);
                output=false;
            }
        }
        else{
            result.insert(key,m2Val); //if key in m2 not in m1, insert it
        }
    }
    return output;
}
void reassign(const Map& m, Map& result){
    while(result.size()>0){ //while result is not empty, erase each element
        KeyType key;
        ValueType val;
        result.get(0,key,val);
        result.erase(key);
    }
    if (m.empty()){ //return empty result
        return;
    }
    KeyType key,prevKey,lastKey;
    ValueType val,prevVal,lastVal;
    
    m.get(0,prevKey,prevVal);
    m.get(m.size()-1,lastKey,lastVal);
    result.insert(prevKey,lastVal); //insert last element val into first element key
    for (int i=1;i<m.size();i++){ //for all other elements, inserts previous element's
        m.get(i,key,val);         //value into next element
        result.insert(key,prevVal);
        prevVal=val;
    }
}

void Map::dump() const{ //dump for testing purposes
    Element *p=head->next;
    cerr<<"----start\n";
    while (p!=tail){
        cerr<<p->k<<" "<<p->v;
        cerr<<"\n";
        p=p->next;
    }
    cerr<<"-----end\n";
}
