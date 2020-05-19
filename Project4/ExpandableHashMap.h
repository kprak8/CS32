// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#include <iostream>
#include <unordered_map>

using namespace std;
template<typename KeyType, typename ValueType>

class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;
    
private:
    struct Node{
        Node(KeyType key, ValueType value)
            : myKey(key), myVal(value), next(nullptr)
        {}
        KeyType myKey;
        ValueType myVal;
        Node* next;
    };
    //helper func
    unsigned int getBucketNum(const KeyType& k) const;
    
    Node **myTable; //array of pointers
    double myMaxLoadFactor; //will be the max load factor with a default max of 0.5
    int occupiedItems; //how many items are filled into map
    int bucketNum; //how many buckets there are
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
    :bucketNum(8),occupiedItems(0)
{
    myTable = new Node* [bucketNum]; //make table with 8 buckets
    
    for(int i = 0; i < bucketNum; i++){ //make all elements of table unassociated
        myTable[i] = nullptr;
    }
    
    if (maximumLoadFactor<=0){ //if load inputted not positive, make loadFactor = 0.5
        myMaxLoadFactor=0.5;
    }
    else{ //otherwise use inputted value for load factor maximum
        myMaxLoadFactor=maximumLoadFactor;
    }

}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    for(int i = 0; i < bucketNum; i++){
        if(myTable[i]!=nullptr){
            Node *p = myTable[i]; //destruction alogrithm for linked list but instead of head ptr, you start with first node at each element of the table
            while(p!= nullptr){
                Node* n = p->next;
                delete p;
                p=n;
            }
        }
    }
    delete [] myTable;
}
template <typename KeyType,typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset() // resets the hashmap back to 8 buckets, deletes all items
{
    for(int i = 0; i < bucketNum; i++){
        if(myTable[i]!=nullptr){
            Node *p = myTable[i]; //destruction alogrithm for linked list but instead of head ptr, you start with first node at each element of the table
            while(p!= nullptr){
                Node* n = p->next;
                delete p;
                p=n;
            }
        }
    }
    delete [] myTable; //^^destructor code
    
    bucketNum = 8; //newly constructed table must have 8 buckets
    occupiedItems=0; //no items have been added yet
    myTable = new Node* [bucketNum]; //make table with 8 buckets
    
    for(int i = 0; i < bucketNum; i++){ //make all elements of table unassociated
        myTable[i] = nullptr;
    }
}
template <typename KeyType,typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const // return the number of associations in the hashmap
{
    return occupiedItems; //returns number of items added to table (associated items)
}

// The associate method associates one item (key) with another (value).
// If no association currently exists with that key, this method inserts
// a new association into the hashmap with that key/value pair. If there is
// already an association with that key in the hashmap, then the item
// associated with that key is replaced by the second parameter (value).
// Thus, the hashmap must contain no duplicate keys.
template <typename KeyType,typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    ValueType* val=find(key);
    if (val!=nullptr){ //if not nullptr, then it exists already, so change val to inputted value
        *val=value;
    }
    else {
        Node* n = new Node(key, value);
        unsigned int index= getBucketNum(key); //use hasher to find the index associated with key
        Node* bucket = myTable[index];
        if (bucket==nullptr){ //if no other nodes at that index, just add n to that spot
            myTable[index]=n;
        }
        else{
            n->next=myTable[index]; //otherwise add this node to the front and point it to the one that was previously at that index
            myTable[index]=n;
        }
        occupiedItems++; //increase how many items are in the table
    }
    double currLoadFactor= occupiedItems / bucketNum; //calculate current load factor
    if (currLoadFactor>myMaxLoadFactor){
        int oldBucketNum=bucketNum; //store old bucket size in variable
        bucketNum = 2 * bucketNum; //create a new map with twice the bucket amount
        Node** biggerTable = new Node*[bucketNum]();

        for (int i = 0; i < oldBucketNum; i++) //traverse through table buckets
        {
            Node* p = myTable[i];
            while (p != nullptr)  //for each bucket, duplicate the nodes and add them to the bigger table
            {
                Node* temp = p->next;
                Node* newNode = new Node(p->myKey, p->myVal);
                int newIndex = getBucketNum(p->myKey);
                newNode->next = biggerTable[newIndex]; //the new node's next pointer will point to the node that is already there
                biggerTable[newIndex] = newNode; //the new node will take the spot at the index (the new "head" pointer)

                delete p; //delete the old, smaller table's node
                p = temp; //pointer p now points to the next node in the old table
            }
        }
        delete[] myTable; //deletes old, smaller table
        myTable = biggerTable; //points myTable to the new, bigger table
    }
}

// If no association exists with the given key, return nullptr; otherwise,
// return a pointer to the value associated with that key. This pointer can be
// used to examine that value, and if the hashmap is allowed to be modified, to
// modify that value directly within the map (the second overload enables
// this). Using a little C++ magic, we have implemented it in terms of the
// first overload, which you must implement.
template <typename KeyType,typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int index= getBucketNum(key); //use hasher to find the index associated with key
    Node* p = myTable[index]; //start at that index
    while (p!=nullptr){//check down the linked list at that index for the desired key
        if (p->myKey==key){ //if found, return the val
            ValueType* value = &(p->myVal);
            return value;
        }
        p = p->next;
    }
    return nullptr; //if not found, return nullptr
}

//Helper function, getBucketNum
template<typename KeyType, typename ValueType>
unsigned int ExpandableHashMap<KeyType, ValueType>::getBucketNum(const KeyType& key) const
{
    unsigned int hasher(const KeyType& k); // prototype
    unsigned int h = hasher(key);
    return h % bucketNum;
}


