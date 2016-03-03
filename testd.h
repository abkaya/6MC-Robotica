#ifndef TESTD
#define TESTD

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int Neighbours[4];		//Index of neighbouring nodes, in order N,E,S,W   -1= no connection
    int Distance[4];		//Distance to neighbouring nodes, in order N,E,S,W

    //Solver variables
    char Visited;			//flag: visited or not
    int DV;                 //Distance value
    int Previous;           //Previous node
    int Next;        //Next node
    int NextAbsDir;         //Absolute direction to next node (0=N, 1=E, 2=S, 3=W)
    int NextRelDir;         //Relative direction to next node (-1:no direction, 1=turn left, 2=forward, 3=turn right)
    int Queued;
} NodeStruct;

//linked list
struct llist
{
    int val;
    struct llist * next;
};

//other declarations
int Current;
int* b,c;
int k,n,i;
int* toVisit[9];

const int MapSize=9;
const char Graph[10]="Graph.dat";
FILE * fp;



/**
 * \brief Read Graph/Nodes and their Members' values from a data file
 * \param Nodes :Array of nodes describing the map --- Equivalent of 'Graaf' On page 58 in "Mobiele Communicatie - Labo Robotica"
 * \param MapSize :Number of nodes in Map
 * \return
*/
void ReadNodes(NodeStruct *Nodes, int MapSize);


/**
 * \brief Calculate path using dijkstra method.
 * \param Nodes :Array of nodes describing the map
 * \param MapSize :Number of nodes in Map
 * \param Start :Start node index for the path to be calculated
 * \param Finish :Finish node index for the path to be calculated
 * \return
 * Path length expressed in number of nodes (not including start node)
*/
int Dijkstra(NodeStruct *Nodes, int MapSize,int Start,int Finish);

/**
 * \brief Set Initial DV, Visited and Previous values for all nodes.
 * \param Nodes :Array of nodes describing the map
 * \param MapSize :Number of nodes in Map
 * \param Start :Start node index for the path to be calculated
*/
void InitDijkstra(NodeStruct *Nodes, int MapSize, int Start);


int SortQueue(NodeStruct *Nodes,int *toVisit);
void Swap(int *b,int *c);
#endif
