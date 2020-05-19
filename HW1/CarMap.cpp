//
//  CarMap.cpp
//  Map
//
//  Created by Komal Prakash on 1/20/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

#include "Map.h"
#include "CarMap.h"
#include <array>
#include <string>
#include <iostream>

CarMap::CarMap(){
}// Create an empty car map.

bool CarMap::addCar(std::string license){
    return myCar.insert(license, 0);
}
          // If a car with the given license plate is not currently in the map,
          // and there is room in the map, add an entry for that car recording
          // that it has been driven 0 miles, and return true.  Otherwise,
          // make no change to the map and return false.

double CarMap::miles(std::string license) const{
    double val=0.00;
    if (myCar.get(license,val)){
        return val;
    }
    return -1;
}
          // If a car with the given license plate is in the map, return how
          // many miles it has been driven; otherwise, return -1.

bool CarMap::drive(std::string license, double distance){
    double val=0.00;
    if (!myCar.contains(license)||distance<0){
        return false;
    }
    myCar.get(license, val);
    myCar.update(license,distance+val);
    return true;
}
          // If no car with the given license plate is in the map or if
          // distance is negative, make no change to the map and return
          // false.  Otherwise, increase by the distance parameter the number
          // of miles the indicated car has been driven and return true.

int CarMap::fleetSize() const{
    return myCar.size();
}

void CarMap::print() const{
    for (int j = 0; j<myCar.size(); j++){
        KeyType plate;
        ValueType mileage;
        if(myCar.get(j, plate, mileage)){
            std::cout<<plate<<" "<<mileage<<"\n";
        }
    }
}
          // Write to cout one line for every car in the map.  Each line
          // consists of the car's license plate, followed by one space,
          // followed by the number of miles that car has been driven.  Write
          // no other text.  The lines need not be in any particular order.

