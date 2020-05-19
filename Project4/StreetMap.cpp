#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cassert>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
    
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> myHash;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    myHash.reset(); //clear the hash
    
    ifstream infile(mapFile);    // infile is a name of our choosing
    if ( ! infile )                // Did opening the file fail?
    {
        cerr << "Error: " << strerror(errno);
        return false; //data file was unable to be found/opened
    }
      // Read each line.  The return value of getline is treated
      // as true if a line was read, false otherwise (e.g., because
      // the end of the file was reached).
    string line;
    while (getline(infile, line))
    {
          // To extract the information from the line, we'll
          // create an input stringstream from it, which acts
          // like a source of input for operator>>
        string streetName=line; //first line will be the street name
        getline(infile, line); //get the next line which will be the number of segments
        int segmentNum;
        istringstream(line)>>segmentNum; //converts string into int and puts in segmentNum
        
        for (int i=0; i<segmentNum; i++){
            getline(infile, line); //get the next line which should be a set of GeoCoords
            istringstream iss(line);
            string lat1,long1,lat2,long2; //vars for all the coordinates
            if (iss>>lat1>>long1>>lat2>>long2){
                GeoCoord b(lat1,long1);
                GeoCoord e(lat2,long2);
                
                vector<StreetSegment>* ptr = myHash.find(b); //look for b in hashmap and store resulting pointer in ptr
                StreetSegment s(b,e,streetName);
                if (ptr == nullptr)
                {
                    vector<StreetSegment> segments;
                    segments.clear(); //clear the vector
                    segments.push_back(s);
                    myHash.associate(b, segments);
                    assert(b.longitudeText!=""&& b.latitudeText!="");
                }
                else
                {
                    ptr->push_back(s);
                }
                
                StreetSegment s2(e,b,streetName); //swapped starting/ending coords in S
                ptr = myHash.find(e); //look for e in hashmap and store resulting pointer in ptr
                if (ptr == nullptr)
                {
                    vector<StreetSegment> segments2;
                    segments2.clear();
                    segments2.push_back(s2);
                    myHash.associate(e, segments2);
                }
                else
                {
                    ptr->push_back(s2);
                }
            }
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   const vector<StreetSegment> *ptr = myHash.find(gc);
  if (myHash.find(gc) != nullptr){ //if not nullptr, this key exists and has associated vector
        segs.clear(); //clear up the vector
      for (int i=0; i<ptr->size();i++){
          StreetSegment mySeg((*ptr)[i].start,(*ptr)[i].end,(*ptr)[i].name); //create a new segment with the same start,end,and name as in the associated vector
          segs.push_back(mySeg); //add this segment to segs
      }
    return true;
  }
  return false; //does return nullptr, then this key does not exist and return false
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
