//
//  list.cpp
//  Homework5
//
//  Created by Komal Prakash on 3/12/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

void listAll(string path, const Class* c)  // two-parameter overload
{
    const vector<Class*>& myVector = c->subclasses();
    
    cout<<path<<c->name()<<endl;
    path+=c->name()+"=>";
    
    for (int i = 0; i<myVector.size(); i++){
        listAll(path, myVector[i]);
    }
}


