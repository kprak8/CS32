#ifndef ACTOR_H_
#define ACTOR_H_
#include "StudentWorld.h"
#include "GraphObject.h"
#include <string>
using namespace std;


class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction startDirection, int depth,StudentWorld* worldPtr, bool alive, int points, bool isDamageable);
    virtual void doSomething()=0;
    int getHitPoints();
    void setHitPoints(int myPts);
    StudentWorld* getWorld();
    virtual void setDead();
    bool isAlive();
    bool damageable();
    virtual bool blocksBacteria();
    virtual bool isEdible() const;
    bool withinDish(double x, double y);
    virtual void imHurt();
    virtual bool preventsLevelMoveOn();
private:
    StudentWorld* worldPtr;
    bool alive;
    int hitPoints;
    bool isDamageable;
};

class Socrates: public Actor{
public:
    Socrates(StudentWorld* myWorld);
    virtual void doSomething();
  //  virtual int soundWhenDie() const;
    void addFlames(int flameNumber);
    void damageAmount(int damage);
    virtual void imHurt();
    int getSprayCharges();
    int getFlameCharges();
    virtual void setDead();
private:
    int sprayCharges;
    int flameCharges;
    int positionalAngle;
};

//Bacteria base class
class Bacteria: public Actor{
public:
    Bacteria(int myID, double xPos, double yPos, int hitPts,StudentWorld* myWorld);
    virtual void doSomething();
    void increaseFoodEaten();
    int getFoodEaten();
    void emptyFoodEaten();
    bool socOverlapAndDamage(int howMuch);
    bool hitTotalFood(string whichBac);
    void overlapFood();
    virtual bool preventsLevelMoveOn();
private:
    int foodEaten;
};

//E. Coli subclass of type Bacteria
class EColi: public Bacteria{
public:
    EColi(double xPos, double yPos,StudentWorld* myWorld);
    virtual void doSomething();
    bool pixelsFromSoc();
    virtual void setDead();
    virtual void imHurt();
private:
};

//Salmonella subclass of type Bacteria
class Salmonella: public Bacteria{
public:
    Salmonella(int myID, double xPos, double yPos, int hitPts,StudentWorld* myWorld);
    virtual void doSomething();
    int getMovePlan();
    void setMovePlan(int newPlan);
    void movementPlan();
    void within128Pixels();
    virtual void setDead();
    virtual void imHurt();
    void MOVE();
private:
    int movePlan;
};

//Aggressive salmonella subclass of type Salmonella
class AggressiveSal: public Salmonella{
public:
    AggressiveSal(double xPos, double yPos,StudentWorld* myWorld);
    virtual void doSomething();
    bool nearbySocrates();
private:
};

//Regular salmonella subclass of type Salmonella
class RegularSal: public Salmonella{
public:
    RegularSal(double xPos, double yPos,StudentWorld* myWorld);
    virtual void doSomething();
private:
};

//Projectile base class
class Projectile: public Actor{
public:
    Projectile(int imageID, double xPos, double yPos, Direction dir, int maxTravel, int impact, StudentWorld* myWorld);
    virtual void doSomething();
private:
    int maxTravel;
    int impact;
    int pixels;
};
//Flame projectile subclass of type Projectile
class FlameProj: public Projectile{
public:
    FlameProj(double xPos, double yPos, Direction startDir, StudentWorld* myWorld);
private:
};

//Spray projectile subclass of type Projectile
class SprayProj: public Projectile{
public:
    SprayProj(double xPos, double yPos, Direction startDir, StudentWorld* myWorld);
private:
};

//GoodieOrFung base class
class GoodieOrFung: public Actor{
public:
    GoodieOrFung(int imageID, double xPos, double yPos, StudentWorld* world);
    virtual void doSomething();
    virtual void pickUp(Socrates* s) = 0;
private:
    int lifetime;
    int runningOut;
};

//Restore Health goodie subclass of type GoodieOrFung
class RestoreHealth: public GoodieOrFung{
public:
    RestoreHealth(double xPos, double yPos, StudentWorld* world);
    virtual void pickUp(Socrates* s);
private:
};

//Flame Thrower goodie subclass of type GoodieOrFung
class FlameGoodie: public GoodieOrFung{
public:
    FlameGoodie(double xPos, double yPos, StudentWorld* world);
    virtual void pickUp(Socrates* s);
private:
};

//Extra Life goodie subclass of type GoodieOrFung
class ExtraLife: public GoodieOrFung{
public:
    ExtraLife(double xPos, double yPos, StudentWorld* world);
    virtual void pickUp(Socrates* s);
private:
};

//Fungus subclass of GoodieOrFung
class Fungus: public GoodieOrFung{
public:
    Fungus(double xPos, double yPos, StudentWorld* world);
    virtual void pickUp(Socrates* s);
private:
};

//Pit(bacteria generator) subclass of Actor
class Pit: public Actor{
public:
    Pit(double xPos, double yPos, StudentWorld* myWorld);
    virtual void doSomething();
    virtual bool preventsLevelMoveOn();
private:
    int regSalNum,aggressiveSalNum, ecoliNum;
};

//Food subclass of Actor
class Food: public Actor{
public:
    Food(double xPos, double yPos,StudentWorld* myWorld);
    virtual void doSomething();
    virtual bool isEdible() const;
};

//Dirt Pile subclass of Actor
class Dirt: public Actor{
public:
    Dirt(double xPos, double yPos);
    virtual void doSomething();
    virtual bool blocksBacteria();
private:
};


#endif // ACTOR_H_
