#include "Actor.h"
#include "StudentWorld.h"
#include<string>
#include <iostream>
using namespace std;
//Actor constructor
Actor:: Actor(int imageID, double startX, double startY, Direction startDirection, int depth, StudentWorld* worldPtr, bool alive, int points, bool isDamageable)
    :GraphObject(imageID, startX, startY, startDirection, depth), worldPtr(worldPtr),hitPoints(points),alive(alive),isDamageable(isDamageable)
{
    
}
//getter for hit points
int Actor:: getHitPoints(){
    return hitPoints;
}

//setter for hit points
void Actor:: setHitPoints(int myPts){
    hitPoints=myPts;
}
//getter for StudentWorld pointer
StudentWorld* Actor:: getWorld(){
    return worldPtr;
}

//getter for alive variable
bool Actor:: isAlive(){
    return alive;
}

//sets object to dead
void Actor:: setDead(){
    alive=false;
}

//getter for isDamageable
bool Actor:: damageable(){
    return isDamageable;
}
//every other actor does not block bacteria (this will be changed for dirt)
bool Actor::blocksBacteria(){
    return false;
}

//determines if it an actor whose presence prevents the level from ending
bool Actor::preventsLevelMoveOn(){
    return false;
}
// Is this object edible?
bool Actor::isEdible() const{
    return false;
}
//plays appropriate sound when Actor is hurt
void Actor::imHurt(){
    
}

//checks if obj trying to go to position x,y will stay within the petri dish
//returns true if it is within the bounds of the dish
bool Actor::withinDish(double x, double y){
    //if the distance from the center is less than or equal to the radius
    if(pow(x-(VIEW_WIDTH/2),2)+pow(y-(VIEW_HEIGHT/2),2)<=pow(VIEW_RADIUS,2)){
        return true;
    }
    return false;
}

//Socrates constructor
Socrates:: Socrates(StudentWorld* world)
    :Actor(IID_PLAYER,0,VIEW_HEIGHT/2,0,0,world,true,100,true),sprayCharges(20),flameCharges(5),positionalAngle(180)
{
}

//sets socrates to dead
void Socrates::setDead(){
    Actor::setDead();
    getWorld()->playSound(SOUND_PLAYER_DIE);
    getWorld()->decLives();
}

//Socrates doSomething
void  Socrates::doSomething()
{
    //check to see if Socrates is currently alive
    if (!(getHitPoints()>0)) return;
    //chooses what to do with Socrates depending on key pressed
    int ch;
    double newX,newY;
    if (getWorld()!=nullptr&& getWorld()->getKey(ch)){
        //adds spray if key pressed is space
        if(ch==KEY_PRESS_SPACE&&sprayCharges>=1){
            double myX,myY;
            myX=getX();
            myY=getY();
            getPositionInThisDirection(getDirection(), SPRITE_WIDTH, myX,myY);
            getWorld()->addSpray(myX,myY,getDirection());
            getWorld()->playSound(SOUND_PLAYER_SPRAY);
            sprayCharges--;
        }
        //adds a circle of flames if key pressed is enter
        if(ch==KEY_PRESS_ENTER&&flameCharges>=1){
            double myX,myY;
            myX=getX();
            myY=getY();
            int i=0;
            int changingDir=getDirection();
            getPositionInThisDirection(getDirection(), SPRITE_WIDTH, myX,myY);
            while (i<=16){
                getWorld()->addFlame(myX,myY,changingDir);
                changingDir+=22;
                getPositionInThisDirection(changingDir,SPRITE_WIDTH,myX,myY);
                i++;
            }
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            flameCharges--;
        }
        //if left,right,a or d pressed, move Socrates 5 degrees clockwise or counter //clockwise accordingly
        if (ch==KEY_PRESS_LEFT||ch=='a'){positionalAngle+=5;}
        else if(ch==KEY_PRESS_RIGHT||ch=='d'){positionalAngle-=5;}
        newX=VIEW_HEIGHT/2*cos(positionalAngle*M_PI/180);
        newY=VIEW_HEIGHT/2*sin(positionalAngle*M_PI/180);
        moveTo(newX+VIEW_HEIGHT/2,newY+VIEW_HEIGHT/2);
        setDirection(positionalAngle+180);
    }
    //if player does not press a key, if Socrates has less than 20 sprays, give him one //more spray
    else{ if (sprayCharges<20){sprayCharges++;}}
}

//getter for spray charges
int Socrates::getSprayCharges(){
    return sprayCharges;
}

//getter for flame charges
int Socrates::getFlameCharges(){
    return flameCharges;
}
//plays appropriate sound when Actor is hurt
void Socrates::imHurt(){
    getWorld()->playSound(SOUND_PLAYER_HURT);
}

//add the inputted number of flame charges to Socrates' arsenal
void Socrates::addFlames(int flameNumber){
    flameCharges+=flameNumber;
}

//reduce Socrates' hitpoints by inputted amount
void Socrates::damageAmount(int damage){
    int pts=getHitPoints();
    setHitPoints(pts-damage);
}

//Bacteria constructor
Bacteria:: Bacteria(int myID, double xPos, double yPos, int hitPts,StudentWorld* myWorld)
    :Actor(myID,xPos,yPos,90,0,myWorld,true,hitPts,true),foodEaten(0)
{
}
void Bacteria::doSomething(){
    
}

//increases food eaten by bacteria by one
void Bacteria::increaseFoodEaten(){
    foodEaten++;
}
//getter for food eaten by bacteria
int Bacteria::getFoodEaten(){
    return foodEaten;
}

//reduce food eaten by bacteria back to zero
void Bacteria::emptyFoodEaten(){
    foodEaten=0;
}

//check for overlap with Socrates and damage him if they do overlap
bool Bacteria::socOverlapAndDamage(int howMuch){
    Socrates* soc = getWorld()->socOverlap(this);
    if (soc!=nullptr){
        soc->damageAmount(howMuch);
        //if hit points reach zero, kill socrates
        if(soc->getHitPoints()<=0){
            soc->setDead();
        }
        else{
        //if not zero play socrates is hurt sound
           soc->imHurt();
        }
        return true;
    }
    return false;
}

//if bacteria has eaten all food, add a new bacteria of the same type at the correct position
bool Bacteria::hitTotalFood(string whichBac){
    if (getFoodEaten()==3){
        double newX=getX();
        if (newX<(VIEW_WIDTH/2)) newX+=(SPRITE_WIDTH/2);
        else if(newX>(VIEW_WIDTH/2)) newX-=(SPRITE_WIDTH/2);
        
        double newY=getY();
        if (newY<(VIEW_HEIGHT/2)) newY+=(SPRITE_HEIGHT/2);
        else if(newY>(VIEW_HEIGHT/2)) newY-=(SPRITE_HEIGHT/2);

        getWorld()->createBacteria(whichBac, newX, newY);
        emptyFoodEaten();
        return true;
    }
    return false;
}

//if the bacteria overlaps a food object, then increase the food count and kill the food
void Bacteria::overlapFood(){
    if (getWorld()->getOverlappingEdible(this)){
        increaseFoodEaten();
        getWorld()->getOverlappingEdible(this)->setDead();
    }
}

//presence of bacteria prevents level from ending
bool Bacteria::preventsLevelMoveOn(){
    return true;
}

//E.Coli constructor
EColi:: EColi(double xPos, double yPos,StudentWorld* myWorld)
    :Bacteria(IID_ECOLI,xPos,yPos,5,myWorld)
{
}

//plays appropriate sound when Actor is hurt
void EColi::imHurt(){
    getWorld()->playSound(SOUND_ECOLI_HURT);
}

//sets ecoli to dead and plays ecoli dead sound
void EColi:: setDead(){
    Actor::setDead();
    getWorld()->playSound(SOUND_ECOLI_DIE);
    //increase user's score by 100
    getWorld()->increaseScore(100);
    //50% chance that a food will be made in place of the bacteria
    int i=randInt(0, 1);
    if (i==1){getWorld()->makeFood(getX(),getY());}
}

//check if the bacteria is less than or equal to 256 pixels from socrates and get the direction towards socrates and move if not blocked
bool EColi::pixelsFromSoc(){
    bool result=false;
    double angle;
    if (getWorld()->getAngleToNearbySocrates(this, 256, angle)){
        result=true;
        int i=0;
        //attempt 10 times to move towards Socrates
        while (i<10){
            double x,y;
            x=getX();
            y=getY();
            getPositionInThisDirection(angle, 2, x, y);
            if ((!getWorld()->isBacteriaMovementBlockedAt(this, x, y))&&withinDish(x, y)){
                setDirection(angle);
                moveAngle(angle,2);
                return result;
            }
            angle+=10;
            if (angle>359)
                angle=360-angle;
            i++;
        }
    }
    return result;
}
void EColi::doSomething(){
    //returns if ecoli bacteria is dead
    if(!isAlive()) return;
    
    //check if ecoli bacteria overlaps with socrates and if it does, damage him
    if (socOverlapAndDamage(4)){ //step 2
        //check if the bacteria is less than or equal to 256 pixels from socrates and get the direction towards socrates and move if not blocked
        pixelsFromSoc(); //step 5
        return;
    }
    //if ecoli has eaten 3 foods, add a new ecoli bacteria at the correct position
    if(hitTotalFood("Ecoli")){
        //check if the bacteria is less than or equal to 256 pixels from socrates and get the direction towards socrates and move if not blocked
        pixelsFromSoc();
        return;
    }
    //if the ecoli overlaps a food object, then increase the food count and kill the food
    overlapFood();
    //check if the bacteria is less than or equal to 256 pixels from socrates and get the direction towards socrates and move if not blocked
    pixelsFromSoc();
}

//Salmonella constructor
Salmonella:: Salmonella(int myID, double xPos, double yPos, int hitPts,StudentWorld* myWorld)
    :Bacteria(myID,xPos,yPos,hitPts, myWorld),movePlan(0)
{
}
void Salmonella::doSomething(){
    
}
//plays appropriate sound when Actor is hurt
void Salmonella::imHurt(){
    getWorld()->playSound(SOUND_SALMONELLA_HURT);
}

//sets salmonella to dead and plays salmonella dead sound
void Salmonella:: setDead(){
    Actor::setDead();
    getWorld()->playSound(SOUND_SALMONELLA_DIE);
    //increase user's score by 100
    getWorld()->increaseScore(100);
    //50% chance that a food will be made in place of the bacteria
    int i=randInt(0, 1);
    if (i==1){getWorld()->makeFood(getX(),getY());}
}

//getter for movement plan
int Salmonella::getMovePlan(){
    return movePlan;
}

//setter for movement plan
void Salmonella::setMovePlan(int newPlan){
    movePlan=newPlan;
}

//if reg has a movement plan greater than 0, decrement the plan and try to move in a direction where it is not blocked by dirt or end of the dish
void Salmonella::movementPlan(){
    if (getMovePlan()>0){
        setMovePlan(getMovePlan()-1);
        double x,y;
        x=getX();
        y=getY();
        getPositionInThisDirection(getDirection(), 3, x, y);
        if ((!getWorld()->isBacteriaMovementBlockedAt(this, x, y))&&withinDish(x, y)){
            moveAngle(getDirection(),3);
        }
        else{
            int ranDir = randInt(0, 359);
            setDirection(ranDir);
            setMovePlan(10);
        }
        return;
    }
}

//check if food can be found within 128 pixels of the bacteria
void Salmonella::within128Pixels(){
    double dir=0;
    if (getWorld()->getAngleToNearestNearbyEdible(this, 128, dir)){
        //if food can be found, move towards the food if it can
        double x,y;
        x=getX();
        y=getY();
        getPositionInThisDirection(dir, 3, x, y);
        if (!getWorld()->isBacteriaMovementBlockedAt(this, x, y)){
            setDirection(dir);
            moveAngle(dir,3);
        }
        else{
        //if bacteria is blocked, pick a random direction and set plan back to 10
            int ranDir=randInt(0, 359);
            setDirection(ranDir);
            setMovePlan(10);
            return;
        }
    }
    else{
        //if no food can be found, pick a random direction and set plan back to 10
        int ranDir=randInt(0, 359);
        setDirection(ranDir);
        setMovePlan(10);
        return;
    }
}

//moves according to movement plan if movePlan variable is greater than 0, else it will search for food near it
void Salmonella::MOVE(){
    if(getMovePlan()>0){
        movementPlan();
    }
    else{
        within128Pixels();
    }
}

//Aggressive Salmonella constructor
AggressiveSal:: AggressiveSal(double xPos, double yPos,StudentWorld* myWorld)
    :Salmonella(IID_SALMONELLA, xPos,yPos,10,myWorld)
{
}

bool AggressiveSal::nearbySocrates(){
    double angle=0;
    if(getWorld()->getAngleToNearbySocrates(this, 72, angle)){
        //tries to move 3 pixels towards Socrates unless it is blocked by dirt
        double x,y;
        x=getX();
        y=getY();
        getPositionInThisDirection(angle, 3, x, y);
        if (!getWorld()->isBacteriaMovementBlockedAt(this, x, y)){
            setDirection(angle);
            moveAngle(angle,3);
        }
        return true;
    }
    return false;
}

void AggressiveSal::doSomething(){
    bool didStep2=false;
    //returns if agg bacteria is dead
    if(!isAlive()) return;
    //the aggressive salmonella checks to see if it’s less than or equal to 72 pixels away from Socrates
    if(nearbySocrates()){
        didStep2=true;
    }
    //check for overlap with Socrates and damage him if they do overlap
    if(socOverlapAndDamage(2)){
        if (didStep2) return;
        else MOVE();
        return;
    }
    //if bacteria has eaten all food, add a new bacteria of the same type at the correct position
    if(hitTotalFood("Aggressive")){
        if (didStep2) return;
        else MOVE();
        return;
    }
    
    //if the bacteria overlaps a food object, then increase the food count and kill the food
    overlapFood();
    MOVE();
}

//Regular Salmonella constructor
RegularSal:: RegularSal(double xPos, double yPos,StudentWorld* myWorld)
    :Salmonella(IID_SALMONELLA,xPos,yPos,4, myWorld)
{
}

//What Reg Salmonella should do during each tick
void RegularSal:: doSomething(){
    //returns if reg bacteria is dead
    if(!isAlive()) return;
    
    //check for overlap with Socrates and damage him if they do overlap
    if(socOverlapAndDamage(2)){
        //moves according to movement plan if movePlan variable is greater than 0, else it will search for food near it
        MOVE();
        return;
    }
    //if bacteria has eaten all food, add a new bacteria of the same type at the correct position
    if(hitTotalFood("Regular")){
        MOVE();
        return;
    }
    //if the bacteria overlaps a food object, then increase the food count and kill the food
    overlapFood();
    MOVE();
}

//Projectile constructor
Projectile:: Projectile(int myID, double xPos, double yPos, Direction dir, int maxTrav,int impact, StudentWorld* myWorld)
    :Actor(myID,xPos,yPos,dir,1,myWorld,true,0,false),maxTravel(maxTrav),impact(impact),pixels(0)
{
}

void Projectile:: doSomething(){
    if (!isAlive()) return;
    //uses StudentWorld function to determine if flame overlaps with damageable object and
    //reduces the hitpoints by "impact" amount if necessary
    if(getWorld()->overlapAndDamageable(getX(), getY(), impact)){
        setDead();
        return;
    };
    //moves SPRITE WIDTH pixels forward while number of moved pixels <=maxTravel
    if(pixels<=maxTravel){moveAngle(getDirection(), SPRITE_WIDTH); pixels+=SPRITE_WIDTH;}
    else{setDead();}
}
//Flame Projectile constructor
FlameProj:: FlameProj(double xPos, double yPos, Direction startDir,StudentWorld* myWorld)
    :Projectile(IID_FLAME,xPos,yPos,startDir,32,5,myWorld)
{
}

//Spray Projectile constructor
SprayProj:: SprayProj(double xPos, double yPos, Direction startDir,StudentWorld* myWorld)
    :Projectile(IID_SPRAY,xPos,yPos,startDir,112,2,myWorld)
{
}

//GoodieOrFungOrFung constructor
GoodieOrFung:: GoodieOrFung(int myID, double xPos, double yPos, StudentWorld* world)
    :Actor(myID,xPos,yPos,0,1,world,true,0,true),runningOut(0)
{
    int L = getWorld()->getLevel();
    lifetime=max(randInt(0, 300 - 10 * L - 1), 50);
}
void GoodieOrFung::doSomething(){
    if (!isAlive()) return;
    //pickUp the goodie using the socOverlap function (this will return socrates if he does overlap with the goodie)
    pickUp(getWorld()->socOverlap(this));
    //increments runningOut to keep track of how many ticks the goodie has lasted. if runningOut has reached lifetime, make the goodie die
    if(runningOut<lifetime){runningOut++;}
    else{setDead();}
}

//Restore Health GoodieOrFung  constructor
RestoreHealth:: RestoreHealth(double xPos, double yPos, StudentWorld* world)
    :GoodieOrFung(IID_RESTORE_HEALTH_GOODIE,xPos,yPos,world)
{
}
void RestoreHealth::pickUp(Socrates* s){
    if (s!=nullptr){
        getWorld()->increaseScore(250);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        s->setHitPoints(100);
        return;
    }
}
//Flame thrower Goodie  constructor
FlameGoodie:: FlameGoodie(double xPos, double yPos, StudentWorld* world)
    :GoodieOrFung(IID_FLAME_THROWER_GOODIE,xPos,yPos,world)
{
}
void FlameGoodie::pickUp(Socrates* s){
    if (s!=nullptr){
        getWorld()->increaseScore(300);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        s->addFlames(5);
        return;
    }
}
//Extra Life GoodieOrFung  constructor
ExtraLife:: ExtraLife(double xPos, double yPos, StudentWorld* world)
   :GoodieOrFung(IID_EXTRA_LIFE_GOODIE,xPos,yPos,world)
{
}
void ExtraLife::pickUp(Socrates* s){
    if (s!=nullptr){
        getWorld()->increaseScore(500);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->incLives();
        return;
    }
}
//Fungus constructor
Fungus:: Fungus(double xPos, double yPos, StudentWorld* world)
    :GoodieOrFung(IID_FUNGUS,xPos,yPos,world)
{
}
void Fungus::pickUp(Socrates* s){
    if (s!=nullptr){
        getWorld()->increaseScore(-50);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        s->damageAmount(20);
        return;
    }
}

//Pit constructor
Pit:: Pit(double xPos, double yPos,StudentWorld* myWorld)
    :Actor(IID_PIT,xPos,yPos,0,1,myWorld,true,0,false),regSalNum(5),aggressiveSalNum(3),ecoliNum(2)
{
}

//What Pit should do during each tick
void Pit:: doSomething(){
    int thisOne=randInt(1,50);
    //check if pit is empty and if it is, set pit to dead
    if (regSalNum<=0&&aggressiveSalNum<=0&&ecoliNum<=0) setDead();
    //check which bacteria are left and randomly pick one to output
    else if(thisOne==1){
        if (aggressiveSalNum>0&&regSalNum>0&&ecoliNum>0){
            int whichOne=randInt(1, 3);
            if (whichOne==1){
                getWorld()->createBacteria("Aggressive",getX(),getY());
                aggressiveSalNum--;
            }
            else if(whichOne==2){
                getWorld()->createBacteria("Regular",getX(),getY());
                regSalNum--;
            }
            else if(whichOne==3){
                getWorld()->createBacteria("Ecoli",getX(),getY());
                ecoliNum--;
            }
        }

        if (aggressiveSalNum>0&&regSalNum>0&&ecoliNum==0){
            int whichOne=randInt(1, 2);
            if (whichOne==1){
                getWorld()->createBacteria("Aggressive",getX(),getY());
                aggressiveSalNum--;
            }
            else if(whichOne==2){
                getWorld()->createBacteria("Regular",getX(),getY());
                regSalNum--;
            }
        }
        if (aggressiveSalNum==0&&regSalNum>0&&ecoliNum>0){
            int whichOne=randInt(2, 3);
            if (whichOne==2){
                getWorld()->createBacteria("Regular",getX(),getY());
                regSalNum--;
            }
            else if(whichOne==3){
                getWorld()->createBacteria("Ecoli",getX(),getY());
                ecoliNum--;
            }
        }
        if (aggressiveSalNum>0&&regSalNum==0&&ecoliNum>0){
            int whichOne=randInt(1, 2);
            if (whichOne==1){
                getWorld()->createBacteria("Aggressive",getX(),getY());
                aggressiveSalNum--;
            }
            else if(whichOne==2){
                getWorld()->createBacteria("Ecoli",getX(),getY());
                ecoliNum--;
            }
        }
        
        //if two types are empty, make a bacteria of the remaining type
        if (aggressiveSalNum>0&&regSalNum==0&&ecoliNum==0){
            getWorld()->createBacteria("Aggressive",getX(),getY());
            aggressiveSalNum--;
        }
        if (aggressiveSalNum==0&&regSalNum>0&&ecoliNum==0){
            getWorld()->createBacteria("Regular",getX(),getY());
            regSalNum--;
        }
        if (aggressiveSalNum==0&&regSalNum==0&&ecoliNum>0){
            getWorld()->createBacteria("Ecoli",getX(),getY());
            ecoliNum--;
        }
    }
}

//presence of pit prevents level from ending
bool Pit::preventsLevelMoveOn(){
    return true;
}

//Food constructor
Food:: Food(double xPos, double yPos,StudentWorld* myWorld)
    :Actor(IID_FOOD,xPos,yPos,90,1,myWorld,true,0,false)
{
}
 
//Food does nothing
void  Food::doSomething()
{
}
//Food is edible
bool Food::isEdible() const{
    return true;
}
//Dirt constructor
Dirt:: Dirt(double xPos, double yPos)
    :Actor(IID_DIRT,xPos,yPos,0,1,nullptr,true,1,true)
{
}
 
//Dirt does nothing
void  Dirt::doSomething()
{
}

bool Dirt::blocksBacteria(){
    return true;
}
