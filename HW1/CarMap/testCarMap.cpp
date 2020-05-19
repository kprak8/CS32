//
//  testCarMap.cpp
//  Map
//
//  Created by Komal Prakash on 1/20/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

#include <stdio.h>

#include "Map.h"
#include "CarMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    CarMap m;  // maps strings to doubles
    assert(m.addCar("12345") && m.miles("12345")==0);
    m.addCar("6789");
    m.drive("12345",65);
    assert(m.fleetSize()==2 && m.miles("12345")==65);
    cout << "Passed all tests" << endl;
}


