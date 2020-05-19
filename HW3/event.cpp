//
//  event.cpp
//  Homework3
//
//  Created by Komal Prakash on 2/10/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//


class Event{
public:
    Event(string eventName){
        myName=eventName;
    }
    virtual ~Event(){
    }
    string name()const{
        return myName;
    }
    virtual string need() const=0;
    virtual bool isSport() const=0;
private:
    string myName;
};

class BasketballGame: public Event{
public:
    BasketballGame(string myName)
        :Event(myName)
    {
    }
    virtual ~BasketballGame(){
        cout<< "Destroying the "+name()+" basketball game"<<endl;
    }
    string need() const {
        return "hoops";
    }
    bool isSport() const{
        return true;
    }
};

class Concert: public Event{
public:
    Concert(string myName, string myGenre)
        :Event(myName)
    {
        genre=myGenre;
    }
    virtual ~Concert(){
        cout<< "Destroying the "+name()+" "+genre+" concert"<<endl;
    }
    string need() const {
        return "a stage";
    }
    bool isSport() const{
        return false;
    }
private:
    string genre;
};

class HockeyGame: public Event{
public:
    HockeyGame(string myName)
        :Event(myName)
    {
    }
    virtual ~HockeyGame(){
        cout<< "Destroying the "+name()+" hockey game"<<endl;
    }
    string need() const {
        return "ice";
    }
    bool isSport() const{
        return true;
    }
};

void display(const Event* e)
{
    cout << e->name() << ": ";
    if (e->isSport())
    cout << "(sport) ";
    cout << "needs " << e->need() << endl;
}


