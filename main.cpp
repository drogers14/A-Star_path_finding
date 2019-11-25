#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <limits.h>
#define INF INT_MAX
//main
//Rogers, Destiny
//ecst_drogers14
using namespace std;

struct Node{
  int x_coor;
  int y_coor;
  char terrain;
  Node(int x, int y, char t): x_coor(x),  y_coor(y),  terrain(t){}
  int cost = 0;
  int fScore =-1 ;
  int gScore = -1;
  int hScore = 0;
  Node* cameFrom=NULL;
};

int width = 0;
int height = 0;
void print_sanity(Node*** array2d);
int getCost(Node* symbol);
void setCost(Node* symbol);
int heuristic(Node* start, Node* goal);
vector<Node*>getNeighbor(Node* cur, Node*** array2d);
bool isValid(int x, int y);
bool isMatch(vector<Node*>closedSet, Node* cur);
void Astar(Node* start, Node* goal, Node*** array2d,vector<Node*>&goodPath);
void prettyPrint(Node*** array2d);
void prettyPath(vector<Node*>goodPath);


int main(int argc, char* argv[]){
  width=atoi(argv[1]);
  height=atoi(argv[2]);
  int startx,starty,goalx,goaly;
  char map_maker;
  Node*** array2d = new Node**[height];
  for(int i=0; i<height; i++){
    array2d[i] = new Node*[width];
  }
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      cin>>map_maker;
      array2d[i][j]=new Node(j,i,map_maker);
      setCost(array2d[i][j]);
    }
  }
    if(argc==5){
      startx=atoi(argv[3]);
      starty=atoi(argv[4]);
      goalx=0;
      goaly=0;
      }
      else if(argc==7){
      startx=atoi(argv[3]);
      starty=atoi(argv[4]);
      goalx=atoi(argv[5]);
      goaly=atoi(argv[6]);
      }
      else{
      startx=0;
      starty=0;
      goalx=width-1;
      goaly=height-1;
      }
  vector<Node*>goodPath;
  Astar(array2d[startx][starty],array2d[goalx][goaly],array2d,goodPath);
  prettyPrint(array2d);
  array2d[startx][starty]->terrain = 'S';
  array2d[goalx][goaly]->terrain = 'G';
  print_sanity(array2d);
//cout<<array2d[goalx][goaly]->cameFrom->terrain<<endl;
  return 0;
}

/*
   This sets the cost of the Node* terrain.
   */
void setCost(Node* symbol){
  if(symbol->terrain == '.'){
    symbol->cost = 1;
  }else if(symbol->terrain == '*'){
    symbol->cost = 3;
  }else if(symbol->terrain == 'o'){
    symbol->cost = 5;
  }else if(symbol->terrain == '~'){
    symbol->cost = 7;
  }else if(symbol->terrain == 'x'){
    symbol->cost = 420;
  }
}

/*
   Returns cost of the terrain.
   */
int getCost(Node* symbol){
  return symbol->cost;
}

/*
   The heuristic function returns an estimate distance from start
   Node* to the goal Node*.
   */
int heuristic(Node* start, Node* goal){
  int h;
  h=abs(start->x_coor - goal->x_coor)+abs(start->y_coor - goal->y_coor);
  return h;
}

/*
   This function returns the cheapest path from a start Node*
   to the goal Node*.
   created open and closed set, found Node* wit lowesst fScore and set that to current
   then evaluated currents neighbors and updated cameFrom and f,g, and hScores.
   */
void Astar(Node* start, Node* goal, Node*** array2d,vector<Node*>&goodPath){
  vector<Node*>openSet;
  vector<Node*>closedSet;
  Node* current=NULL;
  vector<Node*> neighbor;
  start->gScore=0;
  start->fScore=heuristic(start,goal);
  openSet.push_back(start);
  current=start;
  while(openSet.size()>0){
    for(int i=0; i<openSet.size();i++){
      if(openSet[i]->fScore <= current->fScore){
        current = openSet[i];
      }
    }
    if(current == goal){break;}
    //closedSet.push_back(current);
    //remove current from openSet
    for(int i=0; i<openSet.size(); i++){
      if(openSet[i]==current){
        openSet.erase(openSet.begin()+i);
      }
    }
    closedSet.push_back(current);
    if(isValid(current->x_coor-1,current->y_coor)){
      neighbor.push_back(array2d[current->x_coor-1][current->y_coor]);}
    if(isValid(current->x_coor,current->y_coor+1)){
      neighbor.push_back(array2d[current->x_coor][current->y_coor+1]);}
    if(isValid(current->x_coor+1,current->y_coor)){
      neighbor.push_back(array2d[current->x_coor+1][current->y_coor]);}
    if(isValid(current->x_coor,current->y_coor+1)){
      neighbor.push_back(array2d[current->x_coor][current->y_coor+1]);}
    
    //NEIGHBORS FOR LOOP
    for(int i=0; i<neighbor.size(); i++){
      Node* cur_neighbor = neighbor[i];
      if(isMatch(closedSet,cur_neighbor)){continue;}
        int tempG = current->gScore + getCost(cur_neighbor);

        if(!isMatch(openSet,cur_neighbor)){
          openSet.push_back(cur_neighbor);
        }
        else if(tempG >=getCost(cur_neighbor)){
          continue;
          }
          //else{openSet.push_back(cur_neighbor);continue;}
        /*else{
           cur_neighbor->gScore = tempG;
           openSet.push_back(cur_neighbor);
           }*/
        //openSet.push_back(cur_neighbor);
        cur_neighbor->fScore = cur_neighbor->gScore + heuristic(cur_neighbor,goal);;
        cur_neighbor->gScore = tempG;
        cur_neighbor->cameFrom = current;
      //if the neighbor is in closed set ignore
      // if(!isMatch(closedSet,neighbor[i])){
      // score = current->gScore + getCost(neighbor[i]);
      //}
      // Discover a new node
      /* if(isMatch(openSet, neighbor[i])){
         if(neighbor[i]->gScore <= getCost(neighbor[i])){continue;}
         else if(isMatch(closedSet,neighbor[i])){
         if(neighbor[i]->gScore <= getCost(neighbor[i])){continue;}
         openSet.push_back(neighbor[i]);
         for(int i=0; i<closedSet.size(); i++){
         if(closedSet[i]==current){
         closedSet.erase(closedSet.begin()+i);
         }
         }*/

      //openSet.push_back(neighbor[i]);
      // }
      //  }
      // else{
      // openSet.push_back(neighbor[i]);
      // }
      // This is not a better path.
      //else if(score>=neighbor[i]->gScore){continue;}
      //This path is the best until now. Record it!
      // neighbor[i]->cameFrom = current;
      // goodPath.push_back(neighbor[i]->cameFrom);
      // neighbor[i]->gScore = score;
      //neighbor[i]->fScore = neighbor[i]->gScore + heuristic(neighbor[i],goal);
    }//end for loop
    /*closedSet.push_back(current);
      for(int i=0; i<openSet.size(); i++){
      if(openSet[i]==current){
      openSet.erase(openSet.begin()+i);
      }
      }*/
    neighbor.clear();
    //    break;
  }//end while loop
  //break;
}

/*
   This function returns a vector of valid Node*
   that are called neighbors from cur.
   */
vector<Node*>getNeighbor(Node* cur, Node*** array2d){
  Node* east=NULL;
  Node* west=NULL;
  Node* north=NULL;
  Node* south=NULL;
  vector <Node*> neighbors;
  if(isValid((cur->x_coor)-1,cur->y_coor)){
    north = array2d[(cur->x_coor)-1][cur->y_coor];
  }
  if(isValid((cur->x_coor),(cur->y_coor)+1)){
    east=array2d[(cur->x_coor)][(cur->y_coor)+1];
  }
  if(isValid((cur->x_coor),(cur->y_coor)-1)){
    west = array2d[(cur->x_coor)][(cur->y_coor)-1];
  }
  if(isValid((cur->x_coor)+1,cur->y_coor)){
    south = array2d[(cur->x_coor)+1][cur->y_coor];
  }
  if(north->terrain != 'x'&& north!=NULL){
    neighbors.push_back(north);
  }
  if(east->terrain != 'x' && east!=NULL){
    neighbors.push_back(east);
  }
  if(west->terrain != 'x' && west!=NULL){
    neighbors.push_back(west);
  }
  if(south->terrain !='x' && south!=NULL){
    neighbors.push_back(south);
  }
  return neighbors;
}

/*
   A function that checks if a position 
   on the map is valid i.e. not off
   the grid.
   */
bool isValid(int x, int y){
  if((x) < 0 ||((x)) >= height){
    return false;
  }
  if((y) <0 ||(y) >= width){
    return false;
  }
  return true;
}

/*
   checks if current node* is in closedSet.
   if Node* is in list return TRUE.
   */

bool isMatch(vector<Node*>closedSet, Node* cur){
  if(closedSet.size() == 0){return false;}
  for(int i=0; i<closedSet.size(); i++){
    if(cur==closedSet[i]){
      return true;
    }
  }
  return false;
}

void prettyPrint(Node*** array2d){
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(array2d[i][j]->cameFrom != NULL){
      if(array2d[i][j]->cameFrom->x_coor <array2d[i][j]->x_coor){
        array2d[i][j]->terrain = 'v';
      }
      if(array2d[i][j]->cameFrom->x_coor > array2d[i][j]->x_coor){
        array2d[i][j]->terrain = '^';
      }
      if(array2d[i][j]->cameFrom->y_coor <array2d[i][j]->y_coor){
        array2d[i][j]->terrain = '>';
      }
      if(array2d[i][j]->cameFrom->y_coor > array2d[i][j]->y_coor){
        array2d[i][j]->terrain = '<';
      }
    }//isNull
  }//inner for loop
}//outer for loop
}//end of function




/*
   This function checks if I am reading in
   the map correctly.
   */
void print_sanity(Node*** array2d){
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      cout<<array2d[i][j]->terrain;
      //      cout<<i<<" "<<j<<" ";
    }
    cout<<'\n';
  }
  //  cout<<"hi"<<endl;
}
