#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}
//destructor
StudentWorld:: ~StudentWorld(){
    cleanUp();
}

//adds food at the position inputted
void StudentWorld::makeFood(double x, double y){
    Actor* food = new Food(x, y, this);
    myMembers.push_back(food);
}

//adds a Spray in front of Socrates and play the sound
void StudentWorld::addSpray(double posX, double posY, int dir){
    Actor* spray = new SprayProj(posX, posY, dir, this);
    myMembers.push_back(spray);
    playSound(SOUND_PLAYER_SPRAY);
}

//adds flames around Socrates and plays the sound
void StudentWorld::addFlame(double posX, double posY, int dir){
    Actor* fire = new FlameProj(posX, posY, dir, this);
    myMembers.push_back(fire);
    playSound(SOUND_PLAYER_FIRE);
}

//if they overlap and object is damangeable, then reduce the object's hit pts
//if it reduces hit points, return true, otherwise false
bool StudentWorld::overlapAndDamageable(double posX, double posY,int hitPts){
    //using pythagorean thm to determine overlap
    for (int i=0; i<myMembers.size();i++){        if((pow(myMembers[i]->getX()-posX,2)+pow(myMembers[i]->getY()-posY,2))<=pow(SPRITE_WIDTH,2)&&myMembers[i]->damageable()){
            int points=myMembers[i]->getHitPoints();
            myMembers[i]->setHitPoints(points-hitPts);
            if(myMembers[i]->getHitPoints()<=0){
                myMembers[i]->setDead();
            }
            else{
                myMembers[i]->imHurt();
            }
            return true;
        }
    }
    return false;
}
// If actor a overlaps this world's socrates, return a pointer to the
// socrates; otherwise, return nullptr
Socrates* StudentWorld::socOverlap(Actor* a){
    //using pythagorean thm to determine overlap
if((pow(socrates->getX()-a->getX(),2)+pow(socrates->getY()-a->getY(),2))<=pow(SPRITE_WIDTH,2)){
            return socrates;
    }
    return nullptr;
}
// Return true if this world's socrates is within the indicated distance
// of actor a; otherwise false.  If true, angle will be set to the
// direction from actor a to the socrates.
bool StudentWorld::getAngleToNearbySocrates(Actor* a, int dist, double& angle) const{
    double newX=socrates->getX()-a->getX();
    double newY=socrates->getY()-a->getY();
    if ((pow(newX,2)+pow(newY,2))<=pow(dist,2)){
        angle=atan2(newY, newX);
        angle = angle*180/M_PI;
        while (angle < 0) angle+= 360;
        return true;
    }
    return false;
}

  // Return true if there is a living edible object within the indicated
  // distance from actor a; otherwise false.  If true, angle will be set
  // to the direction from actor a to the edible object nearest to it.
bool StudentWorld::getAngleToNearestNearbyEdible(Actor* a, int dist, double& angle) const{
    for (int i=0; i<myMembers.size();i++){
        int newY=myMembers[i]->getY()-a->getY();
        int newX=myMembers[i]->getX()-a->getX();
        if ((myMembers[i]->isEdible())&&(pow(newX,2)+pow(newY,2))<=pow(dist,2)){
            angle=atan2(newY, newX);
            angle = angle*180/M_PI;
            while (angle < 0) angle+= 360;
            return true;
        }
    }
    return false;
}

// Is bacterium a blocked from moving to the indicated location?
bool StudentWorld::isBacteriaMovementBlockedAt(Actor* a, double x, double y) const{
    for (int i=0; i<myMembers.size();i++){
       //if member is dirt and the distance is small enough, bacteria is blocked, so return true
        if ((myMembers[i]->blocksBacteria())&&(pow(myMembers[i]->getX()-x,2)+pow(myMembers[i]->getY()-y,2))<=pow((SPRITE_WIDTH/2),2)){
            return true;
        }
    }
    return false;
}

  // If actor a overlaps a living edible object, return a pointer to the
  // edible object; otherwise, return nullptr
Actor* StudentWorld::getOverlappingEdible(Actor* a) const{
    for (int i=0; i<myMembers.size();i++){
        if ((myMembers[i]->isEdible())&&(pow(a->getX()-myMembers[i]->getX(),2)+pow(a->getY()-myMembers[i]->getY(),2))<=pow(SPRITE_WIDTH,2)){
            return myMembers[i];
        }
    }
    return nullptr;
}

int StudentWorld::init()
{
    socrates = new Socrates(this);
    
    int L = getLevel();
    int dirtNum = 180-20*L;
    if (dirtNum<20) dirtNum=20;
    for (int i =0; i<dirtNum; i++){
        int degrees;
        degrees=randInt(0, 360);
        double rad = degrees* M_PI/180;
        
        int length;
        length = randInt(0, 120);
        
        int x=length*cos(rad);
        int y = length*sin(rad);
        
        Actor* pile = new Dirt(x+VIEW_WIDTH/2,y+VIEW_HEIGHT/2);
        myMembers.push_back(pile);
    }
    //STILL NEED TO CHECK FOR OVERLAP BUT THIS CREATES A PIT
    for (int i=0; i<L;i++){
        int randDistfromCenter=randInt(0,120);
        int randAngle=randInt(0,360);
        double newX=randDistfromCenter*cos(randAngle*M_PI/180);
        double newY=randDistfromCenter*sin(randAngle*M_PI/180);
        Actor* pit = new Pit(newX+VIEW_WIDTH/2,newY+VIEW_HEIGHT/2,this);        
        myMembers.push_back(pit);
    }
    //STILL NEED TO CHECK FOR OVERLAP BUT THIS CREATES FOOD
    int howMany=min(5 * L, 25);
    for (int i=0; i<howMany;i++){
        int randDistfromCenter=randInt(0,120);
        int randAngle=randInt(0,360);
        double newX=randDistfromCenter*cos(randAngle*M_PI/180);
        double newY=randDistfromCenter*sin(randAngle*M_PI/180);
        Actor* food = new Food(newX+VIEW_WIDTH/2,newY+VIEW_HEIGHT/2,this);
        myMembers.push_back(food);
    }
    return GWSTATUS_CONTINUE_GAME;
}

//creates bacteria on the board
void StudentWorld::createBacteria(string typeBac, double posX, double posY){
    if(typeBac=="Aggressive"){
        Actor* aggressiveBac = new AggressiveSal(posX,posY,this);
        myMembers.push_back(aggressiveBac);
    }
    else if(typeBac=="Regular"){
        Actor* regularBac = new RegularSal(posX,posY,this);
        myMembers.push_back(regularBac);
    }
    else if(typeBac=="Ecoli"){
        Actor* ecoliBac = new EColi(posX,posY,this);
        myMembers.push_back(ecoliBac);
    }
    playSound(SOUND_BACTERIUM_BORN);
}


int StudentWorld::move()
{
    //make socrates do something each tick and delete him if he is not alive
    socrates->doSomething();
    if(!(socrates->isAlive())) return GWSTATUS_PLAYER_DIED;
    
    //make other members do something each tick and delete them if they are no longer alive
    for (int i=0; i<myMembers.size(); i++){
        myMembers[i]->doSomething();
    }
    int i = 0;
    while (i<myMembers.size()){
        if(!(myMembers[i]->isAlive())){
            delete myMembers[i];
            myMembers.erase(myMembers.begin()+i); //iterator that points to ith element
        }
        else i++;
    }
    
    
    //introduce new goodies onto the board once and awhile
    int ChanceGoodie = max(510 - getLevel() * 10, 250);
    int ranNum= randInt(0, ChanceGoodie-1);
    int whichOne=randInt(1, 100); //helps choose which type of goodie
    int ranDir=randInt(0, 360); //gives the random direction
    
    if (ranNum==0&&whichOne<=10){
        Actor* extra = new ExtraLife((VIEW_WIDTH/2)*cos(ranDir*M_PI/180)+(VIEW_HEIGHT/2), (VIEW_WIDTH/2)*sin(ranDir*M_PI/180)+(VIEW_HEIGHT/2),this);
        myMembers.push_back(extra);
    }
    else if (ranNum==0&&whichOne<=30){
        Actor* flame = new FlameGoodie((VIEW_WIDTH/2)*cos(ranDir*M_PI/180)+(VIEW_HEIGHT/2), (VIEW_WIDTH/2)*sin(ranDir*M_PI/180)+(VIEW_HEIGHT/2),this);
        myMembers.push_back(flame);
    }
    else if (ranNum==0&&whichOne<=60){
        Actor* health = new RestoreHealth((VIEW_WIDTH/2)*cos(ranDir*M_PI/180)+(VIEW_HEIGHT/2), (VIEW_WIDTH/2)*sin(ranDir*M_PI/180)+(VIEW_HEIGHT/2),this);
        myMembers.push_back(health);
    }
    
    //introduce new fungus onto the board once and awhile
    int ChanceFungus = max(510 - getLevel() * 10, 250);
    int ranNumFung= randInt(0, ChanceFungus-1); //this might be not-inclusive of ChanceGoodie, is this okay?
    int ranDirFung=randInt(0, 360); //gives the random direction
    if (ranNumFung==0){
        Actor* fungus = new Fungus((VIEW_WIDTH/2)*cos(ranDirFung*M_PI/180)+(VIEW_HEIGHT/2), (VIEW_WIDTH/2)*sin(ranDirFung*M_PI/180)+(VIEW_HEIGHT/2),this);
        myMembers.push_back(fungus);
    }
    
   //create the title at the top of the page
   ostringstream oss;
   oss.setf(ios::fixed);
   oss.precision(2);
   oss << "  Score: "<<getScore();
   oss << "  Level: "<<getLevel();
   oss << "  Lives: "<<getLives();
   oss << "  Health: "<<socrates->getHitPoints();
   oss << "  Sprays: "<<socrates->getSprayCharges();
   oss << "  Flames: "<<socrates->getFlameCharges();
   string gameText = oss.str();
   setGameStatText(gameText);
    
    //if socrates has 0 lives, then game over

    //check if bacteria or pits are still there and if so, continue the game
    for (int i=0; i<myMembers.size(); i++){
        if (myMembers[i]->preventsLevelMoveOn())
            return  GWSTATUS_CONTINUE_GAME;
    }
    //otherwise, finish the level and move on to the next level
    return GWSTATUS_FINISHED_LEVEL;
}

void StudentWorld::cleanUp(){
    for (int i = 0; i < myMembers.size(); i++) {
        delete myMembers[i];
    }
    myMembers.erase(myMembers.begin(), myMembers.end());
    delete socrates;
    socrates = nullptr;
}

