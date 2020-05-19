#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <vector>
using namespace std;

class Actor;
class Socrates;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    StudentWorld();
    ~StudentWorld();
    bool overlapAndDamageable(double posX, double posY,int hitPts);
    Socrates* socOverlap(Actor* a);
    void createBacteria(string typeBac, double posX, double posY);
    void addSpray(double posX, double posY, int dir);
    void addFlame(double posX, double posY,int dir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void makeFood(double x, double y);
    bool getAngleToNearbySocrates(Actor* a, int dist, double& angle) const;
      // Return true if there is a living edible object within the indicated
      // distance from actor a; otherwise false.  If true, angle will be set
      // to the direction from actor a to the edible object nearest to it.
    bool getAngleToNearestNearbyEdible(Actor* a, int dist, double& angle) const;
      // Is bacterium a blocked from moving to the indicated location?
    bool isBacteriaMovementBlockedAt(Actor* a, double x, double y) const;
    Actor* getOverlappingEdible(Actor* a) const;
    
private:
    Socrates* socrates;
    vector<Actor*> myMembers;
};

#endif // STUDENTWORLD_H_
