//
//  mazestack.cpp
//  Maze
//
//  Created by Komal Prakash on 2/3/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//

#include <stack>
#include <string>
using namespace std;

class Coord
{
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
 
  private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    stack<Coord> coordStack;
    coordStack.push(Coord(sr,sc));
    maze[sr][sc]= '^';
    while (!coordStack.empty()){
        Coord top = coordStack.top();
        coordStack.pop();
        if (top.r()==er && top.c()==ec){
            return true;
        }
        if (maze[top.r()][top.c()+1]=='.'){
            coordStack.push(Coord(top.r(),top.c()+1));
            maze[top.r()][top.c()+1]='^';
        } //east
        if (maze[top.r()+1][top.c()]=='.'){
            coordStack.push(Coord(top.r()+1,top.c()));
            maze[top.r()+1][top.c()]='^';
        } //south
        if (maze[top.r()][top.c()-1]=='.'){
            coordStack.push(Coord(top.r(),top.c()-1));
            maze[top.r()][top.c()-1]='^';
        }//west
        if (maze[top.r()-1][top.c()]=='.'){
            coordStack.push(Coord(top.r()-1,top.c()));
            maze[top.r()-1][top.c()]='^';
        }//north
    }
    return false;
}

