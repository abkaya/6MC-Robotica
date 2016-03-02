//========================================
//
//  Dijkstra.c | C Implementation of the Dijkstra Algorithm
//  Authors: Abdil Kaya & Robin Janssens
//
//========================================


#include "testd.h"
#define debug


int Dijkstra(NodeStruct *Map,int MapSize,int Start,int Finish)
{
    return 0;
}

void ReadMap(NodeStruct *Map, int MapSize)
{
    int i = 0;
    fp=fopen (Graph, "r");

    if(fp!=NULL)
    {
        while(fscanf(fp, "%d %d %d %d %d %d %d %d", &Map[i].Neighbours[0], &Map[i].Neighbours[1], &Map[i].Neighbours[2], &Map[i].Neighbours[3], &Map[i].Distance[0], &Map[i].Distance[1], &Map[i].Distance[2], &Map[i].Distance[3]) != EOF && i<MapSize)
        {
            #ifdef debug
            printf("%d %d %d %d %d %d %d %d\n", Map[i].Neighbours[0], Map[i].Neighbours[1], Map[i].Neighbours[2], Map[i].Neighbours[3], Map[i].Distance[0], Map[i].Distance[1], Map[i].Distance[2], Map[i].Distance[3]);
            #endif
            i++;
        }
        fclose(fp);
    }
    else
        printf("File does not exist.");

}

