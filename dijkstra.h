#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    int Neighbours[4];		//Index of neighbouring nodes, in order N,E,S,W   -1= no connection
    int Distance[4];		//Distance to neighbouring nodes, in order N,E,S,W

    //Solver variables
    int Visited;			//flag: visited or not; 1: Visited; 0: not Visited
    int DV;                 //Distance value
    int Previous;           //Previous node
    int Next;               //Next node
    int NextAbsDir;         //Absolute direction to next node (0=N, 1=E, 2=S, 3=W)
    int NextRelDir;         //Relative direction to next node (-1:no direction, 1=turn left, 2=forward, 3=turn right) // Default: 2: forward -> PLACE ROBOTS IN FORWARD DIRECTION AT START!
    int Queued;
} NodeStruct;


//other declarations
int Start;
int Finish;
int Current;
int b,c;
int k,n,i,j;
int PathLength, Node;

//Number of nodes in the Graph
int toVisit[11];

//to be used for dynamic allocation of an integer array to store the dijkstra path's  indexes
int *Path[10];

int MapSize=11;
char Graph[10]="Graph.dat";
int LowestDV=999;

//used to read the Graph.dat
FILE * fp;


/**
 * \brief Calculate path using dijkstra method.
 * \param Nodes :Array of nodes describing the map
 * \param MapSize :Number of nodes in Map
 * \param Start :Start node index for the path to be calculated
 * \param Finish :Finish node index for the path to be calculated
 * \return
 * Path length expressed in number of nodes (not including start node)
*/
int Dijkstra(NodeStruct *Nodes,int Start,int Finish);

/**
 * \brief Read Graph/Nodes and their Members' values from a data file
 * \param Nodes :Array of nodes describing the map --- Equivalent of 'Graaf' On page 58 in "Mobiele Communicatie - Labo Robotica"
 * \param MapSize :Number of nodes in Map
 * \return
*/
void ReadNodes(NodeStruct *Nodes, int MapSize);

/**
 * \brief Set Initial DV, Visited, Previous and Next values for all nodes.
 * \param Nodes :Array of nodes describing the map
 * \param MapSize :Number of nodes in Map
 * \param Start :Start node index for the path to be calculated
*/
void InitDijkstra(NodeStruct *Nodes, int MapSize, int Start);

/**
 * \brief Sorts the "toVisit[]" queue
 * \param Nodes :Array of nodes describing the map
 * \param toVisit: Array of nodes yet to visit, thus array of nodes yet to become the "Current Node"
*/
int SortQueue(NodeStruct *Nodes,int *toVisit);

/**
 * \brief Swaps one integer value with another
 * \param b: first integer
 * \param c: second integer
*/
void Swap(int *b,int *c);

/**
 * \brief Calculates the Dijkstra-defined path node indexes in an integer array //could be optimized with dynamic array allocation
 * \param Nodes :Array of nodes describing the map
 * \param MapSize :Number of nodes in Map
 * \param Start :Start node index for the path to be calculated
 * \param Finish :Finish node index for the path to be calculated
*/
int DefPath(NodeStruct *Nodes, int MapSize, int Start, int Finish);

/**
 * \brief Uses the Next/Previous nodes to find the Next Absolute and Relative Direction for the robot's movements
 * \param Nodes :Array of nodes describing the map
 * \param MapSize :Number of nodes in Map
 * \param Start :Start node index for the path to be calculated
 * \param Finish :Finish node index for the path to be calculated
*/
void DefDirs(NodeStruct *Nodes, int MapSize, int Start, int Finish);

/**
 * \brief True modulo operator (using the C _remainder_ operator)
 * \param a in a % b
 * \param b in a % b
*/
int mod(int a, int b);
#endif
