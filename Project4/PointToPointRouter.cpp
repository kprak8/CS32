#include "provided.h"
#include <list>
#include <algorithm>
#include <vector>
#include <stack>
#include "ExpandableHashMap.h"
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> locationOfPreviousWayPoint;
    struct Node
    {
        Node() = default;
        Node(GeoCoord loc){location=loc;}
        bool discovered = false;   //true if we have already visited it
        double globalHeuristic=INFINITY;   //node's distance to the goal (heuristic)
        double localHeuristic=INFINITY;   //node's distance to goal using alternative route
        GeoCoord location;
        Node* parent=nullptr;           //points to the node that is connected to this node and offers the shortest path
    };
        double crowsDist (Node* a, Node* b) const; //finds the crow's distance between two points (this is the heuristic if one of the nodes is at the destination)
    //finds the street segment connecting two nodes
    StreetSegment getSegFromNodes(Node* a, Node* b) const;
    const StreetMap* myMap; //pointer to street map data
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    myMap=sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

//implemented the A* search algorithm using local and global heuristics
DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    //clear the route and set distance traveled to 0 at the beginning
    route.clear();
    totalDistanceTravelled=0;
    
    //if the start or ending coordinate is not found in map, return bad coord
    vector<StreetSegment> vec;
    if (!myMap->getSegmentsThatStartWith(start, vec)||!myMap->getSegmentsThatStartWith(end, vec)){
        return BAD_COORD;
    }
    
    //==============================BUILDS THE GRAPH=====================================


    //build the graph using another map that maps GeoCoords to a node with its heuristic data
    ExpandableHashMap<GeoCoord, Node> mapToNodes;
    mapToNodes.reset();

    
    //initialize start node with correct data
    mapToNodes.associate(start, Node(start));
    
    mapToNodes.find(start) -> localHeuristic = 0.0;
    Node* startNode = mapToNodes.find(start);

    //initialize end node with correct data
    
    mapToNodes.associate(end, Node(end));
    Node* endNode = mapToNodes.find(end);
    
    //initialize distance between start and end node
    startNode->globalHeuristic= crowsDist(startNode, endNode);

    
    //create a current node that starts at the starting location
    Node *currNode = mapToNodes.find(start);
    
    // creates a list of nodes that have not been tested and adds the first position to it
    list<Node*> notTested;
    notTested.push_back(currNode);
    
    //continues while there are still nodes to be tested
    while (!notTested.empty() && currNode->location!=endNode->location){
        //sorts the list by the global scores to determine which is smaller and has a higher chance of being the correct path
        notTested.sort([](const Node* a, const Node* b){ return a->globalHeuristic < b->globalHeuristic; } );
        
        //remove any node that has already been visited
        while(!notTested.empty() && notTested.front()->discovered){
            notTested.pop_front();
        }
        
        //if all nodes have been visited and no path found, return that no route was found
        if (notTested.empty())
            return NO_ROUTE;
        
        
        currNode = notTested.front();
        //the current node that you are on should be marked as discovered so that we do not look at it again
        currNode->discovered = true;
        
        //Check each of the neighboring paths from the current node that you are on
        vector<Node*> tempPath;
        vector<StreetSegment> segs;
        myMap->getSegmentsThatStartWith(currNode->location, segs);
        for (int j=0; j<segs.size();j++){
            if (mapToNodes.find(segs[j].end)==nullptr){
                Node heuristicNode=Node(segs[j].end);
                mapToNodes.associate(segs[j].end, heuristicNode);
            }
            tempPath.push_back(mapToNodes.find(segs[j].end));
        }
        
        for (int i=0; i<tempPath.size(); i++){
            //add a neighboring path to the not tested list only if the path is not visited
            if (!tempPath[i]->discovered){
                notTested.push_back(tempPath[i]);
            }
            //find the neighboring path's potential lowest goal distance
            double potentialLowerGoal = currNode->localHeuristic + crowsDist(currNode, tempPath[i]);
        
            //if this potential goal is lower than the current neighbor's goal, update this neighbor's parent to the current node (this is now the neighbor's chosen path source). Then set the local goal to that smaller number.
            if (potentialLowerGoal < tempPath[i]->localHeuristic){
                tempPath[i]->parent = currNode;
                tempPath[i]->localHeuristic = potentialLowerGoal;
                
                //update the global heuristic based on the changed local heuristic
                tempPath[i]->globalHeuristic = tempPath[i]->localHeuristic + crowsDist(tempPath[i], endNode);
            }
        }
    }
    
    //record the path by starting at the end, and following the parent node trail
    // back to the start
    Node *p = endNode;
    //create a stack on which to add the street segment of the optimal path
    stack <StreetSegment> segStack;
    while (p->parent != nullptr){
        //get the street segment between the parents
        StreetSegment seg = getSegFromNodes(p, p->parent);
        //add the length of each segment to the total distance traveled variable
        totalDistanceTravelled+=distanceEarthMiles(seg.start, seg.end);
        //add each segment to a stack
        segStack.push(seg);
        // Set next node to this node's parent
        p = p->parent;
    }
    //add the contents of stack into the route list (so that it will be in the correct order)
    while (!segStack.empty()){
        route.push_back(segStack.top());
        segStack.pop();
    }
    return DELIVERY_SUCCESS;
}

//finds the crow's distance between two points (this is the heuristic if one of the nodes is at the destination)
double PointToPointRouterImpl::crowsDist(Node* a, Node* b) const{
    double ans = distanceEarthMiles(a->location, b->location);
    return ans;
}

//finds the street segment connecting two nodes
StreetSegment PointToPointRouterImpl::getSegFromNodes (Node* a, Node* b) const{
    vector <StreetSegment> segs;
    //finds the segments from node a
    myMap->getSegmentsThatStartWith(a->location, segs);
    for (int i =0; i<segs.size(); i++){
        //if any segment's ending coord matches node b, then return that segment
        if (segs[i].end == b->location){
            return segs[i];
        }
    }
    return segs[0];
}

//******************** PointToPointRouter functions ************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
