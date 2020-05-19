//
//  mazequeue.cpp
//  Maze2
//
//  Created by Komal Prakash on 2/4/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//
#include <stdio.h>
#include <queue>
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
    queue<Coord> coordQueue;
    coordQueue.push(Coord(sr,sc));
    maze[sr][sc]='^';
    while (!coordQueue.empty()){
        Coord front = coordQueue.front();
        coordQueue.pop();
        if (front.r()==er && front.c()==ec){
            return true;
        }
        if (maze[front.r()][front.c()+1]=='.'){
            coordQueue.push(Coord(front.r(),front.c()+1));
            maze[front.r()][front.c()+1]='^';
        } //east
        if (maze[front.r()+1][front.c()]=='.'){
            coordQueue.push(Coord(front.r()+1,front.c()));
            maze[front.r()+1][front.c()]='^';
        } //south
        if (maze[front.r()][front.c()-1]=='.'){
            coordQueue.push(Coord(front.r(),front.c()-1));
            maze[front.r()][front.c()-1]='^';
        }//west
        if (maze[front.r()-1][front.c()]=='.'){
            coordQueue.push(Coord(front.r()-1,front.c()));
            maze[front.r()-1][front.c()]='^';
        }//north
    }
    return false;
}
 
