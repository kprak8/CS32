//
//  testnewMap.cpp
//  Map
//
//  Created by Komal Prakash on 1/20/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

#include <stdio.h>

#include "newMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k[6] = {"abc","def","ghi","jkl","mno","pqr"};
    ValueType v  = 41;
    // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k[n], v));

        // Failure if we try to insert a pair with a sixth distinct key into b
        //assert(!b.insert(k[5], v));

        // When two Maps' contents are swapped, their capacities are swapped
        // as well:
        a.swap(b);
        assert(!a.insert(k[5], v)  &&  b.insert(k[5], v));
}


